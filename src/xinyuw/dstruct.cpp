#include "dstruct.h"


using namespace std;

Express::Express()
{
    m_type = Express_Type::ET_UNKNOWN;
    m_varname = m_funcname = NULL;
    m_paramlist = NULL;
    m_left = m_right = NULL;
}

Express::~Express()
{
    if (m_varname)
        free(m_varname);

    if (m_funcname)
        free(m_funcname);

    if (m_paramlist)
    {
        for (size_t i = 0; i < m_paramlist->size(); ++i)
            delete (*m_paramlist)[i];
        delete m_paramlist;
    }

    if (m_left)
        delete m_left;

    if (m_right)
        delete m_right;
}

void Express::set_unknown(const string &why)
{
    m_type = Express_Type::ET_UNKNOWN;
    m_why = why;
}

void Express::set_integer(long v)
{
    m_type = Express_Type::ET_INTEGER;
    m_integer = v;
}

void Express::set_float(double v)
{
    m_type = Express_Type::ET_FLOAT;
    m_float = v;
}

void Express::set_string(char *s)
{
    m_type = Express_Type::ET_STRING;
    m_string.reset(s);
}

void Express::set_vector(Vector *v)
{
    m_type = Express_Type::ET_VECTOR;
    m_vector.reset(v);
}

void Express::set_matrix(Matrix *m)
{
    m_type = Express_Type::ET_MATRIX;
    m_matrix.reset(m);
}

void Express::set_variable(Variable* v)
{
    m_type = v->m_type;
    if (is_integer())
        m_integer = v->m_integer;
    else if (is_float())
        m_float = v->m_float;
    else if (is_vector())
        m_vector = v->m_vector;
    else if (is_matrix())
        m_matrix = v->m_matrix;
}

Statement::Statement()
{
    m_type = Stmt_Type::ST_UNKNOWN;
    m_leftvar = NULL;
    m_express = NULL;
    m_vecdata = NULL;
    m_mtxdata = NULL;
}

Statement::~Statement()
{
    if (m_leftvar)
        free(m_leftvar);

    if (m_express)
        delete m_express;

    if (m_vecdata)
    {
        for (size_t i = 0; i < m_vecdata->size(); ++i)
            delete (*m_vecdata)[i];
        delete m_vecdata;
    }

    if (m_mtxdata)
    {
        for (size_t i = 0; i < m_mtxdata->size(); ++i)
        {
            vector<Express*> *explist = (*m_mtxdata)[i];
            for (size_t j = 0; j < explist->size(); ++j)
                delete (*explist)[j];
            delete explist;
        }
        delete m_mtxdata;
    }
}

Variable::Variable()
{
    m_type = Express_Type::ET_UNKNOWN;
}

Variable::Variable(Express *exp)
{
    m_type = exp->m_type;
    if (exp->is_integer())
        m_integer = exp->m_integer;
    else if (exp->is_float())
        m_float = exp->m_float;
    else if (exp->is_string())
        m_string = exp->m_string;
    else if (exp->is_vector())
        m_vector = exp->m_vector;
    else if (exp->is_matrix())
        m_matrix = exp->m_matrix;
}

Variable::Variable(Vector *v) : m_vector(v)
{
    m_type = Express_Type::ET_VECTOR;
}

Variable::Variable(Matrix *m) : m_matrix(m)
{
    m_type = Express_Type::ET_MATRIX;
}

Variable::Variable(Variable &other) : m_string(other.m_string),
    m_vector(other.m_vector), m_matrix(other.m_matrix)
{
    m_type = other.m_type;
    m_integer = other.m_integer;
    m_float = other.m_float;
}

Variable& Variable::operator=(Variable &other)
{
    m_type = other.m_type;
    if (m_type == Express_Type::ET_INTEGER)
        m_integer = other.m_integer;
    else if (m_type == Express_Type::ET_FLOAT)
        m_float = other.m_float;
    else if (m_type == Express_Type::ET_STRING)
        m_string = other.m_string;
    else if (m_type == Express_Type::ET_VECTOR)
        m_vector = other.m_vector;
    else if (m_type == Express_Type::ET_MATRIX)
        m_matrix = other.m_matrix;
    return *this;
}

