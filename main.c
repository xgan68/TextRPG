#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

void waitForAnyKey()
{
	cin.ignore();
	while (1)
	{
		if ('\n' == getchar())
       			break;
	}	
}
   
class SpecialItem;

class Entity
{
public:
	
	friend class SpecialItem;
	
	vector<SpecialItem*> items;

	string description;
	string name;

	int value;
	int health;
	int initHealth;
	int shield;
	int strength;
	bool flagDead;
public:
	Entity() {initHealth = health;}
	~Entity() { ; }

	virtual void update() = 0;
	const string getName() const { return name; }
	const string getDescription() const { return description; }
	
	void checkDead()
	{
		if(health <= 0)
			flagDead = true;
	}

	bool isDead(){ return flagDead; }

	virtual void takeDamage(int damage)
	{
		int realDamage = damage - shield;

		if(realDamage <= 0)
			cout << name << " did not take any damage..." << endl;
		else
			cout << name << " took " << realDamage << " damage!!" << endl;

		health -= realDamage;
	}

	virtual void attack (Entity* player) = 0;
	
	void insertItems(vector<SpecialItem*> given_items)
	{
		items = given_items;
	}
	
};

class SpecialItem
{
protected:
	int hp_mod;
	int str_mod;
	int shld_mod;
	enum item_type { ENHANCER, WEAPON, SPELL };
	bool hidden;
	int use_count;
	item_type type;
	string name, description;

public:
	SpecialItem(int _hp = 1, int _str = 1, int _shld = 1, item_type _type = ENHANCER, int _count = 1, bool _hidden = false)
	{	
		hp_mod = _hp;
		str_mod = _str;
		shld_mod = _shld;
		type = _type;
		use_count = _count;
	}

	~SpecialItem() {;}
		
	
	const string getName() const { return name; }
	const string getDescription() const { return description; }
	const int getHealthMod() const { return hp_mod; }
	const int getShieldMod() const { return shld_mod; }
	const int getStrengthMod() const { return str_mod; }
	const item_type getType() const { return type; }
	
	virtual void onUse(Entity* user, Entity* target) 
	{ 
		if(type == SpecialItem::ENHANCER)
		{
			target->health += getHealthMod();
			target->shield += getShieldMod();	
			target->strength += getStrengthMod();
		
			if(getHealthMod() > 0)
				cout << target->name << "\'s health is now increased by +" << getHealthMod() << " points!" << endl;
			else if(getHealthMod() < 0)
				cout << target->name << "\'s health has been damaged " << getHealthMod() << " points by a curse!" << endl;
	
			if(getShieldMod() > 0)
				cout << target->name << "\'s shield is much greater by +" << getShieldMod() << " points!" << endl;
			else if(getShieldMod() < 0)
				cout << target->name << "\'s shields have weakened! " << getShieldMod() << " points lost by a curse!" << endl;
	
			if(getStrengthMod() > 0)
				cout << target->name << "\'s strength hath increased by +" << getStrengthMod() << " points!" << endl;
			else if(getStrengthMod() < 0)
				cout << target->name << "\'s strength has been decreased " << getStrengthMod() << " points by a curse!" << endl;
		}
		else if(type == SpecialItem::WEAPON || type == SpecialItem::SPELL)
		{
			target->takeDamage(abs(getHealthMod())+user->strength);
			target->shield += getShieldMod();	
			target->strength += getStrengthMod();
			
			if(getShieldMod() > 0)
				cout << target->name << "\'s shield is much greater by +" << getShieldMod() << " points!" << endl;
			else if(getShieldMod() < 0)
				cout << target->name << "\'s shields are down!" << getShieldMod() << " points lost by a curse!" << endl;
	
			if(getStrengthMod() > 0)
				cout << target->name << "\'s strength hath increased by +" << getStrengthMod() << " points!" << endl;
			else if(getStrengthMod() < 0)
				cout << target->name << "\'s strength has been decreased " << getStrengthMod() << " points by a curse!" << endl;	
		}
		
		if(use_count > 0)
			use_count--;
	}
	
	const bool remove() const { return (use_count == 0); }
	
