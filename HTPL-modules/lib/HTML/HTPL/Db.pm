package HTML::HTPL::Db;

use DBI;
use HTML::HTPL::SQL;
use HTML::HTPL::Result;
use HTML::HTPL::Sys qw(getvar htdie);
use HTML::HTPL::Lib;

sub dbdie {
    my $par = shift;
    my $err = $DBI::errstr || $@;
    &HTML::HTPL::Lib::takebroadlog("$par failed: $err");
    &HTML::HTPL::Sys::htdie("Database error. Please report administrator: "
      . $ENV{'SERVER_ADMIN'});
}

sub zap {
    %HTML::HTPL::Sys::db_pool = ();
    %HTML::HTPL::Sys::query_pool = ();
}

sub new {
    my ($class, $dsn, @extra) = @_;

    my $dbh = $HTML::HTPL::Sys::db_pool{$dsn, @extra};
    unless (ref($dbh) eq "DBI::db" && $dbh->{Active}) {
        eval '$dbh = DBI->connect($dsn, @extra);';
        &dbdie("Connection to $dsn") unless ($dbh);
        $HTML::HTPL::Sys::db_pool{$dsn, @extra} = $dbh;
    }
    $self = {'dbh' => $dbh};

    bless $self, $class;
}

sub execsql {
    my ($self, $script, @values) = @_;
    $dbh = $self->{'dbh'};

    $dbh->do($script, undef, @values) || &dbdie(qq!SQL "$script"!);
}

sub add {
    my ($self, $table, @fields) = @_;

    my $dbh = $self->{'dbh'};
    
    my @qs = ();
 
    my $key;
    my @values = ();

    unless (@fields) {
        my $sth = $dbh->prepare("SELECT * FROM $table WHERE 2 = 3");
        $sth->execute;
        @fields = @{$sth->{NAME}};
    }

    foreach $key (@fields) {
        push(@qs, "?");
        push(@values, &getvar($key));
    }
    my $sql = "INSERT INTO $table (" . join(", ", @fields) .
         ") VALUES (" . join(", ", @qs) . ")";
    my $sth = $dbh->prepare($sql) || &dbdie(qq!SQL "$sql"!);
    $sth->execute(@values) || &dbdie(qq!SQL "$sql"!);
}

