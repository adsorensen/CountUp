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
