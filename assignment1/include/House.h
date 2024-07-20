#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include <deque>
#include <string>
#include <utility>

using namespace std;

class House {
public:
    explicit House(const string &filename);

    void print_house() const;
    int get_dirt_level(int x, int y) const;
    bool is_wall(int x, int y) const;
    pair<int, int> get_docking_station() const;
    vector<deque<char> > get_layout() const;
    int get_max_steps_allowed() const;
    int get_max_battery_steps() const;
    int get_total_dirt() const;
    void decrease_dirt(int x, int y);

private:
    int max_steps_allowed = -1;
    int max_battery_steps = -1;
    vector<deque<char> > layout;
    pair<int, int> docking_station;
    bool docking_station_found = false;
    int total_dirt = 0;
    size_t layout_rows_num = 0;
    size_t layout_cols_num = 0;

    void init_house(const string &filename);
    void ensure_walls();
    void pad_layout();
    void decrease_total_dirt();
};

#endif // HOUSE_H
