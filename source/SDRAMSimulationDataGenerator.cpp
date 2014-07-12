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

U32 SDRAMSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
  U8 cmd;
  static U64 count = 0;
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mCLK->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
    if(count == 1)
    {
      cmd = PRE; 
    }

    cmd = count & 0xFF;

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

    mCLK->Advance(1);
    mCS->Advance(1);
    mWE->Advance(1);
    mCAS->Advance(1);
    mRAS->Advance(1);
    mCLK->Transition();

    count++;
	}

  *simulation_channels = mSDRAMSimulationChannels.GetArray();
  return mSDRAMSimulationChannels.GetCount();
}

/*
void SDRAMSimulationDataGenerator::CreateClock()
{

}
*/
