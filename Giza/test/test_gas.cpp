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
    // cout << "Running cmd: " + cmd << endl;
    cmd += " > out";
    system(cmd.c_str());
    ifstream is("out");
    string data;
    getline(is, data, '\0');
    return data;
}

TEST_CASE("Giza.1", "[Giza]") {
    REQUIRE(runcheck("./giza 1") == "*\nFinito\n");
}

TEST_CASE("Giza.2", "[Giza]") {
    REQUIRE(runcheck("./giza 2") == "*\n**\nFinito\n");
}

TEST_CASE("Giza.5", "[Giza]") {
    REQUIRE(runcheck("./giza 5") == "*\n**\n***\n****\n*****\nFinito\n");
}

TEST_CASE("Giza.10", "[Giza]") {
    REQUIRE(runcheck("./giza 10") == "*\n**\n***\n****\n*****\n******\n*******\n********\n*********\n**********\nFinito\n");
}

TEST_CASE("Giza.21", "[Giza]") {
    string result = "";
    for (int i = 1; i <= 21; i++) {
        result += string(i, '*');
        result += "\n";
    }
    result += "Finito\n";
    REQUIRE(runcheck("./giza 21") == result);
}


TEST_CASE("Wc.empty", "[Wc]") {
    cout << "Running wc on empty string... ";
    REQUIRE(runcheck("./wc < /dev/null") == "Result=0\n");
    cout << "success" << endl;
}

TEST_CASE("Wc.one", "[Wc]") {
    cout << "Running wc on single word... ";
    REQUIRE(runcheck("echo \"rambunctious\" | ./wc") == "Result=1\n");
    cout << "success" << endl;
}

TEST_CASE("Wc.space", "[Wc]") {
    cout << "Running wc on spaces between words... ";
    REQUIRE(runcheck("echo \"Rock paper scissors\" | ./wc") == "Result=3\n");
    cout << "success" << endl;
}

TEST_CASE("Wc.multi", "[Wc]") {
    cout << "Running wc on multiple words and spaces... ";
    REQUIRE(runcheck("echo \"\tTo be   or not\n  to be \r\" | ./wc") == "Result=6\n");
    cout << "success" << endl;
}
