//  Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
//  rescue.cpp
//  EECS 281 Project 1
//
//  Created by Ellen Chlachidze on 9/7/22.
//
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include "xcode_redirect.hpp"
using namespace std;

class Rescue{
private:
    
    struct coord {
        uint32_t x, y, z;
    };
    int tiles_discovered;
    bool in_mode;
    bool container_mode;
    bool out_mode;
    uint32_t rooms;
    uint32_t side_length;
    coord start;
    coord current;
    
    struct tile {
        char walk;
        char discover;
    };
    

    
    vector<vector<vector<tile>>> map;
    

    
public:
    Rescue() :
    in_mode{false}, container_mode{false}, out_mode{false}, rooms{0},
    side_length{0}, map{0}, start{}, current{}, tiles_discovered{0}
    {}
    
    int get_tiles() {
        return tiles_discovered;
    }
    
    int find_countess() {
        
        bool found_c = false;
        //discover things haha
        current.x = start.x;
        current.y = start.y;
        current.z = start.z;
        coord countess;
        
        deque<coord> the_deque;
     //   deque<vector<int>> the_other_deque; //i hate myself
        const vector<vector<int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        //while have not discovered countess??
        //check not on warp pipe
        //if on warp pipe, check if valid (add to search deque) if not, skip to pulling from deque
        while (!found_c) {
           
            //std::cout << current.x << " " << current.y << " " << current.z << " " << map[current.x][current.y][current.z].walk << "\n";
            
            //std::cout << map[0][3][2].discover << " hi " << endl;
            if (!(isdigit(map[current.x][current.y][current.z].walk))) {
             //   std::cout << current.x << " " << current.y << " " << current.z << endl;
                for (auto i: directions) {
                    int new_row = current.y+i[0];
                    int new_col = current.z+i[1];
                    if (new_row < 0 || new_row >= side_length || new_col < 0 ||
                        new_col >= side_length || map[current.x][new_row][new_col].walk == '#' ||
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
                        break;
                        //oh neat
                        //done? or are we done when investigating countess??
                    }
//                    if (isdigit(map[current.x][new_row][new_col].walk)) {
//                        std::cout << "found a pipe neat " << current.x << endl;
//                        //the_other_deque.push_back({current[0], new_row, new_col, map[current[0]][new_row][new_col]});
//                        map[current.x][new_row][new_col].discover = static_cast<char>(current.x);
//                    }
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
                        //idk why i'm doing this its the same no matter the container
                        tiles_discovered++;
                    }
                }
            }
            else {
                // we're in a warp pipe slay
                uint32_t pipe_room = static_cast<uint32_t>(map[current.x][current.y][current.z].walk - '0');
                if (pipe_room < rooms && !isdigit(map[pipe_room][current.y][current.z].discover)) {
                    if (!(map[pipe_room][current.y][current.z].walk == '#' ||
                          map[pipe_room][current.y][current.z].walk == '!' ||
                          map[pipe_room][current.y][current.z].walk == 'S' ||
                          map[pipe_room][current.y][current.x].discover == 'n' ||
                          map[pipe_room][current.y][current.x].discover == 'e' ||
                          map[pipe_room][current.y][current.x].discover == 's' ||
                          map[pipe_room][current.y][current.x].discover == 'w')) {
                        map[pipe_room][current.y][current.z].discover = static_cast<char>(current.x + 48);
                        //std::cout << current.x << " " <<  map[pipe_room][current.y][current.z].discover << endl;
                        //std::cout << static_cast<char>(current.x + 48) << endl;
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
                //check if room exists
                //if so, check if spot on other end is ok
                //if so, push to deque
                //
            }
            
            
            if (the_deque.size() == 0) {
                //print no path error
                return 0;
            }
            else {
                if (container_mode) {
                    //if
                    current = the_deque[the_deque.size() - 1];
                    the_deque.pop_back();
                }
                else {
                    current = the_deque[0];
                    the_deque.pop_front();
                }
            }
            
            
        }
        //ok so now backtrack
        //
//        for (int a = 0; a < rooms; a++) {
//            for (int b = 0; b < side_length; b++) {
//                for (int c = 0; c < side_length; c++) {
//                    std::cout << map[a][b][c].discover;
//                }
//                std::cout << "\n";
//            }
//            std::cout << "\n" << "\n";
//        }

        //ok so backtrack
        //bool at_start
        //tile current_back
        //start at current from above, look at tile in opposite direction that discover char points, label it same as that char
        //output map
       // std::cout << "yay found her" << endl;
        //std::cout << map[1][3][2].discover << endl;
        vector<coord> path;
        bool at_start = false;
        coord current_back = countess;
        //std::cout << current_back.x << " " << current_back.y << " " << current_back.z << endl;
        while (!at_start) {
          // std::cout << current_back.x << " " << current_back.y << " " << current_back.z << endl;
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
                }}
            else {
                //std::cout << current_back.x << " " << current_back.y << " " << current_back.z << endl;
                //std::cout << static_cast<int>(map[current_back.x][current_back.y][current_back.z].discover-'0') << endl;
                //std::cout << current_back.y << current_back.z << endl;
                map[static_cast<int>(map[current_back.x][current_back.y][current_back.z].discover-'0')][current_back.y][current_back.z].walk = 'p';
                current_back.x = static_cast<int>(map[current_back.x][current_back.y][current_back.z].discover-'0');
                path.push_back(current_back);
            }

        }
      //  std::cout << map[0][3][2].discover << endl;
      //  std::cout << current.x << " " << current.y << " " << current.z << "\n";
        if (out_mode) {
            std::cout << "Start in room " << start.x << ", row " << start.y << ", column " << start.z << "\n";
            for (int a = 0; a < rooms; a++) {
                std::cout << "//castle room " << a << "\n";
                for (int b = 0; b < side_length; b++) {
                    for (int c = 0; c < side_length; c++) {
                        std::cout << map[a][b][c].walk;
                    }
                    std::cout << "\n";
                }
              //  std::cout << "\n" << "\n";
            }
        }
        else {
            std::cout << "Path taken:\n";
            for (int i = 0; i < path.size(); i++) {
                std::cout << "(" << path[path.size()-i-1].x << "," << path[path.size()-i-1].y << "," << path[path.size()-i-1].z << "," << map[path[path.size()-i-1].x][path[path.size()-i-1].y][path[path.size()-i-1].z].walk << ")\n";
            }
        }
        return 1;
    }


    void make_castle() {
        //might want to reserve space in map didn't pay enough attention during lecture and now i have no idea how that works
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
    /*    map.resize(rooms);
        for (int a = 0; a < side_length; a++) {
             map[a].resize(side_length);
         }
         for (int a = 0; a < side_length; a++) {
             for (int b = 0; b < side_length; b++) {
                 map[a][b].resize(side_length);
             }
         }*/
        map.resize(rooms, vector<vector<tile> >(side_length, vector<tile>(side_length, { '.', 'o' })));
        
//        for (int a = 0; a < rooms; a++) {
//            vector<vector<tile>> v2d;
//            for (int b = 0; b < side_length; b++) {
//                vector<tile> v1d;
//                for (int c = 0; c < side_length; c++) {
//                    tile tile_{'.', 'o'};
//                    v1d.push_back(tile_);
//                }
//                v2d.push_back(v1d);
//            }
//            map.push_back(v2d);
//        }
        
        if (in_mode) {
            //map: TRIED TO CHECK FOR RANDOM COMMENTS IDK IF WORKS
            char map_in;
            string temp;
            getline(cin, temp);
            for (int i = 0; i < rooms; i++) {
                for (int j = 0; j < side_length; j++) {
                    for (int k = 0; k < side_length; k++) {
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
                            std::cerr << "Unknown map character";
                            exit(1);
                        }
                       // std::cout << j << "\n";

                        //map[i][j].push_back(map_in);
                      //  std::cout << map_in << "\n";
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
                            if (roomIn >= 0 && roomIn < rooms && rowIn >= 0 && rowIn < side_length && colIn >= 0 && colIn < side_length) {
                                map[roomIn][rowIn][colIn].walk = tileIn;
                                if (tileIn == 'S') {
                                    start = coord{roomIn, rowIn, colIn};
                                }
                            }
                        }
                }
            }
        }

