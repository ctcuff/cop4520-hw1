#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <ratio>
#include <algorithm>
#include <vector>
#include <numeric>
#include <iomanip>
#include <locale>
#include <sstream>

#define PRIMES_COUNT 100'000'000

// Takes a number and formats it as a comma separated string
std::string formatNumber(long value)
{
    std::stringstream stream;
    stream.imbue(std::locale("en_US.UTF-8"));
    stream << std::fixed << value;
    return stream.str();
}

// Uses the Sieve of Eratosthenes to find all prime numbers up to n.
// Returns a vector containing those primes.
// See: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes#Pseudocode
std::vector<int> *sieveOfEratosthenes(int n)
{
    bool *primeMap = new bool[n + 1];

    std::fill_n(primeMap, n + 1, true);

    for (int i = 2; i * i <= n; i++)
    {
        if (primeMap[i])
        {
            for (int j = i * i; j <= n; j += i)
            {
                primeMap[j] = false;
            }
        }
    }

    std::vector<int> *primes = new std::vector<int>;

    for (int i = 2; i <= n; i++)
    {
        if (primeMap[i])
        {
            primes->push_back(i);
        }
    }

    delete[] primeMap;

    return primes;
}

int main(void)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> *primes = sieveOfEratosthenes(PRIMES_COUNT);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration<double, std::milli>(end - start);
    long sum = std::accumulate(primes->begin(), primes->end(), 0L);

    try
    {
        std::string filePath = "./out/primes.txt";
        std::ofstream file;
        file.open(filePath);

        file << "Execution time: " << duration.count() << "ms" << std::endl;
        file << "Primes found: " << formatNumber(primes->size()) << std::endl;
        file << "Sum: " << formatNumber(sum) << std::endl;
        file << "Top ten primes:" << std::endl;

        std::for_each(primes->end() - 10, primes->end(), [&](int n)
        {
            file << formatNumber(n) << std::endl;
        });

        file.close();

        std::cout << "Results written to " << filePath << std::endl;
    }
    catch (const std::exception& err)
    {
        std::cout << "Error writing results to file: " << err.what() << std::endl;
    }

    delete primes;

    return 0;
}
