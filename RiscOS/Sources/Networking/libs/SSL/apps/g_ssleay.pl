#!/usr/local/bin/perl

$mkprog='mklinks';
$rmprog='rmlinks';

print "#ifndef NOPROTO\n";

grep(s/^asn1pars$/asn1parse/,@ARGV);

foreach (@ARGV)
	{ printf "extern int %s_main(int argc,char *argv[]);\n",$_; }
print "#else\n";
foreach (@ARGV)
	{ printf "extern int %s_main();\n",$_; }
print "#endif\n";


print <<'EOF';

#define FUNC_TYPE_GENERAL	1
#define FUNC_TYPE_MD		2
#define FUNC_TYPE_CIPHER	3

typedef struct {
	int type;
	char *name;
	int (*func)();
	} FUNCTION;

FUNCTION functions[] = {
EOF

foreach (@ARGV)
	{
	push(@files,$_);
	print "#ifndef NO_SOCK\n" if $_ =~ /^s_/;
	printf "\t{FUNC_TYPE_GENERAL,\"%s\",%s_main},\n",$_,$_;
	print "#endif\n" if $_ =~ /^s_/;
	}

foreach ("md2","md5","sha","sha1")
	{
	push(@files,$_);
	printf "\t{FUNC_TYPE_MD,\"%s\",dgst_main},\n",$_;
	}

foreach (
	"base64",
	"des", "des3", "idea", "rc4", "rc2",
	"des-ecb", "des-ede",    "des-ede3",
	"des-cbc", "des-ede-cbc","des-ede3-cbc",
	"des-cfb", "des-ede-cfb","des-ede3-cfb",
	"des-ofb", "des-ede-ofb","des-ede3-ofb",
	"idea-cbc","idea-ecb",   "idea-cfb",  "idea-ofb",
	"rc2-cbc", "rc2-ecb",     "rc2-cfb",  "rc2-ofb")
	{
	push(@files,$_);

	$t=sprintf("\t{FUNC_TYPE_CIPHER,\"%s\",enc_main},\n",$_);
	if    ($_ =~ /rc4/)  { $t="#ifndef NO_RC4\n${t}#endif\n"; }
	elsif ($_ =~ /rc2/)  { $t="#ifndef NO_RC2\n${t}#endif\n"; }
	elsif ($_ =~ /idea/) { $t="#ifndef NO_IDEA\n${t}#endif\n"; }
	print $t;
	}

print "\t{0,NULL,NULL}\n\t};\n";

open(OUT,">$mkprog") || die "unable to open '$prog':$!\n";
print OUT "#!/bin/sh\nfor i in ";
foreach (@files)
	{ print OUT $_." "; }
print OUT <<'EOF';

do
echo making symlink for $i
/bin/rm -f $i
ln -s ssleay $i
done
EOF
close(OUT);
chmod(0755,$mkprog);

open(OUT,">$rmprog") || die "unable to open '$prog':$!\n";
print OUT "#!/bin/sh\nfor i in ";
foreach (@files)
	{ print OUT $_." "; }
print OUT <<'EOF';

do
echo removing $i
/bin/rm -f $i
done
EOF
close(OUT);
chmod(0755,$rmprog);
