#ifndef SDRAM_ANALYZER_H
#define SDRAM_ANALYZER_H

#include <Analyzer.h>
#include "SDRAMAnalyzerResults.h"
#include "SDRAMSimulationDataGenerator.h"

#include "SDRAMCommands.h"

class SDRAMAnalyzerSettings;
class ANALYZER_EXPORT SDRAMAnalyzer : public Analyzer
{
public:
	SDRAMAnalyzer();
	virtual ~SDRAMAnalyzer();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< SDRAMAnalyzerSettings > mSettings;
	std::auto_ptr< SDRAMAnalyzerResults > mResults;
	AnalyzerChannelData *mCLK;
  AnalyzerChannelData *mCS;
  AnalyzerChannelData *mRAS;
  AnalyzerChannelData *mCAS;
  AnalyzerChannelData *mWE;

	SDRAMSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //SDRAM_ANALYZER_H
