eval {
use Cwd;
$dir = cwd;
};

unless ($dir) {
    eval {
        use POSIX;
        $dir = POSIX::getcwd;
    }
}

die "COuld not get current dir" unless ($dir);

print <<EOM;
This package should be installed via its Gnu configure script.
Change directory to $dir and type ./configure
Before doing it, please read installation instructions 
in $dir/INSTALL

Now Break....
EOM

$x = <STDIN>;

die;
