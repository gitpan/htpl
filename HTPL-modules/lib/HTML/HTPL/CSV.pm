package HTML::HTPL::CSV;

use HTML::HTPL::Result;
use Text::ParseWords;
use HTML::HTPL::Txt;

@ISA = qw(HTML::HTPL::Txt);

sub opencsv {
    local($filename, $delimiter, @fields) = @_;

    local ($phrase, @values);

    local ($rowdel, $coldel, $savedel, $chop);


    if (UNIVERSAL::isa($delimiter, 'ARRAY')) {
        ($coldel, $rowdel) = @$delimiter;
        $delimiter = $coldel;
    }

    $delimiter = ' ' if ($delimiter eq 'BLANK');
    $delimiter =~ s/^'(.*)'$/$1/;
    $delimiter = quotemeta($delimiter);

    $hnd = "htpl_csv'CSV" . ++$htpl_csv_handles;

    my $savedel = $/;
    $/ = $rowdel;
 
    &HTML::HTPL::Lib'opendoc($hnd, $filename);

    unless (@fields) {
        <$hnd>;
        @fields = &Text::ParseWords::parse_line($delimiter, undef, $_);
    }

    $/ = $savedel;

    $orig = new HTML::HTPL::CSV($hnd, \@fields, $delimiter, $/, $chop);
    $result = new HTML::HTPL::Result($orig, @fields);


    $result;
}

sub readln {
    my ($self, $line) = @_;
    my $delimiter = $self->{'delimiter'};
    my @values = &Text::ParseWords::parse_line($delimiter, undef, $line);
    \@values;
}

sub new {
    my ($class, $hnd, $fields, $delimiter, $linedel) = @_;
    my $self = HTML::HTPL::Txt::new($class, $hnd, $fields, $linedel);
    $self->{'delimiter'} = $delimiter;
    $self;
}

1;
