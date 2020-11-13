#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include "xinyuw.h"
#include "syntax.tab.h"


using namespace std;

#define YYERROR_SIZE 100
char yyerror_text[YYERROR_SIZE];
map<string, Variable>   g_var;


void evaluate_func(Express *);
void evaluate_binop(Express *);

void yyerror(std::vector<Statement*>* stmtlist, const char* msg)
{
    snprintf(yyerror_text, YYERROR_SIZE, "%s at \"%s\"", msg, yytext);
}

Express* compose_express(int op, Express* left, Express* right)
{
    Express *compose = new Express();
    compose->m_left = left;
    compose->m_right = right;
    switch (op)
    {
    case '+': compose->m_type = ET_PLUS; break;
    case '-': compose->m_type = ET_MINUS; break;
    case '*': compose->m_type = ET_MULTIPLY; break;
    case '/': compose->m_type = ET_RIGHT_DIVIDE; break;
    case '\\': compose->m_type = ET_LEFT_DIVIDE; break;
    case 'U': compose->m_type = ET_UMINUS; break;
    default: compose->m_type = ET_UNKNOWN;
    }
    return compose;
}

void evaluate(Express *exp)
{
    if (!exp || exp->is_unknown()
        || exp->is_integer() || exp->is_float()
        || exp->is_string() || exp->is_vector()
        || exp->is_matrix())
        return;

    if (exp->is_variable())
    {
        Variable *v = search_var(exp->m_varname);
        if (v)
            exp->set_variable(v);
        else
            exp->set_unknown(string("no such variable: ") + exp->m_varname);
        return;
    }

    if (exp->is_function())
        return evaluate_func(exp);

    if (exp->is_uminus())
    {
        Express *left = exp->m_left;
        evaluate(left);

        if (left->is_integer())
            exp->set_integer(left->m_integer * -1);
        else if (left->is_float())
            exp->set_float(left->m_float * -1);
        else if (left->is_vector())
        {
            Vector *v = vector_scalar_multiply(left->m_vector.get(), -1);
            exp->set_vector(v);
        }
        else if (left->is_matrix())
        {
            Matrix *m = matrix_scalar_multiply(left->m_matrix.get(), -1);
            exp->set_matrix(m);
        }
        else
            exp->set_unknown("unknown expression type");
    }
    else
        evaluate_binop(exp);
}