	void showStats()
	{
		cout << endl << "..``..``..``..``..``..``.." << endl;
		
		if(!hidden)
		{
			cout << name << endl << description;
			cout << endl << "[Type] := ";
		
			if(type == WEAPON)
				cout << "weapon";
			else if(type == ENHANCER)
				cout << "enhancer";
			else if(type == SPELL)
				cout << "spell";
			
			cout << endl << "[Health] := " << hp_mod;
			cout << endl << "[Sheild] := " << shld_mod;
			cout << endl << "[Strength] := " << str_mod;
			
			if(use_count >= 0)
				cout << endl << "[Uses Left] := " << use_count;
			else
				cout << endl << "[Uses Left] := /INF/";
		}
		else
		{
			cout << name << endl << description;
			cout << endl << "[Type] := ???";
			cout << endl << "[Value] := ???";
			cout << endl << "[Health] := ???";
			cout << endl << "[Sheild] := ???";
			cout << endl << "[Strength] := ???";
			cout << endl << "[Uses Left] := ???";
		}
		
		cout << endl << "..``..``..``..``..``..``.." << endl;

		cout << "[Press Enter key to continue]" << endl;
		
		waitForAnyKey();
	}
};

/*
Some useful functions for enemies --- UNIMPLEMENTED

void checkForItemsAndUse(Entity* self, vector<Entity*> surroundings)
{
	vector<SpecialItem*>::iterator h;
	
	if(self->items.size() > 0)
	{ 
		/*
			Go through items and see if they are any help
			in a scenario. I.E. if health is too low and 
			something can change that, then use it.
			If there is a sword or something and the use_count
			is high enough, then use it. Things like that.
			 - Be smart
			 - Be resourceful
		
		 
		int overHaul, base;
		base = self->health + self->strength + self->shield;
		SpecialItem* bestOnSelf = 0;
		SpecialItem* bestOnPlayer = 0;
				
		for(h = self->items.begin(); h != self->items.end(); h++)
		{
			if(self->health < (*h)->getHealthMod() + self->health)
			{
				// So it can mod health but should we?
				// If health is too low, do it
				// If it's "okay" randomly determine
				int random = rand() % 5 + 1;
					
				if(static_cast<float>(self->health) <= static_cast<float>(self->initHealth)*0.50)
				{
					if(random == 5)
					{
						(*h)->onUse(self, self);
						return;
					}
				}
				else if(static_cast<float>(self->health) <= static_cast<float>(self->initHealth)*0.25)
				{
					(*h)->onUse(self, self);	
				}
					
				// Okay, now for stats. If something will increase them, then use it.
				// But can randomly be used on another ally.
				// If decrease, use it on an opponent. 
				
				//Check if it increases and find the best one
				overHaul = self->health + item->getHealthMod() 
					+ self->strength + item->getStrengthMod() 
					+ self->shield + item->getShieldMod();
					
				if(overHaul > base)
				{
					best = (*h);
				}
			}
				
				//It does? Use it on yourself or an ally
				int rand_on_self = rand() % surroundings.size() * 4;
				int rand_who_to_use = rand() % surroundings.size() * 2;
					
				if(rand_on_self >= surroundings.size()/2)
				{
					//
				}
					
			}
				
		}
	}

}*/
	
class Goblins : public Entity
{
public:

	Goblins(string _name, string _description) : Entity()
	{
		name = _name;
		description = _description;
		health = 8;
		shield = 1;
		strength = 4;
		value = 1;
		flagDead = false;
	}

	~Goblins() { ; }

	void update()
	{
		checkDead();

		if(isDead()){
			cout << name << " is slain. The corpse falls." << endl;
			return;}

		if(health < 8)
		{
			cout << name << " healed itself by 1 point!" << endl;
			health += 1;
		}
		else
		{
			cout << name << " is in attack position." << endl;
		}
	}

	void attack(Entity *player)
	{
		int random = rand() % 10 + 1;

		if(random == 5 || random == 10)
		{
			cout << name << " lundged at " << player->name << "!!" << endl;

			player->takeDamage(strength);
		}
		else
			cout << name << " lundged... and missed." << endl;
	}
};

class Skeleton: public Entity
{
public:

	Skeleton(string _name, string _description, int _str = 2, int _hp = 10) : Entity()
	{
		name = _name;
		description = _description;
		health = _hp;
		shield = 0;
		strength = _str;
		value = 0;
		flagDead = false;
	}

	~Skeleton() { ; }

	void update()
	{
		checkDead();

		if(isDead()){
			cout << name << " is slain. The corpse falls." << endl;
			return;}

		int random = rand()% 4 + 1;

			if(random == 3)
				cout << name << " is making jokes about your mother." << endl;
			else
			{
				if(shield < 4)
				{
					shield++;
					cout << name << " used Dark Magic to increase it's defense!!" << endl;
				}
			}
	}

