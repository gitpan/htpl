package HTML::HTPL::Config;

require "/usr/local/apache/cgi-bin/htpl-config.pl";

require "./htpl-site.pl" if (-f "htpl-site.pl");
 
1;
