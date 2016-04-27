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

#ifndef LEVEL_H
#define LEVEL_H

class Level
{
public:
    Level();
    Level(int, int ,int, int, int, int, int, int);
    int levelNum;
    int targetNum;
    int movesNum;
    int moduloBomb;
    int divideTwoBomb;
    int multiplyTwoBomb;
    int negativeBomb;
    int multiplyFourBomb;
};

#endif // LEVEL_H
