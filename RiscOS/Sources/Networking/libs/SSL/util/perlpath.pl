#!/usr/local/bin/perl
#
# modify the '#!/usr/local/bin/perl'
# line in all scripts that rely on perl.
#

require "find.pl";

$#ARGV == 0 || print STDERR "usage: perlpath newpath  (eg /usr/bin)\n";
&find(".");

sub wanted
	{
	return unless /\.pl$/ || /^Configure$/;

	open(IN,"<$dir/$_") || die "unable to open $dir/$_:$!\n";
	@a=<IN>;
	close(IN);

	$a[0]="#!$ARGV[0]/perl\n";

	# I'm not going to play safe, but then people should not be
	# running this often.
	open(OUT,">$dir/$_") || die "unable to open $dir/$_:$!\n";
	print OUT @a;
	close(OUT);
	}
