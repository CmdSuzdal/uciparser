#include "uciparser/uci.h"

namespace UciParser
{

    unsigned int UciParser::parse(std::string str)
    {
        if (str == "uci")
            cmd = UCICMD_UCI;
        return cmd;
    }

}   // namespace UciParser