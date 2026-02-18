#include <gtest.h>

// g++ test/*.cpp  -I./include src/*.cpp -I./gtest gtest/gtest-all.cc
// сборка всех тестов

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
