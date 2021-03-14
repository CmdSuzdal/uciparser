#include<bits/stdc++.h>
#include <algorithm>

#include "uciparser/uci.h"



namespace UciParser
{

    // ----------------------------------------------------------------
    // Utility functions
    // courtesy of:
    // https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
    std::string ltrim(const std::string &s, const std::string &charToTrim, bool &found)
    {
        found = false;
        size_t startToTrim = s.find_first_of(charToTrim);
        size_t start = s.find_first_not_of(charToTrim);
        if ((startToTrim != std::string::npos) && (startToTrim < start)) {
            // some of the character to remove found in leading area
            found = true;
        }
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    std::string rtrim(const std::string &s, const std::string &charToTrim, bool &found)
    {
        found = false;
        size_t startToTrim = s.find_first_of(charToTrim);
        size_t end = s.find_last_not_of(charToTrim);
        if (end == std::string::npos) {
            // everything seems to be trimmed, but to declare a trim
            // character found the string lenght shall be of size > 0
            if (s.size() > 0)
                found = true;
        }
        else {
            // something not to be trimmed found, if there is something
            // after the end, we hase some trimmable heading characters
            if (end < s.size() - 1)
                found = true;
        }
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }
    // ----------------------------------------------------------------

    // ----------------------------------------------------------------
    // UciParser class implementation
    unsigned int UciParser::parse(const std::string &str)
    {
        std::string procCmd;

        // reset command
        cmd = UCICMD_NO_COMMAND;

        // clean the string and extract the command
        procCmd = checkForNewLineAndCleanCommand(str);

        // The command string shall be processed in a case insensitive way
        transform(procCmd.begin(), procCmd.end(), procCmd.begin(), ::tolower);
        if (procCmd == "uci")
            cmd = UCICMD_UCI;
        return cmd;
    }
    // ----------------------------------------------------------------

    std::string UciParser::checkForNewLineAndCleanCommand(const std::string &str)
    {
        // The UCI specification, mandates that a command shall be terminated
        // by a newline, with the newline that "can be 0x0d or 0x0a0d or any
        // combination depending on your OS."
        // For this reason we espect that a generic command can have the
        // following format:
        //    "[<wsle>]<command_string_and_args>[<wsle>]<le>[<ws>]"
        // where [<ws>] are optional whitespaces, <le> are mandatory line endings
        // characters and [<wsle>] optional whitespaces + line endings chars mixed

        // Purpose of this function is to return the string
        // <command_string_and_args> if the newline sequence is found in
        // the correct position, or an empty string otherwise

        std::string procCmd;
        bool found;
        // First remove any leading whitespace + line endings and
        // trailing whitespaces only
        procCmd = ltrim(str, WHITESPACE+LINEENDING, found);
        procCmd = rtrim(procCmd, WHITESPACE, found);
        // At this point the command has the format
        //    "<command_string_and_args>[wsle]<newline>"

        // Remove the <newline> characters. Please note that we have to
        // be sure that at least a <newline> character (0x0a, 0x0d) is present,
        // otherwise the command is not valid
        procCmd = rtrim(procCmd, LINEENDING, found);
        if (!found) {
            // command is not valid (no line endings present)
            return "";
        }
        // At this point the command has the format
        //    "<command_string_and_args>[wsle]"
        // so remove any other trailing waste
        procCmd = rtrim(procCmd, WHITESPACE+LINEENDING, found);

        // And finally we have the command without waste
        //    "<command_string_and_args>"
        return procCmd;
    }


}   // namespace UciParser
