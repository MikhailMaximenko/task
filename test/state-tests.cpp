#include "state.h"
#include "event.h"
#include "types.h"
#include <gtest/gtest.h>

TEST(correctness, empty_state_close) {
    computer_club::state st(2, computer_club::club_time(10, 10), computer_club::club_time(12, 12), 10);
    EXPECT_EQ(st.close().size(), 0);
}

TEST(correctness, ok_execution_test) {
    computer_club::club_time t(10, 11);
    computer_club::state st(2, computer_club::club_time(10, 10), computer_club::club_time(12, 12), 10);
    
    auto e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"mike"});
    EXPECT_FALSE(st.proccess_event(e));
    
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"george"});
    EXPECT_FALSE(st.proccess_event(e));
    
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"kirill"});
    EXPECT_FALSE(st.proccess_event(e));
    

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_SET_IN, {"mike", "1"});
    EXPECT_FALSE(st.proccess_event(e));
    
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_SET_IN, {"george", "2"});
    EXPECT_FALSE(st.proccess_event(e));

    t.t_min = 14;

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_AWAITS_IN, {"kirill"});
    EXPECT_FALSE(st.proccess_event(e));

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_LEFT_IN, {"kirill"});
    EXPECT_FALSE(st.proccess_event(e));

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_LEFT_IN, {"mike"});
    EXPECT_FALSE(st.proccess_event(e));
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_LEFT_IN, {"george"});
    EXPECT_FALSE(st.proccess_event(e));
    

    EXPECT_EQ(st.close().size(), 0);

    auto res = st.calculate_results();
    computer_club::club_time rt(1);
    EXPECT_EQ(res.size(), 2);
    EXPECT_EQ(res[0].first, 10);
    EXPECT_FALSE((res[0].second <=> rt) == 0);
    EXPECT_EQ(res[1].first, 10);
    EXPECT_FALSE((res[1].second <=> rt) == 0);
}


TEST(correctness, sneaky_clients_execution_test) {
    computer_club::club_time t(10, 11);
    computer_club::state st(2, computer_club::club_time(10, 12), computer_club::club_time(12, 12), 10);
    auto error_id = computer_club::base_event::event_id::ERROR_OUT;
    
    auto e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"mike"});
    auto res = st.proccess_event(e);
    EXPECT_TRUE(res);
    EXPECT_EQ(res->get_id(), error_id);
    EXPECT_EQ((*res)[0], "NotOpenYet");
    
    t.t_min = 13;

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"george"});
    EXPECT_FALSE(st.proccess_event(e));


    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"mike"});
    res = st.proccess_event(e);
    EXPECT_FALSE(res);

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"mike"});
    res = st.proccess_event(e);
    EXPECT_TRUE(res);
    EXPECT_EQ(res->get_id(), error_id);
    EXPECT_EQ((*res)[0], "YouShallNotPass");

    
    
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_SET_IN, {"mike", "1"});
    EXPECT_FALSE(st.proccess_event(e));
    
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_SET_IN, {"kirill", "2"});
    res = st.proccess_event(e);
    EXPECT_TRUE(res);
    EXPECT_EQ(res->get_id(), error_id);
    EXPECT_EQ((*res)[0], "ClientUnknown");

    
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_SET_IN, {"george", "2"});
    EXPECT_FALSE(st.proccess_event(e));

    t.t_min = 15;

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"kirill"});
    EXPECT_FALSE(st.proccess_event(e));
    
    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_SET_IN, {"kirill", "2"});
    res = st.proccess_event(e);
    EXPECT_TRUE(res);
    EXPECT_EQ(res->get_id(), error_id);
    EXPECT_EQ((*res)[0], "PlaceIsBusy");

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"vanya"});
    EXPECT_FALSE(st.proccess_event(e));

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_AWAITS_IN, {"kirill"});
    EXPECT_FALSE(st.proccess_event(e));

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_CAME_IN, {"vanyassister"});
    EXPECT_FALSE(st.proccess_event(e));

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_AWAITS_IN, {"kirill"});
    res = st.proccess_event(e);
    EXPECT_TRUE(res);
    EXPECT_EQ(res->get_id(), computer_club::base_event::event_id::CLIENT_LEFT_OUT);
    EXPECT_EQ((*res)[0], "kirill");

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_AWAITS_IN, {"kirill"});
    res = st.proccess_event(e);
    EXPECT_TRUE(res);
    EXPECT_EQ(res->get_id(), error_id);
    EXPECT_EQ((*res)[0], "ClientUnknown");

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_LEFT_IN, {"mike"});
    EXPECT_TRUE(res = st.proccess_event(e));
    EXPECT_EQ(res->get_id(),  computer_club::base_event::event_id::CLIENT_SET_OUT);
    EXPECT_EQ((*res)[0], "vanya");
    EXPECT_EQ((*res)[1], "1");

    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_SET_IN, {"vanya", "1"});
    res = st.proccess_event(e);
    EXPECT_TRUE(res);
    EXPECT_EQ(res->get_id(), error_id);
    EXPECT_EQ((*res)[0], "PlaceIsBusy");




    e = computer_club::base_event(t, computer_club::base_event::event_id::CLIENT_LEFT_IN, {"george"});
    EXPECT_TRUE(res = st.proccess_event(e));
    EXPECT_EQ(res->get_id(),  computer_club::base_event::event_id::CLIENT_SET_OUT);
    EXPECT_EQ((*res)[0], "vanyassister");
    EXPECT_EQ((*res)[1], "2");

    EXPECT_EQ(st.close().size(), 2);

    auto res1 = st.calculate_results();
    computer_club::club_time rt(119);
    EXPECT_EQ(res1.size(), 2);
    EXPECT_EQ(res1[0].first, 10 + 20);
    EXPECT_TRUE((res1[0].second <=> rt) == 0);
    EXPECT_EQ(res1[1].first, 10 + 20);
    EXPECT_TRUE((res1[1].second <=> rt) == 0);
}