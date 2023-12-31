/*
 * inidef.h
 *
 *
 * This is where we put the defaults for all of our INI files for
 * the Internet client.  We do this to allow for the EXE to have
 * no need for INI files based on the following defaults.
 *
 */

// default INI file settings

#ifdef WINCE
// [TCP/IP]
static char szTCPIPSect[] =
 "DriverNameWin16=TDWSTCPW.DLL\0" \
 "DriverNameWin32=TDWSTCPC.DLL\0" \
 "Address=\0" \
 "ProtocolSupport=RFrame, Encrypt, Compress\0" \
 "NameEnumeratorWin16=NETCPW.DLL\0" \
 "NameEnumeratorWin32=NETCPC.DLL\0" \
 "NameResolverWin16=NRTCPW.DLL\0"
 "NameResolverWin32=NRTCPC.DLL\0" \
 "TcpBrowserAddress=\0" \
 "TcpBrowserAddress2=\0" \
 "TcpBrowserAddress3=\0" \
 "TcpBrowserAddress4=\0" \
 "TcpBrowserAddress5=\0" \
 "BrowserRetry=\0" \
 "BrowserTimeout=\0" \
 "OutBufCountHost=6\0" \
 "OutBufCountClient=6\0" \
 "OutBufLength=530\0" \
 "RFrame=On\0" \
 "Encrypt=On\0" \
 "Compress=On\0" \
 "\0\0";
#else
// [TCP/IP]
static char szTCPIPSect[] =
 "DriverNameWin16=TDWSTCPW.DLL\0" \
 "DriverNameWin32=TDWSTCPN.DLL\0" \
 "Address=\0" \
 "ProtocolSupport=RFrame, Encrypt, Compress\0" \
 "NameEnumeratorWin16=NETCPW.DLL\0" \
 "NameEnumeratorWin32=NETCPN.DLL\0" \
 "NameResolverWin16=NRTCPW.DLL\0"
 "NameResolverWin32=NRTCPN.DLL\0" \
 "TcpBrowserAddress=\0" \
 "TcpBrowserAddress2=\0" \
 "TcpBrowserAddress3=\0" \
 "TcpBrowserAddress4=\0" \
 "TcpBrowserAddress5=\0" \
 "BrowserRetry=\0" \
 "BrowserTimeout=\0" \
 "OutBufCountHost=6\0" \
 "OutBufCountClient=6\0" \
 "OutBufLength=530\0" \
 "RFrame=On\0" \
 "Encrypt=On\0" \
 "Compress=Off\0" \
 "UseAlternateAddress=\0" \
 "\0\0";
#endif

//[ICA 3.0]
// "DriverName=WDICA30.DDL\0"
#ifdef WINCE
static char szICA30Sect[] =
 "DriverNameWin16=WDICA30W.DLL\0" \
 "DriverNameWin32=WDICA30C.DLL\0" \
 "ProtocolSupport=Modem, RFrame, Frame, Reliable, Encrypt, Compress\0" \
 "VirtualDriver=Thinwire3.0, ClientDrive, ClientComm, ClientPrinter, ClientAudio\0" \
 "BufferLength=2048\0" \
 "XmsReserve=0\0" \
 "LowMemReserve=51200\0" \
 "ConnectTTY=On\0" \
 "ConnectTTYDelay=1000\0" \
 "ReducerWinCE=ICAREDUC.DLL\0" \
"\0\0";
#else
static char szICA30Sect[] =
 "DriverNameWin16=WDICA30W.DLL\0" \
 "DriverNameWin32=WDICA30N.DLL\0" \
 "ProtocolSupport=Modem, RFrame, Frame, Reliable, Encrypt, Compress\0" \
 "VirtualDriver=Thinwire3.0, ClientDrive, ClientPrinter, Clipboard, LicenseHandler\0" \
 "BufferLength=2048\0" \
 "XmsReserve=0\0" \
 "LowMemReserve=51200\0" \
 "ConnectTTY=On\0" \
 "ConnectTTYDelay=1000\0" \
 "\0\0";
#endif

//[RFrame]
static char szRFrameSect[] =
// "DriverName=PDRFRAM.DDL\0"
#ifdef WINCE
 "DriverNameWin16=PDRFRAMW.DLL\0" \
 "DriverNameWin32=PDRFRAMC.DLL\0" \
