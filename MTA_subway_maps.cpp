/*
 Lucas O'Rourke
 lor215
 hw11q3xc.cpp
 LucasMaps for the NYC Subway system
 Test 2 with 902
 Test 3 with 111 to A38 (posted on Piazza)
 Test 3 with A38 to A41 (my commute)
 Test 3 with A41 to 701 (Jay st to queens)
 Test 4 with 901 (times square)
 
 */

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include "catch.hpp"
#include <math.h>
using namespace std;


class trainStopData {
public:
    trainStopData(const string& id, const string& name)
    : stop_id(id), stop_name(name) {}
    string get_id() const { return stop_id; }
    string get_stop_name() 	const { return stop_name; }
  //  double get_latitude() const { return stop_lat; }
  //  double get_longitude() const { return stop_lon; }
private:
    string stop_id;
    string stop_name;
   // double stop_lat; // latitude of train stop location
   // double stop_lon; // longitude of train stop location
};


struct vertexInf {
    int dist;
    string prev;
};


class LucasMaps {
public:
    LucasMaps();
    void parseTransferData();
    void parseTrainData();
    void open();
    void printpath(string j, const unordered_map<string, vertexInf> & vinfo);
    void shortestpaths(string s, string d);
    string findName(string id);
    void printTransfers(string s);
    
    /*
     FUNctors
     */
    class isStopOnRoute {
    public:
        isStopOnRoute(char c) : route(c) {}
        bool operator()(trainStopData stop) {
            if ((stop.get_id())[0] == route) {
                return true;
            }
            return false;
        }
    private:
        char route;
    };
    
    
    class printTrainStopInfo {
    public:
        void operator()(trainStopData stop) {
            cout << "\nStop ID: " << stop.get_id() << "\nStop Name: " << stop.get_stop_name() << endl;
        }
        
    };
    
    class isSubwayStop {
    public:
        isSubwayStop(string s) : stopId(s) {}
        bool operator()(trainStopData stop) {
            if (stop.get_id() == stopId) {
                return true;
            }
            return false;
        }
    private:
        string stopId;
    };
    

    
private:
    unordered_map<string, list<string>> nycMap;
    vector<trainStopData> trainStops;
};



/*
 Constructor Parses all the data
 */
LucasMaps::LucasMaps() {
    parseTransferData();
    parseTrainData();
}


/*
 Perform_if driver for option 1 and 2 on the menu
 */
template <class Iterator, class Pred, class Op>
int perform_if(Iterator itrStart, Iterator itrEnd, Pred pred, Op op) {
    int how_many = 0;
    for (Iterator itr = itrStart; itr != itrEnd; itr++) {
        if (pred(*itr)) {
            op(*itr);
            how_many++;
        }
    }
    return how_many;
}



/*
 Interactive menu for the user to engage with
 */
