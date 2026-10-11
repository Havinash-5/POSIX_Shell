#include "../include/shell.h"
#include "../include/commands.h"
#include "../include/utils.h"
#include<iostream>
#include<readline/history.h>
string historia;
int main() {
    struct passwd* pw = getpwuid(geteuid());
    string username = pw->pw_name;
    

    char buff[256];
    gethostname(buff, sizeof(buff));
    string hostname(buff);

    char path[256];
    getcwd(path, sizeof(path));
    string home_dir(path);
    string last_dir(path);
    historia=home_dir+"/.history";
    //cout<<historia;
    read_history(historia.c_str());
    stifle_history(20);
    run_shell(home_dir, last_dir, username, hostname);
    
    return 0;
}

void run_shell(const string& home_dir, string& last_dir, const string& username, const string& hostname) {
    while (true) {
        char temp[256];
        getcwd(temp, sizeof(temp));
        string current_dir(temp);

        string display_path;
        if (current_dir == home_dir) display_path = "~";
        else if (current_dir.find(home_dir) == 0)
            display_path = "~" + current_dir.substr(home_dir.length());
        else
            display_path = current_dir;

        
        string prompt= username +"@" + hostname + ":" + display_path + "> ";

        char* input1;
        input1=readline(prompt.c_str());
        string input(input1);
        add_history(input.c_str());
        write_history(historia.c_str());
        history_truncate_file(historia.c_str(),20);
        char* dupe = strdup(input.c_str());
        char* c = strtok(dupe, " \t");
        vector<string> v;
        while (c != nullptr) {
            string token(c);
            v.push_back(strip_quotes(token));
            c = strtok(NULL, " \t");
        }
        free(dupe);

        while (!v.empty()) {
            vector<string> new_v = uptoSemicolon(v);
            int check = check_IO(new_v);
            int is_pipe=check_pipe(new_v);
            if (new_v.empty()) continue;
            if (new_v[0] == "echo" && check == 0&& is_pipe==0) {
                execute_echo(new_v);
            }
            else if (new_v[0] == "cd" && check == 0) {
                execute_cd(new_v, home_dir, last_dir);
            }
            else if (new_v[0] == "pwd" && check == 0) {
                execute_pwd();
            }
            else if (new_v[0] == "ls" && check == 0&& is_pipe==0) {
                ls_command(temp, new_v, home_dir, current_dir);
            }
            else if(new_v[0]=="search" && check==0&& is_pipe==0){
                 if(new_v.size() != 2){
                    cout << "wrong argumrnts"<< endl;
                }
                else{
                    char cwd[256];
                    getcwd(cwd, sizeof(cwd));
                    if(search(cwd, new_v[1])){
                        cout << "True"<< endl;
                    }
                    else{
                        cout << "False"<< endl;
                    }
                }
            }
            else if (new_v[0] == "pinfo" && check == 0) {
                pinfo(new_v);
            }
            else if (check == 1&&is_pipe==0) {
                IO_redirection(new_v);
            }
            else if(is_pipe==1){
                Pipe_redirection(new_v);
            }
            else if (new_v[0] == "exit") {
                return;
            }
            else if(new_v[0]=="history"){
                int n;
                if(new_v.size()!=2){
                    if(history_length>10){
                        n=history_length-10;
                    }
                    else{
                        n=0;
                    }
                }
                else{
                    if(stoi(new_v[1])>=20){
                        n=0;
                    }
                    else{
                        n=history_length-stoi(new_v[1]);
                    }
                }
                if(n<0){n=0;}
                vector<string> hist;
                vector<int> index;
                HIST_ENTRY** list=history_list();

                for(int i=history_length-1;i>=n;i--){
                    hist.push_back(list[i]->line);
                    index.push_back(i+1);
                }
                for(int i=hist.size()-1;i>=0;i--){
                    cout<<index[i]<<" "<<hist[i]<<endl;
                }
            }
            else {
                fore_back(new_v);
            }
        }
    }
}
