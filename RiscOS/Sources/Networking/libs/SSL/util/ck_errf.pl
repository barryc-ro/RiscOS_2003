#/usr/local/bin/perl
#
# This is just a quick script to scan for cases where the 'error'
# function name in a SSLerr() macro is wrong.
# 
# Run in the top level by going
# perl util/ch_errf.pl */*.c */*/*.c
#

foreach $file (@ARGV)
	{
	open(IN,"<$file") || die "unable to open $file\n";
	$func="";
	while (<IN>)
		{
		if (/^[a-zA-Z].+[\s*]([A-Za-z_0-9]+)\(.*\)/)
			{
			$func=$1;
			$func =~ tr/A-Z/a-z/;
			}
		if (/[A-Z0-9]+err\(([^,]+)/)
			{
			next if ($func eq "");
			$n=$1;
			if ($n !~ /_F_(.*)$/)
				{ print "$file:$.:$func:$n\n"; next; }
			$n=$1;
			$n =~ tr/A-Z/a-z/;
			if ($n ne $func)
				{ print "$file:$.:$func:$n\n"; next; }
	#		print "$func:$1\n";
			}
		}
        }

