#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int main(int argc, const char * argv[]) {
    
    std::ofstream out;
    out.open("output.bin");
    unsigned int n;
    n = atoi(argv[1]);
    std::vector<double> ans(600);
    for (double i = 1; i <= n; i++) {
        ans[i - 1] = 1 / i;
    }
    
    for (int j = 0; j < n; j++) {
        out << std::scientific << ans[j] << "\n";
    }
    
    return 0;
}
