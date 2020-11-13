#include "matrix_impl.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <memory>


using namespace std;

/***********************************************************************
 * Specific type of Matrix definition
 ***********************************************************************/

Matrix_double::Matrix_double(size_t m, size_t n) : Matrix_T<double>(m, n)
{
    m_type = Data_Type::dt_double;
}

Matrix_single::Matrix_single(size_t m, size_t n) : Matrix_T<float>(m, n)
{
    m_type = Data_Type::dt_single;
}

Matrix_int8::Matrix_int8(size_t m, size_t n) : Matrix_T<char>(m, n)
{
    m_type = Data_Type::dt_int8;
}

Matrix_int16::Matrix_int16(size_t m, size_t n) : Matrix_T<short>(m, n)
{
    m_type = Data_Type::dt_int16;
}

Matrix_int32::Matrix_int32(size_t m, size_t n) : Matrix_T<int>(m, n)
{
    m_type = Data_Type::dt_int32;
}

Matrix_int64::Matrix_int64(size_t m, size_t n) : Matrix_T<long>(m, n)
{
    m_type = Data_Type::dt_int64;
}

Matrix_uint8::Matrix_uint8(size_t m, size_t n) : Matrix_T<unsigned char>(m, n)
{
    m_type = Data_Type::dt_uint8;
}

Matrix_uint16::Matrix_uint16(size_t m, size_t n) : Matrix_T<unsigned short>(m, n)
{
    m_type = Data_Type::dt_uint16;
}

Matrix_uint32::Matrix_uint32(size_t m, size_t n) : Matrix_T<unsigned int>(m, n)
{
    m_type = Data_Type::dt_uint32;
}

Matrix_uint64::Matrix_uint64(size_t m, size_t n) : Matrix_T<unsigned long>(m, n)
{
    m_type = Data_Type::dt_uint64;
}

/***********************************************************************
 * internal functions
 ***********************************************************************/

static void init_rand()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srandom(ts.tv_nsec);
}

static Matrix* matrix_create(size_t m, size_t n, Data_Type dt)
{
    switch (dt)
    {
    case dt_double:
        return new Matrix_double(m, n);
    case dt_single:
        return new Matrix_single(m, n);
    case dt_int8:
        return new Matrix_int8(m, n);
    case dt_int16:
        return new Matrix_int16(m, n);
    case dt_int32:
        return new Matrix_int32(m, n);
    case dt_int64:
        return new Matrix_int64(m, n);
    case dt_uint8:
        return new Matrix_uint8(m, n);
    case dt_uint16:
        return new Matrix_uint16(m, n);
    case dt_uint32:
        return new Matrix_uint32(m, n);
    case dt_uint64:
        return new Matrix_uint64(m, n);
    default:
        error("wrong Data_Type");
        exit(1);
    }
}

static Matrix* matrix_init_all(size_t n, Data_Type dt, int value)
{
    Matrix *m = matrix_create(n, n, dt);
    m->init_all(value);
    return m;
}

static Matrix* matrix_init_all(size_t m, size_t n, Data_Type dt, int value)
{
    Matrix *mx = matrix_create(m, n, dt);
    mx->init_all(value);
    return mx;
}

static Matrix* matrix_init_all(Vector *vec, Data_Type dt, int value)
{
    Vector_T<unsigned int> *vuint32 = NULL;

    if (!vec || vec->get_size() < 2 || !(vuint32 = dynamic_cast<Vector_T<unsigned int>*>(vec)))
    {
        error("wrong Vector");
        return NULL;
    }

    unsigned int *data = (unsigned int*)vec->get_data();
    if (!data)
    {
        error("NULL data in Vector");
        return NULL;
    }

    Matrix *mx = matrix_create(data[0], data[1], dt);
    mx->init_all(value);
    return mx;
}

/**
 * get sub Matrix in order to calculate determinant of the input Matrix
 */
static Matrix* matrix_det_sub(Matrix *mx, size_t m, size_t n)
{
    // check is done by caller
    Matrix_Dim dim = mx->get_size();
    Data_Type dt = mx->get_type();

    Matrix *sub = matrix_create(dim.row - 1, dim.col - 1, dt);
    char *data = (char*)sub->get_data();
    size_t us = sub->get_unit_size();
    for (size_t i = 0; i < dim.row; ++i)
        for (size_t j = 0; j < dim.col; ++j)
            if (i != m && j != n)
            {
                memcpy(data, mx->access_unit(i, j), us);
                data += us;
            }

    return sub;
}


/***********************************************************************
 * API functions
 ***********************************************************************/

/**
 * API: print()
 */
void matrix_print(Matrix *m)
{
    if (m)
        m->print();
    else
        error("Matrix is NULL");
}

