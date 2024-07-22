#include "../include/Simulator.h"


Simulator::Simulator() : rows(-1), cols(-1), maxSteps(-1), maxBatterySteps(-1), totalDirt(0), simTotalSteps(0),
                         batteryLevel(-1) {}

// WallSensor implementation
bool Simulator::isWall(Direction d) const {
    int row = simCurrPosition.first;
    int col = simCurrPosition.second;
    switch (d) {
        case Direction::North:
            row--;
            break;
        case Direction::East:
            col++;
            break;
        case Direction::South:
            row++;
            break;
        case Direction::West:
            col--;
            break;
    }
    // if position is out of house layout bounds -> return true (it's a wall)
    if (row < 0 || row > (int(houseLayout.size()) - 1) || col < 0 || col > (int(houseLayout[row].size()) - 1)) {
        return true;
    } // check if there is a wall in houseLayout[row][col]
    else if (houseLayout[row][col] == 'W') return true;
    else return false;
}

// DirtSensor implementation
int Simulator::dirtLevel() const {
    int row = simCurrPosition.first;
    int col = simCurrPosition.second;
    char c = houseLayout[row][col];
    if (c >= '1' && c <= '9') {
        return c - '0';
    }
        // any character that is not {1, ..., 9} is considered clean
    else return 0;
}

// BatteryMeter implementation
std::size_t Simulator::getBatteryState() const {
    return std::floor(batteryLevel);
}

/* change dirt level in simCurrPosition by num */
void Simulator::updateDirtLevel(int num) {
    houseLayout[simCurrPosition.first][simCurrPosition.second] += num;
}

int Simulator::getTotalDirt() const {
    return totalDirt;
}

void Simulator::updateBatteryLevel(float num) {
    this->batteryLevel += num;
}

void Simulator::setBatteryLevel(float num) {
    this->batteryLevel = num;
}

void Simulator::updateCurrentPosition(Step step) {
    simCurrPosition = moveInDirection(simCurrPosition, getMatchingDirection(step));
}

void Simulator::readHouseFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) throw std::runtime_error("Could not open file");

    /* extract the input file name from the path, to be used in the output file name */
    std::filesystem::path inputPath(filePath);
    input_file_name = inputPath.filename().string();

    /* initialize houseLayoutName, MaxSteps, MaxBattery, Rows, Cols */
    std::string line;
    if (getline(file, houseLayoutName)) {
        //std::cout << "Layout Name: " << houseLayoutName << std::endl; // For debugging
    }
    for (int i = 0; i < 4; i++) {
        if (!getline(file, line)) {
            throw std::runtime_error("File missing information");
        }
        std::istringstream ss(line);
        std::string key;
        int value;
        getline(ss, key, '=');
        ss >> value;
        key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
        if (key == "MaxSteps") maxSteps = value;
        else if (key == "MaxBattery") {
            maxBatterySteps = float(value);
            batteryLevel = float(value);
        } else if (key == "Rows") rows = value;
        else if (key == "Cols") cols = value;
    }
    // sanity check for: maxSteps, maxBatterySteps, rows, cols
    if (maxSteps < 0 || maxBatterySteps < 0 || rows < 0 || cols < 0) {
        throw std::runtime_error("Invalid layout file");
    }
    int houseLayoutRowsNum = 0;
    /* get house layout */
    // if the number of rows in the given layout is greater than 'Rows' -> ignore the rest of the rows
    while (getline(file, line) && houseLayoutRowsNum < rows) {
        houseLayoutRowsNum++;
        std::vector<char> currRow(line.begin(), line.end());
        // if the number of cols in the layout is smaller than 'Cols' -> pad the row with ' '
        if (int(currRow.size()) < cols) {
            currRow.resize(cols, ' ');
        }
            // if the number of cols in the given layout is greater than 'Cols' -> shorten the row by deleting the last elements
        else if (int(currRow.size()) > cols) {
            currRow.resize(cols);
        }
        // append the currRow to the house layout
        houseLayout.push_back(currRow);

        // Check if the currRow contains the docking station 'D'
        auto it = std::find(currRow.begin(), currRow.end(), 'D');
        if (it != currRow.end()) {
            if (!dockingStationFound) {
                dockingStationFound = true;
                int index = std::distance(currRow.begin(), it);
                simDockingStationPosition = std::make_pair(houseLayout.size() - 1, index);
            } else {
                throw std::runtime_error("More than 1 docking station defined in layout");
            }
        }
        // update the total dirt count in the house
        for (char c: currRow) {
            if (c >= '1' && c <= '9') {
                int currRowDirtLevel = c - '0';
                totalDirt += currRowDirtLevel;
            }
        }
    }
    if (!dockingStationFound) {
        throw std::runtime_error("No docking station defined in layout");
    }
    simCurrPosition = simDockingStationPosition;
}

