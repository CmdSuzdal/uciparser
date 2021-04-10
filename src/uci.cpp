#include<bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

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
    UciCommand UciParser::parse(const std::string &str)
    {
        std::string procCmd;

        // reset command
        cmd = UCICMD_NO_COMMAND;

        // Removes all elements in the parameter map
        params.clear();

        // clean the string and extract the command
        procCmd = checkForNewLineAndCleanCommand(str);

        // Split the received command in tokens
        std::istringstream iss(procCmd);
        std::vector<std::string> tokens {
            std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{}};

        // find the first token that is recognized as a command
        for (auto ndx = 0; ndx < tokens.size(); ndx++) {
            // The command string shall be processed in a case insensitive way
            transform(tokens[ndx].begin(), tokens[ndx].end(),
                                tokens[ndx].begin(), ::tolower);
            if (tokens[ndx] == "uci") {
                cmd = UCICMD_UCI;
                break;
            }
            else if (tokens[ndx] == "uciok") {
                cmd = UCICMD_UCIOK;
                break;
            }
            else if (tokens[ndx] == "id") {
                cmd = parseIdCommand(procCmd, tokens, ndx);
                break;
            }
            else if (tokens[ndx] == "isready") {
                cmd = UCICMD_ISREADY;
                break;
            }
            else if (tokens[ndx] == "readyok") {
                cmd = UCICMD_READYOK;
                break;
            }
            else if (tokens[ndx] == "ucinewgame") {
                cmd = UCICMD_UCINEWGAME;
                break;
            }
            else if (tokens[ndx] == "position") {
                cmd = parsePositionCommand(procCmd, tokens, ndx);
                break;
            }
        }
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

    // ----------------------------------------------------------------
    UciCommand UciParser::parseIdCommand(const std::string &cmd,
                        std::vector<std::string> &tokens, int ndx)
    {
        // The token in position ndx has been recognized as "id".
        //
        // An id command - to be valid - shall be followed by one of
        // the following parameters:
        //   - name <engine_name>
        //   - author <author_name>
        //
        // Both <engine_name> and <author_name> can be composed by any text
        //
        // Note that an "id" command alone is not valid

        // First: the command shall have at least 2 additional tokens
        if (tokens.size() <= ndx + 2)
            return UCICMD_NO_COMMAND;

        // Recognize "name" parameter (case insensitive)
        transform(tokens[ndx+1].begin(), tokens[ndx+1].end(),
                                tokens[ndx+1].begin(), ::tolower);
        if (tokens[ndx+1] == "name") {
            // "id name" command
            // anything following "name" is the engine name;
            // we try to preserve the format of the string following "name"
            params["name"] = cmd.substr(cmd.find(tokens[ndx+2]));
        }
        else if (tokens[ndx+1] == "author") {
            // "id author" command
            // anything following "author" is the author name;
            // we try to preserve the format of the string following "name"
            params["author"] = cmd.substr(cmd.find(tokens[ndx+2]));
        }
        return UCICMD_ID;
    }
    // ----------------------------------------------------------------
    UciCommand UciParser::parsePositionCommand(const std::string &cmd,
                        std::vector<std::string> &tokens, int ndx)
    {
        // The token in position ndx has been recognized as "position".
        //
        // A position command - to be valid - shall have one of the
        // following sintax:
        //   - startpos [sequence of moves]
        //   - fen <fenstring> [sequence of moves]
        //
        // Note that a "position" command alone is not valid

        // First: the command shall have at least 1 additional tokens
        if (tokens.size() <= ndx + 1)
            return UCICMD_NO_COMMAND;

        // Recognize "startpos" parameter (case insensitive)
        transform(tokens[ndx+1].begin(), tokens[ndx+1].end(),
                       tokens[ndx+1].begin(), ::tolower);
        if (tokens[ndx+1] == "startpos") {
            // "id startpos" command
            // anything following "startpos" is an optional
            // sequence of moves
            params["position_mode"] = "startpos";
        }
        else {
            return UCICMD_NO_COMMAND;
        }

        // if here, a valid position command has been recognized,
        // checks for optional moves specification
        if (tokens.size() > ndx + 2) {
            if (tokens[ndx+2] == "moves") {
                // everything follows is a sequences of moves
                params["moves"] = "";
                for (auto mndx = ndx+3; mndx < tokens.size(); mndx++) {
                    if (mndx > ndx+3)
                        params["moves"] += " ";
                    params["moves"] += tokens[mndx];
                }
            }
        }
        return UCICMD_POSITION;
    }

}   // namespace UciParser
