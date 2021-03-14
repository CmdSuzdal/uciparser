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
        ASSERT_EQ(up.cmd, UCI_NO_COMMAND);
    }

}   // namespace UciParser