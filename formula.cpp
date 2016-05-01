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

/*
 * Constructor
 */
Formula::Formula()
{
    currentExpression = "";
}

/*
 * Adds to the current formula
 */
void Formula::append(QString term)
{
    currentExpression += term;
}

/*
 * Clears the formula
 */
void Formula::clear()
{
    currentExpression = "";
}

/*
 * Returns the Formula
 */
QString Formula::getCurrentExpression()
{
    return currentExpression;
}
