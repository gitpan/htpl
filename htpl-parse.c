/** HTPL Macro parser **********************************
 ** This file is created automatically by htpl-crp.pl **
 ** Do not attempt to edit *****************************/

#define __HTPARSE__
#include "htpl.h"

#define RETURN(x) {int v = (x); destroypersist(); return v;}
#define numtokens (persist->tokens->num)

int parse_htpl_load(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("LOAD called with %d arguments, minimum needed is 1", numtokens))
    printfcode("die \"Unknown query\" unless $HTML::HTPL::Sys::query_pool{'%s'};\n", gettoken(1));
    printfcode("$%s = $HTML::HTPL::Sys::query_pool{'%s'}->load(qw(%s));\n", gettoken(1), gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_method___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected METHOD"))
    if (currscope->scope != scope_class) RETURN(croak("Now in scope %s from %d and met METHOD, expecting: class", scope_names[currscope->scope], currscope->nline))
    if (numtokens < 1) RETURN(croak("METHOD called with %d arguments, minimum needed is 1", numtokens))
    pushscope(scope_method, 0);
    buff = (STR)mysprintf("#CLSUTILS OTHER\n");
    nest++;
    code = parse_htpl(strchr(buff, '#') + 1, 0);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);
    printfcode("                sub __shadow__%s {\n", gettoken(1));
    printcode("SYNC\n");
    if (numtokens >= 2)  {
        printfcode("my (%s) = @_;\n", gettokenlist(2, ", ", "$"));
    }
    nesting = 0;
    RETURN(1)
}

int parse_htpl_method___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected METHOD"))
    if (currscope->scope != scope_method) RETURN(croak("Now in scope %s from %d and met METHOD, expecting: method", scope_names[currscope->scope], currscope->nline))
    popscope();
    if (!currscope) RETURN(croak("Unexpected METHOD"))
    if (currscope->scope != scope_class) RETURN(croak("Now in scope %s from %d and met METHOD, expecting: class", scope_names[currscope->scope], currscope->nline))
    nesting = 0;
    RETURN(1)
}

int parse_htpl_method(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_method___fwd(stack, untag);
        else return parse_htpl_method___rev(stack, untag);
}

