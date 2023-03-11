#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include <functional>

template<typename T>
void print_vector(std::vector<T> &vector){
    for (auto i: vector){
        std::cout << i << " ";
    }
    std::cout << std::endl;
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

int one_experiment(std::string searching_string, std::vector<char> dictionary){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distrib(0, 2);
    auto f = std::bind(distrib, gen);
    int number_of_guesses = 0;
    bool guessed = false;
    int new_guess;
    std::string last_values("");
    while(!guessed){
        number_of_guesses++;
        new_guess = f();
        last_values += dictionary[new_guess];
        if ((int)last_values.size() > (int)searching_string.size()){
            last_values = last_values.substr(1);
        }
        if (last_values == searching_string){
            guessed = true;
        }
    }
    return number_of_guesses;
}




int main(int argc, const char * argv[]) {
    std::vector<char> dictionary = {'1', '2', '3'};
    std::string searching_string("111");

    std::vector<int> all_results;
    
    std::cout << searching_string << std::endl;

    int power = 1000;

    for (int i = 0; i < power; i++) {
        all_results.push_back(one_experiment(searching_string, dictionary));
    }

    double mean = (double)std::accumulate(std::begin(all_results), std::end(all_results), 0) / static_cast<double>(std::size(all_results));

    std::cout << mean << std::endl;

    std::vector<double> squares(power);

    std::transform(std::begin(all_results), std::end(all_results), std::begin(squares), [mean](int item){return pow(item - mean, 2);});

    std::cout << pow(std::accumulate(std::begin(squares), std::end(squares), 0) / power, 0.5) << std::endl;
//    print_vector<double>(squares);

    return 0;
}
