//  Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
//  rescue.cpp
//  EECS 281 Project 1
//
//  Created by Ellen Chlachidze on 9/7/22.
//
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include "xcode_redirect.hpp"
using namespace std;

class Rescue{
private:
    
    struct tile {
        char walk;
        char discover;
    };
    
    struct coord {
        uint32_t x, y, z;
    };

    bool in_mode;
    bool container_mode;
    bool out_mode;
    uint32_t rooms;
    uint32_t side_length;
    vector<vector<vector<tile>>> map;
    coord start;
    coord current;
    int tiles_discovered;

    
public:
    Rescue() :
    in_mode{false}, container_mode{false}, out_mode{false}, rooms{0},
    side_length{0}, map{0}, start{}, current{}, tiles_discovered{1}
    {}
    
    int get_tiles() {
        return tiles_discovered;
    }
    
    int find_countess() {
        bool found_c = false;
        current.x = start.x;
        current.y = start.y;
        current.z = start.z;
        coord countess;
        deque<coord> the_deque;
        const vector<vector<int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        while (!found_c) {
            // debugging: print out every tile I visit :)
           // std::cout << current.x << " " << current.y << " " << current.z << endl;
            if (!(isdigit(map[current.x][current.y][current.z].walk)))  {
                for (auto i: directions) {
                    int new_row = current.y+i[0];
                    int new_col = current.z+i[1];
                    if (new_row < 0 || new_row >= int(side_length) || new_col < 0 ||
                        new_col >= int(side_length) || map[current.x][new_row][new_col].walk == '#' ||
                        map[current.x][new_row][new_col].walk == '!' ||
                        map[current.x][new_row][new_col].walk == 'S' ||
                        map[current.x][new_row][new_col].discover == 'n' ||
                        map[current.x][new_row][new_col].discover == 'e' ||
                        map[current.x][new_row][new_col].discover == 's' ||
                        map[current.x][new_row][new_col].discover == 'w' ||
                        isdigit(map[current.x][new_row][new_col].discover)) {
                        continue;
                    }
                    if (map[current.x][new_row][new_col].walk == 'C') {
                        if (i==directions[0]){
                            map[current.x][new_row][new_col].discover = 'n';
                        }
                        if (i==directions[1]){
                            map[current.x][new_row][new_col].discover = 'e';
                        }
                        if (i==directions[2]){
                            map[current.x][new_row][new_col].discover = 's';
                        }
                        if (i==directions[3]){
                            map[current.x][new_row][new_col].discover = 'w';
                        }
                        countess.x = current.x;
                        countess.y = new_row;
                        countess.z = new_col;
                        found_c = true;
                    }
                    else {
                        if (i==directions[0]){
                            map[current.x][new_row][new_col].discover = 'n';
                        }
                        if (i==directions[1]){
                            map[current.x][new_row][new_col].discover = 'e';
                        }
                        if (i==directions[2]){
                            map[current.x][new_row][new_col].discover = 's';
                        }
                        if (i==directions[3]){
                            map[current.x][new_row][new_col].discover = 'w';
                        }
                    }
                    if (container_mode) {
                        the_deque.push_back({current.x, static_cast<uint32_t>(new_row), static_cast<uint32_t>(new_col)});
                        tiles_discovered++;
                    }
                    else {
                        the_deque.push_back({current.x, static_cast<uint32_t>(new_row), static_cast<uint32_t>(new_col)});
                        tiles_discovered++;
                    }
                }
            }
            else {
                uint32_t pipe_room = static_cast<uint32_t>(map[current.x][current.y][current.z].walk - '0');
                if (pipe_room < rooms && !isdigit(map[pipe_room][current.y][current.z].discover)) {
                    if (!(map[pipe_room][current.y][current.z].walk == '#' ||
                          map[pipe_room][current.y][current.z].walk == '!' ||
                          map[pipe_room][current.y][current.z].walk == 'S' ||
                          map[pipe_room][current.y][current.z].discover == 'n' ||
                          map[pipe_room][current.y][current.z].discover == 'e' ||
                          map[pipe_room][current.y][current.z].discover == 's' ||
                          map[pipe_room][current.y][current.z].discover == 'w')) {
                        map[pipe_room][current.y][current.z].discover = static_cast<char>(current.x + 48);
                        if (map[pipe_room][current.y][current.z].walk == 'C') {
                            //std::cout << pipe_room << endl;
                            countess.x = pipe_room;
                            countess.y = current.y;
                            countess.z = current.z;
                            found_c = true;
                        }
                        if (container_mode) {
                            the_deque.push_back({pipe_room, current.y, current.z});
                            tiles_discovered++;
                        }
                        else {
                            the_deque.push_back({pipe_room, current.y, current.z});
                            tiles_discovered++;
                        }
                    }
                    
                }
            }
            if (the_deque.size() == 0) {
                return 0;
            }
            else {
                if (container_mode) {
                    current = the_deque[the_deque.size() - 1];
                    the_deque.pop_back();
                }
                else {
                    current = the_deque[0];
                    the_deque.pop_front();
                }
            }
        }
        vector<coord> path;
        bool at_start = false;
        coord current_back = countess;
        //std::cout << map[current_back.x][current_back.y][current_back.z].walk<< endl;
        while (!at_start) {
            if (current_back.x == start.x && current_back.y == start.y && current_back.z == start.z) {
                at_start = true;
                continue;
            }
            if (!isdigit(map[current_back.x][current_back.y][current_back.z].discover)) {
                if (map[current_back.x][current_back.y][current_back.z].discover == 'n') {
                    map[current_back.x][current_back.y+1][current_back.z].walk = 'n';
                    current_back.y+=1;
                    path.push_back(current_back);
                }
                else if (map[current_back.x][current_back.y][current_back.z].discover == 'e') {
                    map[current_back.x][current_back.y][current_back.z-1].walk = 'e';
                    current_back.z-=1;
                    path.push_back(current_back);
                }
                else if (map[current_back.x][current_back.y][current_back.z].discover == 's') {
                    map[current_back.x][current_back.y-1][current_back.z].walk = 's';
                    current_back.y-=1;
                    path.push_back(current_back);
                }
                else if (map[current_back.x][current_back.y][current_back.z].discover == 'w') {
                    map[current_back.x][current_back.y][current_back.z+1].walk = 'w';
                    current_back.z+=1;
                    path.push_back(current_back);
                }
            }
            else {
                //std::cout << "pipe?? " << endl;
                map[static_cast<int>(map[current_back.x][current_back.y][current_back.z].discover-'0')][current_back.y][current_back.z].walk = 'p';
                current_back.x = static_cast<int>(map[current_back.x][current_back.y][current_back.z].discover-'0');
                path.push_back(current_back);
            }

        }
        if (out_mode) {
            std::cout << "Start in room " << start.x << ", row " << start.y << ", column " << start.z << "\n";
            for (int a = 0; a < int(rooms); a++) {
                std::cout << "//castle room " << a << "\n";
                for (int b = 0; b < int(side_length); b++) {
                    for (int c = 0; c < int(side_length); c++) {
                        std::cout << map[a][b][c].walk;
                    }
                    std::cout << "\n";
                }
            }
        }
        else {
            std::cout << "Path taken:\n";
            for (int i = 0; i < int(path.size()); i++) {
                std::cout << "(" << path[path.size()-i-1].x << "," << path[path.size()-i-1].y << "," << path[path.size()-i-1].z << "," << map[path[path.size()-i-1].x][path[path.size()-i-1].y][path[path.size()-i-1].z].walk << ")\n";
            }
        }
        return 1;
    }


