#include "level.h"

Level::Level()
{

}

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
