/*
* cardtest1.c
*

*/

/*
* Include the following lines in your makefile:
*
* cardtest1: cardtest1.c dominion.o rngs.o
*      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
*/


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

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
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	printf("now running cardEffect(smithy...) \n Hand count should +2 (draw two cards)\n Deck count should set to 2. \nCoin count should remain same.\n\n");

	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);


	printf("new hand count = %d, previously = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer]);
	printf("new deck count = %d, previously = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
	printf("new coin count = %d, previously = %d\n", testG.coins, G.coins);

	
	printf("\n >>>>>: Testing complete %s Please examine output versus expected results.<<<<<\n\n", TESTCARD);


	return 0;
}


