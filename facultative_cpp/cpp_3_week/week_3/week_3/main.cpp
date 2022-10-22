#include <iostream>
#include <fstream>

double* range_kutta(int dim, double delta_t, double omega, double t, double x, double v){
    double elem_odd = delta_t;
    double elem_even = 1;
    double* step = new double[4];
    for (int i = 3; i <= dim; i += 2) {
        elem_odd += elem_odd * delta_t * delta_t * (- omega * omega) / ((i - 1) * i);
    }
    for (int i = 2; i <= dim; i += 2) {
        elem_even += elem_even * delta_t * delta_t * (- omega * omega) / ((i - 1) * i);
    }
    double x_temp = x;
    step[0] = t;
    step[1] = x * elem_even + v * elem_odd;
    step[2] = v * elem_even - x_temp * omega * omega * elem_odd;
    step[3] = (omega * omega) * step[1] * step[1] / 2 + step[2] * step[2] /2;
    return step;
}

int main(int argc, const char* argv[]) {
    std::ofstream fout, fout_h, fout_rk, fout_consts;
    fout.open("output.bin", std::ios::binary);
    fout_h.open("output_h.bin", std::ios::binary);
    fout_consts.open("output_consts.bin", std::ios::binary);
    fout_rk.open("output_rk.bin", std::ios::binary);
    
    double x_i, x_i_h, x_i_rk, v_i, v_i_h, v_i_rk;
    
    x_i = x_i_h = x_i_rk = atof(argv[1]);
    v_i = v_i_h = v_i_rk = atof(argv[2]);
    
    double omega = atof(argv[3]);
    double delta_t = atof(argv[4]);
    double max_t = atof(argv[5]);

    double consts[4] = {x_i, v_i, omega, (omega * omega) * x_i * x_i / 2 + v_i * v_i /2};
    fout_consts.write(reinterpret_cast<const char*>(consts), sizeof(double) * 4);
    
    double t = 0;
    
    int length = (int)(max_t / delta_t);
    
    double** data = new double*[length];
    double** data_h = new double*[length];
    double** data_rk = new double*[length];
    for (int i = 0; i < length; i++) {
        data[i] = new double[4];
        data_h[i] = new double[4];
        data_rk[i] = new double[4];
    }
    double zero_step[4] = {t, x_i, v_i, (omega * omega) * x_i * x_i / 2 + v_i * v_i /2};

    data[0] = data_h[0] = data_rk[0] = zero_step;
    
    int i = 1;
    
    while (t < max_t) {
//        метод эйлера
        data[i] = range_kutta(1, delta_t, omega, t, x_i, v_i);
        x_i = data[i][1];
        v_i = data[i][2];
        
//        метод хойна
        data_h[i] = range_kutta(2, delta_t, omega, t, x_i_h, v_i_h);
        x_i_h = data_h[i][1];
        v_i_h = data_h[i][2];

//        метод рунге кутты 4 порядка
        data_rk[i] = range_kutta(4, delta_t, omega, t, x_i_rk, v_i_rk);
        x_i_rk = data_rk[i][1];
        v_i_rk = data_rk[i][2];
        
        t += delta_t;
        i++;
    }
    
    for (i = 0; i < length; i++) {
        fout.write(reinterpret_cast<const char*>(data[i]), sizeof(double) * 4);
        fout_h.write(reinterpret_cast<const char*>(data_h[i]), sizeof(double) * 4);
        fout_rk.write(reinterpret_cast<const char*>(data_rk[i]), sizeof(double) * 4);
    }
    
    fout.close();
    fout_h.close();
    fout_rk.close();
    fout_consts.close();
    
    return 0;
}
