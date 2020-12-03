#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
using namespace std;

struct Player {										// Player object 
	string name;
	double scores[18];									// array of scores, index 0 stores total points for the season, 1-17 store corresponding weekly scores
	double oneC, twoC = 0;								// complement value (points to be gained through substitution) to the starter, twoC used only for RB2 and WR2

	Player(string name, double total) {
		this->name = name;
		scores[0] = total;
	}
	Player() {
		name = "";
	}
};

/*Player* findComplement(vector<Player>& players, Player* starter) {

}

void printStats() {

}*/

int main() {
	unordered_map<string, Player> QBs, RBs, WRs, TEs;		// map from player name to their object allows for O(1) lookup by name
	Player *QB = nullptr, *RB1 = nullptr, *RB2 = nullptr, *WR1 = nullptr, *WR2 = nullptr, *TE = nullptr;				// starters
	Player *QBc, *RB1c, *RB2c, *WR1c, *WR2c, *TEc;			// complements

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

	while (getline(seasonStats, temp, ',')) {				// read in season stats, record starters
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

	cout << "QB: " << QB->name << endl;			// <- for testing
	cout << "RB1: " << RB1->name << endl;
	cout << "RB2: " << RB2->name << endl;
	cout << "WR1: " << WR1->name << endl;
	cout << "WR2: " << WR2->name << endl;
	cout << "TE: " << TE->name << endl;

	// open weekly files
	/**for () {
		// create maxHeap of Player* for each position
		// for each maxHeap
			// calculate top's complement values, remove from heap until top has 0 complement value
	}
	QBc = findComplement(QBs, QB);
	TEc = findComplement(TEs, TE);

	printStats();*/
}