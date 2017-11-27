#include "catch.hpp"
#include "Buffer.h"
#include <iostream>

using namespace std;

TEST_CASE( "insert" ) {
    Buffer buffer;
    buffer.insert('a');
    REQUIRE(buffer.get_rope().compare(crope("a")) == 0);
    buffer.insert("bcd");
    REQUIRE(buffer.get_rope().compare(crope("abcd")) == 0);
}

TEST_CASE( "get_x" ) {
    Buffer buffer;
    buffer.insert("abcd");
    REQUIRE(buffer.get_x() == 4);
    buffer.insert("\nabc");
    REQUIRE(buffer.get_x() == 3);
    REQUIRE(buffer.cursor.start == 9);
}

TEST_CASE( "get_lines" ) {
    Buffer buffer;
    buffer.insert("12345\n1234\n123\n12\n");
    auto lines = buffer.get_lines(0, 4);

    REQUIRE( lines.at(0).rope.compare(crope("12345")) == 0 );
    REQUIRE( lines.at(0).start_idx == 0 );
    REQUIRE( lines.at(1).rope.compare(crope("1234")) == 0);
    REQUIRE( lines.at(1).start_idx == 6 );
    REQUIRE( lines.at(2).rope.compare(crope("123")) == 0);
    REQUIRE( lines.at(2).start_idx == 11 );
    REQUIRE( lines.at(3).rope.compare(crope("12")) == 0);
    REQUIRE( lines.at(3).start_idx == 15 );
}
