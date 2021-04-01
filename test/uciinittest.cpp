#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "uciparser/uci.h"

using namespace std;
using namespace testing;

namespace UciParser
{

    const std::string lf = "\n";   // ASCII 10
    const std::string cr = "\r";   // ASCII 13
    const std::string crlf = cr + lf;
    const std::string lfcr = lf + cr;

    class AnUciParser: public Test {
        public:
            UciParser up;
    };


    // this test is necessary to complete coverage
    TEST(RTRIMFunction, ReturnFoundTrueIfTheStringIsComposedOnlyByTrimmableChars)
    {
        bool found;
        std::string str = rtrim("  \n  \t\n  \r\n", WHITESPACE+LINEENDING, found);
        ASSERT_TRUE(found);
        ASSERT_TRUE(str.size() == 0);
    }

    TEST_F(AnUciParser, ReturnNoCommandAfterInitialization)
    {
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }

    TEST_F(AnUciParser, ReturnNoCommandAWhenParsesAnEmptyString)
    {
        ASSERT_EQ(up.parse(""), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, ReturnNoCommandAWhenParsesAStringWithOnlyDelimiters)
    {
        ASSERT_EQ(up.parse(cr), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse(lf), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse(crlf), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse(lfcr), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, ReturnNoCommandAWhenParsesAnUnknownString)
    {
        ASSERT_EQ(up.parse("UnknownString"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("UnknownString\r"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("UnknownString\n"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("UnknownString\n\r"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("UnknownString\r\n"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, ReturnNoCommandWhenParses_uci_StringWithNoLineEnding)
    {
        ASSERT_EQ(up.parse("uci"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, Return_UCI_CommandWhenParsesAStringEqualTo_uci_WithLineEndings)
    {
        ASSERT_EQ(up.parse("uci" + cr), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("uci" + lf), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("uci" + crlf), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("uci" + lfcr), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
    }
    TEST_F(AnUciParser, Return_UCI_CommandWhenParsesAStringEqualTo_uci_AlsoWithCaseVariations)
    {
        ASSERT_EQ(up.parse("Uci\n"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("uCi\n"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("UCI\n"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
    }

    TEST_F(AnUciParser, Return_UCI_CommandWhenParsesAStringEqualTo_uci_AlsoLeadingAndTrailingSpaces)
    {
        ASSERT_EQ(up.parse(" uci\n"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("uci " + crlf), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("   uci  " + lf), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("\tuci\n"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("  \t  uci\t\r\n"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
    }

    TEST_F(AnUciParser, Recognize_UCI_CommandIfReceiveAStringWith_uci_WithOtherUnknownText)
    {
        ASSERT_EQ(up.parse("uCi UnknownText\n\r"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("UnknownText     uci  \n\r"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.parse("UnknownText uci OtherUnknownTest\n\r"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
    }

    TEST_F(AnUciParser, DontRecognize_UCI_CommandIfReceiveAStringContaining_uci_WithOtherUnknownTextWithoutSeparation)
    {
        ASSERT_EQ(up.parse("uciUnknownText\n\r"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("UnknownTextUci\n\r"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("xxxUCIyyy\n\r"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, WhenRecognizeAn_UCI_CommandSetNumberOfParametersToZero)
    {
        up.parse("uci\n");
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.params.size(), 0);
    }
    TEST_F(AnUciParser, WhenRecognizeAn_UCI_CommandSetNumberOfParametersToZeroAlsoIfOtherTextIsPresent)
    {
        up.parse("uci ThisIsNot AParameter\n");
        ASSERT_EQ(up.cmd, UCICMD_UCI);
        ASSERT_EQ(up.params.size(), 0);
    }

    // Please note that for the next commands we are not nagging ourselves
    // with all the line ending, whitespaces and case variations like with
    // the "uci" command, we are satisfied with the work already done previously
    TEST_F(AnUciParser, Return_UCIOK_CommandWhenParsesAStringEqualTo_uciok_WithCorrectLineEndings)
    {
        ASSERT_EQ(up.parse("uciok"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("uciok\n"), UCICMD_UCIOK);
        ASSERT_EQ(up.cmd, UCICMD_UCIOK);
    }

    // -------------------------------------------------------------------------------------
    // "id" command
    TEST_F(AnUciParser, Discard_ID_CommandWithoutAdditionalParameters)
    {
        ASSERT_EQ(up.parse("id\n"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, Recognize_ID_NAME_Command)
    {
        ASSERT_EQ(up.parse("id name cmdsuzdal\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);

        // An "id name ..." command has exactly one parameter with the format:
        //      id name <engine_name>
        // where <engine_name> is a string composed by any number of words
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("name") != up.params.end());
        ASSERT_EQ(up.params["name"], "cmdsuzdal");
    }
    TEST_F(AnUciParser, Recognize_ID_NAME_CommandWithMultiWordEngineName)
    {
        ASSERT_EQ(up.parse("id name commander suzdal\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("name") != up.params.end());
        ASSERT_EQ(up.params["name"], "commander suzdal");
    }
    TEST_F(AnUciParser, Recognize_ID_NAME_CommandWithMixedCaseEngineName)
    {
        ASSERT_EQ(up.parse("id name Commander Suzdal\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("name") != up.params.end());
        ASSERT_EQ(up.params["name"], "Commander Suzdal");
    }
    TEST_F(AnUciParser, Discard_ID_NAME_IfEngineNameIsNotSpecified)
    {
        ASSERT_EQ(up.parse("id name\n"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, Recognize_ID_NAME_CommandAlsoIf_ID_CommandIsUppercase)
    {
        ASSERT_EQ(up.parse("ID name Commander Suzdal\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("name") != up.params.end());
        ASSERT_EQ(up.params["name"], "Commander Suzdal");
    }
    TEST_F(AnUciParser, Recognize_ID_NAME_CommandAlsoIf_ID_CommandAndKeywordIsMixedCase)
    {
        ASSERT_EQ(up.parse("iD NaMe Commander Suzdal\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("name") != up.params.end());
        ASSERT_EQ(up.params["name"], "Commander Suzdal");
    }
    TEST_F(AnUciParser, Recognize_ID_NAME_CommandAlsoIfAdditionalWhiteSpacesArePresent)
    {
        ASSERT_EQ(up.parse("id name\t   \t  Commander      Suzdal 42\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("name") != up.params.end());
        ASSERT_EQ(up.params["name"], "Commander      Suzdal 42");
    }

    TEST_F(AnUciParser, Recognize_ID_AUTHOR_Command)
    {
        ASSERT_EQ(up.parse("id author rodmcban\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);

        // An "id author ..." command has exactly one parameter with the format:
        //      id author <author_name>
        // where <author_name> is a string composed by any number of words
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("author") != up.params.end());
        ASSERT_EQ(up.params["author"], "rodmcban");
    }
    TEST_F(AnUciParser, Recognize_ID_AUTHOR_CommandWithMultiWordAuthorName)
    {
        ASSERT_EQ(up.parse("id Author Rod McBan\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);
        ASSERT_EQ(up.params.size(), 1);
        ASSERT_TRUE(up.params.find("author") != up.params.end());
        ASSERT_EQ(up.params["author"], "Rod McBan");
    }
    TEST_F(AnUciParser, Discard_ID_AUTHOR_IfAuthorNameIsNotSpecified)
    {
        ASSERT_EQ(up.parse("id author\n"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    // -------------------------------------------------------------------------------------
    TEST_F(AnUciParser, Return_ISREADY_CommandWhenParsesAStringEqualTo_isready_WithCorrectLineEndings)
    {
        ASSERT_EQ(up.parse("isready"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("isready\n"), UCICMD_ISREADY);
    }
    TEST_F(AnUciParser, Return_ISREADY_CommandWhenParsesAStringEqualTo_isready_WithAlsoIfOtherUnknownTextIsPresent)
    {
        ASSERT_EQ(up.parse("abba-dingo ISREADY alpha ralpha boulevard\n"), UCICMD_ISREADY);
    }
    TEST_F(AnUciParser, Return_READYOK_CommandWhenParsesAStringEqualTo_readyok_WithCorrectLineEndings)
    {
        ASSERT_EQ(up.parse("readyok"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("readyok\n"), UCICMD_READYOK);
    }
    TEST_F(AnUciParser, Return_READYOK_CommandWhenParsesAStringEqualTo_readyok_WithAlsoIfOtherUnknownTextIsPresent)
    {
        ASSERT_EQ(up.parse("abba-dingo readyok READYOK ready_OK alpha ralpha boulevard\n"), UCICMD_READYOK);
    }
    // -------------------------------------------------------------------------------------
    TEST_F(AnUciParser, Return_UCINEWGAME_CommandWhenParsesAStringEqualTo_ucinewgame_WithCorrectLineEndings)
    {
        ASSERT_EQ(up.parse("ucinewgame"), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.parse("ucinewgame\n"), UCICMD_UCINEWGAME);
        ASSERT_EQ(up.parse("UciNewGame\n"), UCICMD_UCINEWGAME);
        ASSERT_EQ(up.parse("UCINEWGAME\n"), UCICMD_UCINEWGAME);
    }


}   // namespace UciParser