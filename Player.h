#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;
class Arena;
class Player
{
public:
	// Constructor
	Player(Arena *ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	int  age() const;
	bool isDead() const;

	// Mutators
	string takeComputerChosenTurn();
	void   stand();
	void   move(int dir);
	bool   shoot(int dir);
	void   setDead();

private:
	Arena * m_arena;
	int    m_row;
	int    m_col;
	int    m_age;
	bool   m_dead;
	int nRobotsAround(int r, int c);
	int pickDir();
};
#endif