void LucasMaps::open() {
    cout << "Hello, welcome to LucasMaps for the NYC subway system (much better than Google Maps)!" << endl;
    bool seen_done = false;
    int user_inp;
    vector<trainStopData>::iterator itr1 = trainStops.begin();
    vector<trainStopData>::iterator itr2 = trainStops.end();
    while (seen_done == false) {
        cout << "Would you like to:\n1) Find all routes on a certain line?\n2) Print out all the information about a certain stop?\n3) *NEW* Go somewhere in the least amount of stops!\n4) *NEW* Oh shoot, I might have missed my transfer. What information do you have for my next stop?\n5) Quit\n>> ";
        cin >> user_inp;
        if (user_inp == 1) {
            char route;
            cout << "What route do you want to check? (Enter One Character): \n>> ";
            cin >> route;
            int returned = perform_if(itr1, itr2, isStopOnRoute(route), printTrainStopInfo());
            if (returned == 0) {
                cout << "Line not found" << endl;
            }
            cout << endl;
        }
        else if (user_inp == 2) {
            string stop;
            cout << "\nEnter stop ID: \n>> ";
            cin >> stop;
            int returned = perform_if(itr1, itr2, isSubwayStop(stop), printTrainStopInfo());
            if (returned == 0) {
                cout << "Stop not found!" << endl;
            }
            cout << "*NEW* Transfers available: ";
            printTransfers(stop);
            cout << endl << endl;
        }
        else if (user_inp == 3) {
            string origin;
            string dest;
            cout << "\nOkay lets get traveling, please enter your origin and desitnation stop ID\nOrigin >> ";
            cin >> origin;
            cout << "Destination >> ";
            cin >> dest;
            shortestpaths(origin, dest);
            
        }
        else if (user_inp == 4) {
            string prevID;
            cout << "\nWhat was the most recent stop ID\n>> ";
            cin >> prevID;
            cout << "Your most recent stop was " << findName(prevID) << endl;
            cout << "Your next stop is either:\n1) ";
            
            for (list<string>::iterator i = nycMap[prevID].begin(); i != nycMap[prevID].end(); i++) {
                if (prevID[0] == (*i)[0]) {
                    cout << findName(*i) << endl;
                    cout << "Is this your stop (Y/N)\n>> ";
                    char ans;
                    cin >> ans;
                    if (ans == 'Y' || ans == 'y') {
                        cout << "Transfers available: ";
                        printTransfers(*i);
                        break;
                    }
                    cout << "2) ";
                }
            }
            cout << endl;
        }
        else if (user_inp == 5) {
            cout << "Thank you! Bye!" << endl;
            seen_done = true;
        }
        else {
            cout << "Sorry not a valid input. Please choose a menu item!" << endl;
        }
    }
    
}



/*
 Gets the stop name from the stop ID
 */
string LucasMaps::findName(string id) {
    for (int i = 0; i < trainStops.size(); i++) {
        if (trainStops[i].get_id() == id) {
            return trainStops[i].get_stop_name();
        }
    }
    return "";
}


/*
 Prints all the transfers from a certain stop
 */
void LucasMaps::printTransfers(string s) {
    int count = 0;
    vector<string> printed;
    for (list<string>::iterator i = nycMap[s].begin(); i != nycMap[s].end(); i++) {
        if (s[0] != (*i)[0]) {
            count++;
            bool priAlr = false;
            for (int j = 0; j < printed.size(); j++) {
                if (printed[j] == *i) priAlr = true;
            }
            if (priAlr == false) {
                cout << (*i)[0] << ", ";
                printed.push_back(*i);
            }
        
        }
    }
    if (count == 0) {
        cout << "No transfers from this line" << endl;
    }
    else {
        cout << "trains" << endl;
    }
}



/*
 Printing the path from the origin to the destination
 */
void LucasMaps::printpath(string j, const unordered_map<string, vertexInf> & vinfo) {
    vector<string> stops;
    stack<string> t;
    string current = j;
    while (vinfo.find(current)->second.prev != "--") {
        t.push(current);
        current = vinfo.find(current)->second.prev;
    }
    while (!t.empty()) {
        stops.push_back(t.top());
        t.pop();
    }
    int temp = 1;
    //cout << "Beginning at " << findName(j) << endl;
    for (int i = 0; i < stops.size(); i++) {
        if (stops[i][0] != stops[i+1][0]) {
            cout << "After " << temp << " stop(s) on the " << stops[i][0] << " train,";
            if (stops[i+1] != "") {
                cout << " transfer to the " << stops[i+1][0] << " train at " << findName(stops[i+1]) << endl;
                
                temp = 0;
            }
            else {
                cout << " you have arrived at your destination: " << findName(stops[i]) << endl;
            }
        }
        temp++;
    }
   // cout << endl << stops.size() << " " << stops[0] << endl;
    cout << endl;
}



/*
 Finds the shortest path for every single vertex
 */
const int DEFAULT_VAL =  -1;

