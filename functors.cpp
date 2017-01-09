/*
 Lucas O'Rourke
 lor215
 hw02_2&3.cpp
 
 Problem 2 uses a functor to see if an integer is even or odd. Problem 3 uses a functor to compare string lengths
 */

#include <iostream>
#include <vector>
#include <list>
using namespace std;


//Problem 2
class IsEvenOrOdd {
public:
    
    bool operator()(int n) {
        return (n%2 == 0);
    }
};

template <typename x>
string isEvenOrOdd(x n, IsEvenOrOdd check) {
    return (check(n))? "even": "odd";
}


//Problem 3
class CompTwoStrings {
public:
    bool operator()(const string& str1, const string& str2) {
        return (str1.size() > str2.size());
    }
};


template <typename s>
bool findGreater(const s& x, const s& y, CompTwoStrings checkGreater) {
    return checkGreater(x, y);
}



int main() {
    list<int> l = {1, 2, 3, 4};
    list<int>::iterator itr = l.begin();
    list<int>::iterator itr2 = l.end();
    for (list<int>::iterator i = itr; i != itr2; i--) {
        cout << *i << endl;
    }
    
    //Programming part 2
    int x = 6;
    IsEvenOrOdd q;
    cout << x << " is " << isEvenOrOdd(x, q) << endl;
    
    //Programming part 3
    string str1 = "hi";
    string str2 = "hello";
    CompTwoStrings c;
    cout << boolalpha << findGreater(str1, str2, c) << endl;
}
