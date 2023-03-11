#include <iostream>
#include <random>
#include <vector>
#include <fstream>

class Distribution{
public:
    std::vector<double> bins_borders;
    double bin_number = 0;
    std::vector<std::vector<double>> hist_data;
    std::vector<std::vector<double>> bins;
    Distribution(std::vector<double>& results, long int len, double begin, double end, double step){
        double cur = begin;

        while (cur <= end){
            bins_borders.push_back(cur);
            cur += step;
            bin_number++;
        }
        
        for (int k = 0; k < bin_number - 1; k++) {
            bins.push_back(std::vector<double>());
        }
        
        double elem;
        for (int i = 0; i < len; i++) {
            elem = results[i];
            for (int j = 0; j < bin_number - 1; j++) {
                if (elem >= bins_borders[j] && elem <= bins_borders[j + 1]) {
                    bins[j].push_back(elem);
                }
            }
        }
    }
    
    void hist(){
        for (int i = 0; i < std::size(bins); i++) {
            hist_data.push_back(std::vector<double>());
            hist_data[i].push_back((bins_borders[i] + bins_borders[i + 1])/2);
            hist_data[i].push_back(std::size(bins[i]));
        }
    }
};

int main(int argc, const char * argv[]) {
    std::ofstream fout;
    fout.open("output.txt");
    double m, s, N;
    std::cin >> m;
    std::cin >> s;
    std::cin >> N;
    std::mt19937 gen(564839);
    std::normal_distribution<> distrib(m, s);
    auto f = std::bind(distrib, gen);
    
    std::vector<double> results;

    float min = 1000000;
    float max = -1000000;
    
    for (int i = 0; i < N; i++){
        float elem = f();
        if (elem < min){
            min = elem;
        }
        if (elem > max){
            max = elem;
        }
        results.push_back(elem);
    }
    
    auto dist = Distribution(results, std::size(results), min - 0.1, max + 0.1, 0.1);
//    for (int i = 0; i < std::size(dist.bins); i++) {
//        for (int j = 0; j < std::size(dist.bins[i]); j++) {
//            std::cout << dist.bins[i][j] << " ";
//        }
//        std::cout << std::endl << std::endl;
//    }
//
    dist.hist();
//
//    for (int i = 0; i < std::size(dist.hist_data); i++) {
//        for (int j = 0; j < std::size(dist.hist_data[i]); j++) {
//            std::cout << dist.hist_data[i][j] << " ";
//        }
//        std::cout << std::endl << std::endl;
//    }
    
    for (int i = 0; i < std::size(dist.hist_data); i++) {
        for (int j = 0; j < std::size(dist.hist_data[i]); j++) {
            fout << dist.hist_data[i][j] << " ";
        }
        fout << std::endl;
    }
    fout.close();
    return 0;
}
