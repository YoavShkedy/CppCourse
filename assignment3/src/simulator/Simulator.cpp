#include "Simulator.h"
#include <typeinfo>
#include <cxxabi.h> // Required for demangling (GCC/Clang specific)


Simulator::Simulator() : summaryOnly(false), rows(-1), cols(-1), maxSteps(-1), maxBatterySteps(-1), totalDirt(0), simTotalSteps(0),
                         batteryLevel(-1) {}

Simulator::Simulator(bool summaryOnly) : summaryOnly(summaryOnly), rows(-1), cols(-1), maxSteps(-1), maxBatterySteps(-1), totalDirt(0), simTotalSteps(0),
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
/* returns FALSE if house file cannot be open or is invalid, and TRUE if house file is valid */
bool Simulator::readHouseFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Simulator::readHouseFile ERROR: Failed to open file" << std::endl;
        std::cout << "Invalid file name: " << input_file_name << std::endl;
        return false;
    }

    /* extract the house_files file name from the path, to be used in the output file name */
    std::filesystem::path inputPath(filePath);
    input_file_name = inputPath.filename().string();

    /* initialize houseLayoutName, MaxSteps, MaxBattery, Rows, Cols */
    std::string line;
    for (int i = 0; i < 5; i++) {
        if (!getline(file, line)) {
            std::cout << "Simulator::readHouseFile ERROR: File missing information" << std::endl;
            std::cout << "Invalid file name: " << input_file_name << std::endl;
            file.close();
            return false;
        }
        std::istringstream ss(line);
        std::string key;
        int value;
        getline(ss, key, '=');
        key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1); // Trim trailing spaces from key
        ss >> value;
        if (key == "MaxSteps") maxSteps = value;
        else if (key == "MaxBattery") {
            maxBatterySteps = float(value);
            batteryLevel = float(value);
        } else if (key == "Rows") rows = value;
        else if (key == "Cols") cols = value;
    }
    // sanity check for: maxSteps, maxBatterySteps, rows, cols
    if (maxSteps < 0 || maxBatterySteps < 0 || rows < 0 || cols < 0) {
        std::cout << "Simulator::readHouseFile ERROR: Invalid house file parameter (maxSteps / MaxBatterySteps / house rows / house cols" << std::endl;
        std::cout << "Invalid file: " << input_file_name << std::endl;
        file.close();
        return false;
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
                std::cout << "Simulator::readHouseFile ERROR: More than 1 docking station defined in layout" << std::endl;
                std::cout << "Invalid file name: " << input_file_name << std::endl;
                file.close();
                return false;
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
        std::cout << "Simulator::readHouseFile ERROR: No docking station defined in layout" << std::endl;
        std::cout << "Invalid file name: " << input_file_name << std::endl;
        file.close();
        return false;
    }
    initDirt = totalDirt;
    simCurrPosition = simDockingStationPosition;
    file.close();
    // If house file is valid, return TRUE
    return true;
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

void Simulator::setAlgorithm(std::unique_ptr<AbstractAlgorithm> algorithm) {
    if (algorithm == nullptr) {
        std::string errorFileName = getAlgorithmName(algo) + ".error";
        writeError(errorFileName, "Simulator::setAlgorithm ERROR: Algorithm pointer is nulptr: " + getAlgorithmName(algo));
    }
    this->algo = std::move(algorithm);
    this->algoName = getAlgorithmName(algo);
    this->algo->setMaxSteps(maxSteps);
    this->algo->setWallsSensor(*this);
    this->algo->setDirtSensor(*this);
    this->algo->setBatteryMeter(*this);
}

std::pair<int, int> Simulator::getSimCurrPosition() {
    return simCurrPosition;
}

std::pair<int, int> Simulator::getSimDockingStationPosition() {
    return simDockingStationPosition;
}

