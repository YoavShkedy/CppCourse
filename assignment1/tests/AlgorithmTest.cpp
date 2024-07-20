#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Algorithm.h"

using namespace std;
using namespace testing;

class MockDirtSensor : public DirtSensor {
public:
    MOCK_METHOD(int, get_dirt_level, (), (const, override));
};

class MockWallSensor : public WallSensor {
public:
    MOCK_METHOD((map<string, bool>), surrounding_walls, (), (const, override));
};

class MockBatterySensor : public BatterySensor {
public:
    MOCK_METHOD(int, get_battery_level, (), (const, override));
};

class AlgorithmTest : public ::testing::Test {
protected:
    MockDirtSensor mockDirtSensor;
    MockWallSensor mockWallSensor;
    MockBatterySensor mockBatterySensor;
    Algorithm algorithm;

    AlgorithmTest() : algorithm(make_pair(0, 0), 100) {}

    void SetUp() override {
        algorithm.set_sensors(&mockDirtSensor, &mockWallSensor, &mockBatterySensor);
    }
};

TEST_F(AlgorithmTest, TestMoveToCleanDirt) {
    EXPECT_CALL(mockDirtSensor, get_dirt_level())
            .WillOnce(Return(5));
    EXPECT_CALL(mockBatterySensor, get_battery_level())
            .WillOnce(Return(10));

    string action = algorithm.get_next_move();
    EXPECT_EQ(action, "Stay");
}

TEST_F(AlgorithmTest, TestRandomMovement) {
    EXPECT_CALL(mockDirtSensor, get_dirt_level())
            .WillRepeatedly(Return(0));
    EXPECT_CALL(mockWallSensor, surrounding_walls())
            .WillRepeatedly(Return(map<string, bool>{{"North", false},
                                                     {"South", false},
                                                     {"East",  false},
                                                     {"West",  false}}));
    EXPECT_CALL(mockBatterySensor, get_battery_level())
            .WillRepeatedly(Return(std::numeric_limits<int>::max()));

    string action = algorithm.get_next_move();
    EXPECT_TRUE(action == "North" || action == "South" || action == "East" || action == "West");
}
