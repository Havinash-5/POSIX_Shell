#ifndef COMMANDS_H
#define COMMANDS_H

#include "shell.h"

int execute_cd(const vector<string>& args, const string& home_dir, string& last_dir);
int execute_pwd();
int execute_echo(const vector<string>& args);
void ls_command(const string& path, const vector<string>& args, const string& home_dir, const string& current_dir);
void pinfo(const vector<string>& args);
void fore_back(const vector<string>& args);
void IO_redirection(const vector<string>& args);
void Pipes_implement(vector<string> v);
void Pipe_redirection(vector<string> v);
int search(string path,string target);

#endif
