package HTML::HTPL::Flat;

use HTML::HTPL::Result;
use Text::ParseWords;
use HTML::HTPL::Txt;
use HTML::HTPL::Lib;

@ISA = qw(HTML::HTPL::Txt);

sub openflat {
    local($filename, @fields) = @_;

    local ($phrase, @values);

    $hnd = "htpl_csv'FLAT" . ++$htpl_flat_handles;

    my $savedel = $/;
 
    &HTML::HTPL::Lib'opendoc($hnd, $filename);

    $orig = new HTML::HTPL::Flat($hnd, \@fields, "");
    $result = new HTML::HTPL::Result($orig, @fields);

    $result;
}

sub readln {
    my ($self, $line) = @_;
    my @values = split(/\r?\n/, $line);
    \@values;
}

1;
