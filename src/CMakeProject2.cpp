#include"CMakeProject2.h"

using namespace std;


char *filename = nullptr;
bool n, w, c, r;
char h, t, j;
string error;//保存错误信息

bool isFile(char *str) {
    int len = strlen(str);
    return len >= 4 && strcmp(str + len - 4, ".txt") == 0;
}

void setPara(bool &flag) {
    if (flag) {
        //TODO 重复参数
        cout << "存在重复参数" << endl;
        throw -1;
    }
    flag = true;
}

void setPara(char &ch, int argc, char *argv[], int &idx) {
    idx++;
    if (idx >= argc) {
        //TODO -h-t-j缺少参数值
        cout << "缺少参数值" << endl;
        throw -1;
    }

    if (!isalpha(argv[idx][0])) {
        cout << "缺少参数值或参数值错误" << endl;
        throw -1;
    }

    if (strlen(argv[idx]) > 1) {
        //TODO 参数值过长
        cout << "参数值过长" << endl;
        throw -1;
    }

    if (ch != '\0') {
        //TODO 重复参数
        cout << "存在重复参数" << endl;
        throw -1;
    }
    ch = (char) tolower(argv[idx][0]);
}

void parseArgs(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (isFile(arg)) {
            if (filename == nullptr) {
                filename = arg;
            } else {
                //TODO 多个文件名
                cout << "检测到多个文件" << endl;
                throw -1;
            }
        } else if (arg[0] == '-') {
            // 是参数
            if (arg[1] == '\0' || arg[2] != '\0') {
                //TODO 未知参数格式
                cout << "未知的参数" << endl;
                throw -1;
            }
            switch (arg[1]) {
                case 'n':
                    setPara(n);
                    break;
                case 'w':
                    setPara(w);
                    break;
                case 'c':
                    setPara(c);
                    break;
                case 'h':
                    setPara(h, argc, argv, i);
                    break;
                case 't':
                    setPara(t, argc, argv, i);
                    break;
                case 'j':
                    setPara(j, argc, argv, i);
                    break;
                case 'r':
                    setPara(r);
                    break;
                default:
                    //TODO 未知参数格式
                    cout << "未知的参数" << endl;
                    throw -1;
            }
        } else {
            //TODO 未知参数
            cout << "未知参数" << endl;
            throw -1;
        }
    }
}

void parseConflict() {
    if (n + w + c == 0) {
        cout << "未指定参数" << endl;//TODO
        throw -1;
    } else if (n + w + c > 1) {
        cout << "指定了多个必选参数" << endl;//TODO
        throw -1;
    }
    if (n && (h || t || j || r)) {
        cout << "参数n和可选参数混用" << endl;//TODO
        throw -1;
    }
    if (filename == nullptr) {
        cout << "缺少文件名或文件名非法" << endl;//TODO
        throw -1;
    }
}

char **readFile(ifstream &inputFile, vector <string> &wordList) {
    string str;
    while (getline(inputFile, str)) {
        string word;
        for (char c: str) {
            if (isalpha(c)) {
                c = tolower(c);
                word += c;
            } else if (!word.empty()) {
                wordList.push_back(word);
                word.clear();
            }
        }
        if (!word.empty()) {
            wordList.push_back(word);
            word.clear();
        }
    }

    char **words = new char *[wordList.size()];
    for (int i = 0; i < wordList.size(); i++) {
        words[i] = const_cast<char *>(wordList[i].c_str());
    }
    inputFile.close();

    return words;
}

void writeFile(char **results, int len) {
    ofstream outputFile("./solution.txt");
    if (!outputFile) {
        cerr << "写入文件失败" << endl;
        throw -1;
    }
    for (int i = 0; i < len; i++) {
        outputFile << results[i] << endl;
    }
    outputFile.close();
}

void debug() {
    ifstream inputFile("D:\\college\\3-2\\software-engineering\\Word-Chain\\src\\input.txt");
    if (!inputFile) {
        cerr << "can not to open input.txt" << endl;
        return;
    }

    vector <string> wordList;
    char **words = readFile(inputFile, wordList);
    int len = wordList.size();

    char **results = new char *[100];
    int resLen = gen_chain_word(words, len, results, 'a', 'a', '\0', true);
    for (int i = 0; i < resLen; i++) {
        std::cout << results[i] << std::endl;
    }
}

int main(int argc, char *argv[]) {
//    debug();
//    return 0;
    char **words = nullptr;
    char **results = new char *[100000];
    try {
        parseArgs(argc, argv);
        parseConflict();
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "文件不存在" << endl;
            throw -1;
        }
        vector<string> wordList;
        words = readFile(inputFile, wordList);
        int len = wordList.size();

        Core core;
        int resLen;//results长度
        if (n) {
            resLen = core.gen_chains_all(words, len, results);
        } else if (w) {
            resLen = core.gen_chain_word(words, len, results, h, t, j, r);
        } else {
            resLen = core.gen_chain_char(words, len, results, h, t, j, r);
        }
        // debug用，直接在控制台输出
        for (int i = 0; i < resLen; i++) {
            std::cout << results[i] << std::endl;
        }
        writeFile(results, resLen);
    } catch (int err) {
        cout << "excception found" << endl;
        return -1;
    }

    delete[](words);
    delete[](results);

    return 0;
}
