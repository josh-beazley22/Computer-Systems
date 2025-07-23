#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#define ASSERT_EQ(a, b)     REQUIRE((a) == (b))
#define ASSERT_TRUE(a)      REQUIRE(a)
#define ASSERT_FALSE(a)     REQUIRE(!(a))

// Run the command on the system, and return the output
// This redirects the output of the command to the file named "out",
// then reads the entire file in and returns the data as a string
string runcheck(string cmd) {
    cout << "Running cmd: " + cmd << endl;
    cmd += " > out";
    system(cmd.c_str());
    ifstream is("out");
    string data;
    getline(is, data, '\0');
    return data;
}

TEST_CASE("Convert.6_to_binary", "[Convert]") {
    REQUIRE(runcheck("./convert -b 6") == "6=110\n");
}

TEST_CASE("Convert.789_to_binary", "[Convert]") {
    REQUIRE(runcheck("./convert -b 789") == "789=11 0001 0101\n");
}

TEST_CASE("Convert.10_to_hex", "[Convert]") {
    REQUIRE(runcheck("./convert -x 10") == "10=0xa\n");
}

TEST_CASE("Convert.672_to_hex", "[Convert]") {
    REQUIRE(runcheck("./convert -x 672") == "672=0x2 a0\n");
}

TEST_CASE("Convert.multiple_binary", "[Convert]") {
    REQUIRE(runcheck("./convert -b 87 888 23") == "87=101 0111\n888=11 0111 1000\n23=1 0111\n");
}

TEST_CASE("Convert.multiple_hex", "[Convert]") {
    REQUIRE(runcheck("./convert -x 0 22 390 4096") == "0=0x0\n22=0x16\n390=0x1 86\n4096=0x10 00\n");
}

string USAGE="Usage: ./convert [-x|-b] num1 [num2 ...]\n";

TEST_CASE("Convert.no_option", "[Convert]") {
    REQUIRE(runcheck("./convert 1234 456") == USAGE);
}

TEST_CASE("Convert.bad_args", "[Convert]") {
    REQUIRE(runcheck("./convert -x") == USAGE);
    REQUIRE(runcheck("./convert -b") == USAGE);
    REQUIRE(runcheck("./convert -k") == USAGE);
    REQUIRE(runcheck("./convert -x -p 1234") == USAGE);
}

TEST_CASE("Convert.style", "[Convert]") {
    int status = system("cpplint convert.c");
    REQUIRE(WEXITSTATUS(status) == 0);
}
