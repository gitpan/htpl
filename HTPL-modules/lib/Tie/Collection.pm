package Tie::Collection;
use Tie::Cache;
@ISA = qw(Tie::Cache);

sub TIEHASH {
    my ($class, $storage, $ref, $bless) = @_;
    my $this = Tie::Cache::TIEHASH($class, $ref);
    $this->{'Storage'} = $storage;
    $this->{'Bless'} = $bless;
    $this;
}

sub read {
    my ($self, $key) = @_;
    my $s = $self->{'Storage'};
    my $el = $s->EXISTS($key) ? $s->FETCH($key) : undef;
    my $bless = $self->{'Bless'};
    if ($bless && $el) {
        bless $el, $bless;
        eval '$el->postload;';
    }
    $el;
}

sub write {
    my ($self, $key, $value) = @_;
    my $bless = $self->{'Bless'};
    eval '$value->prestore;' if ($bless && ref($value) =~ /$bless/);
    $self->{'Storage'}->STORE($key, $value);
}
