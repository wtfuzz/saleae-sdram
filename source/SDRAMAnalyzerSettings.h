#ifndef SDRAM_ANALYZER_SETTINGS
#define SDRAM_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SDRAMAnalyzerSettings : public AnalyzerSettings
{
public:
	SDRAMAnalyzerSettings();
	virtual ~SDRAMAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

  Channel mCLKChannel;
  Channel mCSChannel;
  Channel mRASChannel;
  Channel mCASChannel;
  Channel mWEChannel;
  Channel mA10Channel;
  Channel mBA0Channel;
  Channel mBA1Channel;
	
protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mCLKChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mCSChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mRASChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mCASChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mWEChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mA10ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mBA0ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mBA1ChannelInterface;
};

#endif //SDRAM_ANALYZER_SETTINGS
