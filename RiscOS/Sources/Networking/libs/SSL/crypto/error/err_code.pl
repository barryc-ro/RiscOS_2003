#!/usr/local/bin/perl

%errfile=(
	"ERR",	"NONE",
	"BN",	"bn/bn.err",
	"RSA",	"rsa/rsa.err",
	"DSA",	"dsa/dsa.err",
	"DH",	"dh/dh.err",
	"EVP",	"evp/envelope.err",
	"BUF",	"buffer/buffer.err",
	"OBJ",	"objects/objects.err",
	"PEM",	"pem/pem.err",
	"X509",	"x509/x509.err",
	"METH",	"meth/meth.err",
	"ASN1",	"asn1/asn1.err",
	"CONF",	"conf/conf.err",
	"RSAREF","../rsaref/rsaref.err",
	"SSL",	"../ssl/ssl.err",
	);

$function{'RSAREF_F_RSA_BN2BIN'}=1;
$function{'RSAREF_F_RSA_PRIVATE_DECRYPT'}=1;
$function{'RSAREF_F_RSA_PRIVATE_ENCRYPT'}=1;
$function{'RSAREF_F_RSA_PUBLIC_DECRYPT'}=1;
$function{'RSAREF_F_RSA_PUBLIC_ENCRYPT'}=1;
$function{'SSL_F_CLIENT_CERTIFICATE'}=1;

$last="";
while (<>)
	{
	if (/err\(([A-Z0-9]+_F_[0-9A-Z_]+)\s*,\s*([0-9A-Z]+_R_[0-9A-Z_]+)\s*\)/)
		{
		if ($1 != $last)
			{
			if ($function{$1} == 0)
				{
				printf STDERR "$. $1 is bad\n";
				}
			}
		$function{$1}++;
		$last=$1;
		$reason{$2}++;
		}
	}

foreach (keys %function,keys %reason)
	{
	/^([A-Z0-9]+)_/;
	$prefix{$1}++;
	}

@F=sort keys %function;
@R=sort keys %reason;
foreach $j (sort keys %prefix)
	{
	next if $errfile{$j} eq "NONE";
	print STDERR "doing $j\n";
	open(OUT,">$errfile{$j}") || die "unable to open '$errfile{$j}':$!\n";
	@f=grep(/^${j}_/,@F);
	@r=grep(/^${j}_/,@R);
	$num=100;
	print OUT "/* Error codes for the $j functions. */\n\n";
	print OUT "/* Function codes. */\n";
	foreach $i (@f)
		{
		$z=6-int(length($i)/8);
		printf OUT "#define $i%s $num\n","\t" x $z;
		$num++;
		}
	$num=100;
	print OUT "\n/* Reason codes. */\n";
	foreach $i (@r)
		{
		$z=6-int(length($i)/8);
		printf OUT "#define $i%s $num\n","\t" x $z;
		$num++;
		}
	print OUT <<'EOF';

#ifdef  __cplusplus
}
#endif
#endif

EOF
	close(OUT);
	}
