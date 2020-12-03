#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

struct Player {										// Player object 
	string name;									// Player name
	int scores[18];									// array of scores, index 0 stores total points for the season, 1-17 store corresponding weekly scores
	int oneC, twoC = 0;								// complement value (points to be gained through substitution) to the starter, twoC used only for RB2 and WR2
	Player() {
		name = "";
	}
	Player(string name) {
		this->name = name;
	}
};

Player* findComplement(vector<Player>& players, Player* starter) {

}

void printStats() {

}

int main() {
	vector<Player> QBs, RBs, WRs, TEs, DSTs, Ks;
	Player *QB, *RB1, *RB2, *WR1, *WR2, *TE, *DST, *K;
	Player *QBc, *RB1c, *RB2c, *WR1c, *WR2c, *TEc, *DSTc, *Kc;

	// open season file
	cout << "enter the season you would like to compute the perfect roster for: ";
	for () {// read in season stats, record starters

	}
	// open weekly files
	for () {
		// create maxHeap of Player* for each position
		// for each maxHeap
			// calculate top's complement values, remove from heap until top has 0 complement value
	}
	QBc = findComplement(QBs, QB);
	TEc = findComplement(TEs, TE);
	DSTc = findComplement(DSTs, DST);
	Kc = findComplement(Ks, K);

	printStats();
}