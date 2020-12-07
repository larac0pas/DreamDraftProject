#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>
using namespace std;

struct Player {														// Player object
    string name;
    double scores[18] = { 0 };										// array of scores, index 0 stores total points for the season, 1-17 store corresponding weekly scores
    double comp1 = 0, comp2 = 0;									// complement value (points to be gained through substitution) to the starter, comp2 used only for RB2 and WR2

    Player(string name, double total) {
        this->name = name;
        scores[0] = total;
    }
    Player() {
        name = "";
    }
};

class Map{
public:
    void calculateComplement(unordered_map<string, Player>& players, Player* starter, int week);
    void calculateComplements(unordered_map<string, Player>& players, Player* starter1, Player* starter2, int week);
    Player* findComplement(unordered_map<string, Player>& players);
    pair<Player*, Player*> findComplements(unordered_map<string, Player>& players);

};

void Map::calculateComplement(unordered_map<string, Player>& players, Player* starter, int week) { // updates complement values for players in given map to given starter in given week
    for (auto it : players) {
        if (it.second.name != starter->name) {						// starter cannot have complement value
            double complement = it.second.scores[week] - starter->scores[week];
            if (complement > 0.0) {
                Player* p = &players[it.second.name];
                p->comp1 += complement;
            }
        }
    }
}

void Map::calculateComplements(unordered_map<string, Player>& players, Player* starter1, Player* starter2, int week) { // needed to store comp2 values directly
    for (auto it : players) {
        if (&players[it.second.name] != starter1 && &players[it.second.name] != starter2) {	// excludes both starters
            Player* p = &players[it.second.name];
            double complement = it.second.scores[week] - starter1->scores[week];
            if (complement > 0.0)
                p->comp1 += complement;
            complement = it.second.scores[week] - starter2->scores[week];
            if (complement > 0.0)
                p->comp2 += complement;
        }
    }
}


Player* Map::findComplement(unordered_map<string, Player>& players) {
    double maxComp = 0;
    Player* sub = nullptr;
    for (auto it : players) {
        if (it.second.comp1 > maxComp) {
            maxComp = it.second.comp1;
            sub = &players[it.second.name];							// do not change this line
        }
    }
    return sub;
}

pair<Player*, Player*> Map::findComplements(unordered_map<string, Player>& players) {
    double maxComp1 = 0, maxComp2 = 0;
    Player* sub1a = nullptr, * sub1b = nullptr, * sub2a = nullptr, * sub2b = nullptr;
    for (auto it : players) {
        if (it.second.comp1 > maxComp1) {
            maxComp1 = it.second.comp1;
            sub1b = sub1a;
            sub1a = &players[it.second.name];
        }
        if (it.second.comp2 > maxComp2) {
            maxComp2 = it.second.comp2;
            sub2b = sub2a;
            sub2a = &players[it.second.name];
        }
    }
    if (sub1a == sub2a && sub1a != nullptr) {						// if the same player is the best complement to both starters
        if (sub1a->comp1 >= sub1a->comp2) 							// the higher complement value is chosen, and the next highest complement to the other starter is chosen
            sub2a = sub2b;
        else
            sub1a = sub1b;
    }
    return { sub1a, sub2a };
}

