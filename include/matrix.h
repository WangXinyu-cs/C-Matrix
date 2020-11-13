#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "datatype.h"
#include "vector.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>


struct Matrix_Dim
{
    size_t row, col;
    Matrix_Dim() : row(0), col(0) {}
    int operator==(Matrix_Dim &dim)
    {
        return row == dim.row && col == dim.col;
    }
};

/**
 * A 2-dimension matrix
 */
class Matrix
{
public:
    virtual ~Matrix()
    {
    }

    virtual Data_Type get_type() = 0;
    virtual void* get_data() = 0;
    virtual Matrix_Dim get_size() = 0;
    virtual size_t get_unit_size() = 0;
    virtual void print() = 0;
    virtual void assign_stock(size_t, size_t) = 0;  // not used yet
    virtual void *access_unit(size_t, size_t) = 0;    // row and col
    virtual void *access_unit(size_t) = 0;                // flat size up to row*col
    virtual double get_unit(size_t, size_t) = 0;        // row and col
    virtual double get_unit(size_t) = 0;                // flat size up to row*col
    virtual void set_unit(size_t, size_t, void*, Data_Type) = 0;
    virtual void set_unit(size_t, size_t, double) = 0;    // row and col
    virtual void set_unit(size_t, double) = 0;            // flat size up to row*col
    virtual void add_unit(size_t, size_t, void*, Data_Type) = 0;
    virtual void minus_unit(size_t, size_t, void*, Data_Type) = 0;
    virtual void swap_row(size_t, size_t) = 0;
    virtual void divide_row(size_t, size_t) = 0;
    virtual void self_transform(size_t, size_t) = 0;

public:
    virtual void init_all(int) = 0;
    virtual void init_eye() = 0;
    virtual void init_rand() = 0;
    virtual void init_diag(Vector*, int) = 0;
    virtual Vector* diag(int) = 0;
    virtual void scalar_plus(double) = 0;
    virtual void scalar_minus(double) = 0;
    virtual void scalar_multiply(double) = 0;
    virtual void scalar_divide(double) = 0;
};

