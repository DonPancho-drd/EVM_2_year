#include <iostream>
#include <vector>
#include <immintrin.h>
#include <cmath>
#include <cblas.h>

class Matrix
{
public:
    std::vector<float> matrix;
    int n;

    Matrix(int n)
    {
        this->n = n;
        matrix = std::vector<float>(n * n, (float)0);
    }

    void print()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cout << matrix[i * n + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void fillRandom()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                matrix[i * n + j] = rand() / (float)RAND_MAX * 10;
            }
        }
    }
    static Matrix createIdentity(int n)
    {
        Matrix A(n);
        for (int i = 0; i < n; i++)
        {
            A.matrix[i * n + i] = 1;
        }
        return A;
    }

    Matrix multNumber(float number)
    {
        Matrix A(n);
        for (int i = 0; i < n * n; i++)
        {
            A.matrix[i] = matrix[i] * number;
        }
        return A;
    }

    Matrix operator+(Matrix &B)
    {
        Matrix A(n);
        for (int i = 0; i < n * n; i++)
        {
            A.matrix[i] = matrix[i] + B.matrix[i];
        }
        return A;
    }

    Matrix operator-(Matrix &B)
    {
        Matrix A(n);
        for (int i = 0; i < n * n; i++)
        {
            A.matrix[i] = matrix[i] - B.matrix[i];
        }
        return A;
    }

    static float findDifference(Matrix &m1, Matrix &m2)
    {
        float diff = 0.0f;

        for (int i = 0; i < m1.n * m1.n; ++i)
        {
            diff += std::fabs(m1.matrix[i] - m2.matrix[i]);
        }

        return diff;
    }

    Matrix transpose()
    {
        Matrix A(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                A.matrix[j * n + i] = matrix[i * n + j];
            }
        }
        return A;
    }

    float maxByRows()
    {
        float maxSum = 0;
        for (int i = 0; i < n; i++)
        {
            float sum = 0;
            for (int j = 0; j < n; j++)
            {
                sum += std::fabs(matrix[i * n + j]);
            }
            if (sum > maxSum)
            {
                maxSum = sum;
            }
        }
        return maxSum;
    }

    Matrix operator*(Matrix &B)
    {
        Matrix A(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    A.matrix[i * n + k] += matrix[i * n + j] * B.matrix[j * n + k];
                }
            }
        }
        return A;
    }

#pragma GCC optimize("no-tree-vectorize")
    Matrix naiveInverse(int m)
    {
        Matrix At = this->transpose();

        // Нормируем матрицы для вычисления B
        float normA = this->maxByRows();
        float normAt = At.maxByRows();

        Matrix B = At.multNumber(1.0f / (normA * normAt));

        Matrix I = createIdentity(n);
        Matrix BA = B * (*this);
        Matrix R1 = I - BA;

        Matrix R = R1;
        Matrix Res = I + R1;

        for (int i = 2; i < m; i++)
        {
            R = R * R1;
            Res = Res + R;
        }

        return Res * B;
    }
