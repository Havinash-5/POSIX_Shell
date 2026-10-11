#include "commands.h"
#include "utils.h"
#include<iostream>

int execute_cd(const vector<string>& args, const string& home_dir, string& last_dir) {
    if (args.size() > 2) {
        cout << "cd: Invalid Arguments" << endl;
        return 1;
    }

    string target_dir;
    char temp_path[256];
    getcwd(temp_path, sizeof(temp_path));
    string current_dir(temp_path);

    if (args.size() == 1 || args[1] == ".") {
        target_dir = current_dir;
    }
    else if (args[1] == "..") {
        size_t i = current_dir.find_last_of("/");
        target_dir = (i == 0) ? "/" : current_dir.substr(0, i);
    }
    else if (args[1] == "~") {
        target_dir = home_dir;
    }
    else if (args[1] == "-") {
        target_dir = last_dir;
    }
    else {
        target_dir = args[1];
        if (target_dir[0] != '/') {
            target_dir = current_dir + "/" + args[1];
        }
    }

    if (chdir(target_dir.c_str()) != 0) {
        perror("cd");
        return 1;
    }
    last_dir = current_dir;
    return 0;
}

int execute_pwd() {
    char pwd_path[256];
    if (getcwd(pwd_path, sizeof(pwd_path)) == NULL) {
        perror("pwd");
        return 1;
    }
    cout << pwd_path << endl;
    return 0;
}

int execute_echo(const vector<string>& args) {
    for (size_t i = 1; i < args.size(); i++) {
        cout << args[i] << " ";
    }
    cout << endl;
    return 0;
}

