package HTML::HTPL;

BEGIN{
    $VERSION = 2.96;
}

sub import {
    require HTML::HTPL::Lib;
    require HTML::HTPL::Sys;
    import HTML::HTPL::Sys;
    import HTML::HTPL::Lib;
}


1;
__END__
=head1 Soon
Documentaiton soon
