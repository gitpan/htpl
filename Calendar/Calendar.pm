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
