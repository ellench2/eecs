
/* Pack.h
 *
 * Represents a pack of playing cards
 * Project UID 1d9f47bfc76643019cfbf037641defe1
 *
 * by Andrew DeOrio <awdeorio@umich.edu>
 * 2014-12-21
 */


#include "Card.h"
#include "Pack.h"
#include <array>
#include <string>

  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on.
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
Pack::Pack() {
    Card c1 = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card c2 = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card c3 = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card c4 = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card c5 = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card c6 = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    Card c7 = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
    Card c8 = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    Card c9 = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card c10 = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    Card c11 = Card(Card::RANK_KING, Card::SUIT_HEARTS);
    Card c12 = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
    Card c13 = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card c14 = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card c15 = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card c16 = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    Card c17 = Card(Card::RANK_KING, Card::SUIT_CLUBS);
    Card c18 = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
    Card c19 = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card c20 = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    Card c21 = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    Card c22 = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card c23 = Card(Card::RANK_KING, Card::SUIT_SPADES);
    Card c24 = Card(Card::RANK_ACE, Card::SUIT_SPADES);
    cards[0] = c1;
    cards[1] = c2;
    cards[2] = c3;
    cards[3] = c4;
    cards[4] = c5;
    cards[5] = c6;
    cards[6] = c7;
    cards[7] = c8;
    cards[8] = c9;
    cards[9] = c10;
    cards[10] = c11;
    cards[11] = c12;
    cards[12] = c13;
    cards[13] = c14;
    cards[14] = c15;
    cards[15] = c16;
    cards[16] = c17;
    cards[17] = c18;
    cards[18] = c19;
    cards[19] = c20;
    cards[20] = c21;
    cards[21] = c22;
    cards[22] = c23;
    cards[23] = c24;
    next = 0;
    //assert(false);
}

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
Pack::Pack(std::istream& pack_input) {
    for (int i = 0; i < 24; i++) {
        std::string rank_in;
        std::string suit_in;
        std::string temp;
        pack_input >> rank_in >> temp >> suit_in;
        Card c = Card(rank_in, suit_in);
        cards[i] = c;
    }
    next = 0;
    //assert(false);
}

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one() {
    int next1 = next;
    next++;
    return cards[next1];
    //assert(false);
}

  // EFFECTS: Resets next index to first card in the Pack
void Pack::reset() {
    next = 0;
    //assert(false);
}

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle() {
    for (int j = 0; j < 7; j++) {
    std::array<Card, PACK_SIZE> original;
    for (int i = 0; i < PACK_SIZE; i++){
        original[i] = cards[i];
    }
    int a = 0;
    for (int i = 0; i < PACK_SIZE; i++) {
        if (i%2 == 0) {
        cards[i] = original[PACK_SIZE/2 + a];
        }
        else {
        cards[i] = original[a];
        a++;
        }
    }
    }
    //assert(false);
}

  // EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
    if (next == PACK_SIZE){
        return true;
    }
    else return false;
    //assert(false);
}



