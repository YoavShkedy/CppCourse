## Authors
- Yoav Shkedy (ID 314939398)
- Eitamar Ben Shushan (ID 206448649)

## Overview
This project is a program for an automatic vacuum cleaner designed to clean the house as efficiently as possible while adhering to given limitations. The vacuum cleaner's navigation decisions are made by a smart algorithm that receives information from its sensors. The vacuum cleaner's sensors include:
1. Wall Sensor: Detects walls in each direction: North, East, South, West.
2. Dirt Sensor: Detects the dirt level in the vacuum's current location.
3. Battery Sensor: Indicates the remaining battery life.

## How the Algorithm Works
As the vacuum cleaner operates, the algorithm creates a map of the house, remembering the dirt level at each location. After each recharge at the docking station, the algorithm selects the next closest point that it cannot confirm is clean. The vacuum cleaner chooses its route based on a BFS (Breadth-First Search) algorithm applied to the internal map it creates. The goal of this smart algorithm is to minimize the total dirt level in the house, rather than focusing on specific locations.

## Input File Structure
The house input file should follow these strict instructions:
- Line 1: House name/description.
- Line 2: MaxSteps for the simulation in this house.
- Line 3: MaxBattery (in steps) for the simulation in this house.
- Line 4: Number of Rows in the house.
- Line 5: Number of Columns in the house.
- Lines 6 and on: The house itself. The program should receive an input file that represents a house with walls, corridors, dirt levels, and a single docking station:
   - Empty Space (“corridors”): space ` ` as well as any character which does not have other mapping below
   - Wall: `W`
   - Dirt: Numbers from `0` to `9` should represent dirt level (`0` is the same as space ` `)
   - Docking Station: `D` (There shall be one and only one docking station otherwise the file is invalid)

### Example Input Files
1. Example 1:

```
1-Small house
MaxSteps = 100
MaxBattery=20
Rows = 4
Cols =10
WWWW      
W D
W 123456789
```

In this example:
- `MaxSteps` is set to 100.
- `MaxBattery` is set to 20.
- `Rows` is set to 4.
- `Cols` is set to 10.
- The house will be modeled by the program as follows. Note that since `Cols` = 10 then one of the rows was shortened, and since `Rows` = 4, another row has been added.


```
WWWWWWWWWWWW
WWWWW      W
WW D       W
WW 12345678W
W          W
WWWWWWWWWWWW
```

### Output File Structure
The output file is a text file containing the following data:

- `NumSteps` = `<NUMBER>`
- `DirtLeft` = `<NUMBER>`
- `Status` = `<FINISHED/WORKING/DEAD>`
- `Steps` = `<Steps>`

Elaboration:
- `<FINISHED/WORKING/DEAD>`:
   - `FINISHED` - reported when the algorithm is in the docking station and reports Finish, regardless of whether there is dirt left in the house.
   - `DEAD` - reported in the following cases:
      1. When the algorithm expires without reporting Finish because the battery is depleted, and the robot is not in the docking station (if the robot is docked, it cannot be considered as if the battery has been depleted because it is charging anyway).
      2. An algorithm that reported Finish and was not found in the docking station.
   - `WORKING` - reported when the steps quota has ended and the algorithm did not report Finish, but the battery is not empty, regardless of the position of the robot (docked/not docked).

- `<Steps>`: list of characters in one line, no spaces, from: NESWsF – the small s is for STAY, NESW are for North, East, South, West. F is for Finished - only if the algorithm actually returned “Finished”.
  Note: The number of characters in the last line of the file should be equal to the NumSteps value (if there is an F for Finished it will NOT be counted, thus NumSteps might be one less than the number of chars in the steps list).

### Example Output File

```
NumSteps = 100
DirtLeft = 21
Status = FINISHED
Steps: EENEEEEESSNNWWWWWSWWssssssssssssssssssssSsEssEsssEssssNEWWWWssssssssssssssssssssWSSEEEEENsssssWWNWWsF
```