#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <pwd.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>

using namespace std;

// Function declarations
void run_shell(const string& home_dir, string& last_dir, const string& username, const string& hostname);
vector<string> uptoSemicolon(vector<string>& v);
void ls_command(const string& path, const vector<string>& args, const string& home_dir, const string& current_dir);
void fore_back(const vector<string>& args);
void pinfo(const vector<string>& args);
void IO_redirection(const vector<string>& args);
int check_IO(const vector<string>& args);
vector<string> IO_parser(const vector<string>& args);
int execute_cd(const vector<string>& args, const string& home_dir, string& last_dir);
int execute_pwd();
int execute_echo(const vector<string>& args);

#endif