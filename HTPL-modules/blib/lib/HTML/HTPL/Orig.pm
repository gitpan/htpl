package HTML::HTPL::Orig;

sub eof {
    my $self = shift;
    return undef if ($self->{'buffer'});
    ($self->{'buffer'} = $self->realfetch) ? undef : 1;
}

sub fetch {
    my $self = shift;
    my $ret = $self->{'buffer'};
    if ($ret) {
        delete $self->{'buffer'};
        return $ret;
    }
    return $self->realfetch;
}

1;
