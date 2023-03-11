#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <cmath>
#include "json.hpp"

using json = nlohmann::json;

template <typename solver_class, typename state_class>
class Solver{
public:
    Solver(float from, float to, float step, state_class first_state): from(from), to(to), step(step), first_state(first_state), length((int)((to - from) / step)), solver(solver_class()){}
    
    void Solve(){
        float t = from;
        times.push_back(t);
        all_states.push_back(first_state);
        int i = 1;
        while (t < to) {
            all_states.push_back(solver.make_step(all_states[i - 1], step, t));
            t += step;
            times.push_back(t);
            i++;
        }
    }
    
    void dump(std::string filename){
        std::ofstream f;
        f.open(filename);
        for (int i = 1; i < length; i++) {
            f << std::to_string(times[i]) << "\t" << all_states[i].to_str();
        }
        f.close();
    }
    
    void dump_to_bin(std::string filename){
        std::ofstream f;
        f.open(filename, std::ios::binary);
        for (int i = 0; i < length - 2; i++) {
            f.write(reinterpret_cast<const char*>(all_states[i].get_data()), sizeof(all_states[i].get_data()[0]));
        }
        f.close();
    }
    
private:
    float from, to, step, length;
    solver_class solver;
    state_class first_state;
    std::vector<state_class> all_states;
    std::vector<float> times;
};


template <typename T>
class Euler{
public:
    T make_step(T &state, float delta_t, float t){
        T dfdt = state.dfdt(t);
        T next_state = T(state);
        next_state = state + dfdt * delta_t;
        return next_state;
    }
};

template <typename T>
class Hoin{
public:
    T make_step(T &state, float delta_t, float t){
        T next_state = T(state);
        T k_1 = state.dfdt(t) * delta_t;
        T k_2 = (state + k_1).dfdt(t) * delta_t;
        next_state = state + (k_1 + k_2) * 0.5;
        return next_state;
    }
};

template <typename T>
class RungeKutta{
public:
    T make_step(T &state, double delta_t, double t){
        T k_1 = state.dfdt(t);
        T k_2 = (state + k_1 * 0.5 * delta_t).dfdt(t + (delta_t * 0.5));
        T k_3 = (state + k_2 * 0.5 * delta_t).dfdt(t + (delta_t * 0.5));
        T k_4 = (state + k_3 * delta_t).dfdt(t + delta_t);
        T next_state = state + (k_1 + (k_2 * 2) + (k_3 * 2) + k_4) * (delta_t / 6);
        return next_state;
    }
};

class SimpleHarmonic{
public:
    std::array<double, 2> data;
    int length;
    float omega;
    
    SimpleHarmonic(double x_0, double v_0, double omega){
        this->length = 2;
        this->omega = 1;
        data[0] = x_0;
        data[1] = v_0;
        this->omega = omega;
    }
    
    double& operator[](int i) {
        return data[i];
    }
    
    SimpleHarmonic operator*(double c) {
        SimpleHarmonic ans = SimpleHarmonic(data[0] * c, data[1] * c, omega);
        return ans;
    }
    
    SimpleHarmonic operator+(SimpleHarmonic second) {
        SimpleHarmonic ans = SimpleHarmonic(data[0] + second[0], data[1] + second[1], omega);
        return ans;
    }
    
    SimpleHarmonic dfdt(double t){
        return SimpleHarmonic(data[1], -this->omega * this->omega * data[0], this->omega);
    }
    
    void print(){
        std::cout << data[0] << "\t" << data[1] << "\t" << length << std::endl;
    }
    
    std::string to_str(){
        return std::to_string(data[0]) + "\t" + std::to_string(data[1]) + "\n";
    }
    
    double* get_data(){
        double* data_out = new double[length];
        for (int i = 0; i < length; i++) {
            data_out[i] = data[i];
        }
        return data_out;
    }
};


class PhysicalOscillator{
public:
    std::array<float, 2> data;
    int length;
    float omega;
    