//        for (int a = 0; a < rooms; a++) {
//            for (int b = 0; b < side_length; b++) {
//                for (int c = 0; c < side_length; c++) {
//                    std::cout << map[a][b][c].walk;
//                }
//                std::cout << "\n";
//            }
//            std::cout << "\n" << "\n";
//        }
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
            { nullptr, 0,                 nullptr, '\0' }
        };
        while ((choice = getopt_long(argc, argv, "sqo:h", long_options, &option_index)) != -1) {
            switch (choice) {
            case 's':
                if (mode_tracker == 0) {
                    container_mode = true;
                    mode_tracker++;
                    break;
                }
                cerr << "Stack or queue can only be specified once" << endl;
                //exit(1);
                    
            case 'q':
                if (mode_tracker == 0) {
                    container_mode = false;
                    mode_tracker++;
                    break;
                }
                cerr << "Stack or queue can only be specified once" << endl;
                //exit(1);

            case 'o':
                mode = optarg;
                if (mode != "M" && mode != "L") {
                    cerr << "Error: output file format must be specified" << endl; //TO DO: fix?
                    exit(1);
                } // if
                modeSpecified = true;
                    break;

            case 'h':
                cout << "help message" << endl;
                break;
                    
            default:
                cerr << "Error: invalid option" << endl;
                //exit(1);
            } // switch
        } // while

        if (!modeSpecified) {
            cerr << "Error: no mode specified" << endl;
            //exit(1);
        } // if
        
        if (mode_tracker == 0) {
            cerr << "Stack or queue must be specified" << endl;
        }
        if (mode == "M" || mode == "") {
            out_mode = true;
        }
        if (mode == "L") {
            out_mode = false;
        }
    } // getMode()
};



int main(int argc, char *argv[]) {
    //ios_base::sync_with_stdio(false); //comment this out when testing with valgrind
    Rescue rescue_countess = *new Rescue();
    xcode_redirect(argc, argv);
    vector<vector<vector<char>>> castle = {};
    rescue_countess.process(argc,argv);
    rescue_countess.make_castle();
    if (rescue_countess.find_countess()) {
        return 0;
    }
    else{
        std::cout << "No solution, " << rescue_countess.get_tiles() << " tiles discovered.\n";
        return 0;
    }
}

