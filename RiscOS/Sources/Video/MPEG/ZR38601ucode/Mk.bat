@echo off
echo Assemble source files...
asm601 -list -asm -hex Pacemain

echo Link objects...
link601 -map -code=0xD0000,0x800 -uninitdata=0x2400,0x20 -initdata=0x2420,0x200 -executable=PaceMain PaceMain

echo Generate boot file...
exzutil -oh -r -eD0000 Pacemain.exz Pacemain.hex
sed 1d Pacemain.hex > tmp.zbn
hex8bin tmp.zbn ucode.bin
del tmp.zbn

echo Created Pacemain.zbn
