#pragma once
#include <vector>
#include <stack>
#include <unordered_map>

class IntCode
{

};

class BFMachine
{
public:
                BFMachine();
    void        reset();

    void        exec(const std::string& code, bool verbose=false, int delay_ms=77);

    std::string get_output() const;
    void        display_state() const;

public:
    struct Analytics
    {
        size_t max_stack_depth = 0;
        std::unordered_map<int, int> loop_count;
        bool operator==(const Analytics& other) const;
    };
    void        display_analytics() const;
    Analytics   get_analytics() const;

private:
    void        process_char();

private:
    using code_container = std::string;
    code_container code;
    code_container::iterator code_ptr;
    std::stack<code_container::iterator> loop_labels;

    std::vector<int> mem;
    size_t mem_idx = 0;

    std::string output;
    int inactive = 0;

    Analytics analytics;
};