    PhysicalOscillator(float x_0, float v_0, float omega){
        this->length = 2;
        this->omega = 1;
        data[0] = x_0;
        data[1] = v_0;
        this->omega = omega;
    }
    
    float& operator[](int i) {
        return data[i];
    }
    
    PhysicalOscillator operator*(float c) {
        PhysicalOscillator ans = PhysicalOscillator(data[0] * c, data[1] * c, omega);
        return ans;
    }
    
    PhysicalOscillator operator+(PhysicalOscillator second) {
        PhysicalOscillator ans = PhysicalOscillator(data[0] + second[0], data[1] + second[1], omega);
        return ans;
    }
    
    PhysicalOscillator dfdt(float t){
        return PhysicalOscillator(data[1], -this->omega * this->omega * sin(data[0]), this->omega);
    }
    
    void print(){
        std::cout << data[0] << "\t" << data[1] << "\t" << length << std::endl;
    }
    
    std::string to_str(){
        return std::to_string(data[0]) + "\t" + std::to_string(data[1]) + "\n";
    }
    
    double* get_data(){
        double* data_out = new double[length];
        for (int i = 0; i < length; i++) {
            data_out[i] = data[i];
        }
        return data_out;
    }
};

class Fading{
public:
    std::array<float, 2> data;
    int length;
    float omega;
    float gamma;
    
    Fading(float x_0, float v_0, float omega, float gamma){
        this->length = 2;
        this->omega = 1;
        data[0] = x_0;
        data[1] = v_0;
        this->omega = omega;
        this->gamma = gamma;
    }
    
    float& operator[](int i) {
        return data[i];
    }
    
    Fading operator*(float c) {
        Fading ans = Fading(data[0] * c, data[1] * c, omega, gamma);
        return ans;
    }
    
    Fading operator+(Fading second) {
        Fading ans = Fading(data[0] + second[0], data[1] + second[1], omega, gamma);
        return ans;
    }
    
    Fading dfdt(float t){
        return Fading(data[1], -this->omega * this->omega * data[0] - 2 * this->gamma * data[1], this->omega, this->gamma);
    }
    
    void print(){
        std::cout << data[0] << "\t" << data[1] << "\t" << length << std::endl;
    }
    
    std::string to_str(){
        return std::to_string(data[0]) + "\t" + std::to_string(data[1]) + "\n";
    }
    
    double* get_data(){
        double* data_out = new double[length];
        for (int i = 0; i < length; i++) {
            data_out[i] = data[i];
        }
        return data_out;
    }
};

class Forced{
public:
    std::array<float, 2> data;
    int length;
    float omega;
    float gamma;
    float omega_f;
    float A;
    
    Forced(float x_0, float v_0, float omega, float gamma, float omega_f, float A){
        this->length = 2;
        data[0] = x_0;
        data[1] = v_0;
        this->omega = omega;
        this->gamma = gamma;
        this->omega_f = omega_f;
        this->A = A;
    }
    
    float& operator[](int i) {
        return data[i];
    }
    
    Forced operator*(float c) {
        Forced ans = Forced(data[0] * c, data[1] * c, omega, gamma, omega_f, A);
        return ans;
    }
    
    Forced operator+(Forced second) {
        Forced ans = Forced(data[0] + second[0], data[1] + second[1], omega, gamma, omega_f, A);
        return ans;
    }
    
    Forced dfdt(float t){
        return Forced(data[1], A * cos(omega_f * t) - this->omega * this->omega * sin(data[0]) - 2 * this->gamma * this->omega * data[1], this->omega, this->gamma, this->omega_f, this->A);
    }
    
    void print(){
        std::cout << data[0] << "\t" << data[1] << "\t" << length << std::endl;
    }
    
    std::string to_str(){
        return std::to_string(data[0]) + "\t" + std::to_string(data[1]) + "\n";
    }
    
    double* get_data(){
        double* data_out = new double[length];
        for (int i = 0; i < length; i++) {
            data_out[i] = data[i];
        }
        return data_out;
    }
};

