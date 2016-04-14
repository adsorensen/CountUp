#ifndef MATHNODE_H
#define MATHNODE_H

#include <QString>

class MathNode
{
public:
    MathNode();
    MathNode(const MathNode);
    bool isSelected;
    bool isOperator;
    QString value;
};

#endif // MATHNODE_H