#pragma GCC reset_options

    // -------------------------Vectorisation--------------------------------------

    Matrix multiplyWithVectorisation(Matrix &B)
    {
        Matrix C(n);

        for (int i = 0; i < n; ++i)
        {
            float *c = C.matrix.data() + i * n;
            for (int j = 0; j < n; ++j)
            {
                float a = matrix[i * n + j];
                float *b = B.matrix.data() + j * n;

                for (int k = 0; k < n; k += 8)
                {
                    __m256 a_vec = _mm256_set1_ps(a);
                    __m256 b_vec = _mm256_loadu_ps(b + k);
                    __m256 c_vec = _mm256_loadu_ps(c + k);
                    // c[k:k+8] += a * b[k:k+8]
                    c_vec = _mm256_add_ps(c_vec, _mm256_mul_ps(a_vec, b_vec));
                    _mm256_storeu_ps(c + k, c_vec);
                }
            }
        }

        return C;
    }

    Matrix vecInverse(int m)
    {
        Matrix At = this->transpose();

        // Нормируем матрицы для вычисления B
        float normA = this->maxByRows();
        float normAt = At.maxByRows();

        Matrix B = At.scalarMultiply(1.0f / (normA * normAt));

        Matrix I = createIdentity(n);
        Matrix BA = B.multiplyWithVectorisation(*this);
        Matrix R1 = I.subtract(BA);

        Matrix R = R1;
        Matrix Res = I.add(R1);

        for (int i = 2; i < m; i++)
        {
            R = R.multiplyWithVectorisation(R1);
            Res = Res.add(R);
        }

        return Res.multiplyWithVectorisation(B);
    }

    Matrix subtract(const Matrix &B)
    {
        Matrix result(n);

        for (int i = 0; i < n * n; i += 8)
        {
            __m256 a_vec = _mm256_loadu_ps(matrix.data() + i);

            __m256 b_vec = _mm256_loadu_ps(B.matrix.data() + i);
            __m256 result_vec = _mm256_sub_ps(a_vec, b_vec);

            _mm256_storeu_ps(result.matrix.data() + i, result_vec);
        }

        return result;
    }

    Matrix scalarMultiply(float scalar)
    {
        Matrix result(n);
        __m256 scalar_vec = _mm256_set1_ps(scalar);

        for (int i = 0; i < n * n; i += 8)
        {
            __m256 matrix_vec = _mm256_loadu_ps(matrix.data() + i);
            __m256 result_vec = _mm256_mul_ps(scalar_vec, matrix_vec);
            _mm256_storeu_ps(result.matrix.data() + i, result_vec);
        }

        return result;
    }

    Matrix add(const Matrix &B)
    {
        Matrix result(n);

        for (int i = 0; i < n * n; i += 8)
        {
            __m256 a_vec = _mm256_loadu_ps(matrix.data() + i);
            __m256 b_vec = _mm256_loadu_ps(B.matrix.data() + i);

            __m256 result_vec = _mm256_add_ps(a_vec, b_vec);
            _mm256_storeu_ps(result.matrix.data() + i, result_vec);
        }

        return result;
    }

    // ---------------------------------BLAS-----------------------------

    // Order – определяет способ хранения матриц в памяти:
    // CblasRowMajor – матрицы хранятся по строкам (стандартно в C),
    // TransA, TransB – определяют операции op() над матрицами A и B:
    // CblasNoTrans – без транспонирования,

    // CblasNoTrans – без сопряженной;
    // M, N, K – размеры матриц;
    // alpha, beta – коэффициенты;
    // lda, ldb, ldc – число элементов в ведущей размерности матрицы (строке или
    // столбце). Для массивов в языке Си – это число элементов в строке
    // соответствующей матрицы: lda = K, ldb = N, ldc = N.

    static Matrix multiplyMatricesBlas(Matrix &A, Matrix &B)
    {
        Matrix result(A.n);
        // С = 1∙op(A)∙op(B) + 0∙C
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    A.n, B.n, A.n, 1.0f,
                    A.matrix.data(), A.n,
                    B.matrix.data(), B.n,
                    0.0f, result.matrix.data(), result.n);

        return result;
    }

    Matrix findInverseBlas(int m)
    {
        Matrix At = this->transpose();

        // Нормируем матрицы для вычисления B
        float normA = this->maxByRows();
        float normAt = At.maxByRows();

        Matrix B = At;
        cblas_sscal(B.n * B.n, 1.0f / (normA * normAt), B.matrix.data(), 1);

        Matrix I = createIdentity(n);
        Matrix BA = multiplyMatricesBlas(B, *this);
        Matrix R1 = I - BA;

        Matrix R = R1;
        Matrix Res = I + R1;

        for (int i = 2; i < m; i++)
        {
            R = multiplyMatricesBlas(R, R1);
            Res = Res + R;
        }

        return multiplyMatricesBlas(Res, B);
    }
};
