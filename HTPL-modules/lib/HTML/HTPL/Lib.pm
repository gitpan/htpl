package HTML::HTPL::Lib;


use HTML::HTPL::Sys qw(html_table html_table_out publish doredirect
getmailprog proper ch2x safetags checktaint htdie compileutil);

use Socket; 


require Exporter;
@ISA = qw(Exporter);

@EXPORT = qw(html_hidden html_format max min html_table_cols isalive
html_table_rows html_header html_footer include redirect new_table
forkredirect setmimetype setcookie addheader calcweek convyear
sendmail getemail tempfilename readfile catdirfile ampm html_color
opendoc closedoc isurl nslookup revnslookup saferevnslookup validurl
validemail fileexists isip urlencode urldecode trim html_selectbox filesig
txt2html doconnect expect inputlist rewind htmlencode daysin isleap
unitelist intersectlist takelog increasefile lastmodified avg getmdy
begintransaction endtransaction imagesize finger revmap safemkdir mkfile
jewishdate getdmy monthname weekdayname foreachdir slash wrap hebrew_wrap
pusht popt undouble uniq timestep rotate ror rol getcwd hostname core
selfurl querystring takebroadlog subpkg subhash maketime
html_treeview selfsameurl new_template new_select getweekday
elapsed hebrewflip agg sum splitline $STD_BODY @MONTH_NAMES @WEEKDAY_NAMES);

@MONTH_NAMES = qw(January February March April May June July August
September October November December);
@WEEKDAY_NAMES = qw(Sunday Monday Tuesday Wednesday Thursday Friday
Saturday);

sub monthname {
    substr($MONTH_NAMES[shift], 0, 3);
}

sub weekdayname {
    substr($WEEKDAY_NAMES[shift], 0, 3);
}

sub getdmy {
    my $t = shift || time;
    my @t = localtime($t);
    $t[4]++;
    $t[5] = &convyear($t[5]);
    @t[3 .. 5];
}

sub getmdy {
    (getdmy(@_))[(1, 0, 2)];
}
    
