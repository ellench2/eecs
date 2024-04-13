//
//  main.cpp
//  test
//
//  Created by Ellen Chlachidze on 9/29/22.
//
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;
int main(int argc, const char * argv[]) {
    const char *s = "hello";
    char ss[20];
    int length = static_cast<int>(strlen(s));
    for (int i = 0; i < length; ++i)
        ss[i] = s[length - i];
    printf("%s", ss);
    return 0;
}
