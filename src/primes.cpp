#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>

#define PRIMES_COUNT 100'000'000

void generateHugeArray()
{
    bool *primeMap = new bool[PRIMES_COUNT];

    for (int i = 0; i < PRIMES_COUNT; i++)
    {
        primeMap[i] = i % 2 == 0;
    }

    delete[] primeMap;
}

int main(void)
{

    auto start = std::chrono::high_resolution_clock::now();
    generateHugeArray();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Duration: " << duration.count() << "ms" << std::endl;
}
