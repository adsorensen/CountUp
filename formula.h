/**
* Handles mathematical expressions within the CountUp game
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

#ifndef FORMULA_H
#define FORMULA_H

#include <QString>
//#include <Box2D-master/Box2D/Box2D/Box2D.h>
//#include <Box2D-master/Box2D/Box2D/Dynamics/b2World.h>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>

#include <Box2D/Common/b2Draw.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

class formula
{
    QString currentExpression;

public:
    formula();
    void append(QString);

};

#endif // FORMULA_H
