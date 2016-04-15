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
