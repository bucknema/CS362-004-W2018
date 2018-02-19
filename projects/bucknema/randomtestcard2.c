/*

MARK BUCKNER
CS362-400
randomtestcard2.c

Description: This is a random tester for the council room card

To run the test:
	make randomtestcard2

Examine results in file:
	randomtestcard2.out

*/
/*=====================================================================================================*/

// standard header files for strings, input/output, etc.
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// dominion game header files
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

// global variables to keep track of test failures
int EFFECT_FAILS = 0;
int DISCARD_FAILS = 0;
int DRAW_FAILS = 0;
int HANDCOUNT_FAILS = 0;
int BUYCOUNT_FAILS = 0;
int OPPONENT_HANDCOUNT_FAILS = 0;

// this function takes a randomly generated gamestate, and then tests the council room card
void RANDOM_TEST_COUNCIL_ROOM(int p, struct gameState *post_state) {
	int carEff, draw1, draw2, draw3, draw4, w, disCar, i;
	int bonus = 0;

	// game state variable to manually act on the functions actions
	struct gameState pre_state;

	// copy the passed in game state to pre_state
	memcpy(&pre_state, post_state, sizeof(struct gameState));

	// call the card effect function for council room
	carEff = cardEffect(council_room, 0, 0, 0, post_state, 0, &bonus);

	// draw card 4x times
	draw1 = drawCard(p, &pre_state);
	draw2 = drawCard(p, &pre_state);
	draw3 = drawCard(p, &pre_state);
	draw4 = drawCard(p, &pre_state);

	pre_state.numBuys++;

	// each player draws a card
	for (i = 0; i < pre_state.numPlayers; i++) {
		if (i != p) {
			w = drawCard(i, &pre_state);
			
			// check for drawCard fails
			if (w == -1 && pre_state.deckCount[i] != 0) {
				DRAW_FAILS++;
			}
		}
	}

	// check discardCard manually
	disCar = discardCard(0, p, &pre_state, 0);

	// store values of hand and deck counts, pre and post
	int post_handcount = post_state->handCount[p];
	int post_deckcount = post_state->deckCount[p];

	int pre_handcount = pre_state.handCount[p];
	int pre_deckcount = pre_state.deckCount[p];

	// check if buy counts dont match up
	if (pre_state.numBuys != post_state->numBuys) {
		BUYCOUNT_FAILS++;
	}

	// check for drawcard fails
	if (draw1 == -1 && pre_state.deckCount[p] != 0) {
		DRAW_FAILS++;
	}
	if (draw2 == -1 && pre_state.deckCount[p] != 0) {
		DRAW_FAILS++;
	}
	if (draw3 == -1 && pre_state.deckCount[p] != 0) {
		DRAW_FAILS++;
	}
	if (draw4 == -1 && pre_state.deckCount[p] != 0) {
		DRAW_FAILS++;
	}

	// check cardeffect or discardCard fails
	if (!(carEff == 0 && disCar == 0)) {
		if (carEff) {
			EFFECT_FAILS++;
		}
		if (disCar) {
			DISCARD_FAILS++;
		}
	}

	// check if the primary player hand and deck counts are incorrect
	if (!(post_handcount == pre_handcount && post_deckcount == pre_deckcount)) {
		HANDCOUNT_FAILS++;
	}

	// check if the other players hand and deck counts are incorrect (council room affects others too!)
	for (i = 0; i < pre_state.numPlayers; i++) {
		if (i != p) {
			if (!(post_state->handCount[i] == pre_state.handCount[i] &&
				post_state->deckCount[i] == pre_state.deckCount[i])) {
				OPPONENT_HANDCOUNT_FAILS++;
			}
		}
	}
}


/*

==========================================================================
MAIN FUNCTION
==========================================================================

*/

int main() {
	printf(">>>>> RANDOM TEST council room <<<<<\n");
	printf("File: randomcardtest2.c\n");
	printf("\n==========================================================\n");

	//iterations of random testing
	int iterations = 20000; 

	int i, n;
	int player;
	int deckCount, handCount, discardCount;
	int numberOfPlayers[] = { 2,3,4 };

	// the gamestate struct holds all the important game setting variables
	struct gameState Gstate;

	// seed the random number generator
	srand(time(NULL));

	// randomly select values, keeping within the sensible boundaries of Dominion game rules
	for (n = 0; n < iterations; n++) {

		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&Gstate)[i] = floor(Random() * 256);
		}

		// randomly set values, keeping within Dominion boundaries
		Gstate.numPlayers = numberOfPlayers[rand() % 3];
		Gstate.numBuys = 1;
		Gstate.playedCardCount = floor(Random() * (MAX_DECK - 1));
		player = Gstate.numPlayers - 2;
		deckCount = floor(Random() * MAX_DECK);
		handCount = floor(Random() * MAX_HAND);
		discardCount = floor(Random() * MAX_DECK);
		Gstate.whoseTurn = player;

		// set hand and deck values of other players
		for (i = 0; i < Gstate.numPlayers; i++) {
			Gstate.deckCount[i] = deckCount;
			Gstate.handCount[i] = handCount;
			Gstate.discardCount[i] = discardCount;
		}

		// call the test for adventurer, passing in randomly generated gamestate
		RANDOM_TEST_COUNCIL_ROOM(player, &Gstate);
	}


	/*
	===============================================================================
	REPORT RANDOM TEST RESULTS
	===============================================================================
	*/
	printf("\n==========================================================\n");

	// add up failed tests
	int TOTAL_FAILS = EFFECT_FAILS + DISCARD_FAILS + DRAW_FAILS + HANDCOUNT_FAILS + BUYCOUNT_FAILS;
	
	printf("\n>>>>> RANDOM TEST RESULTS <<<<<<\n");

	if (iterations - TOTAL_FAILS <= 0) { // need this check because we can fail more than 1 test per iteration
		printf("PASSED TESTS: 0\n\n");
	}
	else {
		printf("PASSED TESTS: %d\n", iterations - TOTAL_FAILS);
	}
	printf("FAILED TESTS: %d\n", TOTAL_FAILS);

	if (TOTAL_FAILS == 0) {
		printf(">>>>> PASSED RANDOM TESTING WITH 0 FAILS!! <<<<<\n\n");
	}

	else {
		printf("\n>>>>> FAILURE RESULTS <<<<<\n");

		printf("Card effect fails: %d\n", EFFECT_FAILS);
		printf("Draw fails: %d\n", DRAW_FAILS);
		printf("Discard fails: %d\n", DISCARD_FAILS);
		printf("Buy count fails: %d\n", BUYCOUNT_FAILS);
		printf("Other players hand/deck count fails: %d\n", OPPONENT_HANDCOUNT_FAILS);
		printf("Primary player hand/deck count fails: %d\n", HANDCOUNT_FAILS);
		printf(">>>>> FAILED RANDOM TESTING!!! <<<<<\n\n");
	}

	printf("\n==========================================================\n");
	printf(">>>>> COVERAGE <<<<<\n");
	// gcov info will be concatenated after here


	return 0;
}

//EOF