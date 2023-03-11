#include <iostream>
#include <random>

int main(int argc, const char * argv[]) {
    int N, K;
    std::cin >> K;
    std::cin >> N;
    std::mt19937 gen(2);
    std::uniform_int_distribution<> distrib(1, K);
    auto f = std::bind(distrib, gen);

    for (int i = 0; i < N; i++)
        std::cout << f() << " ";
    
    std::cout << "\n";
    return 0;
}
