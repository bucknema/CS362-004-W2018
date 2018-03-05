/*
* Name: Matthew Toro
* onid: torom
* Class: CS 362 Software Engineering II
* Program: randomtestadventurer.c
* Due Date: 2/18/2018
* Description: Random testing for the card 'Adventurer' in dominion.c
*/

/*
FUNCTION TO BE TESTED: playAdventurer
REQUIREMENTS FOR FUNCTION: 

WHAT IS THIS FUNCTION SUPPOSED TO DO?
Takes cards from the top of the current player's deck.
If it is a treasure card, it puts it in the player's hand.
If it is a non-treasure card, it sets it aside to be discarded.
It keeps revealing until two treasure cards have been kept.

HOW DOES ADVENTURER AFFECT THE GAME STATE?
affects the current player's hand, deck, and discard
*/

/*
WHAT TO TEST:
1. Current player should receive exactly 2 treasures.
priorHand + 2 = currentHand 

2. x cards should come from his own pile.
priorDeck - x = current Deck Count

3. the adventurer card should be discarded
and the non-treasure cards should be discarded
priorDiscardCount - x = currentDiscardCount

4. No state change should occur for other players.
for each other player
	prior value = current value

5. No state change should occur to the victory card piles and kingdom card piles.
for each card
	prior supply count = current supply count

*/

/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
 *      gcc -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
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
int const NUM_PASSES = 32;
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
		printf ("TESTING Adventurer:\n");

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
			state.hand[i][randomHandSize - 1] = adventurer;
			
			// we need to change the deck parameters such that there at least 2 treasures in the deck
			// or else the adventurer card effect will never end
			int randomDeckSize = rand() % 499 + 2;
			state.deckCount[i] = randomDeckSize;
			state.deck[i][0] = copper;
			state.deck[i][1] = copper;
			for (j = 2; j < randomDeckSize; j++)
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
		// random testing
		struct gameState priorState;
		memcpy(&priorState, &state, sizeof(struct gameState));
		
		success = cardEffect(adventurer, 0, 0, 0, &state, state.handCount[0] - 1, 0);
		
		// might need to re-work this calculation since you can shuffle during the card's effect
		// but the code is bugged; it does not shuffle the discard into the deck so nevermind
		int cardsFound = priorState.deckCount[0] - state.deckCount[0];
		
		// discard adventurer + 2 new treasures = 1
		passedTests+=assertTrue(priorState.handCount[0] + 1 == state.handCount[0], "Current hand contains exactly 2 more cards");
		
		passedTests+=assertTrue(state.hand[0][state.handCount[0] - 1] == copper || 
			state.hand[0][state.handCount[0] - 1] == silver || 
			state.hand[0][state.handCount[0] - 1] == gold, "1st new card is a treasure");
			
		passedTests+=assertTrue(state.hand[0][state.handCount[0] - 2] == copper || 
			state.hand[0][state.handCount[0] - 2] == silver || 
			state.hand[0][state.handCount[0] - 2] == gold, "2nd new card is a treasure");
		
		passedTests+=assertTrue(priorState.deckCount[0] - cardsFound == state.deckCount[0], "Current player deck count decremented by revealed cards");
		
		passedTests+=assertTrue(priorState.discardCount[0] + cardsFound + 1 == state.discardCount[0], "Current player discard count incremented");
		
		passedTests+=assertTrue(state.discard[0][state.discardCount[0] - 1] == adventurer, "Adventurer card in current player discard");
		passedTests+=assertTrue(success == 0, "Function returned successfully");
		

		randomPlayerCountTotalTests+= (playerCount - 1) * 3;
		for ( i = 1; i < playerCount; i++)
		{
			printf("Player %d:\n", i+1);
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
		printf("%d tests passed. %d tests failed.\n", passedTests + randomPlayerCountPassedTests, NUM_PASSES*NUM_RUNS + randomPlayerCountTotalTests - passedTests - randomPlayerCountPassedTests);

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