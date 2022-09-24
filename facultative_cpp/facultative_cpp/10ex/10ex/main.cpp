#include <iostream>
#include <string>
#include <vector>

int main(int argc, const char * argv[]) {
    unsigned int n;
    n = atoi(argv[1]);
    std::vector<double> ans(600);
    for (double i = 1; i <= n; i++) {
        ans[i - 1] = 1 / i;
    }
    
    for (int j = 0; j < n; j++) {
        std::cout << std::scientific << ans[j] << "\n";
    }
    
    return 0;
}
