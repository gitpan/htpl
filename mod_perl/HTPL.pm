package Apache::HTPL;

no strict;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK);

require Exporter;
require DynaLoader;
require AutoLoader;

@ISA = qw(Exporter DynaLoader);
# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.
@EXPORT = qw(
	
);
$VERSION = '0.01';

bootstrap Apache::HTPL $VERSION;


# Preloaded methods go here.


# Autoload methods go after =cut, and are processed by the autosplit program.


use Shell;

sub handler {
    my ($self, $r) = @_;
    $r = Apache->request unless (ref($r) =~ /Apache/);

    $| = 1;

    my $in_mod_htpl = 1;
    require HTML::HTPL::Lib;

    my $filename = $r->filename;

    if (-d $filename) {
        $filename =~ s|/$||;
        $filename .= "/index.htpl";
    }
    return $Apache::Constants::NOT_FOUND unless (-e $filename);

    %ENV = $r->cgi_env;


    my $ref = $Apache::HTPL::htpl_codes{$filename};

    my $script = &HTML::HTPL::Lib'tempfilename;

    my $ecode;

    if ($ref) {
        my $pagelm = (stat($filename))[10];
        my $scriptlm = $ref->{'lm'};
        undef $ref if ($pagelm > $scriptlm);
    }

    unless ($ref) {
            pipe(READ, WRITE);        
            my $pid;
            unless ($pid = fork) {
                require HTML::HTPL::Config;
                close(STDOUT);
                close(READ);
                open(STDOUT, ">&WRITE");
                close(WRITE);
                compile($filename, $script, $HTML::HTPL::Config'dbgbin);
                close(STDOUT);
                CORE::exit;    
            }
            close(WRITE);
            waitpid($pid, 0);
            my @lines = <READ>;
            my $buff = join("", @lines);
            close(READ);
            if ($buff) {
                $r->content_type("text/plain");
                $r->send_http_header;
                $r->print("Error:\n$buff");
                return 0;
            }

        my $tcode = Shell::cat($script);
        unlink $script;
        my $scrfn = $filename;
        $scrfn =~ s/([^A-Za-z0-9])/"::c" . (unpack("C", $1))/ge;
        my $subname = "__htpl_doit";
        my $pkg = "Apache::ROOT$scrfn";
        my $precode = qq!
package $pkg;
use Apache;
use Apache::HTPL::Lib qw(exit);
sub $subname {
use HTML::HTPL::Lib;
use HTML::HTPL::Sys;
\$HTML::HTPL::Lib'in_mod_htpl = 1;
\$HTML::HTPL::Lib'htpl_pkg = '$pkg';
import Apache::HTPL::Lib qw(exit);
*0 = \'$filename';
$tcode
htpl_lblend:
}
1;
!;	

        $@ = undef;
        eval("undef \&$pkg\::$subname;"); 
        eval $precode;
        $ecode = !$@;
        $ref = {'package' => $pkg,
                               'proc' => $subname,
                               'code' => \&{"$pkg\::$subname"},
                               'str' => "$pkg\::$subname",
                               'lm'   => time} if ($ecode);
        $Apache::HTPL::htpl_codes{$filename} = $ref;
    } else {
        $ecode = 1;
    }

    $ENV{'TEMP'} = '/tmp';
    my $out = &HTML::HTPL::Lib'tempfilename;
    my $head = &HTML::HTPL::Lib'tempfilename;
    $ENV{'HTTP_HEADERS'} = $head;
    open(O, ">$head");
    print O "Content-type: text/html\n";
    close(O);

    open(HTPL_MOD_OUT, ">$out");
    select(HTPL_MOD_OUT);
    my $package = $ref->{'package'};
    my %symbol = eval '%' . "${package}::";

    foreach (keys %symbol) {
        next if ($_ eq 'import');
        my $val = $symbol{$_};
        $val =~ s/^\*//;
        eval "undef \$$val";
        eval "undef \@$val";
        eval "undef \%$val";
    }

    %{"$package\::ENV"} = %ENV;
    my $str = $ref->{'str'};
    $ecode &&= eval("\&$str; 1;");


    select(STDOUT);
    close(HTPL_MOD_OUT);

    unless ($ecode) {
        $r->content_type("text/plain");
        $r->send_http_header;
        $r->print("Error: $@");
        return 0;
    }

    my $hthd = Shell::cat($head) . "\n";
    my $txt = Shell::cat($out);
    unlink $head;
    unlink $out;
    $r->send_cgi_header("$hthd\n");
    if ($HTML::HTPL::Lib'htpl_redirected) {
        return $Apache::Constants::MOVED;
    }
    $r->print($txt);

    return 0;
}


1;