/* return -1 in case of an error, and score otherwise */
int Simulator::run() {
    try {
        while (true) {
            if (simTotalSteps > maxSteps) {
                std::string errorFileName = algoName + ".error";
                writeError(errorFileName, "Simulator::run() ERROR: Total steps taken by the simulation exceed MaxSteps");
                throw std::runtime_error(
                        "Simulator::run() ERROR: Total steps taken by the simulation exceed MaxSteps");
            } else if (batteryLevel == 0 && simCurrPosition != simDockingStationPosition) {
                std::string errorFileName = algoName + ".error";
                writeError(errorFileName, "Simulator::run() ERROR: Run out of battery away from docking station");
                throw std::runtime_error("Simulator::run() ERROR: Run out of battery away from docking station");
            }

            Step simNextStep = this->algo->nextStep();

            if (simNextStep == Step::Finish) {
                updateSimTotalStepsLog(simNextStep);
                break;
            }

            if (simNextStep == Step::Stay) {
                if (simCurrPosition == simDockingStationPosition) {
                    updateBatteryLevel(maxBatterySteps / 20);
                    if (batteryLevel > maxBatterySteps) {
                        setBatteryLevel(maxBatterySteps);
                    }
                } else {
                    updateBatteryLevel(-1);
                    updateDirtLevel(-1);
                    totalDirt--;
                }
            } else {
                updateCurrentPosition(simNextStep);
                updateBatteryLevel(-1);
            }
            updateSimTotalStepsLog(simNextStep);
            simTotalSteps++;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    int score = calcScore();
    if (!summaryOnly) {
        createOutputFile();
    }
    return score;
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
        Step simNextStep = this->algo->nextStep();

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
            // if on docking station -> charge
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

std::string Simulator::calcStatus() {
    std::string status = "UNKNOWN"; // default value
    std::string lastStep = simTotalStepsLog.back();
    // determine the status
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
    return status;
}

std::string Simulator::calcInDock() {
    std::string inDock = (simCurrPosition == simDockingStationPosition) ? "TRUE" : "FALSE";
    return inDock;
}

int Simulator::calcScore() {
    int score;
    std::string status = calcStatus();
    std::string inDock = calcInDock();

    if (status == "DEAD") {
        score = maxSteps + totalDirt * 300 + 2000;
    }
    else if (status == "FINISHED" && inDock == "FALSE") {
        score = maxSteps + totalDirt * 300 + 3000;
    }
    else if (inDock == "TRUE") {
        score = simTotalSteps + totalDirt * 300;
    }
    else {
        score = simTotalSteps + totalDirt * 300 + 1000;
    }
    return score;
}

std::string Simulator::parseSimTotalStepsLog() {
    std::string str;
    for (const auto &s: simTotalStepsLog) {
        str += s;
    }
    return str;
}

void Simulator::createOutputFile() {
    // create outputFileName
    std::string outputFileName = input_file_name + "-" + algoName;
    std::ofstream outputFile(outputFileName);
    if (!outputFile) { // make sure the output file has been created
        std::string err = "Simulator::createOutputFile() Error: Failed to create output file: " + outputFileName;
        throw std::runtime_error(err);
    }
    outputFile << "NumSteps = " << simTotalSteps << std::endl;

    outputFile << "DirtLeft = " << totalDirt << std::endl;

    std::string status = calcStatus();
    outputFile << "Status = " << status << std::endl;

    std::string inDock = calcInDock();
    outputFile << "InDock = " << inDock << std::endl;

    int score = calcScore();
    outputFile << "Score = " << score << std::endl;

    std::string steps_log = parseSimTotalStepsLog();
    outputFile << "Steps: " << steps_log << std::endl;
}

void Simulator::createTimeoutOutputFile(int timeoutScore) {
    // create outputFileName
    std::string outputFileName = input_file_name + "-" + algoName;
    std::ofstream outputFile(outputFileName);
    if (!outputFile) { // make sure the output file has been created
        std::string err = "Simulator::createTimeoutOutputFile() Error: Failed to create output file: " + outputFileName;
        throw std::runtime_error(err);
    }

    outputFile << "NumSteps = " << simTotalSteps << std::endl;

    outputFile << "DirtLeft = " << totalDirt << std::endl;

    outputFile << "Status = DEAD" << std::endl; // CHECK IF RIGHT STATUS FOR TIMEOUT

    std::string inDock = calcInDock();
    outputFile << "InDock = " << inDock << std::endl;

    outputFile << "Score = " << timeoutScore << std::endl;

    std::string steps_log = parseSimTotalStepsLog();
    outputFile << "Steps: " << steps_log << std::endl;
}

std::string Simulator::getAlgorithmName(const std::unique_ptr<AbstractAlgorithm> &algorithm) {
    if (!algorithm) {
        throw std::runtime_error("Algorithm pointer is null.");
    }
    // Use a reference to the dereferenced object
    const AbstractAlgorithm& algoRef = *algorithm;

    // Get the mangled name
    const char* mangled_name = typeid(algoRef).name();

    // Demangle the name (this is compiler specific)
    int status = 0;
    char *demangled_name = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);

    std::string algorithmName = (status == 0) ? demangled_name : mangled_name;

    // Free the memory allocated by __cxa_demangle
    std::free(demangled_name);

    return algorithmName;
}