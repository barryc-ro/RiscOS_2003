#!/usr/local/bin/perl
#
# dodef.pl ... generate the Windows DEF files from a built library
#
# 23-Dec-95 tjh    fixed this hack to be a little more useful
# ................ by actually accepting multiple libs and hiding
# ................ the ssl_ functions from view 
# xx-Dec-95 eay    initial hack
#


foreach $file (@ARGV)
	{
	open(IN,"nm $file|") || die "unable to 'nm $file'\n";
	while (<IN>)
		{
		chop;
		@a=split(/\|/);
		next if ($#a != 7) || ($a[6] =~ /^UNDEF/);
		next unless $a[4] =~ /^GLOB/;
		next unless $a[3] =~ /^FUNC/;
		$func=$a[7];
		if ($func =~ /^SSL/)
			{ push(@SSL,$func); }
		elsif ($func =~ /^ssl/)
		        {                   }
#			{ push(@SSL,$func); }
		elsif ($func =~ /^ERR_load_SSL_strings/)
			{ push(@SSL,$func); }
		elsif ($func =~ /^des/)
			{ push(@DES,$func); }
		elsif ($func =~ /^_des/)
			{ push(@DES,$func); }
		elsif ($func =~ /^crypt/)
			{ push(@DES,$func); }
		else	{ push(@CRYPTO,$func); }
		}
	}

	&output("libdes","LIBDES",@DES);
	&output("ssleay","SSLEAY",@SSL);
	&output("crypto","CRYPTO",@CRYPTO);

sub output
	{
	local($file,$name,@d)=@_;

	@d=sort(@d);

	open(OUT,">$file.def") || die "unable to open ${file}.def:$!\n";
	print OUT <<"EOF";
;
; Definition file for the DLL version of $file
;

LIBRARY      $name

DESCRIPTION  'SSLeay $file - eay@mincom.oz.au'

CODE         PRELOAD MOVEABLE
DATA         PRELOAD MOVEABLE SINGLE

EXETYPE      WINDOWS

HEAPSIZE      4096
STACKSIZE     8192

EXPORTS
EOF
	$i=1;
	foreach (sort @d)
		{
		printf OUT "    _%-40s@%d\n",$_,$i++;
		}
	close(OUT);
	}
