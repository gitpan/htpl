int parse_htpl(STR, int);
int parse_htpl_load(STR, int);
int parse_htpl_method(STR, int);
int parse_htpl_method___fwd(STR, int);
int parse_htpl_method___rev(STR, int);
int parse_htpl_ldap(STR, int);
int parse_htpl_ldap_delete(STR, int);
int parse_htpl_ldap_init(STR, int);
int parse_htpl_ldap_search(STR, int);
int parse_htpl_ldap_modify(STR, int);
int parse_htpl_ldap_dosearch(STR, int);
int parse_htpl_ldap_add(STR, int);
int parse_htpl_include(STR, int);
int parse_htpl_default(STR, int);
int parse_htpl_continue(STR, int);
int parse_htpl_break(STR, int);
int parse_htpl_text(STR, int);
int parse_htpl_text_precsv(STR, int);
int parse_htpl_text_flat(STR, int);
int parse_htpl_text_read(STR, int);
int parse_htpl_text_csv(STR, int);
int parse_htpl_text_cube(STR, int);
int parse_htpl_endif(STR, int);
int parse_htpl_net(STR, int);
int parse_htpl_net_get(STR, int);
int parse_htpl_net_setup(STR, int);
int parse_htpl_for(STR, int);
int parse_htpl_for___fwd(STR, int);
int parse_htpl_for___rev(STR, int);
int parse_htpl_time(STR, int);
int parse_htpl_time_modified(STR, int);
int parse_htpl_sql(STR, int);
int parse_htpl_sql_update(STR, int);
int parse_htpl_sql_exec(STR, int);
int parse_htpl_sql_modify(STR, int);
int parse_htpl_sql_add(STR, int);
int parse_htpl_sql_connect(STR, int);
int parse_htpl_sql_immediate(STR, int);
int parse_htpl_sql_erase(STR, int);
int parse_htpl_sql_declare(STR, int);
int parse_htpl_sql_delete(STR, int);
int parse_htpl_sql_xbase(STR, int);
int parse_htpl_sql_execute(STR, int);
int parse_htpl_sql_postgresql(STR, int);
int parse_htpl_sql_insert(STR, int);
int parse_htpl_sql_query(STR, int);
int parse_htpl_sql_mysql(STR, int);
int parse_htpl_sql_search(STR, int);
int parse_htpl_sql_postgres(STR, int);
int parse_htpl_sql_project(STR, int);
int parse_htpl_sql_msql(STR, int);
int parse_htpl_sql_cursor(STR, int);
int parse_htpl_proc(STR, int);
int parse_htpl_proc___fwd(STR, int);
int parse_htpl_proc___rev(STR, int);
int parse_htpl_class(STR, int);
int parse_htpl_class___fwd(STR, int);
int parse_htpl_class___rev(STR, int);
int parse_htpl_publish(STR, int);
int parse_htpl_end(STR, int);
int parse_htpl_out(STR, int);
int parse_htpl_out_tag(STR, int);
int parse_htpl_foreach(STR, int);
int parse_htpl_foreach___fwd(STR, int);
int parse_htpl_foreach___rev(STR, int);
int parse_htpl_loop(STR, int);
int parse_htpl_ifnotnull(STR, int);
int parse_htpl_ifnotnull___fwd(STR, int);
int parse_htpl_ifnotnull___rev(STR, int);
int parse_htpl_case(STR, int);
int parse_htpl_counter(STR, int);
int parse_htpl_mail(STR, int);
int parse_htpl_mail___fwd(STR, int);
int parse_htpl_mail___rev(STR, int);
int parse_htpl_catch(STR, int);
int parse_htpl_constructor(STR, int);
int parse_htpl_constructor___fwd(STR, int);
int parse_htpl_constructor___rev(STR, int);
int parse_htpl_filter(STR, int);
int parse_htpl_fetchit(STR, int);
int parse_htpl_else(STR, int);
int parse_htpl_fetch(STR, int);
int parse_htpl_fetch___fwd(STR, int);
int parse_htpl_fetch___rev(STR, int);
int parse_htpl_fetchcell(STR, int);
int parse_htpl_ifnull(STR, int);
int parse_htpl_ifnull___fwd(STR, int);
int parse_htpl_ifnull___rev(STR, int);
int parse_htpl_destructor(STR, int);
int parse_htpl_destructor___fwd(STR, int);
int parse_htpl_destructor___rev(STR, int);
int parse_htpl_if(STR, int);
int parse_htpl_if___fwd(STR, int);
int parse_htpl_if___rev(STR, int);
int parse_htpl_graph(STR, int);
int parse_htpl_switch(STR, int);
int parse_htpl_switch_rnd(STR, int);
int parse_htpl_switch_rnd___fwd(STR, int);
int parse_htpl_switch_rnd___rev(STR, int);
int parse_htpl_switch_case(STR, int);
int parse_htpl_switch_case___fwd(STR, int);
int parse_htpl_switch_case___rev(STR, int);
int parse_htpl_clsutils(STR, int);
int parse_htpl_clsutils_other(STR, int);
int parse_htpl_clsutils_imp(STR, int);
int parse_htpl_clsutils_mine(STR, int);
int parse_htpl_throw(STR, int);
int parse_htpl_project(STR, int);
int parse_htpl_fetchcols(STR, int);
int parse_htpl_call(STR, int);
int parse_htpl_img(STR, int);
int parse_htpl_img_rnd(STR, int);
int parse_htpl_mem(STR, int);
int parse_htpl_mem_immediate(STR, int);
int parse_htpl_mem_search(STR, int);
int parse_htpl_mem_project(STR, int);
int parse_htpl_mem_cursor(STR, int);
int parse_htpl_next(STR, int);
int parse_htpl_try(STR, int);
int parse_htpl_try___fwd(STR, int);
int parse_htpl_try___rev(STR, int);

enum scopevalues {no_scope,
    scope_random_switch,
    scope_foreach,
    scope_procedure,
    scope_if_then_else,
    scope_method,
    scope_fetch,
    scope_switch,
    scope_catch,
    scope_mail,
    scope_if_then,
    scope_contsructor,
    scope_class,
    scope_destructor,
    scope_try,
    scope_for};
#ifdef __HTPARSE__
char *scope_names[] = {"none",
    "random-switch",
    "foreach",
    "procedure",
    "if-then-else",
    "method",
    "fetch",
    "switch",
    "catch",
    "mail",
    "if-then",
    "contsructor",
    "class",
    "destructor",
    "try",
    "for"};
int scope_ids[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
extern char *scope_names[];
extern int scope_ids[];
#endif

#define NUM_MACROS 122