template<class T> class Matrix_T: public Matrix
{
protected:
    T *m_data;
    Matrix_Dim    m_dim;
    Data_Type    m_type;

public:

    Matrix_T(size_t m, size_t n)
    {
        m_data = (T*) malloc(sizeof(T) * m * n);
        m_dim.row = m;
        m_dim.col = n;
    }

    virtual ~Matrix_T()
    {
        if (m_data)
        {
            free(m_data);
            m_data = NULL;
            m_dim.row = m_dim.col = 0;
        }
    }

    virtual Data_Type get_type() {    return m_type; }
    virtual void* get_data() { return m_data; }
    virtual Matrix_Dim get_size() { return m_dim; }
    virtual size_t get_unit_size() { return sizeof(T); }

    virtual void print()
    {
        using namespace std;
        string head, format;
        char buf[100];
        vector<size_t> max(m_dim.col, 0);
        vector<vector<string>> text(m_dim.row);

        if (!m_data || !m_dim.row || !m_dim.col)
        {
            printf("[]");
            return;
        }

        head.assign(100, ' ');
        for (vector<vector<string>>::iterator it = text.begin(); it != text.end(); ++it)
            it->assign(m_dim.col, "");

        switch (m_type)
        {
        case dt_single:
            format = "  %f";
            break;
        case dt_int8:
        case dt_int16:
        case dt_int32:
            format = "  %d";
            break;
        case dt_int64:
            format = "  %ld";
            break;
        case dt_uint8:
        case dt_uint16:
        case dt_uint32:
            format = "  %u";
            break;
        case dt_uint64:
            format = "  %lu";
            break;
        default:
            format = "  %lf";
        }

        for (size_t i = 0; i < m_dim.row; ++i)
        {
            for (size_t j = 0; j < m_dim.col; ++j)
            {
                sprintf(buf, format.c_str(), (T) m_data[i * m_dim.col + j]);
                if (strlen(buf) > max[j])
                    max[j] = strlen(buf);
                text[i][j] = head + buf;
            }
        }

        printf("%ld X %ld\n", m_dim.row, m_dim.col);
        for (size_t i = 0; i < m_dim.row; ++i)
        {
            for (size_t j = 0; j < m_dim.col; ++j)
            {
                text[i][j].erase(0, text[i][j].size() - max[j]);
                printf("%s", text[i][j].c_str());
            }
            printf("\n");
        }
    }

    virtual void assign_stock(size_t m, size_t n)
    {
        m_data = (T*) realloc(m_data, sizeof(T) * m * n);
        m_dim.row = m;
        m_dim.col = n;
    }

    virtual void* access_unit(size_t m, size_t n)
    {
        if (m >= m_dim.row || n >= m_dim.col)
        {
            error("wrong parameter");
            return NULL;
        }
        return &m_data[m * m_dim.col + n];
    }

    virtual void* access_unit(size_t flat)
    {
        if (flat >= m_dim.row * m_dim.col)
        {
            error("wrong parameter");
            return NULL;
        }
        return &m_data[flat];
    }

    virtual double get_unit(size_t m, size_t n)
    {
        if (m >= m_dim.row || n >= m_dim.col)
        {
            error("wrong parameter");
            exit(0);
        }
        return (double)m_data[m * m_dim.col + n];
    }

    virtual double get_unit(size_t flat)
    {
        if (flat >= m_dim.row * m_dim.col)
        {
            error("wrong parameter");
            exit(0);
        }
        return (double)m_data[flat];
    }

    virtual void set_unit(size_t m, size_t n, void* data, Data_Type dt)
    {
        if (m >= m_dim.row || n >= m_dim.col || !data)
        {
            error("wrong parameter");
            return;
        }

        size_t flat = m * m_dim.col + n;
        switch (dt)
        {
        case Data_Type::dt_single:
            m_data[flat] = *(float*)data;
            break;
        case Data_Type::dt_double:
            m_data[flat] = *(double*)data;
            break;
        case Data_Type::dt_int8:
            m_data[flat] = *(char*)data;
            break;
        case Data_Type::dt_int16:
            m_data[flat] = *(short*)data;
            break;
        case Data_Type::dt_int32:
            m_data[flat] = *(int*)data;
            break;
        case Data_Type::dt_int64:
            m_data[flat] = *(long*)data;
            break;
        case Data_Type::dt_uint8:
            m_data[flat] = *(unsigned char*)data;
            break;
        case Data_Type::dt_uint16:
            m_data[flat] = *(unsigned short*)data;
            break;
        case Data_Type::dt_uint32:
            m_data[flat] = *(unsigned int*)data;
            break;
        case Data_Type::dt_uint64:
            m_data[flat] = *(unsigned long*)data;
            break;
        default:
            error("wrong Data_Type");
        }
    }

    virtual void set_unit(size_t m, size_t n, double v)
    {
        if (m >= m_dim.row || n >= m_dim.col)
        {
            error("wrong size");
            return;
        }

        m_data[m * m_dim.col + n] = (T)v;
    }

    virtual void set_unit(size_t m, double v)
    {
        if (m >= m_dim.row * m_dim.col)
        {
            error("wrong size");
            return;
        }

        m_data[m] = (T)v;
    }

    virtual void add_unit(size_t m, size_t n, void* data, Data_Type dt)
    {
        if (m >= m_dim.row || n >= m_dim.col || !data)
        {
            error("wrong parameter");
            return;
        }

        size_t flat = m * m_dim.col + n;
        switch (dt)
        {
        case Data_Type::dt_single:
            m_data[flat] += *(float*)data;
            break;
        case Data_Type::dt_double:
            m_data[flat] += *(double*)data;
            break;
        case Data_Type::dt_int8:
            m_data[flat] += *(char*)data;
            break;
        case Data_Type::dt_int16:
            m_data[flat] += *(short*)data;
            break;
        case Data_Type::dt_int32:
            m_data[flat] += *(int*)data;
            break;
        case Data_Type::dt_int64:
            m_data[flat] += *(long*)data;
            break;
        case Data_Type::dt_uint8:
            m_data[flat] += *(unsigned char*)data;
            break;
        case Data_Type::dt_uint16:
            m_data[flat] += *(unsigned short*)data;
            break;
        case Data_Type::dt_uint32:
            m_data[flat] += *(unsigned int*)data;
            break;
        case Data_Type::dt_uint64:
            m_data[flat] += *(unsigned long*)data;
            break;
        default:
            error("wrong Data_Type");
        }
    }

    virtual void minus_unit(size_t m, size_t n, void* data, Data_Type dt)
    {
        if (m >= m_dim.row || n >= m_dim.col || !data)
        {
            error("wrong parameter");
            return;
        }

        size_t flat = m * m_dim.col + n;
        switch (dt)
        {
        case Data_Type::dt_single:
            m_data[flat] -= *(float*)data;
            break;
        case Data_Type::dt_double:
            m_data[flat] -= *(double*)data;
            break;
        case Data_Type::dt_int8:
            m_data[flat] -= *(char*)data;
            break;
        case Data_Type::dt_int16:
            m_data[flat] -= *(short*)data;
            break;
        case Data_Type::dt_int32:
            m_data[flat] -= *(int*)data;
            break;
        case Data_Type::dt_int64:
            m_data[flat] -= *(long*)data;
            break;
        case Data_Type::dt_uint8:
            m_data[flat] -= *(unsigned char*)data;
            break;
        case Data_Type::dt_uint16:
            m_data[flat] -= *(unsigned short*)data;
            break;
        case Data_Type::dt_uint32:
            m_data[flat] -= *(unsigned int*)data;
            break;
        case Data_Type::dt_uint64:
            m_data[flat] -= *(unsigned long*)data;
            break;
        default:
            error("wrong Data_Type");
        }
    }

    virtual void swap_row(size_t m, size_t n)
    {
        if (m >= m_dim.row || n >= m_dim.row || m == n)
            return;

        for (size_t i = 0; i < m_dim.col; ++i)
        {
            T t = m_data[m * m_dim.col + i];
            m_data[m * m_dim.col + i] = m_data[n * m_dim.col + i];
            m_data[n * m_dim.col + i] = t;
        }
    }

    virtual void divide_row(size_t m, size_t n)
    {
        if (m >= m_dim.row || n >= m_dim.col)
            return;

        T t = m_data[m * m_dim.col + n];
        if (t == (T)0)
            return;

        for (size_t i = 0; i < m_dim.col; ++i)
            m_data[m * m_dim.col + i] /= t;
    }

    virtual void self_transform(size_t m, size_t n)
    {
        if (m >= m_dim.row || n >= m_dim.col)
            return;

        T t = m_data[m * m_dim.col + n];
        if (t == (T)0)
            return;

        for (size_t i = 0; i < m_dim.row; ++i)
        {
            if (i == m)
                continue;
            double fac = m_data[i * m_dim.col + n] / t;
            for (size_t j = 0; j < m_dim.col; ++j)
                m_data[i * m_dim.col + j] -= m_data[m * m_dim.col + j] * fac;
        }
    }

    virtual void init_all(int value)
    {
        for (size_t i = 0; i < m_dim.row * m_dim.col; ++i)
            m_data[i] = (T) value;
    }

    virtual void init_eye()
    {
        bzero(m_data, sizeof(T) * m_dim.row * m_dim.col);
        size_t min = m_dim.row >= m_dim.col ? m_dim.col : m_dim.row;
        for (size_t i = 0; i < min; ++i)
            m_data[i + i * m_dim.col] = (T) 1;
    }

    virtual void init_rand()
    {
        if (m_type == Data_Type::dt_single || m_type == Data_Type::dt_double)
            for (size_t i = 0; i < m_dim.row * m_dim.col; ++i)
                m_data[i] = random() / (RAND_MAX + 1.0);
        else
            for (size_t i = 0; i < m_dim.row * m_dim.col; ++i)
                m_data[i] = random();
    }

    virtual void init_diag(Vector* vec, int k = 0)
    {
        if (!vec || !vec->get_data() || !vec->get_size())
        {
            error("wrong Vector");
            return;
        }

        size_t m = vec->get_size();
        size_t n = m + abs(k);
        size_t flat_size = n * n;

        if (flat_size)
        {
            T* diag = (T*)vec->get_data();
            m_data = (T*) malloc(sizeof(T) * flat_size);
            bzero(m_data, sizeof(T) * flat_size);
            size_t s = (k >= 0) ? k : abs(k) * n;
            for (size_t i = 0; i < m; ++i)
                m_data[i * n + s++] = diag[i];
            m_dim.row = m_dim.col = n;
        }
    }

    virtual Vector* diag(int k)
    {
        int m = m_dim.row, n = m_dim.col;
        if (k >= n || k <= -m)
            return NULL;

        std::vector<T> dia;
        if (k >= 0)
        {
            // s - k < m means number of cycle less than m
            for (int s = k; s < n && s - k < m; ++s)
                dia.push_back(m_data[(s - k) * n + s]);
        }
        else
        {
            int s = abs(k) * n;
            // i - abs(k) < n means number of cycle less than n
            for (int i = abs(k); i < m && i - abs(k) < n; ++i)
                dia.push_back(m_data[s++ + (i - abs(k)) * n]);
        }

        Vector *diagv = vector_new(dia.size(), m_type);
        vector_assign(diagv, dia.begin(), dia.end());
        return diagv;
    }

    virtual void scalar_plus(double v)
    {
        for (size_t i = 0; i < m_dim.row; ++i)
            for (size_t j = 0; j < m_dim.col; ++j)
                m_data[i * m_dim.col + j] += (T)v;
    }

    virtual void scalar_minus(double v)
    {
        for (size_t i = 0; i < m_dim.row; ++i)
            for (size_t j = 0; j < m_dim.col; ++j)
                m_data[i * m_dim.col + j] -= (T)v;
    }

    virtual void scalar_multiply(double v)
    {
        for (size_t i = 0; i < m_dim.row; ++i)
            for (size_t j = 0; j < m_dim.col; ++j)
                m_data[i * m_dim.col + j] *= (T)v;
    }

    virtual void scalar_divide(double v)
    {
        for (size_t i = 0; i < m_dim.row; ++i)
            for (size_t j = 0; j < m_dim.col; ++j)
                m_data[i * m_dim.col + j] /= (T)v;
    }
};