void ls_command(const string& path, const vector<string>& args, const string& home_dir, const string& current_dir) {
    size_t n = args.size();
    DIR* dir = nullptr;
    string target_path = path;

    if (n == 1 || (n == 2 && args[1] == ".")) {
        dir = opendir(path.c_str());
        if (!dir) { perror("ls"); return; }
        dirent* file;
        while ((file = readdir(dir)) != NULL) {
            if (file->d_name[0] != '.') cout << file->d_name << endl;
        }
    }
    else if (n == 2 && args[1] == "-a") {
        dir = opendir(path.c_str());
        if (!dir) { perror("ls"); return; }
        dirent* file;
        while ((file = readdir(dir)) != NULL) {
            cout << file->d_name << endl;
        }
    }
    else if ((n == 2 && (args[1] == "-l" || args[1] == "-al" || args[1] == "-la")) ||
             (n == 3 && ((args[1] == "-a" && args[2] == "-l") || (args[1] == "-l" && args[2] == "-a")))) {
        dir = opendir(path.c_str());
        if (!dir) { perror("ls"); return; }
        bool show_hidden = (args[1].find('a') != string::npos || (n == 3 && args[2].find('a') != string::npos));
        dirent* file;
        while ((file = readdir(dir)) != NULL) {
            if (!show_hidden && file->d_name[0] == '.') continue;
            struct stat buff;
            if (stat((path + "/" + file->d_name).c_str(), &buff) == -1) continue;

            if (S_ISDIR(buff.st_mode)) cout << 'd';
            else if (S_ISREG(buff.st_mode)) cout << "-";
            else if (S_ISCHR(buff.st_mode)) cout << "c";
            else if (S_ISBLK(buff.st_mode)) cout << "b";
            else if (S_ISFIFO(buff.st_mode)) cout << "p";
            else if (S_ISSOCK(buff.st_mode)) cout << "s";

            cout << ((buff.st_mode & S_IRUSR) ? "r" : "-");
            cout << ((buff.st_mode & S_IWUSR) ? "w" : "-");
            cout << ((buff.st_mode & S_IXUSR) ? "x" : "-");
            cout << ((buff.st_mode & S_IRGRP) ? "r" : "-");
            cout << ((buff.st_mode & S_IWGRP) ? "w" : "-");
            cout << ((buff.st_mode & S_IXGRP) ? "x" : "-");
            cout << ((buff.st_mode & S_IROTH) ? "r" : "-");
            cout << ((buff.st_mode & S_IWOTH) ? "w" : "-");
            cout << ((buff.st_mode & S_IXOTH) ? "x" : "-");
            cout << " ";

            cout << buff.st_nlink << " ";
            cout << getpwuid(buff.st_uid)->pw_name << " ";
            cout << getgrgid(buff.st_gid)->gr_name << " ";
            cout << buff.st_size << " ";

            char time_buf[20];
            struct tm* tm = localtime(&buff.st_mtime);
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm);
            cout << time_buf << " ";
            cout << file->d_name << endl;
        }
    }
    else if (n == 2 && args[1] == "..") {
        size_t a = current_dir.find_last_of("/");
        target_path = (a == 0) ? "/" : current_dir.substr(0, a);
        dir = opendir(target_path.c_str());
        if (!dir) { perror("ls"); return; }
        dirent* file;
        while ((file = readdir(dir)) != NULL) {
            if (file->d_name[0] != '.') cout << file->d_name << endl;
        }
    }
    else if (n == 2 && args[1] == "~") {
        dir = opendir(home_dir.c_str());
        if (!dir) { perror("ls"); return; }
        dirent* file;
        while ((file = readdir(dir)) != NULL) {
            if (file->d_name[0] != '.') cout << file->d_name << endl;
        }
    }
    else if (n == 2) {
        target_path = args[1];
        if (target_path[0] != '/') target_path = current_dir + "/" + args[1];
        dir = opendir(target_path.c_str());
        if (!dir) { perror("ls"); return; }
        dirent* file;
        while ((file = readdir(dir)) != NULL) {
            if (file->d_name[0] != '.') cout << file->d_name << endl;
        }
    }
    else if (n == 3 && args[1] == "-l") {
        target_path = args[2];
        if (target_path[0] != '/') target_path = current_dir + "/" + args[2];
        dir = opendir(target_path.c_str());
        if (!dir) { perror("ls"); return; }
        dirent* file;
        while ((file = readdir(dir)) != NULL) {
            if (file->d_name[0] == '.') continue;
            struct stat buff;
            if (stat((target_path + "/" + file->d_name).c_str(), &buff) == -1) continue;

            if (S_ISDIR(buff.st_mode)) cout << 'd';
            else if (S_ISREG(buff.st_mode)) cout << "-";
            else if (S_ISCHR(buff.st_mode)) cout << "c";
            else if (S_ISBLK(buff.st_mode)) cout << "b";
            else if (S_ISFIFO(buff.st_mode)) cout << "p";
            else if (S_ISSOCK(buff.st_mode)) cout << "s";

            cout << ((buff.st_mode & S_IRUSR) ? "r" : "-");
            cout << ((buff.st_mode & S_IWUSR) ? "w" : "-");
            cout << ((buff.st_mode & S_IXUSR) ? "x" : "-");
            cout << ((buff.st_mode & S_IRGRP) ? "r" : "-");
            cout << ((buff.st_mode & S_IWGRP) ? "w" : "-");
            cout << ((buff.st_mode & S_IXGRP) ? "x" : "-");
            cout << ((buff.st_mode & S_IROTH) ? "r" : "-");
            cout << ((buff.st_mode & S_IWOTH) ? "w" : "-");
            cout << ((buff.st_mode & S_IXOTH) ? "x" : "-");
            cout << " ";

            cout << buff.st_nlink << " ";
            cout << getpwuid(buff.st_uid)->pw_name << " ";
            cout << getgrgid(buff.st_gid)->gr_name << " ";
            cout << buff.st_size << " ";

            char time_buf[20];
            struct tm* tm = localtime(&buff.st_mtime);
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm);
            cout << time_buf << " ";
            cout << file->d_name << endl;
        }
    }
    else if(n >= 2 && args[1][0] == '-') {
    bool show_hidden = false, long_list = false;
    string path = current_dir; 

    // collect flags and directory
    for(int i=1; i<n; i++){
        if(args[i][0] == '-'){ // it's a flag
            for(char ch : args[i]){
                if(ch == 'a') show_hidden = true;
                if(ch == 'l') long_list = true;
            }
        } 
        else { // not a flag → assume directory/file
            path = args[i];
        }
    }

    DIR* dir = opendir(path.c_str());
    if(!dir){ perror("ls"); return; }
    struct dirent* file;
    struct stat buff;

    while((file=readdir(dir))!=NULL){
        if(!show_hidden && file->d_name[0]=='.') continue;

        if(long_list){
            string filepath = path + "/" + file->d_name;
            stat(filepath.c_str(), &buff);

            // (reuse your -l printing code here)
        }

        cout << file->d_name << endl;
    }
    }
    else {
        cout << "ls: Invalid arguments" << endl;
    }
    if (dir) closedir(dir);
}


void pinfo(const vector<string>& args) {
    int pid = (args.size() == 1) ? getpid() : stoi(args[1]);
    string path = "/proc/" + to_string(pid) + "/stat";
    ifstream stat_file(path);
    if (!stat_file.is_open()) {
        cout << "pinfo: error opening /proc/" << pid << "/stat" << endl;
        return;
    }

    int pid_num;
    string c, state, pgrp, tpgid;
    long vsize;
    stat_file >> pid_num >> c >> state >> c >> pgrp >> c >> c >> tpgid;
    for (int i = 0; i < 20; i++) {
        string temp;
        stat_file >> temp;
    }
    stat_file >> vsize;

    if (pgrp == tpgid) state += '+';
    cout << "Process state--" << state << endl;
    cout << "Memory--" << vsize << endl;

    string path2 = "/proc/" + to_string(pid) + "/exe";
    char path_buff[1024];
    ssize_t len = readlink(path2.c_str(), path_buff, sizeof(path_buff) - 1);
    if (len == -1) {
        cout << "pinfo: error reading executable path" << endl;
    }
    else {
        path_buff[len] = '\0';
        cout << "Executable Path--" << path_buff << endl;
    }
}