void LucasMaps::shortestpaths(string s, string d) {
    queue<string> q;             // q is the queue of vertex numbers
    if (nycMap.find(s) == nycMap.end()) {
        cout << "Invalid Origin" << endl;
        return;
    }
        unordered_map<string, vertexInf> vertices(nycMap.size()); // stores BFS info for the vertices
    // info for vertex j is in position j
    for (unordered_map<string, list<string>>::iterator i = nycMap.begin(); i != nycMap.end(); ++i) {
        vertices[i->first].dist = DEFAULT_VAL;
        vertices[i->first].prev = "--";
    }
    vertices[s].dist = 0;
    q.push(s);
    while  (!q.empty() ) {
        string v = q.front();
        q.pop();
        for (list<string>::const_iterator w = nycMap[v].begin(); w != nycMap[v].end(); w++) {
            if (vertices[*w].dist == DEFAULT_VAL) {        // distance of *w from source not determined yet
                vertices[*w].dist = vertices[v].dist+1;
                vertices[*w].prev = v;
                q.push(*w);
            }
        }
    }
    cout << "Okay, it is going to be " << vertices.find(d)->second.dist << " stops:" << endl << endl;
    unordered_map<string, list<string>>::iterator i = nycMap.find(d);
    cout << "Beginning at: " << findName(s) << " on the " << s[0] << " train" << endl;
    printpath(i->first, vertices);
}


/*
 The code from here until the next comment fills the unordered NYC map
 */
void LucasMaps::parseTrainData() {
    ifstream ifs("/Users/lucasorourke/Desktop/Data Structures/hw10/hw10q2/hw10q2/MTA_train_stop_data.txt");
    // ifstream ifs("MTA_train_stop_data.txt");
    if (!ifs) {
        cerr << "The file for train stop data cannot be found.\n";
        exit(1);
    }
    string line;
    getline(ifs, line);
    
    //This while loop fills a vector with trainStopData
    while (getline(ifs, line)) {
        string element;
        stringstream eachLine(line);
        vector<string> allElements;
        while (getline(eachLine, element, ',')) { allElements.push_back(element); }
        trainStopData t(allElements[0], allElements[2]);
        if (trainStops.empty()) {
            trainStops.push_back(t);
        }
        else {
            if (t.get_stop_name() != trainStops[trainStops.size() - 1].get_stop_name()) {
                trainStops.push_back(t);
            }
        }
    }
    ifs.close();
    
    //This for loop adds all the items from the vector created above into the map
    for (size_t i = 0; i < trainStops.size(); i++) {
        if (i == 0) {
            nycMap[trainStops[i].get_id()].push_back(trainStops[i+1].get_id());
        }
        else if (i == trainStops.size() -1) {
            nycMap[trainStops[i].get_id()].push_back(trainStops[i-1].get_id());
        }
        else {
            if (trainStops[i].get_id()[0] == trainStops[i+1].get_id()[0]) {
                nycMap[trainStops[i].get_id()].push_back(trainStops[i+1].get_id());
            }
            if (trainStops[i].get_id()[0] == trainStops[i-1].get_id()[0]) {
                nycMap[trainStops[i].get_id()].push_back(trainStops[i-1].get_id());
            }
        }
    }
}



void LucasMaps::parseTransferData() {
    ifstream ifs("/Users/lucasorourke/Desktop/Data Structures/hw10/hw10q2/hw10q2/MTA_transfers.txt");
    //ifstream ifs("transfers.txt");
    if (!ifs) {
        cerr << "The file for train stop data cannot be found.\n";
        exit(1);
    }
    string line;
    getline(ifs, line);
    while (getline(ifs, line)) {
        string element;
        stringstream eachLine(line);
        vector<string> allElements;
        while (getline(eachLine, element, ',')) {
            allElements.push_back(element);
        }
        if (allElements[0] != allElements[1]) {
            nycMap[allElements[0]].push_back(allElements[1]);
            nycMap[allElements[1]].push_back(allElements[0]);
        }
    }
}



/*
 Main opens up the LucasMaps "application"
 */
int main() {
    LucasMaps maps;
    maps.open();
}
