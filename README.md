# Kill_The_Robot

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Game.h"
using namespace std;

int main()
{
	// Initialize the random number generator. 
	srand(static_cast<unsigned int>(time(0)));

	// Create a game
	Game g(15, 18, 80);

	// Play the game
	g.play();
}

