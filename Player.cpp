#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "Player.h"
#include "Arena.h"
#include "globals.h"
using namespace std;

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_age = 0;
	m_dead = false;
}

int Player::row() const
{
	return m_row;
}

int Player::col() const
{
	return m_col;
}

int Player::age() const
{
	return m_age;
}

string Player::takeComputerChosenTurn()
{
	// If there's a robot immediately around me, move. Else, shoot.
	if (m_arena->nRobotsAt(row() - 1, col()) ||
		m_arena->nRobotsAt(row() + 1, col()) ||
		m_arena->nRobotsAt(row(), col() - 1) ||
		m_arena->nRobotsAt(row(), col() + 1)) {

		const int DIRECTION = 4;
		// check how dangerous are the surroundings
		std::vector<int> danger;
		danger.push_back(nRobotsAround(row() - 1, col())); // UP
		danger.push_back(nRobotsAround(row() + 1, col())); // DOWN
		danger.push_back(nRobotsAround(row(), col() - 1)); // LEFT
		danger.push_back(nRobotsAround(row(), col() + 1)); // RIGHT
														   // Return the index of the min element in danger
		int direction = std::min_element(danger.begin(),
			danger.end()) - danger.begin();

		move(direction);
		return "Moved.";

	}
	else {
		int shot_dir = pickDir();
		if (shot_dir == -1) {
			stand();
			return "Stood.";
		}
		else {
			bool hit = shoot(shot_dir);
			if (hit)
				return "Shot and hit";
			else return "Shot and missed";
		}
	}
}

int Player::nRobotsAround(int r, int c) {
	// define as a bad move if out of bounds
	int out_of_bounds = (r<1 || c<1 || r>MAXROWS || c>MAXCOLS) ? 9999 : 0;
	// Define bad if a robot is there, but better than out of bounds
	int robot = (m_arena->nRobotsAt(r, c)) ? 999 : 0;
	return m_arena->nRobotsAt(r - 1, c) +
		m_arena->nRobotsAt(r + 1, c) +
		m_arena->nRobotsAt(r, c - 1) +
		m_arena->nRobotsAt(r, c + 1) + out_of_bounds + robot;
}

int Player::pickDir() { // returns the best direction to shoot
	for (int i = 1; i <= MAXSHOTLEN; i++) {
		if (m_arena->nRobotsAt(row() - i, col()))
			return UP;
		if (m_arena->nRobotsAt(row() + i, col()))
			return DOWN;
		if (m_arena->nRobotsAt(row(), col() - i))
			return LEFT;
		if (m_arena->nRobotsAt(row(), col() + i))
			return RIGHT;
	}
	return -1;
}
void Player::stand()
{
	m_age++;
}

void Player::move(int dir)
{
	m_age++;
	switch (dir)
	{
	case UP:
		if (m_row > 1)
			m_row--;
		break;
	case DOWN:
		if (m_row < m_arena->rows())
			m_row++;
		break;
	case LEFT:
		if (m_col > 1)
			m_col--;
		break;
	case RIGHT:
		if (m_col < m_arena->cols())
			m_col++;
		break;
	}
}

bool Player::shoot(int dir)
{
	m_age++;

	if (rand() % 3 == 0)  // miss with 1/3 probability
		return false;
	int distance = 0;
	switch (dir) {
	case UP:
		distance = std::min(MAXSHOTLEN, row() - 1);
		for (int i = 1; i <= distance; i++) {
			if (m_arena->nRobotsAt(row() - i, col())) {
				m_arena->damageRobotAt(row() - i, col());
				return true;
			}
		}
		return false;
		break;
	case DOWN:
		distance = std::min(MAXSHOTLEN, m_arena->rows() - row());
		for (int i = 1; i <= distance; i++) {
			if (m_arena->nRobotsAt(row() + i, col())) {
				m_arena->damageRobotAt(row() + i, col()); //I fixed the mistake I 
				//had row()-1(just for my own record)
				return true;
			}
		}
		return false;
		break;
	case LEFT:
		distance = std::min(MAXSHOTLEN, col() - 1);
		for (int i = 1; i <= distance; i++) {
			if (m_arena->nRobotsAt(row(), col() - i)) {
				m_arena->damageRobotAt(row(), col() - i);
				return true;
			}
		}
		return false;
		break;
	case RIGHT:
		distance = std::min(MAXSHOTLEN, m_arena->cols() - col());
		for (int i = 1; i <= distance; i++) {
			if (m_arena->nRobotsAt(row(), col() + i)) {
				m_arena->damageRobotAt(row(), col() + i);
				return true;
			}
		}
		return false;
	default: return false;
		break;
	}
}

bool Player::isDead() const
{
	if (m_dead)
		return true;
	return false;
}

void Player::setDead()
{
	m_dead = true;
}
