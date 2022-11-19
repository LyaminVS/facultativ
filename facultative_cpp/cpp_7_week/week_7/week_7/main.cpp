#include <iostream>
#include <array>
#include <fstream>
#include <cmath>

template <typename solver_class, typename state_class>
class Solver{
public:
    Solver(float from, float to, float step, state_class first_state){
        this->from = from;
        this->to = to;
        this->step = step;
        this->first_state = first_state;
        this->length = (int)((to - from) / step);
        this->all_states = new state_class[(int)length];
        this->solver = solver_class();
    }
    
    void Solve(){
        float t = from;
        all_states[0] = first_state;
        int i = 1;
        while (t < to) {
            all_states[i] = solver.make_step(all_states[i - 1], step);
            t += step;
            i++;
        }
    }
    
    void dump(std::string filename){
        std::ofstream f;
        f.open(filename);
        for (int i = 0; i < length; i++) {
            f << all_states[i].to_str();
        }
        f.close();
    }
    
    void dump_to_bin(std::string filename){
        std::ofstream f;
        f.open(filename, std::ios::binary);
        for (int i = 0; i < length; i++) {
            f.write(reinterpret_cast<const char*>(all_states[i].get_data()), sizeof(all_states[i].get_data()[0]));
        }
        f.close();
    }
    
private:
    float from, to, step, length;
    solver_class solver;
    state_class first_state;
    state_class* all_states;
};


template <typename T>
class Euler{
public:
    Euler(){
        
    }
    
    T make_step(T &state, float delta_t){
        T dfdt = state.dfdt();
        T next_state = T(state);
        next_state = state + dfdt * delta_t;
        return next_state;
    }
};

template <typename T>
class Hoin{
public:
    Hoin(){
        
    }
    T make_step(T &state, float delta_t){
        T next_state = T(state);
        T k_1 = state.dfdt() * delta_t;
        T k_2 = (state + k_1).dfdt() * delta_t;
        next_state = state + (k_1 + k_2) * 0.5;
        return next_state;
    }
};

class MyClass{
public:
    std::array<float, 2> data;
    int length;
    float omega;
    
    MyClass(){
        this->length = 2;
        this->omega = 1;
    }
    
    MyClass(float x_0, float v_0, float omega){
        this->length = 2;
        this->omega = 1;
        data[0] = x_0;
        data[1] = v_0;
        this->omega = omega;
    }
    
    float& operator[](int i) {
        return data[i];
    }
    
    MyClass operator*(float c) {
        MyClass ans = MyClass(*this);
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] * c;
        }
        return ans;
    }
    
    MyClass operator+(MyClass second) {
        MyClass ans = MyClass(*this);
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] + second.data[i];
        }
        return ans;
    }
    
    MyClass dfdt(){
        return MyClass(data[1], -this->omega * this->omega * sin(data[0]), this->omega);
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
    
    Fading(){
        this->length = 2;
        this->omega = 1;
        this->gamma = 0;
    }
    
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
        Fading ans = Fading(*this);
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] * c;
        }
        return ans;
    }
    
    Fading operator+(Fading second) {
        Fading ans = Fading(*this);
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] + second.data[i];
        }
        return ans;
    }
    
    Fading dfdt(){
        return Fading(data[1], -this->omega * this->omega * sin(data[0]) - 2 * this->gamma * data[1], this->omega, this->gamma);
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
    if (atoi(argv[1]) == 0){
        float x_0 = atof(argv[2]);
        float v_0 = atof(argv[3]);
        float omega = atof(argv[4]);
        float from = atof(argv[5]);
        float to = atof(argv[6]);
        float delta = atof(argv[7]);
        MyClass a = MyClass(x_0, v_0, omega);
        Solver<Euler<MyClass>, MyClass> solver = Solver<Euler<MyClass>, MyClass>(from, to, delta, a);
        solver.Solve();
        solver.dump("output.txt");
    }
    else if(atoi(argv[1]) == 1){
        float x_0 = atof(argv[2]);
        float v_0 = atof(argv[3]);
        float omega = atof(argv[4]);
        float gamma = atof(argv[5]);
        float from = atof(argv[6]);
        float to = atof(argv[7]);
        float delta = atof(argv[8]);
        Fading a = Fading(x_0, v_0, omega, gamma);
        Solver<Euler<Fading>, Fading> solver = Solver<Euler<Fading>, Fading>(from, to, delta, a);
        solver.Solve();
        solver.dump("output_f.txt");
    }
    
    
    return 0;
}
