// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"
#include <cassert>
#include <iostream>

using namespace std;

TEST(get_name_add_card_make_trump) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());
    alice->add_card(Card(Card::RANK_NINE, Card::SUIT_SPADES));
    alice->add_card(Card(Card::RANK_ACE, Card::SUIT_SPADES));
    alice->add_card(Card(Card::RANK_JACK, Card::SUIT_SPADES));
    alice->add_card(Card(Card::RANK_KING, Card::SUIT_CLUBS));
    alice->add_card(Card(Card::RANK_QUEEN, Card::SUIT_SPADES));
    Card upcard = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
    std::string suit = "";
    assert(alice->make_trump(upcard, true, 2, suit));
    assert(suit == "Spades");
    assert(alice->make_trump(upcard, false, 1, suit));
    Card upcard1 = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
    assert(!alice->make_trump(upcard1, false, 1, suit));
    Card upcard2 = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
    assert(!alice->make_trump(upcard2, false, 1, suit));
    //print alice hand
    
    delete alice;
}

TEST(add_and_discard_lead_card_play_card) {
    Player * bella = Player_factory("Bella", "Simple");
    bella->add_card(Card(Card::RANK_NINE, Card::SUIT_HEARTS));
    bella->add_card(Card(Card::RANK_JACK, Card::SUIT_SPADES));
    bella->add_card(Card(Card::RANK_JACK, Card::SUIT_CLUBS));
    bella->add_card(Card(Card::RANK_KING, Card::SUIT_DIAMONDS));
    bella->add_card(Card(Card::RANK_ACE, Card::SUIT_SPADES));
    Card upcard = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    bella->add_and_discard(upcard);
    Card upcard1 = Card(Card::RANK_TEN, Card::SUIT_SPADES);
    bella->add_and_discard(upcard1);
    Card led = bella->lead_card(upcard1.get_suit());
    assert(led.get_suit() == "Spades");
    Card played = bella->play_card(led, "Hearts");
    assert(played.get_suit() == "Spades");
    delete bella;
    
    Player * car = Player_factory("Car", "Simple");
    car->add_card(Card(Card::RANK_NINE, Card::SUIT_CLUBS));
    car->add_card(Card(Card::RANK_TEN, Card::SUIT_CLUBS));
    car->add_card(Card(Card::RANK_JACK, Card::SUIT_CLUBS));
    car->add_card(Card(Card::RANK_QUEEN, Card::SUIT_CLUBS));
    car->add_card(Card(Card::RANK_KING, Card::SUIT_CLUBS));
    Card played_0 = car->play_card(Card(Card::RANK_ACE, Card::SUIT_CLUBS), "Clubs");
    assert(played_0.get_rank() == "King");
    Card played_1 = car->play_card(Card(Card::RANK_JACK, Card::SUIT_SPADES), "Clubs");
    assert(played_1.get_rank() == "Queen");
    Card led_0 = car->lead_card("Spades");
    assert(led_0.get_rank() == "Ten");
    Card led_1 = car->lead_card("Spades");
    assert(led_1.get_rank() == "Nine");
    delete car;

}

// Add more tests here

TEST_MAIN()
