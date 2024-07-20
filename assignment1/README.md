## Authors
- Yoav Shkedy (ID 314939398)
- Eitamar Ben Shushan (ID 206448649)

## Overview
The approach to creating the vacuum cleaner program involves designing a system with clear separation of responsibilities among classes. The House class models the environment, including walls and dirt levels. The VacuumCleaner class represents the robotic vacuum, managing its state, movement, and interactions with the house. The Algorithm class handles decision-making based on inputs from various sensors (DirtSensor, WallSensor, and BatterySensor). The vacuum cleaner operates by iteratively invoking the algorithm to determine the next action (move, clean, or charge) until the mission is completed or the maximum steps are reached. This modular design ensures flexibility, maintainability, and clear abstraction of functionalities.

## Input File Structure
The input file should be a txt file with the following structure:

1. The first two lines specify the maximum steps and the maximum battery steps:
    ```
    max_steps = <number_1>
    max_battery_steps = <number_2>
    ```
    - `max_steps`: The maximum number of steps allowed for the vacuum cleaner to complete the mission.
    - `max_battery_steps`: The maximum number of steps the vacuum cleaner can take on a single charge.


2. The remaining lines define the layout of the house:
    - The house layout consists of rows containing numbers `1-9`, `#`, `D`, and blank spaces `( )`.
    - Each `#` represents a wall in the house.
    - Each number `1-9` represents the dirt level at that specific position.
    - Each blank space represents a clean position in the house.
    - `D` represents the location of the vacuum cleaner's docking station where it recharges and returns to at the end of the mission. NOTE: there must be only a single docking station in the house.

### Example Input Files
1. Example 1:
```
max_steps = 1000
max_battery_steps = 99

###########
#2 345 9 1#
# 1 1 3 2 #
# 13444  1#
# 1271 222#
#9 367 125#
#D  2 188 #
###########
```

In this example:
- `max_steps` is set to 1000.
- `max_battery_steps` is set to 99.
- The house layout includes all 4 walls (`#`), dirt levels (`1-9`), and a docking station (`D`) at the South-West corner of the house.

1. Example 2:
```
max_steps = 2500
max_battery_steps = 150

###############
#2 345 9 1# 2 #
# 1 1 3   # 3 #
 13444  1 #####
 12
9 367 125#
#  2 188 D
```

In this example:
- `max_steps` is set to 2500.
- `max_battery_steps` is set to 150.
- The house layout includes only the North wall of the house and an inner room the vacuum cleaner will not be able to get into (`#`), dirt levels (`1-9`), and a docking station (`D`).

### Output File Structure

The output file is a text file containing the following data:

1. Mission: `<mission_status>` - Can be either `Success` or `Failure`.
2. Total number of steps: `<total_steps>` - This is the total number of steps taken by the vacuum cleaner during the mission.
3. Total dirt left: `<dirt_amount>` - The total amount of dirt left in the house.
4. Battery exhausted: `<bool>` - Indicates whether the battery is exhausted at the end of the mission. Can be `True` or `False`.
5. Steps log: `<steps_log>` - A log of all the steps taken by the vacuum cleaner during the mission, with each step in a separate line. The steps can be:
   - `Move: <direction>` - Indicates the vacuum cleaner moved in a specific direction (`N`, `E`, `S`, `W`).
   - `Clean` - Indicates the vacuum cleaner stayed in place and cleaned its current position.
   - `Charge` - Indicates the vacuum cleaner is on the docking station and is charging.

### Example Output File
```
Mission: Success
Total number of steps: 367
Total dirt left: 0
Battery exhausted: False
Steps log:
```