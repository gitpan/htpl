package HTML::HTPL::Txt;

use HTML::HTPL::Orig;
use HTML::HTPL::Lib;
@ISA = qw(HTML::HTPL::Orig);

sub new {
    my $class = shift;
    my @copy = @_;
    bless {'params' => \@copy}, $class;
}

sub realfetch {
    $self = shift;

    return undef if ($self->{'closed'});

    ($hnd, $fields, $linedel) = @{$self->{'params'}};

    my $savedel = $/;
    $/ = $linedel;
    my $l = <$hnd>;
    return undef unless ($l);
    chomp $l;
    my $retval = $self->readln($l);
    $/ = $savedel;

    if (eof($hnd)) {
        &HTML::HTPL::Lib'closedoc($hnd);
        $self->{'closed'} = 1;
    }

    $retval;
}

1;
