#include <iostream>
#include <array>
#include <math.h>
#include <utility>
#include <vector>
#include <fstream>
#include <filesystem>

class Point{
public:
    double x, y;
    Point(): x(0), y(0){}
    Point(double x, double y): x(x), y(y){}
    Point(std::pair<double, double> coords): x(std::get<0>(coords)), y(std::get<1>(coords)){}
};

class Intersections{
    std::vector<Point> intersections;
    bool empty, infinity;
    int size;

public:
    Intersections(int type, std::vector<std::pair<double, double>> points = {{}}){
        // 0 - empty intersection
        // 1 - some intersections
        // 2 - infinity intersections
        size = 0;
        switch (type) {
            case 0:
                empty = true;
                infinity = false;
                break;
            case 1:
                empty = false;
                infinity = false;
                for (auto point : points) {
                    intersections.push_back(Point(point));
                    size++;
                }
            case 2:
                empty = false;
                infinity = true;
            default:
                break;
        }
    }
    
    bool is_empty(){
        return empty;
    }
    
    bool is_infinity(){
        return infinity;
    }
    
    bool is_regular(){
        return !empty && !infinity;
    }
    
    int get_size(){
        return size;
    }
    
    std::vector<Point> get_intersections(){
        return intersections;
    }
};

class Curve{
public:
    double a_11, a_12, a_22, a_13, a_23, a_33;
    
    Curve():a_11(0), a_12(0), a_22(0), a_13(0), a_23(0), a_33(0){}

    Curve(double a_11, double a_12, double a_22, double a_13, double a_23, double a_33): a_11(a_11), a_12(a_12), a_22(a_22), a_13(a_13), a_23(a_23), a_33(a_33){}
    
    Curve(std::array<double, 6> data): a_11(data[0]), a_12(data[1]), a_22(data[2]), a_13(data[3]), a_23(data[4]), a_33(data[5]){}
    
    std::array<double, 3> get_tangent(double x_1, double y_1){
        double k_1, k_2, k_3;
        k_1 = a_11 * x_1 + a_12 * y_1 + a_13;
        k_2 = a_12 * x_1 + a_22 * y_1 + a_23;
        k_3 = a_13 * x_1 + a_23 * y_1 + a_33;
        
        std::array<double, 3> k_s = {k_1, k_2, k_3};
        return k_s;
    }
    
    double get_tangent_angle(double x_1, double y_1){
        std::array<double, 3> k_s = get_tangent(x_1, y_1);
        if (k_s[1] == 0) {
            return M_PI / 2;
        }
        return atan(- k_s[0] / k_s[1]);
    }
    
    double get_tangent_angle(Point point){
        return get_tangent_angle(point.x, point.y);
    }
    
public:
    Intersections get_points_from_line(double k, double m){
        Intersections intersections(0);
        double a = a_11 + 2 * a_12 * k + a_22 * k * k;;
        double b = 2 * a_12 * m + 2 * a_22 * k * m + 2 * a_13 + 2 * a_23 * k;
        double c = a_22 * m * m + 2 * a_23 * m + a_33;
        
        if (a == 0 && b == 0){
            if (c == 0){
                intersections = Intersections(2);
            }
            else {
                intersections = Intersections(0);
            }
        }
        else if (a == 0){
            double x = - c / b;
            double y = k * x + m;
            std::vector<std::pair<double, double>> point;
            point.push_back(std::make_pair(x, y));
            intersections = Intersections(1, point);
        }
        else {
            double D2 = b * b - 4 * a * c;
            if (D2 < 0) {
                intersections = Intersections(0);
            }
            else {
                double x_1 = (-b + pow(D2, 0.5)) / (2 * a);
                double x_2 = (-b - pow(D2, 0.5)) / (2 * a);
                double y_1 = k * x_1 + m;
                double y_2 = k * x_2 + m;
                std::vector<std::pair<double, double>> points;
                points.push_back(std::make_pair(x_1, y_1));
                points.push_back(std::make_pair(x_2, y_2));
                intersections = Intersections(1, points);
            }
        }
        return intersections;
    }
    
    Intersections get_points_from_x(double x){
        Intersections intersections(0);
        if (a_22 == 0 and 2 * a_12 * x + 2 * a_23 == 0){
            if (a_11 * x * x + 2 * a_13 * x + a_33 == 0){
                intersections = Intersections(2);
            }
            else {
                intersections = Intersections(0);
            }
        }
        else if (a_22 == 0){
            double top = - a_11 * x * x - 2 * a_13 * x - a_33;
            double bottom = 2 * a_12 * x + 2 * a_23;
            std::vector<std::pair<double, double>> point;
            point.push_back(std::make_pair(x, top / bottom));
            intersections = Intersections(1, point);
        }
        else {
            double a = a_22;
            double b = 2 * a_23 + 2 * a_12 * x;
            double c = a_11 * x * x + 2 * a_13 * x + a_33;
            double D2 = b * b - 4 * a * c;
            if (D2 < 0) {
                intersections = Intersections(0);
            }
            else{
                double y_1 = (-b + pow(D2, 0.5)) / (2 * a);
                double y_2 = (-b - pow(D2, 0.5)) / (2 * a);
                std::vector<std::pair<double, double>> points;
                points.push_back(std::make_pair(x, y_1));
                points.push_back(std::make_pair(x, y_2));
                intersections = Intersections(1, points);
            }
        }
        return intersections;
    }
};