	void attack(Entity *player)
	{
		int random = rand() % 10 + 1;

		if(random == 5 || random == 10)
		{
			cout << name << " charged at " << player->name << endl;

			player->takeDamage(strength);
		}
		else
			cout << name << " has refused to move." << endl;
	}
};

class Dragon: public Entity
{
public:

	Dragon(string _name, string _description, int _hp = 20) : Entity()
	{
		name = _name;
		description = _description;
		health = _hp;
		shield = 3;
		strength = 6;
		value = 200;
		flagDead = false;
	}

	~Dragon() { ; }

	void update()
	{
		checkDead();

		if(isDead()){
			cout << name << " is slain. The corpse falls." << endl;
			return;}

		int random = rand()% 4 + 1;

			if(random == 3)
				cout << name << " flares its nostrils. Fumes cloud the air." << endl;
			else if(random == 2)
				cout << name << " intimidates you with a mighty roar." << endl;
	}

	void attack(Entity *player)
	{
		int random = rand() % 10 + 1;

		if(random == 5 || random == 10)
		{
			cout << name << " breathed scorching fire at " << player->name << "!!" << endl;
			player->takeDamage(strength+3);
		}
		else if(random == 3 || random == 6)
		{
			cout << name << " slams " << player->name << " into a wall!" << endl;
			player->takeDamage(strength);
		}
		else
			cout << name << " goes to kill you with it's fearsome claws but misses." << endl;
	}
};

class Troll: public Entity
{
int charge;
public:

	Troll(string _name, string _description, int _str = 4, int _shld = 2, int _hp = 12) : Entity()
	{
		name = _name;
		description = _description;
		health = _hp;
		shield = _shld;
		strength = _str;
		value = 400;
		flagDead = false;
		charge = 0;
	}

	~Troll() { ; }

	void update()
	{
		checkDead();

		if(isDead()){
			cout << name << " is slain. The corpse falls with a loud thud." << endl;
			return;}

		int random = rand()% 40 + 1;

			if(random >= 30)
				cout << name << " stumbles around." << endl;
			if(random >= 0 && random <=10)
			{
				if(charge < 3)
				{
					cout << name << " is holding back strength for an ultimate attack!" << endl;
					charge++;
				}
			}
	}

	void attack(Entity *player)
	{
		int random = rand() % 10 + 1;

		if(charge >= 3)
		{
			cout << name << " lifts parts of the ground into the form of a boulder!" << endl << "He throws it at " << player->name << "!!" 
				<< endl;
			player->takeDamage(strength+5);
			charge = 0;
		}
		else{

		if(random == 5 || random == 10)
		{
			cout << name << " clubs " << player->name << "!!" << endl;
			player->takeDamage(strength);
		}
		else if(random == 2 || random == 4)
			cout << name << " trips and falls to the ground." << endl;
		else
			cout << name << " attacks and misses." << endl;
		}
	}
};

class LongSword : public SpecialItem
{
	public:
	LongSword() : SpecialItem(2, 0, 0, SpecialItem::WEAPON, 20)
	{
		 name = "Bronze Long Sword"; 
		 description = "Basic sword. Somewhat rusty but still useful.";
	}
	
	~LongSword() { ; }
	
	void onUse(Entity* user, Entity* target) 
	{ 
		cout << user->name << " swings the " << name << " at " << target->name << "!" << endl;		
		SpecialItem::onUse(user, target);
	}
};

class Medalion : public SpecialItem
{
	public:
	Medalion(int _hp, int _str, int _shld, string n, string d) 
		: SpecialItem(_hp, _str, _shld, SpecialItem::ENHANCER, 10)
		{
			name = 	n;
			description = d;
		}
		
	~Medalion() {;}
	
	void onUse(Entity* user, Entity* target) 
	{ 
		cout << user->name << " points the " << name << " at " << target->name << "!\nIt glows bright violet!" << endl;		
		SpecialItem::onUse(user, target);
	}
};

class Potion : public SpecialItem
{
	public:
	Potion() : SpecialItem(6, 0, 0,	SpecialItem::ENHANCER)
	{
		name = "Potion";
		description = "Will restore +6 health points.";
	}
	
	~Potion() { ;}
};

class CastFireball : public SpecialItem
{
	public:
	CastFireball() : SpecialItem(3, 0, 0, SpecialItem::SPELL)
	{
		name = "Cast Fireball";
		description = "The pyro-methodic teachings of Urogoth";
	}
	
