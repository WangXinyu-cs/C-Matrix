#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

#include "datatype.h"
#include "vector.h"


class Vector_double : public Vector_T<double>
{
public:
    Vector_double(size_t);
};

class Vector_single : public Vector_T<float>
{
public:
    Vector_single(size_t);
};

class Vector_int8 : public Vector_T<char>
{
public:
    Vector_int8(size_t);
}
;

class Vector_int16 : public Vector_T<short>
{
public:
    Vector_int16(size_t);
};

class Vector_int32 : public Vector_T<int>
{
public:
    Vector_int32(size_t);
};

class Vector_int64 : public Vector_T<long>
{
public:
    Vector_int64(size_t);
};

class Vector_uint8 : public Vector_T<unsigned char>
{
public:
    Vector_uint8(size_t);
};

class Vector_uint16 : public Vector_T<unsigned short>
{
public:
    Vector_uint16(size_t);
};

class Vector_uint32 : public Vector_T<unsigned int>
{
public:
    Vector_uint32(size_t);
};

class Vector_uint64 : public Vector_T<unsigned long>
{
public:
    Vector_uint64(size_t);
};

#endif
