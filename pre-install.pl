use CPAN;
eval 'require Digest::MD5';

@prereq = qw(IO Net::FTP Compress::Zlib Digest::MD5 MIME::Base64
	URI HTML::Parser LWP);
@useful = (qw(Tie::Cache Tie::Depth
        Storable Time::Timezone Sys::Syslog Net::Daemon
        RPC::PlServer
        DBI DBD::XBase DB_File File::Tools Mail::Tools
	SQL::Statement Text::CSV_XS DBD::CSV FreezeThaw IniConf
        Data::Dumper GD MLDBM Convert::BER Net::LDAP XML::Parser
        Net::DNS Net::Whois Net::Country Tie::Cache 
	Filter::Util::Call Template XML::Simple Date::Language
        XML::Conf));

if ($ARGV[0] eq 'NOPREREQ') {
	@modules = @useful;
	shift @ARGV;
} else {
	@modules = (@prereq, @useful);
        if ($] < 5.6) {
            print "Try: perl pre-install NOPREREQ\nif CPAN tries helplessly to download Perl 5.6.0 and install it\07\n";
            sleep 4;
        }
}

unshift(@INC, $ARGV[0]) if (@ARGV);

&cls;
&confcpan;
&makeobjs;

foreach $mod (@objs) {
        &cls;
	eval { $mod->install; };
        if ($loop < @prereq) {
            $nm = $modules[$loop++];
            eval "require $nm;";
        }
}

&cls;
print "\007";

if (@ARGV) {
    system "rm -rf $root";
}

sub makeobjs {
    &cls;
    @objs = map {
	CPAN::Shell->expand('Module', $_);} @modules;
}

sub cls {
    print "\e[2J";
}

sub confcpan {
  eval 'require CPAN::Config';
  if (@ARGV) {
    require "getcwd.pl";
    $cwd = &getcwd;
    $root = "$cwd/temp";
    $arg = `/bin/cat instnonroot.dat`;
    $arg =~ s/\%/$ARGV[0]/g;
    $arg =~ s/\@/$root/;
    mkdir "$root", 0777;
    mkdir "$root/build", 0777;
    mkdir "$root/sources", 0777;
    $CPAN::Config->{'makepl_arg'} = $arg;
    $CPAN::Config->{'cpan_home'} = $root;
    $CPAN::Config->{'build_dir'} = "$root/build";
    $CPAN::Config->{'keep_source_where'} = "$root/sources";
  }
}
