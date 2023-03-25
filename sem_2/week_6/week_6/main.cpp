#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"

std::vector<double> get_data(double alpha=1, double x_0=0, int iteration_number=100){
    std::vector<double> x;
    x.push_back(x_0);
    for (int i = 0; i < iteration_number; i++) {
        x.push_back(x[i] * alpha * (1 - x[i]));
    }
    return x;
}

int main(int argc, const char * argv[]) {
    
    std::ifstream json_stream("input.json");
    nlohmann::json data = nlohmann::json::parse(json_stream);
    std::vector<double> alphas = data["alphas"].get<std::vector<double>>();
    std::vector<double> x_0s = data["x_0s"].get<std::vector<double>>();
    std::vector<int> iteration_numbers = data["iteration_numbers"].get<std::vector<int>>();
    
    nlohmann::json new_data;
    
    for (auto alpha : alphas) {
        for (auto x_0 : x_0s) {
            for (auto iteration_number : iteration_numbers) {
                new_data[std::to_string(alpha)][std::to_string(x_0)][std::to_string(iteration_number)] = get_data(alpha, x_0, iteration_number);
            }
        }
    }
    
    
    
    std::ofstream file;
    file.open("data.json");
    
    file << new_data.dump();
    
    file.close();
//
//    for (double x_i : x) {
//        file << x_i << "\t";
//    }
//
//    file << std::endl;
    
    return 0;
}
