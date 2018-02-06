/* Include the following lines in your makefile:
*
* unittest2: unittest2.c dominion.o rngs.o
*      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
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

	//good kingdom card sets

	printf("Testing kingdomCards() with adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall\n\n");
	int* k = kingdomCards(adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall);
	int a, b, c, d, e, f;
	for (a = 0; a < 10; a++) {
		if (k[a] > 0 || k[a] < 26) {
			continue;
		}
		else { printf(">>>>>ERROR: kingdomCards() failed this test!\n"); }
	}
	printf(">>>>If there is no ERROR printed directly above, the test was successful!\n\n");

	printf("Testing kingdomCards() with minion, steward, tribute, gardens, ambassador, cutpurse, smithy, village, baron, embargo\n\n");
	int* k2 = kingdomCards(minion, steward, tribute, gardens, ambassador, cutpurse, smithy, village, baron, embargo);
	for (b = 0; b < 10; b++) {
		if (k2[b] > 0 || k2[b] < 26) {
			continue;
		}
		else { printf(">>>>>ERROR: kingdomCards() failed this test!\n"); }
	}
	printf(">>>>If there is no ERROR printed directly above, the test was successful!\n\n");


	//junk card input (not actual dominion cards)
	printf("Testing kingdomCards() with junk input (all 10 cards set to smithy) \n\n");
	int* k4 = kingdomCards(smithy, smithy, smithy, smithy, smithy, smithy, smithy, smithy, smithy, smithy);
	for ( d = 0; d < 10; d++) {
		if (k4[d] > 0 || k4[d] < 26) {
			if (d > 0 && k4[d] == k4[d-1]){ printf(">>>>>ERROR: kingdomCards() failed this test!\n"); }
			continue;
		}
		else { printf(">>>>>ERROR: kingdomCards() failed this test!\n"); }
	}
	printf(">>>>If there is no failure printed directly above, the test was successful!\n\n");


	printf("\n\nEND OF UNIT TEST 2 on kingdomCards() \n\n\n\n");
	return 0;

}

