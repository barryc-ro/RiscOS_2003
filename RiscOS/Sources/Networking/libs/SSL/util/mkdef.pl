#!/usr/local/bin/perl
#
# generate a .def file
#
# It does this by parsing the header files and looking for the
# non-prototyped functions.
#

$NT=1;
foreach (@ARGV)
	{
	$NT=1 if $_ eq "32";
	$NT=0 if $_ eq "16";
	$do_ssl=1 if $_ eq "ssl";
	$do_crypto=1 if $_ eq "crypto";
	}

if (!$do_ssl && !$do_crypto)
	{
	print "usage: $0 ( ssl | crypto ) [ 16 | 32 ]\n";
	exit(1);
	}

$ssl="ssl/ssl.h";

$crypto ="crypto/crypto.h";
$crypto.=" crypto/des/des.h";
$crypto.=" crypto/idea/idea.h";
$crypto.=" crypto/rc4/rc4.h";
$crypto.=" crypto/rc2/rc2.h";
$crypto.=" crypto/md/md2.h";
$crypto.=" crypto/md/md5.h";
$crypto.=" crypto/sha/sha.h";

$crypto.=" crypto/bn/bn.h";
$crypto.=" crypto/rsa/rsa.h";
$crypto.=" crypto/dh/dh.h";

$crypto.=" crypto/stack/stack.h";
$crypto.=" crypto/buffer/buffer.h";
$crypto.=" crypto/lhash/lhash.h";
$crypto.=" crypto/conf/conf.h";
$crypto.=" crypto/txt_db/txt_db.h";

$crypto.=" crypto/evp/envelope.h";
$crypto.=" crypto/objects/objects.h";
$crypto.=" crypto/pem/pem.h";
$crypto.=" crypto/meth/meth.h";
$crypto.=" crypto/asn1/asn1.h";
$crypto.=" crypto/asn1/asn1_mac.h";
$crypto.=" crypto/error/err.h";
$crypto.=" crypto/pkcs7/pkcs7.h";
$crypto.=" crypto/x509/x509.h";
$crypto.=" crypto/rand/rand.h";

$match{'NOPROTO'}=1;
$match2{'PERL5'}=1;

&print_def_file(*STDOUT,"SSL",&do_defs("SSL",$ssl)) if $do_ssl == 1;

&print_def_file(*STDOUT,"CRYPT",&do_defs("CRYPT",$crypto)) if $do_crypto == 1;

sub do_defs
	{
	local($name,$files)=@_;
	local(@ret);

	$off=-1;
	foreach $file (split(/\s+/,$files))
		{
#		print STDERR "reading $file\n";
		open(IN,"<$file") || die "unable to open $file:$!\n";
		$depth=0;
		$pr=-1;
		@np="";
		$/=undef;
		$a=<IN>;
		while (($i=index($a,"/*")) >= 0)
			{
			$j=index($a,"*/");
			break unless ($j >= 0);
			$a=substr($a,0,$i).substr($a,$j+2);
		#	print "$i $j\n";
			}
		foreach (split("\n",$a))
			{
			if (/^\#ifndef (.*)/)
				{
				push(@tag,$1);
				$tag{$1}=-1;
				next;
				}
			elsif (/^\#ifdef (.*)/)
				{
				push(@tag,$1);
				$tag{$1}=1;
				next;
				}
			elsif (/^\#if (.*)/)
				{
				push(@tag,$1);
				$tag{$1}=1;
				next;
				}
			elsif (/^\#endif/)
				{
				$tag{$tag[$#tag]}=0;
				pop(@tag);
				next;
				}
			elsif (/^\#else/)
				{
				$t=$tag[$#tag];
				$tag{$t}= -$tag{$t};
				next;
				}
			$t=undef;
			if (/^extern .*;$/)
				{ $t=&do_extern($name,$_); }
			elsif (	($tag{'NOPROTO'} == 1) &&
				($tag{'FreeBSD'} != 1) &&
				(($NT && ($tag{'WIN16'} != 1)) ||
				 (!$NT && ($tag{'WIN16'} != -1))) &&
				($tag{'PERL5'} != 1))
				{ $t=&do_line($name,$_); }
			if (($t ne undef) && (!$done{$name,$t}))
				{
				$done{$name,$t}++;
				push(@ret,$t);
				}
			}
		close(IN);
		}
	return(@ret);
	}

sub do_line
	{
	local($file,$_)=@_;
	local($n);

	return(undef) if /^$/;
	return(undef) if /^\s/;
	if (/(CRYPTO_get_locking_callback)/)
		{ return($1); }
	else
		{
		/\s\**(\S+)\s*\(/;
		return($1);
		}
	}

sub do_extern
	{
	local($file,$_)=@_;
	local($n);

	/\s\**(\S+);$/;
	return($1);
	}

sub print_def_file
	{
	local(*OUT,$name,@functions)=@_;
	local($n)=1;

	if ($NT)
		{ $name.="32"; }
	else
		{ $name.="16"; }

	print OUT <<"EOF";
;
; Definition file for the DDL version of the $name library from SSLeay
;

LIBRARY         $name

DESCRIPTION     'SSLeay $name - eay\@mincom.oz.au'

EOF

	if (!$NT)
		{
		print <<"EOF";

CODE            PRELOAD MOVEABLE
DATA            PRELOAD MOVEABLE SINGLE

EXETYPE		WINDOWS

HEAPSIZE	4096
STACKSIZE	8192

EOF
		}

	print "EXPORTS\n";


	(@e)=grep(/^SSLeay/,@functions);
	(@r)=grep(!/^SSLeay/,@functions);
	@functions=((sort @e),(sort @r));

	foreach $func (@functions)
		{
		printf OUT"    %s%-35s@%d\n",($NT)?"":"_",$func,$n++;
		}
	printf OUT "\n";
	}