void evaluate_binop(Express *exp)
{
    Express *left = exp->m_left;
    Express *right = exp->m_right;

    evaluate(left);
    evaluate(right);

    if (left->is_unknown() || right->is_unknown())
        exp->set_unknown("[left]: " + left->m_why + " [right]: " + right->m_why);
    else if (left->is_integer())
    {
        if (right->is_integer())
        {
            if (exp->is_plus())
                exp->set_integer(left->m_integer + right->m_integer);
            else if (exp->is_minus())
                exp->set_integer(left->m_integer - right->m_integer);
            else if (exp->is_multiply())
                exp->set_integer(left->m_integer * right->m_integer);
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_integer(left->m_integer / right->m_integer);   // divide by 0 ? change to float?
        }
        else if (right->is_float())
        {
            if (exp->is_plus())
                exp->set_float(left->m_integer + right->m_float);
            else if (exp->is_minus())
                exp->set_float(left->m_integer - right->m_float);
            else if (exp->is_multiply())
                exp->set_float(left->m_integer * right->m_float);
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_float(left->m_integer / right->m_float);   // divide by 0 ?
        }
        else if (right->is_string())
            exp->set_unknown("number op string is not supported.");
        else if (right->is_vector())
            exp->set_unknown("scalar op vector is not supported.");
        else if (right->is_matrix())
            exp->set_unknown("scalar op matrix is not supported.");
    }
    else if (left->is_float())
    {
        if (right->is_integer())
        {
            if (exp->is_plus())
                exp->set_float(left->m_float + right->m_integer);
            else if (exp->is_minus())
                exp->set_float(left->m_float - right->m_integer);
            else if (exp->is_multiply())
                exp->set_float(left->m_float * right->m_integer);
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_float(left->m_float / right->m_integer);   // divide by 0 ? change to float?
        }
        else if (right->is_float())
        {
            if (exp->is_plus())
                exp->set_float(left->m_float + right->m_float);
            else if (exp->is_minus())
                exp->set_float(left->m_float - right->m_float);
            else if (exp->is_multiply())
                exp->set_float(left->m_float * right->m_float);
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_float(left->m_float / right->m_float);   // divide by 0 ?
        }
        else if (right->is_string())
            exp->set_unknown("number op string is not supported.");
        else if (right->is_vector())
            exp->set_unknown("scalar op vector is not supported.");
        else if (right->is_matrix())
            exp->set_unknown("scalar op matrix is not supported.");
    }
    else if (left->is_string())
    {
        if (right->is_string() && exp->is_plus())
        {
            char *merge = (char*)malloc(strlen(left->m_string.get()) + strlen(right->m_string.get()) + 1);
            strcpy(merge, left->m_string.get());
            strcat(merge, right->m_string.get());
            exp->set_string(merge);
        }
        else
            exp->set_unknown("string operation is invalid.");
    }
    else if (left->is_vector())
    {
        if (right->is_integer())
        {
            if (exp->is_multiply())
                exp->set_vector(vector_scalar_multiply(left->m_vector.get(), right->m_integer));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_vector(vector_scalar_divide(left->m_vector.get(), right->m_integer));
            else
                exp->set_unknown("vector operation is invalid.");
        }
        else if (right->is_float())
        {
            if (exp->is_multiply())
                exp->set_vector(vector_scalar_multiply(left->m_vector.get(), right->m_float));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_vector(vector_scalar_divide(left->m_vector.get(), right->m_float));
            else
                exp->set_unknown("vector operation is invalid.");
        }
        else if (right->is_string())
            exp->set_unknown("vector op string is not supported.");
        else if (right->is_vector())
        {
            if (exp->is_plus())
                exp->set_vector(vector_plus(left->m_vector.get(), right->m_vector.get()));
            else if (exp->is_minus())
                exp->set_vector(vector_minus(left->m_vector.get(), right->m_vector.get()));
            else
                exp->set_unknown("vector operation is invalid.");
        }
        else if (right->is_matrix())
            exp->set_unknown("vector op matrix is invalid.");
    }
    else if (left->is_matrix())
    {
        if (right->is_integer())
        {
            if (exp->is_plus())
                exp->set_matrix(matrix_scalar_plus(left->m_matrix.get(), right->m_integer));
            else if (exp->is_minus())
                exp->set_matrix(matrix_scalar_minus(left->m_matrix.get(), right->m_integer));
            else if (exp->is_multiply())
                exp->set_matrix(matrix_scalar_multiply(left->m_matrix.get(), right->m_integer));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_matrix(matrix_scalar_divide(left->m_matrix.get(), right->m_integer));   // divide by 0 ? change to float?
        }
        else if (right->is_float())
        {
            if (exp->is_plus())
                exp->set_matrix(matrix_scalar_plus(left->m_matrix.get(), right->m_float));
            else if (exp->is_minus())
                exp->set_matrix(matrix_scalar_minus(left->m_matrix.get(), right->m_float));
            else if (exp->is_multiply())
                exp->set_matrix(matrix_scalar_multiply(left->m_matrix.get(), right->m_float));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_matrix(matrix_scalar_divide(left->m_matrix.get(), right->m_float));   // divide by 0 ? change to float?
        }
        else if (right->is_string())
            exp->set_unknown("matrix op string is not supported.");
        else if (right->is_matrix())
        {
            if (exp->is_plus())
                exp->set_matrix(matrix_plus(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_minus())
                exp->set_matrix(matrix_minus(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_multiply())
                exp->set_matrix(matrix_multiply(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_r_divide())
                exp->set_matrix(matrix_right_divide(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_l_divide())
                exp->set_matrix(matrix_left_divide(left->m_matrix.get(), right->m_matrix.get()));
        }
    }
}

Matrix *matrix_api(const char* fn, long p1)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1);
    else
        return NULL;
}

Matrix *matrix_api(const char* fn, long p1, long p2)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1, p2);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1, p2);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1, p2);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1, p2);
    else
        return NULL;
}

Matrix *matrix_api(const char* fn, long p1, Data_Type p2)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1, p2);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1, p2);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1, p2);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1, p2);
    else
        return NULL;
}

Matrix *matrix_api(const char* fn, long p1, long p2, Data_Type p3)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1, p2, p3);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1, p2, p3);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1, p2, p3);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1, p2, p3);
    else
        return NULL;
}

