#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

int main(int argc, const char * argv[]) {
    unsigned long long int n;
    n = atoll(argv[1]);
    
    std::ofstream out;
    out.open("output.txt");
    out << 1 << "\t" << 1 << "\n";
    const unsigned long long int norm = 1000000000000000;
    
    unsigned long long int n_1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned long long int n_2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    unsigned long long int tmp[10] = {0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0};
    
    for (unsigned long long int i = 2; i <= n; i++) {
        
        for (int j = 0; j < 10; j++) {
            tmp[j] = n_2[j];
        }
        
        long long int tmp_n;
        n_2[9] = (n_2[9] + n_1[9]);
        tmp_n = n_2[9] / norm;
        n_2[9] = n_2[9] % norm;
        for (int l = 1; l < 10; l++){
            n_2[10 - 1 - l] = n_2[10 - 1 - l] + n_1[10 - 1 - l] + tmp_n;
            tmp_n = n_2[10 - 1 - l] / norm;
            n_2[10 - 1 - l] = n_2[10 - 1 - l] % norm;
        }
        
        for (int h = 0; h < 10; h++) {
            n_1[h] = tmp[h];
        }
        
        out << i << "\t";
        for (int k = 0; k < 10; k++){
            if (n_2[k] != 0){
                if ((k != 0) && n_2[k - 1] == 0 || (k == 0)){
                    out << n_2[k];
                }
                    
                else if ((k != 0) && n_2[k - 1] != 0){
                    std::string tmp_for_print = std::to_string(n_2[k]);
                    while (tmp_for_print.length() != 15) {
                        tmp_for_print = "0" + tmp_for_print;
                    }
                    out << tmp_for_print;
                }
                
            }
        }
        out << "\n";
        
    }

    out.close();
    return 0;
}
