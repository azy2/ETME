#include "catch.hpp"
#include "LineBoundaries.h"
#include <Buffer.h>

using namespace __gnu_cxx;

TEST_CASE ( "lengthen/insert" ) {
    crope rope;
    LineBoundaries lineBoundaries(rope);

    REQUIRE(lineBoundaries.at(0).start_idx == 0);
    REQUIRE(lineBoundaries.at(0).length == 0);

    lineBoundaries.lengthen(0, 5);

    REQUIRE(lineBoundaries.at(0).start_idx == 0);
    REQUIRE(lineBoundaries.at(0).length == 5);

    lineBoundaries.insert(0, 2);
    lineBoundaries.insert(0, 3);
    lineBoundaries.insert(0, 4);

    REQUIRE(lineBoundaries.at(0).start_idx == 0);
    REQUIRE(lineBoundaries.at(0).length == 5);
    REQUIRE(lineBoundaries.at(1).start_idx == 6);
    REQUIRE(lineBoundaries.at(1).length == 4);
    REQUIRE(lineBoundaries.at(2).start_idx == 11);
    REQUIRE(lineBoundaries.at(2).length == 3);
    REQUIRE(lineBoundaries.at(3).start_idx == 15);
    REQUIRE(lineBoundaries.at(3).length == 2);
}

TEST_CASE ("remove_line") {
    crope rope;
    LineBoundaries lineBoundaries(rope);

    lineBoundaries.lengthen(0, 5);
    lineBoundaries.insert(0, 10);
    lineBoundaries.insert(0, 5);
    lineBoundaries.insert(0, 5);

    REQUIRE(lineBoundaries.at(0).start_idx == 0);
    REQUIRE(lineBoundaries.at(0).length == 5);
    REQUIRE(lineBoundaries.at(1).start_idx == 6);
    REQUIRE(lineBoundaries.at(1).length == 5);
    REQUIRE(lineBoundaries.at(2).start_idx == 12);
    REQUIRE(lineBoundaries.at(2).length == 5);
    REQUIRE(lineBoundaries.at(3).start_idx == 18);
    REQUIRE(lineBoundaries.at(3).length == 10);

    lineBoundaries.remove_line(0);

    REQUIRE(lineBoundaries.at(0).start_idx == 0);
    REQUIRE(lineBoundaries.at(0).length == 5);
    REQUIRE(lineBoundaries.at(1).start_idx == 6);
    REQUIRE(lineBoundaries.at(1).length == 5);
    REQUIRE(lineBoundaries.at(2).start_idx == 12);
    REQUIRE(lineBoundaries.at(2).length == 10);
    REQUIRE(lineBoundaries.size() == 3);
}
