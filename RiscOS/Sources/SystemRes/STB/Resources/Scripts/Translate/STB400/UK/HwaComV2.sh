#!/bin/csh
#
««Cmnt00»» HwaCom V2 ««Cmnt01»»
««Cmnt02»»
««Cmnt03»»
««Cmnt04»»
««Cmnt05»»
««Cmnt06»»
««Cmnt07»»
««Cmnt08»»
««Cmnt09»»
««Cmnt10»»
««Cmnt11»»
««Cmnt12»»
««Cmnt13»»
««Cmnt14»»
««Cmnt15»»
# '/stbboot/435hwacom2/' ««Cmnt16»»
««Cmnt17»»
««Cmnt18»»
««Cmnt19»»
#   ./HwaComV2.sh /stbboot/Releases/435 /stbboot/435hwacom2
««Cmnt20»»
««Cmnt21»» ««Version»», ««Date»»

echo Create HwaCom V2 \(Taiwan\) Baseline...

««Wipe01»»
««Wipe02»»
««Wipe03»»

««Inst00»»
««Inst01»»

««InstCore»»
««InstSTB3»»
««InstSTB4»»
««InstDebug»»
««InstFrescoV2»»
««InstEmail»»
««InstTaiwan»»
««InstRCMM»»
««InstUSB»»
#««InstHwaCom»»

««Conf00»»
««Conf01»»
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key0 \&85'   'SetEval EngMenu\$Key0 \&85'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key1 \&88'   'SetEval EngMenu\$Key1 \&88'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key2 \&90'   'SetEval EngMenu\$Key2 \&90'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key3 \&90'   'SetEval EngMenu\$Key3 \&90'
replace.sh SendMsg,feb          '\|Set SendMessage\$Protocol -u' 'Set SendMessage\$Protocol -u'
replace.sh SendMsg,feb          '\|Set SendMessage\$Quiet'       'Set SendMessage\$Quiet'
replace.sh SendMsg,feb          '\|Set SendMessage\$Host 1'      'Set SendMessage\$Host 1'
replace.sh SendMsg,feb          '\|Set SendMessage\$InStandby'   'Set SendMessage\$InStandby'
replace.sh SendMsg,feb          '\|Set SendMessage\$OutStandby'  'Set SendMessage\$OutStandby'
replace.sh Territory/Config,feb '\|Set STB\$NTPServer'           'Set STB\$NTPServer'
replace.sh Territory/Config,feb '\|Set NetTime\$DST No'          'Set NetTime\$DST No'
replace.sh Territory/Config,feb '\|Set STB\$TimeZone \"-8:0\"'   'Set STB\$TimeZone \"+8:0\"'
replace.sh Territory/Config,feb '\|Set STB\$Territory USA'       'Set STB\$Territory Taiwan'
replace.sh Territory/Config,feb '\|Set STB\$Languages en-us'     'Set STB\$Languages zh-tw,en,en-us'
replace.sh Territory/Config,feb '\|Set STB\$Encoding 2252'       'Set STB\$Encoding 2026'
replace.sh NCMail/Users         'passwd'                         'dvt01'
replace.sh CheckURL             '.pace.co.uk		-'       '.pace.co.uk		M'
««Conf02»»

««Attr00»»
««Attr01»»
««Attr02»»

««Attr03»»
