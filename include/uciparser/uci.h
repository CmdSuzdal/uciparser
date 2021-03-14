#if !defined CSZD_UCIPARSERMAIN_HEADER
#define CSZD_UCIPARSERMAIN_HEADER

#include <string>

namespace UciParser
{
    const std::string WHITESPACE = " \t\f\v";
    const std::string LINEENDING = "\n\r";
    std::string ltrim(const std::string &s, const std::string &charToTrim, bool &found);
    std::string rtrim(const std::string &s, const std::string &charToTrim, bool &found);

    enum UciCommand : unsigned int
    {
        UCICMD_NO_COMMAND = 0,
        UCICMD_UCI,
        UCICMD_UCIOK
    };

    struct UciParser {
        UciCommand cmd = UCICMD_NO_COMMAND;

        UciCommand parse(const std::string &str);
    private:
        std::string checkForNewLineAndCleanCommand(const std::string &str);
    };

} // namespace UciParser

#endif // if !defined CSZD_UCIPARSERMAIN_HEADER
