use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'NAME'	=> 'Locale::Hebrew::Calendar',
    'VERSION_FROM' => 'Calendar.pm', # finds $VERSION
    'LIBS'	=> [],   # e.g., '-lm' 
    'DEFINE'	=> '',     # e.g., '-DHAVE_SOMETHING' 
    'INC'	=> '',     # e.g., '-I/usr/include/other' 
    'OBJECT'    => 'Calendar.o hdate.o hcom.o',
    'FUNCLIST'  => 'g2j j2g'
);
