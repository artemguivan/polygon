#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono> 
#include <sstream> 
#include <cassert>

using namespace std;


void gnomeSort(std::vector<int>& vec) {
    int n = vec.size(); 
    int index = 0; 
  
    while (index < n) { 
        if (index == 0) 
            index++; 
        if (vec[index] >= vec[index - 1]) 
            index++; 
        else { 
            std::swap(vec[index], vec[index - 1]); 
            index--; 
        } 
    } 
}
// Best case O(n) - отсортированный вектор
// Worst case O(n^2) - средний случай
// Worst case O(n^2) - худший случай(когда массив отсортирован inverse)
// Memorry O(1) - на месте


void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
// Best case O(n log(n)) - отсортированный вектор
// Worst case O(n log(n)) - средний случай
// Worst case O(n log(n)) - худший случай
// Memorry O(n)

void pigeonholeSort(std::vector<int>& vec) {
    int min_val = *std::min_element(vec.begin(), vec.end());
    int max_val = *std::max_element(vec.begin(), vec.end());
    int range = max_val - min_val + 1;

    std::vector<int> holes(range, 0);
    for (int x : vec) {
        holes[x - min_val]++;
    }

    int idx = 0;
    for (int i = 0; i < range; i++) {
        while (holes[i]-- > 0) {
            vec[idx++] = i + min_val;
        }
    }
}
// Best case O(n + k) - отсортированный вектор
// Worst case O(n + k) - средний случай
// Worst case O(n + k) - худший случай
// Memorry O(k) - диапазон значений

vector<string> getVectorPaths() {
    vector<string> paths;
    for (int i = 1000; i <= 8000; i += 1000) {
        paths.push_back("test" + to_string(i) + "N.txt");
    }
    return paths;
}

vector<int> readVectorFromFile(string path) {
    vector<int> vec;
    fstream file(path);
    if (file.is_open()) {
        int el;
        while (file >> el) { 
            vec.push_back(el);
        }
        file.close();
    } else {
        cerr << "Error: Could not open file " << path << endl;
    }
    return vec;
}

void addTextToFile(string path, string text, string mode) {
    fstream file;

    if (mode == "in") {
        file.open(path, std::ios::out | std::ios::in | std::ios::trunc);
    } else if (mode == "app") {
        file.open(path, std::ios::out | std::ios::app);
    } else {
        cerr << "YOU MUST USE ONLY IN OR APP MODE" << endl;
        return;  
    }

    if (file.is_open()) {
        file << text << endl; 
        file.close();
    } else {
        cerr << "ERROR!!!!! " << path << " in AddTextToFile function." << endl;
    }
}


void writeResultsToFile(string algorithm) {
    string outpath;
    vector<string> paths = getVectorPaths();
    for (string path : paths) {
        cout << "Processing file: " << path << endl;

        vector<int> vec = readVectorFromFile(path);

        if (vec.empty()) {
            cerr << "ERROR: FILE " << path << endl;
            continue;
        }

        auto start_time = std::chrono::high_resolution_clock::now();  
        if (algorithm == "pigeonholeSort") {
            outpath = "pigeonholeSort.txt";
            pigeonholeSort(vec);
        } else if (algorithm == "gnomeSort") {
            outpath = "gnomeSort.txt";
            gnomeSort(vec);
        } else if (algorithm == "mergeSort") {
            outpath = "mergeSort.txt";
            mergeSort(vec, 0, vec.size() - 1);
        } else {
            cerr << "PLEASE, CHECK NAME OF THE FILE!!!!" << endl;
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();  
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        string text = path + " " + to_string(duration.count());
        addTextToFile(outpath, text, "app");
    }
        
        
}

bool isSorted(const vector<int>& vec) {
    for (int i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) {
            return false;
        }
    }
    return true;
}

void testPigeonholeSort() {
    cout << "Testing Pigeonhole Sort" << endl;

    // лучший случай - вектор отсортированный
    std::vector<int> bestCase = {1, 2, 3, 4, 5};
    pigeonholeSort(bestCase);
    assert(isSorted(bestCase));

    // Средний случай -  вектор с произвольным порядком элементов
    std::vector<int> averageCase = {4, 2, 5, 1, 3};
    pigeonholeSort(averageCase);
    assert(isSorted(averageCase));

    // Худший случай -  вектор с большим диапазоном значений
    std::vector<int> worstCase = {1, 1000, 1000000};
    pigeonholeSort(worstCase);
    assert(isSorted(worstCase));

    cout << "Pigeonhole Sort passed all tests!" << endl;
}

void testGnomeSort() {
    cout << "Testing Gnome Sort" << endl;

    // Лучший случай - уже отсортированный массив
    vector<int> bestCase = {1, 2, 3, 4, 5};
    gnomeSort(bestCase);
    assert(isSorted(bestCase));  

    // Средний случай - массив с произвольным порядком элементов
    vector<int> averageCase = {4, 2, 5, 1, 3};
    gnomeSort(averageCase);
    assert(isSorted(averageCase));

    // Худший случай - массив, отсортированный в обратном порядке
    vector<int> worstCase = {5, 4, 3, 2, 1};
    gnomeSort(worstCase);
    assert(isSorted(worstCase));
    cout << "Gnome Sort passed all tests!" << endl;
}

void testMergeSort() {
    cout << "Testing Merge Sort..." << endl;

    // Лучший случай: уже отсортированный массив
    vector<int> bestCase = {1, 2, 3, 4, 5};
    mergeSort(bestCase, 0, bestCase.size() - 1);
    assert(isSorted(bestCase));

    // Средний случай: массив с произвольным порядком элементов
    vector<int> averageCase = {4, 2, 5, 1, 3};
    mergeSort(averageCase, 0, averageCase.size() - 1);
    assert(isSorted(averageCase));

    // Худший случай: массив, отсортированный в обратном порядке
    vector<int> worstCase = {5, 4, 3, 2, 1};
    mergeSort(worstCase, 0, worstCase.size() - 1);
    assert(isSorted(worstCase));

    cout << "Merge Sort passed all tests!\n" << std::endl;
}

int main() {

    // for (int i = 0; i < 50; i++){
    //     writeResultsToFile("pigeonholeSort");
    //     writeResultsToFile("gnomeSort");
    //     writeResultsToFile("mergeSort");
    // }

    testGnomeSort();
    testMergeSort();
    testPigeonholeSort();

    return 0;
}
