#include <iostream>
#include <string>
#include <array>

int main(int argc, const char * argv[]) {
    unsigned long long int n;
    n = atoll(argv[1]);
    std::array<double, 600> ans;
    for (double i = 1; i <= n; i++) {
        ans[i - 1] = 1 / i;
    }
    
    for (int j = 0; j < n; j++) {
        std::cout << std::scientific << ans[j] << "\n";
    }
    
    return 0;
}