/**
 * create a Matrix without initializaion
 */
Matrix* matrix_new(size_t m, size_t n, Data_Type dt)
{
    return matrix_create(m, n, dt);
}

/**
 * copy a Matrix along with data
 */
Matrix* matrix_copy(Matrix *m)
{
    if (!m)
        return NULL;

    Matrix_Dim dim = m->get_size();
    Matrix *n = matrix_create(dim.row, dim.col, m->get_type());
    memcpy(n->get_data(), m->get_data(), m->get_unit_size() * dim.row * dim.col);
    return n;
}

/**
 * API: zeros(size_t)
 */
Matrix* matrix_zeros(size_t n, Data_Type dt)
{
    return matrix_init_all(n, dt, 0);
}

/**
 * API: zeros(size_t, size_t)
 */
Matrix* matrix_zeros(size_t m, size_t n, Data_Type dt)
{
    return matrix_init_all(m, n, dt, 0);
}

/**
 * API: zeros(Vector)    // must be Vector_T<unsigned int>
 */
Matrix* matrix_zeros(Vector *vec, Data_Type dt)
{
    return matrix_init_all(vec, dt, 0);
}

/**
 * API: ones(size_t)
 */
Matrix* matrix_ones(size_t n, Data_Type dt)
{
    return matrix_init_all(n, dt, 1);
}

/**
 * API: ones(size_t, size_t)
 */
Matrix* matrix_ones(size_t m, size_t n, Data_Type dt)
{
    return matrix_init_all(m, n, dt, 1);
}

/**
 * API: zeros(Vector)    // must be Vector_T<unsigned int>
 */
Matrix* matrix_ones(Vector *vec, Data_Type dt)
{
    return matrix_init_all(vec, dt, 1);
}

/**
 * API: eye(int)
 */
Matrix* matrix_eye(size_t n, Data_Type dt)
{
    Matrix *m = matrix_create(n, n, dt);
    m->init_eye();
    return m;
}

/**
 * API: eye(size_t, size_t)
 */
Matrix* matrix_eye(size_t m, size_t n, Data_Type dt)
{
    Matrix *mx = matrix_create(m, n, dt);
    mx->init_eye();
    return mx;
}

/**
 * API: eye(Vector)    must be a Vector_T<unsigned int>
 */
Matrix* matrix_eye(Vector *vec, Data_Type dt)
{
    Vector_T<unsigned int> *vuint32 = NULL;

    if (!vec || vec->get_size() < 2 || !(vuint32 = dynamic_cast<Vector_T<unsigned int>*>(vec)))
    {
        error("wrong Vector");
        return NULL;
    }

    unsigned int *data = (unsigned int*)vec->get_data();
    if (!data)
    {
        error("NULL data in Vector");
        return NULL;
    }

    Matrix *m = matrix_create(data[0], data[1], dt);
    m->init_eye();
    return m;
}

/**
 * API: rand()
 */
double matrix_rand()
{
    init_rand();
    return rand() / (RAND_MAX + 1.0);
}

/**
 * API: rand(size_t)
 */
Matrix* matrix_rand(size_t n, Data_Type dt)
{
    init_rand();
    Matrix *m = matrix_create(n, n, dt);
    m->init_rand();
    return m;
}

/**
 * API: rand(size_t, size_t)
 */
Matrix* matrix_rand(size_t m, size_t n, Data_Type dt)
{
    init_rand();
    Matrix *mx = matrix_create(m, n, dt);
    mx->init_rand();
    return mx;
}

/**
 * API: rand(Vector)    must be a Vector_T<unsigned int>
 */
Matrix* matrix_rand(Vector *vec, Data_Type dt)
{
    Vector_T<unsigned int> *vuint32 = NULL;

    if (!vec || vec->get_size() < 2 || !(vuint32 = dynamic_cast<Vector_T<unsigned int>*>(vec)))
    {
        error("wrong Vector");
        return NULL;
    }

    unsigned int *data = (unsigned int*)vec->get_data();
    if (!data)
    {
        error("NULL data in Vector");
        return NULL;
    }

    Matrix *m = matrix_create(data[0], data[1], dt);
    m->init_rand();
    return m;
}

/**
 * API: diag(Vector, int)
 */
Matrix *matrix_diag(Vector *vec, int k)
{
    if (!vec || !vec->get_data() || !vec->get_size())
    {
        error("wrong Vector");
        return NULL;
    }

    size_t m = vec->get_size();
    size_t n = m + abs(k);
    Matrix *mx = matrix_create(n, n, vec->get_type());
    mx->init_diag(vec, k);
    return mx;
}

/**
 * API: diag(Matrix)
 */
Vector* matrix_diag(Matrix *m, int k)
{
    return m->diag(k);
}

