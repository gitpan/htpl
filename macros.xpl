<?xml version="1.0" ?>
<!DOCTYPE XPL>
<HTPL>
<SQL>
	<__PRE>use HTML::HTPL::Db;</__PRE>
	<CONNECT MIN="1" MAX="3">$HTML::HTPL::htpl_db_obj = HTML::HTPL::Db->new("%1%", "%2%", "%3%");</CONNECT>
	<MYSQL MIN="1" MAX="3"><__ALIAS>SQL CONNECT DBI:mysql:%1% %2*%</__ALIAS></MYSQL>
	<MSQL MIN="1" MAX="1"><__ALIAS>SQL CONNECT DBI:mSQL:%1%</__ALIAS></MSQL>
	<XBASE MIN="1" MAX="1"><__ALIAS>SQL CONNECT DBI:XBase:%1%</__ALIAS></XBASE>
	<POSTGRESQL MIN="1"><__ALIAS>SQL CONNECT DBI:Pg:dbname=%1%</__ALIAS></POSTGRESQL>
	<POSTGRES><__ALIAS>SQL POSTGRESQL %1*%</__ALIAS></POSTGRES>

        <EXEC MIN="1">$HTML::HTPL::htpl_db_obj->execsql(&amp;HTML::HTPL::Db'parse_sql("%1*%"));</EXEC>
	<EXECUTE><__ALIAS>SQL EXEC %1*%</__ALIAS></EXECUTE>
	<DECLARE>$HTML::HTPL::Sys::query_pool('%1%'} ||= $HTML::HTPL::htpl_db_obj->prepare("%2*");</DECLARE>
	<CURSOR MIN="2">$%1% = $HTML::HTPL::htpl_db_obj->cursor(&amp;HTML::HTPL::Db'parse_sql("%2*%"));</CURSOR>
	<SEARCH><__ALIAS>SQL CURSOR %1*%</__ALIAS></SEARCH>
	<IMMEDIATE><__DO>{ my $imm;</__DO>
		<__INCLUDE>SQL CURSOR imm %1*%</__INCLUDE>
		<__INCLUDE>FETCHIT imm</__INCLUDE>
		<__DO>}</__DO>
	</IMMEDIATE>
	<PROJECT MIN="2"><__DO>{ my $imm;</__DO>
                <__INCLUDE>SQL CURSOR imm %2*%</__INCLUDE>
                <__INCLUDE>PROJECT imm %1% %1%</__INCLUDE>
		<__DO>}</__DO>
	</PROJECT>

	<INSERT>$HTML::HTPL::htpl_db_obj->add("%1%", qw(%2*%));</INSERT>
	<ADD><__ALIAS>SQL INSERT %1*%</__ALIAS></ADD>

	<UPDATE>$HTML::HTPL::htpl_db_obj->update("%1%", qw(%2*%));</UPDATE>
	<MODIFY><__ALIAS>SQL UPDATE %1*%</__ALIAS></MODIFY>
	<DELETE>$HTML::HTPL::htpl_db_obj->delete("%1%, qw(%2*%));</DELETE>
	<ERASE><__ALIAS>SQL ERASE %1*%</__ALIAS></ERASE>

	<QUERY>$%1% = $HTML::HTPL::htpl_db_obj->query("%2%", qw(%3*%));</QUERY>
</SQL>

<LDAP>
	<__PRE>use HTML::HTPL::LDAP;</__PRE>
	<INIT MIN="1" MAX="4">$HTML::HTPL::htpl_dir_obj = new
              HTML::HTPL::LDAP(qw(%1*%));</INIT>
        <SEARCH MIN="1" SCOPE="1"><__INCLUDE>LDAP DOSEARCH %1*%</__INCLUDE>
                <__DO>$%1% = $HTML::HTPL::ldap_query;</__DO></SEARCH>
	<DOSEARCH PRIVATE="1" MANDATORY="filter,base" PARAMS="1">
		$HTML::HTPL::ldap_query =
			$HTML::HTPL::htpl_dir_obj->search(
			$tags{'FILTER'}, $tags{'BASE'}, $tags{'SCOPE'},
                        $tags{'ATTR'} . $tags{'ATTRS'} .
			$tags{'ATTRIBUTES'}, $tags{'SIZE'} .
			$tags{'LIMIT'} . $tags{'SIZELIMIT'}, $tags{'KEY'}
			. $tags{'SORTKEY'});
	</DOSEARCH>
	<ADD MIN="2">$HTML::HTPL::htpl_dir_obj->add('%1%', '%2*');</ADD>
	<MODIFY MIN="2">$HTML::HTPL::htpl_dir_obj->modify('%1%', '%2*');</MODIFY>
	<DELETE MIN="1" MAX="1">$HTML::HTPL::htpl_dir_obj->modify('%1%');</DELETE>
 </LDAP>

<MEM>
	<__PRE>use HTML::HTPL::Mem;
use HTML::HTPL::Db;</__PRE>
	<CURSOR>$%1% = HTML::HTPL::Mem'cursor(&amp;HTML::HTPL::Db'parse_sql("%2*%"));</CURSOR>
	<SEARCH><__ALIAS>MEM CURSOR %1*%</__ALIAS></SEARCH>
	<IMMEDIATE><__DO>{ my $imm;</__DO>
		<__INCLUDE>MEM CURSOR imm %1*%</__INCLUDE>
		<__INCLUDE>FETCHIT imm</__INCLUDE>
		<__DO>}</__DO>
	</IMMEDIATE>
	<PROJECT><__DO>{ my $imm;</__DO>
                <__INCLUDE>MEM CURSOR imm %2*%</__INCLUDE>
                <__INCLUDE>PROJECT imm %1% %1%</__INCLUDE>
		<__DO>}</__DO>
	</PROJECT>
</MEM>

<TEXT>
        <PRECSV NOOP="1" PRIVATE="1"><__PRE>use HTML::HTPL::CSV;</__PRE></PRECSV>
	<CSV MIN="3"><__INCLUDE>TEXT PRECSV</__INCLUDE>
	<__DO>$%1% = &amp;HTML::HTPL::CSV'opencsv("%2%", "%3%", qw(%4*%));</__DO></CSV>

	<FLAT MIN="3"><__PRE>use HTML::HTPL::Flat;</__PRE>
	$%1% = &amp;HTML::HTPL::Flat'openflat("%2%", qw(%3*%));</FLAT>

        <CUBE MIN="4"><__INCLUDE>TEXT PRECSV</__INCLUDE>
	<__DO>$%1% = &amp;HTML::HTPL::CSV'opencsv("%2%", ["%3%", "%4%"],
qw(%5*%));</__DO></CUBE>


	<PREFIXED NOOP="1" PRIVATE="1"><__PRE>use HTML::HTPL::Fixed;</__PRE></PREFIXED>
	<FIXED MIN="3"><__INCLUDE>TEXT PREFIXED</__INCLUDE>
	<__DO>$%1% = &amp;HTML::HTPL::Fixed'openfixed("%2%", qw(%3*%));</__DO></FIXED>
	<RECORDS MIN="3"><__INCLUDE>TEXT PREFIXED</__INCLUDE>
	<__DO>$%1% = &amp;HTML::HTPL::Fixed'openfixed("%2%", \"IBM", qw(%3*%));</__DO></RECORDS>

	<READ MIN="2" MAX="2">$%1% = &amp;readfile("%2%");</READ>

	<TEMPLATE AREA="1" SCOPE="1"><__PRE>use Template;</__PRE>
	<__FWD MIN="1" MAX="1">my $__htpl_params = \%%%1%;
	&amp;begintransaction;</__FWD>
	<__REV>{ my $text = &amp;endtransaction;
		 my $temp = new Template({ 'INCLUDE_PATH' => $ORIG_DIR,
			'INTERPOLATE' => 1, 'EVAL_PERL' => 1});
		$temp->process(\$text, $__htpl_params); }</__REV>
	</TEMPLATE>

</TEXT>

<LOAD MIN="1">die "Unknown query" unless $HTML::HTPL::Sys::query_pool{'%1%'};
$%1% = $HTML::HTPL::Sys::query_pool{'%1%'}->load(qw(%2*%));</LOAD>

<FETCH AREA="1"><__FWD MIN="1" MAX="1" PUSH="fetch">$%1%->rewind if ($%1%);
while ($%1% &amp;&amp; !$%1%->eof &amp;&amp; $%1%->fetch) {</__FWD>
<__REV MAX="0"><__ALIAS>LOOP</__ALIAS></__REV>
</FETCH>
<FETCHIT MIN="1" MAX="1">$%1%->fetch;</FETCHIT>
<FETCHCOLS MIN="2" MAX="2">foreach %2% (%1%->cols) {</FETCHCOLS>
<FETCHCELL MIN="3" MAX="3">$%3% = $%1%->get("%2%");</FETCHCELL>
<PROJECT MIN="3">@%3% = $%1%->project(qw(%2*%));</PROJECT>
<FILTER>$%2% = $%1%->filter(sub {%2*%});</FILTER>

<IFNULL AREA="1"><__FWD MIN="1" MAX="1" PUSH="if-then">if (!$%1% || $%1%->none) {</__FWD>
<__REV MAX="0"><__ALIAS>ENDIF</__ALIAS></__REV>
</IFNULL>
<IFNOTNULL AREA="1"><__FWD MIN="1" MAX="1" PUSH="if-then">unless (!$%1% || $%1%->none) {</__FWD>
<__REV MAX="0"><__ALIAS>ENDIF</__ALIAS></__REV>
</IFNOTNULL>

<IF AREA="1"><__FWD MIN="1" PUSH="if-then">if (%1*%) {</__FWD>
<__REV MAX="0"><__ALIAS>ENDIF</__ALIAS></__REV>
</IF>

<FOR AREA="1" BLOCK="for">
<__FWD MIN="1" MAX="4">
<__DO MAX="1">foreach (1 .. %1%) {</__DO>
<__DO MIN="2" MAX="2">foreach $%1% (1 .. %2%) {</__DO>
<__DO MIN="3" MAX="3">foreach $%1% (%1% .. %2%) {</__DO>
<__DO MIN="4" MAX="4">for ($%1% = %2%; $%1% &lt;= %3%; $%1% += %4%) {</__DO>
</__FWD>
<__REV>}</__REV>
</FOR>

<FOREACH AREA="1" BLOCK="foreach">
<__FWD MIN="2">foreach $%1% (qw(%2*%)) {</__FWD>
<__REV>}</__REV>
</FOREACH>

<NEXT POP="for,foreach">}</NEXT>
<BREAK BROTHER="for,foreach">last;</BREAK>
<CONTINUE BROTHER="for,foreach">next;</CONTINUE>

<LOOP MAX="0" POP="fetch">}</LOOP>
<ENDIF MAX="0" POP="if-then, if-then-else">}</ENDIF>
<ELSE MAX="0"><__POP>if-then</__POP>
<__PUSH>if-then-else</__PUSH>
<__DO>} else {</__DO></ELSE>

<FILTER>$%2% = $%1%->filter(sub {%3*%;});</FILTER>

<OUT><TAG PRIVATE="1">print &amp;outhtmltag("%1%", %2%);</TAG></OUT>

<IMG>
	<RND PARAMS="1" MANDATORY="SRC" SCOPE="1">
<__DO>my @ims = split(/,\s*/, $tags{'SRC'}); my $f = $ims[int(rand() * ($#ims + 1))];
		$tags{'SRC'} = $f; </__DO>
		<__INCLUDE>OUT TAG IMG %%tags</__INCLUDE>
	</RND>
</IMG>

<SWITCH>
	<CASE AREA="1" BLOCK="switch" SCOPE="1">
		<__FWD><__DO>my %%__htpl_cases, @__htpl_cases_scope;
my $__htpl_cases_defopt, $__htpl_cases_default, $__htpl_cases_choose,
$__htpl_case_last = 0; 
		$__htpl_cases_choose = eval("%1*%"); 
		{</__DO></__FWD>
		<__REV>}; my $__htpl_proc = $__htpl_cases{$__htpl_cases_choose}
				|| $__htpl_cases_default;
			&amp;$__htpl_proc if (ref($__htpl_proc) =~ /CODE/);
		</__REV>
	</CASE>
	<RND AREA="1" BLOCK="random-switch" SCOPE="1">
	<__FWD><__INCLUDE>SWITCH CASE</__INCLUDE></__FWD>
	<__REV><__DO>}; my @__htpl_case_keys = keys %%__htpl_cases;
        my $__htpl_rcase = int(rand(@__htpl_case_keys));
	$__htpl_cases_choose = $__htpl_case_keys[$__htpl_rcase]; {</__DO>
	<__INCLUDE DIR="REV">SWITCH CASE</__INCLUDE></__REV>
	</RND>	
</SWITCH>

<CASE BROTHER="switch">
<__DO>}; @__htpl_cases_scope = (%1*%);
       @__htpl_cases_scope =
         defined($__htpl_case_last) ? ($__htpl_case_last + 1) : ()
         unless (@__htpl_cases_scope);
      @__htpl_cases_scope = ( '__' . ++$__htpl_cases_defopt)
        unless (@__htpl_cases_scope &amp;&amp;
        !@__htpl_cases{@__htpl_cases_scope});
</__DO>
<__DO MIN="1">$__htpl_case_last = (!$#__htpl_cases_scope &amp;&amp;
$__htpl_cases_scope[0] =~ /^\d+$/) ? $__htpl_cases_scope[0] : undef; 
</__DO>
<__DO MAX="0">$__htpl_case_last++;</__DO>
<__DO>@__htpl_cases{@__htpl_cases_scope} = revmap \@__htpl_cases_scope,
sub {</__DO></CASE> 
<DEFAULT BROTHER="switch" MAX="0">}; 
$__htpl_cases_default = sub
{</DEFAULT>


<TIME>
	<MODIFIED>print scalar(localtime(&amp;lastmodified()));</MODIFIED>
</TIME>

<COUNTER>print &amp;increasefile("%1%");</COUNTER>

<END>
<__ALIAS DIR="REV">%1*%</__ALIAS>
</END>

<TRY AREA="1" SCOPE="1"><__FWD PUSH="try">my %%__htpl_handler, $__htpl_default_handler; 
my $__htpl_try_sub = sub {
</__FWD>
<__REV POP="catch"><__DO>};
$@ = undef; 
eval '&amp;$__htpl_try_sub';
if ($@) {
	foreach (keys %%__htpl_handler) {
		my $v = $__htpl_handler{$_};
		if ($@ =~ /$_/ &amp;&amp; ref($v) =~ /CODE/) {
			&amp;$v($@);
			goto __htpl_try_lbl%id%;
		} 
	} 
        if (ref($__htpl_default_handler) =~ /CODE/) {
		&amp;$__htpl_default_handler;
		goto __htpl_try_lbl%id%;
	}
	die $@;
__htpl_try_lbl%id%:
} 
</__DO></__REV>
</TRY>
<CATCH BROTHER="try,catch" CHANGE="catch" MAX="1"><__DO>};
</__DO>
<__DO MAX="0">$__htpl_default_handler = sub {$_ = shift; </__DO>
<__DO MIN="1">$__htpl_handler{'%1%'} = sub {$_ = shift; </__DO>
</CATCH>
<THROW>
#line %line% %page%
die "%1*%";
#line %rlineplus1% %script%
</THROW>

<MAIL BLOCK="mail" AREA="1" SCOPE="1">
	<__FWD PARAMS="1" MANDATORY="FROM,TO,SUBJECT">
	<__DO>
		my %%mailtags = %%tags;
		my %%params = %%{$mailtags{'params'}};
		&amp;begintransaction();
	</__DO>
	</__FWD>
	<__REV>
		$message = &amp;endtransaction();
		$message = &amp;subhash($message, '#', %%params)
			if ($mailtags{'params'});
		delete $mailtags{'params'};
		&amp;sendmail('Msg' => $message, %%mailtags);
	</__REV>
</MAIL>

<PUBLISH>&amp;publish(%%$%1%);</PUBLISH>
<NET>
	<__PRE>use HTML::HTPL::Client;</__PRE>
	<SETUP MIN="1" MAX="3">$htpl_net_obj = HTML::HTPL::Client->setup("%1%", "%2%", "%3%");</SETUP>
	<GET MIN="2">$%1% = $htpl_net_obj->get("%2%", qw(%3*%));</GET>
</NET>
<PROC BLOCK="procedure" AREA="1">
	<__FWD MIN="1"><__DO>sub %1% {</__DO>
		<__DO MIN="2">my (%2!%) = @_;</__DO>
       </__FWD>
	<__REV>
}
	</__REV>
</PROC>

<METHOD BLOCK="method" AREA="1" BROTHER="class">
	<__FWD MIN="1">
                <__DO>
#CLSUTILS OTHER
                sub __shadow__%1% {
SYNC
</__DO>
		<__DO MIN="2">my (%2!%) = @_;</__DO>
       </__FWD>
	<__REV>}</__REV>
</METHOD>
<CLASS AREA="1" BLOCK="class">
<__FWD MIN="1">
	<__BLOCK ASSERT="%1:2%">
	<__DO>
	package %1:1%;
	@ISA = split(/:/, "%1%");
	shift @ISA;
        package %1:1%::__shadow__;
        @ISA = map { "${_}::__shadow__"; } @%1:1%::ISA;

	</__DO>
	<__SET VALUE="%1:1%">cls</__SET>
	</__BLOCK>
	<__BLOCK ASSERT="! %1:2%">
	<__DO>
	package %1%;
	</__DO>
	<__SET VALUE="%1%">cls</__SET>
	</__BLOCK>
	<__DO>
	use HTML::HTPL::Munge qw(%2*%);
	</__DO>
	<__DO>	sub set {
	my $self = shift;
	my %%hash = @_;
	foreach (keys %%hash) {
		$self->{$_} = $hash{$_};
	}
}</__DO>
</__FWD>
<__REV NOOP="1"></__REV>
</CLASS>

<CLSUTILS PRIVATE="1">
	<MINE><__INCLUDE>CLSUTILS IMP</__INCLUDE>
	<__DO>package %$cls%;</__DO></MINE>
	<OTHER><__INCLUDE>CLSUTILS IMP</__INCLUDE>
        <__DO>package %$cls%::__shadow__;</__DO></OTHER>
	<IMP><__IMPORT SCOPE="class">cls</__IMPORT></IMP>
</CLSUTILS>

<CONSTRUCTOR AREA="1" BLOCK="contsructor" BROTHER="class">
	<__FWD>
<__IMPORT SCOPE="class">constructor</__IMPORT>
<__CROAK ASSERT="%$constructor%">Only one constructor per class! Previous at %$constructor%</__CROAK>
<__SET VALUE="%line%">constructor</__SET>
<__EXPORT SCOPE="class">constructor</__EXPORT>
<__DO>
#CLSUTILS MINE
sub new {
	%$cls%::__shadow__::new(@_);
}
#CLSUTILS OTHER
sub new {
	my $class = shift;
	my $self = {};
	bless $self, $class;
	unshift(@_, $self);
SYNC
	</__DO>
	<__DO MIN="1">
	my (%1!%) = @_;</__DO></__FWD>
	<__REV>    $self;
}</__REV>
</CONSTRUCTOR>
<DESTRUCTOR AREA="1" BLOCK="destructor" BROTHER="class">
<__FWD MAX="0">
<__IMPORT SCOPE="class">destructor</__IMPORT>
<__CROAK ASSERT="%$destructor%">Only one destructor per class! Previous at %$destructor%</__CROAK>
<__SET VALUE="%line%">destructor</__SET>
<__EXPORT SCOPE="class">destructor</__EXPORT>
<__DO>
#CLSUTILS MINE	
	sub DESTROY {
SYNC
	</__DO>
</__FWD>
	<__REV>}</__REV>
</DESTRUCTOR>
<GRAPH MIN="3" SCOPE="1"><__PRE>use HTML::HTPL::Graph;</__PRE>
<__DO>
	my $g = new HTML::HTPL::Graph;
	$g->set('data' => [$%1%->project('%2%')]);
	$g->set('labels' => [$%1%->project('%3%')]);
</__DO>
<__DO MIN="4">
	$g->set('width' => %4%);
</__DO>
<__DO MIN="5">
	$g->set('cols' => %5%);
</__DO>
<__DO MIN="6">
	$g->set('legend' => [split(/:/, "%6*%")]);
</__DO>
<__DO>
	print $g->ashtml;
</__DO></GRAPH>

<CALL MIN="1">&amp;%1%(%2, *%);</CALL>

<INCLUDE MIN="1">&amp;include(qw(%1*%));</INCLUDE>

</HTPL>