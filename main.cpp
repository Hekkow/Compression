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
char strDivider = '>';
unordered_map<int, tuple<int, char>> compress(string fileName);
void decompress(string fileName);
void decompress();
string printMap();
string followPath(int index);
int isInMap(string str);
int main() {
    // decompress("compressed.ctxt");
    compress("test.txt");
    // decompress();
    return 0;
}
unordered_map<int, tuple<int, char>> m;
unordered_map<string, int> mStrings;
unordered_map<int, tuple<int, char>> compress(string fileName) {
    ifstream f(fileName);
    if (!f.is_open()) {
        cout << "Error opening file" << endl;
        return unordered_map<int, tuple<int, char>>();
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
    ofstream outF("testCompressed.ctxt");
    outF << printMap();
    return m;
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
    unordered_map<int, tuple<int, char>>::iterator it = m.begin();
    cout << endl;
    string str;
    while (it != m.end()) {
        str += std::to_string(it->first) + "=" +
        std::to_string(get<0>(it->second)) + "," +
        get<1>(it->second);
        ++it;
    }
    return str;
}

void decompress() {
    unordered_map<int, tuple<int, char>>::iterator it = m.begin();
    string decompressed;
    while (it != m.end()) {
        decompressed += followPath(it->first);
        ++it;
    }
    // cout << decompressed;
}