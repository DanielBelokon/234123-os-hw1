#pragma once
#include <string>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include <cstring>

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY() \
    cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT() \
    cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

// include for memset

namespace CommandUtils
{
    inline std::string _ltrim(const std::string &s)
    {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    inline std::string _rtrim(const std::string &s)
    {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    inline std::string _trim(const std::string &s)
    {
        return _rtrim(_ltrim(s));
    }

    inline int _parseCommandLine(const char *cmd_line, char **args)
    {
        FUNC_ENTRY()
        int i = 0;
        std::istringstream iss(_trim(std::string(cmd_line)).c_str());
        for (std::string s; iss >> s;)
        {
            args[i] = (char *)malloc(s.length() + 1);
            std::memset(args[i], 0, s.length() + 1);
            std::strcpy(args[i], s.c_str());
            args[++i] = NULL;
        }
        return i;

        FUNC_EXIT()
    }

    inline bool _isBackgroundComamnd(const char *cmd_line)
    {
        const std::string str(cmd_line);
        return str[str.find_last_not_of(WHITESPACE)] == '&';
    }

    inline void _removeBackgroundSign(char *cmd_line)
    {
        const std::string str(cmd_line);
        // find last character other than spaces
        unsigned int idx = str.find_last_not_of(WHITESPACE);
        // if all characters are spaces then return
        if (idx == std::string::npos)
        {
            return;
        }
        // if the command line does not end with & then return
        if (cmd_line[idx] != '&')
        {
            return;
        }
        // replace the & (background sign) with space and then remove all tailing spaces.
        cmd_line[idx] = ' ';
        // truncate the command line string up to the last non-space character
        cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
    }
}