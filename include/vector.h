#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "datatype.h"
#include <cstdlib>
#include <cstdio>

#define error(msg) fprintf(stderr, "[%s:%d] %s\n", __FILE__, __LINE__, msg)

class Vector
{
public:
    virtual ~Vector() {}

    virtual Data_Type get_type() = 0;
    virtual void* get_data() = 0;
    virtual size_t get_size() = 0;
    virtual size_t get_unit_size() = 0;
    virtual void print() = 0;
    virtual void* access_unit(size_t) = 0;
    virtual void set_unit(size_t, void*, Data_Type) = 0;
    virtual void set_unit(size_t, double) = 0;
    virtual void add_unit(size_t, void*, Data_Type) = 0;
    virtual void minus_unit(size_t, void*, Data_Type) = 0;
    virtual void scalar_multiply(double) = 0;
    virtual void scalar_divide(double) = 0;
};


template<class T> class Vector_T: public Vector
{
protected:
    T            *m_data;
    size_t        m_size;
    Data_Type    m_type;

public:
    using value_type = T;

    Vector_T(size_t m)
    {
        m_data = (T*) malloc(sizeof(T) * m);
        m_size = m;
    }

    virtual ~Vector_T()
    {
        if (m_data)
        {
            free(m_data);
            m_data = NULL;
        }
    }

    virtual Data_Type get_type() { return m_type; }
    virtual void* get_data() { return m_data; }
    virtual size_t get_size() { return m_size; }
    virtual size_t get_unit_size() { return sizeof(T); }

    virtual void print()
    {
        const char* format;

        if (!m_data || !m_size)
        {
            printf("<>");
            return;
        }

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

        for (size_t i = 0; i < m_size; ++i)
            printf(format, m_data[i]);
        printf("\n");
    }

    virtual void* access_unit(size_t index)
    {
        return index >= m_size ? NULL : &m_data[index];
    }

    virtual void set_unit(size_t n, void* data, Data_Type dt)
    {
        if (n >= m_size || !data)
        {
            error("wrong parameter");
            return;
        }

        switch (dt)
        {
        case Data_Type::dt_single:
            m_data[n] = *(float*)data;
            break;
        case Data_Type::dt_double:
            m_data[n] = *(double*)data;
            break;
        case Data_Type::dt_int8:
            m_data[n] = *(char*)data;
            break;
        case Data_Type::dt_int16:
            m_data[n] = *(short*)data;
            break;
        case Data_Type::dt_int32:
            m_data[n] = *(int*)data;
            break;
        case Data_Type::dt_int64:
            m_data[n] = *(long*)data;
            break;
        case Data_Type::dt_uint8:
            m_data[n] = *(unsigned char*)data;
            break;
        case Data_Type::dt_uint16:
            m_data[n] = *(unsigned short*)data;
            break;
        case Data_Type::dt_uint32:
            m_data[n] = *(unsigned int*)data;
            break;
        case Data_Type::dt_uint64:
            m_data[n] = *(unsigned long*)data;
            break;
        default:
            error("wrong Data_Type");
        }
    }

    virtual void set_unit(size_t m, double v)
    {
        if (m >= m_size)
        {
            error("wrong size");
            return;
        }

        m_data[m] = (T)v;
    }

    virtual void add_unit(size_t n, void* data, Data_Type dt)
    {
        if (n >= m_size || !data)
        {
            error("wrong parameter");
            return;
        }

        switch (dt)
        {
        case Data_Type::dt_single:
            m_data[n] += *(float*)data;
            break;
        case Data_Type::dt_double:
            m_data[n] += *(double*)data;
            break;
        case Data_Type::dt_int8:
            m_data[n] += *(char*)data;
            break;
        case Data_Type::dt_int16:
            m_data[n] += *(short*)data;
            break;
        case Data_Type::dt_int32:
            m_data[n] += *(int*)data;
            break;
        case Data_Type::dt_int64:
            m_data[n] += *(long*)data;
            break;
        case Data_Type::dt_uint8:
            m_data[n] += *(unsigned char*)data;
            break;
        case Data_Type::dt_uint16:
            m_data[n] += *(unsigned short*)data;
            break;
        case Data_Type::dt_uint32:
            m_data[n] += *(unsigned int*)data;
            break;
        case Data_Type::dt_uint64:
            m_data[n] += *(unsigned long*)data;
            break;
        default:
            error("wrong Data_Type");
        }
    }

    virtual void minus_unit(size_t n, void* data, Data_Type dt)
    {
        if (n >= m_size || !data)
        {
            error("wrong parameter");
            return;
        }

        switch (dt)
        {
        case Data_Type::dt_single:
            m_data[n] -= *(float*)data;
            break;
        case Data_Type::dt_double:
            m_data[n] -= *(double*)data;
            break;
        case Data_Type::dt_int8:
            m_data[n] -= *(char*)data;
            break;
        case Data_Type::dt_int16:
            m_data[n] -= *(short*)data;
            break;
        case Data_Type::dt_int32:
            m_data[n] -= *(int*)data;
            break;
        case Data_Type::dt_int64:
            m_data[n] -= *(long*)data;
            break;
        case Data_Type::dt_uint8:
            m_data[n] -= *(unsigned char*)data;
            break;
        case Data_Type::dt_uint16:
            m_data[n] -= *(unsigned short*)data;
            break;
        case Data_Type::dt_uint32:
            m_data[n] -= *(unsigned int*)data;
            break;
        case Data_Type::dt_uint64:
            m_data[n] -= *(unsigned long*)data;
            break;
        default:
            error("wrong Data_Type");
        }
    }

    virtual void scalar_multiply(double v)
    {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] *= (T)v;
    }

    virtual void scalar_divide(double v)
    {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] /= (T)v;
    }
};


/***********************************************************************
 * Vector API
 ***********************************************************************/

/**
 * print the Vector
 */
void vector_print(Vector*);

/**
 * create a Vector without assignment
 */
Vector* vector_new(size_t m, Data_Type dt = Data_Type::dt_double);

/**
 * assign a Vector. InputIterator point to double value.
 */
template<class InputIterator>
void vector_assign(Vector* v, InputIterator first, InputIterator last)
{
    if (!v)
    {
        error("NULL Vector");
        return;
    }

    size_t m = v->get_size();
    for (size_t i = 0; i < m && first != last; ++i, ++first)
    {
        v->set_unit(i, *first);
    }
}

/**
 * copy a Vector along with data
 */
Vector* vector_copy(Vector *v);

/**
 * Vector scalar multiply, return a new Vector
 */
Vector* vector_scalar_multiply(Vector *m, double v);

/**
 * Vector scalar division, return a new Vector
 */
Vector* vector_scalar_divide(Vector *m, double v);

/**
 * Vector addition
 */
Vector* vector_plus(Vector *one, Vector *two, Data_Type dt);

/**
 * Vector addition, the returned Vector take type from one
 */
Vector* vector_plus(Vector *one, Vector *two);

/**
 * Vector subtraction
 */
Vector* vector_minus(Vector *one, Vector *two, Data_Type dt);

/**
 * Vector subtraction, the returned Vector take type from one
 */
Vector* vector_minus(Vector *one, Vector *two);

#endif
