package HTML::HTPL::Parser;

$VERSION = 1.0;

sub new {
    my $class = shift;
    my $self = {};
    my %hash = map {(ref($_) ? $_ : lc($_))}  @_;
    $self->{'lexer'} = $hash{'lexer'};
    $self->{'grammar'} = $hash{'grammar'};
    bless $self, $class;
}

sub lexer {
    my ($self, $l) = @_;
    $self->{'lexer'} = $l;
}

sub grammar {
    my ($self, $g) = @_;
    $self->{'grammar'} = $g;
}

sub parse {
    my $self = shift;
    my ($start, @ts) = @_;
    my ($opt);

    my $yy = $self->{'grammar'};

    if ($start !~ /^\$/) {
        if (lc($ts[0]->[0]) eq lc($start)) {
            return ((shift @ts)->[0], @ts);
        }
    }

    $oldstart = $start;
    $start =~ s/^\$//;
    local ($optref) = $yy->{$start};

    if ($self->{'isterm'}->{$start}) {
        my $pair = shift @ts;
        return (undef, ()) unless ($pair);
        my ($item, $type) = @$pair;
        return ([$start, $item], @ts) if ($type eq $start || !$type);
        return (undef, $pair, @ts);
    }

    die "Unknown $start" unless ($optref);

    if ($start =~ /\*$/) {
        chop $oldstart;
        local (@these) = @ts;
        local (@r, @left, $t) = ();
        do {
            ($t, @left)= $self->parse($oldstart, @these, $card);
            if ($t) {
                @these = @left;
                push(@r, $t);
            }
        } while ($t);
        if (@r) {
            my @ret = ($start, @r);
            return (\@ret, @these);
        }
        return (undef, @ts);
    }


    foreach $opt (@$optref) {
        local (@cons) = split(/\s+/, $opt);
        local (@l);
        local ($c);
        local (@these) = @ts;
        goto none if ($#cons > $#ts);
        print "y: Matching " . join(" ", map {$_->[0]} @ts) . " against "
                   . join(" ", @cons) ."\n" if ($DEBUG =~ /y/i);
        foreach $c (@cons) {
            
            unless ($c =~ /^\$/) {
                if ($c eq $these[0]->[0]) {
                    shift @these;
                    push(@l, $c);
                    next;
                }
                goto none;
            }
            local ($t, @left) = $self->parse($c, @these);
            $cr += $#these - $#left;
            goto none unless ($t);
            push(@l, $t);
            @these = @left;
        }
        my @ret = ($start, \@l);
        if ($DEBUG =~ /e(\d+)/i) {
            &outit(\@ret, $1) ;
        }
        return (\@ret, @these);
none:
    }
    return (undef, @ts);
}

sub out {
    local ($i) = @_;
   
    unless (ref($i)) {
        return ($i, 1);
    }

    local ($c = 0);

    return ("[" . join(" ", map {my @p = out($_); $c += $p[1]; $p[0];
             } @$i) . "]", $c);
}

sub parser {
    my $self = shift;
    my $str = shift;
    my $start = shift || $self->{'grammar'}->{'start'};
    $self->init;
    my @ts = $self->tokenizer($str);
    return undef unless (@ts);
    my ($ref, @left) = $self->parse($start, @ts);
    return undef unless ($ref);
    return ($ref, @left);
}

sub tokenizer {
    my ($self, $str) = @_;
    my $lexer = $self->{'lexer'};
    my @result, $k, $t;
    while ($str) {
        $str =~ s/^\s+//;
        $str =~ s/\s+$//;
        next unless ($str);
        my $flag;
        my @l = @$lexer;
        while (@l) {
            ($k, $t) = splice(@l, 0, 2);
            print "l: Matching $str against $t\n" if($DEBUG =~ /l/);
            my $x = ($str =~ s/^($t)//);
            if ($x) {
                push(@result, [$1, $k]);
                $flag = 1;
                last;
            }
        }
        return undef unless ($flag);
    }
    return @result;
}

sub init {
    my ($self, $w) = @_;
    my $lexer = $self->{'lexer'};
    my @terminals = @$lexer;
    my $isterm = {};
    while (@terminals) {
        $isterm->{shift @terminals} = 1;
        shift @terminals;
    }
    $self->{'isterm'} = $isterm;
}

sub outit {
    my ($ref, $min) = @_;
    my ($str, $c) = &out($ref);
    print "$str\n" if ($c >= $min);
}

1;
