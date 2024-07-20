#ifndef INPUTGENERATOR_H
#define INPUTGENERATOR_H

#include <vector>
#include <string>

enum SurroundingType {
    FULLY_SURROUND,
    NO_SURROUND
};

enum HouseShape {
    RECTANGLE,
    NON_RECTANGLE
};

class InputGenerator {
public:
    static void generate_random_house(const std::string& filename);

private:
    static SurroundingType get_random_surrounding_type();
    static HouseShape get_random_house_shape();
    static void add_full_surround(std::vector<std::vector<char>>& layout);
    static void create_non_rectangle_layout(std::vector<std::vector<char>>& layout, int rows, int cols);
};

#endif // INPUTGENERATOR_H