void fore_back(const vector<string>& args) {
    vector<string> v = args;
    int flag = 0;
    if (!v.empty() && v[v.size() - 1] == "&") {
        flag = 1;
        v.erase(v.end() - 1);
    }
    if (!v.empty() && v[0][v[0].size() - 1] == '&') {
        flag = 1;
        v[0] = v[0].substr(0, v[0].size() - 1);
    }

    vector<char*> exec_args;
    for (const auto& arg : v) {
        exec_args.push_back(const_cast<char*>(arg.c_str()));
    }
    exec_args.push_back(NULL);

    int pid = fork();
    if (pid == 0) {
        if (execvp(exec_args[0], exec_args.data()) == -1) {
            perror("execvp");
            exit(1);
        }
    }
    else {
        if (flag == 0) {
            int status;
            waitpid(pid, &status, 0);
        }
        else {
            cout << pid << endl;
        }
    }
}

int search(string path, string target){
    DIR* dir = opendir(path.c_str());
    if(!dir) return 0;

    struct dirent* file;
    struct stat buff;

    while((file = readdir(dir)) != NULL){
        string name = file->d_name;

        if(name == "." || name == "..") continue;

        string new_path = path + "/" + name;
        stat(new_path.c_str(), &buff);

        if(name == target){
            closedir(dir);
            return 1;  
        }

        if(S_ISDIR(buff.st_mode)){
            if(search(new_path, target) == 1){
                closedir(dir);
                return 1; 
            }
        }
    }
    closedir(dir);
    return 0; 
}


void IO_redirection(const vector<string>& args) {
    vector<string> v = IO_parser(args);
    int in_fd = -1, out_fd = -1;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == "<") {
            in_fd = open(v[i + 1].c_str(), O_RDONLY);
            if (in_fd < 0) { perror("cannot open input file"); }
            v.erase(v.begin() + i, v.begin() + i + 2);
            i--;
        }
        else if (v[i] == ">>") {
            out_fd = open(v[i + 1].c_str(), O_CREAT | O_APPEND | O_WRONLY, 0644);
            if (out_fd < 0) { perror("cannot create output file"); }
            v.erase(v.begin() + i, v.begin() + i + 2);
            i--;
        }
        else if (v[i] == ">") {
            out_fd = open(v[i + 1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (out_fd < 0) { perror("cannot create output file"); }
            v.erase(v.begin() + i, v.begin() + i + 2);
            i--;
        }
    }

    vector<char*> exec_args;
    for (const auto& arg : v) {
        exec_args.push_back(const_cast<char*>(arg.c_str()));
    }
    exec_args.push_back(NULL);

    int pid = fork();
    if (pid == 0) {
        if (in_fd != -1) dup2(in_fd, 0);
        if (out_fd != -1) dup2(out_fd, 1);
        if (execvp(exec_args[0], exec_args.data()) == -1) {
            perror("execvp");
            exit(1);
        }
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        if (in_fd != -1) close(in_fd);
        if (out_fd != -1) close(out_fd);
    }
}


void Pipe_redirection(vector<string> v){
    v=Pipes_parser(v);
    vector<vector<string>> commands;
    vector<string> temp;
    for(int i=0;i<v.size();i++){
        if(v[i]=="|"){
            commands.push_back(temp);
            temp.clear();
        } else {
            temp.push_back(v[i]);
        }
    }
    if(!temp.empty()) commands.push_back(temp);

    vector<vector<string>> new_commands;
    for(auto &cmd : commands){
        new_commands.push_back(IO_parser(cmd));
    }

    int n = new_commands.size();
    int prev_in = 0;

    for(int i=0;i<n;i++){
        int pipefd[2];
        if(i < n-1) pipe(pipefd);

        int in_fd=-1, out_fd=-1;

        for(int j=0;j<new_commands[i].size();j++){
            if(new_commands[i][j]=="<"){
                in_fd = open(new_commands[i][j+1].c_str(), O_RDONLY);
                new_commands[i].erase(new_commands[i].begin()+j, new_commands[i].begin()+j+2);
                j--;
            }
            else if(new_commands[i][j]==">"){
                out_fd = open(new_commands[i][j+1].c_str(), O_CREAT|O_WRONLY|O_TRUNC,0644);
                new_commands[i].erase(new_commands[i].begin()+j, new_commands[i].begin()+j+2);
                j--;
            }
            else if(new_commands[i][j]==">>"){
                out_fd = open(new_commands[i][j+1].c_str(), O_CREAT|O_WRONLY|O_APPEND,0644);
                new_commands[i].erase(new_commands[i].begin()+j, new_commands[i].begin()+j+2);
                j--;
            }
        }

        int pid=fork();
        if(pid==0){
            if(prev_in!=0){
                dup2(prev_in, STDIN_FILENO);
                close(prev_in);
            }

            if(i < n-1){
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }
           
            if(in_fd!=-1) dup2(in_fd, STDIN_FILENO);
            if(out_fd!=-1) dup2(out_fd, STDOUT_FILENO);

         
            vector<char*> args;
            for(auto &s : new_commands[i]) args.push_back((char*)s.c_str());
            args.push_back(NULL);

            execvp(args[0], args.data());
            perror("execvp");
            exit(1);
        }
        else{
            wait(NULL);
            if(prev_in!=0) close(prev_in);
            if(i < n-1){
                close(pipefd[1]);
                prev_in = pipefd[0];
            }
        }
    }
}
