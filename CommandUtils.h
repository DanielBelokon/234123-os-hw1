#ifndef SMASH_COMMANDUTILS_H_
#define SMASH_COMMANDUTILS_H_
#include <string>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include <cstring>
#include <sstream>

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

    inline bool _isBackgroundComamnd(const char *cmd_line)
    {
        const std::string str(cmd_line);
        return str[str.find_last_not_of(WHITESPACE)] == '&';
    }

    inline void _removeBackgroundSign(std::string &cmd_line)
    {
        cmd_line = _trim(cmd_line);
        if (cmd_line[cmd_line.length() - 1] == '&')
        {
            cmd_line = cmd_line.substr(0, cmd_line.length() - 1);
        }
    }

    inline int _parseCommandLine(const char *cmd_line, char **args)
    {
        FUNC_ENTRY()
        int i = 0;
        std::string cmd_s = std::string(cmd_line);
        _removeBackgroundSign(cmd_s);
        std::istringstream iss(_trim(cmd_s).c_str());
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

    inline std::vector<std::string> _split(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }
        return elems;
    }

    inline std::vector<std::string> _split(const std::string &s)
    {
        std::vector<std::string> words;
        std::istringstream iss(s);
        std::string word;

        while (iss >> word)
        {
            if (word[0] == '"')
            {
                std::string quoted;
                std::getline(iss, quoted, '"');
                word += quoted;
            }
            words.push_back(word);
        }

        return words;
    }

    inline bool is_digits(const std::string &str)
    {
        return str.find_first_not_of("0123456789") == std::string::npos;
    }
}

#endif // SMASH_COMMANDUTILS_H_