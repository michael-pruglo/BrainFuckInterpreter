#include <iostream>
#include <iomanip>
#include <cassert>
#include "BFMachine.hpp"

BFMachine::BFMachine()
{
    reset();
}

void BFMachine::reset()
{
    code.clear();

    mem.clear();
    mem.push_back(0);
    mem_ptr = mem.begin();

    output.clear();
}

std::string BFMachine::get_output() const
{
    return output;
}

void BFMachine::display_state() const
{
    std::cout << "code__:" << code << "\n";
    const auto code_pos = code_ptr-code.begin();
    std::cout << std::string(7+code_pos,' ') << "^\n";
    std::cout << "stack size: " << loop_labels.size() << "\n";
    std::cout << "inactive: " << inactive << "\n";

    std::cout << "memory:";
    for (const auto& c: mem)
        std::cout << "| " << std::setw(2) << c << " ";
    std::cout << "|\n";
    const auto mem_pos = mem_ptr-mem.begin();
    std::cout << std::string(8+mem_pos*5+2,' ') << "^\n";

    std::cout << "output:" << output << "\n\n\n";

    std::cout << std::flush;
}

void BFMachine::exec(const std::string& code_, bool verbose)
{
    code = code_;
    for (code_ptr = code.begin(); code_ptr!=code.end(); )
    {
        process_char();
        if (verbose)
            display_state();
    }
}

void BFMachine::process_char()
{
    switch(*code_ptr)
    {
        case '-': if (!inactive) { --*mem_ptr; } break;
        case '+': if (!inactive) { ++*mem_ptr; } break;
        case '<': if (!inactive) { if(mem_ptr==mem.begin())mem.push_front(0); --mem_ptr; } break;
        case '>': if (!inactive) { if(mem_ptr==mem.end()-1)mem.push_back(0); ++mem_ptr; } break;
        case '.': if (!inactive) { output+=char(*mem_ptr); } break;
        case '[':
        {
            loop_labels.push(code_ptr);
            if (!inactive && !*mem_ptr)
                inactive = loop_labels.size();
            break;
        }
        case ']':
            assert(!loop_labels.empty());
            if (!inactive)
                code_ptr = loop_labels.top();
            else
                ++code_ptr;
            if (loop_labels.size() == inactive)
                inactive = false;
            loop_labels.pop();
            return;
        default:
        {
            output += "\ninvalid symbol '"+std::string(1,*code_ptr)+
                "' at position "+std::to_string(code_ptr-code.begin())+"\n";
        }
    }
    ++code_ptr;
}
