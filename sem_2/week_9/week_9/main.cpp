#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <mutex>
#include "json.hpp"

class Cell
{
public:
    double phi;
    double sigma;
    
    Cell(double phi, double sigma=1): phi(phi), sigma(sigma){}
};

class Grid
{
private:
    const int width, height;
    const double delta_x, delta_y;
    
    std::vector<double> boundary_left;
    std::vector<double> boundary_right;
    
    std::vector<std::vector<Cell>> cells;
    std::vector<std::vector<Cell>> new_cells;
    
//    std::vector<double> cells_test(100000000000, 1.0);
    
    
public:
    Grid(int width, int height, double delta_x = 1, double delta_y = 1): width(width), height(height), delta_x(delta_x), delta_y(delta_y){}
    
    void set_boundaries(std::vector<double> left, std::vector<double> right){
        boundary_left = left;
        boundary_right = right;
    }
    
    void set_boundaries(std::vector<Cell> left, std::vector<Cell> right){
        for (int i = 0; i < height + 1; i++) {
            boundary_left[i + 1] = left[i].phi;
            boundary_right[i + 1] = right[i].phi;
        }
    }
    
    void change_left_boundary_by_buffers(std::vector<Cell> left){
        cells[0] = left;
    }
    
    void change_right_boundary_by_buffers(std::vector<Cell> right){
        cells[width + 1] = right;
    }
    
    void print_cells(){
        for (auto raw : cells) {
            for (auto cell : raw) {
                std::cout << cell.phi << " ";
            }
            std::cout << std::endl;
        }
    }
    
    void generate_random_grid(double min, double max){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib(min, max);

        for (int i = 0; i < width + 2; i++) {
            std::vector<Cell> raw;
            std::vector<Cell> new_raw;
            for (int j = 0; j < height + 2; j++) {
                raw.push_back(Cell(distrib(gen)));
                new_raw.push_back(Cell(distrib(gen)));
            }
            cells.push_back(raw);
            new_cells.push_back(new_raw);
        }
        
        create_blocks();
        create_buffers();
    }
    
    std::vector<Cell> get_first_colomn(){
        std::vector<Cell> first_colomn;
        first_colomn = cells[1];
        return first_colomn;
    }
    
    std::vector<Cell> get_last_colomn(){
        std::vector<Cell> last_colomn;
        last_colomn = cells[width];
        return last_colomn;
    }
    
    std::string get_cells_as_string(){
        std::string ans("");
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                ans = ans + std::to_string(cells[i + 1][j + 1].phi) + " ";
            }
        }
        return ans;
    }
    
    void dump(std::ofstream& stream){
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                stream << cells[i + 1][j + 1].phi << " ";
            }
        }
    }
    
    void make_one_step(double delta_t){
        #pragma omp parallel for
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                new_cells[i + 1][j + 1].phi = ((cells[i + 2][j + 1].phi + cells[i][j + 1].phi - 2 * cells[i + 1][j + 1].phi) / (delta_x * delta_x) + (cells[i + 1][j].phi + cells[i + 1][j + 2].phi - 2 * cells[i + 1][j + 1].phi) / (delta_y * delta_y)) * delta_t + cells[i + 1][j + 1].phi;
            }
        }
        
        cells.swap(new_cells);
        create_blocks();
        create_buffers();
    }

    void create_buffers(){
        for (int i = 0; i < height; i++) {
            cells[0][i + 1].phi = 2 * boundary_left[i] - cells[1][i + 1].phi;
            cells[width + 1][i + 1].phi = 2 * boundary_right[i] - cells[width][i + 1].phi;
        }
    }
    
    void create_blocks(){
        for (int i = 0; i < width; i++) {
            cells[i + 1][0].phi = cells[i + 1][1].phi;
            cells[i + 1][height + 1].phi = cells[i + 1][height].phi;
        }
    }
    
};

class Simulator
{
    double delta_t;
    int width, height, iteration_number;
    Grid grid;
    std::vector<std::string> history;
    std::ofstream& stream;
    
public:
    Simulator(int width, int height, std::vector<double> left_boundary, std::vector<double> right_boundary, int iteration_number, std::ofstream& stream, double delta_t=0.2): delta_t(delta_t), width(width), height(height), grid(width, height), iteration_number(iteration_number), history(iteration_number), stream(stream)
    {
        double min_1 = *std::min_element(std::begin(left_boundary), std::end(left_boundary));
        double min_2 = *std::min_element(std::begin(right_boundary), std::end(right_boundary));
        
        double max_1 = *std::max_element(std::begin(left_boundary), std::end(left_boundary));
        double max_2 = *std::max_element(std::begin(right_boundary), std::end(right_boundary));
        
        double min = std::min(min_1, min_2);
        double max = std::max(max_1, max_2);
        
        grid.set_boundaries(left_boundary, right_boundary);
        
        grid.generate_random_grid(min, max);
    }
    
    void run(){
        for (int i = 0; i < iteration_number; i++) {
            grid.make_one_step(delta_t);
        }
        grid.dump(stream);
        
    }
    
    void dump(std::ofstream& stream){
        for (auto step : history) {
            stream << step << std::endl;
        }
    }
};



int main(int argc, const char * argv[]) {
    std::filesystem::path json_path("input.json");
    if (argc == 2) {
        json_path = argv[1];
    }
    std::ifstream json_stream(json_path);
    nlohmann::json data = nlohmann::json::parse(json_stream);
    json_stream.close();
    
    int width = std::stoi((std::string)data["width"]);
    int height = std::stoi((std::string)data["height"]);
    int iteration_number = std::stoi((std::string)data["iteration_number"]);
    
    std::vector<double> left_boundaries;
    std::vector<double> right_boundaries;

    for (std::string elem : data["left_boundaries"]){
        left_boundaries.push_back(std::stod(elem));
    }
    for (std::string elem : data["right_boundaries"]){
        right_boundaries.push_back(std::stod(elem));
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distrib(0, 10);
    std::ofstream file;
    file.open("test.txt");
    auto sim = Simulator(width, height, left_boundaries, right_boundaries, iteration_number, file);
    sim.run();
//    sim.dump(file);
    file.close();
    return 0;
}
