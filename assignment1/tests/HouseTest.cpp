#include <gtest/gtest.h>
#include "../include/House.h"

// Create a test fixture class for House
class HouseTest : public ::testing::Test {
protected:
    House house_1;
    House house_2;
    House house_3;

    HouseTest()
        : house_1("../tests_input/test_input_1.txt"), house_2("../tests_input/test_input_2.txt"), house_3("../tests_input/test_input_3.txt") {}

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};



// Test the constructor and init_house function
TEST_F(HouseTest, LoadHouse) {
//house_1
EXPECT_EQ(house_1.get_max_steps_allowed(), 1000);
EXPECT_EQ(house_1.get_max_battery_steps(), 99);
EXPECT_EQ(house_1.get_docking_station(), std::make_pair(6, 1));

//house_2
EXPECT_EQ(house_2.get_max_steps_allowed(), 1200);
EXPECT_EQ(house_2.get_max_battery_steps(), 100);
EXPECT_EQ(house_2.get_docking_station(), std::make_pair(5, 9));

//house_3
EXPECT_EQ(house_3.get_max_steps_allowed(), 7000);
EXPECT_EQ(house_3.get_max_battery_steps(), 60);
EXPECT_EQ(house_3.get_docking_station(), std::make_pair(1, 7));
}

// Test total_dirt
TEST_F(HouseTest, TotalDirt) {
//house_1
EXPECT_EQ(house_1.get_total_dirt(), 117);
//house_2
EXPECT_EQ(house_2.get_total_dirt(), 93);
//house_3
EXPECT_EQ(house_3.get_total_dirt(), 87);
}

// Test get_dirt_level
TEST_F(HouseTest, GetDirtLevel) {
EXPECT_EQ(house_1.get_dirt_level(1, 1), 2);
EXPECT_EQ(house_2.get_dirt_level(3, 3), 3);
EXPECT_EQ(house_3.get_dirt_level(0, 0), 0); // (0,0) is always a wall
}

// Test decrease_dirt
TEST_F(HouseTest, DeacreaseDirt) {
//house_1
house_1.decrease_dirt(1, 1);
EXPECT_EQ(house_1.get_dirt_level(1, 1), 1);
//house_2
house_2.decrease_dirt(3, 4);
EXPECT_EQ(house_2.get_dirt_level(3, 4), 3);
//house_3
house_3.decrease_dirt(5, 8);
EXPECT_EQ(house_3.get_dirt_level(5, 8), 1);
}

// Test decrease_total_dirt functions
TEST_F(HouseTest, DecreaseTotalDirt) {
//house_1
int initial_dirt_1 = house_1.get_total_dirt();
house_1.decrease_dirt(1, 1);
EXPECT_EQ(house_1.get_total_dirt(), initial_dirt_1 - 1);
house_1.decrease_dirt(1, 1);
EXPECT_EQ(house_1.get_total_dirt(), initial_dirt_1 - 2);
}

// Test is_wall function
TEST_F(HouseTest, IsWall) {
EXPECT_TRUE(house_1.is_wall(0, 0));
EXPECT_FALSE(house_2.is_wall(1, 4));
EXPECT_FALSE(house_3.is_wall(1, 7)); // (1,7) = docking station
}

// Test get_layout function
TEST_F(HouseTest, GetLayout) {
// house_1
auto layout_1 = house_1.get_layout();
EXPECT_EQ(layout_1.size(), 8);
EXPECT_EQ(layout_1[0].size(), 11);
// house_2
auto layout_2 = house_2.get_layout();
EXPECT_EQ(layout_2.size(), 9);
EXPECT_EQ(layout_2[0].size(), 11);
// house_3
auto layout_3 = house_3.get_layout();
EXPECT_EQ(layout_3.size(), 8);
EXPECT_EQ(layout_3[0].size(), 11);
}

// Test print_house function
TEST_F(HouseTest, PrintHouse) {
testing::internal::CaptureStdout();
house_1.print_house();
std::string output = testing::internal::GetCapturedStdout();

std::string expected_output =
        "###########\n"
        "#2 345 9 1#\n"
        "# 1 1 3 2 #\n"
        "# 13444  1#\n"
        "# 1271 222#\n"
        "#9 367 125#\n"
        "#D  2 188 #\n"
        "###########\n";

EXPECT_EQ(output, expected_output);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

