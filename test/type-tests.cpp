#include "types.h"

#include <gtest/gtest.h>
#include <ctime>
#include <iomanip>
#include <stdexcept>
TEST(correctness, equal_times) {
    computer_club::club_time t1(10, 11);
    computer_club::club_time t2(611);
    std::stringstream s("10:11");
    std::tm t3;
    s >> std::get_time(&t3, computer_club::util_values::time_format);
    EXPECT_TRUE((t1 <=> t2) == 0);
    EXPECT_TRUE((t2 <=> t3) == 0);
    EXPECT_TRUE((t1 <=> t3) == 0);
}

TEST(correctness, other_time_comparisons) {
    computer_club::club_time t1(10, 11);
    computer_club::club_time t2(6);
    std::stringstream s("11:11");
    std::tm t3;
    computer_club::club_time t4(10, 12);
    s >> std::get_time(&t3, computer_club::util_values::time_format);
    EXPECT_TRUE((t1 <=> t2) > 0);
    EXPECT_TRUE((t2 <=> t3) < 0);
    EXPECT_TRUE((t1 <=> t3) < 0);
    EXPECT_TRUE((t1 <=> t4) < 0);
    EXPECT_TRUE((t2 <=> t4) < 0);
    EXPECT_TRUE((t3 <=> t4) > 0);
}

TEST(correctness, time_operations) {
    computer_club::club_time t1(10, 11);
    computer_club::club_time t2(10, 11);
    computer_club::club_time t3(7, 5);
    computer_club::club_time t4(20, 22);
    computer_club::club_time t5(3, 6);
    EXPECT_TRUE(((t1 += t2) <=> t4) == 0);
    EXPECT_THROW(t1 += t2, std::domain_error);
    EXPECT_TRUE(((t2 - t3) <=> t5) == 0);
    EXPECT_THROW(t3 - t2, std::domain_error);
    EXPECT_EQ(t2.to_string(), "10:11");
    EXPECT_EQ(t3.to_string(), "07:05");
    EXPECT_EQ(t4.to_string(), "20:22");
    EXPECT_EQ(t5.to_string(), "03:06");
    EXPECT_EQ(t2.to_mins(), 611);
    EXPECT_EQ(t3.to_mins(), 425);
}

TEST(correctness, table_operations) {
    computer_club::table table(1);
    computer_club::club_time t1(10, 11);
    table.take(t1);
    computer_club::club_time t2(11, 12);
    
    table.free(t2, 10);
    EXPECT_EQ(table.get_money(), 20);
    EXPECT_TRUE((table.total_time() <=> computer_club::club_time(1, 1)) == 0);
}

