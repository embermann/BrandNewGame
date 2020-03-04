#include <iostream> 
#include <string>
using namespace std;

const int AbilityLibSize = 20;

Ability abilityLib[20];

void abilityLib_filling() {

	for (int i = 0; i < 20; i++)
	{
		// здесь программа заполняет массив abilityLib[20]
		// из файла что бы хранить базу абилок его в ОЗУ
	}

}


class Weapon {
private: 
	char name[30];
public:
	int area;

	void print_name() {

		cout << "Country name: " << name << endl;

	}

	void setArea() {

		cout << "Write country area: ";
		cin >> area;

	}


};

class Ability { 
public:
	short int ID; // ид скилла

	// base info
	bool active; // активная или пассивная
	int damage;
	int element; // тип атаки
	int duration; // длительность


	int target; // куда направленна атака (player, enemy, area, line, teammates)
	int origin; // начало скилла(позиция в мире)
	int end; // где заканчивается скилл(позиция)


	

	

	short int stat_ID; // на что воздействует (параметр игрока)
	bool add; // если true то +, если false то *
	float modifier; // на сколько менять


	// затраты на абилку
	int what_costs; // что тратится
	int cost; // в каком колличестве




};

class Skill:Ability {
private:
	int ability_count;
	short int *ability_id = new short int[ability_count];
	int cost;
	int what_costs;
public:

	int ab() {

		cout << abilityLib[1].cost;

	}

};

class Summons {
private:
	Character_stats stats;

public:


};

class Player {
private:
	Ability skill[5];

public:

	bool use_skill(int which_skill_was_used) {

			

	}


};

class NPC:Player {



};


struct Character_stats {

	int hp;
	int defense;
	int attack_speed;
	int move_speed;
	

};

struct Teammate:Character_stats {

	int teammate_count;


};



int main() {
	

	

	

	


	return 0;
}