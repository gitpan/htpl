package HTML::HTPL::Config;

eval <<'EOM';
	require "/home/httpd/cgi-bin/htpl-config.pl";

        my $reg = "./htpl-site.pl";
        my $alt = "../htpl-site.pl";
        $reg = $alt if ((stat("."))[1] == (stat("../htpl-cache"))[1]);
        require $reg if (-f $reg);
 
EOM

1;
