#include "SDRAMAnalyzer.h"
#include "SDRAMAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

SDRAMAnalyzer::SDRAMAnalyzer()
:	Analyzer(),  
	mSettings( new SDRAMAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

SDRAMAnalyzer::~SDRAMAnalyzer()
{
	KillThread();
}

void SDRAMAnalyzer::WorkerThread()
{
	mResults.reset( new SDRAMAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mCLKChannel );

	mSampleRateHz = GetSampleRate();

	mCLK = GetAnalyzerChannelData( mSettings->mCLKChannel );
	mCS = GetAnalyzerChannelData( mSettings->mCSChannel );
	mRAS = GetAnalyzerChannelData( mSettings->mRASChannel );
	mCAS = GetAnalyzerChannelData( mSettings->mCASChannel );
	mWE = GetAnalyzerChannelData( mSettings->mWEChannel );

	if( mCLK->GetBitState() == BIT_LOW )
		mCLK->AdvanceToNextEdge();

  while(1)
  {
    U64 start_edge;
    U64 last_transition;
    U64 fall;

    int changed = 0;
    
    Frame frame;

    U8 bits;

	  if( mCLK->GetBitState() == BIT_LOW )
  		mCLK->AdvanceToNextEdge();

    start_edge = mCLK->GetSampleNumber();
    mRAS->AdvanceToAbsPosition(start_edge);
    mCAS->AdvanceToAbsPosition(start_edge);
    mWE->AdvanceToAbsPosition(start_edge);
    mCS->AdvanceToAbsPosition(start_edge);

    bits = 0;
    if(mRAS->GetBitState() == BIT_HIGH)
      bits |= 1<<2;
    if(mCAS->GetBitState() == BIT_HIGH)
      bits |= 1<<1;
    if(mWE->GetBitState() == BIT_HIGH)
      bits |= 1;

    if(mCS->GetBitState() == BIT_HIGH)
    {
      if(frame.mData1 != NOP)
        changed = 1;
      frame.mData1 = NOP;
    }
    else
    {
      if(frame.mData1 != bits)
        changed = 1;
      frame.mData1 = bits;
    }

 		mCLK->AdvanceToNextEdge();
    mCLK->AdvanceToNextEdge();

    if(changed)
    {
  	  frame.mFlags = 0;
  	  frame.mStartingSampleInclusive = start_edge;
  	  frame.mEndingSampleInclusive = mCLK->GetSampleNumber();

      mResults->AddFrame( frame );
      mResults->CommitResults();
      ReportProgress( frame.mEndingSampleInclusive );

      last_transition = frame.mEndingSampleInclusive;
    }

  }
}

bool SDRAMAnalyzer::NeedsRerun()
{
	return false;
}

U32 SDRAMAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 SDRAMAnalyzer::GetMinimumSampleRateHz()
{
  return 24000000;
	//return mSettings->mBitRate * 4;
}

const char* SDRAMAnalyzer::GetAnalyzerName() const
{
	return "SDRAM Command Analyzer";
}

const char* GetAnalyzerName()
{
	return "SDRAM Command Analyzer";
}

Analyzer* CreateAnalyzer()
{
	return new SDRAMAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