"\0\0";
#else
 "DriverNameWin16=PDRFRAMW.DLL\0" \
 "DriverNameWin32=PDRFRAMN.DLL\0" \
"\0\0";
#endif

//[Encrypt]
static char szEncryptSect[] =
// "DriverName=PDCRYPT.DDL\0"
#ifdef WINCE
 "DriverNameWin16=PDCRYPTW.DLL\0" \
 "DriverNameWin32=PDCRYPTC.DLL\0" \
"\0\0";
#else
 "DriverNameWin16=PDCRYPTW.DLL\0" \
 "DriverNameWin32=PDCRYPTN.DLL\0" \
"\0\0";
#endif

 //[EncryptionLevelAuth]
static char szEncryptionLevelAuthSect[] =
 "Basic=Encrypt\0" \
"\0\0";

 //[EncryptionLevelSession]
static char szEncryptionLevelSessionSect[] =
 "Basic=Encrypt\0" \
"\0\0";

 //[Encryption Levels]
static char szEncryptionLevelsSect[] =
 "Basic=Encrypt\0" \
"\0\0";

//[Compress]
static char szCompressSect[] =
// "DriverName=PDCOMP.DDL\0"
#ifdef WINCE
 "DriverNameWin16=PDCOMPW.DLL\0" \
 "DriverNameWin32=PDCOMPC.DLL\0" \
"\0\0";
#else
 "DriverNameWin16=PDCOMPW.DLL\0" \
 "DriverNameWin32=PDCOMPN.DLL\0" \
"\0\0";
#endif

 //[Frame]
static char szFrameSect[] =
// "DriverName=PDFRAME.DDL\0"
#ifdef WINCE
 "DriverNameWin16=PDFRAMEW.DLL\0" \
 "DriverNameWin32=PDFRAMEC.DLL\0" \
"\0\0";
#else
 "DriverNameWin16=PDFRAMEW.DLL\0" \
 "DriverNameWin32=PDFRAMEN.DLL\0" \
"\0\0";
#endif

//[Reliable]
static char szReliableSect[] =
// "DriverName=PDRELI.DDL\0"
#ifdef WINCE
 "DriverNameWin16=PDRELIW.DLL\0" \
 "DriverNameWin32=PDRELIC.DLL\0" \
"\0\0";
#else
 "DriverNameWin16=PDRELIW.DLL\0" \
 "DriverNameWin32=PDRELIN.DLL\0" \
"\0\0";
#endif

//[Modem]
static char szTapiModemSect[] = 
//  "DriverName=PDTAPI.DLL\0"  
#ifdef WINCE
 "DriverNameWin16=PDTAPIW.DLL\0" \
 "DriverNameWin32=PDTAPIC.DLL\0" \
"\0\0";
#else
 "DriverNameWin16=PDTAPIW.DLL\0" \
 "DriverNameWin32=PDTAPIN.DLL\0" \
"\0\0";
#endif

//[Standard COM Port]
static char szStandardCOMPort[] = 
//  "DriverName=TDCOMM\0"  
#ifdef WINCE
	"DriverNameWin16=TDCOMMW.DLL\0" \
	"DriverNameWin32=TDCOMMC.DLL\0" \
	"ProtocolSupport=Modem, Frame, Reliable, Encrypt, Compress\0" \
 "Encrypt=On\0" \
 "Reliable=On\0" \
 "Frame=On\0" \
"\0\0";
#else
	"DriverNameWin16=TDCOMMW.DLL\0" \
	"DriverNameWin32=TDCOMMN.DLL\0" \
	"ProtocolSupport=Modem, Frame, Reliable, Encrypt, Compress\0" \
 "Reliable=On\0" \
 "Frame=On\0" \
 "Encrypt=On\0" \
"\0\0";
#endif

//[Thinwire3.0]
static char szThinwire30Sect[] =
// "DriverName=VDTW30.DDL\0"
#ifdef WINCE
 "DriverNameWin16=VDTW30W.DLL\0"
 "DriverNameWin32=VDTW30C.DLL\0" \
 "MinSpecialCache16Color=8\0" \
 "MaxSpecialCache16Color=32\0" \
 "MinMemoryCache=750\0" \
 "MaxMemoryCache=8192\0" \
 "MaxCache16Color=8192\0" \
 "MaxDiskCache=2048\0" \
 "MinDiskLeft=2048\0" \
 "DiskCacheDirectory=\0" \
 "SVGACapability=Off\0" \
 "SVGAPreference=Off\0" \
 "DesiredHRES=640\0" \
 "DesiredVRES=480\0" \
 "WindowsCache=3072\0" \
 "ClickTicks=5\0" \
 "DesiredColor=0x0001\0" \
 "ScreenPercent=0" \
