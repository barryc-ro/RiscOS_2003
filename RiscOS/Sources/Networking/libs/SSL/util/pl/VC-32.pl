#
# VCw32lib.pl - the file for Visual C++ 4.[01] for windows NT, static libraries
#

$ssl=	"ssl32";
$crypto="crypt32";
$RSAref="RSAref32";

$o='\\';
$cp='copy';
$rm='del';

# C compiler stuff
$cc='cl';
$cflags='/W3 /WX /G5 /Ox /O2 /Ob2 /Gs0 /nologo -DWIN32 -DL_ENDIAN';
$lflags="/nologo /subsystem:console /machine:I386";
$mlflags='';
if ($debug)
	{
	$cflags="/W3 /WX /Zi /Yd /Od /nologo -DWIN32 -D_DEBUG -DL_ENDIAN";
	$lflags.=" /debug";
	$mlflags.=' /debug';
	}
$obj='.obj';
$ofile="/Fo";

# EXE linking stuff
$link="link";
$efile="/out:";
$exep='.exe';
if ($no_sock)
	{ $ex_libs=""; }
else	{ $ex_libs="wsock32.lib user32.lib"; }

# static library stuff
$mklib='lib';
$ranlib='';
$plib="";
$libp=".lib";
$shlibp=($shlib)?".dll":".lib";
$lfile='/out:';

$shlib_ex_obj="";
$app_ex_obj="setargv.obj";

$asm='ml /Cp /coff /c /Cx';
$afile='/Fo';
if ($noasm)
	{
	$bn_mulw_obj='';
	$bn_mulw_src='';
	}
else
	{
	$bn_mulw_obj='crypto\bn\asm\x86nt32.obj';
	$bn_mulw_src='crypto\bn\asm\x86nt32.asm';
	}

if ($shlib)
	{
	$mlflags.=" $lflags /dll";
	$cflags.=" /MD";
	$cflags.="d" if ($debug);
	}

sub do_lib_rule
	{
	local($target,$name,$shlib)=@_;
	local($ret,$Name);

	$taget =~ s/\//$o/g if $o ne '/';
	($Name=$name) =~ tr/a-z/A-Z/;

	$ret.="$target: \$(${Name}OBJ)\n";
	if (!$shlib)
		{
		$ret.="\t\$(RM) \$(O_$Name)\n";
		$ret.="\t\$(MKLIB) $lfile$target @<<\n  \$(${Name}OBJ)$ex\n<<\n";
		}
	else
		{
		local($ex)=($Name eq "SSL")?' $(L_CRYPTO)':'';
		$ex.=' wsock32.lib';
		$ret.="\t\$(LINK) \$(MLFLAGS) $efile$target /def:ms/${name}32.def @<<\n  \$(SHLIB_EX_OBJ) \$(${Name}OBJ)$ex\n<<\n";
		}
	$ret.="\n";
	return($ret);
	}

sub do_link_rule
	{
	local($target,$files,$dep_libs,$libs)=@_;
	local($ret,$_);
	
	$file =~ s/\//$o/g if $o ne '/';
	$n=&bname($targer);
	$ret.="$target: $files $dep_libs\n";
	$ret.="  \$(LINK) \$(LFLAGS) $efile$target @<<\n";
	$ret.="  \$(APP_EX_OBJ) $files $libs\n<<\n\n";
	return($ret);
	}

1;
