#include "gtest/gtest.h"
#include "BuiltInCommands.h"
#include <string>

TEST(BuiltInCommandsTests, TestBuiltInCommands)
{
    EXPECT_EQ(0, 0);
}

TEST(GetCurrentDirectoryTests, TestGetCurrentDirectory)
{
    GetCurrDirCommand command = GetCurrDirCommand("pwd");
    std::ostringstream out;
    command.setOutputStream(&out);
    // Get the current working directory, and store it in a string.
    char buffer[256];
    getcwd(buffer, sizeof(buffer));
    std::string actual(buffer);

    command.execute();
    EXPECT_EQ(out.str(), actual);
}