#include <iostream>
#include <fstream>
#include <math.h>

int main(int argc, const char * argv[]) {
    std::ofstream fout, fout_h;
    fout.open("output.bin");
    fout_h.open("output_h.bin");
    float x_0 = 10;
    float v_0 = 0;
    
    float omega = 1;
    
    float delta_t = 0.001;

    float v_i = v_0;
    float x_i = x_0;
    float E_i = (1 / sqrt(omega)) * x_i * x_i / 2 + v_i * v_i /2;
    
    float v_i_h = v_0;
    float x_i_h = x_0;
    float E_i_h = (1 / sqrt(omega)) * x_i_h * x_i_h / 2 + v_i_h * v_i_h /2;
    
    float t = 0;
    
    float temp_x_i;
    float temp_x_i_h;
    
    float k_1_x, k_2_x, k_1_v, k_2_v;
    
    fout << t << " " << x_i << " " << v_i << " " << E_i << std::endl;
    
    fout_h << t << " " << x_i_h << " " << v_i_h << " " << E_i_h << std::endl;
    
    while (t < 1000) {
//        метод эйлера
        temp_x_i = x_i;
        x_i = x_i + delta_t * v_i;
        v_i = v_i - delta_t * temp_x_i * omega * omega;
        E_i = (1 / sqrt(omega)) * x_i * x_i / 2 + v_i * v_i / 2;
        fout << t << " " << x_i << " " << v_i << " " << E_i << std::endl;
//        метод хойна
        temp_x_i_h = x_i_h;
        k_1_x = delta_t * v_i_h;
        k_1_v = - delta_t * x_i_h * omega * omega;
        k_2_x = delta_t * (v_i_h + k_1_v);
        k_2_v = -delta_t * (x_i_h + k_1_x);
        x_i_h = x_i_h + (k_1_x + k_2_x) / 2;
        v_i_h = v_i_h + (k_1_v + k_2_v) / 2;
        E_i_h = (1 / sqrt(omega)) * x_i_h * x_i_h / 2 + v_i_h * v_i_h / 2;
        
        fout_h << t << " " << x_i_h << " " << v_i_h << " " << E_i_h << std::endl;
        
        t += delta_t;
    }
    
    
    return 0;
}
