#include <iostream>
#include <fstream>



int main(int argc, const char * argv[]) {

    std::ofstream out;
    out.open("output.txt");

    for (int i = 1; i <= 30; i++) {
        out << i << "\n";
    }

    out.close();

    return 0;
}


