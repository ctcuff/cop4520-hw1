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
#include <cmath>
#include <array>

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
// See: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes#Pseudocode
void sieveOfEratosthenes(std::array<bool, PRIMES_COUNT + 1>& primeMap, int end)
{
    long iterations = 0;
    for (int i = 2; i <= (int)std::sqrt(end); i++)
    {
        iterations++;
        if (primeMap[i])
        {
            for (long j = 2; j <= end; j += i)
            {
                iterations++;
                primeMap[j] = false;
            }
        }
    }
}

int main(void)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::array<bool, PRIMES_COUNT + 1> *primeMap = new std::array<bool, PRIMES_COUNT + 1>;
    std::fill(primeMap->begin(), primeMap->end(), true);

    sieveOfEratosthenes(*primeMap, PRIMES_COUNT);

    auto end = std::chrono::high_resolution_clock::now();

    long sum = 0;
    long primesFound = 0;
    std::vector<int> lastPrimes;

    for (int i = 2; i < primeMap->size(); i++)
    {
        if (primeMap->at(i))
        {
            sum += i;
            primesFound++;
        }
    }

    for (int i = primeMap->size() - 1; i >= 0; i--)
    {
        if (primeMap->at(i))
        {
            lastPrimes.push_back(i);
        }

        if (lastPrimes.size() == 10) {
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

        file << "Execution time: " << duration.count() << "ms" << std::endl;
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
    catch (const std::exception& err)
    {
        std::cout << "Error writing results to file: " << err.what() << std::endl;
    }

    delete primeMap;

    return 0;
}
