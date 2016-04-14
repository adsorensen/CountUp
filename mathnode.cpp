#include "mathnode.h"

MathNode::MathNode()
{

}

MathNode::MathNode(const MathNode &obj)
{
    isSelected = obj.isSelected;
    isOperator = obj.isOperator;
    value = obj.value;
}
