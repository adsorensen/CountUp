/**
* Logical backend behind the MathNodes in the game
* A MathNode is an object that holds a number or operator and can make up a formula
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

#include "mathnode.h"

MathNode::MathNode()
{
    value = "0";
    isOperator = false;
    isSelected = false;
}

MathNode::MathNode(const MathNode &obj)
{
    isSelected = obj.isSelected;
    isOperator = obj.isOperator;
    value = obj.value;
}
