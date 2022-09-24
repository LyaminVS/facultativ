#include <iostream>
#include <fstream>



int main(int argc, const char * argv[]) {

    std::ofstream out;
    out.open("output.txt");
    int n;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        out << i << "\n";
    }

    out.close();

    return 0;
}

