eval '
use Cwd;
$dir = cwd;
';

unless ($dir) {
    eval {
        use POSIX;
        $dir = POSIX::getcwd;
    }
}

die "Could not get current dir" unless ($dir);

eval '
	use Tie::Parent;
	$loaded = 1;
';

unless ($loaded) {
	die "You should first install Tie::Parent from Tie-Tools";
}

eval '
	require Apache;
	$may_apache = 1;
';

if ($may_apache) {
	print "Do you want to run HTPL as a mod_perl extension? ";
	$apache = quotemeta(scalar(<STDIN>));
	chop $apache;
	chop $apache;
	if ("yes" =~ /^$apache/i) {
		&doit("--enable-modperl");
	}
}

print "Does the webserver run with the UID of the HTPL file owner? ";

$dep = quotemeta(scalar(<STDIN>));
chop $dep;
chop $dep;
if ("no" =~ /^$dep/i) {
   	print "Enabling dpenedency database, read the README\n";
	&doit("--with-depdb");
}

&doit;

sub doit {
	my $par = shift;
	print "Running: ./configure $par\n";
	sleep(2);
	system "$dir/configure $par";
	exit(0);
}
