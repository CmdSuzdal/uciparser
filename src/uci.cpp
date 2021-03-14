#include<bits/stdc++.h>
#include <algorithm>

#include "uciparser/uci.h"



namespace UciParser
{

    // ----------------------------------------------------------------
    // Utility functions
    // courtesy of:
    // https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
    const std::string WHITESPACE = " \n\r\t\f\v";

    std::string ltrim(const std::string& s)
    {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }
    std::string rtrim(const std::string& s)
    {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }
    std::string trim(const std::string& s)
    {
        return rtrim(ltrim(s));
    }
    // ----------------------------------------------------------------

    // ----------------------------------------------------------------
    // UciParser class implementation
    unsigned int UciParser::parse(std::string str)
    {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        if (trim(str) == "uci")
            cmd = UCICMD_UCI;
        return cmd;
    }
    // ----------------------------------------------------------------

}   // namespace UciParser
