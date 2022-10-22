#include <iostream>
#include <math.h>

int main(int argc, const char* argv[]) {
    double x_0 = 30;
    double sum = 0;
    double elem = x_0;
    for (int i = 1; i < 1000000000; i++) {
        sum += elem;
        elem = - (elem * x_0 * x_0) / (2 * i * (2 * i + 1));
    }
    
    std::cout << sum << "\t" << sin(x_0) << std::endl;
    return 0;
}
