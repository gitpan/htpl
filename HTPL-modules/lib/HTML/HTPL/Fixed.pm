package HTML::HTPL::Fixed;

use HTML::HTPL::Result;
use HTML::HTPL::Txt;

@ISA = qw(HTML::HTPL::Txt);

sub openfixed {
    local($filename, @fields) = @_;

    my (@cols, @heads);
    foreach (@fields) {
        my ($name, $len) = split(/:/, $_);
        push(@cols, $name);
        push(@heads, $len);
    }

    my $hnd = "htpl_fixed'Fixed" . ++$htpl_fixed_handles;

    &HTML::HTPL::Lib'opendoc($hnd, $filename);

    $orig = new HTML::HTPL::Fixed($hnd, \@heads);
    $result = new HTML::HTPL::Result($orig, @cols);


    $result;
}

sub readln {
    my ($self, $line) = @_;
    my $re = $self->{'re'};
    my @values = ($line =~ /$re/);
    my @dummy = map {s/\s+$//;} @values;
    \@values;
}

sub new {
    my ($class, $hnd, $lens) = @_;
    my $self = HTML::HTPL::Txt::new($class, $hnd, $fields, "\n");
    my $re = join("", map {"(" . ("." x $_) . ")";} @$lens);
    $self->{'re'} = $re;
    $self;
}

1;