/***********************************************************************
 * Matrix API
 ***********************************************************************/

/**
 * print a Matrix
 */
void matrix_print(Matrix*);
/**
 * create a Matrix without assignment
 */
Matrix* matrix_new(size_t m, size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * assign a Matrix. InputIterator point to double value.
 */
template<class InputIterator>
void matrix_assign(Matrix* m, InputIterator first, InputIterator last)
{
    if (!m)
    {
        error("NULL matrix");
        return;
    }

    Matrix_Dim dim = m->get_size();
    for (size_t i = 0; i < dim.row * dim.col && first != last; ++i, ++first)
    {
        m->set_unit(i, *first);
    }
}
/**
 * copy a Matrix along with data
 */
Matrix* matrix_copy(Matrix *m);
/**
 * zeros(size_t)
 */
Matrix* matrix_zeros(size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * zeros(size_t, size_t)
 */
Matrix* matrix_zeros(size_t m, size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * zeros(Vector)    must be Vector_T<unsigned int>
 */
Matrix* matrix_zeros(Vector *dim, Data_Type dt = Data_Type::dt_double);
/**
 * ones(size_t)
 */
Matrix* matrix_ones(size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * ones(size_t, size_t)
 */
Matrix* matrix_ones(size_t m, size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * ones(Vector)    must be Vector_T<unsigned int>
 */
Matrix* matrix_ones(Vector *dim, Data_Type dt = Data_Type::dt_double);
/**
 * eye(size_t)
 */
Matrix* matrix_eye(size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * eye(size_t, size_t)
 */
Matrix* matrix_eye(size_t m, size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * eye(Vector)    must be a Vector_T<unsigned int>
 */
Matrix* matrix_eye(Vector *vec, Data_Type dt = Data_Type::dt_double);
/**
 * rand()
 */
double matrix_rand();
/**
 * rand(size_t)
 */
Matrix* matrix_rand(size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * rand(size_t, size_t)
 */
Matrix* matrix_rand(size_t m, size_t n, Data_Type dt = Data_Type::dt_double);
/**
 * rand(Vector)    must be a Vector_T<unsigned int>
 */
Matrix* matrix_rand(Vector *vec, Data_Type dt = Data_Type::dt_double);
/**
 * diag(Vector, int)
 */
Matrix *matrix_diag(Vector *vec, int k = 0);
/**
 * diag(Matrix, int)
 */
Vector* matrix_diag(Matrix *m, int k = 0);

/**
 * blkdiag(Matrix...)
 * first/last are iterators point to some Matrix*
 */
template<class InputIterator>
Matrix* matrix_blkdiag(InputIterator first, InputIterator last, Data_Type dt = Data_Type::dt_double)
{
    size_t m = 0, n = 0;
    Matrix_Dim dim;

    for (InputIterator it = first; it != last; ++it)
    {
        dim = (*it)->get_size();
        m += dim.row;
        n += dim.col;
    }
    Matrix *mx = matrix_zeros(m, n, dt);

    m = 0;
    n = 0;
    for (InputIterator it = first; it != last; ++it)
    {
        dim = (*it)->get_size();
        for (size_t i = 0; i < dim.row; ++i)
        {
            for (size_t j = 0; j < dim.col; ++j)
            {
                mx->set_unit(m + i, n + j, (*it)->access_unit(i, j), (*it)->get_type());
            }
        }
        m += dim.row;
        n += dim.col;
    }

    return mx;
}

/**
 * cat(size_t, Matrix..., Data_Type)
 * dim must be 1 or 2 for 2-demensions Matrix
 * first/last are iterators point to some Matrix*
 */
template<class InputIterator>
Matrix* matrix_cat(size_t d, InputIterator first, InputIterator last, Data_Type dt = Data_Type::dt_double)
{
    Matrix_Dim dim, each;
    size_t sum = 0, accum = 0;

    if (first == last)
    {
        error("wrong Iterator");
        return NULL;
    }
    dim = (*first)->get_size();

    if (d == 1)
    {
        for (InputIterator it = first; it != last; ++it)
        {
            each = (*it)->get_size();
            if (each.col != dim.col)
            {
                error("wrong Matrix dimension");
                return NULL;
            }
            sum += each.row;
        }

        accum = 0;
        Matrix *m = matrix_zeros(sum, dim.col, dt);
        for (InputIterator it = first; it != last; ++it)
        {
            each = (*it)->get_size();
            void* data = (*it)->get_data();
            Data_Type dt = (*it)->get_type();
            for (size_t i = 0; i < each.row; ++i)
                for (size_t j = 0; j < each.col; ++j)
                    m->set_unit(i + accum, j, (*it)->access_unit(i, j), dt);
            accum += each.row;
        }
        return m;
    }
    else if (d == 2)
    {
        for (InputIterator it = first; it != last; ++it)
        {
            each = (*it)->get_size();
            if (each.row != dim.row)
            {
                error("wrong Matrix dimension");
                return NULL;
            }
            sum += each.col;
        }

        accum = 0;
        Matrix *m = matrix_zeros(dim.row, sum, dt);
        for (InputIterator it = first; it != last; ++it)
        {
            each = (*it)->get_size();
            void* data = (*it)->get_data();
            Data_Type dt = (*it)->get_type();
            for (size_t i = 0; i < each.row; ++i)
                for (size_t j = 0; j < each.col; ++j)
                    m->set_unit(i, j + accum, (*it)->access_unit(i, j), dt);
            accum += each.col;
        }
        return m;
    }
    else
    {
        error("wrong dimension");
        return NULL;
    }
}

/**
 * Matrix addition
 */
Matrix* matrix_plus(Matrix *one, Matrix *two, Data_Type dt);

/**
 * Matrix addition, the returned Matrix take type from one
 */
Matrix* matrix_plus(Matrix *one, Matrix *two);

/**
 * Matrix subtraction
 */
Matrix* matrix_minus(Matrix *one, Matrix *two, Data_Type dt);

/**
 * Matrix subtraction, the returned Matrix take type from one
 */
Matrix* matrix_minus(Matrix *one, Matrix *two);

/**
 * Matrix multiply
 */
Matrix* matrix_multiply(Matrix *one, Matrix *two, Data_Type dt);

/**
 * Matrix multiply, the returned Matrix take type from one
 */
Matrix* matrix_multiply(Matrix *one, Matrix *two);

/**
 * Matrix inversion
 */
Matrix* matrix_inverse(Matrix *m);

/**
 * Matrix right division
 */
Matrix* matrix_right_divide(Matrix *one, Matrix *two);

/**
 * Matrix left division
 */
Matrix* matrix_left_divide(Matrix *one, Matrix *two);

/**
 * Matrix scalar addition, return a new Matrix
 */
Matrix* matrix_scalar_plus(Matrix *m, double v);

/**
 * Matrix scalar subtraction, return a new Matrix
 */
Matrix* matrix_scalar_minus(Matrix *m, double v);

/**
 * Matrix scalar multiply, return a new Matrix
 */
Matrix* matrix_scalar_multiply(Matrix *m, double v);

/**
 * Matrix scalar division, return a new Matrix
 */
Matrix* matrix_scalar_divide(Matrix *m, double v);

/**
 * transpose a Matrix, return a new one
 */
Matrix* matrix_transpose(Matrix *m);

/**
 * matrix determinant
 */
double matrix_determinant(Matrix *m);

#endif
