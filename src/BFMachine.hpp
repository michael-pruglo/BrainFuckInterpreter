#pragma once
#include <string>
#include <deque>
#include <stack>

class IntCode
{

};

class BFMachine
{
public:
                BFMachine();
    void        reset();

    void        exec(const std::string& code, bool verbose=false);

    std::string get_output() const;
    void        display_state() const;

private:
    void process_char();

private:
    using code_container = std::string;
    code_container code;
    code_container::iterator code_ptr;
    std::stack<code_container::iterator> loop_labels;

    using mem_container = std::deque<int>;
    mem_container mem;
    mem_container::iterator mem_ptr;

    std::string output;
    int inactive = 0;
};
