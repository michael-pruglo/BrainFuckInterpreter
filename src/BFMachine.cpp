#include <iostream>
#include <iomanip>
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>
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

    std::cout << "output:" << output << "\n";

    std::cout << std::flush;
}

void BFMachine::display_analytics() const
{
    std::cout << "Analytics:\n";
    std::cout << "  max stack depth: " << analytics.max_stack_depth << "\n";
    std::cout << "  loops: \n";
    if (analytics.loop_count.empty())
        std::cout << "    none\n";
    else
    {
        using vec_t = std::vector<std::pair<int, int>>;
        vec_t loops_vec;
        std::copy(analytics.loop_count.begin(),analytics.loop_count.end(),std::back_inserter<vec_t>(loops_vec));
        std::sort(loops_vec.begin(), loops_vec.end(),[](const auto& l, const auto& r){
                return l.second != r.second ? l.second < r.second : l.first < r.first;
        });
        for (const auto& [pos,cnt]: loops_vec)
            std::cout << "    [at " << std::setw(4) << pos << ", x" << std::setw(2) << cnt << "]\n";
    }
    std::cout << "  memory used: < " << analytics.moves_left << "   > " << analytics.moves_right << "  total " << mem.size() << "\n";
    assert(analytics.moves_left+analytics.moves_right == int(mem.size())-1);
    auto curr_pos = mem_ptr - mem.begin() - analytics.moves_left;
    std::cout << "  current position: " << curr_pos << "\n";

    std::cout << std::flush;
}

BFMachine::Analytics BFMachine::get_analytics() const
{
    return analytics;
}

void BFMachine::exec(const std::string& code_, bool verbose)
{
    code = code_;
    for (code_ptr = code.begin(); code_ptr!=code.end(); )
    {
        process_char();
        if (verbose)
        {
            display_state();
            display_analytics();
            std::cout<<"\n\n\n\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(77));
        }
    }
}

void BFMachine::process_char()
{
    switch(*code_ptr)
    {
        case '-': if (!inactive) { --*mem_ptr; } break;
        case '+': if (!inactive) { ++*mem_ptr; } break;
        case '<': if (!inactive) { if(mem_ptr==mem.begin()){mem.push_front(0);++analytics.moves_left;} --mem_ptr; } break;
        case '>': if (!inactive) { if(mem_ptr==mem.end()-1){mem.push_back(0);++analytics.moves_right;} ++mem_ptr; } break;
        case '.': if (!inactive) { output+=char(*mem_ptr); } break;
        case '[':
        {
            loop_labels.push(code_ptr);
            analytics.max_stack_depth = std::max(analytics.max_stack_depth, loop_labels.size());
            if (!inactive && !*mem_ptr)
                inactive = loop_labels.size();
            if (!inactive)
                analytics.loop_count[code_ptr-code.begin()] += !!*mem_ptr;
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
