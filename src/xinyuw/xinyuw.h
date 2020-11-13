#ifndef _XINYUW_H_
#define _XINYUW_H_

#include "dstruct.h"

extern char *yytext;
extern char yyerror_text[];
extern int yylex (void);

void yyerror(std::vector<Statement*>*, const char*);
int yyparse (std::vector<Statement*> *stmt_list);
Express* compose_express(int, Express*, Express*);
Variable* search_var(const char*);
void set_var(const char*, Variable&);
void delete_explist(std::vector<Express*>*);
void delete_mtxrows(std::vector<std::vector<Express*>*>*);

#endif

