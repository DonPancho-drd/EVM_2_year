#include <new>
#include <random>
#include <gtest/gtest.h>
#include "matrix.h"
#include <Eigen/Dense>

void create_random_diagonally_dominant_matrix(Matrix& matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.1f, 10.0f);

    for (int i = 0; i < matrix.n * matrix.n; ++i) {
        matrix.matrix[i] = dis(gen);
    }
    int n = matrix.n;

    for (int i = 0; i < n; ++i) {
        float row_sum = 0.0f;
        for (int j = 0; j < n; ++j) {
            row_sum += matrix.matrix[i * n + j];
        }
        matrix.matrix[i * n + i] += row_sum;
    }
}

class MatrixInversionParametrizedTest : public ::testing::TestWithParam<std::size_t> {
protected:
    std::size_t N;
};

TEST_P(MatrixInversionParametrizedTest, test1) {
    std::size_t N = GetParam();
    Matrix matrix(N);
    std::cout << "N = " << N << std::endl;

    // Проверка метода naiveInverse
    create_random_diagonally_dominant_matrix(matrix);
    Matrix inverse_matrix_naive = matrix.naiveInverse(30);
    Eigen::MatrixXf eigen_matrix_naive = Eigen::Map<Eigen::MatrixXf>(matrix.matrix.data(), N, N);
    Eigen::MatrixXf eigen_inverse_naive = Eigen::Map<Eigen::MatrixXf>(inverse_matrix_naive.matrix.data(), N, N);
    Eigen::MatrixXf identity_check_naive = eigen_matrix_naive * eigen_inverse_naive;
    EXPECT_TRUE(identity_check_naive.isIdentity(0.1));

}

TEST_P(MatrixInversionParametrizedTest, test2) {
    std::size_t N = GetParam();
    Matrix matrix(N);
    std::cout << "N = " << N << std::endl;

    // Проверка метода vecInverse
    create_random_diagonally_dominant_matrix(matrix);
    Matrix inverse_matrix_vec = matrix.vecInverse(30);
    Eigen::MatrixXf eigen_matrix_vec = Eigen::Map<Eigen::MatrixXf>(matrix.matrix.data(), N, N);
    Eigen::MatrixXf eigen_inverse_vec = Eigen::Map<Eigen::MatrixXf>(inverse_matrix_vec.matrix.data(), N, N);
    Eigen::MatrixXf identity_check_vec = eigen_matrix_vec * eigen_inverse_vec;
    EXPECT_TRUE(identity_check_vec.isIdentity(0.1));

}

TEST_P(MatrixInversionParametrizedTest, test3) {
    std::size_t N = GetParam();
    Matrix matrix(N);
    std::cout << "N = " << N << std::endl;

    // Проверка метода findInverseBlas
    create_random_diagonally_dominant_matrix(matrix);
    Matrix inverse_matrix_blas = matrix.findInverseBlas(30);
    Eigen::MatrixXf eigen_matrix_blas = Eigen::Map<Eigen::MatrixXf>(matrix.matrix.data(), N, N);
    Eigen::MatrixXf eigen_inverse_blas = Eigen::Map<Eigen::MatrixXf>(inverse_matrix_blas.matrix.data(), N, N);
    Eigen::MatrixXf identity_check_blas = eigen_matrix_blas * eigen_inverse_blas;
    EXPECT_TRUE(identity_check_blas.isIdentity(0.1));
}

INSTANTIATE_TEST_SUITE_P(
    MatrixInversionTests,
    MatrixInversionParametrizedTest,
    ::testing::Values(64, 256, 1024)
);
