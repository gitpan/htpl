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
# Below is the stub of documentation for your module. You better edit it!

=head1 NAME

Locale::Hebrew::Calendar - Perl extension for blah blah blah

=head1 SYNOPSIS

  use Locale::Hebrew::Calendar;
  blah blah blah

=head1 DESCRIPTION

Stub documentation for Locale::Hebrew::Calendar was created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

Blah blah blah.

=head1 AUTHOR

A. U. Thor, a.u.thor@a.galaxy.far.far.away

=head1 SEE ALSO

perl(1).

=cut