    void make_castle() {
        char mode_in;
        cin >> mode_in;
        if (mode_in == 'M') {
            in_mode = true;
        }
        else if (mode_in == 'L') {
            in_mode = false;
        }
        else {
            cerr << "input mode read in error\n";
        }
        uint32_t rooms_in;
        cin >> rooms_in;
        if (rooms_in < 10 && rooms_in > 0) {
            rooms = rooms_in;
        }
        else {
            cerr << "room number read in error\n";
        }
        uint32_t side_length_in;
        cin >> side_length_in;
        if (side_length_in > 0) {
            side_length = side_length_in;
        }
        else {
            cerr << "side length read in error\n";
        }
        map.resize(rooms, vector<vector<tile> >(side_length, vector<tile>(side_length, { '.', 'o' })));
        if (in_mode) {
            char map_in;
            string temp;
            getline(cin, temp);
            for (int i = 0; i < int(rooms); i++) {
                for (int j = 0; j < int(side_length); j++) {
                    for (int k = 0; k < int(side_length); k++) {
                        cin >> map_in;
                        if (map_in == '/') {
                            getline(cin, temp);
                            k--;
                            continue;
                        }
                        if (map_in == '.' || map_in == '#' || map_in == '!' ||
                            isdigit(map_in) || map_in == 'S' || map_in == 'C') {
                            tile tile_{map_in, 'o'};
                            map[i][j][k] = tile_;
                        }
                        else {
                            std::cerr << "Unknown map character\n";
                            exit(1);
                        }
                        if (map_in == 'S') {
                            start.x = i;
                            start.y = j;
                            start.z = k;
                        }
                    }
                }
                getline(cin, temp);
            }
        }
        else {
            string temp;
            char coord_;
            while (cin >> coord_){
                if (coord_ == '/') {
                    getline(cin, temp);
                }
                else {
                    char junk;
                    uint32_t roomIn, rowIn, colIn;
                    char tileIn;
                    cin >> roomIn >> junk >> rowIn >> junk >> colIn >> junk >> tileIn >> junk;
                        if (isdigit(tileIn) || tileIn == '!' || tileIn == '#' || tileIn == '.' || tileIn == 'C' || tileIn == 'S') {
                            if (roomIn < rooms && rowIn < side_length && colIn < side_length) {
                                map[roomIn][rowIn][colIn].walk = tileIn;
                                if (tileIn == 'S') {
                                    start = coord{roomIn, rowIn, colIn};
                                }
                            }
                            else if (roomIn >= rooms) {
                                std::cerr << "Invalid room number\n";
                                exit(1);
                            }
                            else if (rowIn >= side_length) {
                                std::cerr << "Invalid row number\n";
                                exit(1);
                            }
                            else if (colIn >= side_length) {
                                std::cerr << "Invalid column number\n";
                                exit(1);
                            }
                        }
                        else {std::cerr << "Unknown map character\n";
                            exit(1);
                        }
                }
            }
        }


    }
    