"\0\0";
#else
 "DriverNameWin16=VDTW30W.DLL\0"
 "DriverNameWin32=VDTW30N.DLL\0" \
 "MinSpecialCache16Color=8\0" \
 "MaxSpecialCache16Color=32\0" \
 "MinMemoryCache=750\0" \
 "MaxMemoryCache=8192\0" \
 "MaxCache16Color=8192\0" \
 "MaxDiskCache=2048\0" \
 "MinDiskLeft=2048\0" \
 "DiskCacheDirectory=\0" \
 "SVGACapability=Off\0" \
 "SVGAPreference=Off\0" \
 "DesiredHRES=640\0" \
 "DesiredVRES=480\0" \
 "WindowsCache=3072\0" \
 "ClickTicks=5\0" \
 "DesiredColor=0x0001\0" \
 "ScreenPercent=0" \
"\0\0";
#endif

//[ClientAudio]
#ifdef WINCE		// Does Internet client do audio
static char szClientAudioSect[] =
//  "DriverName=VDCPM30.DDL\0"
  "DriverNameWin16=VDCAMW.DLL\0" \
  "DriverNameWin32=VDCAMC.DLL\0" \
  "AudioHWSection=AudioConverters\0" \
  "NumCommandBuffers=64\0" \
  "NumDataBuffers=32\0" \
  "MaxDataBufferSize=2048\0" \
  "CommandAckThresh=10\0" \
  "DataAckThresh=10\0" \
  "AckDelayThresh=350\0" \
  "PlaybackDelayThresh= 250\0" \
"\0\0";

//[AudioConverters]
static char szClientAudioConverters[] =
//  "DriverName=Unsupported\0"
  "DriverNameWin16=AUDCVTW.DLL\0" \
  "DriverNameWin32=AUDCVTC.DLL\0" \
  "AudioHWSection=AudioHardware\0" \
  "ConverterSection=AudioConverterList\0" \
"\0\0";

//[AudioHardware]
static char szClientAudioHardware[] =
//  "DriverName=Unsupported\0"
  "DriverNameWin16=AUDHALW.DLL\0" \
  "DriverNameWin32=AUDHALC.DLL\0" \
"\0\0";

//[AudioConverterList]
static char szClientAudioConverterList[] =
  "NumConverters=1\0" \
  "Converter0=ConverterADPCM\0" \
"\0\0";

//[ConverterADPCM]
static char szClientConverterADPCM[] =
//  "DriverName=Unsupported\0"
  "DriverNameWin16=ADPCM.DLL\0" \
  "DriverNameWin32=ADPCMC.DLL\0" \
"\0\0";

#endif	/* WINCE */

//[ClientDrive]
static char szClientDriveSect[] =
//  "DriverName  =VDCDM30.DDL\0"
#ifdef WINCE
  "DriverNameWin16=VDCDM30W.DLL\0" \
  "DriverNameWin32=VDCDM30C.DLL\0" \
  "MaxWindowSize=6276\0" \
  "MaxRequestSize=1046\0" \
  "CacheTimeout=600\0" \
  "CacheTimeoutHigh=0\0" \
  "CacheTransferSize=0\0" \
  "CacheDisable=FALSE\0" \
  "CacheWriteAllocateDisable=FALSE\0" \
"\0\0";
#else
  "DriverNameWin16=VDCDM30W.DLL\0" \
  "DriverNameWin32=VDCDM30N.DLL\0" \
  "MaxWindowSize=6276\0" \
  "MaxRequestSize=1046\0" \
  "CacheTimeout=600\0" \
  "CacheTimeoutHigh=0\0" \
  "CacheTransferSize=0\0" \
  "CacheDisable=FALSE\0" \
  "CacheWriteAllocateDisable=FALSE\0" \
"\0\0";
#endif

//[ClientPrinter]
static char szClientPrinterSect[] =
//  "DriverName=VDCPM30.DDL\0"
#ifdef WINCE
  "DriverNameWin16=VDSPL30W.DLL\0" \
  "DriverNameWin32=VDCPM30C.DLL\0" \
  "WindowSize=1024\0" \
  "MaxWindowSize=2048\0" \
