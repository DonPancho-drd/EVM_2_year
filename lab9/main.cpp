#include <iostream>
#include <fstream>
#include <chrono>
#include <limits>


#define L1 32 * 1024         // 32 KB
#define L2 0.5 * 1024 * 1024 // 0.5 MB
#define L3 4 * 1024 * 1024   // 4 MB

#define RUNS 10
#define K 5

#define MIN(A, B) ((A) > (B) ? (B) : (A))


void init_array(unsigned int *array, unsigned int fragments_cnt, unsigned int size, unsigned int offset)
{
    for(int i = 0; i < offset / fragments_cnt; ++i) {
        for(int j = 0; j < fragments_cnt - 1; ++j) {
            array[i + j * offset] = i + (j + 1) * offset;
        }

        array[i + (fragments_cnt - 1) * offset] = i + 1;
    }

    array[size / fragments_cnt - 1 + (fragments_cnt - 1) * offset] = 0;
}

double test(unsigned int *array, unsigned int array_size)
{
    unsigned int k = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < array_size * K; i++)
    {
        k = array[k];
    }

    auto end = std::chrono::high_resolution_clock::now();

    if (k == 1)
    {
        std::cout << ".";
    }

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    double x = static_cast<double>(duration.count()) / (array_size * K);
    return x;
}

int main()
{
    const std::string paths[] = {"l1.txt", "l2.txt", "l3.txt"};
    const unsigned int offsets[] = {static_cast<unsigned int>(L3 / sizeof(unsigned int))};


    for ( int i = 0; i < 1; i++)
    {
        std::ofstream file(paths[i]);

        for (int fragments_cnt = 1; fragments_cnt <= 32; fragments_cnt++)
        {
            unsigned int offset = offsets[i];
            unsigned int array_size = fragments_cnt * offset;
            unsigned int *array = new unsigned int[array_size]();
            init_array(array, fragments_cnt, array_size, offset);

            double min_time = std::numeric_limits<double>::max();

            for (unsigned int j = 0; j < RUNS; j++)
            {
                double current_time = test(array, array_size);
                min_time = MIN(min_time, current_time);
            }

            file << fragments_cnt << " " << min_time << "\n";
            delete[] array;
        }



        file.close();
    }

    return 0;
}
