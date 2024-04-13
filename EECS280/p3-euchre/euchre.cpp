// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <iostream>
#include <fstream>
#include <string>
#include "Player.h"
#include "Pack.h"
#include "Card.h"
#include <algorithm>
#include <array>
#include <vector>
#include <cassert>

using namespace std;

class Game {
private:
    std::array<Player*, 4> players;
    Pack pack_of_cards;
    std::string ordered_up;
    std::string filename;
    int dealer_;
    int who_ordered_up;
    int leader;
    bool shuffle;
    int points_to_win;
    int curr_hand;
    int curr_trick;
    int A_points;
    int B_points;
    std::array<Card, 4> table;
public:
    Game(const std::array<std::string, 8> players_in,
         bool shuffle_in, int points_in, const std::string filename_in) {
        shuffle = shuffle_in;
        points_to_win = points_in;
        Player* player0 = Player_factory(players_in[0], players_in[1]);
        players[0] = player0;
        Player* player1 = Player_factory(players_in[2], players_in[3]);
        players[1] = player1;
        Player* player2 = Player_factory(players_in[4], players_in[5]);
        players[2] = player2;
        Player* player3 = Player_factory(players_in[6], players_in[7]);
        players[3] = player3;
        curr_hand = 0;
        curr_trick = 0;
        A_points = 0;
        B_points = 0;
        table = {};
        leader = 1;
        dealer_ = 0;
        filename = filename_in;
        ifstream ifs(filename);
        pack_of_cards = Pack(ifs);
    }
    //make players, intialize everything
    void shuffle_() {
        if (shuffle) {
            pack_of_cards.shuffle();
        }
        else {
            ifstream ifs(filename);
            pack_of_cards = Pack(ifs);
        }
    }
    
    void reset_() {
        pack_of_cards.reset();
    }
    
    //just call the card function of the same name
    void start_hand() {
        std::cout << "Hand " << curr_hand << std::endl;
        std::cout << players[dealer_]->get_name() << " deals" << std::endl;
        
        players[(dealer_ + 1) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 1) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 1) % 4]->add_card(pack_of_cards.deal_one());
            
        players[(dealer_ + 2) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 2) % 4]->add_card(pack_of_cards.deal_one());
        
        players[(dealer_ + 3) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 3) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 3) % 4]->add_card(pack_of_cards.deal_one());
            
        players[dealer_]->add_card(pack_of_cards.deal_one());
        players[dealer_]->add_card(pack_of_cards.deal_one());

        players[(dealer_ + 1) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 1) % 4]->add_card(pack_of_cards.deal_one());
        
        players[(dealer_ + 2) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 2) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 2) % 4]->add_card(pack_of_cards.deal_one());
        
        players[(dealer_ + 3) % 4]->add_card(pack_of_cards.deal_one());
        players[(dealer_ + 3) % 4]->add_card(pack_of_cards.deal_one());
        
        players[dealer_]->add_card(pack_of_cards.deal_one());
        players[dealer_]->add_card(pack_of_cards.deal_one());
        players[dealer_]->add_card(pack_of_cards.deal_one());

        table[0] = pack_of_cards.deal_one();
        table[1] = pack_of_cards.deal_one();
        table[2] = pack_of_cards.deal_one();
        table[3] = pack_of_cards.deal_one();
        
        std::cout << table[0] << " turned up" << std::endl;
        
    }
    //print all the things, deal cards
    void make() {
        std::string empty = "";
        bool orderup = false;
        int i = 0;
        while(!orderup && i < 4) {
            if ((dealer_ + 1 + i) % 4 != 0) {
    orderup = players[(dealer_ + 1 + i) % 4]->make_trump(table[0], false, 1, empty);
            }
            else {
                orderup = players[0]->make_trump(table[0], true, 1, empty);
            }
            if (orderup) {
                std::cout << players[(dealer_ + 1 + i) % 4]->get_name() <<
                " orders up " << empty << std::endl;
                players[dealer_]->add_and_discard(table[0]);
                ordered_up = empty;
                who_ordered_up = (dealer_ + 1 + i) % 4;
                table = {};
            }
            else {
    std::cout << players[(dealer_ + 1 + i) % 4]->get_name() << " passes" << std::endl;
            }
            i++;
            empty = "";
        }
        if (!orderup) {
            i = 0;
            while (!orderup && i < 4) {
                if ((dealer_ + 1 + i) % 4 != 0) {
    orderup = players[(dealer_ + 1 + i) % 4]->make_trump(table[0], false, 2, empty);
                }
                else {
    orderup = players[(dealer_ + 1 + i) % 4]->make_trump(table[0], true, 2, empty);
                }
                if (orderup) {
                    std::cout << players[(dealer_ + 1 + i) % 4]->get_name()
                    << " orders up "
                    << empty << std::endl;
                    ordered_up = empty;
                    who_ordered_up = (dealer_ + 1 + i) % 4;
                    table = {};
                }
                else {
                    std::cout << players[(dealer_ + 1 + i) % 4]->get_name() <<
                    " passes" << std::endl;
                }
                i++;
                empty = "";
            }
        }
    }
    //make_trump() each player, print decisions
    void play_hand() {
        
        leader = (dealer_ + 1) % 4;
        int a_wins = 0;
        int b_wins = 0;
        for (int k = 0; k < 5; k++) {
            std::cout << std::endl;
            
        table[0] = players[leader]->lead_card(ordered_up);
        std::cout << table[0] << " led by " <<
            players[leader]->get_name() << std::endl;
        table[1] = players[(leader + 1) % 4]->play_card(table[0], ordered_up);
        std::cout << table[1] << " played by " <<
            players[(leader + 1) % 4]->get_name() << std::endl;
        table[2] = players[(leader + 2) % 4]->play_card(table[0], ordered_up);
        std::cout << table[2] << " played by " <<
            players[(leader + 2) % 4]->get_name() << std::endl;
        table[3] = players[(leader + 3) % 4]->play_card(table[0], ordered_up);
        std::cout << table[3] << " played by " <<
            players[(leader + 3) % 4]->get_name() << std::endl;
            
        //find largest of cards on table;
        int largest = -1;
        for (int i = 0; i < 4; i++) {
            int smaller = 0;
            for (int j = 0; j < 4; j++) {
                if (j != i && Card_less(table[i], table[j], table[0], ordered_up)) {
                    smaller++;
                }
            }
            if (smaller == 0) {
                largest = i;
            }
        }
        std::cout << players[(leader + largest) % 4]->get_name() <<
            " takes the trick" << std::endl;
            if ((leader + largest) % 4 == 0 || (leader + largest) % 4 == 2) {
                a_wins++;
            }
            else if ((leader + largest) % 4 == 1 || (leader + largest) % 4 == 3) {
                b_wins++;
            }
        leader = (leader + largest) % 4;
            std::cout << std::endl;
        
        
        table = {};
        curr_trick++;
        }
        assert(a_wins != b_wins);
        if (a_wins > b_wins) {
            std::cout << players[0]->get_name() << " and " <<
                players[2]->get_name() << " win the hand" << std::endl;
            if (who_ordered_up == 0 || who_ordered_up == 2) {
                if (a_wins == 3 || a_wins == 4) {
                    A_points += 1;
                }
                else {
                    assert(a_wins == 5);
                    std::cout << "march!" << std::endl;
                    A_points += 2;
                }
            }
            else {
                assert(who_ordered_up == 1 || who_ordered_up == 3);
                A_points += 2;
                
                std::cout << "euchred!" << std::endl;
            }
            std::cout << players[0]->get_name() << " and " << players[2]->get_name()
                << " have " << A_points << " points" << std::endl;
            std::cout << players[1]->get_name() << " and " << players[3]->get_name()
                << " have " << B_points << " points" << std::endl;
        }
        else {
            std::cout << players[1]->get_name() << " and " <<
                players[3]->get_name() << " win the hand" << std::endl;
            if (who_ordered_up == 1 || who_ordered_up == 3) {
                if (b_wins == 3 || b_wins == 4) {
                    B_points += 1;
                }
                else {
                    assert(b_wins == 5);
                    std::cout << "march!" << std::endl;
                    B_points += 2;
                }
            }
            else {
                assert(who_ordered_up == 0 || who_ordered_up == 2);
                B_points += 2;
                std::cout << "euchred!" << std::endl;
            }
            std::cout << players[0]->get_name() << " and " << players[2]->get_name()
                << " have " << A_points << " points" << std::endl;
            std::cout << players[1]->get_name() << " and " << players[3]->get_name()
                << " have " << B_points << " points" << std::endl;
        }
        std::cout << std::endl;
        curr_hand++;
        dealer_ = (dealer_ + 1) % 4;
    }
    bool end_game() {
        if (A_points >= points_to_win && A_points > B_points) {
            std::cout << players[0]->get_name() << " and " <<
                players[2]->get_name() << " win!" << std::endl;
            return true;
        }
        if (B_points >= points_to_win && B_points > A_points) {
            std::cout << players[1]->get_name() << " and " <<
                players[3]->get_name() << " win!" << std::endl;
            return true;
        }
        return false;
    }
    //play_card() every player, print decision,
    //figure out who won (update all member variables)
    ~Game() {}
};