int main() {
    unordered_map<string, Player> QBs, RBs, WRs, TEs;
    Player* QB = nullptr, * RB1 = nullptr, * RB2 = nullptr, * WR1 = nullptr, * WR2 = nullptr, * TE = nullptr;
    Player* QBc, * RB1c, * RB2c, * WR1c, * WR2c, * TEc;

    //object
    Map mapObj;

    // open season file
    string season, temp, name, pos, score;
    cout << "enter the season you would like to compute the perfect roster for: ";
    cin >> season;
    string filename = "data_v2/yearly/" + season + ".csv";
    ifstream seasonStats(filename);


    // read in season stats, record starters
    getline(seasonStats, temp);										// clear first line (headings)
    while (getline(seasonStats, temp, ',')) {
        getline(seasonStats, name, ',');							// reads in player name
        getline(seasonStats, temp, ',');
        getline(seasonStats, pos, ',');								// reads in position
        for (int i = 0; i < 23; i++) { getline(seasonStats, temp, ','); }
        getline(seasonStats, score);								// reads total season score, the last column
        double seasonTotal = stod(score);
        if (pos == "QB") {
            QBs[name] = Player(name, seasonTotal);
            if (QB == nullptr || seasonTotal > QB->scores[0])
                QB = &QBs[name];
        }
        else if (pos == "RB") {
            RBs[name] = Player(name, seasonTotal);
            if (RB1 == nullptr || seasonTotal > RB1->scores[0]) {
                RB2 = RB1;
                RB1 = &RBs[name];
            }
            else if (RB2 == nullptr || seasonTotal > RB2->scores[0])
                RB2 = &RBs[name];
        }
        else if (pos == "WR") {
            WRs[name] = Player(name, seasonTotal);
            if (WR1 == nullptr || seasonTotal > WR1->scores[0]) {
                WR2 = WR1;
                WR1 = &WRs[name];
            }
            else if (WR2 == nullptr || seasonTotal > WR2->scores[0])
                WR2 = &WRs[name];
        }
        else if (pos == "TE") {
            TEs[name] = Player(name, seasonTotal);
            if (TE == nullptr || seasonTotal > TE->scores[0])
                TE = &TEs[name];
        }
    }


    // open weekly files
    for (int i = 1; i <= 16; i++) {
        filename = "data_v2/weekly/" + season + "/week" + to_string(i) + ".csv";
        ifstream weekStats(filename);


        getline(weekStats, temp);
        while (getline(weekStats, name, ',')) {
            getline(weekStats, pos, ',');
            for (int j = 0; j < 14; j++) { getline(weekStats, temp, ','); }
            getline(weekStats, score, ',');
            getline(weekStats, temp);
            if (pos == "QB")
                QBs[name].scores[i] = stod(score);
            else if (pos == "RB")
                RBs[name].scores[i] = stod(score);
            else if (pos == "WR")
                WRs[name].scores[i] = stod(score);
            else if (pos == "TE")
                TEs[name].scores[i] = stod(score);
        }

        mapObj.calculateComplement(QBs, QB, i);
        mapObj.calculateComplements(RBs, RB1, RB2, i);
        mapObj.calculateComplements(WRs, WR1, WR2, i);
        mapObj.calculateComplement(TEs, TE, i);
    }


    QBc = mapObj.findComplement(QBs);
    pair<Player*, Player*> RBcs = mapObj.findComplements(RBs);
    RB1c = RBcs.first;
    RB2c = RBcs.second;
    pair<Player*, Player*> WRcs = mapObj.findComplements(WRs);
    WR1c = WRcs.first;
    WR2c = WRcs.second;
    TEc = mapObj.findComplement(TEs);

    cout << "Best possible 12-player roster on draft day: " << endl;
    cout << "QB: " << QB->name << endl;
    cout << "QBc: " << QBc->name  << " +" << QBc->comp1 << endl;
    cout << "RB1: " << RB1->name << endl;
    cout << "RB1c: " << RB1c->name << " +" << RB1c->comp1 <<endl;
    cout << "RB2: " << RB2->name << endl;
    cout << "RB2c: " << RB2c->name << " +" << RB2c->comp2 << endl;
    cout << "WR1: " << WR1->name << endl;
    cout << "WR1c: " << WR1c->name << " +" << WR1c->comp1 << endl;
    cout << "WR2: " << WR2->name << endl;
    cout << "WR2c: " << WR2c->name << " +" << WR2c->comp2 << endl;
    cout << "TE: " << TE->name << endl;
    cout << "TEc: " << TEc->name << " +" << TEc->comp1 << endl;
}