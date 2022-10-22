#include <iostream>
#include <cmath>

int main(int argc, const char* argv[]) {
    double n = 0;
    double sum = 0;
    while (n < pow(10, 9)){
        sum += 1 / (pow(2, n));
        n++;
    }
    
    std::cout << std::scientific << sum;
    return 0;
}