void evaluate_func(Express *exp)
{
    vector<Express*> &pm = *exp->m_paramlist;
    const size_t num = pm.size();
    const char* fn = exp->m_funcname;
    Data_Type dt;

    for (size_t i = 0; i < num; ++i)
    {
        evaluate(pm[i]);
        if (pm[i]->is_unknown())
        {
            char buf[100];
            sprintf(buf, "the %dth param is invalid.", i + 1);
            exp->set_unknown(buf);
            return;
        }
    }

    if (!strcmp(fn, "zeros") || !strcmp(fn, "ones") || !strcmp(fn, "eye")
        || !strcmp(fn, "rand"))
    {
        if (num == 1)
        {
            if (pm[0]->is_integer() && pm[0]->m_integer >= 0)
            {
                exp->set_matrix(matrix_api(fn, pm[0]->m_integer));
                return;
            }
        }
        else if (num == 2)
        {
            if (pm[0]->is_integer() && pm[0]->m_integer >= 0)
            {
                if (pm[1]->is_integer() && pm[1]->m_integer >= 0)
                {
                    exp->set_matrix(matrix_api(fn, pm[0]->m_integer, pm[1]->m_integer));
                    return;
                }
                if (pm[1]->is_string() && (dt = name_to_dt(pm[1]->m_string.get())))
                {
                    exp->set_matrix(matrix_api(fn, pm[0]->m_integer, dt));
                    return;
                }
            }
        }
        else if (num == 3)
        {
            if (pm[0]->is_integer() && pm[0]->m_integer >= 0
                && pm[1]->is_integer() && pm[1]->m_integer >= 0
                && pm[2]->is_string() && (dt = name_to_dt(pm[2]->m_string.get())))
            {
                exp->set_matrix(matrix_api(fn, pm[0]->m_integer, pm[1]->m_integer, dt));
                return;
            }
        }
    }
    else if (!strcmp(fn, "diag"))
    {
        if (num == 1)
        {
            if (pm[0]->is_vector())
            {
                exp->set_matrix(matrix_diag(pm[0]->m_vector.get()));
                return;
            }
            else if (pm[0]->is_matrix())
            {
                exp->set_vector(matrix_diag(pm[0]->m_matrix.get()));
                return;
            }
        }
        else if (num == 2)
        {
            if (pm[0]->is_vector() && pm[1]->is_integer())
            {
                exp->set_matrix(matrix_diag(pm[0]->m_vector.get(), pm[1]->m_integer));
                return;
            }
            else if (pm[0]->is_matrix() && pm[1]->is_integer())
            {
                exp->set_vector(matrix_diag(pm[0]->m_matrix.get(), pm[1]->m_integer));
                return;
            }
        }
    }
    else if (!strcmp(fn, "blkdiag"))
    {
        vector<Matrix*> mx;
        for (size_t i = 0; i < num - 1; ++i)
        {
            if (pm[i]->is_matrix())
                mx.push_back(pm[i]->m_matrix.get());
            else
                goto wrong_param;
        }
        if (pm[num - 1]->is_matrix())
        {
            mx.push_back(pm[num - 1]->m_matrix.get());
            exp->set_matrix(matrix_blkdiag(mx.begin(), mx.end()));
            return;
        }
        else if (pm[num - 1]->is_string() && (dt = name_to_dt(pm[num - 1]->m_string.get())))
        {
            exp->set_matrix(matrix_blkdiag(mx.begin(), mx.end(), dt));
            return;
        }
    }
    else if (!strcmp(fn, "cat"))
    {
        vector<Matrix*> mx;
        if (num >= 2 && pm[0]->is_integer() && pm[0]->m_integer > 0)
        {
            for(size_t i = 1; i < num - 1; ++i)
            {
                if (pm[i]->is_matrix())
                    mx.push_back(pm[i]->m_matrix.get());
                else
                    goto wrong_param;
            }
            if (pm[num - 1]->is_matrix())
            {
                mx.push_back(pm[num - 1]->m_matrix.get());
                exp->set_matrix(matrix_cat(pm[0]->m_integer, mx.begin(), mx.end()));
                return;
            }
            else if (pm[num - 1]->is_string() && (dt = name_to_dt(pm[num - 1]->m_string.get())))
            {
                exp->set_matrix(matrix_cat(pm[0]->m_integer, mx.begin(), mx.end(), dt));
                return;
            }
        }
    }
    else if (!strcmp(fn, "transpose"))
    {
        if (num == 1 && pm[0]->is_matrix())
        {
            exp->set_matrix(matrix_transpose(pm[0]->m_matrix.get()));
            return;
        }
    }
    else if (!strcmp(fn, "inv"))
    {
        if (num == 1 && pm[0]->is_matrix())
        {
            exp->set_matrix(matrix_inverse(pm[0]->m_matrix.get()));
            return;
        }
    }
    else if (!strcmp(fn, "det"))
    {
        if (num == 1 && pm[0]->is_matrix())
        {
            exp->set_float(matrix_determinant(pm[0]->m_matrix.get()));
            return;
        }
    }

wrong_param:
    exp->set_unknown(string(fn) + " with the param list is unknown.");
}

Variable* search_var(const char* vn)
{
    map<string, Variable>::iterator ite = g_var.find(vn);
    if (ite != g_var.end())
        return &(ite->second);
    else
        return NULL;
}

void set_var(const char* vn, Variable &var)
{
    g_var[vn] = var;
}

