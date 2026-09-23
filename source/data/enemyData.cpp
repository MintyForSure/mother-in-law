//
// Created by tailofhell on 8/28/26.
//

#include "enemyData.h"
#include "cheesyRat_png.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
/*alignments declared with
w - weak/susceptible
r - resist/not susceptible
i - null/immune
n - neutral
*/

struct enemyData {
    int maxHp;
    int hp;
    int maxPp;
    int pp;
    int atk;
    int def;
    int spd;
    char hypnosis;
    char para;
    char phys;
    char fire;
    char ice;
    char elec;
};

class cheesyRat {
    public:
    char name;
    int maxHp;
    int hp;
    int maxPp;
    int pp;
    int atk;
    int def;
    int spd;
    char hypnosis;
    char para;
    char phys;
    char fire;
    char ice;
    char elec;

};

vector<int> getEnemyStats(const string &enemy) {
    vector<int> stats;
    //returns vector with stats being maxhp,maxpp,atk,def,speed
    if (enemy=="cheesyRat") {
        stats = {30, 0, 2, 2, 1};
    }
    return stats;
}

int getEnemyData(const string &enemy,string stat) {
    if (enemy == "cheesyRat") {
        cheesyRat cheesyRatObj{};
        cout << cheesyRatObj.name << endl;
        cheesyRatObj.name=*"Cheesy Rat";
        cheesyRatObj.maxHp=30;
        cheesyRatObj.maxPp=0;
        cheesyRatObj.atk=3;
        cheesyRatObj.def=2;
        cheesyRatObj.spd=1;
        cheesyRatObj.hypnosis='w';
        cheesyRatObj.para='n';
        cheesyRatObj.phys='w';
        cheesyRatObj.fire='n';
        cheesyRatObj.ice='n';
        cheesyRatObj.elec='i';
    }
    return 1;
}
