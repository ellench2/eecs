//  Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
//  zombbb.cpp
//  project_2
//
//  Created by Ellen Chlachidze on 9/25/22.
//

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include "xcode_redirect.hpp"
#include "P2random.h"
#include <queue>
#include <cmath>
#include <cassert>
using namespace std;


class Zombie_Fight {
private:
    struct Zombie {
        std::string name;
        uint32_t distance;
        uint32_t speed;
        uint32_t health;
        uint32_t birth_round_;
        uint32_t death_round_;
    };
    class z_compare {
    private:
        void print_zombie(Zombie z) {
            std::cout << z.name << " " << z.distance << " " << z.speed << " " << z.health << " " << z.birth_round_ << endl;
        }
    public:
        z_compare(){}
        bool operator()(Zombie* b, Zombie* a) {
            if (a->distance / a->speed < b->distance / b->speed) {
                return true;
            }
            else if (a->distance / a->speed > b->distance / b->speed) {
                return false;
            }
            else {
                if (a->health < b->health) {
                    return true;
                }
                else if (a->health > b->health) {
                    return false;
                }
                else {
                    if (a->name < b->name) {
                        return true;
                    }
                    else if (a->name > b->name) {
                        return false;
                    }
                    else {
                        std::cout << "WE SHOULD NEVER BE HERE SOMETHING WENT WRONG??" << endl;//REMOVE LATER
                        print_zombie(*a);
                        print_zombie(*b);
                        return false;
                    }
                }
            }
        }
    };
    class z_compare_active {
    private:
        bool active;
    public:
        z_compare_active(bool active_in) {
            active = active_in;
        }
        bool operator() (Zombie a, Zombie b) {
            if (!active) {
                if ((a.death_round_ - a.birth_round_) != (b.death_round_ - b.birth_round_)) {
                    return (a.death_round_ - a.birth_round_) < (b.death_round_ - b.birth_round_);
                }
                else {
                    return a.name < b.name;
                }
            }
            else {
                if ((a.death_round_ - a.birth_round_) != (b.death_round_ - b.birth_round_)) {
                    return (a.death_round_ - a.birth_round_) > (b.death_round_ - b.birth_round_);
                }
                else {
                    return a.name < b.name;
                }
            }
        }
    };
    uint32_t round_;
    uint32_t next_round_;
    deque<Zombie> master;
    uint32_t quiver_capacity;
    uint32_t rand_dist;
    uint32_t rand_speed;
    uint32_t rand_health;
    uint32_t rand_seed;
    uint32_t arrows;
    bool verbose;
    bool statistics;
    bool median;
    uint32_t stats;
    bool alive;
    std::priority_queue<Zombie*, std::deque<Zombie*>, z_compare> shoot_list;
    
public:
    Zombie_Fight() : round_{0}, next_round_{0}, master{0}, quiver_capacity{0}, rand_dist{0}, rand_speed{0},
    rand_health{0}, rand_seed{0}, arrows{0}, verbose{false}, statistics{false}, median{false}, stats{0}, alive{true},
    shoot_list{} {}
    
    void get_zombies(P2random &rand_gen) {
        if (round_ == next_round_) {
            string temp;
            uint32_t rand_z;
            cin >> temp >> rand_z;
            for (uint32_t i = 0; i < rand_z; i++) {
                //make a random zombie and add to master list?
                Zombie random;
                random.name = rand_gen.getNextZombieName();
                random.distance = rand_gen.getNextZombieDistance();
                random.speed = rand_gen.getNextZombieSpeed();
                random.health = rand_gen.getNextZombieHealth();
                random.birth_round_ = round_;
                random.death_round_ = round_;
                if (verbose) {
                    std::cout << "Created: " << random.name << " (distance: " << random.distance << ", speed: " <<
                    random.speed << ", health: " << random.health << ")\n";
                }
                master.push_back(random);
                shoot_list.push(&master.back());
            }
            uint32_t named_num;
            cin >> temp >> named_num;
            for (uint32_t i = 0; i < named_num; i++) {
                Zombie named;
                cin >> named.name >> temp >> named.distance >> temp >> named.speed >> temp >> named.health;
                named.birth_round_ = round_;
                named.death_round_ = round_;
                if (verbose) {
                    std::cout << "Created: " << named.name << " (distance: " << named.distance << ", speed: " <<
                    named.speed << ", health: " << named.health << ")\n";
                }
                master.push_back(named);
                shoot_list.push(&master.back());
            }
            if (getline(cin,temp)) {
                getline(cin,temp);
                cin >> temp >> next_round_;
            }
        }
    }
    
