#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#define ASSERT_EQ(a, b)     REQUIRE((a) == (b))
#define ASSERT_TRUE(a)      REQUIRE(a)
#define ASSERT_FALSE(a)     REQUIRE(!(a))

extern "C" {
#include "ultron.h"
}


TEST_CASE("Ultron.Creating", "[Ultron]") {
    Ultron ultron;
    ultron_init(&ultron);
    ASSERT_EQ(0, ultron_pc(&ultron));
    ASSERT_EQ(0, ultron_accumulator(&ultron));
    for (int i = 0; i < ULTRON_MEMORY_SIZE; i++) {
        ASSERT_EQ(0, ultron_memory(&ultron, i));
    }
}

TEST_CASE("Ultron.Loading", "[Ultron]") {
    Ultron ultron;
    ultron_init(&ultron);
    int program[] = {
        1010,
        1011,
        2010,
        3011,
        2112,
        1112,
        4300,
    };
    ultron_load(&ultron, sizeof(program) / sizeof(int), program);
    for (int i = 0; i < sizeof(program) / sizeof(int); i++) {
        ASSERT_EQ(program[i], ultron_memory(&ultron, i));
    }
    for (int i = sizeof(program) / sizeof(int); i < ULTRON_MEMORY_SIZE; i++) {
        ASSERT_EQ(0, ultron_memory(&ultron, i));
    }
}

TEST_CASE("Ultron.Adding", "[Ultron]") {
    Ultron ultron;
    ultron_init(&ultron);
    int program[] = {
        2005,
        3006,
        2107,
        4300,
        0,
        42,
        66,
    };
    ultron_load(&ultron, sizeof(program) / sizeof(int), program);
    int result = ultron_run(&ultron);
    ASSERT_EQ(0, result);
    ASSERT_EQ(42 + 66, ultron_memory(&ultron, 7));
}


TEST_CASE("Ultron.Subtracting", "[Ultron]") {
    Ultron ultron;
    ultron_init(&ultron);

    int program[] = {
        2005,
        3106,
        2107,
        4300,
        0,
        42,
        22,
    };
    ultron_load(&ultron, sizeof(program) / sizeof(int), program);
    int result = ultron_run(&ultron);
    ASSERT_EQ(0, result);
    ASSERT_EQ(42 - 22, ultron_memory(&ultron, 7));
}

TEST_CASE("Ultron.Branching", "[Ultron]") {
    Ultron ultron;
    ultron_init(&ultron);
    int program[] = {
        2010,
        4003,
        4300,
        2011,
        2108,
        4300,
        0,
        0,
        0,
        0,
        -2,
        13,
    };
    ultron_load(&ultron, sizeof(program) / sizeof(int), program);
    int result = ultron_run(&ultron);
    ASSERT_EQ(0, result);
    ASSERT_EQ(13, ultron_memory(&ultron, 8));
}

TEST_CASE("Ultron.Looping", "[Ultron]") {
    Ultron ultron;
    ultron_init(&ultron);
    int program[] = {
        2008,
        3109,
        4007,
        2108,
        1108,
        2010,
        4000,
        4300,
        101,
        1,
        -1,
    };
    ultron_load(&ultron, sizeof(program) / sizeof(int), program);
    int result = ultron_run(&ultron);
    ASSERT_EQ(0, result);
}

TEST_CASE("Ultron.CountingProgram", "[Ultron]") {
    Ultron ultron;
    ultron_init(&ultron);
    int program[] = {
        2008,
        3109,
        4007,
        2108,
        1108,
        2010,
        4000,
        4300,
        101,
        1,
        -1,
    };
    ultron_load(&ultron, sizeof(program) / sizeof(int), program);
    int result = ultron_run(&ultron);
    ASSERT_EQ(0, result);
}
