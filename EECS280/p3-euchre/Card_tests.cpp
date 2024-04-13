 // Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());
    Card a(Card::RANK_KING, Card::SUIT_HEARTS);
    ASSERT_TRUE(c>a);
    ASSERT_FALSE(c<a);
    Card led(Card::RANK_THREE, Card::SUIT_DIAMONDS);
    Card test(Card::RANK_JACK, Card::SUIT_SPADES);
    Card test1(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_TRUE(Card_less(test1, test, led, "Spades"));
    ASSERT_TRUE(Card_less(test1, test, "Spades"));
    Card test2(Card::RANK_ACE, Card::SUIT_SPADES);
    ASSERT_TRUE(Card_less(test2, test1, led, "Spades") && Card_less(test2, test, led, "Spades"));
    ASSERT_TRUE(Card_less(test2, test1, "Spades"));
    Card test3(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    ASSERT_TRUE(Card_less(test3, test2, led, "Spades") && Card_less(test3, test1, led, "Spades"));
    ASSERT_TRUE(Card_less(test3, test, led, "Spades"));
    Card test4(Card::RANK_TEN, Card::SUIT_CLUBS);
    Card test5(Card::RANK_TEN, Card::SUIT_HEARTS);
    ASSERT_TRUE(Card_less(test5, test4, led, "Spades") && Card_less(test5, test3, led, "Spades"));
    Card test6(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card test7(Card::RANK_ACE, Card::SUIT_CLUBS);
    ASSERT_TRUE(Card_less(test6, test7, "Spades"));
    ASSERT_FALSE(Card_less(test6, test7, led, "Spades"));
    
}

// Add more test cases here

TEST_MAIN()
