#if !defined CSZD_UCIPARSERMAIN_HEADER
#define CSZD_UCIPARSERMAIN_HEADER

#include <string>

namespace UciParser
{

    constexpr unsigned int UCICMD_NO_COMMAND = 0;
    constexpr unsigned int UCICMD_UCI = 1;

    struct UciParser {
        unsigned int cmd = UCICMD_NO_COMMAND;

        unsigned int parse(std::string str);
    };

} // namespace UciParser

#endif // if !defined CSZD_UCIPARSERMAIN_HEADER
