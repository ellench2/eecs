// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <iostream>
#include <string>
#include "Player.h"
//#include "Pack.h"
#include <array>
#include "Card.h"
#include <algorithm>
#include <cassert>

using namespace std;

class Human : public Player {
private:
    std::string name;
    std::array<Card, MAX_HAND_SIZE> hand;
    int num_cards;
public:
    Human(const std::string &name_in) : name(name_in), hand({}), num_cards(0) {
    }
    
    const std::string & get_name() const override {
        return name;
    }
    void add_card(const Card &c) override {
        assert(num_cards < MAX_HAND_SIZE);
        hand[num_cards] = c;
        num_cards++;
        //std::sort(hand.begin(), hand.end());
    }
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, std::string &order_up_suit) const override {
        
        for (int i = 0; i < num_cards; i++) {
                    std::cout << "Human player " << name << "'s hand: [" << i << "] ";
                    int test = 0;
                    int card = -1;
                    for (int j = 0; j < num_cards; j++) {
                        test = 0;
                        for (int k = 0; k < num_cards; k++) {
                            if (hand[j] > hand[k]) {
                                test++;
                            }
                        }
                        if (test == i) {
                            card = j;
                        }
                        
                    }
            std::cout << hand[card] << std::endl;
        }
  /*      for (int i = 0; i < num_cards; i++) {
            std::cout << "Human player " << name << "'s hand: [" << i << "] " <<
                hand[i] << std::endl;
        } */
        std::cout << "Human player " << name << ", please enter a suit, or \"pass\":"
            << std::endl;
        std::string input;
        std::cin >> input;
        if (input == "pass") {
            return false;
        }
        else {
            order_up_suit = input;
            return true;
        }
    }
    void add_and_discard(const Card &upcard) override {
        std::sort(hand.begin(), hand.end());
        for (int i = 0; i < num_cards; i++) {
            std::cout << "Human player " << name << "'s hand: [" << i << "] " <<
                hand[i] << std::endl;
        }
        std::cout << "Discard upcard: [-1]" << std::endl;
        std::cout << "Human player " << name << ", please select a card to discard:"
            << std::endl;
        char input;
        std::cin >> input;
        
        if (input == '-') {
            //wig do nothing
        }
        else {
            const char* input_1 = &input;
            int discard = std::atoi(input_1);
            hand[discard] = upcard;
            
        }
    }
    Card lead_card(const std::string &trump) override {
        std::sort(hand.begin(), hand.end());
        for (int i = 0; i < num_cards; i++) {
            std::cout << "Human player " << name << "'s hand: [" << i << "] "
                << hand[i] << std::endl;
        }
        std::cout << "Human player " << name << ", please select a card:" << std::endl;
        int d = 0;
        d++;
        char input;
        std::cin >> input;
        const char* input_ = &input;
        int lead = std::atoi(input_);
        num_cards--;
        Card played = hand[lead];
        if (lead != num_cards) {
            for (int i = lead; i < num_cards; i++) {
                hand[i] = hand[i+1];
            }
        }
        return played;
    }
    Card play_card(const Card &led_card, const std::string &trump) override {
        std::sort(hand.begin(), hand.end());
        for (int i = 0; i < num_cards; i++) {
            std::cout << "Human player " << name << "'s hand: [" << i << "] " <<
                hand[i] << std::endl;
        }
        std::cout << "Human player " << name << ", please select a card:" << std::endl;
        char input;
        std::cin >> input;
        const char* input_ = &input;
        int lead = std::atoi(input_);
        num_cards--;
        Card played = hand[lead];
        if (lead != num_cards) {
            for (int i = lead; i < num_cards; i++) {
                hand[i] = hand[i+1];
            }
        }
        return played;
    }
    ~Human() {}

};

