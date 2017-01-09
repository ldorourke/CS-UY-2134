/*
 Lucas O'Rourke
 
 Uses stacks and recursion for the user to play a Towers of Hanoi game
 */

#include <iostream>
#include <stack>
using namespace std;


void checkValid(stack<int>& orig, stack<int>& dest) {
    if (orig.size() == 0) {
        cout << "Not a valid move\n";
        return;
    }
    else if (dest.size() == 0) {
        dest.push(orig.top());
        orig.pop();
    }
    else {
        if (orig.top() > dest.top()) {
            cout << "Not a valid move\n";
            return;
        }
        dest.push(orig.top());
        orig.pop();
    }
}


void printContents(stack<int> tower) {
    cout << "Top --> Bottom: ";
    if (tower.size() == 0) {
        cout << endl;
        return;
    }
    size_t size = tower.size();
    for (int i = 0; i < size; i++) {
        cout << tower.top() << " ";
        tower.pop();
    }
    cout << endl;
}

int main() {
    stack<int> towerOne;
    stack<int> towerTwo;
    stack<int> towerThree;
    cout << "How many disks would you like to play with? >> ";
    int amt;
    cin >> amt;
    for (int i = amt; i > 0; i--) {
        towerOne.push(i);
    }
    cout << "Tower One contents: ";
    printContents(towerOne);
    cout << "Tower Two contents: ";
    printContents(towerTwo);
    cout << "Tower Three contents: ";
    printContents(towerThree);
    cout << endl;
    
    bool gameOver = false;
    
    int orig;
    int dest;
    while (gameOver == false) {
        cout << "Which tower would you like to move from >> ";
        cin >> orig;
        cout << "Which tower would you like to move to >> ";
        cin >> dest;
        if (orig == 1 && dest == 1) {
            cout << "Not a valid move\n";
        }
        if (orig == 1 && dest == 2) {
            checkValid(towerOne, towerTwo);
        }
        if (orig == 1 && dest == 3) {
            checkValid(towerOne, towerThree);
        }
        if (orig == 2 && dest == 1) {
            checkValid(towerTwo, towerOne);
        }
        if (orig == 2 && dest == 2) {
            cout << "Not a valid move\n";
        }
        if (orig == 2 && dest == 3) {
            checkValid(towerTwo, towerThree);
        }
        if (orig == 3 && dest == 1) {
            checkValid(towerThree, towerOne);
        }
        if (orig == 3 && dest == 2) {
            checkValid(towerThree, towerTwo);
        }
        if (orig == 3 && dest == 3) {
            cout << "Not a valid move\n";
        }
        cout << "\nTower One contents: ";
        printContents(towerOne);
        cout << "Tower Two contents: ";
        printContents(towerTwo);
        cout << "Tower Three contents: ";
        printContents(towerThree);
        cout << endl;
        if (towerThree.size() == amt) {
            cout << "Congrats you did it!" << endl;
            gameOver = true;
        }
        
    }
}