package HTML::HTPL;

BEGIN{
    $VERSION = 3.02;
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
