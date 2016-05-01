/**
* Representation of levels in the game
*
* CS3505, April 2016.
* Team: It's Lit (fire, fire, fire)
*
* @author Adam Sorensen
*         Connor Douglas
*         Johnny Le
*         Michelle Nguyen
*         Steven Sun
*         Trung Le
*         Yu Song
*         Zhi Wong
*/

#include "level.h"

/*
 * Default constructor
 */
Level::Level()
{
}

/*
 * Constructor
 */
Level::Level(int lvlNum, int trgtNum, int moves, int mBomb, int mul2Bomb, int div2Bomb, int negBomb, int mul4Bomb)
{
    levelNum = lvlNum;
    targetNum = trgtNum;
    movesNum = moves;
    moduloBomb = mBomb;
    divideTwoBomb = div2Bomb;
    multiplyTwoBomb = mul2Bomb;
    negativeBomb = negBomb;
    multiplyFourBomb = mul4Bomb;
}
