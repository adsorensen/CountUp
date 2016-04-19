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