/**
 * Matrix addition
 */
Matrix* matrix_plus(Matrix *one, Matrix *two, Data_Type dt)
{
    if (!one || !two)
    {
        error("NULL Matrix");
        return NULL;
    }

    Matrix_Dim dim1 = one->get_size();
    Matrix_Dim dim2 = two->get_size();
    if (!(dim1 == dim2))
    {
        error("invalid size");
        return NULL;
    }

    Matrix* m = matrix_new(dim1.row, dim1.col, dt);
    for (size_t i = 0; i < dim1.row; ++i)
        for (size_t j = 0; j < dim1.col; ++j)
        {
            m->set_unit(i, j, one->access_unit(i, j), one->get_type());
            m->add_unit(i, j, two->access_unit(i, j), two->get_type());
        }
    return m;
}

/**
 * Matrix addition, the returned Matrix take type from one
 */
Matrix* matrix_plus(Matrix *one, Matrix *two)
{
    if (!one || !two)
    {
        error("NULL Matrix");
        return NULL;
    }

    Matrix_Dim dim1 = one->get_size();
    Matrix_Dim dim2 = two->get_size();
    if (!(dim1 == dim2))
    {
        error("invalid size");
        return NULL;
    }

    Matrix* m = matrix_copy(one);
    for (size_t i = 0; i < dim1.row; ++i)
        for (size_t j = 0; j < dim1.col; ++j)
            m->add_unit(i, j, two->access_unit(i, j), two->get_type());
    return m;
}

/**
 * Matrix subtraction
 */
Matrix* matrix_minus(Matrix *one, Matrix *two, Data_Type dt)
{
    if (!one || !two)
    {
        error("NULL Matrix");
        return NULL;
    }

    Matrix_Dim dim1 = one->get_size();
    Matrix_Dim dim2 = two->get_size();
    if (!(dim1 == dim2))
    {
        error("invalid size");
        return NULL;
    }

    Matrix* m = matrix_new(dim1.row, dim1.col, dt);
    for (size_t i = 0; i < dim1.row; ++i)
        for (size_t j = 0; j < dim1.col; ++j)
        {
            m->set_unit(i, j, one->access_unit(i, j), one->get_type());
            m->minus_unit(i, j, two->access_unit(i, j), two->get_type());
        }
    return m;
}

/**
 * Matrix subtraction, the returned Matrix take type from one
 */
Matrix* matrix_minus(Matrix *one, Matrix *two)
{
    if (!one || !two)
    {
        error("NULL Matrix");
        return NULL;
    }

    Matrix_Dim dim1 = one->get_size();
    Matrix_Dim dim2 = two->get_size();
    if (!(dim1 == dim2))
    {
        error("invalid size");
        return NULL;
    }

    Matrix* m = matrix_copy(one);
    for (size_t i = 0; i < dim1.row; ++i)
        for (size_t j = 0; j < dim1.col; ++j)
            m->minus_unit(i, j, two->access_unit(i, j), two->get_type());
    return m;
}

/**
 * Matrix multiply
 */
Matrix* matrix_multiply(Matrix *one, Matrix *two, Data_Type dt)
{
    if (!one || !two)
    {
        error("NULL Matrix");
        return NULL;
    }

    Matrix_Dim dim1 = one->get_size();
    Matrix_Dim dim2 = two->get_size();
    if (dim1.col != dim2.row)
    {
        error("invalid size");
        return NULL;
    }

    Matrix* m = matrix_zeros(dim1.row, dim2.col, dt);
    for (size_t i = 0; i < dim1.row; ++i)
        for (size_t j = 0; j < dim2.col; ++j)
        {
            double sum = 0;
            for (size_t k = 0; k < dim1.col; ++k)
            {
                double unit1 = one->get_unit(i, k);
                double unit2 = two->get_unit(k, j);
                sum += unit1 * unit2;
            }
            m->set_unit(i, j, &sum, Data_Type::dt_double);
        }

    return m;
}

/**
 * Matrix multiply, the returned Matrix take type from one
 */
Matrix* matrix_multiply(Matrix *one, Matrix *two)
{
    if (!one)
    {
        error("NULL Matrix");
        return NULL;
    }

    return matrix_multiply(one, two, one->get_type());
}

/**
 * Matrix inversion
 */