    void fight_zombies() {
//        bool first_ = false;
        priority_queue<uint32_t, deque<uint32_t>> lifetimes_a;
        priority_queue<uint32_t, deque<uint32_t>, std::greater<uint32_t>> lifetimes_b;
        vector<string> killed;
        bool victory = false;
        P2random rand_gen;
        if (round_ == 0) {
            string temp;
            getline(cin, temp);
            cin >> temp >> quiver_capacity;
            cin >> temp >> rand_seed;
            cin >> temp >> rand_dist;
            cin >> temp >> rand_speed;
            cin >> temp >> rand_health;
            getline(cin,temp);
            getline(cin,temp);
            cin >> temp >> next_round_;
        }
        rand_gen.initialize(rand_seed, rand_dist, rand_speed, rand_health);
        string least = "";
        int first = 0; // how many times have you been killed :D
        string first_z; // who killed you first :D
        while (alive && !victory) { //round loop
            
            round_++;
            arrows = quiver_capacity;
            if (verbose) {
                std::cout << "Round: " << round_ << "\n";
            }

            
            for (uint32_t i = 0; i < master.size(); i++) {
                if (master[i].health > 0) {
                    master[i].death_round_ = round_;
                    if (master[i].speed > master[i].distance) {
                        master[i].distance = 0;
                    }
                    else {
                        master[i].distance -= master[i].speed;
                    }
                    if (master[i].distance == 0) {
                        first++;
                        if (first == 1) {
                            first_z = master[i].name;
                        }
                        alive = false;
                    }
                    if (verbose) {
                        std::cout << "Moved: " << master[i].name << " (distance: " << master[i].distance << ", speed: " <<
                        master[i].speed << ", health: " << master[i].health << ")\n";
                    }
                }
            }
            
            if (!alive) {
                std::cout << "DEFEAT IN ROUND " << round_ << "! " << first_z << " ate your brains!\n";
                if (statistics) {
                    std::cout << "Zombies still active: " << shoot_list.size() << "\n";
                    std::cout << "First zombies killed:\n";
                    for (uint32_t i = 0; i < min(static_cast<size_t>(stats), killed.size()); i++) {
                        std::cout << killed[i] << " " << i+1 << "\n";
                    }
                    std::cout << "Last zombies killed:\n";
                    for (uint32_t i = 0; i < min(static_cast<size_t>(stats), killed.size()); i++) {
                        std::cout << killed[killed.size()-i-1] << " " << min(static_cast<size_t>(stats), killed.size())-i << "\n";
                    }
                    std::cout << "Most active zombies:\n";
                    z_compare_active compare_3(true);
                    sort(master.begin(), master.end(), compare_3);
                    for (uint32_t i = 0; i < min(static_cast<size_t>(stats), master.size()); i++) {
                        std::cout << master[i].name << " " << master[i].death_round_  -  master[i].birth_round_ + 1 << "\n";
                    }
                    z_compare_active compare_4(false);
                    std::cout << "Least active zombies:\n";
                    sort(master.begin(), master.end(), compare_4);
                    for (uint32_t i = 0; i < min(static_cast<size_t>(stats), master.size()); i++) {
                        std::cout << master[i].name << " " << master[i].death_round_  -  master[i].birth_round_ + 1 << "\n";
                    }
                }
                break;
            }
            
            else {
                get_zombies(rand_gen);
                
                while (!shoot_list.empty() && arrows > 0) {
                    bool added = false;
                    assert(shoot_list.top()->health > 0);
                    shoot_list.top()->health--;
                    arrows--;
                    if (shoot_list.top()->health == 0) {
                        if (verbose) {
                            std::cout << "Destroyed: " << shoot_list.top()->name << " (distance: " << shoot_list.top()->distance << ", speed: " <<
                            shoot_list.top()->speed << ", health: " << shoot_list.top()->health << ")\n";
                        }
                        shoot_list.top()->death_round_ = round_;
                        killed.push_back(shoot_list.top()->name);
                        if (lifetimes_a.empty()) {
                            added = true;
                            lifetimes_a.push(round_ - shoot_list.top()->birth_round_ + 1);
                        }
                        if (lifetimes_b.empty() && !added) {
                            added = true;
                            lifetimes_b.push(round_ - shoot_list.top()->birth_round_ + 1);
                        }
                        if (!added) {
                            double sum = lifetimes_a.top() + lifetimes_b.top();
                            if (!added && (round_ - shoot_list.top()->birth_round_) >= sum/2) {
                                added = true;
                                lifetimes_b.push(round_ - shoot_list.top()->birth_round_ + 1);
                            }
                            else {
                                assert(!added);
                                added = true;
                                lifetimes_a.push(round_ - shoot_list.top()->birth_round_ + 1);
                            }
                        }
                        if ((lifetimes_a.size() + 2) == lifetimes_b.size()) {
                            lifetimes_a.push(lifetimes_b.top());
                            lifetimes_b.pop();
                        }
                        else if ((lifetimes_b.size() + 2) == lifetimes_a.size()) {
                            lifetimes_b.push(lifetimes_a.top());
                            lifetimes_a.pop();
                        }
                        if ((lifetimes_a.size() > lifetimes_b.size() + 2) || (lifetimes_b.size() > lifetimes_a.size() + 2)) {
                            assert(false);
                        }
                        
                        if (!((lifetimes_a.size() + lifetimes_b.size()) == killed.size())) {
                            std::cout << lifetimes_a.size() << " " << lifetimes_b.size() << " " << killed.size() << endl;\
                            assert(false);
                        }
                        shoot_list.pop();
                    }
                }

                if (median && !killed.empty()) {
                    if (lifetimes_a.size() > lifetimes_b.size()) {
                        std::cout << "At the end of round " << round_ << ", the median zombie lifetime is " << lifetimes_a.top() << "\n";
                    }
                    else if (lifetimes_b.size() > lifetimes_a.size()) {
                        std::cout << "At the end of round " << round_ << ", the median zombie lifetime is " << lifetimes_b.top() << "\n";
                    }
                    else {
                        int x = (lifetimes_a.top() + lifetimes_b.top()) / 2;
                        std::cout << "At the end of round " << round_ << ", the median zombie lifetime is " << x << "\n";
                    }
                }
                
                if (shoot_list.empty() && next_round_ <= round_) {
                    victory = true;
                    std::cout << "VICTORY IN ROUND " << round_ << "! " << killed[killed.size()-1] << " was the last zombie.\n";
                    if (statistics) {
                        std::cout << "Zombies still active: " << shoot_list.size() << "\n";
                        std::cout << "First zombies killed:\n";
                        for (uint32_t i = 0; i < min(static_cast<size_t>(stats), killed.size()); i++) {
                            std::cout << killed[i] << " " << i+1 << "\n";
                        }
                        std::cout << "Last zombies killed:\n";
                        for (uint32_t i = 0; i < min(static_cast<size_t>(stats), killed.size()); i++) {
                            std::cout << killed[killed.size()-i-1] << " " << min(static_cast<size_t>(stats), killed.size())-i << "\n";
                        }
                        std::cout << "Most active zombies:\n";
                        z_compare_active compare_3(true);
                        sort(master.begin(), master.end(), compare_3);
                        for (uint32_t i = 0; i < min(static_cast<size_t>(stats), master.size()); i++) {
                            std::cout << master[i].name << " " << master[i].death_round_  -  master[i].birth_round_ + 1 << "\n";
                        }
                        z_compare_active compare_4(false);
                        std::cout << "Least active zombies:\n";
                        sort(master.begin(), master.end(), compare_4);
                        for (uint32_t i = 0; i < min(static_cast<size_t>(stats), master.size()); i++) {
                            std::cout << master[i].name << " " << master[i].death_round_  -  master[i].birth_round_ + 1 << "\n";
                        }
                    }
                }
            }
            
            
        }
    }
    
    void process(int argc, char *argv[]) {
        char* num;
        opterr = false;
        int choice;
        int option_index = 0;
        option long_options[] = {
            {"verbose", no_argument, nullptr, 'v'},
            {"median", no_argument, nullptr, 'm'},
            {"help", no_argument, nullptr, 'h'},
            {"statistics", required_argument, nullptr, 's'},
            { nullptr, 0,          nullptr, '\0' }
        };
        while ((choice = getopt_long(argc, argv, "vmhs:", long_options, &option_index)) != -1) {
            switch(choice) {
                case 'v':
                    verbose = true;
                    break;
                case 'm':
                    median = true;
                    break;
                case 'h':
                    cout << "help message here ya go\n";
                    exit(0);
                case 's':
                    statistics = true;
                    num = optarg;
                    stats = atoi(num);
                    break;
            }
        }
    }
};


int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    Zombie_Fight zombbb;
    zombbb.process(argc, argv);
    zombbb.fight_zombies();
}