sub getweekday {
    require Time::Local;
    my ($t) = ($#_ > 4) ? Time::Local::timelocal(@_) :
                   ($#_ == 2 ? &maketime(@_)
                        :
                ($_[0] || time)
             );
    (localtime($t))[6];
}
    

sub convyear {
   $_[0] + int((2100 - $_[0]) / 1900) *
    (1900 + 100 * int((170 - $_[0]) / 100));
}

sub jewishdate {
    require Locale::Hebrew::Calendar;
    my @d = Locale::Hebrew::Calendar::g2j($#_ == 2 ? @_ : &getdmy(@_));
    $d[1]++;
    @d;
}


sub html_hidden {
    my ($key, $value) = @_; # noout deprecated
    $value = &HTML::HTPL::Sys::getvar($key) unless ($value);
    $code = "<INPUT NAME=\"$key\" VALUE=\"$value\" TYPE=HIDDEN>\n";
    print $code unless defined wantarray;
    $code;
}

sub html_format {
    my ($msg, $tags, $nonl) = @_;
    my (@ht_tags) = (UNIVERSAL::isa($tags, 'ARRAY')) ? @$tags : split(/\|/, $tags);
    my ($i, @t, $s);

    $s = join("", map {"<$_>";} @ht_tags);
    $s .= $msg;
    $s .= join("", map { s/\s.*$//; "</$_>";} reverse @ht_tags);
    $s .= "\n" unless ($nonl);
    print $s unless defined wantarray;
    $s;
}

sub max {
    (sort {$a <=> $b} @_)[-1];
}

sub min {
    (sort {$a <=> $b} @_)[0];
}

sub agg (&@) {
    my ($code, @elems) = @_;
    local ($a, $b);
    $a = shift @elems;
    while (@elems) {
        $b = shift @elems;
        $a = &$code;
    }
    $a;
}

sub sum {
    return agg {$a + $b;} @_;
}

sub avg {
    return undef unless (@_);
    &sum(@_) / @_;
}

sub html_table_rows {
    my (%tags) = @_;
    &html_table(@_, 'expr' => sub {($_ % $tags{"cols"}, int($_ / $tags{"cols"}))});
}

sub html_table_cols {
    my (%tags) = @_;
    my ($items) = $tags{'items'};
    $tags{'percol'} = int(($#$items + 1) / ($tags{'cols'})) + 1;
    &html_table(%tags, 'expr' =>  sub {(int($_ / $tags{"percol"}), $_ % $tags{"percol"})});
}

$STD_BODY = "BGCOLOR=#FFFFFF TEXT=#000000 LINK=#0000FF VLINK=#FF00FF ALINK=#FF0000";

sub html_header {
    my ($title, $body) = @_;
    my ($html) = <<EOM;
<HTML>
<HEAD>
<TITLE>
  $title
</TITLE>
</HEAD>
EOM
    if ($body) {
        $body = &HTML::HTPL::Sys::evit($body);
        $html .= <<EOM;
<BODY $body>
EOM
    }
    print $html unless defined wantarray;
    $html;
}

sub html_footer {
    my ($html) = <<EOM;
</BODY>
</HTML>
EOM
    print $html unless defined wantarray;
    $html;
}

sub html_color {
    sprintf("#%02X%02X%02X", @_);
}

sub include {
    my ($fn);
    foreach $fn (@_) {
        my ($expr);
        opendoc(I, $fn);
        $expr = "";
        while (<I>) {
            s/[\n\r]+$//;
#        s/\\/\\\\/g;
            s/"/\\"/g;
            s/#(.*?)#/$1 ? '$' . $1 : '#'/ge;
            $expr .= "print \"$_\\n\";\n";
        }
        eval $expr;
        closedoc(I);
    }
}

sub redirect {
    local($url) = @_;
    return unless ($HTML::HTPL::Sys::on_htpl);
    &doredirect($url);
    exit(0);
}

sub forkredirect {
    my ($url) = @_;
    return unless ($HTML::HTPL::Sys::on_htpl);

    if (fork) {
        &doredirect($url) if ($url);
        exit(0);
    }

    close(STDOUT);
    close(STDERR);


}

sub eraseheader {
    my $re = shift;

    return unless($HTML::HTPL::Sys::on_htpl);
    
    seek(HTML::HTPL::Sys::HEADERS, 0, 0);
    my @lines;
    while (<HTML::HTPL::Sys::HEADERS>) {
        chop;
        push(@lines, $_) if ($_ && !/$re/);
    }
    seek(HTML::HTPL::Sys::HEADERS, 0, 0);
    chomp @lines;
    print HTML::HTPL::Sys::HEADERS join("\n", @lines, "");
    truncate(HTML::HTPL::Sys::HEADERS, 0);
}

sub setmimetype {
    my ($type) = @_;
    &eraseheader(/^Content-type/);
    print HTML::HTPL::Sys::HEADERS join("\n", @lines, 
      "Content-type: $type", "");
}



sub setcookie {
    return unless($HTML::HTPL::Sys::on_htpl);
    my ($key, $val);
    while (@_) {
        $key = shift;
        $val = shift;
        &eraseheader("^Set-Cookie: $key=");
        unless (UNIVERSAL::isa($val, 'HASH')) {
            &addheader("Set-Cookie: $key=$val");
        } else {
            &HTML::HTPL::Sys::safehash($val);
            my @ary;
            push(@ary, $val->{'data'});
            my $server = $val->{'server'};
            push(@ary, "server=$server") if ($server);
            my $domain = $val->{'domain'};
            push(@ary, "domain=$domain") if ($domain);
            my $expires = $val->{'expires'};
            if ($expires eq '*') {
                $expires = "Tue, 19-Jan-2038 00:00:00 GMT";
                push(@ary, "expires=$expires");
            }
            &addheader("Set-Cookie: $key=" . join("; ", @ary));
        }
    }
}

sub erasecookie {
    &setcookie(shift, "");
    1;
}

sub addheader {
    my $str = shift;
    return unless ($HTML::HTPL::Sys::on_htpl);
    print HTML::HTPL::Sys::HEADERS "$str\n";
}

sub sendmail {
    my (@preattrs) = @_;
    my (%attrs) = &proper(sub {ucfirst(lc($_));}, @preattrs);

#    my ($mailprog) = &getmailprog;
    my ($to, $from, $subject, $addr, $extra, $me, $mailsrv, $msg);

    require Net::SMTP;

    $msg = $attrs{'Msg'};

    $to = $attrs{'To'};
    $from = $attrs{'From'};
    $subject = $attrs{'Subject'};
    $mailsrv = $attrs{'Mailhost'} ||
        $Net::Config::NetConfig{'smtp_hosts'}->[0] || 'localhost';

    $addr = &getemail($to);
    &htdie("Invalid email $to") unless (&validemail($addr));
    $me = &getemail($from);
    &htdie("Invalid email $from") unless (&validemail($me));

    delete @attrs{qw(From To Subject Mailhost Msg)};
    my $smtp = new Net::SMTP($mailsrv, Timeout => 60) || &htdie("Can't connect mail server");
    $smtp->mail($me);
    $smtp->to($addr);

    $smtp->data();
    $smtp->datasend(<<EOM);
From: $from
To: $to
Subject: $subject
EOM
     
    foreach $extra (keys %attrs) {
        $smtp->datasend("$extra: " . $attrs{$extra} . "\n");
    }
    
    $smtp->datasend("\n$msg");
    $smtp->quit;
}

sub getemail {
    my ($recpt) = @_;

    require Mail::Address;
    my (@a) = Mail::Address->parse($recpt);
    my ($email) = $a[0]->address;
    return &HTML::HTPL::Sys::checktaint($email);
}

sub tempfilename {
    my ($fn, $prg, $dir);

    $prg = lc($0);
    $prg =~ s/[^a-z0-9]//g;
    $fn = sprintf("~~%s-%05d-%05d-%08d-%02d.tmp", $prg, $$, getppid,
      time, $generate_temp_filenames++);
    $dir = $ENV{'TEMP'} || $HTML::HTPL::Config::temp;
    &catfile($dir, $fn);
}


sub getcc {
    return $HTML::HTPL::Config::ccprog if ($HTML::HTPL::Config::ccprog);

    my ($cc, $p, $d);
    foreach $cc (qw(gcc shlicc2 cc egcs)) {
        $p = `which $cc`;
        chop $p;
        return $p if ($p);
        foreach $d (qw(/bin /sbin /usr/bin /usr/sbin
              /usr/local/bin/ /usr/local/sbin/ /u/local/bin/)) {
            $p = "$d/$cc";
            return $p if (-e $p);
        }
    }
    Carp::croak("No C compiler");
}

sub readfile {
    my ($filename) = @_;
    my ($txt) = '';
    opendoc(I, $filename);
    $txt = join("", <I>);
    closedoc(I);
    $txt;
}

sub opendoc {
    my ($handle, $filename) = @_;
    my ($fn, $txt);
    $gottenfromweb{$handle} = undef;
    if (&isurl($filename)) {
        $fn = &tempfilename;
        $gottenfromweb{$handle} = $fn;
        require LWP::Simple;
        $txt = LWP::Simple::get($filename);
        open($handle, ">$fn");
        print $handle $txt;
        close($handle);
        open($handle, $fn);
    } else {
        open($handle, $filename);
    }
}

sub closedoc {
    my ($handle) = @_;
    close($handle);
    my ($fn);
    $fn = $gottenfromweb{$handle};
    return unless ($fn);
    unlink $fn;
    $gottenfromweb{$handle} = undef;
}

sub isurl {
    $_[0] =~ /^\w+:\/\//i;
}

sub nslookup {
    my ($host) = @_;

    return $host if (&isip($host));

    my @p = gethostbyname($host);
    my ($a) = $p[4];

    return undef unless ($a);

    my @ip = unpack("C*", $a);

    return join(".", @ip);
}

sub revnslookup {
    my ($host) = @_;
    return undef unless (&isip($host));

    my @p = gethostbyaddr(pack("C*", split(/\./, $host)), AF_INET);

    my ($answer) = $p[0];

    return $host unless ($answer);
    return $answer;
}

sub saferevnslookup {
    my ($host) = @host;
    return (&isip($host) ? &revnslookup($host) : $host);
}

sub validurl {
    my ($url) = @_;
    require LWP::UserAgent;
    require HTTP::Request;
    my ($ua, $request, $response);
    $ua = LWP::UserAgent->new;
    $request = HTTP::Request->new('GET', $url);
    $response = $ua->request($request);
    return !($response->is_error);
}

sub validemail {
    my ($user, $host) = split(/\@/, shift);
    return undef unless ($user && $host);
    return undef if ($user =~ /\s'"/);
    return undef unless ($host =~ /^(\S+\.)+\S+$/);
    return undef if (lc($host) eq 'microsoft.com' && grep {/$user/} qw(bill gates bgates));
    local(@mx);
    require Net::DNS;
    @mx = Net::DNS::mx($host);
    return undef if $#mx < 0;
    return 1;
}

sub fileexists {
    my ($file) = @_;
    return &validurl($file) if (&isurl($file));
    return undef unless (-f $file);
    1;
}

sub isip {
    my ($a) = @_;
    $a =~ /^(\d{1,3}\.){3}\d{1,3}$/;
}

sub isalive {
    require Net::Ping;
    my $p = new Net::Ping("icmp", 10, 32);
    my $r = $p->ping(shift);
    $p->close;
    $r;
}

sub urlencode {
    my ($u) = @_;
    require URI::Escape;
    URI::Escape::uri_escape($u);
}

sub urldecode {
    my ($u) = @_;
    $u =~ s/\+/ /g;
    $u =~ s/\%(\d\d)/pack("c", hex($1))/oge;
}

sub trim {
    local($str) = @_;
    $str =~ s/^\s+//;
    $str =~ s/\s+$//;
    $str =~ s/\s{2,}/ /g;
    $str;
}

sub html_selectbox {
    my ($par) = shift;
    my ($o, $n, $default, %opt) = ();
    my ($name, $attr, $code);
    
    unless (UNIVERSAL::isa($par, 'HASH')) {
        $default = $par;
    } else {
        &HTML::HTPL::Sys::safehash($par);
        $default = $par->{'default'};
        $attr = $par->{'attr'};
        $attr = " $attr" if ($attr);
        $name = $par->{'name'};
        if ($name) {
            $code .= qq!<SELECT NAME="$name"$attr>! . "\n";
        }
        $noout = $par->{'noout'};
    }

    if ($default) {
        $default = [$default] unless (UNIVERSAL::isa($default, 'ARRAY'));
        foreach (@$default) {
            $opt{$_} = " SELECTED";
        }
    }

    while (@_) {
        $o = shift;
        $n = shift;
	$o = $n unless (defined($o));
        $code .= ' ' x 4 . '<OPTION VALUE="' . $o . '"' . $opt{$o} . '>' . $n . "\n";
    }
    $code .= "</SELECT>\n" if ($name);
    print $code unless defined wantarray;
    $code;
}

sub txt2html {
    my ($txt) = @_;
    $txt =~ s/\</&lt;/g;
    $txt =~ s/\>/&gt;/g;
    $txt =~ s/"/&quot;/g;
    $txt =~ s/\&/&amp;/g;
    $txt =~ s/\n\r?/<BR>/g;

    $txt;
}

sub htmlencode {
    txt2html(@_);
}

sub doconnect {
    my ($hnd, $host, $port) = @_;
    my ($iaddr, $paddr, $proto);

    require IO::Socket::INET;
    import IO::Socket::INET;
    my $h = new IO::Socket::INET('PeerAddr' => $host, 'PeerPort' => $port,
            'Proto' => 'tcp', 'Type' => SOCK_STREAM)
          || Carp::croak("Could not connect to $host:$port");
    $hnd = eval("\\*${htpl_pkg}::$hnd") unless (ref($hnd));
    *{$hnd} = $h;
}

sub expect {
    my ($hnd, $re, $err) = @_;
    my ($line, @ts);
    $line = <$hnd>;
    return @ts if (@ts = ($line =~ /$re/));
    $_ = $line;
    &$err if ($err && UNIVERSAL::isa($err, 'CODE'));
    return undef;
}

sub doread {
    my ($hnd) = @_;
    my ($buff, $n);
    $n = sysread($hnd, $buff, 8192);
    return substr($buff, 0, $n);
}

sub dowrite {
    my ($hnd, $line) = @_;
    syswrite $hnd, $line, length($line);
}

sub inputlist {
    my ($values, @tags) = @_;
    my (%tags) = &safetags(@tags);
    my ($code);
    my ($oncheck, $name, $type, $default, $attr) = 
        @tags{('oncheck', 'name', 'type', 'default', 'attr')};
    $default = [$default] unless (UNIVERSAL::isa($default, 'ARRAY'));
    my (%opt);
    foreach (@$default) {
        $opt{$_} = " CHECKED";
    }

    $oncheck = qq! onCheck="$oncheck"! if ($oncheck);
    $attr = " $attr" if ($attr);

    my @code = map {
        my $val = $_->{'value'};
        my $text = $_->{'text'};
        my $chk = $opt{$val};
        qq!<INPUT TYPE=$type NAME="$name" VALUE="$val"$oncheck$attr$chk> $text!;
    } @$values;
    wantarray ? @code : join("\n", @code);
}

sub rewind {
    return unless ($HTML::HTPL::Sys::on_htpl);
    $hnd = ($in_mod_htpl ? HTPL_MOD_OUT : STDOUT);
    seek($hnd, 0, 0);
    truncate($hnd, 0);
}

sub unitelist {
    my ($a, $b) = @_;
    my (%opt, @c);

    foreach (@$a) {
        $opt{$_} = 1;
    }

    foreach (@$b) {
        $opt{$_} = 1;
    }

    @c = sort keys %opt;
    return \@c;
}

sub intersectlist {
    my ($a, $b) = @_;
    my (%opt, @c);

    foreach (@$a) {
        $opt{$_}++;
    }

    foreach (@$b) {
        $opt{$_}++;
    }

    @c = sort grep {$opt{$_} > 1;} keys %opt;
    return \@c;
}

sub takelog {
    my $msg = shift;
    my $name = $0;
    $name =~ s/[^\.]+$/log/;

    my $lfn = $_[0] || $HTML::HTPL::Config::htpl_default_log_file || $name;
    open(TAKE_LOG, ">>$lfn");
    print TAKE_LOG $0 . ", " . scalar(localtime) . " : $msg\n";
    close(TAKE_LOG);
}

sub takebroadlog {
    my @params;
    my $l = 0;
    my $slash = &slash;
    while (1) {
        my @c = caller[$l++];
        my ($package, $filename, $line, $subroutine,
                    $hasargs, $wantarray, $evaltext, $is_require) = @c;
        last if ($package =~ /^Apache::HTPL/ || !@c);
        $filename =~ s/^.*$slash//;
        push(@params, "$package($filename):$line\&$subroutine");
    }
    my $rem = &HTML::HTPL::Sys::getvar('REMOTE_USER');
    &takelog(($rem ? ("u:$rem") : ()), join("/", @params), @_);
}

sub core {
    my $name = $0;
    $name =~ s/[^\.]+$/dump/;
    open(CORE, ">$name");
    print CORE "Core dumped at " . scalar(localtime) . "\n";
    my $i;
    foreach (@_) {
        print CORE ++$i . ") $_\n";
    }
    close(CORE);
}

sub increasefile {
    my ($filename) = @_;
    my ($l);

    require Shell;
    $l = Shell::cat($filename);
    chomp $l;
    my ($cnt, $lasthost) = split(/:/, $l);
    $cnt++ unless ($lasthost && $lasthost eq &HTML::HTPL::Sys::getvar("REMOTE_HOST"));
    &mkfile($filename, "$cnt:" . &HTML::HTPL::Sys::getvar('REMOTE_HOST'));
    return $cnt;
}

sub lastmodified {
    my ($fn) = (@_, $0);
    my @sa = stat($fn);
    $sa[9];
}

sub imagesize {
    my ($pic, $ashtml) = @_;
    require GD;
    open(GIF_HANDLE, $pic);
    binmode GIF_HANDLE;
    my $img = newFromGif GD::Image(GIF_HANDLE);
    close(GIF_HANDLE);
    return undef unless ($img);
    return $img->getBounds;
}

sub finger {
    require Net::Finger;
    my ($u) = shift;
    wantarray ? Net::Finger($u, 1) : scalar(Net::Finger($u, 1));
}

sub hebrewflip {
    require Locale::Hebrew;
    Locale::Hebrew::hebrewflip(shift);
}

sub splitline {
    require Text::ParseWords;
    my @a = Text::ParseWords::parse_line(",", undef, shift);
    map {s/^\s+//; s/\s+$//;} @a;
    return @a;
}

sub foreachdir (&$@) {
    my ($proc, $par, @dirs) = @_;
    require File::Recurse;
    my $save = $File::Recurse::MAX_DEPTH;
    $File::Recurse::MAX_DEPTH = $par->{'depth'} if ($par->{'depth'});
    my $code = $proc;
    if ($par->{'filter'}) {
        my $exp = $par->{'types'};
        $exp =~ s/<(-\w)>/$1 \$_/;
        $code = eval "sub {&\$proc if ($exp);};";
    }
    foreach (@dirs) {
        File::Recurse::recurse($code, $_);
    }
    $File::Recurse::MAX_DEPTH = $save;
}


sub checkwin {
    require Config;
    my $os = $^O || $Config::Config{'osname'};
    $iswin = ($os =~ /Win32/);
}

sub slash {
    &checkwin;
    return ($iswin ? "\\" : "/");
}

sub safemkdir {
    my $dir = shift;
    my $mode = shift || 0755;
    my $slash = &slash;
    my @tokens = split($slash, $dir);
    my @done;
    while (@tokens) {
        push(@done, shift @tokens);
        my $this = join($slash, @done);
        mkdir $this, $mode unless (-d $this);
    }
} 

sub catdirfile {
    my ($dir, $file) = @_;
    $dir . ($dir ? &slash : "") . $file;
}

sub daysin {
    my ($m, $y) = @_;
    my $leap = &isleap($y);
    my @days = qw(31 28 31 30 31 30 31 31 30 31 30 31);
    $days[2]++ if ($leap);
    $days[$m];
}

sub isleap {
    my $y = &convyear(shift);
    !(($y % 4) || ($y % 400) && !($y % 100));
}

sub maketime {
    require Time::Local;
    Time::Local::timelocal((reverse @_[3 .. 5]), @_[0 .. 2]);
}

sub calcweek {
    require Time::Local;
    my ($t) = ($#_ > 4) ? Time::Local::timelocal(@_) : 
                   ($#_ == 2 ? &maketime(@_)
                        : 
                ($_[0] || time)
             );
    my (@t) = localtime($t);
    my ($yday) = $t[7];
    @t[3, 4] = (1, 0);
    $t = Time::Local::timelocal(@t);
    @t = localtime($t);
    my ($wdayjan1) = $t[6];
    int(($yday + $wdayjan1) / 7);
}

sub ampm {
    my $h = shift;
    my $h12 = ($h + 11) % 12 + 1;
    my $hp = int($h / 12);
    my @hn = qw(AM PM);
    return sprintf("%02d%s", $h12, $hn[$hp]);
}

sub wrap {
    my ($line, $ln) = @_;
    require Text::Wrap;
    require Text::Tabs;
    my $save = $Text::Wrap::columns;
    $Text::Wrap::columns = $ln;
    $line =~ s/\t/\t /;
    my $str = Text::Tabs::expand(Text::Wrap::wrap('', '', 
               split(/ /, $line)));
    $Text::Wrap::columns = $save;
    wantarray ? split(/\n/, $str) : $str;
}

sub hebrew_wrap {
    my ($line, $ln) = @_;
    return &wrap($line) unless HTML::HTPL::Sys::isheb($line);
    my @lines = split(/\n/, &wrap($line));
    join("\n", map { hebrewflip($_); } @lines);

}

sub dos2unix {
    my $txt = shift;
    $txt =~ s/\r\n/\n/g;
    $txt =~ tr/\x80-\x9A/\xE0-\xFA/ if ($HTML::HTPL::Config::htpl_flip_hebrew);
    $txt;
}

sub readurl {
    my $url = shift;
    require HTML::LinkExtor;
    require LWP::Simple;
    require LWP::MediaType;
    my $txt = LWP::Simple::get($txt);
    my (@imgs, @links);
    my $p = new HTML::LinkExtor(sub {
        my ($tag, $ref, $link) = @_;
        my $type = LWP::MediaTypes::guess_media_type($link);
        push(@links, $link) if (uc($ref) eq 'HREF' && $type =~ /^text\/.*html$/);
        push(@imgs, $link) if (uc($ref) eq 'SRC' && $type =~ /^image\//);
    }, $url);
    $p->parse($txt);
    return ($txt, [@imgs], [@links]); # Don't return references to arrays
                                      # but copy them
}

sub catfile {
    require File::Spec;
    File::Spec->catfile(@_);
}

sub devnull {
    require File::Spec;
    File::Spec->devnull;
}

sub mkfile {
    my ($fn, $txt) = @_;
    open(OF, ">$fn");
    print OF $txt;
    close(OF);
}

sub filesig {
    my $fn = shift;
    my @s = stat($fn);
    splice @s, 8;
    join(":", @s);
}

sub new_table {
    require HTML::HTPL::Table;
    return new HTML::HTPL::Table(@_);
}

sub new_template {
    require HTML::HTPL::Template;
    return new HTML::HTPL::Template(@_);
}

sub new_select {
    require HTML::HTPL::Select;
    return new HTML::HTPL::Select(@_);
}

sub pusht {
    push(@__htpl_timer, time);
}

sub popt {
    time - (pop @__htpl_timer);
}

sub timestep {
    $started ||= $HTML::HTPL::Sys::started_time || &HTML::HTPL::Sys::mytime;
    my $t = &HTML::HTPL::Sys::mytimesince($started);
    $started = &mytime;
    $t;
}

sub elapsed {
    &HTML::HTPL::Sys::mytimesince($HTML::HTPL::Sys::started_time);
}

sub undouble {
    my %hash = ();
    @hash{@_} = map {1;} @_;
    sort keys %hash;
}

sub uniq {
    my (@r, %h);
    foreach (@_) {
        next if ($h{$_});
        push(@r, $_);
        $h{$_} = 1;
    }
    @r;
}

sub rotate {
    my ($offset, @list) = @_;
    if ($offset > 0) {
        my @these = splice(@list, 0, $offset);
        return (@list, @these);
    }
    if ($offset < 0) {
        my @these = splice(@list, $offset, 0);
        return ((reverse @these), @list);
    }
    @list;
}

sub rol {
    $_ = shift;
    (@_, $_);
}

sub ror {
    $_ = pop;
    ($_, @_);
}

sub getcwd {
    require Cwd;
    &Cwd::getcwd();
}

sub hostname {
    require Sys::Hostname;
    Sys::Hostname::hostname;
}

sub querystring {
    my %hash = @_;
    my $result;
    my ($k, $v, $a);
    my @vars;
    while (($k, $v) = each %hash) {
        $a = ((UNIVERSAL::isa($v, 'ARRAY')) ? $v : [$v]);
        foreach (@$a) {
            push(@vars, &urlencode($k) . '=' . &urlencode($_));
        }
    }
    join("&", @vars);
}

sub selfurl {
    my $query = &querystring(@_);
    $query =~ s/^(.)/?$1/;
    &HTML::HTPL::Sys::getvar("SELF_URL") . $query;
}

sub selfsameurl {
    my %hash = &HTML::HTPL::Sys::gethash("url");
    &selfurl(%hash, @_);
}

sub subhash {
    my ($t, $delim, @hash) = @_;
    my (%hash, %tags);
    if (UNIVERSAL::isa($hash[0], 'HASH')) {
        %hash = %{shift @hash};
        %tags = @hash;
    } else {
        %hash = @hash;
    }
    local ($text, $buff);
    if ($tags{'out'}) {
        *text = $tags{'out'};
    } else {
        *text = \$buff;
    }
    $text = $t;
    my %h2;
    @h2{(map {lc($_);} (keys %hash))} = (values %hash);
    %hash = %h2;
    my $result;
    my $delim2 = quotemeta($delim);
    while (1) {
        unless ($text =~ s/^(.*?)$delim2//sm) {
            return "$result$text";
        }
        $result .= $1;
        unless ($text =~ s/^(.*?)$delim2//sm) {
           return "$result$text";
        }
        my $cmd = $1;
        if ($cmd eq '') {
            $result .= $delim;
            next;
        }
        unless ($cmd =~ s/^\?//) {
            $result .= $hash{lc($cmd)};
            next;
        }
        if ($cmd =~ /^foreach\s+(.+)$/i) {
            my $ary = $hash{lc($1)};
            my $save = $text;
            foreach (@$ary) {
                $result .= &subhash($save, $delim, $_);
            }
            my $dummy = &subhash($save, $delim, {}, 'out' => \$text);
            next;
        }
        if ($cmd =~ /^if\s+(.*)$/i) {
            my $fun = &HTML::HTPL::Sys::compileutil($1);
            my $block = &subhash($text, $delim, \%hash, 'out' => \$text);
            $result .= $block if (&$fun(\%hash));
            next;
        }
        if ($cmd =~ /^end$/i) {
           return $result;
        }
        if ($cmd =~ /^self(\s+.*)?$/i) {
            my $par = &trim($1);
            my %hash = &HTML::HTPL::Sys::gethash("url");
            foreach (split(/ /, $par)) {
                delete $hash{$_};
            }
            $result .= &selfurl(%hash);
            next;
        }
        if ($cmd =~ /^esc(ape)?\s+(.*)$/i) {
            $result .= &urlencode($hash{lc($2)});
            next;
        }
        &htdie("Invalid template: $cmd");
    }
}

sub subpkg {
    my ($text, $delim, $pkg) = @_;
    $text =~ s/$delim(.*?)$delim/$1 ? ${"${pkg}::$1"} : $delim/ge;
    $text;
}

sub html_treeview {
    my ($tree, $param) = @_;
    my %hash = &HTML::HTPL::Sys::gethash('url');
    my $init = $hash{$param};
    my @open = split(/\$/, $init);
    my %open;
    @open{@open} = @open;
    my $result;
    foreach (@$tree) {
        my ($key, $html, $hash, $children) = @$_;
        my $this;
        if (UNIVERSAL::isa($hash, 'SCALAR')) {
            $hash = {$$hash => $key};
            $html = qq!<A HREF="#SELF#">$html</A>!;
        }
        if (UNIVERSAL::isa($hash, 'HASH')) {
            $this = &selfsameurl(%$hash);
        }            
        $html =~ s/#SELF#/$this/g;
        my $state = $open{$key};
        if (!@$children) {
            $result .= qq!<LI> $html\n!;
            next;
        }
        unless ($state) {
            my @new = sort (@open, $key);
            my $item = join('$', @new);
            my $ref = &selfsameurl($param => $item);
            $result .= qq!<LI> <B><A HREF="$ref">+</A></B> $html\n!;
            next;
        }
        my %new = %open;
        delete $new{$key};
        my @new = sort keys %new;
        my $item = join('$', @new);
        my $ref = &selfsameurl($param => $item);
        $result .= qq!<LI> <B><A HREF="$ref">-</A></B> $html\n!;
        $result .= &html_treeview($children, $param, 1);
    }
    $result = "<UL>$result</UL>";
    print $result unless defined wantarray;
    $result;
}

sub encrypt {
    my ($block, $cipher) = @_;
    my $len = length($block);
    my $round = (int($len / 8) + (1 - int((8 - $len % 8) / 8))) * 8;
    $block = substr($block . ("\0" x 8), 0, $round);
    my $result;
    foreach ($block =~ /(.{8})/gs) {
        $result .= $cipher->encrypt($_);
    }
    return pack("w", $len) . $result;
}

sub decrypt {
    my ($block, $cipher) = @_;
    my $len = unpack("w", $block);
    my $rev = quotemeta(pack("w", $len));
    return undef unless ($block =~ s/^$rev//);
    my $round = length($block);
    return undef if ($round % 8);
    my $result;

    @chunks = ($block =~ /(.{8})g/s);
    foreach ($block =~ /(.{8})/gs) {
        $result .= $cipher->decrypt($_);
    }
    return substr($result, 0, $len);
}

sub begintransaction {
    my $hnd = "HTPL_TR-" . scalar(@htpl_transactions);

    push(@htpl_transactions, {'old' => select, 'hnd' => $hnd});
    pipe(*{"R$hnd"}, *{"W$hnd"});
    select *{"W$hnd"};
    $| = 1;
}

sub endtransaction {
    my $item = pop @htpl_transactions;
    my $hnd = $item->{'hnd'};
    close *{"W$hnd"};
    my $rd = *{"R$hnd"};
    my @lines = <$rd>;
    close($rd);
    select *{$item->{'old'}};
    (wantarray ? @lines : join("", @lines));
}


1;
