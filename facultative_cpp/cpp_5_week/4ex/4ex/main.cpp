#include <iostream>
#include <cmath>

float func(float x){
    return exp(-x * x);
}

int main(int argc, const char* argv[]) {
    float step = 0.01;
    float sum = 0;
    for (float i = 0; i < 10; i += step) {
        sum += step * (func(i) + func(i + step)) / 2;
    }
    std::cout << sum  << std::endl;
    return 0;
}