	void onUse(Entity* user, Entity* target) 
	{ 
		cout << user->name << " shoots a large ball of white fire at " << target->name << "!" << endl;		
		SpecialItem::onUse(user, target);
	}
	
	~CastFireball() { ;}
};

class Hero : public Entity
{
protected:
bool inBattle;
bool blessed;
bool gameOver;
int round;
vector<Entity*> area;

public:
	~Hero() { ; }
	
	void insertSurroundings(vector<Entity*> surrounding)
	{
		area = surrounding;
		inBattle = true;

		cout << endl << "A(n) ";

		int it = 0;
		for(vector<Entity*>::iterator i = area.begin(); i != area.end(); i++)
		{
			it++;

			if(area.size() > 2)
			{
				cout << (*i)->name;
				if(it != area.size() - 1)
				{
					if(it != area.size())
					{
						cout << ", ";
					}
				}
				else
					cout << ", and ";
			}
			else
			{
				cout << (*i)->name;
				if(it == 1 && area.size() == 2)
					cout << " and ";
			}
		}
	
		cout << " has appeared!" << endl;
	}
	
	Hero(string _name) : Entity()
	{
		name = _name;
		value = 9001;
		health = 20;
		strength = 3;
		shield = 1;
		flagDead = false;
		blessed = false;
		inBattle = false;
		gameOver = false;
		round = 1;
	}

	const int getRound() const { return round; }

	bool isGameOver() { return gameOver; }

	void setGameState(bool isOver)
	{
		gameOver = isOver;
	}

	bool isInBattle() { return inBattle;}

	void setBattleState(bool true_false)
	{
		inBattle = true_false;
	}

	void atWin()
	{
		
		/*
			After specific rounds, likewise specific things should happen
		*/
		
		if(round == 1)
		{
			//Give them the Bronze Long Sword
			cout << "You stepped on something... It was a Bronze Long Sword!" << endl;
			items.push_back(new LongSword());	
		}
		
		int random = rand() % 100 + 1;

		if(random >= 10 && random <= 25)
		{
			cout << "You found 1 potion!" << endl;
			items.push_back(new Potion());
		}
		else
		{	
			cout << "You found a Fireball spell!" << endl;
			items.push_back(new CastFireball());
		}
		
		if(random >= 30 && random <= 45)
		{
			cout << "You feel experienced. All stats have increased slightly." << endl;
			health += 10;
			strength++;
			shield++;
		}
		else if(random >= 85 && random <= 95){
			cout << "A wise old man noticed your battle and blesses you." << endl;
			blessed = true;		
		}
		else
		{
			cout << "Nothing else interesting happened after the battle..." << endl;
		}

		checkStats();
		round++;
			
				
	}

	void takeDamage(int damage)
	{
		int tempHealth = health;
		int realDamage = damage - shield;

		int random = rand() % 100 + 1;

		if(random == 1)
		{
			cout << name << " dodged the attack!" << endl;
			return;
		}
				
		if(realDamage <= 0)
			cout << name << " did not take any damage." << endl;
		
		health -= realDamage;

		cout << name << " took " << realDamage << " damage!!" << endl;

		if(health <= 0 && blessed)
		{
			cout << "The blessing of the old man has protected you! Stats increased tremendously!!" << endl;
			strength += 2;
			shield += 2;
			health = 15 + tempHealth;
			blessed = false;
		}
	}

	void attack(Entity *player)
	{
		int size = 0;
		int selection = 0;
		vector<Entity*>::iterator h;

		cout << "Enemies in room: " << endl;

		for(h = area.begin(); h != area.end(); h++)
		{
			size++;
			cout << size << " - " << (*h)->name << endl;
		}

		while(!(selection > 0 && selection <= size))
		{
			cout << "Choose which enemy to attack:" << endl;
			cin >> selection;
		}
	
		cout << "#+-+-+-+#+-+-+-+#+-+-+-+#+-+-+-+" << endl;
		area.at(selection-1)->takeDamage(strength);
		cout << "#+-+-+-+#+-+-+-+#+-+-+-+#+-+-+-+" << endl;

		cout << "[Press Enter key to continue]" << endl;
		
		waitForAnyKey();
	}

