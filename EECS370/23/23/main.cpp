//
//  main.cpp
//  23
//
//  Created by Ellen Chlachidze on 5/3/23.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    int a = 0;
    int b = 1;
    int c = 2;
    a = b = c;
    std::cout << a << " " << b << " " << c << std::endl;
    a = 0;
    b = 1;
    c = 2;
    a = b;
    b = c;
    std::cout << a << " " << b << " " << c << std::endl;
    a = 0;
    b = 1;
    c = 2;
    b = c;
    a = b;
    std::cout << a << " " << b << " " << c << std::endl;
    a = 0;
    b = 1;
    c = 2;
    int d = 3;
    a = b = c = d;
    std::cout << a << " " << b << " " << c << " " << d << std::endl;
    return 0;
}
