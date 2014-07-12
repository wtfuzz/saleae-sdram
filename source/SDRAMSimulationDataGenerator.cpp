#include "SDRAMSimulationDataGenerator.h"
#include "SDRAMAnalyzerSettings.h"
#include "SDRAMCommands.h"

#include <AnalyzerHelpers.h>

SDRAMSimulationDataGenerator::SDRAMSimulationDataGenerator()
:	mSerialText( "SDRAM Command Analyzer" ),
	mStringIndex( 0 )
{
}

SDRAMSimulationDataGenerator::~SDRAMSimulationDataGenerator()
{
}

void SDRAMSimulationDataGenerator::Initialize( U32 simulation_sample_rate, SDRAMAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

  mCLK = mSDRAMSimulationChannels.Add(settings->mCLKChannel, mSimulationSampleRateHz, BIT_HIGH);
  mCS = mSDRAMSimulationChannels.Add(settings->mCSChannel, mSimulationSampleRateHz, BIT_HIGH);
  mRAS = mSDRAMSimulationChannels.Add(settings->mRASChannel, mSimulationSampleRateHz, BIT_HIGH);
  mCAS = mSDRAMSimulationChannels.Add(settings->mCASChannel, mSimulationSampleRateHz, BIT_HIGH);
  mWE = mSDRAMSimulationChannels.Add(settings->mWEChannel, mSimulationSampleRateHz, BIT_HIGH);

}

enum {
  STATE_INIT_START,
  STATE_INIT_PRE,
  STATE_INIT_REFRESH,
  STATE_INIT_MODE
} states;

enum {
  tRP = 3,
  tRC = 10,
  tMRD = 2
} latency;

U32 SDRAMSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
  U8 cmd;
  U64 count = 0;
  U64 wait = 0;
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

  U8 state = STATE_INIT_START;
  U8 refresh_count = 8;

	while( mCLK->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{

    if(mCLK->GetCurrentBitState() == BIT_HIGH)
    {
      if(count == wait)
      {
        // Wait state expired

        switch(state)
        {
          case STATE_INIT_START:
            refresh_count = 8;
            state = STATE_INIT_PRE;
            cmd = PRE;
            wait = count + tRP;
            break;
          case STATE_INIT_PRE:
            state = STATE_INIT_REFRESH;
            cmd = REF;
            wait = count + tRC;
            break;
          case STATE_INIT_REFRESH:
            if(refresh_count == 0)
            {
              state = STATE_INIT_MODE;
              cmd = MRS;
              wait = count + tMRD;
            }
            else
            {
              refresh_count--;
              cmd = REF;
              wait = count + tRC;
            }
            break;
          case STATE_INIT_MODE:
            state = STATE_INIT_START;
            wait = count + 2;
            break;
        } 
      }
      else
      {
        cmd = NOP;
      }

      if(cmd == NOP)
      {
        mCS->TransitionIfNeeded(BIT_HIGH); 
      }
      else
      {
        mCS->TransitionIfNeeded(BIT_LOW);

        mWE->TransitionIfNeeded(cmd & 1 ? BIT_HIGH : BIT_LOW);
        mCAS->TransitionIfNeeded(cmd & 2 ? BIT_HIGH : BIT_LOW);
        mRAS->TransitionIfNeeded(cmd & 4 ? BIT_HIGH : BIT_LOW);
      }
    }

    // Increment count on each rising edge
    if(mCLK->GetCurrentBitState() == BIT_HIGH)
      count++;

    mCLK->Advance(1);
    mCS->Advance(1);
    mWE->Advance(1);
    mCAS->Advance(1);
    mRAS->Advance(1);
    mCLK->Transition();
  
	}

  *simulation_channels = mSDRAMSimulationChannels.GetArray();
  return mSDRAMSimulationChannels.GetCount();
}

/*
void SDRAMSimulationDataGenerator::CreateClock()
{

}
*/
