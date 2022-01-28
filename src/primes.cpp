#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <vector>
#include <locale>
#include <sstream>
#include <cmath>
#include <array>

#define PRIMES_COUNT 100'000'000
#define THREAD_COUNT 8

// Takes a number and returns it as a comma separated string
// with 2 decimal places (if it has any)
template <class T>
std::string formatNumber(T value)
{
    std::stringstream stream;
    stream.imbue(std::locale("en_US.UTF-8"));
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}

// See: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes#Pseudocode
// Uses the Sieve of Eratosthenes to find all prime numbers up to PRIMES_COUNT.
// Because this function will be running on multiple threads, we can
// start at some start value, and increment by THREAD_COUNT to speed
// up the process.
void sieveOfEratosthenes(std::array<bool, PRIMES_COUNT + 1> &primeMap, int start)
{
    for (int i = start; i <= (int)std::sqrt(PRIMES_COUNT); i += THREAD_COUNT)
    {
        if (primeMap[i])
        {
            for (int j = i * i; j <= PRIMES_COUNT; j += i)
            {
                primeMap[j] = false;
            }
        }
    }
}

int main(void)
{
    std::array<bool, PRIMES_COUNT + 1> *primeMap = new std::array<bool, PRIMES_COUNT + 1>;
    std::fill(primeMap->begin(), primeMap->end(), true);
    std::thread threads[THREAD_COUNT];

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads[i] = std::thread(sieveOfEratosthenes, std::ref(*primeMap), i + 2);
    }

    for (std::thread &thread : threads)
    {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    long sum = 0;
    long primesFound = 0;
    std::vector<int> lastPrimes;

    for (size_t i = 2; i < primeMap->size(); i++)
    {
        if (primeMap->at(i))
        {
            sum += i;
            primesFound++;
        }
    }

    for (size_t i = primeMap->size() - 1; i >= 0; i--)
    {
        if (primeMap->at(i))
        {
            lastPrimes.push_back(i);
        }

        if (lastPrimes.size() == 10)
        {
            break;
        }
    }

    std::reverse(lastPrimes.begin(), lastPrimes.end());

    auto duration = std::chrono::duration<double, std::milli>(end - start);

    try
    {
        std::string filePath = "./out/primes.txt";
        std::ofstream file;
        file.open(filePath);

        file << "Execution time: " << formatNumber(duration.count()) << "ms" << std::endl;

        file << "Primes found: " << formatNumber(primesFound) << std::endl;
        file << "Sum: " << formatNumber(sum) << std::endl;
        file << "Top ten primes:" << std::endl;

        for (int n : lastPrimes)
        {
            file << formatNumber(n) << std::endl;
        }

        file.close();

        std::cout << "Results written to " << filePath << std::endl;
    }
    catch (const std::exception &err)
    {
        std::cout << "Error writing results to file: " << err.what() << std::endl;
    }

    delete primeMap;

    return 0;
}