class Simple : public Player {
private:
    std::string name;
    std::array<Card, MAX_HAND_SIZE> hand;
    int num_cards;
public:
    Simple(const std::string &name_in) :
        name(name_in), hand({}), num_cards(0) {
    }
    const std::string & get_name() const override {
        return name;
    }
    void add_card(const Card &c) override {
        assert(num_cards  < MAX_HAND_SIZE);
        hand[num_cards] = c;
        num_cards++;
    }
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, std::string &order_up_suit) const override {
        assert(round == 1 || round == 2);
        if (round == 1) {
            int trump_face_cards = 0;
            for (int i = 0; i < num_cards; i++) {
                if (hand[i].is_trump(upcard.get_suit()) && hand[i].is_face()) {
                    trump_face_cards++;
                }
            }
            if (trump_face_cards >= 2) {
                order_up_suit = upcard.get_suit();
                return true;
            }
            else return false;
        }
        if (round == 2) {
            std::string const check_suit = upcard.get_suit();
            int trump_face_cards = 0;
            for (int i = 0; i < num_cards; i++) {
                if (hand[i].is_trump(Suit_next(check_suit)) && hand[i].is_face()) {
                    trump_face_cards++;
                }
            }
            if (trump_face_cards >= 1 || is_dealer) {
                
                order_up_suit = Suit_next(check_suit);
                return true;
            }
            else return false;
        }
        else return false;
    }
    void add_and_discard(const Card &upcard) override {
        assert(num_cards >=1);
        int cards_greater = 0;
        for (int i = 0; i < num_cards; i++) {
            if (Card_less(hand[i], upcard, upcard.get_suit())) {
                cards_greater++;
            }
        }
        if (cards_greater == 0) {
            //neat
        }
        else {
            int smallest = -1;
            for (int i = 0; i < num_cards; i++) {
                int cards_greater = 0;
                for (int j = 0; j < num_cards; j++) {
                    if (j != i && Card_less(hand[j], hand[i], upcard.get_suit())) {
                        cards_greater++;
                    }
                }
                if (cards_greater == 0) {
                    smallest = i;
                }
            }
            hand[smallest] = upcard;
        }
    }
    Card lead_card(const std::string &trump) override {
      /* std::sort(hand.begin(), hand.end());
        for (int i = 0; i < num_cards; i++) {
            std::cout << "player " << name << "'s hand: [" << i << "] " <<
            hand[i] << std::endl;
        }*/
        assert(num_cards >= 1);
        int non_trumps = 0;
        for (int i = 0; i < num_cards; i++) {
            if (!hand[i].is_trump(trump)) {
                non_trumps++;
            }
        }
        if (non_trumps == 0) {
            int highest = -1;
            for (int i = 0; i < num_cards; i++) {
                int cards_less = 0;
                for (int j = 0; j < num_cards; j++) {
                    if ( j != i && Card_less(hand[i], hand[j], trump)) {
                        cards_less++;
                    }
                }
                if (cards_less == 0) {
                    highest = i;
                }
            }
            num_cards--;
            Card played = hand[highest];
            if (highest != num_cards) {
                for (int i = highest; i < num_cards; i++) {
                    hand[i] = hand[i+1];
                }
            }
            return played;
        }
        else {
            int highest = -1;
            for (int i = 0; i < num_cards; i++) {
                int cards_less = 0;
                if (!hand[i].is_trump(trump)) {
                for (int j = 0; j < num_cards; j++) {
                    if (!hand[j].is_trump(trump) && hand[j] > hand[i] && j != i) {
                        cards_less++;
                    }
                }
                    if (cards_less == 0) {
                        highest = i;
                    }
            }
            }
            num_cards--;
            
            Card played = hand[highest];
            if (highest != num_cards) {
                for (int i = highest; i < num_cards; i++) {
                    hand[i] = hand[i+1];
                }
            }
            return played;
        }
    }
    Card play_card(const Card &led_card, const std::string &trump) override {
      /* std::sort(hand.begin(), hand.end());
        for (int i = 0; i < num_cards; i++) {
            std::cout << "player " << name << "'s hand: [" << i << "] " <<
            hand[i] << std::endl;
        }
        std::cout << std::endl; */
        assert(num_cards >= 1);
        int led = -1;
        for (int i = 0; i < num_cards; i++) {
            if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
                led = i;
            }
        }
        if (led != -1) {
            int card_lesser = 0;
            for (int a = 0; a < num_cards; a++) {
                for (int b = 0; b < num_cards; b++) {
                    if (a != b && hand[a].get_suit(trump) == led_card.get_suit(trump)
                        && hand[b].get_suit(trump) == led_card.get_suit(trump)) {

                    if (Card_less(hand[a], hand[b], trump)) {
                        card_lesser++;
                    }
                    }
                }
                if (hand[a].get_suit(trump) == led_card.get_suit(trump) &&
                    card_lesser == 0) {
                    led = a;
                }
            }
            num_cards--;
            Card played = hand[led];
            if (led != num_cards) {
                for (int i = led; i < num_cards; i++) {
                    hand[i] = hand[i+1];
                }
            }
            return played;
        }
        else {
            int cards_greater = 0;
            for (int i = 0; i < num_cards; i++) {
                cards_greater = 0;
                for (int j = 0; j < num_cards; j++) {
                    if (!led_card.is_left_bower(trump)) {
                    if (i != j && Card_less(hand[j], hand[i], led_card, trump)) {
                        cards_greater++;
                    }
                    }
                    else {
                        if (i != j && Card_less(hand[j], hand[i], trump)) {
                            cards_greater++;
                        }
                    }
                }
                if (cards_greater == 0) {
                    led = i;
                }
            }
            num_cards--;
            
            Card played = hand[led];
            if (led != num_cards) {
                for (int i = led; i < num_cards; i++) {
                    hand[i] = hand[i+1];
                }
            }
            return played;
        }
    }
    ~Simple() {}
};

Player * Player_factory(const std::string &name, const std::string &strategy) {
    if (strategy == "Simple") {
        return new Simple(name);
    }
    if (strategy == "Human") {
        return new Human(name);
    }
    assert(false);
    return nullptr;
}
std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}

