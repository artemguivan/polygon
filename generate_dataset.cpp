#include<iostream>
#include<ostream>
#include<fstream>
#include<string>
#include<chrono>
#include<vector>
#include<filesystem>
#include<random>
#include<limits>

using namespace std;

// you shouldn't use open you can use just fstream(path) 

// ifstream or ofstream or fstream

struct Point 
{
    Point(double x, double y): x{x}, y{y} {}
    double x;
    double y;
};

// struct PointThis
// {
//     Point(double x, double y) {
//         this->x = x;
//         this->y = y;
//     }
// };

void readDataFromFile(string path) {
    // ios::in ofstream object; object.open(file)
    fstream stream;
    stream.open(path, ios::in); 
    if (stream.is_open()) {
         cout << "file is opened";
         stream << "some text" << endl;
    } else {
        cerr << "There are some mistakes";
    }
    stream.close();
    cout << "File has been written" << endl;
}

void outputDataFromFile(string path) {
    fstream out;
    out.open(path, ios::out);
}

void addDataToFromFile(string path) {
    fstream stream;
    stream.open(path, ios::app);
    if (stream.is_open()) { 
        auto time_now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        tm* tm_now = localtime(&time_now);
        stream << "add some text " << put_time(tm_now, "%Y-%m-%d %H:%M:%S") << endl;

    } else {
        cerr << "Some mistake";
    }
    stream.close();
    cout << "File has been written" << endl;
}

void allFromFile(string path) {
    fstream stream;
    stream.open(path, ios::out | ios::app | ios::in);
}

void values(string path){
    vector<Point> points{Point(0, 0), Point{4, 5}, Point(-5, 7)};
    fstream stream(path);
    if (filesystem::exists(path)) {
        if (stream.is_open()) 
        {
            for (const Point& point: points) 
            {
                stream << point.x << " " << point.y << endl;
            }
        } else {
            cerr << "Error! Check name of the file!" << endl;
        }
    } else {
        cout << "File does not exists" << endl;
    }
    
    stream.close();
    cout << "Function values has been worked" << endl;
}

class DataGenerator {
private:
    string path;
public:
    DataGenerator(const string& filePath) : path(filePath) {}

    void generateVector(int N) {
        ofstream file(path);
        if (file.is_open()) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> distrib(-10000, 10000);

            for (int i = 0; i < N; i++) {
                file << distrib(gen) << " ";
            }

            cout << "Successfully wrote " << N << " random numbers to " << path << endl;
            file.close();
        } else {
            cerr << "Error: Could not open file " << path << " for writing!" << endl;
        }
    }
};
    

int main() {
    vector<int> thousands;
    vector<string> path_thousands;

    for (int i = 1000; i < 8001; i+=1000) {
        thousands.push_back(i);
    }

    for (int thousand : thousands) {
        path_thousands.push_back("test" + to_string(thousand) + "N.txt");
    }

    
    for (int i = 0; i < thousands.size(); i++) {
        DataGenerator generator(path_thousands[i]);
        generator.generateVector(thousands[i]);
    } 

}