void delete_explist(vector<Express*>* explist)
{
    for (size_t i = 0; i < explist->size(); ++i)
        delete (*explist)[i];
    delete explist;
}

void delete_mtxrows(vector<vector<Express*>*> *rows)
{
    for (size_t i = 0; i < rows->size(); ++i)
    {
        vector<Express*> *explist = (*rows)[i];
        for (size_t j = 0; j < explist->size(); ++j)
            delete (*explist)[j];
        delete explist;
    }
    delete rows;
}

int main(int argc, char *argv[])
{
    vector<Statement*> stmtlist;
    while (1)
    {
        printf(">");
        yyparse(&stmtlist);
        for (int i = 0; i < stmtlist.size(); ++i)
        {
            Statement* stmt = stmtlist[i];
            if (stmt->m_type == Stmt_Type::ST_ERROR)
                printf("ERROR: %s\n", stmt->m_error.c_str());
            else if (stmt->m_type == Stmt_Type::ST_EXPRESS)
            {
                evaluate(stmt->m_express);
                if (stmt->m_express->is_integer())
                    printf("%ld\n", stmt->m_express->m_integer);
                else if (stmt->m_express->is_float())
                    printf("%lf\n", stmt->m_express->m_float);
                else if (stmt->m_express->is_string())
                    printf("%s\n", stmt->m_express->m_string.get());
                else if (stmt->m_express->is_vector())
                    vector_print(stmt->m_express->m_vector.get());
                else if (stmt->m_express->is_matrix())
                    matrix_print(stmt->m_express->m_matrix.get());
                else
                    printf("reason: %s\n", stmt->m_express->m_why.c_str());
            }
            else if (stmt->m_type == Stmt_Type::ST_ASSIGN)
            {
                evaluate(stmt->m_express);
                if (stmt->m_express->is_unknown())
                    printf("fail to assign: %s\n", stmt->m_express->m_why.c_str());
                else
                {
                    Variable var(stmt->m_express);
                    set_var(stmt->m_leftvar, var);
                }
            }
            else if (stmt->m_type == Stmt_Type::ST_INIT_VECTOR)
            {
                Vector *v = NULL;
                Data_Type dt = Data_Type::dt_int64;
                vector<double> data;
                for (size_t i = 0; i < stmt->m_vecdata->size(); ++i)
                {
                    Express *e = (*stmt->m_vecdata)[i];
                    evaluate(e);
                    if (e->is_integer())
                        data.push_back(e->m_integer);
                    else if (e->is_float())
                    {
                        data.push_back(e->m_float);
                        dt = Data_Type::dt_double;
                    }
                    else
                    {
                        printf("the unit [%d] is invalid data for Vector!\n", i);
                        goto vector_exit;
                    }
                }
                v = vector_new(stmt->m_vecdata->size(), dt);
                vector_assign(v, data.begin(), data.end());
vector_exit:
                if (v)
                {
                    Variable var(v);
                    set_var(stmt->m_leftvar, var);
                }
            }        
            else if (stmt->m_type == Stmt_Type::ST_INIT_MATRIX)
            {
                Matrix *m = NULL;
                size_t cols;
                Data_Type dt = Data_Type::dt_int64;
                vector<double> data;
                if (!stmt->m_mtxdata->size() || !(cols = (*stmt->m_mtxdata)[0]->size()))
                    m = matrix_new(0, 0);
                else
                {
                    for (size_t i = 0; i < stmt->m_mtxdata->size(); ++i)
                    {
                        vector<Express*> *explist = (*stmt->m_mtxdata)[i];
                        if (explist->size() != cols)
                        {
                            printf("different size of columns!\n");
                            goto matrix_exit;
                        }
                        for (size_t j = 0; j < explist->size(); ++j)
                        {
                            Express *e = (*explist)[j];
                            evaluate(e);
                            if (e->is_integer())
                                data.push_back(e->m_integer);
                            else if (e->is_float())
                            {
                                data.push_back(e->m_float);
                                dt = Data_Type::dt_double;
                            }
                            else
                            {
                                printf("the unit [%d %d] is invalid data for Matrix!\n", i, j);
                                goto matrix_exit;
                            }
                        }
                    }
                    m = matrix_new(stmt->m_mtxdata->size(), cols, dt);
                    matrix_assign(m, data.begin(), data.end());
                }
matrix_exit:
                if (m)
                {
                    Variable v(m);
                    set_var(stmt->m_leftvar, v);
                }
            }
            else if (stmt->m_type == Stmt_Type::ST_EOF)
            {
                printf("good-bye\n");
                exit(0);
            }
            else
                printf("What's the statement type %d ?\n", stmt->m_type);

            delete stmt;
            stmtlist[i] = NULL;
        }
        stmtlist.clear();
    }
    return 0;
}

