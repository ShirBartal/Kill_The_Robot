#include <iostream>
#include "Arena.h"
#include "Robot.h"
#include "Player.h"

using namespace std;

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nRobots = 0;
	// m_history = new History(nRows, nCols);
}

Arena::~Arena()
{
	delete m_player;
	for (int i = 0; i < m_nRobots; i++)
	{
		delete m_robots[i];
	}


}

int Arena::rows() const
{
	return m_rows;
}

int Arena::cols() const
{
	return m_cols;
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::robotCount() const
{
	return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const
{
	int count = 0;
	for (int i = 0; i < m_nRobots; i++) {
		if (m_robots[i] != nullptr) {
			if (m_robots[i]->row() == r && m_robots[i]->col() == c)
				count++;
		}
	}
	return count;
}

void Arena::display(string msg) const
{
	// Position (row,col) in the arena coordinate system is represented in
	// the array element grid[row-1][col-1]
	char grid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill the grid with dots
	for (r = 0; r < rows(); r++)
		for (c = 0; c < cols(); c++)
			grid[r][c] = '.';

	for (int i = 0; i < m_nRobots; i++) {
		char ch;
		switch (grid[m_robots[i]->row() - 1][m_robots[i]->col() - 1]) {
		case '.':
			ch = 'R';
			break;
		case 'R':
			ch = '2';
			break;
		case '2':
			ch = '3';
			break;
		case '3':
			ch = '4';
			break;
		case '4':
			ch = '5';
			break;
		case '5':
			ch = '6';
			break;
		case '6':
			ch = '7';
			break;
		case '7':
			ch = '8';
			break;
		default:
			ch = '9';
			break;
		}
		grid[m_robots[i]->row() - 1][m_robots[i]->col() - 1] = ch;
	}
	// Indicate player's position
	if (m_player != nullptr)
	{
		// Set the char to '@', unless there's also a robot there,
		// in which case set it to '*'.
		char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
		if (gridChar == '.')
			gridChar = '@';
		else
			gridChar = '*';
	}

	// Draw the grid
	clearScreen();
	for (r = 0; r < rows(); r++)
	{
		for (c = 0; c < cols(); c++)
			cout << grid[r][c];
		cout << endl;
	}
	cout << endl;

	// Write message, robot, and player info
	cout << endl;
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << robotCount() << " robots remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player." << endl;
	else
	{
		if (m_player->age() > 0)
			cout << "The player has lasted " << m_player->age() << " steps." << endl;
		if (m_player->isDead())
			cout << "The player is dead." << endl;
	}
}

bool Arena::addRobot(int r, int c) //did by myself, check.
{
	if (m_nRobots == MAXROBOTS)
		return false;
	else {
		Robot* a = new Robot(this, r, c);
		m_robots[m_nRobots] = a;
		m_nRobots++;
		return true;
	}
}

bool Arena::addPlayer(int r, int c)
{
	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Dynamically allocate a new Player and add it to the arena
	m_player = new Player(this, r, c);
	return true;
}

void Arena::damageRobotAt(int r, int c)
{
	for (int i = 0; i < m_nRobots; i++) {
		Robot* Rob = m_robots[i];
		if (Rob != nullptr) {
			if (Rob->row() == r && Rob->col() == c) {
				if (Rob->takeDamageAndLive()) { // If robot supposed to live after hit
					Rob->hitRobot();
				}
				else { // If robot does not survive the hit
					delete Rob;
					// Switch the deceased robot with the last on the array to prevent 
					// discrepancies between m_nRobots and the actual array.
					m_robots[i] = m_robots[m_nRobots - 1];
					m_robots[m_nRobots - 1] = nullptr;
					m_nRobots--;
				}
				return; // Return when found a robot.
			}
		}
	}
}

bool Arena::moveRobots()
{
	for (int k = 0; k < m_nRobots; k++)
	{
		Robot* Rob = m_robots[k];
		if (Rob != nullptr) {
			Rob->move();
			if (Rob->row() == m_player->row() && Rob->col() == m_player->col())
				m_player->setDead();
		}
	}
	// return true if the player is still alive, false otherwise
	return !m_player->isDead();
}

// History& Arena::history() {
// 	return *m_history;
// }
