#!/bin/csh
#
��Cmnt00�� Japan V2 ��Cmnt01��
��Cmnt02��
��Cmnt03��
��Cmnt04��
��Cmnt05��
��Cmnt06��
��Cmnt07��
��Cmnt08��
��Cmnt09��
��Cmnt10��
��Cmnt11��
��Cmnt12��
��Cmnt13��
��Cmnt14��
��Cmnt15��
# '/stbboot/435Japan/' ��Cmnt16��
��Cmnt17��
��Cmnt18��
��Cmnt19��
#   ./Japan.sh /stbboot/Releases/435 /stbboot/435Japan
��Cmnt20��
��Cmnt21�� ��Version��, ��Date��

echo Create Japan V2 \(Japanese\) Baseline...

��Wipe01��
��Wipe02��
��Wipe03��

��Inst00��
��Inst01��

��InstCore��
��InstSTB4��
��InstDebug��
��InstFrescoV2��
��InstSSLv2s��
��InstSSLv2sx��
��InstJapan��

��Conf00��
��Conf01��
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key0 \&85'     'SetEval EngMenu\$Key0 \&00'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key1 \&88'     'SetEval EngMenu\$Key1 \&00'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key2 \&90'     'SetEval EngMenu\$Key2 \&00'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key3 \&90'     'SetEval EngMenu\$Key3 \&00'
replace.sh Territory/Config,feb '\|Set STB\$NTPServer'             'Set STB\$NTPServer'
replace.sh Territory/Config,feb '\|Set NetTime\$DST No'            'Set NetTime\$DST No'
replace.sh Territory/Config,feb '\|Set STB\$TimeZone \"-8:0\"'     'Set STB\$TimeZone \"+9:0\"'
replace.sh Territory/Config,feb '\|Set STB\$Territory USA'         'Set STB\$Territory Japan'
replace.sh Territory/Config,feb '\|Set STB\$Languages en-us'       'Set STB\$Languages ja,en,en-us'
replace.sh Territory/Config,feb '\|Set STB\$Encoding 2252'         'Set STB\$Encoding 4004'
replace.sh NCFresco,feb         '\|Set NCFresco\$SSL \"HTTPsMod\"' 'Set NCFresco\$SSL \"HTTPsMod\"'
replace.sh CheckURL             '.pace.co.uk		-'         '.pace.co.uk		M'
��Conf02��

��Attr00��
��Attr01��
��Attr02��

��Attr03��
