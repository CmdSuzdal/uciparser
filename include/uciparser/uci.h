#if !defined CSZD_UCIPARSERMAIN_HEADER
#define CSZD_UCIPARSERMAIN_HEADER

namespace UciParser
{

    constexpr unsigned int UCI_NO_COMMAND = 0;

    struct UciParser {
        unsigned int cmd = UCI_NO_COMMAND;
    };

} // namespace UciParser

#endif // if !defined CSZD_UCIPARSERMAIN_HEADER
