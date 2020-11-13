/************************************************************************
 * The test file illustrate how to use c-matrix API
 ************************************************************************/

#include "vector.h"
#include "matrix.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

/*
 * test the API: vector_new, vector_assign,
 * vector_scalar_multiply, vector_scalar_divide,
 * vector_plus, vector_minus
 */
void test1()
{
    printf("======= test 1 ========\n");
    vector<double> vec1{2345.3463, 564566.75454, 65.633, 5254.6457};
    int vec2[] = {1, 2, 3, 4};

    unique_ptr<Vector> v1{vector_new(vec1.size(), Data_Type::dt_double)};
    vector_assign(v1.get(), vec1.begin(), vec1.end());
    vector_print(v1.get());

    unique_ptr<Vector> v2{vector_scalar_multiply(v1.get(), 0.1)};
    vector_print(v2.get());

    unique_ptr<Vector> v3{vector_scalar_divide(v1.get(), 10)};
    vector_print(v3.get());

    unique_ptr<Vector> v4{vector_new(sizeof(vec2)/sizeof(vec2[0]))};
    vector_assign(v4.get(), vec2, vec2 + sizeof(vec2)/sizeof(vec2[0]));
    vector_print(v4.get());

    unique_ptr<Vector> v5{vector_plus(v1.get(), v4.get())};
    vector_print(v5.get());

    unique_ptr<Vector> v6{vector_minus(v1.get(), v4.get())};
    vector_print(v6.get());
}

/*
 * test the API: matrix_diag, matrix_ones,
 * matrix_eye, matrix_rand
 */
void test2()
{
    printf("======= test 2 ========\n");
    unique_ptr<Matrix> m1{matrix_zeros(5,7,Data_Type::dt_single)};
    matrix_print(m1.get());

    unique_ptr<Matrix> m2{matrix_ones(5,7,Data_Type::dt_int64)};
    matrix_print(m2.get());

    unique_ptr<Matrix> m3{matrix_eye(7,Data_Type::dt_int32)};
    matrix_print(m3.get());

    unique_ptr<Matrix> m4{matrix_rand(9, 7)};
    matrix_print(m4.get());
}

/*
 * test the API: matrix_diag
 */
void test3()
{
    printf("======= test 3 ========\n");
    vector<double> vec{2345.3463, 564566.75454, 65.633, 5254.6457};
    Vector *v1 = vector_new(vec.size(), Data_Type::dt_double);
    vector_assign(v1, vec.begin(), vec.end());
    Matrix *m1 = matrix_diag(v1, -2);
    matrix_print(m1);

    Vector *v2 = matrix_diag(m1, -2);
    vector_print(v2);
}

/*
 * test the API: matrix_blkdiag
 */
void test4()
{
    printf("======= test 4 ========\n");
    Matrix *m1 = matrix_eye(5, Data_Type::dt_int32);
    Matrix *m2 = matrix_rand(4, Data_Type::dt_int64);
    Matrix *m3[2] = {m1, m2};
    Matrix *m4 = matrix_blkdiag(m3, m3 + 2, Data_Type::dt_int32);
    matrix_print(m4);
}

/*
 * test the API: matrix_cat
 */
void test5()
{
    printf("======= test 5 ========\n");
    Matrix *m1 = matrix_rand(4,5);
    Matrix *m2 = matrix_rand(3,5);
    Matrix *m3[2] = {m1, m2};
    Matrix *m4 = matrix_cat(1, m3, m3+2, Data_Type::dt_single);
    matrix_print(m1);
    matrix_print(m2);
    matrix_print(m4);

    printf("\n");

    Matrix *m5 = matrix_rand(4,5);
    Matrix *m6 = matrix_rand(4,4);
    Matrix *m7[2] = {m5, m6};
    Matrix *m8 = matrix_cat(2, m7, m7+2, Data_Type::dt_single);
    matrix_print(m5);
    matrix_print(m6);
    matrix_print(m8);
}

/*
 * test the API: matrix_multiply
 */
