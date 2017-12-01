#include <iostream>
#include "catch.hpp"
#include "Buffer.h"

using namespace std;

TEST_CASE( "no newlines", "[insert]" ) {
    Buffer buffer;
    buffer.insert('a');
    REQUIRE(buffer.get_rope().compare(crope("a")) == 0);
    buffer.insert("bcd");
    REQUIRE(buffer.get_rope().compare(crope("abcd")) == 0);
}

TEST_CASE( "newlines", "[insert]" ) {
    Buffer buffer;
    buffer.insert("abc");
    buffer.insert('\n');
    buffer.insert("abc\nabc");
    buffer.insert("\nabc");
    REQUIRE(buffer.get_rope().compare(crope("abc\nabc\nabc\nabc")) == 0);
}

TEST_CASE( "get_x" ) {
    Buffer buffer;
    buffer.insert("abcd");
    REQUIRE(buffer.get_x() == 4);
    buffer.insert("\nabc");
    REQUIRE(buffer.get_x() == 3);
    REQUIRE(buffer.get_idx() == 8);
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

TEST_CASE( "backspace" ) {
    Buffer buffer;
    buffer.insert("1234\n5678");
    buffer.back();
    buffer.backspace();
    buffer.up();
    buffer.backspace();
    buffer.down();
    buffer.backspace();

    auto lines = buffer.get_lines(0, 2);
    REQUIRE(lines.at(0).rope.compare(crope("134")) == 0);
    REQUIRE(lines.at(1).rope.compare(crope("68")) == 0);

    buffer.backspace();
    REQUIRE(buffer.get_x() == 3);
    REQUIRE(buffer.get_y() == 0);
    auto line = buffer.get_line(0);
    REQUIRE(line.rope.compare(crope("13468")) == 0);
}
// 134
// 68

TEST_CASE( "back_word" ) {
    Buffer buffer;
    buffer.insert("word word abc");
    buffer.back_word();
    REQUIRE(buffer.get_x() == 10);
    buffer.back_word();
    REQUIRE(buffer.get_x() == 5);
    buffer.back_word();
    REQUIRE(buffer.get_x() == 0);
}

TEST_CASE( "forward_word" ) {
    Buffer buffer;
    buffer.insert("word word abc");
    buffer.beginning_of_line();
    REQUIRE(buffer.get_x() == 0);
    buffer.forward_word();
    REQUIRE(buffer.get_x() == 5);
}
