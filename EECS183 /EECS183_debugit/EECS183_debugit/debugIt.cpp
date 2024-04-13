// debugIt.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <string>
using namespace std;


int multiply_int(int m, int q){
    
    int vb = 31;
    
    vb = vb + 4;
    q += vb;
    vb = vb + m + 68;
    
    
    return vb/q;
}

int divide_int(double b, int w){
    
    b = w;
    w = b+45;
    return b *w;
}

int IDontWannaDoMath(double cs){
    cs *= 45;
    cs /= 21;
    ++cs;
    cs -= 2;
    return cs;
}

int crazy_sum(int xyz){
    
    xyz += 2;
    xyz += 4;
    xyz += 356;
    xyz -= 45;
    xyz += 4;
    xyz += 67;
    xyz += 1234;
    xyz -= 1233;
    xyz += 7;
    xyz += 12;
    xyz += 8;
    xyz += 17;
    xyz += 123;
    xyz += 55;
    xyz -= 123;
    xyz -= 12;
    xyz += 2;
    xyz += 1;
    xyz += 2;
    xyz += 4;
    xyz += 356;
    xyz -= 45;
    xyz += 4;
    xyz += 67;
    xyz += 1234;
    xyz -= 1233;
    xyz += 7;
    xyz += 12;
    xyz += 8;
    xyz += 17;
    xyz += 123;
    xyz += 55;
    xyz -= 123;
    xyz -= 12;
    xyz += 2;
    xyz += 1;
    xyz += 123;
    xyz += 55;
    xyz -= 123;
    xyz -= 12;
    xyz += 2;
    xyz += 1;
    xyz += 2;
    xyz += 4;
    xyz += 356;
    xyz -= 45;
    xyz += 4;
    xyz += 67;
    xyz += 1234;
    xyz -= 1233;
    xyz += 7;
    xyz += 12;
    xyz += 8;
    xyz += 17;
    xyz += 123;
    xyz += 55;
    xyz -= 123;
    xyz -= 12;
    xyz += 2;
    xyz += 1;
    xyz += 2;
    xyz += 4;
    xyz += 356;
    xyz -= 45;
    xyz += 4;
    xyz += 67;
    xyz += 1234;
    xyz -= 1233;
    xyz += 7;
    xyz += 12;
    xyz += 8;
    xyz += 4;
    xyz += 67;
    xyz += 1234;
    xyz -= 1233;
    xyz += 7;
    xyz = xyz + IDontWannaDoMath(xyz);
    xyz += 55;
    xyz -= 123;
    xyz -= 12;
    xyz += 2;
    xyz += 1;
    
    return xyz;
}

string AddThatExclamationPoint(string original, bool addIt){
    
    if (addIt == true){
        original += "!";
    }
    return original;
}

int main( )
{
    
    int m = 5;
    int x = 3;
    int r = 4;
    r += x - 2;
    
    m = multiply_int(m, 2);
    x = divide_int(x, 1);
    
    m = m + crazy_sum(m);
    //this should stay the same
    m = m + (r - 5);
    
    string congratulate = "Congratulations!";
    bool removeIt = true;
    congratulate = AddThatExclamationPoint(congratulate, removeIt);
    
    cout << congratulate;
    cout << " You finished DebugIt!" << endl;
    
    
    return 0;
    
}

