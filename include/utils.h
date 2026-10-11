#ifndef UTILS_H
#define UTILS_H

#include "shell.h"

vector<string> uptoSemicolon(vector<string>& v);
vector<string> IO_parser(const vector<string>& v);
int check_IO(const vector<string>& v);
vector<string> Pipes_parser(vector<string> v);
int check_pipe(vector<string> v);
string strip_quotes(const string& s);

#endif