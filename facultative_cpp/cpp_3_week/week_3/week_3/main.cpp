#include <iostream>
#include <fstream>

int main(int argc, const char * argv[]) {
    std::ofstream fout, fout_h, fout_consts;
    fout.open("output.json");
    fout_h.open("output_h.json");
    fout_consts.open("output_consts.json");
    
    
    
    long double x_0 = atof(argv[1]);
    long double v_0 = atof(argv[2]);
    
    long double omega = atof(argv[3]);
    
    long double delta_t = atof(argv[4]);

    long double v_i = v_0;
    long double x_i = x_0;
    long double E_i = (omega * omega) * x_i * x_i / 2 + v_i * v_i /2;
    
    fout_consts << x_0 << " " << v_0 << " " << omega << " " << E_i;
    
    long double v_i_h = v_0;
    long double x_i_h = x_0;
    long double E_i_h = (omega * omega) * x_i_h * x_i_h / 2 + v_i_h * v_i_h /2;
    
    long double t = 0;
    
    long double max_t = atof(argv[5]);
    
    long double temp_x_i;
    long double temp_x_i_h;
    
    long double k_1_x, k_2_x, k_1_v, k_2_v;
    
//    fout << t << " " << x_i << " " << v_i << " " << E_i << std::endl;
//
//    fout_h << t << " " << x_i_h << " " << v_i_h << " " << E_i_h << std::endl;
    
    while (t < max_t) {
//        метод эйлера
        temp_x_i = x_i;
        x_i = x_i + delta_t * v_i;
        v_i = v_i - delta_t * temp_x_i * omega * omega;
        E_i = (omega * omega) * x_i * x_i / 2 + v_i * v_i / 2;
        fout.precision(1000);
        fout << t << " " << x_i << " " << v_i << " " << E_i << std::endl;
//        метод хойна
        temp_x_i_h = x_i_h;
        k_1_x = delta_t * v_i_h;
        k_1_v = - delta_t * x_i_h * omega * omega;
        k_2_x = delta_t * (v_i_h + k_1_v);
        k_2_v = -delta_t * (x_i_h + k_1_x) * omega * omega;
        x_i_h = x_i_h + (k_1_x + k_2_x) / 2;
        v_i_h = v_i_h + (k_1_v + k_2_v) / 2;
        E_i_h = (omega * omega) * x_i_h * x_i_h / 2 + v_i_h * v_i_h / 2;
        fout_h.precision(1000);
        fout_h << t << " " << x_i_h << " " << v_i_h << " " << E_i_h << std::endl;
        
        t += delta_t;
    }
    
    
    return 0;
}
