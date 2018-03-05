/*

MARK BUCKNER
CS362-400
randomtestcard1.c

Description: This is a random tester for the smithy card

To run the test: 
	make randomtestcard1

Examine results in file: 
	randomtestcard1.out

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

// this function takes a randomly generated gamestate, and then checks the smithy card
void RANDOM_TEST_SMITHY(int p, struct gameState *post_state) {
	
	// vars to hold return results of various function tests
	int carEff, draw1, draw2, draw3, disCar;

	// pre-gamestate variable (for comparison with postgamestate)
	struct gameState pre_state;

	// copy the passed in randomly generated gamestate to pre-state
	memcpy(&pre_state, post_state, sizeof(struct gameState));
	int bonus = 0;

	// call the card effect function with the smithy card
	carEff = cardEffect(smithy, 0, 0, 0, post_state, 0, &bonus);

	// call draw card 3 times
	draw1 = drawCard(p, &pre_state);
	draw2 = drawCard(p, &pre_state);
	draw3 = drawCard(p, &pre_state);

	// call discardCard
	disCar = discardCard(0, p, &pre_state, 0);

	// store values of hand and deck counts, pre and post
	int post_handcount = post_state->handCount[p];
	int post_deckcount = post_state->deckCount[p];

	int pre_handcount = pre_state.handCount[p];
	int pre_deckcount = pre_state.deckCount[p];

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

	// check if cardeffect or discardCard failed
	if (!(carEff == 0 && disCar == 0)) {
		if (carEff) {
			EFFECT_FAILS++;
		}
		if (disCar) {
			DISCARD_FAILS++;
		}
	}

	// check if the hand and deck counts dont match up
	if (!(post_handcount == pre_handcount && post_deckcount == pre_deckcount)) {
		HANDCOUNT_FAILS++;
	}
}


/*

==========================================================================
MAIN FUNCTION
==========================================================================

*/


int main() {
	printf(">>>>> RANDOM TEST smithy <<<<<\n");
	printf("File: randomtestcard1.c\n");
	printf("\n==========================================================\n");

	//iterations of random testing
	int iterations = 20000;

	int i, n;
	int player;

	// the gamestate struct holds all the important game setting variables
	struct gameState Gstate;

	// seed random number generator
	srand(time(NULL));

	// randomly initialize the gamestate
	for (n = 0; n < iterations; n++) {

		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&Gstate)[i] = floor(Random() * 256);
		}

		// randomly select values, keeping within the sensible boundaries of Dominion game rules
		player = floor(Random() * MAX_PLAYERS);
		Gstate.deckCount[player] = floor(Random() * MAX_DECK);
		Gstate.discardCount[player] = floor(Random() * MAX_DECK);
		Gstate.handCount[player] = floor(Random() * MAX_HAND);
		Gstate.playedCardCount = floor(Random() * (MAX_DECK - 1));
		Gstate.whoseTurn = player;

		// call the test for adventurer, passing in randomly generated gamestate 
		RANDOM_TEST_SMITHY(player, &Gstate);
	}

	// add up failed tests
	int TOTAL_FAILS = EFFECT_FAILS + DISCARD_FAILS + DRAW_FAILS + HANDCOUNT_FAILS;

	/*
	===============================================================================
	REPORT RANDOM TEST RESULTS
	===============================================================================
	*/

	printf("\n==========================================================\n");

	printf("\n>>>>> RANDOM TEST RESULTS <<<<<\n");

	if (iterations - TOTAL_FAILS < 0){
		printf("PASSED TESTS: 0");
	}
	else {
		printf("PASSED TESTS: %d\n", iterations - TOTAL_FAILS);
	}
	printf("FAILED TESTS: %d\n", TOTAL_FAILS);

	if (TOTAL_FAILS == 0) {
		printf(">>>>> PASSED RANDOM TESTING WITH 0 FAILS!!! <<<<<\n\n");
	}

	else {
		printf("\n>>>>> FAILURE RESULTS <<<<<\n");

		printf("Card effect fails: %d\n", EFFECT_FAILS);
		printf("Draw card fails: %d\n", DRAW_FAILS);
		printf("Discard fails: %d\n", DISCARD_FAILS);
		printf("Primary player hand/deck count fails: %d\n", HANDCOUNT_FAILS);
		printf(">>>>> FAILED RANDOM TEST!!! <<<<<\n\n");
	}

	printf("\n==========================================================\n");
	printf(">>>>> COVERAGE <<<<<\n");
	// gcov info will be concatenated after here

	return 0;
}

//EOF