void test6()
{
    printf("======= test 6 ========\n");
    Matrix* m1 = matrix_rand(3, Data_Type::dt_int8);
    Matrix* m2 = matrix_rand(3, Data_Type::dt_int8);
    Matrix* m3 = matrix_multiply(m1, m2, Data_Type::dt_int32);
    matrix_print(m1);
    matrix_print(m2);
    matrix_print(m3);
}

/*
 * test the API: matrix_inverse
 */
void test7()
{    
    printf("======= test 7 ========\n");
    int data[] = {1, 2, 3, 2, 3, 4, 1, 2, 5};
    Matrix* m1 = matrix_new(3, 3, Data_Type::dt_int32);
    matrix_assign(m1, data, data + sizeof(data)/sizeof(data[0]));
    Matrix* m2 = matrix_inverse(m1);
    matrix_print(m1);
    matrix_print(m2);
}

/*
 * test the API: matrix_right_divide, matrix_left_divide
 */
void test8()
{    
    printf("======= test 8 ========\n");
    Matrix* m1 = matrix_rand(5, Data_Type::dt_int16);
    Matrix* m2 = matrix_rand(5, Data_Type::dt_int16);
    matrix_print(m1);
    matrix_print(m2);

    Matrix* m3 = matrix_right_divide(m1, m2);
    if (m3)
        matrix_print(m3);
    else
        printf("m3 is NULL");

    Matrix* m4 = matrix_left_divide(m1, m2);
    if (m4)
        matrix_print(m4);
    else
        printf("m4 is NULL");
}

/*
 * test the API: matrix_scalar_plus, matrix_scalar_minus,
 * matrix_scalar_multiply, matrix_scalar_divide
 */
void test9()
{    
    printf("======= test 9 ========\n");
    Matrix* m1 = matrix_rand(3, Data_Type::dt_single);
    Matrix* m2 = matrix_scalar_plus(m1, 10);
    Matrix* m3 = matrix_scalar_minus(m1, 3);
    Matrix* m4 = matrix_scalar_multiply(m1, 2);
    Matrix* m5 = matrix_scalar_divide(m1, 2);
    matrix_print(m1);
    matrix_print(m2);
    matrix_print(m3);
    matrix_print(m4);
    matrix_print(m5);
}

/*
 * test the API: matrix_transpose
 */
void test10()
{    
    printf("======= test 10 ========\n");
    Matrix* m1 = matrix_rand(3, 5, Data_Type::dt_single);
    Matrix* m2 = matrix_transpose(m1);
    matrix_print(m1);
    matrix_print(m2);
    delete m1;
    delete m2;
}

/*
 * test the API: matrix_determinant
 */
void test11()
{    
    printf("======= test 11 ========\n");
    int data[] = {1, -2, 4, -5, 2, 0, 1, 0, 3};
    Matrix* m1 = matrix_zeros(3, 3, Data_Type::dt_int32);
    matrix_assign(m1, data, data + sizeof(data)/sizeof(data[0]));

    double det = matrix_determinant(m1);
    matrix_print(m1);
    printf("det = %f\n", det);
    
    Matrix* m2 = matrix_rand(10, 10);
    det = matrix_determinant(m2);
    matrix_print(m2);
    printf("det = %f\n", det);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("usage: test <test name>\n");
        return 0;
    }

    if (!strcmp(argv[1], "1"))
        test1();
    else if (!strcmp(argv[1], "2"))
        test2();
    else if (!strcmp(argv[1], "3"))
        test3();
    else if (!strcmp(argv[1], "4"))
        test4();
    else if (!strcmp(argv[1], "5"))
        test5();
    else if (!strcmp(argv[1], "6"))
        test6();
    else if (!strcmp(argv[1], "7"))
        test7();
    else if (!strcmp(argv[1], "8"))
        test8();
    else if (!strcmp(argv[1], "9"))
        test9();
    else if (!strcmp(argv[1], "10"))
        test10();
    else if (!strcmp(argv[1], "11"))
        test11();

    return 0;
}

