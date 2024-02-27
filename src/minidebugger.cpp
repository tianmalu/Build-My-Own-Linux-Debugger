#include <vector>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/personality.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../ext/linenoise-master/linenoise.h"

#include "../include/debugger.hpp"
#include "../include/registers.hpp"
using namespace minidebugger;

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> out{};
    std::stringstream ss {s};
    std::string item;

    while (std::getline(ss,item,delimiter)) {
        out.push_back(item);
    }

    return out;
}

bool is_prefix(const std::string& s, const std::string& of) {
    if (s.size() > of.size()) return false;
    return std::equal(s.begin(), s.end(), of.begin());
}

void debugger::run(){
    int wait_status;
    auto options = 0;
    waitpid(m_pid, &wait_status, options);

    char* line = nullptr;
    while((line =  linenoise("minidebugger> ")) != nullptr){
        handle_command(line);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }

}

void debugger::handle_command(const std::string& line) {
    auto args = split(line,' ');
    auto command = args[0];

    if (is_prefix(command, "continue")) {
        continue_execution();
    }
    else {
        std::cerr << "Unknown command\n";
    }
}

void debugger::continue_execution() {
    ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(m_pid, &wait_status, options);
}


int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr << "Program name not specified";
        return -1;
    }

    auto prog = argv[1]; //save program name
    auto pid = fork(); // create new process, return pid -> return 0 when kind process start

    if(pid == 0){
        //In Child Process
        //we want to replace whatever we’re currently executing with the program we want to debug.
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        execl(prog, prog, nullptr);
    }
    else if (pid >= 1){
        //In Father Process
        std::cout << "Started debugging child process " << pid << '\n';
        debugger dbg{prog, pid};
        dbg.run();
    }
}
