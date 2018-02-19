/*

MARK BUCKNER
CS362-400
randomtestadventurer.c

Description: This is a random tester for the adventurer card

To run the test: 
	make randomtestadventurer

Examine your results in file: 
	randomtestadventurer.out

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

// global fail counter variables
int EFFECT_FAILS = 0;
int SHUFF_FAILS = 0;
int DRAW_FAILS = 0;
int HANDCOUNT_FAILS = 0;
int TREASURECOUNT_FAILS = 0;

// this function takes a randomly generated gamestate, and then tests the adventurer card
void RANDOM_TEST_ADVENTURER(int p, struct gameState *post_state) {
	
	int post_treasure = 0;
	int pre_treasure = 0;
	int temphand[MAX_HAND];
	int drawn_treasures = 0;

	// gamestate struct, test vars 
	struct gameState pre_state;
	int cardDrawn, card;
	int bonus = 0;
	int carEff, shuf, drawCar;
	int i;
	int z = 0;

	// copy the passed in gamestate to prestate
	memcpy(&pre_state, post_state, sizeof(struct gameState));

	// call the card effect function for adventurer on post-gamestate
	carEff = cardEffect(adventurer, 0, 0, 0, post_state, 0, &bonus);

	// check for cardeffect failure
	if (carEff) {
		EFFECT_FAILS++;
	}

	// execute the adventurer card manually for pregamestat for comparison
	while (drawn_treasures < 2) {

		if (pre_state.deckCount[p] < 1) {//if the deck is empty we need to shuffle discard and add to deck
			shuf = shuffle(p, &pre_state);

			// check if shuffle failed
			if (shuf == -1 && pre_state.deckCount[p] >= 1) {
				SHUFF_FAILS++;
			}
		}

		drawCar = drawCard(p, &pre_state);

		// check if drawcard failed
		if (drawCar == -1 && pre_state.deckCount[p] != 0) {
			DRAW_FAILS++;
		}

		cardDrawn = pre_state.hand[p][pre_state.handCount[p] - 1];//top card of hand is most recently drawn card.
		if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
			drawn_treasures++;
		else {
			temphand[z] = cardDrawn;
			pre_state.handCount[p]--; //this should just remove the top card (the most recently drawn one).
			z++;
		}
	}

	while (z - 1 >= 0) {
		pre_state.discard[p][pre_state.discardCount[p]++] = temphand[z - 1]; // discard all cards in play that have been drawn
		z = z - 1;
	}

	// get the treasure count from post-gamestate
	for (i = 0; i < post_state->handCount[p]; i++) {
		card = post_state->hand[p][i];
		if (card == copper || card == silver || card == gold) {
			post_treasure++;
		}
	}

	// get the treasure count pre-gamestate
	for (i = 0; i < pre_state.handCount[p]; i++) {
		card = pre_state.hand[p][i];
		if (card == copper || card == silver || card == gold) {
			pre_treasure++;
		}
	}

	// check if treasure counts are correct
	if (post_treasure != pre_treasure) {
		TREASURECOUNT_FAILS++;
	}

	// store values of hand and discard counts, pre and post
	int post_handcount = post_state->handCount[p];
	int post_deckcount = post_state->deckCount[p];
	int post_discardcount = post_state->discardCount[p];

	int pre_handcount = pre_state.handCount[p];
	int pre_deckcount = pre_state.deckCount[p];
	int pre_discardcount = pre_state.discardCount[p];

	// check if the hand, discard and deck counts dont match up
	if (!(post_handcount == pre_handcount && post_deckcount == pre_deckcount && post_discardcount == pre_discardcount)) {
		HANDCOUNT_FAILS++;
	}
}


/*

==========================================================================
MAIN FUNCTION
==========================================================================

*/


int main() {
	printf(">>>>> RANDOM TEST adventurer <<<<<\n");
	printf("File: randomtestadventurer.c\n");
	printf("\n==========================================================\n");

	//iterations of random testing
	int iterations = 20000;

	int treasures[] = { copper,silver,gold };
	int num_treasure;
	int i, n;
	int player;

	// the gamestate struct holds all the important game setting variables
	struct gameState Gstate;

	// we need a minimum of 3 cards in the deck
	int min_cards = 3;

	// seed the random number generator
	srand(time(NULL));

	// randomly initialize the game state
	for (n = 0; n < iterations; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
			((char*)&Gstate)[i] = floor(Random() * 256);
		}

		// randomly select values, keeping within the sensible boundaries of Dominion game rules
		player = floor(Random() * MAX_PLAYERS);
		Gstate.deckCount[player] = floor(Random() * ((MAX_DECK - min_cards) + 1) + min_cards);
		num_treasure = floor(Random() * ((Gstate.deckCount[player] - min_cards) + 1) + min_cards);

		// set minimum of 3 treasure cards in deck to find via adventure
		for (i = 0; i < num_treasure; i++) {
			Gstate.deck[player][i] = treasures[rand() % 3];
		}

		Gstate.discardCount[player] = 0;
		Gstate.handCount[player] = floor(Random() * ((MAX_HAND - min_cards) + 1) + min_cards);
		Gstate.whoseTurn = player;

		// call the test for adventurer, passing in randomly generated gamestate
		RANDOM_TEST_ADVENTURER(player, &Gstate);
	}

	// add up failed tests
	int TOTAL_FAILS = EFFECT_FAILS + DRAW_FAILS + SHUFF_FAILS + HANDCOUNT_FAILS + TREASURECOUNT_FAILS;

	/*
	===============================================================================
	REPORT RANDOM TEST RESULTS
	===============================================================================
	*/
	printf("\n==========================================================\n");


	if (iterations - TOTAL_FAILS <= 0) {
		printf("\n>>>>> RANDOM TEST RESULTS <<<<<\n");

		// manually report 0, because we can fail more than 1 test per iteration
		printf("PASSED TESTS: %d\n", 0); 
		printf("FAILED TESTS: %d\n", iterations);
	}

	else {
		printf("\n>>>>> RANDOM TEST RESULTS <<<<<\n");
		printf("PASSED TESTS: %d\n", iterations - TOTAL_FAILS);
		printf("FAILED TESTS: %d\n", TOTAL_FAILS);
	}

	if (TOTAL_FAILS == 0) {
		printf(">>>>> PASSED RANDOM TESTING WITH 0 FAILS! <<<<<\n\n");
	}

	else {
		printf("\n>>>>> FAILURE RESULTS <<<<<\n");
		printf("Card effect fails: %d\n", EFFECT_FAILS);
		printf("Shuffle fails: %d\n", SHUFF_FAILS);
		printf("Draw card fails: %d\n", DRAW_FAILS);
		printf("Treasure count fails: %d\n", TREASURECOUNT_FAILS);
		printf("Primary player hand/deck count fails: %d\n", HANDCOUNT_FAILS);
		printf(">>>>> FAILED RANDOM TESTING!!! <<<<<\n\n");
	}

	printf("\n==========================================================\n");
	printf(">>>>> COVERAGE <<<<<\n");
	// gcov info will be concatenated after here
	return 0;
}

//EOF