	void useAnItem(Entity* player, SpecialItem* item, int item_pos)
	{
		int size = 0;
		int selection = -1;
		vector<Entity*>::iterator h;

		cout << "Choose which entity to use the " << item->getName() << " on:" << endl;

		cout << size << " - " << player->name << " [SELF]" << endl;
		
		for(h = area.begin(); h != area.end(); h++)
		{
			size++;
			cout << size << " - " << (*h)->name << endl;
		}

		while(!(selection >= 0 && selection <= size))
		{
			cin >> selection;
		}
	
		cout << "#+-+-+-+#+-+-+-+#+-+-+-+#+-+-+-+" << endl;
		if(selection != 0)
			item->onUse(this, area.at(selection-1));
		else //using it on self
			item->onUse(this, this);
		
		if(item->remove())	
			items.erase(items.begin()+item_pos);
		cout << "#+-+-+-+#+-+-+-+#+-+-+-+#+-+-+-+" << endl;
			
		cout << "[Press Enter key to continue]" << endl;
		
		waitForAnyKey();
	}
	
	void observe()
	{
		int size = 0;
		int selection = 0;
		vector<Entity*>::iterator h;

		cout << "Observe in room: " << endl;

		for(h = area.begin(); h != area.end(); h++)
		{
			size++;
			cout << size << " - " << (*h)->name << endl;
		}

		while(!(selection > 0 && selection <= size))
		{
			cout << "Choose which entity to observe:" << endl;
			cin >> selection;
		}
	
		cout << endl << "><><><><><><><><><><><" << endl;
		cout << area.at(selection-1)->name << endl << area.at(selection-1)->description;
		cout << endl << "[Value] := " << area.at(selection-1)->value;
		cout << endl << "[Health] := " <<  area.at(selection-1)->health;
		cout << endl << "[Sheild] := " << area.at(selection-1)->shield;
		cout << endl << "[Strength] := " << area.at(selection-1)->strength;
		cout << endl << "><><><><><><><><><><><" << endl;

		cout << "[Press Enter key to continue]" << endl;
		
		waitForAnyKey();
	}

	void checkStats()
	{
		cout << endl << "**********************" << endl;
		cout << name;
			
		if(blessed) 
			cout << " ~~BLESSED";

		cout << endl << description;
		cout << endl << "[Value] := " << value;
		cout << endl << "[Health] := " << health;
		cout << endl << "[Sheild] := " << shield;
		cout << endl << "[Strength] := " << strength;
		cout << endl << "**********************" << endl;


		cout << "[Press Enter key to continue]" << endl;
		
		waitForAnyKey();

		cout << endl;
	}
	
	void itemListUse()
	{
		int size = 0;
		int selection = 0;
		vector<SpecialItem*>::iterator h;

		cout << "Items in inventory: " << endl;

		for(h = items.begin(); h != items.end(); h++)
		{
			size++;
			cout << size << " - " << (*h)->getName() << endl;
		}

		while(!(selection > 0 && selection <= size))
		{
			cout << "Choose an item to use:" << endl;
			cin >> selection;
		}		
		
		useAnItem(this, items.at(selection-1), (selection-1));
	}
	
	void itemListCheck()
	{
		int size = 0;
		int selection = 0;
		vector<SpecialItem*>::iterator h;

		cout << "Items in inventory: " << endl;

		for(h = items.begin(); h != items.end(); h++)
		{
			size++;
			cout << size << " - " << (*h)->getName() << endl;
		}

		while(!(selection > 0 && selection <= size))
		{
			cout << "Choose an item to check:" << endl;
			cin >> selection;
		}	
		
		items.at(selection-1)->showStats();
	}
	
	int jumpToMenu()
	{
		int selection = 0;

		cout << "0. !Check Stats!" << endl;
		cout << "1. Attack with fists" << endl;
		cout << "2. Observe an enemy" << endl;
		cout << "3. ";
		
		if(items.size() > 0)
			cout << "Use an item" << endl;
		else
			cout << "//No items to use//" << endl;
			
		cout << "4. ";
		
		if(items.size() > 0)
			cout << "Check an item" << endl;
		else
			cout << "//No items to check//" << endl;
		
		cin >> selection;

		switch(selection)
		{
			case 0:
				checkStats();
				jumpToMenu();
				break;
			case 1:
				attack(this);
				break;
			case 2:
				observe();
				jumpToMenu();
				break;
			case 3:
				if(items.size() > 0)
					itemListUse();
				else
					jumpToMenu();
				break;
			case 4:
				if(items.size() > 0)
					itemListCheck();
				jumpToMenu();
				break;
			default:
				cout << "Unknown command." << endl;
				return -1;
				break;
		}

		return 0;
	}
			
