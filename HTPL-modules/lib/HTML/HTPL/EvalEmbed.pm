package HTML::HTPL::EvalEmbed;
use overload;


sub import {
        shift;
        eval {overload::constant('q' => \&mystr);};
}

sub unimport {
	shift;
	eval {overload::remove_constant('q');};
}

BEGIN {

eval <<'EOV';
sub mystr {
        my $s = shift;
	return $s unless ($_[1] eq 'qq');
	&unimport;
	eval {$s =~ s/(?<!\\)\[(.*?)\]/eval($1)/ge;};
	$s =~ s/\\(.)/eval(qq!"\\$1"!)/ge;
	&import;
	$s;
}
EOV
}



1;
