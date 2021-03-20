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
    TEST_F(AnUciParser, Recognize_id_CommandWithEngineNameInformation)
    {
        ASSERT_EQ(up.parse("id name Commander Suzdal\n"), UCICMD_ID);
        ASSERT_EQ(up.cmd, UCICMD_ID);
    }



}   // namespace UciParser