	void update()
	{
		checkDead();

		if(this->isDead())
		{
			cout << "You fail as a hero. " << name << " has been slain..." << endl;
			flagDead = true;
			inBattle = false;
			gameOver = true;
			return;
		}

		for(vector<Entity*>::iterator i = area.begin(); i != area.end(); i++)
		{
			if((*i)->isDead())
			{
				if(area.size() > 0)
				{
					area.erase(i);
					i--;
					continue;
				}
				else
				{
					area.clear();
					break;	
				}
			}
		}

		if(area.size() == 0)
		{
			cout << name << " has slain all the enemies! What a hero!" << endl;	
			inBattle = false;
			atWin();

			return;
		}

		cout << name << "\'s turn. What will you do?" << endl;
		
		while(jumpToMenu() != 0);

		for(vector<Entity*>::iterator i = area.begin(); i != area.end(); i++)
		{
			cout << endl << "__--__--__--__--__--__--__--__" << endl;
			(*i)->update();

			if(!(*i)->isDead())
				(*i)->attack(this);


			cout << "[Press Enter key to continue]" << endl;
		
			waitForAnyKey();
		}
	
		cout << endl;
		cout << "//--//--//--//--//--//--//--//--//--//--//--//" << endl;
	}
};

bool loadNewWave(int match, vector<Entity*> surroundings, Hero* player)
{
	if(player->isDead() || player->isGameOver())
		return false;

	surroundings.clear();

	switch(match)
	{
		case 1:
			cout << "The first dungeon crawls with skeletons and those of the deceased..." << endl;
			surroundings.push_back( new Skeleton("Skeleton Menace", "Pain in the ass.", 5));
			surroundings.push_back( new Skeleton("Walking Corpse", "Ew.", 2, 5));
			break;
		case 2:
			cout << "Fighting off the hordes of the undead, it becomes darker and darker.\nThe next dungeon holds all sorts of disturbing creatures..." << endl;
			surroundings.push_back( new Goblins("Goblin demi", "Standard Goblin. Nothing special."));
			surroundings.push_back( new Goblins("Goblin Saji", "Goblin with jewel-studded armour."));
			surroundings.push_back( new Skeleton("Skeleton Guard", "Dude with an attitude. Oh and he's a skeleton.", 3, 6));
			surroundings.push_back( new Goblins("Goblin demi", "His name is Karl."));
			break;
		case 3:
			cout << "Goblins, critters, and the undead were no match for the final dungeon." << endl;
			cout << "The gigantic door blows open to reveal a massive winged-lizard" << endl;
			cout << "from hell: A Dragon. The ultimate challenge." << endl;
			surroundings.push_back( new Dragon("Faermoore the Dragon", "Golden dragon with sharp talons with which to rip its prey.", 35));
			break;

		case 4: 
			cout << "With Faeroomore slain, the walls of the dugeon collapsed.\nTwo large Trolls storm in and chase you down..." << endl;
			surroundings.push_back( new Troll("Dopus", "Brother of Doofus"));
			surroundings.push_back( new Troll("Doofus", "Brother of Dopus", 7, 4, 14));
			break;

		default:
			return false;
			break;
	}

	player->insertSurroundings(surroundings);

	return true;

}

void battle(Hero* player)
{
	while(player->isInBattle())
	{
		player->update();
	}
}

int main()
{
	srand(time(0));

	vector<Entity*> area;
	
	vector<SpecialItem*> items;
	items.push_back(new Potion());
	items.push_back(new Potion());
	items.push_back(new Medalion(0, 0, -2, "Medalion of Skrag", "A violet-colored gem sits in the crest of the medalion."));
	
	cout << "Hero-To-Be... What is thou name?" << endl << ":_ ";
	string name;

	getline(cin, name);
	Hero player(name);
	player.insertItems(items);

	cout << "WELCOME, " << player.name << "! You will have many battles to prove your worth." << endl;
	cout << "You must pursue all of the enemies you encounter and you must destroy them."<< endl;
	cout << "If you withstand the dragon Faermoore, you will be hailed as the world's true hero..." << endl;
	cout << "======================<><><><><><><><><><><><><><><>==========================" << endl;

	while(loadNewWave(player.getRound(), area, &player))
	{
		battle(&player);
	}

	if(!player.isDead())
	{
		cout << "Is is as it was prophecised! " << player.name << " is our true hero of the world!!" << endl;
		cout << "                                 THE END                 " << endl;
	}

	return 0;
}	
