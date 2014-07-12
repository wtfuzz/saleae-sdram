#ifndef SDRAM_SIMULATION_DATA_GENERATOR
#define SDRAM_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class SDRAMAnalyzerSettings;

class SDRAMSimulationDataGenerator
{
public:
	SDRAMSimulationDataGenerator();
	~SDRAMSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, SDRAMAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	SDRAMAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

  SimulationChannelDescriptorGroup mSDRAMSimulationChannels;
  SimulationChannelDescriptor *mCLK;
  SimulationChannelDescriptor *mCS;
  SimulationChannelDescriptor *mRAS;
  SimulationChannelDescriptor *mCAS;
  SimulationChannelDescriptor *mWE;

};
#endif //SDRAM_SIMULATION_DATA_GENERATOR
