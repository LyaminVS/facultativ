#include <iostream>
#include <math.h>
#include <fstream>

int main(int argc, const char* argv[]) {
    std::ofstream fout;
    fout.open("output.bin", std::ios::binary);
    double** sums = new double*[1000];
    for (int i = 0; i < 1000; i++) {
        sums[i] = new double[2];
    }
    double x_0;
    double sum = 0;
    for (int i = 0; i < 1000; i++) {
        x_0 = (double)i / 10;
        sum = 0;
        double elem = x_0;
        int j = 1;
        while (elem < 0.1) {
            sum += elem;
            elem = - (elem * x_0 * x_0) / (2 * j * (2 * j + 1));
        }
        sums[i][0] = x_0;
        sums[i][1] = sum;
    }
    for (int i = 0; i < 1000; i++) {
        fout.write(reinterpret_cast<const char*>(sums[i]), sizeof(double) * 2);
    }
    
    fout.close();
    return 0;
}
