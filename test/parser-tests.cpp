#include "parser.h"
#include "event.h"
#include "types.h"
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

TEST(correctness, parser) {
    std::stringstream ss;
    computer_club::parser p(ss);
    ss << "10:10 1 a\n";
    auto e = p.parse_event();
    computer_club::club_time t(10, 10);
    EXPECT_EQ(e.get_id(), computer_club::base_event::event_id::CLIENT_CAME_IN);
    EXPECT_TRUE((e.get_time() <=> t) == 0);
    EXPECT_EQ(e.get_body().size(), 1);
    ss << "10:10 2 a 1\n";
    e = p.parse_event();
    EXPECT_EQ(e.get_id(), computer_club::base_event::event_id::CLIENT_SET_IN);
    EXPECT_TRUE((e.get_time() <=> t) == 0);
    EXPECT_EQ(e.get_body().size(), 2);
    ss << "10:10 3 a\n";
    e = p.parse_event();
    EXPECT_EQ(e.get_id(), computer_club::base_event::event_id::CLIENT_AWAITS_IN);
    EXPECT_TRUE((e.get_time() <=> t) == 0);
    EXPECT_EQ(e.get_body().size(), 1);
    ss << "10:10 4 a\n";
    e = p.parse_event();
    EXPECT_EQ(e.get_id(), computer_club::base_event::event_id::CLIENT_LEFT_IN);
    EXPECT_TRUE((e.get_time() <=> t) == 0);
    EXPECT_EQ(e.get_body().size(), 1);
    ss << "10:10 17 a\n";
    EXPECT_THROW(p.parse_event(), std::domain_error);


    ss << "10:10 1 a a a\n";
    EXPECT_THROW(p.parse_event(), std::domain_error);
    ss << "10:10 1\n";
    EXPECT_THROW(p.parse_event(), std::domain_error);
    ss << "101 1 a\n";
    EXPECT_THROW(p.parse_event(), computer_club::parse_error);
    ss << "10:\n";
    EXPECT_THROW(p.parse_event(), computer_club::parse_error);
    
}