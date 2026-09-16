#include <catch2/catch_test_macros.hpp>

#include "bml-dsp/circular-buffer.h"

namespace BML
{
    TEST_CASE("Circular Buffer")
    {
        SECTION("Basic Buffer")
        {
            CircularBuffer circBuff(3);
            circBuff.write(2.1);
            circBuff.write(25.21);
            circBuff.write(5.72);

            // 2.1, 25.21, 5.72
            REQUIRE(circBuff.read() == 2.1);
            REQUIRE(circBuff.read() == 25.21);
            REQUIRE(circBuff.read() == 5.72);
            REQUIRE(circBuff.read() == 2.1);

            circBuff.write(78.2);
            // 78.2, 25.21, 5.72
            REQUIRE(circBuff.read() == 25.21);
            REQUIRE(circBuff.read() == 5.72);

            circBuff.write(-9.4);
            // // 78.2, -9.4, 5.72
            REQUIRE(circBuff.read() == 78.2);
            REQUIRE(circBuff.read() == -9.4);
            REQUIRE(circBuff.read() == 5.72);
        }

        SECTION("More Complicated Buffer -- Vector writing and reading")
        {
            CircularBuffer circBuff(5);
            std::vector<double> valuesToWrite({10.3, 1.5, 19.3, -1.4, 934.1});
            circBuff.write(valuesToWrite);
            // 10.3RW, 1.5, 19.3, -1.4, 934.1
            
            std::vector<double> out = circBuff.read(3);
            REQUIRE(out == std::vector<double>({10.3, 1.5, 19.3}));
            // 10.3W, 1.5, 19.3, -1.4R, 934.1

            valuesToWrite = std::vector<double>({-14.0, 12.3});
            circBuff.write(valuesToWrite);

            // -14.0, 12.3, 19.3W, -1.4R, 934.1
            out = circBuff.read(3);
            REQUIRE(out == std::vector<double>({-1.4, 934.1, -14.0}));
            REQUIRE(circBuff.read() == 12.3);
            // -14.0, 12.3, 19.3WR, -1.4, 934.1

            valuesToWrite = std::vector<double>({1.4, 10.2, 154.0, 103.1, 103.5, 60.0});
            circBuff.write(valuesToWrite);
            // 103.1, 103.5, 60.0R, 10.2W, 154.0

            out = circBuff.read(7);
            std::vector<double> shouldBe({60.0, 10.2, 154.0, 103.1, 103.5, 60.0, 10.2});
            REQUIRE(out == shouldBe);
        }

        SECTION("Read New Values in the Buffer")
        {
            CircularBuffer circBuff(5);
            std::vector<double> valuesToWrite({10.3, 1.5, 19.3, -1.4});
            circBuff.write(valuesToWrite);
            REQUIRE(circBuff.readNew() == valuesToWrite);

            valuesToWrite = std::vector<double>({10.3, 82.1, 94.3});
            circBuff.write(valuesToWrite);
            REQUIRE(circBuff.readNew() == valuesToWrite);

            valuesToWrite = std::vector<double>({8.1, 0.5});
            circBuff.write(valuesToWrite);
            REQUIRE(circBuff.readNew() == valuesToWrite);

            valuesToWrite = std::vector<double>({8.4, 9.2, 10.8, -90.1, 103.2});
            circBuff.write(valuesToWrite);
            REQUIRE(circBuff.readNew().size() == 0);  // Fully lapped, so the buffer thinks no new data has come in.

            valuesToWrite = std::vector<double>({8.4, 9.2, -90.1, 103.2});
            circBuff.write(valuesToWrite);
            REQUIRE(circBuff.readNew() == valuesToWrite);        
        }
    }
}
