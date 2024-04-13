//
//  main.cpp
//  376hw_helper:)
//
//  Created by Ellen Chlachidze on 12/5/22.
//

#include <iostream>
#include <vector>


using namespace std;

int main(int argc, const char * argv[]) {
    vector<uint> input = {2,1,0,1,4};
    int n = input.size();
    int cur_pos = 0;
    int reach = 0;

    do {
        if (cur_pos + input[cur_pos] > reach) {
            reach = cur_pos + input[cur_pos];
        }
        cur_pos++;
    } while (cur_pos != n - 1 && reach > cur_pos);
    bool result = (cur_pos == n - 1);
    std::cout << result;
}
