package Locale::Hebrew::Calendar;

use strict;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK);

require Exporter;
require DynaLoader;
require AutoLoader;

@ISA = qw(Exporter DynaLoader);
# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.
@EXPORT = ();
@EXPORT_OK = qw(
g2j j2g	
);
$VERSION = '0.01';

bootstrap Locale::Hebrew::Calendar $VERSION;

# Preloaded methods go here.

sub g2j {
    @{_g2j(shift, shift, shift)};
}

sub j2g {
    @{_j2g(shift, shift, shift)};
}

# Autoload methods go after =cut, and are processed by the autosplit program.

1;
__END__

#

=head1 NAME

Locale::Hebrew::Calendar - Jewish Calendar.

=head1 SYNOPSIS

use Locale::Hebrew::Calendar;

($d, $m, $y) = Locale::Hebrew::Calendar::g2j($dd, $mm, $yy); # Gregorian
to Jewish

($d, $m, $y) = Locale::Hebrew::Calendar::j2g($dd, $mm, $yy); # Jewish to
Gregorian

=head1 DESCRIPTION

This is an XSUB interface to a code which can be found on seveal main FTP
servers. I have never contacted the author, but "He who says things in the
name of their originators brings redemption to the world" - The actual
code was written by Amos Shapir. (Email unknown).

=head1 AUTHOR

Amos Shapir (email unknown).
XSUB by Ariel Brosh, schop@cpan.org.

=head1 COPYRIGHT

Tie::Collection is part of the HTPL package. See L<HTML::HTPL>
 
=cut