"\0\0";
#else
  "DriverNameWin16=VDSPL30W.DLL\0" \
  "DriverNameWin32=VDSPL30N.DLL\0" \
  "WindowSize=1024\0" \
  "MaxWindowSize=2048\0" \
"\0\0";
#endif

//[ClientComm]
static char szClientCommSect[] =
//  "DriverName=VDCOM30.DDL\0"
#ifdef WINCE
  "DriverNameWin16=VDCOM30W.DLL\0" \
  "DriverNameWin32=VDCOM30C.DLL\0" \
  "WindowSize=1024\0" \
"\0\0";
#else
  "DriverNameWin16=VDCOM30W.DLL\0" \
  "DriverNameWin32=VDCOM30N.DLL\0" \
  "WindowSize=1024\0" \
"\0\0";
#endif

//[Clipboard]
static char szClipboardSect[] =
//  "DriverName=Unsupported\0"
#ifdef WINCE
  "DriverNameWin16=VDCLIPW.DLL\0" \
  "DriverNameWin32=VDCLIPC.DLL\0" \
"\0\0";
#else
  "DriverNameWin16=VDCLIPW.DLL\0" \
  "DriverNameWin32=VDCLIPN.DLL\0" \
"\0\0";
#endif

//[LicenseHandler]
static char szLicenseHandlerSect[] =
//  "DriverName=Unsupported\0"
  "DriverNameWin16=VDLICW.DLL\0" \
  "DriverNameWin32=VDLICN.DLL\0" \
"\0\0";


//[Hotkey Shift States]
//char szHotkeyShiftSect[] =
//"(none)=0\0"
//"Shift=3\0"
//"Ctrl=4\0"
//"Alt=8\0"
//"\0\0";

#ifdef WINCE
//[Hotkey Shift States]
static char szHotkeyShiftSect[] =
  "(none)=0\0" \
  "Shift=3\0" \
  "Ctrl=4\0" \
  "Alt=8\0" \
  "\0\0";
#endif

//[Hotkey Keys]
//char szHotkeyKeysSect[] =
//"(none)=0\0"
//"F1=59\0"
//"F2=60\0"
//"F3=61\0"
//"F4=62\0"
//"F5=63\0"
//"F6=64\0"
//"F7=65\0"
//"F8=66\0"
//"F9=67\0"
//"F10=68\0"
//"F11=87\0"
//"F12=88\0"
//"Esc=1\0"
//"minus=74\0"
//"plus=78\0"
//"star=55\0"
//"tab=15\0"
//"\0\0";

#ifdef WINCE
static char szHotkeyKeysSect[] =
  "0=11\0" \
  "1=2\0" \
  "2=3\0" \
  "3=4\0" \
  "4=5\0" \
  "5=6\0" \
  "6=7\0" \
  "7=8\0" \
  "8=9\0" \
  "9=10\0" \
  "\0\0";
#endif


//[KeyboardLayout]
//char szKeyboardLayoutSect[] =
//"(User Profile)=0x00000000\0"
//"Belgian Dutch=0x00000813\0"
//"Belgian French=0x0000080C\0"
//"Brazilian (ABNT)=0x00000416\0"
//"British=0x00000809\0"
//"Bulgarian=0x00000402\0"
//"Canadian English (Multilingual)=0x00001009\0"
//"Canadian French=0x00000C0C\0"
//"Canadian French (Multilingual)=0x00010C0C\0"
//"Croatian=0x0000041A\0"
//"Czech=0x00000405\0"
//"Danish=0x00000406\0"
//"Dutch=0x00000413\0"
//"Finnish=0x0000040B\0"
//"French=0x0000040C\0"
//"German=0x00000407\0"
//"Greek=0x00000408\0"
//"Hungarian=0x0000040E\0"
//"Icelandic=0x0000040F\0"
//"Italian=0x00000410\0"
//"Italian (142)=0x00010410\0"
//"Latin American=0x0000080A\0"
//"Norwegian=0x00000414\0"
//"Polish (Programmers)=0x00000415\0"
//"Portuguese=0x00000816\0"
//"Romanian=0x00000418\0"
//"Russian=0x00000419\0"
//"Slovak=0x0000041B\0"
//"Slovenian=0x00000424\0"
//"Spanish=0x0000040A\0"
//"Spanish variation=0x0001040A\0"
//"Swedish=0x0000041D\0"
//"Swiss French=0x0000100C\0"
//"Swiss German=0x00000807\0"
//"Turkish (F)=0x0001041F\0"
//"Turkish (Q)=0x0000041F\0"
//"US=0x00000409\0"
//"US-Dvorak=0x00010409\0"
//"US-International=0x00020409\0"
//"\0\0";

