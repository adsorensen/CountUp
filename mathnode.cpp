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
