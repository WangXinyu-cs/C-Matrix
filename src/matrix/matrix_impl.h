#ifndef _MATRIX_IMPL_H_
#define _MATRIX_IMPL_H_

#include "datatype.h"
#include "matrix.h"


class Matrix_double: public Matrix_T<double>
{
public:
//    Matrix_double();
    Matrix_double(size_t m, size_t n);
};

class Matrix_single: public Matrix_T<float>
{
public:
//    Matrix_single();
    Matrix_single(size_t m, size_t n);
};

class Matrix_int8: public Matrix_T<char>
{
public:
    //Matrix_int8();
    Matrix_int8(size_t m, size_t n);
};

class Matrix_int16: public Matrix_T<short>
{
public:
    //Matrix_int16();
    Matrix_int16(size_t m, size_t n);
};

class Matrix_int32: public Matrix_T<int>
{
public:
//    Matrix_int32();
    Matrix_int32(size_t m, size_t n);
};

class Matrix_int64: public Matrix_T<long>
{
public:
    //Matrix_int64();
    Matrix_int64(size_t m, size_t n);
};

class Matrix_uint8: public Matrix_T<unsigned char>
{
public:
//    Matrix_uint8();
    Matrix_uint8(size_t m, size_t n);
};

class Matrix_uint16: public Matrix_T<unsigned short>
{
public:
    //Matrix_uint16();
    Matrix_uint16(size_t m, size_t n);
};

class Matrix_uint32: public Matrix_T<unsigned int>
{
public:
//    Matrix_uint32();
    Matrix_uint32(size_t m, size_t n);
};

class Matrix_uint64: public Matrix_T<unsigned long>
{
public:
//    Matrix_uint64();
    Matrix_uint64(size_t m, size_t n);
};


#endif
