#!/bin/csh
#
��Cmnt00�� AC/3 Test ��Cmnt01��
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
# '/stbboot/435ac3/' ��Cmnt16��
��Cmnt17��
��Cmnt18��
��Cmnt19��
#   ./AC3.sh /stbboot/Releases/435 /stbboot/435ac3
��Cmnt20��
��Cmnt21�� ��Version��, ��Date��

echo Create AC3 V2 \(USA\) Baseline...

��Wipe01��
��Wipe02��
��Wipe03��

��Inst00��
��Inst01��

��InstCore��
��InstSTB4��
��InstDebug��
��InstFrescoV2��
��InstAC3��

��Conf00��
��Conf01��
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key0 \&85' 'SetEval EngMenu\$Key0 \&00'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key1 \&88' 'SetEval EngMenu\$Key1 \&00'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key2 \&90' 'SetEval EngMenu\$Key2 \&00'
replace.sh EngMenu,feb          '\|SetEval EngMenu\$Key3 \&90' 'SetEval EngMenu\$Key3 \&00'
replace.sh Territory/Config,feb '\|Set STB\$NTPServer'         'Set STB\$NTPServer'
replace.sh Territory/Config,feb '\|Set NetTime\$DST No'        'Set NetTime\$DST No'
replace.sh Territory/Config,feb '\|Set STB\$Territory USA'     'Set STB\$Territory USA'
replace.sh Territory/Config,feb '\|Set STB\$Languages en-us'   'Set STB\$Languages en-us,en,en-gb'
replace.sh Territory/Config,feb '\|Set STB\$Encoding 2252'     'Set STB\$Encoding 2252'
replace.sh CheckURL             '.pace.co.uk		-'     '.pace.co.uk		M'
��Conf02��

��Attr00��
��Attr01��
��Attr02��

��Attr03��
