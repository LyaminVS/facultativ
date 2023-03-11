#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>
#include <string>

void print_vector(std::vector<int> &vector){
    for (auto i: vector){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

double get_result_for_N(int N, int K, auto f){
    std::vector<int> all_nums;
    
    for (int i = 0; i < K; i++) {
        all_nums.push_back(i + 1);
    }
    
    int all_iter = 0;
    int good_iter = 0;
    
    for (int j = 0; j < 1000; j++) {
        std::vector<int> nums_copy;
        std::copy(std::begin(all_nums), std::end(all_nums), std::back_inserter(nums_copy));
        for (int i = 0; i < N; i++) {
            int rand_num = f();
            auto temp = std::remove_if(std::begin(nums_copy), std::end(nums_copy), [rand_num](int x){return rand_num == x;});
            if (temp != std::end(nums_copy)) {
                nums_copy.erase(temp, std::end(nums_copy));
            }
        }
        all_iter++;
        if (nums_copy.empty()) {
            good_iter++;
        }
    }
    return (double)good_iter / all_iter;
}

std::vector<int> int_to_base(int num, int base, int N){
    std::vector<int> result(N);
    std::fill(std::begin(result), std::end(result), 0);
    int i = 0;
    while (num != 0) {
        result[N - 1 - i] = num % base;
        num = num / base;
        i++;
    }
    return result;
}

double get_real_distrib(int N, int K) {
    int good_cases = 0;
    std::vector<int> event(N);
    std::vector<int> all_nums(K);
    std::iota(std::begin(all_nums), std::end(all_nums), 1);
    std::fill(std::begin(event), std::end(event), 1);
    int flag;
    for (int i = 0; i < pow(K, N); i++) {
        flag = 1;
        for (auto item: all_nums) {
            if (std::count(std::begin(event), std::end(event), item) == 0){
                flag = 0;
            }
        }
        if (flag == 1) {
            good_cases++;
        }
        
        event = int_to_base(i, K, N);
        std::for_each(std::begin(event), std::end(event), [](auto &x){x++;});
    }
    
    return (double)good_cases/pow(K, N);
}
    
int main(int argc, const char * argv[]) {
    int K = std::stoi(argv[2]);
    int N = std::stoi(argv[1]);
//    int K = 2;
//    int N = 4;
    
    std::mt19937 gen(5649);
    std::uniform_int_distribution distrib(1, K);
    auto f = std::bind(distrib, gen);
    std::ofstream file, real_file;
    file.open("file.txt", std::ios::out);
    real_file.open("real_file.txt", std::ios::out);
    std::vector<std::string> results;
    std::vector<std::string> results_real;
    for (int i = 1; i <= N; i++) {
        results.push_back(std::to_string(get_result_for_N(i, K, f)));
    }
    for (int i = 1; i <= N; i++) {
        results_real.push_back(std::to_string(get_real_distrib(i, K)));
    }

    std::ostream_iterator<std::string> out_itr(file, "\n");
    copy(results.begin(), results.end(), out_itr);

    std::ostream_iterator<std::string> out_itr_1(real_file, "\n");
    copy(results_real.begin(), results_real.end(), out_itr_1);

    file.close();
    

    
    return 0;
}