int parse_htpl_ldap_delete(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("LDAP DELETE called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("LDAP DELETE called with %d arguments, maximum needed is 1", numtokens))

    printfcode("$HTML::HTPL::htpl_dir_obj->modify('%s');\n", gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ldap_init(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("LDAP INIT called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 4) RETURN(croak("LDAP INIT called with %d arguments, maximum needed is 4", numtokens))

    printcode("$HTML::HTPL::htpl_dir_obj = new\n");
    printfcode("              HTML::HTPL::LDAP(qw(%s));\n", gettokenlist(1, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ldap_search(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{\n");
    if (numtokens < 1) RETURN(croak("LDAP SEARCH called with %d arguments, minimum needed is 1", numtokens))
    code = 1;
    buff = (STR)mysprintf("LDAP DOSEARCH %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("$%s = $HTML::HTPL::ldap_query;\n", gettoken(1));
    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ldap_modify(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("LDAP MODIFY called with %d arguments, minimum needed is 2", numtokens))
    printfcode("$HTML::HTPL::htpl_dir_obj->modify('%s', '%2*');\n", gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ldap_dosearch(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{\n");
    printfcode("my %%tags = &HTML::HTPL::Sys::parse_tags('%s');\n", gettokenlist(1, " ", ""));
    printcode("&publish(&proper(sub {uc($_);}, %tags));\n");
    printcode("&HTML::HTPL::Sys::enforce_tags('filter,base', 'LDAP DOSEARCH', %tags);\n");
    if (!nest) RETURN(0)
    printcode("		$HTML::HTPL::ldap_query =\n");
    printcode("			$HTML::HTPL::htpl_dir_obj->search(\n");
    printcode("			$tags{'FILTER'}, $tags{'BASE'}, $tags{'SCOPE'},\n");
    printcode("                        $tags{'ATTR'} . $tags{'ATTRS'} .\n");
    printcode("			$tags{'ATTRIBUTES'}, $tags{'SIZE'} .\n");
    printcode("			$tags{'LIMIT'} . $tags{'SIZELIMIT'}, $tags{'KEY'}\n");
    printcode("			. $tags{'SORTKEY'});\n");
    printcode("	\n");
    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ldap_add(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("LDAP ADD called with %d arguments, minimum needed is 2", numtokens))
    printfcode("$HTML::HTPL::htpl_dir_obj->add('%s', '%2*');\n", gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ldap(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::LDAP;\n");
    }
    eat(&stack, token);
    if (!strcasecmp(token, "DELETE")) return parse_htpl_ldap_delete(stack, untag);
    if (!strcasecmp(token, "INIT")) return parse_htpl_ldap_init(stack, untag);
    if (!strcasecmp(token, "SEARCH")) return parse_htpl_ldap_search(stack, untag);
    if (!strcasecmp(token, "MODIFY")) return parse_htpl_ldap_modify(stack, untag);
    if (!strcasecmp(token, "DOSEARCH")) return parse_htpl_ldap_dosearch(stack, untag);
    if (!strcasecmp(token, "ADD")) return parse_htpl_ldap_add(stack, untag);
    return 0;
}

int parse_htpl_rem___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("&begintransaction;\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_rem___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("&endtransaction;\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_rem(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_rem___fwd(stack, untag);
        else return parse_htpl_rem___rev(stack, untag);
}

int parse_htpl_include(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("INCLUDE called with %d arguments, minimum needed is 1", numtokens))
    printfcode("&include(qw(%s));\n", gettokenlist(1, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_default(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected DEFAULT"))
    if (currscope->scope != scope_switch) RETURN(croak("Now in scope %s from %d and met DEFAULT, expecting: switch", scope_names[currscope->scope], currscope->nline))
    if (numtokens > 0) RETURN(croak("DEFAULT called with %d arguments, maximum needed is 0", numtokens))

    printcode("}; \n");
    printcode("$__htpl_cases_default = sub\n");
    printcode("{\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_continue(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected CONTINUE"))
    if (currscope->scope != scope_for && currscope->scope != scope_foreach) RETURN(croak("Now in scope %s from %d and met CONTINUE, expecting: for,foreach", scope_names[currscope->scope], currscope->nline))
    printcode("next;\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_break(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected BREAK"))
    if (currscope->scope != scope_for && currscope->scope != scope_foreach) RETURN(croak("Now in scope %s from %d and met BREAK, expecting: for,foreach", scope_names[currscope->scope], currscope->nline))
    printcode("last;\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_fixed(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 3) RETURN(croak("TEXT FIXED called with %d arguments, minimum needed is 3", numtokens))
    code = 1;
    buff = (STR)mysprintf("TEXT PREFIXED");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("$%s = &HTML::HTPL::Fixed'openfixed(\"%s\", qw(%s));\n", gettoken(1), gettoken(2), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_records(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 3) RETURN(croak("TEXT RECORDS called with %d arguments, minimum needed is 3", numtokens))
    code = 1;
    buff = (STR)mysprintf("TEXT PREFIXED");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("$%s = &HTML::HTPL::Fixed'openfixed(\"%s\", \\\"IBM\", qw(%s));\n", gettoken(1), gettoken(2), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_precsv(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!nest) RETURN(0)
    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::CSV;\n");
    }
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_prefixed(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!nest) RETURN(0)
    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::Fixed;\n");
    }
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_flat(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 3) RETURN(croak("TEXT FLAT called with %d arguments, minimum needed is 3", numtokens))
    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::Flat;\n");
    }
    printfcode(" $%s = &HTML::HTPL::Flat'openflat(\"%s\", qw(%s));\n", gettoken(1), gettoken(2), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_template___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("TEXT TEMPLATE called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("TEXT TEMPLATE called with %d arguments, maximum needed is 1", numtokens))

    printcode("{\n");
    printfcode("my $__htpl_params = \\%%%s;\n", gettoken(1));
    printcode("	&begintransaction;\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_template___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{ my $text = &endtransaction;\n");
    printcode("		 my $temp = new Template({ 'INCLUDE_PATH' => $ORIG_DIR,\n");
    printcode("			'INTERPOLATE' => 1, 'EVAL_PERL' => 1});\n");
    printcode("		$temp->process(\\$text, $__htpl_params); }\n");
    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_template(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!done) {
        done = 1;
        printcode("use Template;\n");
    }
    if (!untag) return parse_htpl_text_template___fwd(stack, untag);
        else return parse_htpl_text_template___rev(stack, untag);
}

int parse_htpl_text_read(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("TEXT READ called with %d arguments, minimum needed is 2", numtokens))
    if (numtokens > 2) RETURN(croak("TEXT READ called with %d arguments, maximum needed is 2", numtokens))

    printfcode("$%s = &readfile(\"%s\");\n", gettoken(1), gettoken(2));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_csv(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 3) RETURN(croak("TEXT CSV called with %d arguments, minimum needed is 3", numtokens))
    code = 1;
    buff = (STR)mysprintf("TEXT PRECSV");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("$%s = &HTML::HTPL::CSV'opencsv(\"%s\", \"%s\", qw(%s));\n", gettoken(1), gettoken(2), gettoken(3), gettokenlist(4, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text_cube(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 4) RETURN(croak("TEXT CUBE called with %d arguments, minimum needed is 4", numtokens))
    code = 1;
    buff = (STR)mysprintf("TEXT PRECSV");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("$%s = &HTML::HTPL::CSV'opencsv(\"%s\", [\"%s\", \"%s\"],\n", gettoken(1), gettoken(2), gettoken(3), gettoken(4));
    printfcode("qw(%s));\n", gettokenlist(5, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_text(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "FIXED")) return parse_htpl_text_fixed(stack, untag);
    if (!strcasecmp(token, "RECORDS")) return parse_htpl_text_records(stack, untag);
    if (!strcasecmp(token, "PRECSV")) return parse_htpl_text_precsv(stack, untag);
    if (!strcasecmp(token, "PREFIXED")) return parse_htpl_text_prefixed(stack, untag);
    if (!strcasecmp(token, "FLAT")) return parse_htpl_text_flat(stack, untag);
    if (!strcasecmp(token, "TEMPLATE")) return parse_htpl_text_template(stack, untag);
    if (!strcasecmp(token, "READ")) return parse_htpl_text_read(stack, untag);
    if (!strcasecmp(token, "CSV")) return parse_htpl_text_csv(stack, untag);
    if (!strcasecmp(token, "CUBE")) return parse_htpl_text_cube(stack, untag);
    return 0;
}

int parse_htpl_endif(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens > 0) RETURN(croak("ENDIF called with %d arguments, maximum needed is 0", numtokens))

    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected ENDIF"))
    if (currscope->scope != scope_if_then && currscope->scope != scope_if_then_else) RETURN(croak("Now in scope %s from %d and met ENDIF, expecting: if-then, if-then-else", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_net_get(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("NET GET called with %d arguments, minimum needed is 2", numtokens))
    printfcode("$%s = $htpl_net_obj->get(\"%s\", qw(%s));\n", gettoken(1), gettoken(2), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_net_setup(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("NET SETUP called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 3) RETURN(croak("NET SETUP called with %d arguments, maximum needed is 3", numtokens))

    printfcode("$htpl_net_obj = HTML::HTPL::Client->setup(\"%s\", \"%s\", \"%s\");\n", gettoken(1), gettoken(2), gettoken(3));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_net(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::Client;\n");
    }
    eat(&stack, token);
    if (!strcasecmp(token, "GET")) return parse_htpl_net_get(stack, untag);
    if (!strcasecmp(token, "SETUP")) return parse_htpl_net_setup(stack, untag);
    return 0;
}

int parse_htpl_for___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("FOR called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 4) RETURN(croak("FOR called with %d arguments, maximum needed is 4", numtokens))

    pushscope(scope_for, 0);
    if (numtokens <= 1)  {
        printfcode("foreach (1 .. %s) {\n", gettoken(1));
    }
    if (numtokens >= 2 && numtokens <= 2)  {
        printfcode("foreach $%s (1 .. %s) {\n", gettoken(1), gettoken(2));
    }
    if (numtokens >= 3 && numtokens <= 3)  {
        printfcode("foreach $%s (%s .. %s) {\n", gettoken(1), gettoken(1), gettoken(2));
    }
    if (numtokens >= 4 && numtokens <= 4)  {
        printfcode("for ($%s = %s; $%s <= %s; $%s += %s) {\n", gettoken(1), gettoken(2), gettoken(1), gettoken(3), gettoken(1), gettoken(4));
    }
    nesting = 0;
    RETURN(1)
}

int parse_htpl_for___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected FOR"))
    if (currscope->scope != scope_for) RETURN(croak("Now in scope %s from %d and met FOR, expecting: for", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_for(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_for___fwd(stack, untag);
        else return parse_htpl_for___rev(stack, untag);
}

int parse_htpl_time_modified(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("print scalar(localtime(&lastmodified()));\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_time(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "MODIFIED")) return parse_htpl_time_modified(stack, untag);
    return 0;
}

int parse_htpl_sql_batch(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("SQL BATCH called with %d arguments, minimum needed is 2", numtokens))
    if (numtokens > 2) RETURN(croak("SQL BATCH called with %d arguments, maximum needed is 2", numtokens))

    printfcode("$HTML::HTPL::htpl_db_obj->batch_insert('%s', $%s);\n", gettoken(1), gettoken(2));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_update(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("$HTML::HTPL::htpl_db_obj->update(\"%s\", qw(%s));\n", gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_exec(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL EXEC called with %d arguments, minimum needed is 1", numtokens))
    printfcode("$HTML::HTPL::htpl_db_obj->execsql(&HTML::HTPL::Db'parse_sql(\"%s\"));\n", gettokenlist(1, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_modify(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL MODIFY"))
    buff = (STR)mysprintf("SQL UPDATE %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_add(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL ADD"))
    buff = (STR)mysprintf("SQL INSERT %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_connect(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL CONNECT called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 3) RETURN(croak("SQL CONNECT called with %d arguments, maximum needed is 3", numtokens))

    printfcode("$HTML::HTPL::htpl_db_obj = HTML::HTPL::Db->new(\"%s\", \"%s\", \"%s\");\n", gettoken(1), gettoken(2), gettoken(3));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_immediate(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{ my $imm;\n");
    code = 1;
    buff = (STR)mysprintf("SQL CURSOR imm %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    code = 1;
    buff = (STR)mysprintf("FETCHIT imm");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_erase(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL ERASE"))
    buff = (STR)mysprintf("SQL ERASE %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_declare(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("$HTML::HTPL::Sys::query_pool{'%s'} ||= $HTML::HTPL::htpl_db_obj->prepare(\"%s\");\n", gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_delete(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("$HTML::HTPL::htpl_db_obj->delete(\"%s, qw(%s));\n", gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_xbase(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL XBASE called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("SQL XBASE called with %d arguments, maximum needed is 1", numtokens))

    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL XBASE"))
    buff = (STR)mysprintf("SQL CONNECT DBI:XBase:%s", gettoken(1));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_execute(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL EXECUTE"))
    buff = (STR)mysprintf("SQL EXEC %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_postgresql(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL POSTGRESQL called with %d arguments, minimum needed is 1", numtokens))
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL POSTGRESQL"))
    buff = (STR)mysprintf("SQL CONNECT DBI:Pg:dbname=%s", gettoken(1));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_insert(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("$HTML::HTPL::htpl_db_obj->add(\"%s\", qw(%s));\n", gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_query(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("$%s = $HTML::HTPL::htpl_db_obj->query(\"%s\", qw(%s));\n", gettoken(1), gettoken(2), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_mysql(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL MYSQL called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 3) RETURN(croak("SQL MYSQL called with %d arguments, maximum needed is 3", numtokens))

    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL MYSQL"))
    buff = (STR)mysprintf("SQL CONNECT DBI:mysql:%s %s", gettoken(1), gettokenlist(2, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_search(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL SEARCH"))
    buff = (STR)mysprintf("SQL CURSOR %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_project(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("SQL PROJECT called with %d arguments, minimum needed is 2", numtokens))
    printcode("{ my $imm;\n");
    code = 1;
    buff = (STR)mysprintf("SQL CURSOR imm %s", gettokenlist(2, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    code = 1;
    buff = (STR)mysprintf("PROJECT imm %s %s", gettoken(1), gettoken(1));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_postgres(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL POSTGRES"))
    buff = (STR)mysprintf("SQL POSTGRESQL %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_scope_connect(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL SCOPE CONNECT called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 3) RETURN(croak("SQL SCOPE CONNECT called with %d arguments, maximum needed is 3", numtokens))

    setvar("dbobj", (STR)mysprintf("$__htpl_db_%s_%d", getblockid(""), random()));
    if (!exportvar("dbobj", "")) RETURN(croak("Scope  not found in stack"));
    printfcode("my %s = \n", getvar("dbobj"));
    printfcode("   HTML::HTPL::Db->new(\"%s\", \"%s\", \"%s\");\n", gettoken(1), gettoken(2), gettoken(3));
    printcode("		\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_scope_exec(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL SCOPE EXEC called with %d arguments, minimum needed is 1", numtokens))
    code = 1;
    buff = (STR)mysprintf("SQL SCOPE RETRIEVE");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("%s->execsql(&HTML::HTPL::Db'parse_sql(\"%s\"));\n", getvar("dbobj"), gettokenlist(1, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_scope_cursor(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("SQL SCOPE CURSOR called with %d arguments, minimum needed is 2", numtokens))
    code = 1;
    buff = (STR)mysprintf("SQL SCOPE RETRIEVE");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("$%s = %s->cursor(&HTML::HTPL::Db'parse_sql(\"%s\"));\n", gettoken(1), getvar("dbobj"), gettokenlist(2, " ", ""));
    printcode("		\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_scope_retrieve(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!nest) RETURN(0)
    if (!importvar("dbobj", "")) RETURN(croak("Scope  not found in stack"));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_scope(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "CONNECT")) return parse_htpl_sql_scope_connect(stack, untag);
    if (!strcasecmp(token, "EXEC")) return parse_htpl_sql_scope_exec(stack, untag);
    if (!strcasecmp(token, "CURSOR")) return parse_htpl_sql_scope_cursor(stack, untag);
    if (!strcasecmp(token, "RETRIEVE")) return parse_htpl_sql_scope_retrieve(stack, untag);
    return 0;
}

int parse_htpl_sql_msql(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("SQL MSQL called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("SQL MSQL called with %d arguments, maximum needed is 1", numtokens))

    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in SQL MSQL"))
    buff = (STR)mysprintf("SQL CONNECT DBI:mSQL:%s", gettoken(1));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql_cursor(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("SQL CURSOR called with %d arguments, minimum needed is 2", numtokens))
    printfcode("$%s = $HTML::HTPL::htpl_db_obj->cursor(&HTML::HTPL::Db'parse_sql(\"%s\"));\n", gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_sql(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::Db;\n");
    }
    eat(&stack, token);
    if (!strcasecmp(token, "BATCH")) return parse_htpl_sql_batch(stack, untag);
    if (!strcasecmp(token, "UPDATE")) return parse_htpl_sql_update(stack, untag);
    if (!strcasecmp(token, "EXEC")) return parse_htpl_sql_exec(stack, untag);
    if (!strcasecmp(token, "MODIFY")) return parse_htpl_sql_modify(stack, untag);
    if (!strcasecmp(token, "ADD")) return parse_htpl_sql_add(stack, untag);
    if (!strcasecmp(token, "CONNECT")) return parse_htpl_sql_connect(stack, untag);
    if (!strcasecmp(token, "IMMEDIATE")) return parse_htpl_sql_immediate(stack, untag);
    if (!strcasecmp(token, "ERASE")) return parse_htpl_sql_erase(stack, untag);
    if (!strcasecmp(token, "DECLARE")) return parse_htpl_sql_declare(stack, untag);
    if (!strcasecmp(token, "DELETE")) return parse_htpl_sql_delete(stack, untag);
    if (!strcasecmp(token, "XBASE")) return parse_htpl_sql_xbase(stack, untag);
    if (!strcasecmp(token, "EXECUTE")) return parse_htpl_sql_execute(stack, untag);
    if (!strcasecmp(token, "POSTGRESQL")) return parse_htpl_sql_postgresql(stack, untag);
    if (!strcasecmp(token, "INSERT")) return parse_htpl_sql_insert(stack, untag);
    if (!strcasecmp(token, "QUERY")) return parse_htpl_sql_query(stack, untag);
    if (!strcasecmp(token, "MYSQL")) return parse_htpl_sql_mysql(stack, untag);
    if (!strcasecmp(token, "SEARCH")) return parse_htpl_sql_search(stack, untag);
    if (!strcasecmp(token, "PROJECT")) return parse_htpl_sql_project(stack, untag);
    if (!strcasecmp(token, "POSTGRES")) return parse_htpl_sql_postgres(stack, untag);
    if (!strcasecmp(token, "SCOPE")) return parse_htpl_sql_scope(stack, untag);
    if (!strcasecmp(token, "MSQL")) return parse_htpl_sql_msql(stack, untag);
    if (!strcasecmp(token, "CURSOR")) return parse_htpl_sql_cursor(stack, untag);
    return 0;
}

int parse_htpl_poof(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("print \"poof\\n\";\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_proc___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("PROC called with %d arguments, minimum needed is 1", numtokens))
    pushscope(scope_procedure, 0);
    printfcode("sub %s {\n", gettoken(1));
    if (numtokens >= 2)  {
        printfcode("my (%s) = @_;\n", gettokenlist(2, ", ", "$"));
    }
    nesting = 0;
    RETURN(1)
}

int parse_htpl_proc___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}\n");
    printcode("	\n");
    if (!currscope) RETURN(croak("Unexpected PROC"))
    if (currscope->scope != scope_procedure) RETURN(croak("Now in scope %s from %d and met PROC, expecting: procedure", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_proc(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_proc___fwd(stack, untag);
        else return parse_htpl_proc___rev(stack, untag);
}

int parse_htpl_class___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("CLASS called with %d arguments, minimum needed is 1", numtokens))
    pushscope(scope_class, 0);
    if (disposetrue((STR)mysprintf("%s", getsubtoken(1, ':', 2))))  {
        printfcode(" package %s;\n", getsubtoken(1, ':', 1));
        printfcode(" @ISA = split(/:/, \"%s\");\n", gettoken(1));
        printcode("	shift @ISA;\n");
        printfcode("        package %s::__shadow__;\n", getsubtoken(1, ':', 1));
        printfcode("        @ISA = map { \"${_}::__shadow__\"; } @%s::ISA;\n", getsubtoken(1, ':', 1));
        printcode("	\n");
        setvar("cls", (STR)mysprintf("%s", getsubtoken(1, ':', 1)));
    }
    if (!(disposetrue((STR)mysprintf("%s", getsubtoken(1, ':', 2)))))  {
        printfcode(" package %s;\n", gettoken(1));
        printcode("	\n");
        setvar("cls", (STR)mysprintf("%s", gettoken(1)));
    }
    printfcode(" use HTML::HTPL::Munge qw(%s);\n", gettokenlist(2, " ", ""));
    printcode("	\n");
    printcode("	sub set {\n");
    printcode("	my $self = shift;\n");
    printcode("	my %hash = @_;\n");
    printcode("	foreach (keys %hash) {\n");
    printcode("		$self->{$_} = $hash{$_};\n");
    printcode("	}\n");
    printcode("}\n");
    buff = (STR)mysprintf("#CLSUTILS OTHER\n");
    nest++;
    code = parse_htpl(strchr(buff, '#') + 1, 0);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);
    printcode("sub __shadow__clone {\n");
    printcode("	require Clone;\n");
    printcode("	Clone::clone($self);\n");
    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_class___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected CLASS"))
    if (currscope->scope != scope_class) RETURN(croak("Now in scope %s from %d and met CLASS, expecting: class", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_class(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_class___fwd(stack, untag);
        else return parse_htpl_class___rev(stack, untag);
}

int parse_htpl_publish(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("&publish(%%$%s);\n", gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_end(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in END"))
    buff = (STR)mysprintf("%s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, 1);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_out_tag(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!nest) RETURN(0)
    printfcode("print &outhtmltag(\"%s\", %s);\n", gettoken(1), gettoken(2));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_out(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "TAG")) return parse_htpl_out_tag(stack, untag);
    return 0;
}

int parse_htpl_foreach___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("FOREACH called with %d arguments, minimum needed is 2", numtokens))
    pushscope(scope_foreach, 0);
    printfcode("foreach $%s (qw(%s)) {\n", gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_foreach___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected FOREACH"))
    if (currscope->scope != scope_foreach) RETURN(croak("Now in scope %s from %d and met FOREACH, expecting: foreach", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_foreach(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_foreach___fwd(stack, untag);
        else return parse_htpl_foreach___rev(stack, untag);
}

int parse_htpl_loop(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens > 0) RETURN(croak("LOOP called with %d arguments, maximum needed is 0", numtokens))

    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected LOOP"))
    if (currscope->scope != scope_fetch) RETURN(croak("Now in scope %s from %d and met LOOP, expecting: fetch", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ifnotnull___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("IFNOTNULL called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("IFNOTNULL called with %d arguments, maximum needed is 1", numtokens))

    pushscope(scope_if_then, 0);
    printfcode("unless (!$%s || $%s->none) {\n", gettoken(1), gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ifnotnull___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens > 0) RETURN(croak("IFNOTNULL called with %d arguments, maximum needed is 0", numtokens))

    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in IFNOTNULL"))
    buff = (STR)mysprintf("ENDIF");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_ifnotnull(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_ifnotnull___fwd(stack, untag);
        else return parse_htpl_ifnotnull___rev(stack, untag);
}

int parse_htpl_case(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected CASE"))
    if (currscope->scope != scope_switch) RETURN(croak("Now in scope %s from %d and met CASE, expecting: switch", scope_names[currscope->scope], currscope->nline))
    printfcode("}; @__htpl_cases_scope = (%s);\n", gettokenlist(1, " ", ""));
    printcode("       @__htpl_cases_scope =\n");
    printcode("         defined($__htpl_case_last) ? ($__htpl_case_last + 1) : ()\n");
    printcode("         unless (@__htpl_cases_scope);\n");
    printcode("      @__htpl_cases_scope = ( '__' . ++$__htpl_cases_defopt)\n");
    printcode("        unless (@__htpl_cases_scope &&\n");
    printcode("        !@__htpl_cases{@__htpl_cases_scope});\n");
    if (numtokens >= 1)  {
        printcode("$__htpl_case_last = (!$#__htpl_cases_scope &&\n");
        printcode("$__htpl_cases_scope[0] =~ /^\\d+$/) ? $__htpl_cases_scope[0] : undef; \n");
    }
    if (numtokens <= 0)  {
        printcode("$__htpl_case_last++;\n");
    }
    printcode("@__htpl_cases{@__htpl_cases_scope} = revmap \\@__htpl_cases_scope,\n");
    printcode("sub {\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_counter(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("print &increasefile(\"%s\");\n", gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_mail___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{\n");
    printfcode("my %%tags = &HTML::HTPL::Sys::parse_tags('%s');\n", gettokenlist(1, " ", ""));
    printcode("&publish(&proper(sub {uc($_);}, %tags));\n");
    printcode("&HTML::HTPL::Sys::enforce_tags('FROM,TO,SUBJECT', 'MAIL', %tags);\n");
    pushscope(scope_mail, 0);
    printcode("{\n");
    printcode("		my %mailtags = %tags;\n");
    printcode("		my %params = %{$mailtags{'params'}};\n");
    printcode("		&begintransaction();\n");
    printcode("	\n");
    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_mail___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("		$message = &endtransaction();\n");
    printcode("		$message = &subhash($message, '#', %params)\n");
    printcode("			if ($mailtags{'params'});\n");
    printcode("		delete $mailtags{'params'};\n");
    printcode("		&sendmail('Msg' => $message, %mailtags);\n");
    printcode("	\n");
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected MAIL"))
    if (currscope->scope != scope_mail) RETURN(croak("Now in scope %s from %d and met MAIL, expecting: mail", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_mail(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_mail___fwd(stack, untag);
        else return parse_htpl_mail___rev(stack, untag);
}

int parse_htpl_catch(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected CATCH"))
    if (currscope->scope != scope_try && currscope->scope != scope_catch) RETURN(croak("Now in scope %s from %d and met CATCH, expecting: try,catch", scope_names[currscope->scope], currscope->nline))
    popscope();
    pushscope(scope_catch, 0);
    printcode("};\n");
    if (numtokens <= 0)  {
        printcode("$__htpl_default_handler = sub {$_ = shift; \n");
    }
    if (numtokens >= 1)  {
        printfcode("$__htpl_handler{'%s'} = sub {$_ = shift; \n", gettokenlist(1, " ", ""));
    }
    nesting = 0;
    RETURN(1)
}

int parse_htpl_constructor___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected CONSTRUCTOR"))
    if (currscope->scope != scope_class) RETURN(croak("Now in scope %s from %d and met CONSTRUCTOR, expecting: class", scope_names[currscope->scope], currscope->nline))
    pushscope(scope_contsructor, 0);
    if (!importvar("constructor", "class")) RETURN(croak("Scope class not found in stack"));
    if (disposetrue((STR)mysprintf("%s", getvar("constructor"))))  {
        RETURN(croak("Only one constructor per class! Previous at %s", getvar("constructor")))
    }
    setvar("constructor", (STR)mysprintf("%d", nline));
    if (!exportvar("constructor", "class")) RETURN(croak("Scope class not found in stack"));
    buff = (STR)mysprintf("#CLSUTILS MINE\n");
    nest++;
    code = parse_htpl(strchr(buff, '#') + 1, 0);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);
    printcode("sub new {\n");
    printfcode(" %s::__shadow__::new(@_);\n", getvar("cls"));
    printcode("}\n");
    buff = (STR)mysprintf("#CLSUTILS OTHER\n");
    nest++;
    code = parse_htpl(strchr(buff, '#') + 1, 0);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);
    printcode("sub new {\n");
    printcode("	my $class = shift;\n");
    printcode("	my $self = {};\n");
    printcode("	bless $self, $class;\n");
    printcode("	unshift(@_, $self);\n");
    printcode("SYNC\n");
    printcode("	\n");
    if (numtokens >= 1)  {
        printfcode(" my (%s) = @_;\n", gettokenlist(1, ", ", "$"));
    }
    nesting = 0;
    RETURN(1)
}

int parse_htpl_constructor___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("    $self;\n");
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected CONSTRUCTOR"))
    if (currscope->scope != scope_contsructor) RETURN(croak("Now in scope %s from %d and met CONSTRUCTOR, expecting: contsructor", scope_names[currscope->scope], currscope->nline))
    popscope();
    if (!currscope) RETURN(croak("Unexpected CONSTRUCTOR"))
    if (currscope->scope != scope_class) RETURN(croak("Now in scope %s from %d and met CONSTRUCTOR, expecting: class", scope_names[currscope->scope], currscope->nline))
    nesting = 0;
    RETURN(1)
}

int parse_htpl_constructor(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_constructor___fwd(stack, untag);
        else return parse_htpl_constructor___rev(stack, untag);
}

int parse_htpl_filter(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("$%s = $%s->filter(sub {%s;});\n", gettoken(2), gettoken(1), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_fetchit(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("FETCHIT called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("FETCHIT called with %d arguments, maximum needed is 1", numtokens))

    printfcode("$%s->fetch;\n", gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_else(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens > 0) RETURN(croak("ELSE called with %d arguments, maximum needed is 0", numtokens))

    if (!currscope) RETURN(croak("Unexpected ELSE"))
    if (currscope->scope != scope_if_then) RETURN(croak("Now in scope %s from %d and met ELSE, expecting: if-then", scope_names[currscope->scope], currscope->nline))
    popscope();
    pushscope(scope_if_then_else, 0);
    printcode("} else {\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_fetch___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("FETCH called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("FETCH called with %d arguments, maximum needed is 1", numtokens))

    pushscope(scope_fetch, 0);
    printfcode("$%s->rewind if ($%s);\n", gettoken(1), gettoken(1));
    printfcode("while ($%s && !$%s->eof && $%s->fetch) {\n", gettoken(1), gettoken(1), gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_fetch___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens > 0) RETURN(croak("FETCH called with %d arguments, maximum needed is 0", numtokens))

    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in FETCH"))
    buff = (STR)mysprintf("LOOP");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_fetch(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_fetch___fwd(stack, untag);
        else return parse_htpl_fetch___rev(stack, untag);
}

int parse_htpl_fetchcell(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 3) RETURN(croak("FETCHCELL called with %d arguments, minimum needed is 3", numtokens))
    if (numtokens > 3) RETURN(croak("FETCHCELL called with %d arguments, maximum needed is 3", numtokens))

    printfcode("$%s = $%s->get(\"%s\");\n", gettoken(3), gettoken(1), gettoken(2));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ifnull___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("IFNULL called with %d arguments, minimum needed is 1", numtokens))
    if (numtokens > 1) RETURN(croak("IFNULL called with %d arguments, maximum needed is 1", numtokens))

    pushscope(scope_if_then, 0);
    printfcode("if (!$%s || $%s->none) {\n", gettoken(1), gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_ifnull___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens > 0) RETURN(croak("IFNULL called with %d arguments, maximum needed is 0", numtokens))

    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in IFNULL"))
    buff = (STR)mysprintf("ENDIF");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_ifnull(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_ifnull___fwd(stack, untag);
        else return parse_htpl_ifnull___rev(stack, untag);
}

int parse_htpl_destructor___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!currscope) RETURN(croak("Unexpected DESTRUCTOR"))
    if (currscope->scope != scope_class) RETURN(croak("Now in scope %s from %d and met DESTRUCTOR, expecting: class", scope_names[currscope->scope], currscope->nline))
    if (numtokens > 0) RETURN(croak("DESTRUCTOR called with %d arguments, maximum needed is 0", numtokens))

    pushscope(scope_destructor, 0);
    if (!importvar("destructor", "class")) RETURN(croak("Scope class not found in stack"));
    if (disposetrue((STR)mysprintf("%s", getvar("destructor"))))  {
        RETURN(croak("Only one destructor per class! Previous at %s", getvar("destructor")))
    }
    setvar("destructor", (STR)mysprintf("%d", nline));
    if (!exportvar("destructor", "class")) RETURN(croak("Scope class not found in stack"));
    buff = (STR)mysprintf("#CLSUTILS MINE \n");
    nest++;
    code = parse_htpl(strchr(buff, '#') + 1, 0);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);
    printcode("	sub DESTROY {\n");
    printcode("SYNC\n");
    printcode("	\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_destructor___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected DESTRUCTOR"))
    if (currscope->scope != scope_destructor) RETURN(croak("Now in scope %s from %d and met DESTRUCTOR, expecting: destructor", scope_names[currscope->scope], currscope->nline))
    popscope();
    if (!currscope) RETURN(croak("Unexpected DESTRUCTOR"))
    if (currscope->scope != scope_class) RETURN(croak("Now in scope %s from %d and met DESTRUCTOR, expecting: class", scope_names[currscope->scope], currscope->nline))
    nesting = 0;
    RETURN(1)
}

int parse_htpl_destructor(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_destructor___fwd(stack, untag);
        else return parse_htpl_destructor___rev(stack, untag);
}

int parse_htpl_if___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("IF called with %d arguments, minimum needed is 1", numtokens))
    pushscope(scope_if_then, 0);
    printfcode("if (%s) {\n", gettokenlist(1, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_if___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens > 0) RETURN(croak("IF called with %d arguments, maximum needed is 0", numtokens))

    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in IF"))
    buff = (STR)mysprintf("ENDIF");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_if(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_if___fwd(stack, untag);
        else return parse_htpl_if___rev(stack, untag);
}

int parse_htpl_graph(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{\n");
    if (numtokens < 3) RETURN(croak("GRAPH called with %d arguments, minimum needed is 3", numtokens))
    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::Graph;\n");
    }
    printcode("	my $g = new HTML::HTPL::Graph;\n");
    printfcode(" $g->set('data' => [$%s->project('%s')]);\n", gettoken(1), gettoken(2));
    printfcode(" $g->set('labels' => [$%s->project('%s')]);\n", gettoken(1), gettoken(3));
    if (numtokens >= 4)  {
        printfcode(" $g->set('width' => %s);\n", gettoken(4));
    }
    if (numtokens >= 5)  {
        printfcode(" $g->set('cols' => %s);\n", gettoken(5));
    }
    if (numtokens >= 6)  {
        printfcode(" $g->set('legend' => [split(/:/, \"%s\")]);\n", gettokenlist(6, " ", ""));
    }
    printcode("	print $g->ashtml;\n");
    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_switch_rnd___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    pushscope(scope_random_switch, 0);
    printcode("{\n");
    code = 1;
    buff = (STR)mysprintf("SWITCH CASE");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_switch_rnd___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}; my @__htpl_case_keys = keys %__htpl_cases;\n");
    printcode("        my $__htpl_rcase = int(rand(@__htpl_case_keys));\n");
    printcode("	$__htpl_cases_choose = $__htpl_case_keys[$__htpl_rcase]; {\n");
    code = 1;
    buff = (STR)mysprintf("SWITCH CASE");
    nest++;
    code = parse_htpl(buff, 1);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected SWITCH RND"))
    if (currscope->scope != scope_random_switch) RETURN(croak("Now in scope %s from %d and met SWITCH RND, expecting: random-switch", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_switch_rnd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_switch_rnd___fwd(stack, untag);
        else return parse_htpl_switch_rnd___rev(stack, untag);
}

int parse_htpl_switch_case___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    pushscope(scope_switch, 0);
    printcode("{\n");
    printcode("my %__htpl_cases, @__htpl_cases_scope;\n");
    printcode("my $__htpl_cases_defopt, $__htpl_cases_default, $__htpl_cases_choose,\n");
    printcode("$__htpl_case_last = 0; \n");
    printfcode("  $__htpl_cases_choose = eval(\"%s\"); \n", gettokenlist(1, " ", ""));
    printcode("		{\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_switch_case___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}; my $__htpl_proc = $__htpl_cases{$__htpl_cases_choose}\n");
    printcode("				|| $__htpl_cases_default;\n");
    printcode("			&$__htpl_proc if (ref($__htpl_proc) =~ /CODE/);\n");
    printcode("		\n");
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected SWITCH CASE"))
    if (currscope->scope != scope_switch) RETURN(croak("Now in scope %s from %d and met SWITCH CASE, expecting: switch", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_switch_case(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_switch_case___fwd(stack, untag);
        else return parse_htpl_switch_case___rev(stack, untag);
}

int parse_htpl_switch(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "RND")) return parse_htpl_switch_rnd(stack, untag);
    if (!strcasecmp(token, "CASE")) return parse_htpl_switch_case(stack, untag);
    return 0;
}

int parse_htpl_clsutils_other(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    code = 1;
    buff = (STR)mysprintf("CLSUTILS IMP");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("package %s::__shadow__;\n", getvar("cls"));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_clsutils_imp(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (!importvar("cls", "class")) RETURN(croak("Scope class not found in stack"));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_clsutils_mine(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    code = 1;
    buff = (STR)mysprintf("CLSUTILS IMP");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printfcode("package %s;\n", getvar("cls"));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_clsutils(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!nest) RETURN(0)
    eat(&stack, token);
    if (!strcasecmp(token, "OTHER")) return parse_htpl_clsutils_other(stack, untag);
    if (!strcasecmp(token, "IMP")) return parse_htpl_clsutils_imp(stack, untag);
    if (!strcasecmp(token, "MINE")) return parse_htpl_clsutils_mine(stack, untag);
    return 0;
}

int parse_htpl_throw(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("%#line %d %s\n", nline, thefilename);
    printfcode("die \"%s\";\n", gettokenlist(1, " ", ""));
    printfcode("%#line %d %s\n", rline + currbuffer->lines + 1 + 1, thescript);
    nesting = 0;
    RETURN(1)
}

int parse_htpl_pts_set(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{\n");
    if (numtokens < 2) RETURN(croak("PTS SET called with %d arguments, minimum needed is 2", numtokens))
    if (numtokens > 3) RETURN(croak("PTS SET called with %d arguments, maximum needed is 3", numtokens))

    printfcode("my @t = split(/:/, \"%s\");\n", gettoken(1));
    printcode("push(@t, $HTML::HTPL::Config::htpl_pts_port);\n");
    printcode("$HTML::HTPL::pts_obj = new RPC::PlClient(\n");
    printcode("             peeraddr => $t[0],\n");
    printcode("             peerport => $t[1],\n");
    printfcode("             user => \"%s\",\n", gettoken(2));
    printfcode("             password => \"%s\",\n", gettoken(3));
    printcode("             application => \"pts\",\n");
    printcode("             version => 1.0);\n");
    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_pts_create(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("PTS CREATE called with %d arguments, minimum needed is 2", numtokens))
    printfcode("$%s = $HTML::HTPL::pts_obj->ClientObject(\"%s\", \"new\",\n", gettoken(1), gettoken(2));
    printfcode("                      qw(%s));\n", gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_pts_pool(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 3) RETURN(croak("PTS POOL called with %d arguments, minimum needed is 3", numtokens))
    if (numtokens > 3) RETURN(croak("PTS POOL called with %d arguments, maximum needed is 3", numtokens))

    printfcode("$%s = $HTML::HTPL::pts_obj->ClientObject(\"%s\", \n", gettoken(1), gettoken(2));
    printfcode(" \"getObject\", \"%s\");\n", gettoken(3));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_pts_call(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("PTS CALL called with %d arguments, minimum needed is 2", numtokens))
    printfcode("$%s = $HTML::HTPL::pts_obj->Call(\"%s\", qw(%s));\n", gettoken(1), gettoken(1), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_pts(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!done) {
        done = 1;
        printcode("use RPC::PlClient;\n");
    }
    eat(&stack, token);
    if (!strcasecmp(token, "SET")) return parse_htpl_pts_set(stack, untag);
    if (!strcasecmp(token, "CREATE")) return parse_htpl_pts_create(stack, untag);
    if (!strcasecmp(token, "POOL")) return parse_htpl_pts_pool(stack, untag);
    if (!strcasecmp(token, "CALL")) return parse_htpl_pts_call(stack, untag);
    return 0;
}

int parse_htpl_project(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 3) RETURN(croak("PROJECT called with %d arguments, minimum needed is 3", numtokens))
    printfcode("@%s = $%s->project(qw(%s));\n", gettoken(2), gettoken(1), gettokenlist(3, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_fetchcols(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 2) RETURN(croak("FETCHCOLS called with %d arguments, minimum needed is 2", numtokens))
    if (numtokens > 2) RETURN(croak("FETCHCOLS called with %d arguments, maximum needed is 2", numtokens))

    printfcode("foreach %s (%s->cols) {\n", gettoken(2), gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_call(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    if (numtokens < 1) RETURN(croak("CALL called with %d arguments, minimum needed is 1", numtokens))
    printfcode("&%s(%2, *%);\n", gettoken(1));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_img_rnd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{\n");
    printfcode("my %%tags = &HTML::HTPL::Sys::parse_tags('%s');\n", gettokenlist(1, " ", ""));
    printcode("&publish(&proper(sub {uc($_);}, %tags));\n");
    printcode("&HTML::HTPL::Sys::enforce_tags('SRC', 'IMG RND', %tags);\n");
    printcode("my @ims = split(/,\\s*/, $tags{'SRC'}); my $f = $ims[int(rand() * ($#ims + 1))];\n");
    printcode("		$tags{'SRC'} = $f; \n");
    code = 1;
    buff = (STR)mysprintf("OUT TAG IMG %%tags");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_img(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "RND")) return parse_htpl_img_rnd(stack, untag);
    return 0;
}

int parse_htpl_mem_immediate(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{ my $imm;\n");
    code = 1;
    buff = (STR)mysprintf("MEM CURSOR imm %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    code = 1;
    buff = (STR)mysprintf("FETCHIT imm");
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_mem_search(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    nesting++;
    if (nesting > 1) RETURN(croak("Infinite loop in MEM SEARCH"))
    buff = (STR)mysprintf("MEM CURSOR %s", gettokenlist(1, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    nesting = 0;
    RETURN(1)
}

int parse_htpl_mem_project(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("{ my $imm;\n");
    code = 1;
    buff = (STR)mysprintf("MEM CURSOR imm %s", gettokenlist(2, " ", ""));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    code = 1;
    buff = (STR)mysprintf("PROJECT imm %s %s", gettoken(1), gettoken(1));
    nest++;
    code = parse_htpl(buff, untag);
    nest--;
    if (!code) {
        croak("Unification of '%s' failed", buff);
        free(buff);
        RETURN(0)
    }
    free(buff);

    printcode("}\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_mem_cursor(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printfcode("$%s = HTML::HTPL::Mem'cursor(&HTML::HTPL::Db'parse_sql(\"%s\"));\n", gettoken(1), gettokenlist(2, " ", ""));
    nesting = 0;
    RETURN(1)
}

int parse_htpl_mem(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!done) {
        done = 1;
        printcode("use HTML::HTPL::Mem;\nuse HTML::HTPL::Db;\n");
    }
    eat(&stack, token);
    if (!strcasecmp(token, "IMMEDIATE")) return parse_htpl_mem_immediate(stack, untag);
    if (!strcasecmp(token, "SEARCH")) return parse_htpl_mem_search(stack, untag);
    if (!strcasecmp(token, "PROJECT")) return parse_htpl_mem_project(stack, untag);
    if (!strcasecmp(token, "CURSOR")) return parse_htpl_mem_cursor(stack, untag);
    return 0;
}

int parse_htpl_next(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected NEXT"))
    if (currscope->scope != scope_for && currscope->scope != scope_foreach) RETURN(croak("Now in scope %s from %d and met NEXT, expecting: for,foreach", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_try___fwd(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    pushscope(scope_try, 0);
    printcode("{\n");
    printcode("my $__htpl__try__sub = sub {\n");
    nesting = 0;
    RETURN(1)
}

int parse_htpl_try___rev(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    makepersist(stack);
    printcode("};\n");
    printcode("$@ = undef; \n");
    printcode("eval '&$__htpl__try__sub;';\n");
    printcode("if ($@) {\n");
    printcode("	foreach (keys %__htpl_handler) {\n");
    printcode("		my $v = $__htpl_handler{$_};\n");
    printcode("		if ($@ =~ /$_/ && ref($v) =~ /CODE/) {\n");
    printcode("			&$v($@);\n");
    printfcode("   goto __htpl_try_lbl%s;\n", getblockid(""));
    printcode("		} \n");
    printcode("	} \n");
    printcode("        if (ref($__htpl_default_handler) =~ /CODE/) {\n");
    printcode("		&$__htpl_default_handler;\n");
    printfcode("  goto __htpl_try_lbl%s;\n", getblockid(""));
    printcode("	}\n");
    printcode("	die $@;\n");
    printfcode("__htpl_try_lbl%s:\n", getblockid(""));
    printcode("} \n");
    printcode("}\n");
    if (!currscope) RETURN(croak("Unexpected TRY"))
    if (currscope->scope != scope_catch) RETURN(croak("Now in scope %s from %d and met TRY, expecting: catch", scope_names[currscope->scope], currscope->nline))
    popscope();
    nesting = 0;
    RETURN(1)
}

int parse_htpl_try(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    if (!untag) return parse_htpl_try___fwd(stack, untag);
        else return parse_htpl_try___rev(stack, untag);
}

int parse_htpl(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "LOAD")) return parse_htpl_load(stack, untag);
    if (!strcasecmp(token, "METHOD")) return parse_htpl_method(stack, untag);
    if (!strcasecmp(token, "LDAP")) return parse_htpl_ldap(stack, untag);
    if (!strcasecmp(token, "REM")) return parse_htpl_rem(stack, untag);
    if (!strcasecmp(token, "INCLUDE")) return parse_htpl_include(stack, untag);
    if (!strcasecmp(token, "DEFAULT")) return parse_htpl_default(stack, untag);
    if (!strcasecmp(token, "CONTINUE")) return parse_htpl_continue(stack, untag);
    if (!strcasecmp(token, "BREAK")) return parse_htpl_break(stack, untag);
    if (!strcasecmp(token, "TEXT")) return parse_htpl_text(stack, untag);
    if (!strcasecmp(token, "ENDIF")) return parse_htpl_endif(stack, untag);
    if (!strcasecmp(token, "NET")) return parse_htpl_net(stack, untag);
    if (!strcasecmp(token, "FOR")) return parse_htpl_for(stack, untag);
    if (!strcasecmp(token, "TIME")) return parse_htpl_time(stack, untag);
    if (!strcasecmp(token, "SQL")) return parse_htpl_sql(stack, untag);
    if (!strcasecmp(token, "POOF")) return parse_htpl_poof(stack, untag);
    if (!strcasecmp(token, "PROC")) return parse_htpl_proc(stack, untag);
    if (!strcasecmp(token, "CLASS")) return parse_htpl_class(stack, untag);
    if (!strcasecmp(token, "PUBLISH")) return parse_htpl_publish(stack, untag);
    if (!strcasecmp(token, "END")) return parse_htpl_end(stack, untag);
    if (!strcasecmp(token, "OUT")) return parse_htpl_out(stack, untag);
    if (!strcasecmp(token, "FOREACH")) return parse_htpl_foreach(stack, untag);
    if (!strcasecmp(token, "LOOP")) return parse_htpl_loop(stack, untag);
    if (!strcasecmp(token, "IFNOTNULL")) return parse_htpl_ifnotnull(stack, untag);
    if (!strcasecmp(token, "CASE")) return parse_htpl_case(stack, untag);
    if (!strcasecmp(token, "COUNTER")) return parse_htpl_counter(stack, untag);
    if (!strcasecmp(token, "MAIL")) return parse_htpl_mail(stack, untag);
    if (!strcasecmp(token, "CATCH")) return parse_htpl_catch(stack, untag);
    if (!strcasecmp(token, "CONSTRUCTOR")) return parse_htpl_constructor(stack, untag);
    if (!strcasecmp(token, "FILTER")) return parse_htpl_filter(stack, untag);
    if (!strcasecmp(token, "FETCHIT")) return parse_htpl_fetchit(stack, untag);
    if (!strcasecmp(token, "ELSE")) return parse_htpl_else(stack, untag);
    if (!strcasecmp(token, "FETCH")) return parse_htpl_fetch(stack, untag);
    if (!strcasecmp(token, "FETCHCELL")) return parse_htpl_fetchcell(stack, untag);
    if (!strcasecmp(token, "IFNULL")) return parse_htpl_ifnull(stack, untag);
    if (!strcasecmp(token, "DESTRUCTOR")) return parse_htpl_destructor(stack, untag);
    if (!strcasecmp(token, "IF")) return parse_htpl_if(stack, untag);
    if (!strcasecmp(token, "GRAPH")) return parse_htpl_graph(stack, untag);
    if (!strcasecmp(token, "SWITCH")) return parse_htpl_switch(stack, untag);
    if (!strcasecmp(token, "CLSUTILS")) return parse_htpl_clsutils(stack, untag);
    if (!strcasecmp(token, "THROW")) return parse_htpl_throw(stack, untag);
    if (!strcasecmp(token, "PTS")) return parse_htpl_pts(stack, untag);
    if (!strcasecmp(token, "PROJECT")) return parse_htpl_project(stack, untag);
    if (!strcasecmp(token, "FETCHCOLS")) return parse_htpl_fetchcols(stack, untag);
    if (!strcasecmp(token, "CALL")) return parse_htpl_call(stack, untag);
    if (!strcasecmp(token, "IMG")) return parse_htpl_img(stack, untag);
    if (!strcasecmp(token, "MEM")) return parse_htpl_mem(stack, untag);
    if (!strcasecmp(token, "NEXT")) return parse_htpl_next(stack, untag);
    if (!strcasecmp(token, "TRY")) return parse_htpl_try(stack, untag);
    return 0;
}

int parse_(stack, untag)
    int untag;
    STR stack; {

    TOKEN token;
    static done = 0;
    STR buff;
    int code;
    static int nesting = 0;

    eat(&stack, token);
    if (!strcasecmp(token, "HTPL")) return parse_htpl(stack, untag);
    return 0;
}

