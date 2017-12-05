#include "catch.hpp"
#include "LineBoundaries.h"
#include <Buffer.h>

using namespace __gnu_cxx;

TEST_CASE ( "lengthen/insert" ) {
    crope rope;
    LineBoundaries lineBoundaries(128, rope.begin(), rope.end());

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
    LineBoundaries lineBoundaries(128, rope.begin(), rope.end());

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
    REQUIRE(lineBoundaries.last_index() == 2);
}

TEST_CASE( "capacity" ) {
    auto *fcp = new Buffer::file_char_prod("../core/test/files/war_and_peace.txt");
    crope rope(fcp, fcp->len(), true);
    LineBoundaries line_boundaries(8, rope.begin(), rope.end());
    line_boundaries.expand_boundary(rope, LineBoundaries::Dir::END, 7);

    REQUIRE(line_boundaries.first_index() == 0);
    REQUIRE(line_boundaries.last_index() == 7);


    SECTION("Expanding boundary moves offset") {
        line_boundaries.expand_boundary(rope, LineBoundaries::Dir::END, 1);

        REQUIRE(line_boundaries.first_index() == 1);
        REQUIRE(line_boundaries.last_index() == 8);

        line_boundaries.expand_boundary(rope, LineBoundaries::Dir::END, 7);

        REQUIRE(line_boundaries.first_index() == 8);
        REQUIRE(line_boundaries.last_index() == 15);

    }

    SECTION("Beginning of buffer short jump") {
        line_boundaries.expand_boundary(rope, LineBoundaries::Dir::END, 4);

        REQUIRE(line_boundaries.first_index() == 4);
        REQUIRE(line_boundaries.last_index() == 11);

        // Short jump (first_index < capacity). Testing expand up
        line_boundaries.beginning_of_buffer(rope);

        REQUIRE(line_boundaries.first_index() == 0);
        REQUIRE(line_boundaries.last_index() == 7);
    }

    SECTION("Beginning of buffer long jump") {
        line_boundaries.expand_boundary(rope, LineBoundaries::Dir::END, 16);

        REQUIRE(line_boundaries.first_index() == 16);
        REQUIRE(line_boundaries.last_index() == 23);

        // Long jump (first_index >= capacity). Testing recalculation of lines
        line_boundaries.beginning_of_buffer(rope);

        REQUIRE(line_boundaries.first_index() == 0);
        REQUIRE(line_boundaries.last_index() == 7);
    }

    SECTION("End of buffer long jump") {
        line_boundaries.end_of_buffer(rope);

        REQUIRE(line_boundaries.first_index() == 66047);
        REQUIRE(line_boundaries.last_index() == 66054);
    }

}

/*
 *12345
 *1234
 *123
 */
