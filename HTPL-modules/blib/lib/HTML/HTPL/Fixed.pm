package HTML::HTPL::Fixed;

use HTML::HTPL::Lib;
use HTML::HTPL::Result;
use HTML::HTPL::Txt;

@ISA = qw(HTML::HTPL::Txt);

sub openfixed {
    local($filename, @fields) = @_;

    my $sub;
    if (UNIVERSAL::isa($fields[0], 'SCALAR')) {
        $sub = ${shift @fields};
    }
    my $class = 'HTML::HTPL::Fixed' . ($sub ? "::$sub" : "");
    my (@cols, @heads);
    foreach (@fields) {
        my ($name, $len) = split(/:/, $_);
        push(@cols, $name);
        push(@heads, $len);
    }

    my $hnd = "htpl_fixed'Fixed" . ++$htpl_fixed_handles;

    &HTML::HTPL::Lib'opendoc($hnd, $filename);

    $orig = $class->new($hnd, \@heads);
    $result = new HTML::HTPL::Result($orig, @cols);


    $result;
}

sub readln {
    my ($self, $line) = @_;
    my $re = $self->{'re'};
    my @values = ($line =~ /$re/);
    my @dummy = map {s/[\0\s]+$//;} @values;
    \@values;
}

sub new {
    my ($class, $hnd, $lens) = @_;
    my $self = HTML::HTPL::Txt::new($class, $hnd, $fields, "\n");
    my $re = join("", map {"(" . ("." x $_) . ")";} @$lens);
    $self->{'re'} = $re;
    $self->{'len'} = &HTML::HTPL::Lib::sum(@$lens);
    $self;
}

package HTML::HTPL::Fixed::IBM;
@ISA = qw(HTML::HTPL::Fixed);

sub realread {
    my ($self, $hnd) = @_;
    my $line;
    my $len = $self->{'len'};
    return undef unless (read($hnd, $line, $len) > $len / 2);
    $line;
}

1;
