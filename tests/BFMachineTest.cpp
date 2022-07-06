#include <optional>
#include "gtest/gtest.h"
#include "BFMachine.hpp"
#include "BFMachine.cpp"

void run_test_program(
    const std::string& code,
    const std::string& exp_output,
    std::optional<BFMachine::Analytics> analytics = std::nullopt
)
{
    BFMachine bfm;
    bfm.exec(code, false);
    ASSERT_EQ(bfm.get_output(), exp_output);
    if (analytics)
    {
        std::cout<<"Here"<<std::flush;
        ASSERT_EQ(bfm.get_analytics(), *analytics);
    }
}

TEST(BrainFuckI, Empty)
{
    run_test_program("", "");
}

TEST(BrainFuckI, SimplePlus)
{
    for (auto ch: {'a', 'R', 'z', 'U'})
        run_test_program(std::string(ch, '+')+".", std::string(1, ch));
}

TEST(BrainFuckI, SimplePlusMinus)
{
    run_test_program("+-+-+-+++--+--"+std::string(int('x'), '+')+".", "x");
}

TEST(BrainFuckI, MultiplePrints)
{
    run_test_program(std::string(int('x'), '+')+".+.+.---.", "xyzw");
}

TEST(BrainFuckI, FirstIgnoreLoop)
{
    run_test_program(
        std::string(int('a'), '+')+">[<...]<+.",
        "b",
        BFMachine::Analytics{
            .max_stack_depth=1,
            .first_ignore_loops_number=1,
            .moves_left=0,
            .moves_right=1
        }
    );
}

TEST(BrainFuckI, Lua)
{
    run_test_program(
        "+++++++++++++++++++++++++[>++>+++>++++>+++++<<<<-]+++++++++++++++++++++++++>>+.>>--------.<---.<<<---------------.",
        "Lua\n"
    );
}

TEST(BrainFuckI, PHP)
{
    run_test_program(
        "+++++++++++++++++++++[>++>+++>++++>+++++>++++++<<<<<-]+++++++++++++++++++++>>>----.--------.++++++++.<<<-----------.",
        "PHP\n"
    );
}

TEST(BrainFuckI, Random1)
{
    run_test_program(
        "++++++++++[>++++++++++>++++++++++++<<-]>--->++>+++++++++++++[<.-<.+>>-]++++[<<-------------------------.>>-]",
        "zaybxcwdveuftgshriqjpkolnmU<#\n"
    );
}

TEST(BrainFuckI, Difficult1)
{
    run_test_program(
        ">>>>>>>>>>>>>>>>>>>>>>>>>>>>++++++++++++++++++++++++++[-<<[+<]+[>]>][<<[[-]-----<]>[>]>]<<[++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++<]>[.>]++++++++++.",
        "abcdefghijklmnopqrstuvwxyz\n"
    );
}