Matrix* matrix_inverse(Matrix *m)
{
    if (!m)
    {
        error("NULL matrix");
        return NULL;
    }

    Matrix_Dim dim = m->get_size();
    if (!dim.row || dim.row != dim.col)
    {
        error("invalid dimension");
        return NULL;
    }

    unique_ptr<Matrix> e(matrix_eye(dim.row, m->get_type()));
    Matrix *group[2] = { m, e.get() };
    unique_ptr<Matrix> c(matrix_cat(2, group, group + 2));

    for (size_t i = 0; i < dim.col; ++i)
    {
        if (c->get_unit(i, i) == 0.0d)
        {
            size_t j = 0;
            for (; j < dim.row; ++j)
                if (c->get_unit(j, i) != 0.0d)
                    break;
            if (j == dim.row)
            {
                error("singular matrix");
                return NULL;
            }
            c->swap_row(i, j);
        }
        c->divide_row(i, i);
        c->self_transform(i, i);
    }

    Matrix *mx = matrix_new(dim.row, dim.col);
    for (size_t i = 0; i < dim.row; ++i)
        for (size_t j = 0; j < dim.col; ++j)
            mx->set_unit(i, j, c->get_unit(i, j + dim.col));

    return mx;
}

/**
 * Matrix right division
 */
Matrix* matrix_right_divide(Matrix *one, Matrix *two)
{
    if (!one || !two)
    {
        error("NULL Matrix");
        return NULL;
    }

    Matrix_Dim dim1 = one->get_size();
    Matrix_Dim dim2 = two->get_size();
    if (dim2.row != dim2.col || dim1.col != dim2.row)
    {
        error("invalid size");
        return NULL;
    }

    unique_ptr<Matrix> m(matrix_inverse(two));
    if (!m)
    {
        error("get NULL inverse matrix");
        return NULL;
    }

    return matrix_multiply(one, m.get(), m->get_type());
}

/**
 * Matrix left division
 */
Matrix* matrix_left_divide(Matrix *one, Matrix *two)
{
    if (!one || !two)
    {
        error("NULL Matrix");
        return NULL;
    }

    Matrix_Dim dim1 = one->get_size();
    Matrix_Dim dim2 = two->get_size();
    if (dim1.row != dim1.col || dim1.col != dim2.row)
    {
        error("invalid size");
        return NULL;
    }

    unique_ptr<Matrix> m(matrix_inverse(one));
    if (!m)
    {
        error("get NULL inverse matrix");
        return NULL;
    }

    return matrix_multiply(m.get(), two, m->get_type());
}

/**
 * Matrix scalar addition, return a new Matrix
 */
Matrix* matrix_scalar_plus(Matrix *m, double v)
{
    Matrix* n = matrix_copy(m);
    if (!n)
    {
        error("NULL matirx");
        return NULL;
    }
    n->scalar_plus(v);
    return n;
}

/**
 * Matrix scalar subtraction, return a new Matrix
 */
Matrix* matrix_scalar_minus(Matrix *m, double v)
{
    Matrix* n = matrix_copy(m);
    if (!n)
    {
        error("NULL matirx");
        return NULL;
    }
    n->scalar_minus(v);
    return n;
}

/**
 * Matrix scalar multiply, return a new Matrix
 */
Matrix* matrix_scalar_multiply(Matrix *m, double v)
{
    Matrix* n = matrix_copy(m);
    if (!n)
    {
        error("NULL matirx");
        return NULL;
    }
    n->scalar_multiply(v);
    return n;
}

/**
 * Matrix scalar division, return a new Matrix
 */
Matrix* matrix_scalar_divide(Matrix *m, double v)
{
    if (v == 0.0d)
    {
        error("divide by 0");
        return NULL;
    }

    Matrix* n = matrix_copy(m);
    if (!n)
    {
        error("NULL matirx");
        return NULL;
    }
    n->scalar_divide(v);
    return n;
}

/**
 * transpose a Matrix, return a new one
 */
Matrix* matrix_transpose(Matrix *m)
{
    if (!m)
    {
        error("NULL matirx");
        return NULL;
    }

    Matrix_Dim dim = m->get_size();
    Data_Type dt = m->get_type();
    Matrix* n = matrix_create(dim.col, dim.row, dt);
    for (size_t i = 0; i < dim.col; ++i)
        for (size_t j = 0; j < dim.row; ++j)
            n->set_unit(i, j, m->access_unit(j, i), dt);

    return n;
}

/**
 * matrix determinant
 */
double matrix_determinant(Matrix *m)
{
    if (!m)
    {
        error("NULL matirx");
        return 0.0d;
    }

    Matrix_Dim dim = m->get_size();
    if (dim.row != dim.col)
    {
        error("not a square matirx");
        return 0.0d;
    }

    if (dim.row == 1)
        return m->get_unit(0);

    double sum = 0.0d;
    int sign = 1;
    for (size_t i = 0; i < dim.col; ++i)
    {
        unique_ptr<Matrix> sub(matrix_det_sub(m, 0, i));
        sum += sign * m->get_unit(0, i) * matrix_determinant(sub.get());
        sign *= -1;
    }

    return sum;
}

