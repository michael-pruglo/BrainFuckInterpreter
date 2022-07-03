#include "gtest/gtest.h"
#include "BFMachine.hpp"
#include "BFMachine.cpp"

TEST(BFMachine1c, Init)
{
    std::cout << "First test case";
    EXPECT_EQ(1, 1);
}

void run_test_program(const std::string& code, const std::string& exp_output)
{
    BFMachine bfm;
    bfm.exec(code, true);
    ASSERT_EQ(bfm.get_output(), exp_output);
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