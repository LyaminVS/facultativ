#include <iostream>
#include <fstream>
int main(int argc, const char * argv[]) {
    std::ofstream fout;
    fout.open("")
    double x_0 = 0;
    double v_0 = 0;
    
    double omega = 1;
    
    double delta_t = 0.01;

    double v_i = v_0;
    double x_i = x_0;
    
    double t = 0;
    
    while (t != 100) {
        x_i = x_i + delta_t * v_i;
        v_i = v_i - delta_t * x_i * omega * omega;
        t += delta_t;
    }
    
    
    
    
    return 0;
}