void Simulator::printHouseLayout() const {
    std::cout << '\n' << "House Name: " << houseLayoutName << std::endl;
    std::cout << "MaxSteps = " << maxSteps << std::endl;
    std::cout << "MaxBattery = " << maxBatterySteps << std::endl;
    std::cout << "Rows = " << rows << std::endl;
    std::cout << "Cols = " << cols << std::endl;
    std::cout << "TotalDirtLevel = " << totalDirt << '\n' << std::endl;

    std::string wall;
    for (int i = 0; i < int(houseLayout[0].size()) + 2; i++) {
        wall += 'W';
    }
    // print North wall
    std::cout << wall << std::endl;
    for (const auto &currRow: houseLayout) {
        bool newRow = true;
        for (const auto &cell: currRow) {
            if (newRow) {
                newRow = false;
                std::cout << 'W';
            }
            std::cout << cell;
        }
        std::cout << 'W' << std::endl;
    }
    // print South wall
    std::cout << wall << std::endl;
}

void Simulator::setAlgorithm(Algorithm algorithm) {
    this->algo = algorithm;
    this->algo.setMaxSteps(maxSteps);
    this->algo.setWallsSensor(*this);
    this->algo.setDirtSensor(*this);
    this->algo.setBatteryMeter(*this);
}

std::pair<int, int> Simulator::getSimCurrPosition() {
    return simCurrPosition;
}

std::pair<int, int> Simulator::getSimDockingStationPosition() {
    return simDockingStationPosition;
}

void Simulator::run() {
    if (simTotalSteps > maxSteps) {
        throw std::runtime_error("Mission failed: Used maximum number of steps allowed.");
    }
    // while mission not completed
    while (true) {
        Step simNextStep = algo.nextStep();

        // if the algorithm returns 'Finish' -> break the loop
        if (simNextStep == Step::Finish) {
            updateSimTotalStepsLog(simNextStep);
            break;
        } // if the battery is empty and not on docking station
        if (batteryLevel == 0 && simCurrPosition != simDockingStationPosition) {
            throw std::runtime_error("Run out of battery away from docking station");
        }
        if (simNextStep == Step::Stay) {
            // if on docking station -> charge()
            if (simCurrPosition == simDockingStationPosition) {
                updateBatteryLevel(maxBatterySteps / 20);
                // prevent over charging the battery
                if (batteryLevel > maxBatterySteps) {
                    setBatteryLevel(maxBatterySteps);
                }
            } else { // if not on docking station -> clean()
                updateBatteryLevel(-1);
                updateDirtLevel(-1);
                totalDirt--;
            }
        } else { // simNextStep != 'Stay'
            // update the current position according to the step
            updateCurrentPosition(simNextStep);
            updateBatteryLevel(-1);
        }
        updateSimTotalStepsLog(simNextStep);
        simTotalSteps++;
    }
    createOutputFile();
    return;
}

void Simulator::updateSimTotalStepsLog(Step step) {
    std::string s;
    switch (step) {
        case Step::Stay:
            s = 's';
            break;
        case Step::North:
            s = 'N';
            break;
        case Step::East:
            s = 'E';
            break;
        case Step::South:
            s = 'S';
            break;
        case Step::West:
            s = 'W';
            break;
        case Step::Finish:
            s = 'F';
            break;
    }
    simTotalStepsLog.push_back(s);
}

