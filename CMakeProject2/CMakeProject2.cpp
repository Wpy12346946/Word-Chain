#include"CMakeProject2.h"

int gen_chain(char* words[]) {
    Graph graph;
    graph.init();
    graph.makeGraph(words, true, false);
    return 1;
}

int main() {

    ifstream inputFile("D:\\college\\3-2\\c++Project\\input.txt");
    if (!inputFile) {
        cerr << "can not to open input.txt" << endl;
        return 1;
    }

    vector<string> wordList;
    string str;
    while (getline(inputFile, str)) {
        string word;
        for (char c : str) {
            if (isalpha(c)) {
                c = tolower(c);
                word += c;
            }
            else if (!word.empty()) {
                wordList.push_back(word);
                word.clear();
            }
        }
        if (!word.empty()) {
            wordList.push_back(word);
            word.clear();
        }
    }

    char** words = new char* [wordList.size()];
    for (int i = 0; i < wordList.size(); i++) {
        words[i] = const_cast<char*>(wordList[i].c_str());
    }

    gen_chain(words);

    return 0;
}
