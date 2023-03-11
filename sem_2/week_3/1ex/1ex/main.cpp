#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include <functional>
#include <fstream>

template<typename T>
void print_vector(std::vector<T> &vector){
    for (auto i: vector){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int mean(std::vector<int> vec){
    return std::accumulate(std::begin(vec), std::end(vec), 0) / (int)std::size(vec);
}


int main(int argc, const char * argv[]) {
    int T = atoi(argv[1]);
    int steps[] = {-1, 1};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distrib(0, 1);
    auto f = std::bind(distrib, gen);
    int sum = 0;
    std::vector<int> results;
    results.push_back(sum);
//    mean_results.p
    for (int i = 0; i < T; i++) {
        sum += steps[f()];
        results.push_back(sum);
        
    }
//    print_vector<std::string>(results);
    
    std::ofstream file;
    file.open("file.txt", std::ios::out);
    
    
    
    std::ostream_iterator<std::string> out_itr(file, "\n");
    copy(results.begin(), results.end(), out_itr);
    return 0;
}
