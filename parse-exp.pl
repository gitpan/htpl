use Text::Wrap;
use lib "./HTPL-modules/lib";
use Parser::TopDown;

$Text::Wrap::columns = 80;

$y = {
'start' => '$expression',
'expression' => ['$disj', '$token'],
'token' => ['$conj', '$neg', '$value'],
'value' => ['$paran', '$comp', '$atom'],
'atom' => ['$string'],
'comp' => ['$string $ratio $string'],
'ratio' => ['>', '<', '$notequal', '$equal'],
'equal' => ['==', '='],
'notequal' => ['!=', '<>'],
'paran' => ['( $expression )'],
'conj' => ['$value $and $token'],
'disj' => ['$token $or $expression'],
'neg' => ['$not $token'],
'and' => ['and', '&&', '&'],
'or' => ['or', '||', '|'],
'not' => ['not', '!', '~']
};	

$o = join("|", map {s/(.)/\\$1/g; $_;} 
      qw(|| | && & <> < > == = != ! ~));

$l = [
'pp', '[()]',
'op', $o,
'string', '(%.*?%|\w+)+|".*?"'
];

$HTML::HTPL::Parser::DEBUG = "";

sub code2c {
    my $s = shift;
    $p = new HTML::HTPL::Parser;
    $p->lexer($l);
    $p->grammar($y);
    ($ref) = $p->parser($s);
    die "Parser failed for $s" unless ($ref);
    &formit($ref);
}

sub wrapcode {
    $_ = shift;
    eval {
        $_ = &Text::Wrap::wrap('', '        ', split(/\s+/, $_));
    };
    s/\0/ /g;
    $_;
}

sub formit {
    my $it = shift;

    my $op = $it->[0];
    my $par = $it->[1];
    $par = [$par] unless (ref($par));

    if ($op eq 'string') {
        my $s = $par->[0];
        $s =~ s/^"(.*)"$/$1/;
        my $c = &assemble($s);
        return "(STR)mysprintf($c)";
    }

    if ($op eq 'atom') {
        my $s = &formit($par->[0]);
        return qq!(disposetrue($s))!;
    }
    if ($op eq 'comp') {
        my $s1 = &formit($par->[0]);
        my $s2 = &formit($par->[2]);
        my $rop = $par->[1]->[1]->[0];
        my $s = qq!disposecmp(populate("$s1", stack), populate("$s2", stack))!;
        return "!$s" if ($rop->[0] eq 'equal');
        return "$s" if ($rop->[0] eq 'notequal');
        return "($s > 0)" if ($rop eq '>');
        return "($s < 0)" if ($rop eq '<');
        die "Unknown ratio $rop " . $rop->[0];
    }

    if ($op eq 'paran') {
        return "(" . &formit($par->[0]) . ")";
    }

    
    if ($op eq 'neg') {
        return "!" . &formit($par->[1]);
    }

    if ($op eq 'conj') {
        my $s1 = &formit($par->[0]);
        my $s2 = &formit($par->[2]);
        return "$s1 && $s2";
    }

    if ($op eq 'disj') {
        my $s1 = &formit($par->[0]);
        my $s2 = &formit($par->[2]);
        return "$s1 || $s2";
    }

    return &formit($par->[0]) if ($op eq 'value' || $op eq 'expression' || 
      $op eq 'token'); 

    die "Unresolved symbol $op";
}

sub assemble {
    my $str = shift;
    my @ary;
    $str =~ s/\s/\0/g;
    $str =~ s/\%([^\%\0]*?)\%/&proctoken($1, \@ary)/gei;
    $main'glob_ary = @ary;
    return join(", ", qq!"$str"!, @ary);
}

sub proctoken {
    my ($token, $aryref) = @_;
    return "%%" if (!$token);
    if ($token =~ /^\d+$/) {
        push(@$aryref, "gettoken($token)");
    } elsif ($token =~ /^(\d+)\*$/) {
        push(@$aryref, qq!gettokenlist($1, " ", "")!);
    } elsif ($token =~ /^(\d+)\!$/) {
        push(@$aryref, qq!gettokenlist($1, ", ", "\$")!);
    } elsif ($token =~ /^(\d+)(.*?)\*$/) {
        push(@$aryref, qq!gettokenlist($1, "$2", "")!);
    } elsif ($token =~ /^(\d+)(.)(\d+)$/) {
        push(@$aryref, "getsubtoken($1, '$2', $3)");
    } elsif ($token eq "?") {
        push(@$aryref, "numtokens");
        return "%d";
    } elsif ($token =~ /^\$(.*)$/) {
        push(@$aryref, qq!getvar("$1")!);
    } elsif ($token eq 'id') {
        push(@$aryref, qq!getblockid("")!);
    } elsif ($token =~ /^\@(.*)$/) {
        push(@$aryref, qq!getblockid("$1")!);
    } elsif ($token eq 'key') {
        push(@$aryref, "getkey()");
    } elsif ($token eq 'now') {
        push(@$aryref, "convtime(time(NULL))");
    } elsif ($token eq 'random') {
        push(@$aryref, "random()");
        return "%d";
    } elsif ($token eq 'page') {
        push(@$aryref, "thefilename");
    } elsif ($token eq 'script') {
        push(@$aryref, "thescript");
    } elsif ($token eq 'line') {
        push(@$aryref, "nline");
        return "%d";
    } elsif ($token eq 'rline') {
        push(@$aryref, "rline + currbuffer->lines + 1");
        return "%d";
    } elsif ($token eq 'rlineplus1') {
        push(@$aryref, "rline + currbuffer->lines + 1 + 1");
        return "%d";
    } else {
        die "Unknown token $token";
    }
    return "%s";
}

sub expandstr {
    my $c = &assemble(shift);
    $c =~ s/\0/ /g;
    return "(STR)mysprintf($c)";
}

1;
