($fn, $cgi, $sysdir) = @ARGV;

if ( -f $fn ) {
open(I, $fn);
while (<I>) {
    exit if (/application\/x-htpl/);
}
close(I);
} 

open(O, ">>$fn");
print O <<EOC;

AddType application/x-htpl .htpl
Action application/x-htpl /htpl-code-exec/
ScriptAlias /htpl-code-exec/ $cgi/

<Directory $sysdir>
AuthType Basic
AuthName "HTPL site administration"
AuthUserFile $sysdir/.passwd
AuthGroupFile /dev/null

<Limit GET POST>
require valid-user
</Limit>
</Directory>

EOC
close(O);

eval 'mkdir $sysdir, 0777;';
open(O, ">>$sysdir/.passwd");
print O "admin:" . crypt("admin", "admin");
close(O);

@ps = `ps -ax`;
@pps = grep /httpd/, @ps;
foreach (@pps) {
    s/^\s+//;
    @items = split(/\s/);
    push(@pids, $items[0]);
}

if (@pids && !$<) {
print "\n\nKilling -HUP processes " . join(", ", @pids) . "\n";

eval 'kill -1, @pids;';
}

print <<EOM;


---------------------------------------------------------------------
---------------------------------------------------------------------
You can login to http://yoursite/htpl (or any other directory
you chose) with the user admin and password admin
---------------------------------------------------------------------
---------------------------------------------------------------------


EOM
