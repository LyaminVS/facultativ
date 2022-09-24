#include <iostream>
#include <fstream>

int main(int argc, const char * argv[]) {
    int n;
    n = atoi(argv[1]);
    std::ofstream out;
    out.open("output.txt");
    for (int i = 1; i <= n; i++) {
        out << i << "\n";
    }

    out.close();
    return 0;
    
}
