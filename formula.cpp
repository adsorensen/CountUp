/**
* Represents a math expression
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

#include "formula.h"

Formula::Formula()
{
    currentExpression = "";
}

void Formula::append(QString term)
{
    currentExpression += term;
}

void Formula::clear()
{
    currentExpression = "";
}

QString Formula::getCurrentExpression()
{
    return currentExpression;
}
