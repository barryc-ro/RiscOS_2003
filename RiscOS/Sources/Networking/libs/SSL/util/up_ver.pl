#!/usr/local/bin/perl
#
# Up the version numbers in the files.
#

@files=(
	"crypto/des/ecb_enc.c",
	"crypto/idea/i_ecb.c",
	"crypto/lhash/lhash.c",
	"crypto/conf/conf.c",
	"crypto/md/md2_dgst.c",
	"crypto/md/md5_dgst.c",
	"crypto/pem/pem_lib.c",
	"crypto/bn/bn_lib.c",
	"crypto/dh/dh_lib.c",
	"crypto/rc4/rc4_enc.org",
	"crypto/rc2/rc2_ecb.c",
	"crypto/rsa/rsa_lib.c",
	"crypto/dsa/dsa_lib.c",
	"crypto/sha/sha1dgst.c",
	"crypto/sha/sha_dgst.c",
	"crypto/asn1/asn1_lib.c",
	"crypto/x509/x509_vrf.c",
	"crypto/evp/evp_enc.c",
	"crypto/rand/md5_rand.c",
	"crypto/stack/stack.c",
	"crypto/txt_db/txt_db.c",
	"crypto/cryptlib.c",
	"ssl/ssl_lib.c",
	"README",
	);

@month=('Jan','Feb','Mar','Apr','May','Jun',
	'Jul','Aug','Sep','Oct','Nov','Dec');
@a=localtime(time());
$time=sprintf("%02d-%s-%04d",$a[3],$month[$a[4]],$a[5]+1900);

$ver=$ARGV[0];
($ver ne "") || die "no version number specified\n";

foreach $file (@files)
	{
	open(IN,"<$file") || die "unable to open $file:$!\n";
	open(OUT,">$file.new") || die "unable to open $file.new:$!\n";
	$found=0;

	while (<IN>)
		{
		if (s/SSLeay [^"]*(\"|\s)/SSLeay $ver $time\1/)
			{
			$found++;
			print OUT;
			while (<IN>) { print OUT; }
			last;
			}
		print OUT;
		}
	close(IN);
	close(OUT);
	(!$found) && die "unable to update the version number in $file\n";
	rename($file,"$file.old") || die "unable to rename $file:$!\n";
	rename("$file.new",$file) || die "unable to rename $file.new:$!\n";
	}
