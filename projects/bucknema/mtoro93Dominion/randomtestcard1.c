/*
* Name: Matthew Toro
* onid: torom
* Class: CS 362 Software Engineering II
* Program: testcard1.c
* Due Date: 2/18/2018
* Description: Random testing for the card 'Village' in dominion.c
*/

/*
FUNCTION TO BE TESTED: playGreatHall
REQUIREMENTS FOR FUNCTION: 

WHAT IS THIS FUNCTION SUPPOSED TO DO?
draw 1 card. add 2 actions

HOW DOES VILLAGE AFFECT THE GAME STATE?
affects current player's hand, deck, and discard
affects state.numActions

*/

/*
WHAT TO TEST:
1. Current player should receive exactly 1 card.
priorHand = currentHand after village is discarded

2. 1 card should come from his own pile.
priorDeck - 1 = current Deck Count

3. the village card should be discarded
state.discardCount = prior count + 1
state.discard[0][discardCount - 1] = village card number

4. the current player should receive exactly 2 actions

5. No state change should occur for other players.
for each other player
	prior value = current value

6. No state change should occur to the victory card piles and kingdom card piles.
for each card
	prior supply count = current supply count

*/

/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * randomtestcard1: randomtestcard1.c dominion.o rngs.o
 *      gcc -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0
int const NUM_PASSES = 31;
int const NUM_RUNS = 200;
int assertTrue(int booleanExpression, char* testCase);
int checkForRepeat(int* indices, int length, int target);

int main() {
    int i, j, k;
    int randomSeed = 1000;
	int success;
	int passedTests = 0;
	int randomPlayerCountPassedTests = 0;
	int randomPlayerCountTotalTests = 0;
	for (k = 0; k < NUM_RUNS; k++)
	{
		// generate a random set of non-repeat kingdomCards 
		// integers between 7-26 because 0 - 6 are required
		int kingdomCards[10];
		int priorIndices[10];
		int randomIndex;
		for (i = 0; i < 10; i++)
		{
			do
			{
				randomIndex = rand() % 20 + 7;
			}while(checkForRepeat(priorIndices, 10, randomIndex));
			
			priorIndices[i] = randomIndex;
			kingdomCards[i] = randomIndex;
		}
		
		// generate a random playerCount of 2 - 4
		int playerCount = rand() % 3 + 2;
		struct gameState state;
		printf ("TESTING Village:\n");

		// initialize a new game using random player count and kingdom cards
		success = initializeGame(playerCount, kingdomCards, randomSeed, &state);
		
		// now we will randomize the game state
		for (i = 0; i < playerCount; i++)
		{
			// maximum hand_size is 500
			// maximum deck_size is 500
			int randomHandSize = rand() % 500 + 1;
			state.handCount[i] = randomHandSize;
			for (j = 0; j < randomHandSize - 2; j++)
			{
				int randChoice = rand() % 2;
				if (randChoice == 0)
				{
					int randomIndex = rand() % 10;
					state.hand[i][j] = kingdomCards[randomIndex];
				}
				else
				{
					int randomIndex = rand() % 7;
					state.hand[i][j] = randomIndex;
				}
				
			}
			state.hand[i][randomHandSize - 1] = village;
			
			int randomDeckSize = rand() % 500 + 1;
			state.deckCount[i] = randomDeckSize;
			for (j = 0; j < randomDeckSize; j++)
			{
				int randChoice = rand() % 2;
				if (randChoice == 0)
				{
					int randomIndex = rand() % 10;
					state.hand[i][j] = kingdomCards[randomIndex];
				}
				else
				{
					int randomIndex = rand() % 7;
					state.hand[i][j] = randomIndex;
				}
			}
			
			// i have to keep discard less than 500
			// however, since the hand is not discarded in this test case,
			// I can bound it only be the deck size
			int randomDiscardSize = 500 - randomDeckSize - 1;
			state.discardCount[i] = randomDiscardSize;
			
			for (j = 0; j < randomDiscardSize; j++)
			{
				int randChoice = rand() % 2;
				if (randChoice == 0)
				{
					int randomIndex = rand() % 10;
					state.hand[i][j] = kingdomCards[randomIndex];
				}
				else
				{
					int randomIndex = rand() % 7;
					state.hand[i][j] = randomIndex;
				}
			}
		}
		
		printf ("TESTING Village:\n");
		// initialize a new game using starting decks for these tests
		// since initialize also draws their starting hands then the starting deck begins with 5 cards
		success = initializeGame(playerCount, kingdomCards, randomSeed, &state);
		struct gameState priorState;
		memcpy(&priorState, &state, sizeof(struct gameState));
		
		success = cardEffect(village, 0, 0, 0, &state, state.handCount[0] - 1, 0);
		
		// these two tests should fail because of the bug introduced in assignment-2
		passedTests+=assertTrue(priorState.handCount[0] == state.handCount[0], "Current hand count remains the same");
		passedTests+=assertTrue(priorState.deckCount[0] - 1 == state.deckCount[0], "Current player deck count decremented by 1");
		
		passedTests+=assertTrue(priorState.discardCount[0] + 1 == state.discardCount[0], "Current player discard count incremented");
		
		passedTests+=assertTrue(priorState.numActions + 2 == state.numActions, "number of actions incremented by 2");
		
		// these tests actually discover a bug either in Smithy or discardCard
		passedTests+=assertTrue(state.discard[0][state.discardCount[0] - 1] == village, "Village card in current player discard");
		passedTests+=assertTrue(success == 0, "Function returned successfully");
		
		randomPlayerCountTotalTests+= (playerCount - 1) * 3;
		for ( i = 1; i < playerCount; i++)
		{
			printf("Player %d:\n", i);
			randomPlayerCountPassedTests+=assertTrue(priorState.handCount[i] == state.handCount[i], "Hand remains the same");
			randomPlayerCountPassedTests+=assertTrue(priorState.deckCount[i] == state.deckCount[i], "Deck remains the same");
			randomPlayerCountPassedTests+=assertTrue(priorState.discardCount[i] == state.discardCount[i], "Discard remains the same");	
		}

		for (i = 0; i < 25; i++)
		{
			printf("Kingdom Card: %d\n", i);
			passedTests+=assertTrue(priorState.supplyCount[i] == state.supplyCount[i], "Kingdom card supply remains the same");
		}
	}
	if (passedTests + randomPlayerCountPassedTests == NUM_PASSES*NUM_RUNS + randomPlayerCountTotalTests)
		printf("All tests passed!\n");
	else
		printf("%d tests passed. %d tests failed.\n", passedTests + randomPlayerCountPassedTests, NUM_PASSES*NUM_RUNS + randomPlayerCountTotalTests - randomPlayerCountPassedTests - passedTests);

    return 0;
}


int assertTrue(int booleanExpression, char* testCase)
{
	if (booleanExpression == 0)
	{
		printf("Test failed: ");
		printf(testCase);
		printf("\n");
		return 0;
	}
	else
	{
		printf("Test passed: ");
		printf(testCase);
		printf("\n");
		return 1;
	}	
}
int checkForRepeat(int* indices, int length, int target)
{
	int i;
	for (i = 0; i < length; i++)
	{
		if (target == indices[i])
			return 1;
	}
	return 0;
}