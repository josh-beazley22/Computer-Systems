#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#define ASSERT_EQ(a, b)     REQUIRE((a) == (b))
#define ASSERT_TRUE(a)      REQUIRE(a)
#define ASSERT_FALSE(a)     REQUIRE(!(a))


// Run a check for the shell running input commands, with the expected
// output strings. strict indicates whether these should be strictly increasing
// in position
bool runcheck(string input, vector<string>& expected, bool strict = false) {
    cout << "Test input: [\n" << input << "\n]" << endl;
    string cmd = "echo \"" + input + "\" | ./shell" + " > out";
    int rv = system(cmd.c_str());
    REQUIRE(rv == 0);

    string actualcontent;
    getline(ifstream("out"), actualcontent, '\0');
    cout << "Actual output:\n" << actualcontent << endl;
    unlink("out");

    cout << "Checking for embedded content..." << endl;
    if (strict) {
        string embedded = "";
        for (int i = 0; i < expected.size(); i++) {
            embedded += expected[i] + "\n";
        }
        int pos = actualcontent.find(embedded);
        if (pos < 0) {
            cout << "Could not find: " + embedded << endl;
        }
        REQUIRE(pos >= 0);
    } else {
        for (int i = 0; i < expected.size(); i++) {
            cout << "\t" << expected[i] << endl;
            int pos = actualcontent.find(expected[i] + "\n");
            if (pos < 0) {
                cout << "ERROR: could not find " << expected[i] << endl;
            }
            REQUIRE(pos >= 0);
        }
    }
    cout << "Success" << endl;
    return true;
}


TEST_CASE("Shell.LsNoArgs", "[Ls]") {
    vector<string> expected{
        "DIR .",
        "DIR ..",
        "REG shell.o",
        "DIR test",
        "REG Makefile",
        "REG README.md",
        "REG shell.c",
        "REG .gitignore",
        "DIR .git",
    };
    runcheck("ls", expected);
}

TEST_CASE("Shell.LsOneArg", "[Ls]") {
    vector<string> expected{
        "DIR .",
        "DIR ..",
        "REG catch.hpp",
        "REG test_main.cpp",
        "REG moby.txt",
        "REG moby-short.txt",
        "DIR magicdir",
    };
    runcheck("ls test", expected);

    vector<string> expected2{
        "DIR .",
        "DIR ..",
        "REG fileinsidemagicdir",
    };
    runcheck("ls test/magicdir", expected2);
}


TEST_CASE("Shell.CdOneArg", "[Cd]") {
    vector<string> expected{
        "DIR .",
        "DIR ..",
        "REG catch.hpp",
        "REG test_main.cpp",
        "REG moby.txt",
        "REG moby-short.txt",
        "DIR magicdir",
    };
    runcheck("cd test\nls", expected);
}

TEST_CASE("Shell.CdNoArgs", "[Cd]") {
    vector<string> expected{
        // These are from the cd to the test directory
        "REG catch.hpp",
        "REG test_main.cpp",
        "REG moby.txt",
        "REG moby-short.txt",
        "DIR magicdir",
        // These are from the cd back to home
        "DIR .",
        "DIR ..",
        "REG shell.o",
        "DIR test",
        "REG Makefile",
        "REG README.md",
        "REG shell.c",
        "REG .gitignore",
        "DIR .git",

    };
    runcheck("cd test\nls\ncd\nls", expected);
}

TEST_CASE("Shell.Last4Long", "[Last4]") {
    vector<string> expected{
        "-- Last 4 lines --",
        "world. But the time of his justification has now come. His three whales",
        "are as good whales as were ever published in Wapping, at any rate; and",
        "his stump as unquestionable a stump as any you will find in the western",
        "clearings",
        "-- End --",
    };
    runcheck("last4 test/moby.txt", expected, true);
}


TEST_CASE("Shell.Last4Short", "[Last4]") {
    vector<string> expected{
        "-- Last 4 lines --",
        "crippled beggar (or kedger, as the sailors say) holding a painted board",
        "before him, representing the tragic scene in which he lost his leg.",
        "-- End --",
    };
    runcheck("last4 test/moby-short.txt", expected, true);
}
