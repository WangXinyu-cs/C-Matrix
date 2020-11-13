#include "vector_impl.h"
#include <cstdio>
#include <cstring>


using namespace std;

/***********************************************************************
 * Specific type of Vector definition
 ***********************************************************************/

Vector_double::Vector_double(size_t m) : Vector_T<double>(m)
{
    m_type = Data_Type::dt_double;
}

Vector_single::Vector_single(size_t m) : Vector_T<float>(m)
{
    m_type = Data_Type::dt_single;
}

Vector_int8::Vector_int8(size_t m) : Vector_T<char>(m)
{
    m_type = Data_Type::dt_int8;
}

Vector_int16::Vector_int16(size_t m) : Vector_T<short>(m)
{
    m_type = Data_Type::dt_int16;
}

Vector_int32::Vector_int32(size_t m) : Vector_T<int>(m)
{
    m_type = Data_Type::dt_int32;
}

Vector_int64::Vector_int64(size_t m) : Vector_T<long>(m)
{
    m_type = Data_Type::dt_int64;
}

Vector_uint8::Vector_uint8(size_t m) : Vector_T<unsigned char>(m)
{
    m_type = Data_Type::dt_uint8;
}

Vector_uint16::Vector_uint16(size_t m) : Vector_T<unsigned short>(m)
{
    m_type = Data_Type::dt_uint16;
}

Vector_uint32::Vector_uint32(size_t m) : Vector_T<unsigned int>(m)
{
    m_type = Data_Type::dt_uint32;
}

Vector_uint64::Vector_uint64(size_t m) : Vector_T<unsigned long>(m)
{
    m_type = Data_Type::dt_uint64;
}

/***********************************************************************
 * API functions
 ***********************************************************************/

Vector* vector_new(size_t m, Data_Type dt)
{
    switch (dt)
    {
    case dt_double:
        return new Vector_double(m);
    case dt_single:
        return new Vector_single(m);
    case dt_int8:
        return new Vector_int8(m);
    case dt_int16:
        return new Vector_int16(m);
    case dt_int32:
        return new Vector_int32(m);
    case dt_int64:
        return new Vector_int64(m);
    case dt_uint8:
        return new Vector_uint8(m);
    case dt_uint16:
        return new Vector_uint16(m);
    case dt_uint32:
        return new Vector_uint32(m);
    case dt_uint64:
        return new Vector_uint64(m);
    default:
        error("wrong Data_Type");
        exit(1);
    }
}

/**
 * API: print()
 */
void vector_print(Vector *v)
{
    v->print();
}

/**
 * copy a Vector along with data
 */
Vector* vector_copy(Vector *v)
{
    if (!v)
        return NULL;

    Vector *n = vector_new(v->get_size(), v->get_type());
    memcpy(n->get_data(), v->get_data(), v->get_unit_size() * v->get_size());
    return n;
}

/**
 * Vector scalar multiply, return a new Vector
 */
Vector* vector_scalar_multiply(Vector *v, double d)
{
    Vector *n = vector_copy(v);
    if (n)
        n->scalar_multiply(d);
    return n;
}

/**
 * Vector scalar division, return a new Vector
 */
Vector* vector_scalar_divide(Vector *v, double d)
{
    Vector *n = vector_copy(v);
    if (n)
        n->scalar_divide(d);
    return n;
}

/**
 * Vector addition
 */
Vector* vector_plus(Vector *one, Vector *two, Data_Type dt)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_new(one->get_size(), dt);
    for (size_t i = 0; i < n->get_size(); ++i)
    {
        n->set_unit(i, one->access_unit(i), one->get_type());
        n->add_unit(i, two->access_unit(i), two->get_type());
    }

    return n;
}

/**
 * Vector addition, the returned Vector take type from one
 */
Vector* vector_plus(Vector *one, Vector *two)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_copy(one);
    for (size_t i = 0; i < n->get_size(); ++i)
        n->add_unit(i, two->access_unit(i), two->get_type());

    return n;
}

/**
 * Vector subtraction
 */
Vector* vector_minus(Vector *one, Vector *two, Data_Type dt)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_new(one->get_size(), dt);
    for (size_t i = 0; i < n->get_size(); ++i)
    {
        n->set_unit(i, one->access_unit(i), one->get_type());
        n->minus_unit(i, two->access_unit(i), two->get_type());
    }

    return n;
}

/**
 * Vector subtraction, the returned Vector take type from one
 */
Vector* vector_minus(Vector *one, Vector *two)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_copy(one);
    for (size_t i = 0; i < n->get_size(); ++i)
        n->minus_unit(i, two->access_unit(i), two->get_type());

    return n;
}


