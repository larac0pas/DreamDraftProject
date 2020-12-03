#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>
using namespace std;

struct Player {										// Player object 
	string name;
	double scores[18] = { 0 };								// array of scores, index 0 stores total points for the season, 1-17 store corresponding weekly scores
	double comp1, comp2 = 0;								// complement value (points to be gained through substitution) to the starter, twoC used only for RB2 and WR2

	Player(string name, double total) {
		this->name = name;
		scores[0] = total;
	}
	Player() {
		name = "";
	}
};

Player* findComplement(unordered_map<string, Player>& players) {
	double maxComp = 0;
	Player* sub = nullptr;
	for (auto it : players) {
		if (it.second.comp1 > maxComp) {
			maxComp = it.second.comp1;
			sub = &it.second;
		}
	}
	return sub;
}



int main() {
	unordered_map<string, Player> QBs, RBs, WRs, TEs;			// map from player name to their object allows for O(1) lookup by name
	Player* QB = nullptr, * RB1 = nullptr, * RB2 = nullptr, * WR1 = nullptr, * WR2 = nullptr, * TE = nullptr;				// starters
	Player* QBc, * RB1c, * RB2c, * WR1c, * WR2c, * TEc;			// complements

	// open season file
	string season, temp, name, pos, score;
	cout << "enter the season you would like to compute the perfect roster for: ";
	cin >> season;
	string filename = "data_v2/yearly/" + season + ".csv";
	cout << filename << endl;
	ifstream seasonStats(filename);
	if (seasonStats.is_open())
		cout << "file opened successfully" << endl;
	else
		cout << "file not opened" << endl;
	getline(seasonStats, temp);								// clear first line (headings)

	// read in season stats, record starters
	while (getline(seasonStats, temp, ',')) {				
		getline(seasonStats, name, ',');					// reads in player name				
		getline(seasonStats, temp, ',');
		getline(seasonStats, pos, ',');						// reads in position
		for (int i = 0; i < 23; i++) { getline(seasonStats, temp, ','); }
		getline(seasonStats, score);						// reads total season score, the last column
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
	for (int i = 1; i <= 17; i++) {
		priority_queue<Player*> QBm, RBm, WRm, TEm;									// create maxHeap of Player* for each position
		filename = "data_v2/weekly/" + season + "/week" + to_string(i) + ".csv";
		ifstream weekStats(filename);
		if (weekStats.is_open())
			cout << "file opened successfully" << endl;
		else
			cout << "file not opened" << endl;

		getline(weekStats, temp);
		while (getline(weekStats, name, ',')) {						// reads weeks data
			getline(weekStats, pos, ',');
			for (int j = 0; j < 15; j++) { getline(weekStats, temp, ','); }
			getline(weekStats, score, ',');
			getline(weekStats, temp);
			if (pos == "QB") {
				QBs[name].scores[i] = stod(score);					// adds weekly scores to Player arrays
				if (name != QB->name)								// does not add the starters to the maxHeap
					QBm.push(&QBs[name]);
			}
			else if (pos == "RB") {
				RBs[name].scores[i] = stod(score);
				if (name != RB1->name && name != RB2->name)
					RBm.push(&RBs[name]);
			}
			else if (pos == "WR") {
				WRs[name].scores[i] = stod(score);
				if (name != WR1->name && name != WR2->name)
					WRm.push(&WRs[name]);
			}
			else if (pos == "TE") {
				TEs[name].scores[i] = stod(score);
				if (name != TE->name)
					TEm.push(&TEs[name]);
			}
		}

		// for each maxHeap
			// calculate top's complement values, remove from heap until top has 0 complement value
		while (QBm.top()->scores[i] - QB->scores[i] > 0 && !QBm.empty()) {
			QBm.top()->comp1 += QBm.top()->scores[i] - QB->scores[i];
			QBm.pop();
		}
		while ((RBm.top()->scores[i] - RB1->scores[i] > 0 || RBm.top()->scores[i] - RB2->scores[i] > 0) && !RBm.empty()) {
			if (RBm.top()->scores[i] - RB1->scores[i] > 0)
				RBm.top()->comp1 += RBm.top()->scores[i] - RB1->scores[i];
			if (RBm.top()->scores[i] - RB2->scores[i] > 0)
				RBm.top()->comp2 += RBm.top()->scores[i] - RB2->scores[i];
			RBm.pop();
		}
		while ((WRm.top()->scores[i] - WR1->scores[i] > 0 || WRm.top()->scores[i] - WR2->scores[i] > 0) && !WRm.empty()) {
			if (WRm.top()->scores[i] - WR1->scores[i] > 0)
				WRm.top()->comp1 += WRm.top()->scores[i] - WR1->scores[i];
			if (WRm.top()->scores[i] - WR2->scores[i] > 0)
				WRm.top()->comp2 += WRm.top()->scores[i] - WR2->scores[i];
			WRm.pop();
		}
		while (TEm.top()->scores[i] - TE->scores[i] > 0 && !TEm.empty()) {
			TEm.top()->comp1 += TEm.top()->scores[i] - TE->scores[i];
			TEm.pop();
		}
	}
	QBc = findComplement(QBs);
	TEc = findComplement(TEs);

	cout << "QB: " << QB->name << endl;
	cout << "QBc: " << QBc->name << endl;
	cout << "RB1: " << RB1->name << endl;
	cout << "RB2: " << RB2->name << endl;
	cout << "WR1: " << WR1->name << endl;
	cout << "WR2: " << WR2->name << endl;
	cout << "TE: " << TE->name << endl;
	cout << "TEc: " << TEc->name << endl;
}