sub update {
    my ($self, $table, @pars) = @_;

    my @parts = split(/ WHERE /, join(" ", @pars));
    my @fields = split(/ /, $parts[0]);
    my @conds = split(/ /, $parts[1]);

    my $dbh = $self->{'dbh'};

    my @qs = ();

    my @tokens = ();

    my $key, @values, @ws;

    foreach $key (@fields) {
        push(@qs, "$key = ?");
        push(@values, &getvar($key));
    } 

    foreach $key (@conds) {
        push(@values, &getvar($key));
        my $eq = '=';
        $eq = 'LIKE' if ($val =~ /[\%\#\!\*\?]/);
        push(@ws, "$key $eq ?");
    } 

    my $sql = "UPDATE $table SET " . join(", ", @qs) . " WHERE " .
	join(" AND ", @ws);
    my $sth = $dbh->prepare($sql) || &dbdie(qq!SQL "$sql"!);

    $sth->execute(@values) || &dbdie(qq!SQL "$sql"!);

}

sub DESTROY {
    my $self = shift;

    $dbh = $self->{'dbh'};

    $dbh->disconnect;
}

sub cursor {
    my ($self, $sql, @values) = @_;

    my $dbh = $self->{'dbh'};
    my $sth = $dbh->prepare($sql) || &dbdie(qq!SQL "$sql"!);

    return $sth && $sth->execute(@values) && &load($sth)
        || &dbdie(qq!SQL "$sql"!);

}

sub load {
    my ($sth) = @_;

    my $rows = $sth->rows;
    return new HTML::HTPL::Result(undef, @{$sth->{NAME}}) unless ($rows);

    my $hashref = $sth->fetchrow_hashref;

    @fields = keys %$hashref;

    my $orig = HTML::HTPL::Db::Orig->new($sth, @fields);
    my $result = new HTML::HTPL::Result($orig, @fields);
    $result->add($hashref);

    $result;
}

sub __add {
    my ($result, $hashref, @fields) = @_;
    my $key;
    my @ary = ();

    my %hash = %$hashref;

    foreach $key (@fields) {
        push(@ary, $hash{$key});
    }

    $result->addrow(@ary);
}

sub ___addcgi {
    my ($self, $table, $param) = @_;
    my @fields;
    if (!$param) {
        @fields = keys %{$HTML::HTPL::Lib'htpl_pkg ."'in"};
    } else {
        @fields = &seperate($param);
    }

    $self->add($table, @fields);
}

sub ___updatecgi {
    my ($self, $table, $param) = @_;
    my @fields, @conds, $strw, $strf;

    ($strf, $strw) = ($param =~ /^(.*) WHERE (.*)$/i);
    @fields = &seperate($strf);
    @conds = &seperate($strw);
    
    $self->update($table, \@fields, \@conds);
}

sub query {
    my ($self, $table, @conds) = @_;

    my $dbh = $self->{'dbh'};

    my @tokens = ();

    my @values = ();

    my $key, $where;

    my $sql = "";

    my @ws;

    foreach $key (@conds) {
        push(@values, &getvar($key));
        my $eq = '=';
        $eq = 'LIKE' if ($val =~ /[\%\#\!\*\?]/);
        push(@ws, "$key $eq ?");
    };

    if (@conds) {
        $sql = " WHERE " . join(" AND ", @ws);
    }

    my $sth = $dbh->prepare("SELECT * FROM $table" . $sql) || &dbdie(qq!SQL "$sql"!);

    return $sth->execute(@values) && &load($sth) || &dbdie(qq!SQL "$sql"!);

}

sub ___querycgi {
    my ($self, $table, $param) = @_;
    my @conds;

    @conds = &seperate($param);
    
    return $self->query($table, @conds);
}

sub parse_sql2 {
    my $sql = shift;
    my $tokens = &HTML::HTPL::SQL'tokenize_sql($sql);
    my @vars;
    my @result = map {if (/^:/) {
                s/^://; push(@vars, getvar($_, 1));"?";
            } else {
	        $_;
            } 
        } @$tokens;
    (join('', @result), @vars);
}

sub parse_sql {
    my @ary = &parse_sql2(@_);
    (shift @ary, map {$$_;} @ary);
}

sub delete {
    my ($self, $table, @conds) = @_;

    my $dbh = $self->{'dbh'};

    my @tokens = ();

    my @values = ();

    my $key, $where;

    my $sql = "";

    my @ws;

    foreach $key (@conds) {
        my $val = getval($key);
        $val = eval($val) if ($val =~ /^\d+$/);
        push(@values, $val);
        my $eq = '=';
        $eq = 'LIKE' if ($val =~ /[\%\#\!\*\?]/);
        push(@ws, "$key $eq ?");
    };

    if (@conds) {
        $sql = " WHERE " . join(" AND ", @ws);
    }

    my $script = "DELETE FROM $table$sql";
    my $sth = $dbh->prepare($script) || &dbdie(qq!SQL "$script"!);
    $sth->execute(@values) || &dbdie(qq!SQL "$script"!);
}


sub batch_insert {
    my ($self, $table, $src) = @_;
    my $dbh = $self->{'dbh'};
    my $sth = $dbh->prepare("SELECT * FROM $table WHERE 1 = 2");
    $sth->execute;
    my @fields = @{$sth->{'NAME'}};
    my $sql = "INSERT INTO $table (" . join(", ", @fields) . ") 
       VALUES (" . join(", ", map {"?"} @fields) . ")";
    $sth = $dbh->prepare($sql);
    &HTML::HTPL::Sys::pushvars(@fields);
    my $save = $src->index;
    $src->rewind;
    while ($src->fetch) {
        my @values = map {&getvar($_);} @fields;
        $sth->execute(@values);
    }
    $src->access($save);
    &HTML::HTPL::Sys::popvars;
}

sub prepare {
    my ($self, $cursor, $sql) = @_;
    my ($code, @vars) = &parse_sql2($sql);
    my $dbh = $self->{'dbh'};
    new HTML::HTPL::Db::Query($dbh, $sql, \@vars);
}

package HTML::HTPL::Db::Orig;

use HTML::HTPL::Orig;

@HTML::HTPL::Db::Orig::ISA = qw(HTML::HTPL::Orig);

use DBI;

sub new {
    my ($class, $sth, @fields) = @_;

    $self = {'sth' => $sth,
             'fields' => \@fields};
    bless $self, $class;
}

sub realfetch {
    $self = shift;
    $self->{'sth'}->fetchrow_hashref;
}

package HTML::HTPL::Db::Query;

sub new {
    my ($class, $dbh, $sql, $vars) = @_;
    $sql =~ s/\$(\$|\d+)/$1 eq '$' ? '$' : ':__' . $1/ge;
    my ($code, @vars) = &HTML::HTPL::Db::parse_sql2($sql);
    bless {'dbh' => $dbh, 'sql' => $code, 'vars' => \@vars}, $class;
}

sub load {
    my $self = shift;
    my $sth = $self->{'sth'};
    my @ary = @{$self->{'vars'}};
    unless ($sth) {
        my $dbh = $self->{'dbh'};
        my $sql = $self->{'sql'};
        $sth = $dbh->prepare($sql) || &dbdie(qq!SQL "$sql"!);
        $self->{'sth'} = $sth;
    } 
    my ($i, $boundary, $re);
    while (1) {
        $boundary = pack("C*", map {int(random(256));} (0 .. 10));
        $re = quotemeta($boundary);
        last unless grep /^$re$/, @_;
    }
    my $str = join($boundary, @_);
    $re = join($boundary, map {".*";} @_);
    reset;
    $str =~ /^$re$/;
    $sth->execute(map {$$_;} @ary);
    &HTML::HTPL::Db::load($sth);
}

1;
