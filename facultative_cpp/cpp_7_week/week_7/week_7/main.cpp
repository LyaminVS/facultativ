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
        for (int i = 0; i < length; i++) {
            f << std::to_string(times[i]) << "\t" << all_states[i].to_str();
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
    std::vector<state_class> all_states;
    std::vector<float> times;
};


template <typename T>
class Euler{
public:
    Euler(){
        
    }
    
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
    Hoin(){
        
    }
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
    RungeKutta(){
        
    }
    T make_step(T &state, float delta_t, float t){
        T k_1, k_2, k_3, k_4;
        T next_state = T();
        k_1 = state.dfdt(t) * delta_t;
        k_2 = (state + k_1 * 0.5).dfdt(t + delta_t * 0.5) * delta_t;
        k_3 = (state + k_2 * 0.5).dfdt(t + delta_t * 0.5) * delta_t;
        k_4 = (state + k_3).dfdt(t + delta_t) * delta_t;
        next_state = state + (k_1 + k_2 * 2 + k_3 * 3 + k_4) * 0.1666666666666666666;
        return next_state;
    }
};

class SimpleGarmonic{
public:
    std::array<float, 2> data;
    int length;
    float omega;
    
    SimpleGarmonic(){
    }
    
    SimpleGarmonic(float x_0, float v_0, float omega){
        this->length = 2;
        this->omega = 1;
        data[0] = x_0;
        data[1] = v_0;
        this->omega = omega;
    }
    
    float& operator[](int i) {
        return data[i];
    }
    
    SimpleGarmonic operator*(float c) {
        SimpleGarmonic ans = SimpleGarmonic();
        ans.omega = this->omega;
        ans.length = this->length;
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] * c;
        }
        return ans;
    }
    
    SimpleGarmonic operator+(SimpleGarmonic second) {
        SimpleGarmonic ans = SimpleGarmonic();
        ans.omega = this->omega;
        ans.length = this->length;
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] + second.data[i];
        }
        return ans;
    }
    
    SimpleGarmonic dfdt(float t){
        return SimpleGarmonic(data[1], -this->omega * this->omega * sin(data[0]), this->omega);
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
    
    Fading dfdt(float t){
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

class Forced{
public:
    std::array<float, 2> data;
    int length;
    float omega;
    float gamma;
    float omega_f;
    float A;
    
    Forced(){
    }
    
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
        Forced ans = Forced();
        ans.omega = this->omega;
        ans.length = this->length;
        ans.gamma = this->gamma;
        ans.omega_f = this->omega_f;
        ans.A = this->A;
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] * c;
        }
        return ans;
    }
    
    Forced operator+(Forced second) {
        Forced ans = Forced();
        ans.omega = this->omega;
        ans.length = this->length;
        ans.gamma = this->gamma;
        ans.omega_f = this->omega_f;
        ans.A = this->A;
        for(int i = 0; i < this->length; i++){
            ans.data[i] = this->data[i] + second.data[i];
        }
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

int main(int argc, const char * argv[]) {
    std::ifstream f(argv[1]);

    json data = json::parse(f);

    int type = std::stoi(data.value("type", "oops"));
    
    int method_type = std::stoi(data.value("method_type", "oops"));
    
    std::cout << type;
    

    
    using ModelType = SimpleGarmonic;
    
    if (type == 0) {
        using ModelType = SimpleGarmonic;
    }
    else if (type == 1) {
        using ModelType = Fading;
    }
    else{
        using ModelType = Forced;
    }
    
    using AccuracyType = Euler<ModelType>;
    
    if (method_type == 0) {
        using AccuracyType = Euler<ModelType>;
    }
    else if (method_type == 1) {
        using AccuracyType = Hoin<ModelType>;
    }
    else{
        using AccuracyType = RungeKutta<ModelType>;
    }
    
    ModelType a = ModelType(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
    Solver<AccuracyType, ModelType> solver = Solver<AccuracyType, ModelType>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
    solver.Solve();
    solver.dump("output.txt");
    
    
//    if (type == 0){
//        MyClass a = MyClass(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")));
//        Solver<Euler<MyClass>, MyClass> solver = Solver<Euler<MyClass>, MyClass>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
//        solver.Solve();
//        solver.dump("output.txt");
//    }
//    else if (type == 1){
//        Fading a = Fading(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")));
//        Solver<Euler<Fading>, Fading> solver = Solver<Euler<Fading>, Fading>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
//        solver.Solve();
//        solver.dump("output_f.txt");
//    }
//    else if (type == 2){
//        Forced a = Forced(std::stof(data.value("x_0", "oops")), std::stof(data.value("v_0", "oops")), std::stof(data.value("omega", "oops")), std::stof(data.value("gamma", "oops")), std::stof(data.value("omega_f", "oops")), std::stof(data.value("A", "oops")));
//        Solver<RungeKutta<Forced>, Forced> solver = Solver<RungeKutta<Forced>, Forced>(std::stof(data.value("from", "oops")), std::stof(data.value("to", "oops")), std::stof(data.value("delta", "oops")), a);
//        solver.Solve();
//        solver.dump("output_rk.txt");
//    }
    

    return 0;
}
