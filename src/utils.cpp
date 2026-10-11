#include "utils.h"

vector<string> uptoSemicolon(vector<string>& v) {
    vector<string> new_v;
    size_t i = 0;
    while (i < v.size()) {
        string token = v[i];
        size_t pos = token.find(';');
        if (pos != string::npos) {
            if (pos > 0) {
                new_v.push_back(token.substr(0, pos));
            }
            v.erase(v.begin(), v.begin() + i + 1);
            if (pos + 1 < token.size()) {
                v.insert(v.begin(), token.substr(pos + 1));
            }
            return new_v;
        }
        else {
            new_v.push_back(token);
            i++;
        }
    }
    v.clear();
    return new_v;
}

vector<string> IO_parser(const vector<string>& v) {
    vector<string> new_v;
    for (size_t i = 0; i < v.size(); i++) {
        int k = 0;
        for (size_t j = 0; j < v[i].length(); j++) {
            if (v[i][j] == '<') {
                if (j - k > 0) new_v.push_back(v[i].substr(k, j - k));
                new_v.push_back("<");
                k = j + 1;
            }
            else if (v[i][j] == '>' && j + 1 < v[i].size() && v[i][j + 1] == '>') {
                if (j - k > 0) new_v.push_back(v[i].substr(k, j - k));
                new_v.push_back(">>");
                k = j + 2;
                j++;
            }
            else if (v[i][j] == '>') {
                if (j - k > 0) new_v.push_back(v[i].substr(k, j - k));
                new_v.push_back(">");
                k = j + 1;
            }
        }
        if (k < static_cast<int>(v[i].size())) {
            string str = v[i].substr(k);
            if (!str.empty()) new_v.push_back(str);
        }
    }
    return new_v;
}

int check_IO(const vector<string>& v) {
    for (const auto& str : v) {
        for (char c : str) {
            if (c == '<' || c == '>') return 1;
        }
    }
    return 0;
}

vector<string> Pipes_parser(vector<string> v){
    vector<string> new_v;
    for(int i=0; i<v.size(); i++){
        int k=0;
        for(int j=0;j<v[i].length();j++){
            if(v[i][j]=='|'){
                if(j-k>0) new_v.push_back(v[i].substr(k,j-k));
                new_v.push_back("|");
                k=j+1;
            }
        }
         if(k < v[i].size()){
            string str = v[i].substr(k);
            if(!str.empty()) new_v.push_back(str);
        }
    }
    return new_v;
}

int check_pipe(vector<string> v){
    for (const auto& str : v) {
        for (char c : str) {
            if (c == '|') return 1;
        }
    }
    return 0;
}

string strip_quotes(const string &s) {
    if(s.size() >= 2) {
        if((s.front()=='"' && s.back()=='"') || (s.front()=='\'' && s.back()=='\'')) {
            return s.substr(1, s.size()-2);
        }
    }
    return s;
}

