package HTML::HTPL::Config;

###
# Created by configure

$mailprog = "/usr/sbin/sendmail";
$ccprog = "/usr/bin/gcc";
$dbgbin = "/usr/local/apache/cgi-bin/htpldbg";
$temp = "/tmp";

###
# Uncomment to allow persistency

# $htpl_persistent = 1;
# $htpl_db_file = "/usr/local/apache/htdocs/htpl/htpl.db";
# $htpl_per_session_idle_time = 3600;
# $htpl_persist_cachesize = 4;

###
# Uncomment to use cookies

# $htpl_use_cookies = 1;
# $htpl_cookie = "htpl-ck-id";

###
# Uncomment if your web site contains forms in hebrew

# $htpl_flip_hebrew = 1;

###
# Uncomment to have logs redirected to a single file. Default is
# Script name + ".log"

# $htpl_default_log_file = "put filename here";

###
# Security and system logs.
$htpl_system_log = "/usr/local/apache/logs/htpl_log";

###
# Put configuration for directories in htpl-site.pl in directories
# Override variables for these directories

require "./htpl-site.pl" if (-f "htpl-site.pl");


1;
