/*
 Lucas O'Rourke
 lor215
 gamesec.cpp
 
 A game of fighters and warriors that the user can play
 */

#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <map>
using namespace std;


class Fighter {
public:
    Fighter() : preHealth(5), health(5) {}
    
    virtual int getChoice() = 0;

    void counter() {
        if (checkDead() == false) {
            
        }
    }
    
    bool checkDead() const {
        return health == 0;
    }
    
    void rest() {
        if (checkDead() == false) {
            if (health < 5) health++;
        }
    }
    
    void counterChange() {
        if (checkDead() == false) {
            health -= 2;
        }
    }
    
    void attackChange() {
        if (checkDead() == false) {
            health--;
        }
    }
    
    int getHealth() {
        return health;
    }
    
private:
    int preHealth;
    int health;
};

class Player;
class Enemy : public Fighter {
public:
    Enemy(): Fighter(), attacker(nullptr) {}
    
    int getChoice() {
        int c = rand()%3; //Create random integer between 0 and 2
        return c;
    }
    template <class obj>
    void attack(vector<obj>& v)  {
        if (checkDead() == false) {
            int plAtt = rand()%3;
            v[plAtt].attackChange();
            v[plAtt].changeAttacker(*this);
            cout << " Player " << plAtt + 1 << endl;
        }
        else {
            cout << " but is dead already lol" << endl;
        }
    }
    
    template <class o>
    void changeAttacker(o& att) {
        attacker = &att;
    }
    
    void counter() {
        if (checkDead() == false) {
            if (attacker != nullptr) {
                //attacker->counterChange();
            }
            else {
                cout << " but had no one to counter";
            }
            cout << endl;
        }
    }
    
private:
    Player* attacker;
};


class Player : public Fighter {
public:
    Player() : Fighter(), attacker(nullptr) {}
    
    int getChoice() {
        cout << "Would you like to (enter number):\n\t1) Attack\n\t2) Counter\n\t3) Rest\n>> ";
        int user_inp;
        cin >> user_inp;
        if (user_inp < 0 || user_inp > 3) {
            return getChoice();
        }
        else {
            return user_inp;
        }
        
    }
    
    void attack(vector<Enemy>& v)  {
        if (checkDead() == false) {
            cout << "Who would you like to attack?\n\t1) Enemy 1\n\t2) Enemy 2\n\t3) Enemy 3\n>> ";
            int user_inp;
            cin >> user_inp;
            v[user_inp -1].attackChange();
            v[user_inp-1].changeAttacker(*this);
            cout << "You attacked Enemy " << user_inp << "!\n";
        }
    }
    
    void changeAttacker(Enemy& e) {
        attacker = &e;
    }
    
    void counter() {
        if (checkDead() == false) {
            if (attacker != nullptr) {
                attacker->counterChange();
            }
            else {
                cout << " but had no one to counter!";
            }
            cout << endl;
        }
    }
    
private:
    Enemy* attacker;
};




bool checkAllDead(vector<Enemy> e, vector<Player> p) {
    int deadEn = 0;
    int deadPl = 0;
    for (size_t i = 0; i < e.size(); i++) {
        if (e[i].checkDead() == true) {
            deadEn++;
        }
        if (p[i].checkDead() == true) {
            deadPl++;
        }
    }
    if (deadEn == 3 || deadPl == 3) {
        return true;
    }
    return false;
}

template <class obj>
void printVec(vector<obj> v) {
    for (size_t i = 0; i < v.size(); i++) {
        cout << "\tPlayer " << i+1 << " Health: " << v[i].getHealth() << endl;
    }
}


int main() {
    bool gameOver = false;
    //Create two teams with 3 enemies and 3 players
    Enemy e1;
    Enemy e2;
    Enemy e3;
    Player p1;
    Player p2;
    Player p3;
    vector<Enemy> ven;
    ven.push_back(e1);
    ven.push_back(e2);
    ven.push_back(e3);
    vector<Player> vpl;
    vpl.push_back(p1);
    vpl.push_back(p2);
    vpl.push_back(p3);
    
    srand(time(nullptr));

    while (gameOver == false) {
    
        
        cout << "~~~~~~~~~~~~~~~BEGIN TURN~~~~~~~~~~~~~~~~~\nENEMY TEAM: " << endl;
        printVec(ven);
        cout << "PLAYER TEAM: " << endl;
        printVec(vpl);
        cout << endl;
        
        queue<int> eventQueue;
        queue<pair<int,int>> fightQueue;
    
    
        //Choose team and fighter, and fills the queues
        for (int i = 0; i < 4; i++) {
            int enOrPl = rand()%2; //create a number either 0 or 1
            int fighterNum = rand()%3; // random number 0, 1, or 2
            int retVal;
            
            pair<int, int> teamPlay (enOrPl, fighterNum);
            if (enOrPl == 0) {
                cout << "Player " << fighterNum + 1 << ", ";
                retVal = vpl[fighterNum].getChoice();
            }
            else if (enOrPl == 1) {
                retVal = ven[fighterNum].getChoice();
            }
            eventQueue.push(retVal);
            fightQueue.push(teamPlay);
        }
    
        //Calls the attacks methods
        for (int i = 0; i < 4; i++) {
            if (fightQueue.front().first == 0) {
                if (eventQueue.front() == 1) {
                    cout << "Player " << fightQueue.front().second + 1 << ", ";
                    vpl[fightQueue.front().second].attack(ven);
                }
                else if (eventQueue.front() == 2) {
                    cout << "Player " << fightQueue.front().second + 1 << " countered";
                    vpl[fightQueue.front().second].counter();
                    
                }
                else {
                    cout << "Player " << fightQueue.front().second+1 << " rested\n";
                    vpl[fightQueue.front().second].rest();
                }
            }
            else {
                if (eventQueue.front() == 1) {
                    cout << "Enemy " << fightQueue.front().second+1 << " attacked";
                    ven[fightQueue.front().second].attack(vpl);
                }
                else if (eventQueue.front() == 2) {
                    cout << "Enemy " << fightQueue.front().second+1 << " countered";
                    ven[fightQueue.front().second].counter();
                }
                else {
                    cout << "Enemy " << fightQueue.front().second+1 << " rested\n";
                    ven[fightQueue.front().second].rest();
                }
            }
            fightQueue.pop();
            eventQueue.pop();
        }
        

        cout << "~~~~~~~~~~~~~~~~END TURN~~~~~~~~~~~~~~~~~~" << endl;
    
       if (checkAllDead(ven, vpl) == true) {
           cout << "~~~~~~~~~~~~~~~GAME OVER~~~~~~~~~~~~~~~~" << endl;
            gameOver = true;
        }
        
    }
    
}