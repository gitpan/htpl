use Text::Wrap;

use Parse::Lex;
use htplparse;
use Data::Dumper;

$o = join("|", map {quotemeta($_);}
      qw(|| | && & <> < > == = != ! ~));

@token = (qw(
  AND \&\&?|\b[Aa][Nn][Dd]\b
  OR \|\|?|\b[Oo][Rr]\b
  NOT [!~]|\b[Nn][Oo][Tt]
  EQUAL \=\=?
  NOTEQUAL \!\=|\<\>
  OP [><()]
  STRING \w*(%.*?%\w*)+|".*?"|'.*?'), sub {my $s = $_[1]; ("" =~ /^()$/);
                   $s =~ s/^"(.*)"$/$1/; $s =~ s/^'(.*)'$/$1/ unless($1);
                                $s;}, qw(
  ERROR .*) , sub {
                   die qq!can\'t analyze: "$_[1]"!;
                 }
 );
  
$lexer = new Parse::Lex(@token);
$lexer->skip('\s+');

sub lexer {
    my $token = $lexer->next;
    return ('', undef) if ($token->eoi || $token->name eq 'EOI');
    return ($token->name, $token->text);
}

$parser = new htplparse;


sub code2c {
    my $code = shift;
    $lexer->from($code);
    my $st = $parser->YYParse(yylex=>\&lexer);
    my $c = &formit($st);
    $c;
}

sub formit {
    my $node = shift;
    if (ref($node) !~ /ARRAY/)  {
        return &expandstr($node);
    }
    my @ary = @$node;
    if (@ary == 1) {
        my $s = &expandstr($ary[0]);
        return qq!disposetrue($s)!;
    }
    if (@ary == 2) {
        my $s = &formit($ary[1]);
        return "!($s)"
    }
    if (@ary == 3) {
        my $l = &formit($ary[1]);
        my $r = &formit($ary[2]);
        my $o = $ary[0];
        return "($l $o $r)";
    }
    if (@ary == 4) {
        my $l = &expandstr($ary[2]);
        my $r = &expandstr($ary[3]);
        my $o = $ary[1];
        my $s = qq!(disposecmp($l, $r) $o 0)!;
    }
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


sub wrapcode {
    $_ = shift;
    s/\0/ /g;
return $_;
    eval {
        $_ = &Text::Wrap::wrap('', '        ', split(/\s+/, $_));
    };
    $_;
}
 
1;
