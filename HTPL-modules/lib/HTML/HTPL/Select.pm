package HTML::HTPL::Select;
use HTML::HTPL::Lib;

sub new {
    my $class = shift;
    my $self = {'name' => shift, @_, 'rows' => []};
    bless $self, $class;
}

sub add {
    my ($self, $key, $value) = @_;
    $value ||= $key;
    push(@{$self->{'rows'}}, [$key, $value]);
}

sub add_default {
    my ($self, $key, $value) = @_;
    $value ||= $key;
    push(@{$self->{'rows'}}, [$key, $value]);
    $self->set_default($key);
}

sub set_default {
    my ($self, $key) = @_;
    $self->{'default'} = $key;
}

sub as_html {
    my $self = shift;
    my $attr;
    my $rows = $self->{'rows'};
    $attr = " MULTIPLE SIZE=$rows" if ($rows > 1);
    my $name = $self->{'name'};
    my $default = $self->{'default'};
    my $hash = {'name' => $name, $attr => $attr, 'noout' => 1};
    $hash->{'default'} = $default if ($default);
    my @elems;
    foreach (@{$self->{'rows'}}) {
        push(@elems, @$_);
    }
    HTML::HTPL::Lib::html_selectbox($hash, @elems);
}
