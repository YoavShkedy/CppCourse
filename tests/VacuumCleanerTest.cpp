#include <gtest/gtest.h>
#include "../include/VacuumCleaner.h"

// Create a test fixture class for VacuumCleaner
class VacuumCleanerTest : public ::testing::Test {
protected:
    House house;
    VacuumCleaner vacuum;

    VacuumCleanerTest()
            : house("../tests_input/test_input_4.txt"), vacuum(house) {}

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

// Test the constructor and initial state
TEST_F(VacuumCleanerTest, InitialState) {
    EXPECT_EQ(vacuum.get_total_steps_taken(), 0);
    EXPECT_EQ(vacuum.get_assignment_log(), "");
    EXPECT_FALSE(vacuum.battery_exhausted());
}

// Test run functionality without simulation
TEST_F(VacuumCleanerTest, Run) {
    vacuum.run();
    EXPECT_TRUE(vacuum.mission_completed(house));
    EXPECT_LE(vacuum.get_total_steps_taken(), house.get_max_steps_allowed());
}

// Test sensor functionality through the algorithm
TEST_F(VacuumCleanerTest, Sensors) {
    VacuumCleaner::Sensors sensors(vacuum);
    EXPECT_EQ(sensors.get_dirt_level(), house.get_dirt_level(house.get_docking_station().first, house.get_docking_station().second));
    EXPECT_EQ(sensors.get_battery_level(), house.get_max_battery_steps());

    auto walls = sensors.surrounding_walls();
    auto docking_station = house.get_docking_station();
    EXPECT_EQ(walls["North"], house.is_wall(docking_station.first - 1, docking_station.second));
    EXPECT_EQ(walls["South"], house.is_wall(docking_station.first + 1, docking_station.second));
    EXPECT_EQ(walls["East"], house.is_wall(docking_station.first, docking_station.second + 1));
    EXPECT_EQ(walls["West"], house.is_wall(docking_station.first, docking_station.second - 1));
}
