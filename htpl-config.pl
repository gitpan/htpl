package HTML::HTPL::Config;

###
# Created by configure

$mailprog = "/usr/sbin/sendmail";
$ccprog = "/usr/bin/gcc";
$dbgbin = "/usr/home/schop/public_html/cgi-bin/htpldbg";
$pcbin = "/usr/home/schop/public_html/cgi-bin/htplp";
$temp = "/tmp";

###
# Uncomment to allow persistency

# $htpl_persistent = 1;
# $htpl_db_file = "/usr/home/schop/public_html/htpl/htpl.db";
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
$htpl_system_log = "/usr/home/schop/public_html/.htpl.log";

###
# Locale. Currently supported via the DoW and MoY arrays
$htpl_language = "English"; # Norwegian, Austrian, English, German,
                            # Czech, French, Italian

####
# Comment to disable persistent DB connections under mod_perl
$htpl_db_save = 1;

####
# HTPL PTS port
$htpl_pts_port = 4002;

###
# Put configuration for directories in htpl-site.pl in directories
# Override variables for these directories

require "./htpl-site.pl" if (-f "htpl-site.pl");


1;
