#include <vector> 
#include <iostream>
#include <cmath>

using namespace std;

bool isPrime(int num) { 
    for (int i = 2; i <= static_cast<int>(sqrt(num)); i++) {
        if (num % i == 0 | num < 2) {
            return false;
        }
    }
    return true;
}

int main() {
    vector<int> test = {2, 3, 4, 5, 5, 6, 7 ,8 ,9, 10, 13, 56, 52, 47};
    for (int num : test) {
        if (isPrime(num)) {
            cout << num << endl;
        }
    }
    return 0;
}
