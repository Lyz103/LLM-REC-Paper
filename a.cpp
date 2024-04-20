#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // Set the seed for random number generation
    std::srand(std::time(nullptr));

    int t = 10000000;
    int ans = 0;
    for (int i = 0; i < t; i++) {
        // Generate a random integer between 1 and 80
        int a = std::rand() % 80 + 1;
        if(a >= 1 && a <= 10){
            int aa = std::rand() % 50 + 1;
            while(aa != a) {
                aa = std::rand() % 50 + 1;
            }
            if(aa >= 1 && aa <= 10) {
                ans ++;
            }
        }
        else if(a > 10 && a <= 50) {
            continue;
        }
        else if(a >= 68){
            continue;
        }
        else {
            int aa = std::rand() % 30 + 50;
            while(aa != a) {
                aa = std::rand() % 30 + 50;
            }
            if(aa >= 51 && aa <= 68) {
                ans ++;
            }
        }
    }
    std::cout << ans << std::endl;
    return 0;
}


