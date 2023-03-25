#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <utility>
#include <algorithm>
#include <filesystem>
#include "json.hpp"

template<typename T>
void print_vector(std::vector<T> &vector){
    for (auto i: vector){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
   
class Ball{
public:
    double x;
    double v;
    double m;
    double r;
    
public:
    Ball(double x, double v, double m, double r): x(x), v(v), m(m), r(r){};
    
    void print(std::ostream& stream, bool to_string = true){
        if (to_string) {
            stream << x << " " << v << " " << m;
        }
        else{
            stream << "x = " << x << std::endl << "v = " << v << std::endl << "m = " << m << std::endl;
        }
    }
};

class Simulator{
    std::vector<Ball> balls;
    double delta_t;
    int iteration_number;
    int ball_number;
    std::random_device rd;
    std::mt19937 gen;
    double left_bound;
    double right_bound;
    std::vector<std::vector<double>> x_history;
    
public:
    Simulator(int iteration_number, double delta_t, int ball_number): iteration_number(iteration_number), delta_t(delta_t), ball_number(ball_number), left_bound(0), right_bound(80){
        gen = std::mt19937(rd());
        std::uniform_real_distribution<double> distrib(-2, 2);
        for (int i = 0; i < ball_number; i++) {
            balls.push_back(Ball(i * 3 + 2, distrib(gen), abs(distrib(gen)), 1.0));
        }
    }
    
    void colide(std::pair<std::vector<int>, double> data){
        std::vector<int> balls_id = std::get<0>(data);

        // столкновение со стеной
        if (static_cast<int>(balls_id.size()) == 1){
            
            Ball& ball = balls[balls_id[0]];
            ball.v = -ball.v;
        }
        
        // столкновение с другим шариком
        if (static_cast<int>(balls_id.size()) == 2){
            Ball& first_ball = balls[balls_id[0]];
            Ball& second_ball = balls[balls_id[1]];
            double temp_first_v = first_ball.v;
            first_ball.v = ((first_ball.m - second_ball.m) * first_ball.v + 2. * second_ball.m * second_ball.v) / (first_ball.m + second_ball.m);
            second_ball.v = ((second_ball.m - first_ball.m) * second_ball.v + 2. * first_ball.m * temp_first_v) / (first_ball.m + second_ball.m);
        }
        
    }
    
    double get_bound_collision_time(Ball ball){
        // скорость 0
        if (ball.v == 0){
            return INFINITY;
        }
        // столкновение с правой границей
        if (ball.v > 0) {
            return (right_bound - ball.x - ball.r) / ball.v;
        }
        // столкновение с левой границей
        if (ball.v < 0) {
//            std::cout << ball.x << " " << left_bound << " " << ball.r << std::endl;
//            std::cout << ball.x - left_bound - ball.r;
            return (ball.x - left_bound - ball.r) / abs(ball.v);
        }
        return INFINITY;
        
    }
    
    double get_collision_time(Ball first, Ball second){
        // равные скорости
        if (first.v == second.v){
            return INFINITY;
        }
        
        // положительные скорости
        if (first.v >= 0 && second.v >= 0 && first.v > second.v && first.x > second.x){
            return INFINITY;
        }
        if (first.v >= 0 && second.v >= 0 && first.v > second.v && first.x < second.x){
            return ((second.x - first.x) - first.r - second.r) / (first.v - second.v);
        }
        if (first.v >= 0 && second.v >= 0 && first.v < second.v && first.x > second.x){
            return ((first.x - second.x) - first.r - second.r) / (second.v - first.v);
        }
        if (first.v >= 0 && second.v >= 0 && first.v < second.v && first.x < second.x){
            return INFINITY;
        }
        
        // отрицаткльные скорости
        if (first.v <= 0 && second.v <= 0 && abs(first.v) > abs(second.v) && first.x > second.x){
            return ((first.x - second.x) - first.r - second.r) / (abs(first.v) - abs(second.v));
        }
        if (first.v <= 0 && second.v <= 0 && abs(first.v) > abs(second.v) && first.x < second.x){
            return INFINITY;
        }
        if (first.v <= 0 && second.v <= 0 && abs(first.v) < abs(second.v) && first.x > second.x){
            return INFINITY;
        }
        if (first.v <= 0 && second.v <= 0 && abs(first.v) < abs(second.v) && first.x < second.x){
            return ((second.x - first.x) - first.r - second.r) / (abs(second.v) - abs(first.v));
        }
        
        // скорости разных знаков
        if (first.v <= 0 && second.v >= 0 && first.x > second.x){
            return (first.x - second.x - first.r - second.r) / (second.v - first.v);
        }
        if (first.v <= 0 && second.v >= 0 && first.x < second.x){
            return INFINITY;
        }
        if (first.v >= 0 && second.v <= 0 && first.x > second.x){
            return INFINITY;
        }
        if (first.v >= 0 && second.v <= 0 && first.x < second.x){
            return (second.x - first.x - first.r - second.r) / (first.v - second.v);
        }
        return INFINITY;
    }
    
    std::pair<std::vector<int>, double> next_collision(){
        std::vector<int> next_balls;
        double min_time = INFINITY;
        
        // столкновение со стеной
        for (int i = 0; i < static_cast<int>(balls.size()); i++){
            double t = get_bound_collision_time(balls[i]);
//            std::cout << t;
            if (t < min_time){
                next_balls.clear();
                next_balls.push_back(i);
                min_time = t;
            }
        }
        
//        std::cout << min_time;
        
        // столкновение с другим шариком
        for (int i = 0; i < static_cast<int>(balls.size()); i++) {
            for (int j = i + 1; j < static_cast<int>(balls.size()); j++) {
                double t = get_collision_time(balls[i], balls[j]);
                if (t < min_time){
                    next_balls.clear();
                    next_balls.push_back(i);
                    next_balls.push_back(j);
                    min_time = t;
                }
            }
        }
        
        return std::make_pair(next_balls, min_time);
    }
    
    void move_all(double time){
        for (Ball &ball : balls) {
            ball.x = ball.x + ball.v * time;
        }
    }
    
    void move(int ball_id, double time){
        balls[ball_id].x =  balls[ball_id].x + balls[ball_id].v * time;
    }
    
    void add_to_history(){
        std::vector<double> new_vector;
        for (auto ball : balls) {
            new_vector.push_back(ball.x);
        }
        for (auto ball : balls) {
            new_vector.push_back(ball.v);
        }
        for (auto ball : balls) {
            new_vector.push_back(ball.m * ball.v * ball.v / 2);
        }
        x_history.push_back(new_vector);
    }
    
    void run(){
        add_to_history();
        for (int i = 0; i < iteration_number; i++) {
            iterate();
        }
    }
    
    void run_to_next_collision(double collision_time){
        while (collision_time > delta_t){
            move_all(delta_t);
            collision_time -= delta_t;
            add_to_history();
        }
        move_all(collision_time);
        if (collision_time == 0){
            add_to_history();
        }
        
    }
    
    void iterate(){
        auto next_collision_result = next_collision();
        double collision_time = std::get<1>(next_collision_result);
        std::vector<int> colision_balls = std::get<0>(next_collision_result);
        run_to_next_collision(collision_time);
        colide(next_collision_result);
    }
    
    void dump(std::filesystem::path filename){
        std::ofstream file;
        file.open(filename);
        for (auto vec: x_history) {
            for (auto x : vec) {
                file << x << " ";
            }
            file << std::endl;
        }
    }
};

int main(int argc, const char * argv[]) {
    std::filesystem::path json_path("input.json");
    if (argc == 2) {
        json_path = argv[1];
    }
    std::ifstream json_stream("input.json");
    nlohmann::json data = nlohmann::json::parse(json_stream);
    
    int collision_number = std::stoi(data.value("collision_number", "oops"));
    double delta_t = std::stod(data.value("delta_t", "oops"));
    int particle_number = std::stoi(data.value("particle_number", "oops"));
    std::cout << collision_number << "\t" << delta_t << "\t" << particle_number;
    Simulator sim(collision_number, delta_t, particle_number);
    sim.run();
    std::filesystem::path path("test.txt");
    sim.dump(path);
    return 0;
}
