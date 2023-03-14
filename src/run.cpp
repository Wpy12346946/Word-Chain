#include "run.h"

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
        error = "duplicated parameter detected";
        throw DUPLICATE_PARAM_EXCEPTION;
    }
    flag = true;
}

void setPara(char &ch, int argc, char *argv[], int &idx) {
    idx++;
    if (idx >= argc) {
        error = "required value of parameter -h -t -j not exist";
        throw UNDEFINED_PARAM_EXCEPTION;
    }

    if (!isalpha(argv[idx][0])) {
        error = "wrong format of the value of parameter -h -t -j";
        throw PARAM_FORMAT_EXCEPTION;
    }

    if (strlen(argv[idx]) > 1) {
        error = "wrong format of the value of parameter -h -t -j";
        throw PARAM_FORMAT_EXCEPTION;
    }

    if (ch != '\0') {
        error = "duplicated parameter";
        throw DUPLICATE_PARAM_EXCEPTION;
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
                error = "duplicated filenames detected";
                throw DUPLICATE_FILE_EXCEPTION;
            }
        } else if (arg[0] == '-') {
            // 是参数
            if (arg[1] == '\0' || arg[2] != '\0') {
                error = "undefined parameter detected";
                throw UNDEFINED_PARAM_EXCEPTION;
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
                    error = "undefined parameter detected";
                    throw UNDEFINED_PARAM_EXCEPTION;
            }
        } else {
            error = "undefined parameter detected";
            throw UNDEFINED_PARAM_EXCEPTION;
        }
    }
}

void parseConflict() {
    if (n + w + c == 0) {
        error = "required parameter not exist";
        throw LACK_PARAM_EXCEPTION;
    } else if (n + w + c > 1) {
        error = "parameter conflict";
        throw CONFLICT_PARAM_EXCEPTION;
    }
    if (n && (h || t || j || r)) {
        error = "parameter conflict";
        throw CONFLICT_PARAM_EXCEPTION;
    }
    if (filename == nullptr) {
        error = "required filename not exist";
        throw LACK_FILE_EXCEPTION;
    }
}

char **readFile(ifstream &inputFile, vector<string> &wordList) {
    string str;
    while (getline(inputFile, str)) {
        string word;
        for (char ch: str) {
            if (isalpha(ch)) {
                ch = tolower(ch);
                word += ch;
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
        error = "write file fail";
        throw WRITE_FILE_EXCEPTION;
    }
    for (int i = 0; i < len; i++) {
        outputFile << results[i] << endl;
    }
    outputFile.close();
}

void debug() {
    ifstream inputFile("E:\\coding\\C++\\VisualStudio\\Word-Chain\\input.txt");
    if (!inputFile) {
        cerr << "can not to open input.txt" << endl;
        return;
    }

    vector<string> wordList;
    char **words = readFile(inputFile, wordList);
    int len = wordList.size();

    char **results = new char *[100];
//    int resLen = gen_chains_all(words, len, results);
    int resLen = gen_chain_word(words, len, results, '\0', '\0', '\0', true);
    cout << resLen << endl;
    for (int i = 0; i < resLen; i++) {
        std::cout << results[i] << std::endl;
    }
}

int run(int argc, char *argv[]) {
    debug();
    return 0;
    char **words = nullptr;
    char **results = new char *[20000];
    try {
        parseArgs(argc, argv);
        parseConflict();
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "read file " << filename << " fail" << endl;
            throw READ_FILE_EXCEPTION;
        }
        vector<string> wordList;
        words = readFile(inputFile, wordList);
        int len = wordList.size();

        int resLen;//results长度
        if (n) {
            resLen = gen_chains_all(words, len, results);
        } else if (w) {
            resLen = gen_chain_word(words, len, results, h, t, j, r);
        } else {
            resLen = gen_chain_char(words, len, results, h, t, j, r);
        }
        if (resLen < 0) {
            throw resLen;
        }
        // debug用，直接在控制台输出
        for (int i = 0; i < resLen; i++) {
            std::cout << results[i] << std::endl;
        }
        writeFile(results, resLen);
    } catch (int err) {
        if (err == WORD_CYCLE_EXCEPTION) {
            error = "chain circle detected";
        } else if (err == TOO_LONG_EXCEPTION) {
            error = "too long chain";
        }
        cerr << "exception found: " << error << endl;
        return err;
    }

    delete[](words);
    delete[](results);
    return 0;
}
