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
        ASSERT_EQ(bfm.get_analytics(), *analytics);
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
    int n = int('a');
    run_test_program(
        std::string(n, '+')+">[<...]<+.",
        "b",
        BFMachine::Analytics{
            .max_stack_depth=1,
            .loop_count={{n+1,0}}
        }
    );
}

TEST(BrainFuckI, Lua)
{
    run_test_program(
        "+++++++++++++++++++++++++[>++>+++>++++>+++++<<<<-]+++++++++++++++++++++++++>>+.>>--------.<---.<<<---------------.",
        "Lua\n",
        BFMachine::Analytics{
            .max_stack_depth=1,
            .loop_count={{25,25}}
        }
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

TEST(BrainFuckI, Difficult2)
{
    run_test_program(
        "+++++[>+++++[>++>++>+++>+++>++++>++++<<<<<<-]<-]+++++[>>[>]<[+.<<]>[++.>>>]<[+.<]>[-.>>]<[-.<<<]>[.>]<[+.<]<-]++++++++++.",
        "eL34NfeOL454KdeJ44JOdefePK55gQ67ShfTL787KegJ77JTeghfUK88iV9:XjgYL:;:KfiJ::JYfijgZK;;k[<=]lh^L=>=KgkJ==J^gklh_K>>m`?@bnicL@A@KhmJ@@JchmnidKAA\n"
    );
}

TEST(BrainFuckI, Comment)
{
    run_test_program(
        "++++++++++[>++++++++++>++++++++++++<<-]>-- this code has comments in it ->++>+++++++++++++[<.-<.+>>-]++++[<<-------------------------.>>-]",
        "zaybxcwdveuftgshriqjpkolnmU<#\n"
    );
}

TEST(BrainFuckI, squares)
{
    run_test_program(
        R"(
            ++++[>+++++<-]>[<+++++>-]+<+[
                >[>+>+<<-]++>>[<<+>>-]>>>[-]++>[-]+
                >>>+[[-]++++++>>>]<<<[[<++++++++<++>>-]+<.<[>----<-]<]
                <<[>>>>>[>>>[-]+++++++++<[>-<-]+++++++++>[-[<->-]+[<<<]]<[>+<-]>]<<-]<<-
            ]
        )",
        R"(0
1
4
9
16
25
36
49
64
81
100
121
144
169
196
225
256
289
324
361
400
441
484
529
576
625
676
729
784
841
900
961
1024
1089
1156
1225
1296
1369
1444
1521
1600
1681
1764
1849
1936
2025
2116
2209
2304
2401
2500
2601
2704
2809
2916
3025
3136
3249
3364
3481
3600
3721
3844
3969
4096
4225
4356
4489
4624
4761
4900
5041
5184
5329
5476
5625
5776
5929
6084
6241
6400
6561
6724
6889
7056
7225
7396
7569
7744
7921
8100
8281
8464
8649
8836
9025
9216
9409
9604
9801
10000
)"
    );
}

TEST(BrainFuckI, quine1)
{
    const std::string quine = "->++>+++>+>+>++>>+>+>+++>>+>+>++>+++>+++>+>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>+>+>++>>>+++>>>>>+++>+>>>>>>>>>>>>>>>>>>>>>>+++>>>>>>>++>+++>+++>+>>+++>+++>+>+++>+>+++>+>++>+++>>>+>+>+>+>++>+++>+>+>>+++>>>>>>>+>+>>>+>+>++>+++>+++>+>>+++>+++>+>+++>+>++>+++>++>>+>+>++>+++>+>+>>+++>>>+++>+>>>++>+++>+++>+>>+++>>>+++>+>+++>+>>+++>>+++>>+[[>>+[>]+>+[<]<-]>>[>]<+<+++[<]<<+]>>>[>]+++[++++++++++>++[-<++++++++++++++++>]<.<-<]";
    run_test_program(quine, quine);
}