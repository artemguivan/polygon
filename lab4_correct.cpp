#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

// перебираем все пождмножества из данных множеств
void getComb(const vector<set<int>>& sets, int idx, vector<set<int>> currComb, vector<vector<set<int>>>& allComb) {
    if (idx == sets.size()) {
        allComb.push_back(currComb);
        return;
    }

    getComb(sets, idx + 1, currComb, allComb);

    currComb.push_back(sets[idx]);
    getComb(sets, idx + 1, currComb, allComb);
}

// находим минимальное подмножество 
vector<set<int>> findMinUniverse(const vector<set<int>>& sets, const set<int>& universe) {
    vector<vector<set<int>>> allCombs;
    vector<set<int>> currComb;

    getComb(sets, 0, currComb, allCombs);

    vector<set<int>> minCover;  
    int minSize = sets.size() + 1; 

    for (const auto& combination : allCombs) {
        set<int> unionSet;  
        for (const auto& s : combination) {
            unionSet.insert(s.begin(), s.end());  
        }

        bool isCover = true;
        for (int elem : universe) {
            if (unionSet.find(elem) == unionSet.end()) {
                isCover = false;
                break;
            }
        }

        if (isCover && combination.size() < minSize) {
            minSize = combination.size();
            minCover = combination;
        }
    }

    return minCover;
}
// функция для чтения данных с файла
void readTestsFromFile(const string& filename, vector<pair<vector<set<int>>, set<int>>>& tests) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Can't open a file!" << filename << endl;
        return;
    }

    string line;
    vector<set<int>> currSets;
    set<int> currUniverse;
    bool readingSets = false, readingUniverse = false;

    while (getline(infile, line)) {
        if (line == "test:") { 
            if (!currSets.empty() || !currUniverse.empty()) {
                tests.emplace_back(currSets, currUniverse);
                currSets.clear();
                currUniverse.clear();
            }
            readingSets = false;
            readingUniverse = false;
        } else if (line == "sets:") {
            readingSets = true;
            readingUniverse = false;
        } else if (line == "universe:") {
            readingSets = false;
            readingUniverse = true;
        } else if (readingSets) {
            set<int> currSet;
            stringstream ss(line);
            int num;
            while (ss >> num) {
                currSet.insert(num);
            }
            currSets.push_back(currSet);
        } else if (readingUniverse) {
            stringstream ss(line);
            int num;
            while (ss >> num) {
                currUniverse.insert(num);
            }
        }
    }

    if (!currSets.empty() || !currUniverse.empty()) {
        tests.emplace_back(currSets, currUniverse);
    }

    infile.close();
}
// запускаем тесты из файла tests
void runTestsFromFile(const string& filename) {
    vector<pair<vector<set<int>>, set<int>>> tests;
    // читаем с файла данные
    readTestsFromFile(filename, tests);

    int testNumber = 1;
    for (const auto& test : tests) {
        const auto& sets = test.first;
        const auto& universe = test.second;

        cout << "test " << testNumber++ << ":\n";
        cout << "Sets: ";
        for (const auto& s : sets) {
            cout << "{";
            for (int elem : s) {
                cout << elem << " ";
            }
            cout << "} ";
        }
        cout << "\nUniverse: {";
        for (int elem : universe) {
            cout << elem << " ";
        }
        cout << "}\n";

        auto start = chrono::high_resolution_clock::now();
        vector<set<int>> minCover = findMinUniverse(sets, universe);
        auto end = chrono::high_resolution_clock::now();

        cout << "answer: ";
        for (const auto& s : minCover) {
            cout << "{";
            for (int elem : s) {
                cout << elem << " ";
            }
            cout << "} ";
        }
        cout << endl;

        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        std::ofstream out;           
        out.open("hello.txt", std::ios::app); 
        if (out.is_open())
        {
            out << testNumber + 5 - 2 << " " << duration.count() << std::endl;
        }
        out.close(); 


    std::cout << "Ok!" << std::endl;
        cout << "time: " << duration.count() << " microseconds\n";
        cout << "---------------------------------\n";
    }
}

int main() {

    runTestsFromFile("tests.txt");  

    return 0;
}

// O(2^N*Universe) - асимптотика
