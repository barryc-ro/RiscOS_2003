#
# VCw16lib.pl - the file for Visual C++ 1.52b for windows, static libraries
#

$ssl=	"ssl16";
$crypto="crypt16";
$RSAref="RSAref16";

$o='\\';
$cp='copy';
$rm='del';

# C compiler stuff
$cc='cl';

if ($debug)
	{
	$op="/Od /Zi";
	$base_lflags="/NOLOGO /NOD /SEG:1024 /ONERROR:NOEXE /CO";
	}
else	{
	$op="/G2 /f- /Ocgnotb2";
	$base_lflags="/NOLOGO /NOD /SEG:1024 /ONERROR:NOEXE /NOE /PACKC:60000";
	$base_lflags.=" /FARCALL";
	if ($win16)
		{
		$base_lflags.=" /PACKD:60000";
		}
	}

$cflags="/ALw /Gx- /Gf $op /W3 /WX -DL_ENDIAN /nologo";
# I add the stack opt
$lflags="$base_lflags /STACK:16384";

if ($win16)
	{
	$cflags.=" -DWIN16";
	$app_cflag="/Gw";
	$lib_cflag="/Gw";
	$lib_cflag.=" -DWIN16TTY" if !$shlib;
	$lflags.=" /ALIGN:16";
	$ex_libs.="oldnames llibcewq libw";
	}
else
	{
	$no_sock=1;
	$cflags.=" -DMSDOS";
	$lflags.=" /EXEPACK";
	$ex_libs.="oldnames.lib llibce.lib";
	}

if ($shlib)
	{
	$mlflags="$base_lflags";
	$libs="oldnames ldllcew libw";
	$shlib_ex_obj="";
#	$no_asm=1;
	}
else
	{ $mlflags=''; }

$app_ex_obj="setargv.obj";

$obj='.obj';
$ofile="/Fo";

# EXE linking stuff
$link="link";
$efile="";
$exep='.exe';
$ex_libs.=$no_sock?"":" winsock";

# static library stuff
$mklib='lib';
$ranlib='';
$plib="";
$libp=".lib";
$shlibp=($shlib)?".dll":".lib";
$lfile='';

$asm='ml /Cp /c /Cx';
$afile='/Fo';
if ($no_asm)
	{
	$bn_mulw_obj='';
	$bn_mulw_src='';
	}
else
	{
	if ($asmbits == 32)
		{
		$bn_mulw_obj='crypto\bn\asm\x86w32.obj';
		$bn_mulw_src='crypto\bn\asm\x86w32.asm';
		}
	else
		{
		$bn_mulw_obj='crypto\bn\asm\x86w16.obj';
		$bn_mulw_src='crypto\bn\asm\x86w16.asm';
		}
	}

sub do_lib_rule
	{
	local($target,$name,$shlib)=@_;
	local($ret,$Name);

	$taget =~ s/\//$o/g if $o ne '/';
	($Name=$name) =~ tr/a-z/A-Z/;

	$ret.="$target: \$(${Name}OBJ)\n";
	$ret.="\t\$(RM) \$(O_$Name)\n";

	# Due to a pathetic line length limit, I unwrap the args.
	local($lib_names)="";
	local($dll_names)="  \$(SHLIB_EX_OBJ) +\n";
	foreach $_ (sort split(/\s+/,$Vars{"${Name}OBJ"}))
		{
		$lib_names.="+$_ &\n";
		$dll_names.="  $_ +\n";
		}

	if (!$shlib)
		{
		$ret.="\t\$(MKLIB) @<<\n$target\ny\n$lib_names\n\n<<\n";
		}
	else
		{
		local($ex)=($Name eq "SSL")?'$(L_CRYPTO)':"";
		$ex.=' winsock';
		$ret.="\t\$(LINK) \$(MLFLAGS) @<<\n";
		$ret.=$dll_names;
		$ret.="\n  $target\n\n  $ex $libs\nms$o${name}16.def;\n<<\n";
		($out_lib=$target) =~ s/O_/L_/;
		$ret.="\timplib /noignorecase /nowep $out_lib $target\n";
		}
	$ret.="\n";
	return($ret);
	}

sub do_link_rule
	{
	local($target,$files,$dep_libs,$libs)=@_;
	local($ret,$f,$_,@f);
	
	$file =~ s/\//$o/g if $o ne '/';
	$n=&bname($targer);
	$ret.="$target: $files $dep_libs\n";
	$ret.="  \$(LINK) \$(LFLAGS) @<<\n";
	
	# Due to a pathetic line length limit, I have to unwrap the args.
	if ($files =~ /\(([^)]*)\)$/)
		{
		@a=('$(APP_EX_OBJ)');
		push(@a,sort split(/\s+/,$Vars{$1}));
		for $_ (@a)
			{ $ret.="  $_ +\n"; }
		}
	else
		{ $ret.="  \$(APP_EX_OBJ) $files"; }
	$ret.="\n  $target\n\n  $libs\n\n<<\n\n";
	return($ret);
	}

1;
