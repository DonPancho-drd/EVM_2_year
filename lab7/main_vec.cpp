
#include <chrono>

#include "matrix.h"

int main(int argc, char **argv)
{
    srand(time(0));
    int N = 2048, M = 10;
    if (argc == 3)
    {
        N = atoi(argv[1]);
        M = atoi(argv[2]);
    }
    Matrix A(N);
    A.fillRandom();
    //     std::cout << "initial matrix" << std::endl;
    //  //   A.print();
    //     std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    Matrix nB = A.naiveInverse(M);
    auto end = std::chrono::high_resolution_clock::now();
    int time = (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

    std::cout << "Time of naive: " << time << " microseconds" << "(" << time / 1e6 << " seconds)" << std::endl;
    //   nB.print();
    //   std::cout << std::endl;

    auto start1 = std::chrono::high_resolution_clock::now();
    Matrix B = A.vecInverse(M);
    auto end1 = std::chrono::high_resolution_clock::now();
    time = (std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count());

    std::cout << "Time vectorised: " << time << " microseconds" << "(" << time / 1e6 << " seconds)" << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();
    Matrix bB = A.findInverseBlas(M);
    auto end2 = std::chrono::high_resolution_clock::now();
    time = (std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count());

    std::cout << "Time blass: " << time << " microseconds" << "(" << time / 1e6 << " seconds)" << std::endl;

    //   B.print();
    //  std::cout << std::endl;
    //  std::cout << "naive identity" << std::endl;
    //     Matrix C = A * B;
    //  //   C.print();
    //    //  std::cout << std::endl;
    //    //  std::cout << "vec identity" << std::endl;
    //     Matrix D = A.multiplyWithVectorisation(B);

    //  //   D.print();
    //     std::cout << std::endl;
    //     Matrix I = Matrix::createIdentity(N);
    //     std::cout << Matrix::findDifference(C, I) << std::endl;
    //     std::cout << Matrix::findDifference(D, I) << std::endl;
    // Matrix I = Matrix::createIdentity(N);
    // Matrix C = I * A;
    // C.print();

    return 0;
}