int main(int argc, char *argv[]) { 
    //check command line arguments, print them
    const char* points_in = *(argv + 3);
    const char* shuffle_in = *(argv + 2);
    std::string shuffle_ = "shuffle";
    std::string noshuffle_ = "noshuffle";
    const char* type_0 = *(argv + 5);
    const char* type_1 = *(argv + 7);
    const char* type_2 = *(argv + 9);
    const char* type_3 = *(argv + 11);
    std::string human_ = "Human";
    std::string simple_ = "Simple";
    if (argc != 12 || std::atoi(points_in) < 1 || std::atoi(points_in) > 100 ||
        (shuffle_in != shuffle_ && shuffle_in != noshuffle_)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;

    }
    else if ((type_0 != human_ && type_0 != simple_) ||
             (type_1 != human_ && type_1 != simple_) ||
             (type_2 != human_ && type_2 != simple_) ||
             (type_3 != human_ && type_3 != simple_)) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    std::array<std::string, 8> players_in;
    for (int i = 0; i < 8; i++) {
        players_in[i] = *(argv + i + 4);
    }
    bool shuffle_in_;
    int points_in_ = std::atoi(points_in);
    if (shuffle_in == shuffle_) {
        shuffle_in_ = true;
    }
    else {shuffle_in_ = false;}
    const char* filename_input = *(argv + 1);
    for (int i = 0; i < argc; i++) {
        std::cout << *(argv + i) << " ";
    }
    std::cout << std::endl;
    Game euchre = *new Game(players_in, shuffle_in_, points_in_, filename_input);
    euchre.shuffle_();
    euchre.start_hand();
    euchre.make();
    euchre.play_hand();
    while (!euchre.end_game()) {
        euchre.reset_();
        euchre.shuffle_();
        euchre.start_hand();
        euchre.make();
        euchre.play_hand();
        std::cout << std::endl;
    }
    return 0;
}
