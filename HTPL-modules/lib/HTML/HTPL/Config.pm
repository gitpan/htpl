package HTML::HTPL::Config;

eval {
	require "/home/httpd/cgi-bin/htpl-config.pl";

	require "./htpl-site.pl" if (-f "htpl-site.pl");
 
};

1;