//[Keyboard Dll Codes]
static char szKBDllSect[] =
   "kbdbe.dll=0x0000080C\0"
   "kbdbr.dll=0x00000416\0"
   "kbduk.dll=0x00000809\0"
   "kbdfc.dll=0x00001009\0"
   "kbdda.dll=0x00000406\0"
   "kbdne.dll=0x00000413\0"
   "kbdfi.dll=0x0000040B\0"
   "kbdfr.dll=0x0000040C\0"
   "kbdca.dll=0x00010C0C\0"
   "kbdgr.dll=0x00000407\0"
   "kbdic.dll=0x0000040F\0"
   "kbdit.dll=0x00000410\0"
   "kbdla.dll=0x0000080A\0"
   "kbdno.dll=0x00000414\0"
   "kbdpo.dll=0x00000816\0"
   "kbdsp.dll=0x0000040A\0"
   "kbdsw.dll=0x0000041D\0"
   "kbdsf.dll=0x0000100C\0"
   "kbdsg.dll=0x00000807\0"
   "kbdus.dll=0x00000409\0"
   "kbddv.dll=0x00010409\0"
   "kbdusx.dll=0x00020409\0"
"\0\0";

static DEFINISECT ModuleSect[] = {
    {"TCP/IP",szTCPIPSect},
    {"ICA 3.0",szICA30Sect},
    {"RFrame",szRFrameSect},
    {"Encrypt",szEncryptSect},
    {"EncryptionLevelSession",szEncryptionLevelSessionSect},
    {"EncryptionLevelAuth",szEncryptionLevelAuthSect},
    {"Encryption Levels",szEncryptionLevelsSect},
    {"Thinwire3.0",szThinwire30Sect},
    {"ClientDrive",szClientDriveSect},
    {"Frame",szFrameSect},
    {"Compress",szCompressSect},
    {"Reliable",szReliableSect},
    {"TAPIModem",szTapiModemSect},
	{"Standard COM Port",szStandardCOMPort},
#ifdef WINCE		// Does internet client support audio
	{"ClientAudio",szClientAudioSect},
	{"AudioConverters",szClientAudioConverters},
	{"AudioHardware",szClientAudioHardware},
	{"AudioConverterList",szClientAudioConverterList},
	{"ConverterADPCM",szClientConverterADPCM},
#endif /* WINCE */
    {"ClientComm",szClientCommSect},
    {"ClientPrinter",szClientPrinterSect},
#ifndef WINCE
    {"Clipboard",szClipboardSect},
    {"LicenseHandler",szLicenseHandlerSect},
#endif
    {"Keyboard Dll Codes",szKBDllSect},
#ifdef WINCE
   {"Hotkey Shift States",szHotkeyShiftSect},
    {"Hotkey Keys",szHotkeyKeysSect},
#endif
//    {"KeyboardLayout",szKeyboardLayoutSect},
    {NULL,NULL}
    };

//[WFClient] Wfclient.INI
static char szWfclientSect[] =
"Version=2\0" \
"ServerToClientBuffPowerOf2=15\0" \
"ClientToServerBuffPowerOf2=13\0" \
"\0\0";

//[Thinwire3.0] Wfclient.INI
static char szWFThinWireSect[] =
"SVGACapability=Off\0" \
"SVGAPreference=Off\0" \
"DesiredHRES=640\0" \
"DesiredVRES=480\0" \
"DesiredColor=0x0002\0" \
"ScreenPercent=0" \
"UseSafeArea=Off\0" \
"\0\0";


// default sections for WFCLIENT.INI
static DEFINISECT WfclientSect[] = {
    {"Thinwire3.0"/*,szWFThinWireSect*/},
    {"WFClient"/*,szWfclientSect*/},
    {NULL,NULL}
    };

// default ini files for WFCLIENT.INI and MODULE.INI
DEFINIFILE DefIniSect[] = {
    {"module"/*, ModuleSect*/},
    {"wfclient"/*, WfclientSect*/},
    {NULL,NULL}
    };