void Simulator::printHouseLayoutForSim(const std::string &action) const {
    system("clear"); // Clear the screen. Use "CLS" for Windows.

    std::cout << "Max steps allowed: " << maxSteps << std::endl;
    std::cout << "Total Steps Taken: " << simTotalSteps << "\n" << std::endl;
    std::cout << "Action: " << action << std::endl;
    std::cout << "Battery Level: " << getBatteryState() << std::endl;
    std::cout << "Max battery steps: " << maxBatterySteps << std::endl;
    std::cout << "Total Dirt Left: " << totalDirt << "\n" << std::endl;

    std::string wall(cols + 2, 'W');
    std::cout << wall << std::endl;
    for (int r = 0; r < rows; ++r) {
        std::cout << 'W';
        for (int c = 0; c < cols; ++c) {
            if (std::make_pair(r, c) == simCurrPosition) {
                std::cout << 'V'; // Represent the robot with 'V'
            } else {
                std::cout << houseLayout[r][c];
            }
        }
        std::cout << 'W' << std::endl;
    }
    std::cout << wall << "\n" << std::endl;
}

void Simulator::runWithSim() {
    if (simTotalSteps > maxSteps) {
        throw std::runtime_error("Mission failed: Used maximum number of steps allowed.");
    }
    // Initial print
    printHouseLayoutForSim("Starting");

    // while mission not completed
    while (true) {
        Step simNextStep = algo.nextStep();

        std::string action = getMatchingString(simNextStep);

        // if the algorithm returns 'Finish' -> break the loop
        if (simNextStep == Step::Finish) {
            updateSimTotalStepsLog(simNextStep);
            break;
        } // if the battery is empty and not on docking station
        if (batteryLevel == 0 && simCurrPosition != simDockingStationPosition) {
            throw std::runtime_error("Run out of battery away from docking station");
        }
        if (simNextStep == Step::Stay) {
            // if on docking station -> charge()
            if (simCurrPosition == simDockingStationPosition) {
                updateBatteryLevel(maxBatterySteps / 20);
                // prevent over charging the battery
                if (batteryLevel > maxBatterySteps) {
                    setBatteryLevel(maxBatterySteps);
                }
            } else { // if not on docking station -> clean()
                updateBatteryLevel(-1);
                updateDirtLevel(-1);
                totalDirt--;
            }
        } else { // simNextStep != 'Stay'
            // update the current position according to the step
            updateCurrentPosition(simNextStep);
            updateBatteryLevel(-1);
        }
        updateSimTotalStepsLog(simNextStep);
        simTotalSteps++;

        // Print the house layout after each action
        printHouseLayoutForSim(action);
        std::this_thread::sleep_for(std::chrono::milliseconds(350));
    }
    createOutputFile();
    return;
}

void Simulator::createOutputFile() {
    // create outputFileName
    std::string outputFileName = "output_" + input_file_name;
    std::ofstream outputFile(outputFileName);
    if(!outputFile) { // make sure the output file has been created
        std::string err = "Error opening file: " + outputFileName;
        throw std::runtime_error(err);
    }
    outputFile << "NumSteps = " << simTotalSteps << std::endl;
    outputFile << "DirtLeft = " << totalDirt << std::endl;
    std::string status = "UNKOWN"; // default value
    std::string lastStep = simTotalStepsLog.back();
    // determin the status
    if (lastStep == "F") {
        if (simCurrPosition == simDockingStationPosition) {
            status = "FINISHED";
        } else {
            status = "DEAD";
        }
    } else if (batteryLevel == 0 && simCurrPosition != simDockingStationPosition) {
        status = "DEAD";
    } else if (simTotalSteps == maxSteps && batteryLevel > 0) {
        status = "WORKING";
    }

    outputFile << "Status = " << status << std::endl;
    std::string steps_log = parseSimTotalStepsLog();
    outputFile << "Steps: " << steps_log << std::endl;
}

std::string Simulator::parseSimTotalStepsLog() {
    std::string str;
    for (const auto& s : simTotalStepsLog) {
        str += s;
    }
    return str;
}