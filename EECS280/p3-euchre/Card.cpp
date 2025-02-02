// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
using namespace std;

// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

// add your code below


// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec (see the appendices) before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=


//EFFECTS Initializes Card to the Two of Spades
Card::Card() : rank("Two"), suit("Spades") {
    //assert(false);
}

//REQUIRES rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
//  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
//  suit is one of "Spades", "Hearts", "Clubs", "Diamonds"
//EFFECTS Initializes Card to specified rank and suit
Card::Card(const std::string &rank_in, const std::string &suit_in)
    : rank(rank_in), suit(suit_in) {
    //assert(false);
}

//EFFECTS Returns the rank
std::string Card::get_rank() const {
    return rank;
    //assert(false);
}

//EFFECTS Returns the suit.  Does not consider trump.
std::string Card::get_suit() const {
    return suit;
    //assert(false);
}

//REQUIRES trump is a valid suit
//EFFECTS Returns the suit
//HINT: the left bower is the trump suit!
std::string Card::get_suit(const std::string &trump) const {
    if (is_left_bower(trump)) {
        return trump;
    }
    else return suit;
    //assert(false);
}
                                        

//EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
bool Card::is_face() const {
    if (rank == "Jack" || rank == "Queen" || rank == "King" || rank == "Ace") {
        return true;
    }
    else return false;
    //assert(false);
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the trump suit
bool Card::is_right_bower(const std::string &trump) const {
    if (suit == trump && rank == "Jack") {
        return true;
    }
    else return false;
    //assert(false);
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the next suit
bool Card::is_left_bower(const std::string &trump) const {
    if (suit == Suit_next(trump) && rank == "Jack") {
        return true;
    }
    else return false;
    //assert(false);
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if the card is a trump card.  All cards of the trump
// suit are trump cards.  The left bower is also a trump card.
bool Card::is_trump(const std::string &trump) const {
    if (suit == trump || is_left_bower(trump)) {
        return true;
    }
    else return false;
    //assert(false);
}

//EFFECTS Returns true if lhs is lower value than rhs.
//  Does not consider trump.
bool operator<(const Card &lhs, const Card &rhs) {
    if (lhs.get_rank() == rhs.get_rank()) {
        if (lhs.get_suit() == "Diamonds") {
            return false;
        }
        else if (rhs.get_suit() == "Diamonds") {
            return true;
        }
        else if (lhs.get_suit() == "Clubs") {
            return false;
        }
        else if (rhs.get_suit() == "Clubs") {
            return true;
        }
        else if (lhs.get_suit() == "Hearts") {
            return false;
        }
        else if (rhs.get_suit() == "Hearts") {
            return true;
        }
        else return false;
    }
    else if (lhs.get_rank() == "Ace") {
        return false;
    }
    else if (rhs.get_rank() == "Ace") {
        return true;
    }
    else if (lhs.get_rank() == "King") {
        return false;
    }
    else if (rhs.get_rank() == "King") {
        return true;
    }
    else if (lhs.get_rank() == "Queen") {
        return false;
    }
    else if (rhs.get_rank() == "Queen") {
        return true;
    }
    else if (lhs.get_rank() == "Jack") {
        return false;
    }
    else if (rhs.get_rank() == "Jack") {
        return true;
    }
    else if (lhs.get_rank() == "Ten") {
        return false;
    }
    else if (rhs.get_rank() == "Ten") {
        return true;
    }
    else return false;
    //assert(false);
}

//EFFECTS Returns true if lhs is lower value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator<=(const Card &lhs, const Card &rhs) {
    return (lhs<rhs || lhs==rhs);
    //assert(false);
}

//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs) {
    return (rhs<lhs);
    //assert(false);
}

//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator>=(const Card &lhs, const Card &rhs) {
    return (rhs<lhs || rhs==lhs);
    //assert(false);
}

//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs) {
    return (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit());
    //assert(false);
}

//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs) {
    return !(lhs==rhs);
    //assert(false);
}

//REQUIRES suit is a valid suit
//EFFECTS returns the next suit, which is the suit of the same color
std::string Suit_next(const std::string &suit) {
    if (suit == "Hearts") {
        return "Diamonds";
    }
    else if (suit == "Diamonds") {
        return "Hearts";
    }
    else if (suit == "Spades") {
        return "Clubs";
    }
    else if (suit == "Clubs") {
        return "Spades";
    }
    else return ":((";
    //assert(false);
}

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card) {
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const std::string &trump) {
    if (b.is_right_bower(trump)) {
        return true;
    }
    else if (a.is_right_bower(trump)){
        return false;
    }
    else if (b.is_left_bower(trump)) {
        int c = 0;
        c++;
        return true;
    }
    else if (a.is_left_bower(trump)) {
        return false;
    }
    else if (b.get_suit() == trump) {
        if (a.get_suit() == trump) {
            if (a<b) {
                return true;
            }
            else return false;
        }
        else return true;
    }
    else if (a.get_suit() == trump) {
        int d = 0;
        d++;
        return false;
    }
    else if (a<b) {
        return true;
    }
    else if (a>b) {
        return false;
    }
    else return false;
    
    //assert(false);
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card,
               const std::string &trump) {
    if (b.is_right_bower(trump)) {
        return true;
    }
    else if (a.is_right_bower(trump)) {
        return false;
    }
    else if (b.is_left_bower(trump)) {
        return true;
    }
    else if (a.is_left_bower(trump)) {
        return false;
    }
    else if (b.get_suit() == trump) {
        if (a.get_suit() == trump) {
            if (a<b) {
                return true;
            }
            else return false;
        }
        else return true;
    }
    else if (a.get_suit() == trump) {
        return false;
    }
    else if (b.get_suit() == led_card.get_suit()) {
        if(a.get_suit() == led_card.get_suit()) {
            if (a<b) {
                return true;
            }
            else return false;
        }
        else return true;
    }
    else if (a.get_suit() == led_card.get_suit()) {
        return false;
    }
    else if (a<b) {
        return true;
    }
    else if (a>b) {
        return false;
    }
    else return false;
    
    //assert(false);
}

