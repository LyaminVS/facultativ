#include <iostream>
#include <cmath>

int main(int argc, const char* argv[]) {
    float n = 1;
    float sum = 0;
    float sum_reverse = 0;
    while (n < pow(10, 7)){
        sum += 1 / n;
        sum_reverse += 1 / (pow(10, 7) - n);
        n++;
    }
    std::cout.precision(10);
    std::cout << std::scientific << sum << std::endl;
    std::cout << std::scientific << sum_reverse << std::endl;
    return 0;
}