class DoublePendulum{
public:
    std::array<float, 4> data;
    int length;
    float l;
    float m;
    float g;
    
    DoublePendulum(float phi_1, float phi_2, float v_1, float v_2, float l, float m){
        length = 4;
        data[0] = phi_1;
        data[1] = phi_2;
        data[2] = v_1;
        data[3] = v_2;
        this->l = l;
        this->m = m;
        this->g = 9.8;
    }
    
    float& operator[](int i) {
        return data[i];
    }
    
    DoublePendulum operator*(float c) {
        DoublePendulum ans = DoublePendulum(data[0] * c, data[1] * c, data[2] * c, data[3] * c, l, m);
        return ans;
    }
    
    DoublePendulum operator+(DoublePendulum second) {
        DoublePendulum ans = DoublePendulum(data[0] + second[0], data[1] + second[1], data[2] + second[2], data[3] + second[3], l, m);
        return ans;
    }
    
    DoublePendulum dfdt(float t){
        float a = - data[3] * data[3] * l * sin(data[1] - data[0]) + 2 * g * sin(data[0]);
        float b = data[2] * data[2] * l * sin(data[1] - data[0]) + g * sin(data[1]);
        return DoublePendulum(data[2], data[3], (b * cos(data[1] - data[0]) - a) / (2 * l - l * cos(data[1] - data[0]) * cos(data[1] - data[0])), (a * cos(data[1] - data[0]) - 2 * b) / (2 * l - l * cos(data[1] - data[0]) * cos(data[1] - data[0])), l, m);
    }
    
    void print(){
        std::cout << data[0] << "\t" << data[1] << "\t" << data[2] << "\t" << data[3] << "\t" << length << std::endl;
    }
    
    std::string to_str(){
        return std::to_string(data[0]) + "\t" + std::to_string(data[1]) + "\t" + std::to_string(data[2]) + "\t" + std::to_string(data[3]) + "\n";
    }
    
    double* get_data(){
        double* data_out = new double[length];
        for (int i = 0; i < length; i++) {
            data_out[i] = data[i];
        }
        return data_out;
    }
};

class Kapitsa{
public:
    std::array<float, 2> data;
    int length;
    float nu;
    float l;
    float a;
    float g;
    
    Kapitsa(float phi, float v, float nu, float l, float a){
        this->length = 2;
        data[0] = phi;
        data[1] = v;
        this->nu = nu;
        this->l = l;
        this->a = a;
        this->g = 9.8;
    }
    
    float& operator[](int i) {
        return data[i];
    }
    
    Kapitsa operator*(float c) {
        Kapitsa ans = Kapitsa(data[0] * c, data[1] * c, nu, l, a);
        return ans;
    }
    
    Kapitsa operator+(Kapitsa second) {
        Kapitsa ans = Kapitsa(data[0] + second[0], data[1] + second[1], nu, l, a);
        return ans;
    }
    
    Kapitsa dfdt(float t){
        return Kapitsa(data[1], -(a * nu * nu * cos(nu * t) + g) * sin(data[0]) / l, nu, l, a);
    }
    
    void print(){
        std::cout << data[0] << "\t" << data[1] << "\t" << length << std::endl;
    }
    
    std::string to_str(){
        return std::to_string(data[0]) + "\t" + std::to_string(data[1]) + "\n";
    }
    
    double* get_data(){
        double* data_out = new double[length];
        for (int i = 0; i < length; i++) {
            data_out[i] = data[i];
        }
        return data_out;
    }
};