    void process(int argc, char * argv[]) {
        bool modeSpecified = false;
        string mode = "";
        opterr = false;
        int choice;
        int option_index = 0;
        int mode_tracker = 0;
        option long_options[] = {
            { "stack", no_argument, nullptr, 's'},
            { "queue", no_argument, nullptr, 'q'},
            { "output", required_argument, nullptr, 'o' },
            { "help", no_argument, nullptr, 'h' },
            { nullptr, 0,          nullptr, '\0' }
        };
        while ((choice = getopt_long(argc, argv, "sqo:h", long_options, &option_index)) != -1) {
            switch (choice) {
            case 's':
                if (mode_tracker == 0) {
                    container_mode = true;
                    mode_tracker++;
                    break;
                }
                cerr << "Stack or queue can only be specified once\n" << endl;
                exit(1);
            case 'q':
                if (mode_tracker == 0) {
                    container_mode = false;
                    mode_tracker++;
                    break;
                }
                cerr << "Stack or queue can only be specified once\n" << endl;
                    exit(1);
            case 'o':
                modeSpecified = true;
                mode = optarg;
                break;
            case 'h':
                cout << "help message\n" << endl;
                    exit(0);
                break;
            default:
                cerr << "Unknown command line option\n" << endl;
                exit(1);
            }
        }
        if (mode_tracker == 0) {
            cerr << "Stack or queue must be specified\n" << endl;
            exit(1);
        }
        if (mode == "M" || modeSpecified == false) {
            out_mode = true;
        }
        if (mode == "L") {
            out_mode = false;
        }
    }
    ~Rescue() {}
};



int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    Rescue rescue_countess;
    xcode_redirect(argc, argv);
    vector<vector<vector<char>>> castle = {};
    rescue_countess.process(argc,argv);
    rescue_countess.make_castle();
    if (rescue_countess.find_countess()) {
        return 0;
    }
    else{
        if (rescue_countess.get_tiles() > 1){
            std::cout << "No solution, " << rescue_countess.get_tiles() << " tiles discovered.\n";
        }
        else {
            std::cout << "No solution, " << rescue_countess.get_tiles() << " tile discovered.\n";
        }
        return 0;
    }
}


