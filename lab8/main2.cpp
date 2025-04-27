#include <iostream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <random>

#define MULTIPLIER 1.1
#define MIN_N (256)         // 1 KByte
#define MAX_N (1024*1024 ) // 4 MBytes
#define CNT 5               // сколько раз проходимся по массиву
#define TEST_CNT 20       // сколько раз запускаем тест для n



void shuffle(int *buf, int n)
{
    for (int i = 0; i < n; i++)
    {
        int j = i + rand() % (n - i);
        std::swap(buf[i], buf[j]);
    }
}

int *createStraight(int n)
{
    int *buffer = new int[n];
    for (int i = 0; i < n; i++)
    {
        buffer[i] = i + 1;
    }
    buffer[n - 1] = 0;
    return buffer;
}

int *createReverse(int n)
{
    int *buffer = new int[n];
    for (int i = 1; i < n; i++)
    {
        buffer[i] = i - 1;
    }
    buffer[0] = n - 1;
    return buffer;
}

int *createRandom(int n)
{
    int *buffer = new int[n];
    int *ind = createStraight(n);
    shuffle(ind, n);

    auto zero = std::find(ind, ind + n, 0);
    std::swap(*zero, ind[n - 1]);

    int pos = 0;
    for(int i = 0; i < n; i++){
        buffer[pos] = ind[i];
        pos = ind[i];
    }
    delete[] ind;
    return buffer;
}


void traverseBuf(int *buf, FILE *file, double *mintime, int n)
{

        int k = 0;

        for (int i = 0; i < n ; i++)
        {
            k = buf[k];
        }
        if(k == 0){
            std::cout << ".";
        }

        for(int i = 0; i < TEST_CNT; i++){
            k = 0;
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < n * CNT; i++)
            {
                k = buf[k];

            }
            if (k == 0){
                std::cout << ".";
            }

            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            double dur = static_cast<double>(duration.count()) / (n * CNT);

            if (mintime[n - MIN_N] == 0.0 || dur < mintime[n - MIN_N])
            {
                mintime[n - MIN_N] = dur;
            }

        }
    }

    void printToFile(FILE * file, double *mintime)
    {
        for (int i = 0; i < MAX_N - MIN_N + 1; i++)
        {
            if (mintime[i] != 0.0)
            {
                fprintf(file, "%ld %.6f\n", (i + MIN_N) * sizeof(int), mintime[i]);
            }
        }
    }

    int main()
    {
        srand(time(0));

        FILE *file1 = fopen("out_s.txt", "w");
        FILE *file2 = fopen("out_rev.txt", "w");
        FILE *file3 = fopen("out_ran.txt", "w");

        double *min1 = new double[MAX_N - MIN_N + 1]();
        double *min2 = new double[MAX_N - MIN_N + 1]();
        double *min3 = new double[MAX_N - MIN_N + 1]();

        for (int n = MIN_N; n <= MAX_N; n *= MULTIPLIER)
        {
            int *straight_buf = createStraight(n);
            int *reverse_buf = createReverse(n);
            int *random_buf = createRandom(n);


            traverseBuf(straight_buf, file1, min1, n);
            traverseBuf(reverse_buf, file2, min2, n);
            traverseBuf(random_buf, file3, min3, n);

            delete[] straight_buf;
            delete[] reverse_buf;
            delete[] random_buf;
        }
        std::cout << "done" << std::endl;


        printToFile(file1, min1);
        printToFile(file2, min2);
        printToFile(file3, min3);

        delete[] min1;
        delete[] min2;
        delete[] min3;

        fclose(file1);
        fclose(file2);
        fclose(file3);

        return 0;
    }