int main(int argc, const char * argv[]) {
    std::ifstream f(argv[1]);

    json data = json::parse(f);

    int type = std::stoi(data.value("type", "oops"));
    
    int method_type = std::stoi(data.value("method_type", "oops"));
    
    if (type == 0){
        switch (method_type) {
            case 0:
            {
                SimpleHarmonic a = SimpleHarmonic(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
                Solver<Euler<SimpleHarmonic>, SimpleHarmonic> solver = Solver<Euler<SimpleHarmonic>, SimpleHarmonic>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 1:
            {
                SimpleHarmonic a = SimpleHarmonic(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
                Solver<Hoin<SimpleHarmonic>, SimpleHarmonic> solver = Solver<Hoin<SimpleHarmonic>, SimpleHarmonic>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 2:
            {
                SimpleHarmonic a = SimpleHarmonic(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
                Solver<RungeKutta<SimpleHarmonic>, SimpleHarmonic> solver = Solver<RungeKutta<SimpleHarmonic>, SimpleHarmonic>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
        }
        
    }
    else if (type == 1){
        switch (method_type) {
            case 0:
            {
                PhysicalOscillator a = PhysicalOscillator(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
                Solver<Euler<PhysicalOscillator>, PhysicalOscillator> solver = Solver<Euler<PhysicalOscillator>, PhysicalOscillator>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 1:
            {
                PhysicalOscillator a = PhysicalOscillator(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
                Solver<Hoin<PhysicalOscillator>, PhysicalOscillator> solver = Solver<Hoin<PhysicalOscillator>, PhysicalOscillator>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 2:
            {
                PhysicalOscillator a = PhysicalOscillator(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
                Solver<RungeKutta<PhysicalOscillator>, PhysicalOscillator> solver = Solver<RungeKutta<PhysicalOscillator>, PhysicalOscillator>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
        }
    }
    else if (type == 2){
        switch (method_type) {
            case 0:
            {
                Fading a = Fading(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")));
                Solver<Euler<Fading>, Fading> solver = Solver<Euler<Fading>, Fading>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 1:
            {
                Fading a = Fading(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")));
                Solver<Hoin<Fading>, Fading> solver = Solver<Hoin<Fading>, Fading>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 2:
            {
                Fading a = Fading(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")));
                Solver<RungeKutta<Fading>, Fading> solver = Solver<RungeKutta<Fading>, Fading>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
        }
    }

    else if (type == 3){
        switch (method_type) {
            case 0:
            {
                Forced a = Forced(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")), std::stof(data.value("omega_f", "oops")), std::stof(data.value("A", "oops")));
                Solver<Euler<Forced>, Forced> solver = Solver<Euler<Forced>, Forced>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 1:
            {
                Forced a = Forced(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")), std::stof(data.value("omega_f", "oops")), std::stof(data.value("A", "oops")));
                Solver<Hoin<Forced>, Forced> solver = Solver<Hoin<Forced>, Forced>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
            case 2:
            {
                Forced a = Forced(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")), std::stof(data.value("omega_f", "oops")), std::stof(data.value("A", "oops")));
                Solver<RungeKutta<Forced>, Forced> solver = Solver<RungeKutta<Forced>, Forced>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
                solver.Solve();
                solver.dump(data.value("output", "oops"));
                break;
            }
        }
    }
    
    else if(type == 4){
        DoublePendulum a = DoublePendulum(std::stof(data.value("phi_1", "oops")), std::stof(data.value("phi_2", "oops")), std::stof(data.value("v_1", "oops")), std::stof(data.value("v_2", "oops")), std::stof(data.value("l", "oops")), std::stof(data.value("m", "oops")));
        Solver<RungeKutta<DoublePendulum>, DoublePendulum> solver = Solver<RungeKutta<DoublePendulum>, DoublePendulum>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
        solver.Solve();
        solver.dump(data.value("output", "oops"));
    }
    
    else if(type == 5){
        Kapitsa a = Kapitsa(std::stof(data.value("phi", "oops")), std::stof(data.value("v", "oops")), std::stof(data.value("nu", "oops")), std::stof(data.value("l", "oops")), std::stof(data.value("a", "oops")));
        Solver<RungeKutta<Kapitsa>, Kapitsa> solver = Solver<RungeKutta<Kapitsa>, Kapitsa>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
        solver.Solve();
        solver.dump(data.value("output", "oops"));
    }

    return 0;
}
