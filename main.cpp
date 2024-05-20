#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <atomic>

 
int64_t count_num_points_in_circle(int64_t all_points) {
   
    std::random_device rd;  
    std::mt19937 gen(rd());  
    std::uniform_real_distribution<> dis( -1.0, 1.0); 
    
    int64_t in_circle = 0;
    for (int64_t i = 0; i < all_points; ++i) {
        double x = dis(gen);
        double y = dis(gen);
        if (x * x + y * y <= 1.0) {  
            in_circle++;
        }
    }

    return in_circle;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {  
        std::cerr << "Usage: " << argv[0] << " <number of threads>";
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    const int64_t all_points = 1000000; 

    int64_t points_per_thread = all_points / num_threads;

    std::atomic<int64_t>  all_in_circle(0);

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&]() {
            int in_circle = count_num_points_in_circle(points_per_thread);
            all_in_circle += in_circle;
            });
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (auto& t : threads) {  
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end - start;

    double pi = 4.0 * all_in_circle / all_points; 
    std::cout << "Pi: " << pi << std::endl;
    std::cout << "Time : " << elapsed_seconds.count() << " seconds " << std::endl;

    return 0;
}

