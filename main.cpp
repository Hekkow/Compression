#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <unordered_map>
#include <vector>

using namespace std;

char keyDivider = '|';
char equals = '=';
char comma = ',';

map<int, tuple<int, char>> compress(string fileName);
void decompress(string fileName);
string printMap();
string followPath(int index);
string fileNameWithoutExtension(string fileName);
int isInMap(string str);
int main() {
    // decompress("compressed.ctxt");
    compress("test.txt");
    decompress("test.compro");
    return 0;
}
map<int, tuple<int, char>> m;
unordered_map<string, int> mStrings;
map<int, tuple<int, char>> compress(string fileName) {
    ifstream f(fileName);
    if (!f.is_open()) {
        cout << "Error opening file" << endl;
        return map<int, tuple<int, char>>();
    }
    char ch;
    string str;
    int indexInMap = 0;
    int previousIndex = 0;
    int index = 1;
    bool doneString = false;

    auto startTime = chrono::system_clock::now();

    while (f.get(ch)) {
        str += ch;
        previousIndex = indexInMap;
        indexInMap = isInMap(str);
        doneString = false;
        if (indexInMap == 0) {
            m.insert({index, make_tuple(previousIndex, ch)});
            mStrings.insert({str, index});
            str = "";
            index++;
            doneString = true;
        }
        // cout << ch;
    }
    if (!doneString) m.insert({index, make_tuple(previousIndex, ch)});
    auto endTime = chrono::system_clock::now();
    chrono::duration<double> elapsedTime = endTime-startTime;
    cout << "Total time: " << elapsedTime.count() << endl;
    // printMap(m);
    f.close();


    ofstream outF(fileNameWithoutExtension(fileName) + ".compro");
    outF << printMap();
    return m;
}
string fileNameWithoutExtension(string fileName) {
    string newFileName = fileName;
    size_t lastPeriodPosition = fileName.find_last_of('.');
    if (lastPeriodPosition != string::npos) newFileName = fileName.substr(0, lastPeriodPosition);
    return newFileName;
}
int isInMap(string str) {
    auto it = mStrings.find(str);
    if (it != mStrings.end()) return it->second;
    return 0;
}
string followPath(int index) {
    string str;
    tuple<int, char> t = m.at(index);
    str += get<1>(t);
    while (get<0>(t) != 0) {
        t = m.at(get<0>(t));
        str += get<1>(t);
    }
    ranges::reverse(str);
    return str;
}
string printMap() {
    map<int, tuple<int, char>>::iterator it = m.begin();
    cout << endl;
    string str;

    while (it != m.end()) {
        str += std::to_string(it->first) + equals +
        std::to_string(get<0>(it->second)) + comma +
        get<1>(it->second);
        ++it;
    }
    return str;
}

void decompress(string fileName) {
    m = map<int, tuple<int, char>>();
    ifstream f(fileName);
    if (!f.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }
    ofstream outF(fileNameWithoutExtension(fileName) + ".txt");
    char ch;
    string str;
    string index;
    string pathIndex;
    int getting = 0; // 0 for index, 1 for path index, 2 for character
    while (f.get(ch)) {
        if (getting == 0) {
            if (ch == equals) {
                getting = 1;
            }
            else {
                index += ch;
            }
        }
        else if (getting == 1) {
            if (ch == comma) {
                getting = 2;
            }
            else {
                pathIndex += ch;
            }
        }
        else {
            int indexInt = stoi(index);
            m.insert({indexInt, make_tuple(stoi(pathIndex), ch)});
            outF << followPath(indexInt);
            getting = 0;
            pathIndex = "";
            index = "";
        }
        // cout << ch;
    }
    // unordered_map<int, tuple<int, char>>::iterator it = m.begin();
    // string decompressed;
    // while (it != m.end()) {
    //     decompressed += followPath(it->first);
    //     ++it;
    // }
    // cout << decompressed;
}