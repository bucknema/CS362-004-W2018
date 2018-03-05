/* Include the following lines in your makefile:
*
* unittest1: unittest1.c dominion.o rngs.o
*      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
* ---------------------------------------------------------------------- -
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>


int main() {

	int newCards = 0;
	int discarded = 1;
	int xtraCoins = 0;
	int shuffledCards = 0;

	int i, j, m;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int remove1, remove2;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G1, testG1, G2, testG2, G3, testG3, G4, testG4, G5, testG5;

	//kingdom card set
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };

	//test result variable
	int RESULT;


	printf(">>>>Testing initialize game with 2 players and seed 1000 \n Expecting: 0\n");
	// initialize a game state and player cards
	RESULT = initializeGame(2, k, 1000, &G1);
	printf("Result: %d\n", RESULT);
	if (RESULT == 0) { printf("SUCCESS: %d\n", RESULT); }
	else { printf("FAILURE: %d player count error \n", RESULT); }
	

	printf(">>>>Testing initialize game with 4 players and seed 5 \n Expecting: 0\n");
	// initialize a game state and player cards
	RESULT = initializeGame(4, k, 5, &G2);
	printf("Result: %d\n", RESULT);
	if (RESULT == 0) { printf("SUCCESS: %d\n", RESULT); }
	else { printf("FAILURE: %d player count error \n", RESULT); }
	
	printf(">>>>Testing initialize game with 0 players and seed 1000 \n Expecting: -1\n");
	// initialize a game state and player cards
	RESULT = initializeGame(0, k, 1000, &G3);
	printf("Result: %d\n", RESULT);
	if (RESULT == 0) { printf("SUCCESS: %d\n", RESULT); }
	else { printf("FAILURE: %d player count error \n", RESULT); }
	
	printf(">>>>Testing initialize game with -1 players and seed 2 \n Expecting: -1\n");
	// initialize a game state and player cards
	RESULT = initializeGame(0, k, 1000, &G4);
	printf("Result: %d\n", RESULT);
	if (RESULT == 0) { printf("SUCCESS: %d\n", RESULT); }
	else { printf("FAILURE: %d player count error \n", RESULT); }

	printf(">>>>Testing initialize game with 5 players and seed 1000 \n Expecting: -1\n");
	// initialize a game state and player cards
	RESULT = initializeGame(0, k, 1000, &G5);
	if (RESULT == 0){ printf("SUCCESS: %d\n", RESULT); }
	else{ printf("FAILURE: %d player count error \n\n\n", RESULT); }


	printf("\n\nEND OF UNIT TEST 1 on initializeGamer() \n\n\n\n");
	return 0;
	
}

