#ifndef DEALS_H
#define DEALS_H

// EECS 281 LAB 10: DYNAMIC PROGRAMMING
// Identifier: D7E20F91029D0CB08715A2C54A782E0E8DF829BF
// Ellen Chlachidze

// Your favorite restaurant has a customer loyalty program. It works like this: whenever you buy a
// meal, you can add one holepunch to your punchcard. Once you have 5 punches, you can turn in the
// card for a free meal (and a new, unmarked punchcard).

// For example, if your meals cost [3, 3, 3, 3, 3, 3, 3, 120] then you should earn hole punches
// from the first 5 meals ($15), pay normally for the next two, and then turn in the punchcard
// so that the $120 meal is free! The total cost would be $21 (or with coupons [see below] $19).

// However, you ALSO have a lot of coupons to this restaurant. In fact, you have enough coupons
// that you can apply one to any and every meal! If you apply a coupon, you get a 25% discount
// on that meal. HOWEVER, you don't get to add a holepunch to your card if you use a coupon!

// For example, if your meals cost [2, 2, 2, 2, 1000, 100], then if you use the first 5 meals to
// earn holepunches, you'll need to spend $1008. Then you'll get $100 free. It would be much better
// to just apply the 25% discount to each item, so that you pay a total of $829.

// There are, however, many cases where it makes sense to use a mixture of punchcard discounts and
// discounting coupons. This is where your program comes in!

// You will be given a list of meal prices. For the given meal prices, find the LEAST POSSIBLE
// cost needed to pay for all the meals using the holepunch loyalty program and coupons.

// Notes/Clarifications/Hints:
// * use the "discounted" function below to compute discount
//    (so that you round the same way as the instructor solution).
// * you should always EITHER use a coupon, or apply the punchcard
// * you have an unlimited number of coupons
// * your program should be able to run in linear time.
// * greedy solutions will not work
// * use dynamic programming

#include <iostream>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>
using namespace std;
// TODO: import whatever you want

// This is a type synonym.
using cost = long long;

// How much you pay for a discounted (25% off) meal.
cost discounted(cost full_price) {
    return full_price * 3 / 4;
}

cost sum_cost(vector<cost> &costs) {
    cost sum = 0;
    for (uint i = 0; i < costs.size(); i++) {
        sum += costs[i];
    }
    return sum;
}

cost sum_discounted(vector<cost> &costs) {
    cost sum = 0;
    for (uint i = 0; i < costs.size(); i++) {
        sum += discounted(costs[i]);
    }
    return sum;
}

cost best_price(const std::vector<cost>& prices) {
    if (prices.size() <= 5) { // if size <= 5, always cheapest to just coupon everything
        cost sum = 0;
        for (uint i = 0; i < prices.size(); i++) {
            sum += discounted(prices[i]);
        }
        return sum;
    }
    else {
        vector<vector<cost>> memo; // make memo
        memo.resize(6, vector<cost>(prices.size()-4));
        vector<cost> not_punched;
        not_punched.reserve(5);
        for (uint i = 0; i < 5; i++) {
            not_punched.push_back(prices[i]);
        }
        memo[5][0] = sum_cost(not_punched);
        memo[0][0] = sum_discounted(not_punched);
        vector<cost> punched = {};
        for (uint i = 1; i < 5; i++) {
            punched.push_back(*min_element(not_punched.begin(), not_punched.end()));
            not_punched.erase(min_element(not_punched.begin(), not_punched.end()));
            memo[i][0] = sum_cost(punched) + sum_discounted(not_punched);
        }
        
        for (uint i = 1; i < prices.size()-4; i++) {
            memo[0][i] = std::min(memo[5][i-1], memo[0][i-1] + discounted(prices[i+4]));
            for (uint j = 1; j < 6; j++) {
                cost a = memo[j][i-1] + discounted(prices[i+4]);
                cost b = memo[j-1][i-1] + prices[i+4];
                if (a < b) {
                    memo[j][i] = a;
                }
                else {
                    memo[j][i] = b;
                }
            }
        }
        vector<cost> row;
        row.reserve(6);
        for (uint i = 0; i < 6; i++) {
            row.push_back(memo[i][prices.size() - 5]);
        }
        return *min_element(row.begin(), row.end());
    }
}

#endif
