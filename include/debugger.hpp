#ifndef MINIDBG_DEBUGGER_HPP
#define MINIDBG_DEBUGGER_HPP

#include <utility>
#include <string>
#include <linux/types.h>
#include <unordered_map>

#include "breakpoint.hpp"
//#include "dwarf/dwarf++.hh"
//#include "elf/elf++.hh"


namespace minidebugger{
    class debugger {
        public:
            debugger (std::string prog_name, pid_t pid):
                m_pid(pid), m_prog_name{std::move(prog_name)} {}
            
            void run();
            
        private:
            void handle_command(const std::string& line);
            void continue_execution();


            std::string m_prog_name;
            pid_t m_pid;

            
    };
}

#endif