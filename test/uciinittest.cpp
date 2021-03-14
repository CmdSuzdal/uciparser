#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "uciparser/uci.h"

using namespace std;
using namespace testing;

namespace UciParser
{

    class AnUciParser: public Test {
        public:
            UciParser up;
    };

    TEST_F(AnUciParser, ReturnNoCommandAfterInitialization)
    {
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }

    TEST_F(AnUciParser, ReturnNoCommandAWhenParsesAnEmptyString)
    {
        ASSERT_EQ(up.parse(""), UCICMD_NO_COMMAND);
        ASSERT_EQ(up.cmd, UCICMD_NO_COMMAND);
    }
    TEST_F(AnUciParser, Return_UCI_CommandWhenParsesAStringEqualTo_uci_)
    {
        ASSERT_EQ(up.parse("uci"), UCICMD_UCI);
        ASSERT_EQ(up.cmd, UCICMD_UCI);
    }

}   // namespace UciParser