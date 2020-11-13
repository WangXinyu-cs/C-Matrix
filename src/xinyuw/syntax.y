%parse-param { std::vector<Statement*> *stmt_list }

%{
#include "dstruct.h"
#include "xinyuw.h"
%}

%union
{
    char    *name;
    char    *number;
    char    *string;
    Express     *express;
    std::vector<Express*>   *explist;
    std::vector<std::vector<Express*>*> *mtx_rows;
    Statement   *stmt;
}

%token <name>   VARNAME FUNCTION
%token <number> INTEGER FLOAT
%token <string> STRING
%token EOL YYEOF QUIT

%type <express> express
%type <stmt>    stmt assign
%type <explist> explist paramlist
%type <mtx_rows>    mtx_rows


%left '+' '-'
%left '*' '/' '\\'
%precedence  UMINUS

%destructor { free($$); } <name> <number> <string>
%destructor { delete $$; } <express>
%destructor { delete $$; } <stmt>
%destructor { delete_explist($$); } <explist>
%destructor { delete_mtxrows($$); } <mtx_rows>

%start stmtlist

%%

stmtlist
  : stmtprefix EOL  { YYACCEPT; }
  | stmtprefix stmt EOL
    {
        stmt_list->push_back($2);
        YYACCEPT;
    }
  | stmtprefix error EOL
    {
        Statement *st = new Statement();
        st->m_type = Stmt_Type::ST_ERROR;
        st->m_error = yyerror_text;
        stmt_list->push_back(st);
        YYABORT;
    }
  | YYEOF
    {
        Statement *st = new Statement();
        st->m_type = Stmt_Type::ST_EOF;
        stmt_list->push_back(st);
        YYABORT;
    }
  | QUIT EOL
    {
        Statement *st = new Statement();
        st->m_type = Stmt_Type::ST_EOF;
        stmt_list->push_back(st);
        YYABORT;
    }
  ;

stmtprefix
  : %empty
  | stmtprefix stmt ';'
    {
        stmt_list->push_back($2);
    }
  ;

stmt
  : express
    {
        $$ = new Statement();
        $$->m_type = Stmt_Type::ST_EXPRESS;
        $$->m_express = $1;
    }
  | assign
    {
        $$ = $1;
    }
  ;

express
  : express '+' express { $$ = compose_express('+', $1, $3); }
  | express '-' express { $$ = compose_express('-', $1, $3); }
  | express '*' express { $$ = compose_express('*', $1, $3); }
  | express '/' express { $$ = compose_express('/', $1, $3); }
  | express '\\' express { $$ = compose_express('\\', $1, $3); }
  | '(' express ')' { $$ = $2; }
  | VARNAME
    {
        $$ = new Express();
        $$->m_type = ET_VARIABLE;
        $$->m_varname = $1;
    }
  | '-' express %prec UMINUS { $$ = compose_express('U', $2, NULL); }
  | INTEGER
    {
        $$ = new Express();
        $$->set_integer(atol($1));
    }
  | FLOAT
    {
        $$ = new Express();
        $$->set_float(atof($1));
    }
  | STRING
    {
        $$ = new Express();
        $$->set_string($1);
    }
  | FUNCTION '(' paramlist ')'
    {
        $$ = new Express();
        $$->m_type = Express_Type::ET_FUNCTION;
        $$->m_funcname = $1;
        $$->m_paramlist = $3;
    }
  ;

assign
  : VARNAME '=' express
    {
        $$ = new Statement();
        $$->m_type = Stmt_Type::ST_ASSIGN;
        $$->m_leftvar = $1;
        $$->m_express = $3;
    }
  | VARNAME '=' '[' mtx_rows ']'
    {
        $$ = new Statement();
        $$->m_type = Stmt_Type::ST_INIT_MATRIX;
        $$->m_leftvar = $1;
        $$->m_mtxdata = $4;
    }
  | VARNAME '=' '<' explist '>'
    {
        $$ = new Statement();
        $$->m_type = Stmt_Type::ST_INIT_VECTOR;
        $$->m_leftvar = $1;
        $$->m_vecdata = $4;
    }
  ;

mtx_rows
  : explist
    {
        $$ = new std::vector<std::vector<Express*>*>();
        $$->push_back($1);
    }
  | mtx_rows row_sepa explist
    {
        $1->push_back($3);
        $$ = $1;
    }
  ;

explist
  : express
    {
        $$ = new std::vector<Express*>();
        $$->push_back($1);
    }
  | explist ',' express
    {
        $1->push_back($3);
        $$ = $1;
    }
  ;

row_sepa
  : ';'
  | EOL
  | ';' EOL
  ;

paramlist
  : %empty  { $$ = new std::vector<Express*>(); }
  | explist
    { $$ = $1; }
  ;
