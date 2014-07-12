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
    U64 fall;
    
    Frame frame;

    U8 bits;

	  if( mCLK->GetBitState() == BIT_LOW )
  		mCLK->AdvanceToNextEdge();

    start_edge = mCLK->GetSampleNumber();
 		mCLK->AdvanceToNextEdge();

    fall = mCLK->GetSampleNumber();
    mRAS->AdvanceToAbsPosition(fall);
    mCAS->AdvanceToAbsPosition(fall);
    mWE->AdvanceToAbsPosition(fall);
    mCS->AdvanceToAbsPosition(fall);

    bits = 0;
    if(mRAS->GetBitState() == BIT_HIGH)
      bits |= 1<<2;
    if(mCAS->GetBitState() == BIT_HIGH)
      bits |= 1<<1;
    if(mWE->GetBitState() == BIT_HIGH)
      bits |= 1;

    if(mCS->GetBitState() == BIT_HIGH)
    {
      frame.mData1 = NOP;
    }
    else
    {
      frame.mData1 = bits;
    }

 		mCLK->AdvanceToNextEdge();

  	frame.mFlags = 0;
  	frame.mStartingSampleInclusive = start_edge;
  	frame.mEndingSampleInclusive = mCLK->GetSampleNumber();

  	mResults->AddFrame( frame );
  	mResults->CommitResults();
  	ReportProgress( frame.mEndingSampleInclusive );
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
