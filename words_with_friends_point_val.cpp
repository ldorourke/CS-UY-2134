/*
 Lucas O'Rourke
 lor215
 hw08_2.cpp
 
 Finds Scrabble point values for every word in the dictionary
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;


// Get point values from the file and store them in the vector
void initValues (vector<int>& pointValues) {
    ifstream ifs("/Users/lucasorourke/Desktop/Data Structures/hw08/hw08_2/hw08_2/WWF_Letter_point_value.txt");
    if (!ifs) {
        cout << "Could not open file!" << endl;
        return;
    }
    string point_val;
    string letter;
    for (size_t i = 0; i < pointValues.size(); i++) {
        getline(ifs, point_val, ' ');
        getline(ifs, letter, '\n');
        int point_value = atof(point_val.c_str());
        char let = letter[0];
        for (int t = 65; t <= 90; t++) {
            if (let == char(t)) {
                pointValues[t-65] = point_value;
            }
        }
    }
    ifs.close();
}


// Compute point value of a word
int wordValue (const vector<int>& pointValues, const string& word) {
    int point_val = 0;
    for(int i = 0; i < word.size(); i++) {
        for (size_t t = 0; t < pointValues.size(); t++) {
            if ((int(word[i])-97) == t) {
                point_val += pointValues[t];
            }
        }
    }
    return point_val;
}

// Store all words and their point values in the map
void initWords (const vector<int>& pointValues, map<string, int>& wordValues) {
    ifstream ifs("/Users/lucasorourke/Desktop/Data Structures/hw08/hw08_2/hw08_2/WWF_dictionary.txt");
    if (!ifs) {
        cout << "Could not open file!" << endl;
        return;
    }
    string word;
    int point_val;
    while (ifs) {
        getline(ifs, word);
        point_val = wordValue(pointValues, word);
        wordValues[word] = point_val;
    }
    ifs.close();
    
}

SCENARIO("Testing words with friends"){
    GIVEN("Given a map and vector"){
        vector<int> pointValues(26,0);
        map<string, int> wordValues;
        WHEN("The map is initialized with this vector with word values"){
            initValues(pointValues);
            initWords(pointValues, wordValues);
            THEN("abacterial should be valued 17"){
                string test = "abacterial";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 17);
            }
            THEN("zyzzyva should be valued 42"){
                string test = "zyzzyva";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 42);
            }
            THEN("fuzees should be valued 19"){
                string test = "fuzees";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 19);
            }
            THEN("poly should be valued 10"){
                string test = "poly";
                map<string, int>::iterator itr = wordValues.find(test);
                REQUIRE(itr->second == 10);
            }
        }
    }
}

