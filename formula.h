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

#ifndef FORMULA_H
#define FORMULA_H

#include <QObject>
#include <QString>

class Formula : public QObject
{
    Q_OBJECT
    QString currentExpression;
public:

    Formula();
    void append(QString term);
    void clear();
    QString getCurrentExpression();

signals:
    void displayCurrentExpression();

public slots:
};

#endif // FORMULA_H