class Ray{
public:
    double x;
    double y;
    double v_x;
    double v_y;
    
    Ray(): x(0), y(0), v_x(0), v_y(0){}
    
    Ray(double x, double y, double v_x, double v_y): x(x), y(y), v_x(v_x), v_y(v_y){}
    
    Ray(std::array<double, 4> data): x(data[0]), y(data[1]), v_x(data[2]), v_y(data[3]){}

    std::pair<double, double> get_line(){
        double k = v_y / v_x;
        double b = y - k * x;
        return std::make_pair(k, b);
    }
    
    double get_angle(){
        if (v_x == 0) {
            return M_PI / 2;
        }
        return atan(v_y / v_x);
    }
    
    void change_speeds_by_angle(double angle){
        double v = pow(v_x * v_x + v_y * v_y, 0.5);
        v_x = v * cos(angle);
        v_y = v * sin(angle);
    }
    
    void move(double time){
        x += v_x * time;
        y += v_y * time;
    }
    
    std::string to_string(){
        return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(v_x) + " " +  std::to_string(v_y);
    }
};

class Simulator{
    std::vector<Ray> rays;
    std::vector<Curve> curves;
    
    std::vector<std::vector<std::string>> history;
public:
    Simulator(std::vector<std::array<double, 4>> input_rays, std::vector<std::array<double, 6>> input_curves){
        std::vector<std::string> rays_to_history;
        for (auto ray : input_rays) {
            rays.push_back(Ray(ray));
            
        }
        for (auto curve : input_curves) {
            curves.push_back(Curve(curve));
        }
        for (auto ray : rays) {
            rays_to_history.push_back(ray.to_string());
        }
        history.push_back(rays_to_history);
        
    }
    
    std::tuple<Ray, Curve, Point, int, double> get_next_collision(){
        
        
        double min_time = INFINITY;
        double new_time;
        std::tuple<Ray, Curve, Point, int, double> new_collision;
        
        int ray_number = 0;
        for (auto ray : rays) {
            auto[k, b] = ray.get_line();
            for (auto curve : curves) {
                auto intersections = curve.get_points_from_line(k, b);
                
                for (auto point : intersections.get_intersections()) {
//                    std::cout << point.x;
                    new_time = (point.x - ray.x) / ray.v_x;
                    if (new_time > 0 && new_time < min_time){
                        min_time = new_time;
                        new_collision = {ray, curve, point, ray_number, min_time};
                    }
                }
            }
            ray_number++;
        }
        
        return new_collision;
    }
    
    void move_to_next_collision(std::tuple<Ray, Curve, Point, int, double> collision){
//        std::cout << std::get<2>(collision).x << " " << std::get<2>(collision).y << std::endl;
        
//        std::cout << std::get<4>(collision) << std::endl;
        
        // здесь все ломается
        for (auto& ray : rays) {
            ray.move(std::get<4>(collision));
        }
        // здесь все ломается
        
//        for (auto ray : rays) {
//            std::cout << ray.to_string() << std::endl;
//        }
        
        auto curve = std::get<1>(collision);
        auto curve_angle = curve.get_tangent_angle(std::get<2>(collision));
        auto ray_angle = std::get<0>(collision).get_angle();
        auto new_ray_angle = 2 * curve_angle - ray_angle;
        
        rays[std::get<3>(collision)].change_speeds_by_angle(new_ray_angle);
        
        std::vector<std::string> rays_to_history;
        
        for (auto ray : rays) {
//            std::cout << ray.to_string() << std::endl;
            rays_to_history.push_back(ray.to_string());
        }
        history.push_back(rays_to_history);
        
    }
    
    void run(){
        for (int i = 0; i < 100; i++) {
            move_to_next_collision(get_next_collision());
        }
    }
    
    void dump(std::filesystem::path file){
        std::ofstream stream;
        stream.open(file);
        for (auto history_step : history) {
            for (auto ray : history_step) {
//                std::cout << ray << " ";
                stream << ray << " ";
            }
            stream << std::endl;
        }
    }
    
};

int main(int argc, const char * argv[]) {
    std::vector<std::array<double, 4>> rays;
    rays.push_back({0, 0, 1, 0});
    std::vector<std::array<double, 6>> curves;
    curves.push_back({1, 0, 1, 0, 0, -1});
    Simulator simulator(rays, curves);
    simulator.run();
//    std::ofstream stream;
//    stream.open("data.txt");
//    stream.close();
    auto file = std::filesystem::path("data.txt");
    simulator.dump(file);
    return 0;
}
