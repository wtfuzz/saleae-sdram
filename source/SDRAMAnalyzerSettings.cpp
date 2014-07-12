#include "SDRAMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


SDRAMAnalyzerSettings::SDRAMAnalyzerSettings()
:	mCLKChannel( UNDEFINED_CHANNEL ),
  mCSChannel( UNDEFINED_CHANNEL ),
  mRASChannel( UNDEFINED_CHANNEL ),
  mCASChannel( UNDEFINED_CHANNEL ),
  mWEChannel( UNDEFINED_CHANNEL ),
  mA10Channel( UNDEFINED_CHANNEL ),
  mBA0Channel( UNDEFINED_CHANNEL ),
  mBA1Channel( UNDEFINED_CHANNEL )
{
	mCLKChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mCLKChannelInterface->SetTitleAndTooltip( "CLK", "CLK Channel" );
	mCLKChannelInterface->SetChannel( mCLKChannel );

	mCSChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mCSChannelInterface->SetTitleAndTooltip( "/CS", "/CS Channel" );
	mCSChannelInterface->SetChannel( mCSChannel );

	mRASChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mRASChannelInterface->SetTitleAndTooltip( "/RAS", "/RAS Channel" );
	mRASChannelInterface->SetChannel( mRASChannel );

	mCASChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mCASChannelInterface->SetTitleAndTooltip( "/CAS", "/CAS Channel" );
	mCASChannelInterface->SetChannel( mCASChannel );

	mWEChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mWEChannelInterface->SetTitleAndTooltip( "/WE", "/WE Channel" );
	mWEChannelInterface->SetChannel( mWEChannel );

	AddInterface( mCLKChannelInterface.get() );
	AddInterface( mCSChannelInterface.get() );
	AddInterface( mRASChannelInterface.get() );
	AddInterface( mCASChannelInterface.get() );
	AddInterface( mWEChannelInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mCLKChannel, "CLK", false );
	AddChannel( mCSChannel, "/CS", false );
	AddChannel( mRASChannel, "/RAS", false );
	AddChannel( mCASChannel, "/CAS", false );
	AddChannel( mWEChannel, "/WE", false );
}

SDRAMAnalyzerSettings::~SDRAMAnalyzerSettings()
{
}

bool SDRAMAnalyzerSettings::SetSettingsFromInterfaces()
{
	mCLKChannel = mCLKChannelInterface->GetChannel();
	mCSChannel = mCSChannelInterface->GetChannel();
	mRASChannel = mRASChannelInterface->GetChannel();
	mCASChannel = mCASChannelInterface->GetChannel();
	mWEChannel = mWEChannelInterface->GetChannel();

	ClearChannels();
	AddChannel( mCLKChannel, "CLK", true );
	AddChannel( mCSChannel, "/CS", true );
	AddChannel( mRASChannel, "/RAS", true );
	AddChannel( mCASChannel, "/CAS", true );
	AddChannel( mWEChannel, "/WE", true );

	return true;
}

void SDRAMAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mCLKChannelInterface->SetChannel( mCLKChannel );
	mCSChannelInterface->SetChannel( mCSChannel );
	mRASChannelInterface->SetChannel( mRASChannel );
	mCASChannelInterface->SetChannel( mCASChannel );
	mWEChannelInterface->SetChannel( mWEChannel );
}

void SDRAMAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mCLKChannel;
	text_archive >> mCSChannel;
	text_archive >> mRASChannel;
	text_archive >> mCASChannel;
	text_archive >> mWEChannel;

	ClearChannels();
	AddChannel( mCLKChannel, "CLK", true );
	AddChannel( mCSChannel, "/CS", true );
	AddChannel( mRASChannel, "/RAS", true );
	AddChannel( mCASChannel, "/CAS", true );
	AddChannel( mWEChannel, "/WE", true );

	UpdateInterfacesFromSettings();
}

const char* SDRAMAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mCLKChannel;
	text_archive << mCSChannel;
	text_archive << mRASChannel;
	text_archive << mCASChannel;
	text_archive << mWEChannel;

	return SetReturnString( text_archive.GetString() );
}
