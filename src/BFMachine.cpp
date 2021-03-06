#include <iostream>
#include <iomanip>
#include <cassert>
#include <chrono>
#include <thread>
#include <iterator>
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
    mem_idx = 0;

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
    std::cout << std::string(8+mem_idx*5+2,' ') << "^\n";

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
    std::cout << "  memory used: < " << mem.size() << "\n";
    std::cout << "  current position: " << mem_idx << "\n";

    std::cout << std::flush;
}

BFMachine::Analytics BFMachine::get_analytics() const
{
    return analytics;
}

void BFMachine::exec(const std::string& code_, bool verbose, int delay_ms)
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
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
    }
}

void BFMachine::process_char()
{
    switch(*code_ptr)
    {
        case '-': if (!inactive) { --mem[mem_idx]; } break;
        case '+': if (!inactive) { ++mem[mem_idx]; } break;
        case '<': if (!inactive) { assert(mem_idx>0); --mem_idx; } break;
        case '>': if (!inactive) { if(mem_idx==mem.size()-1) mem.push_back(0); ++mem_idx; } break;
        case '.': if (!inactive) { output+=char(mem[mem_idx]); } break;
        case '[':
        {
            loop_labels.push(code_ptr);
            analytics.max_stack_depth = std::max(analytics.max_stack_depth, loop_labels.size());
            if (!inactive)
            {
                analytics.loop_count[code_ptr-code.begin()] += !!mem[mem_idx];
                if (!mem[mem_idx])
                    inactive = loop_labels.size();
            }
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
            //any other symbol ignored as a comment
            break;
    }
    ++code_ptr;
}

bool BFMachine::Analytics::operator==(const BFMachine::Analytics& other) const
{
    return max_stack_depth == other.max_stack_depth
        && loop_count == other.loop_count;
}
