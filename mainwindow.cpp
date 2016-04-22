/**
* Main view controller for the CountUp game
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Box2D/Box2D.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QDebug>
#include <QRadialGradient>
#include <QMouseEvent>
#include <QtGui>
#include <QPixmap>
#include <QAbstractItemView>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>
#include <QtOpenGL>
#include <QTimerEvent>

// Setup
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Centers application on start up
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    ui->setupUi(this);
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->show();

    // Sets the background image
    this->setStyleSheet("MainWindow {border-image: url(:/background/Resources/bg.png); };");

    // Initializes the game "grid"
    ui->tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setMouseTracking(true);
    ui->tableWidget->viewport()->setMouseTracking(true);
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->viewport()->installEventFilter(this);


    // Connect slots and signals
    QObject::connect(ui->shuffleButton, SIGNAL(clicked(bool)), this, SLOT(on_entry()));

    //Create world
    b2Vec2 gravity(0.0f, 100.0f); //normal earth gravity, 9.8 m/s/s straight down!
    World = new b2World(gravity);

    createWalls();
    createBalls();

    begin = true;

}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete World;
}

void MainWindow::createBalls()
{
    int offsetX = 95.0f;
    int offsetY = 90.0f;
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            int dx = offsetX + j*70;
            int dy = offsetY + i*70;
            _objects.append(createBall(b2Vec2(dx, dy), radius));
        }
    }
}

void MainWindow::createWalls()
{
    //_objects.append(createWall(60.0f, 305.0f, 560.0f, 1.0f, 0.0)); //test wall

    //Create walls for grid
    walls.append(createWall(60.0f, 615.0f, 560.0f, 1.0f, 0.0)); //ground
    walls.append(createWall(60.0f, 55.0f, 1.0f, 560.0f, 0.0));  //left border
    walls.append(createWall(620.0f, 55.0f, 1.0f, 560.0f, 0.0));  //right border

    //balls do not populate grid, they fall outside and into the abyss of nothingness

    //funnel 1
    walls.append(createWall(-30.0f, 28.0f, 100.0f, 3.0f, -0.25f*b2_pi));
    walls.append(createWall(613.0f, 28.0f, 100.0f, 3.0f, 0.25f*b2_pi));

    //funnel2
//   _objects.append(createWall(-210.0f, -45.0f, 300.0f, 1.0f, -0.25f*b2_pi));
//   _objects.append(createWall(590.0f, -45.0f, 300.0f, 1.0f, 0.25f*b2_pi));

    //inner borders
    walls.append(createWall(130.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(200.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(270.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(340.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(410.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(480.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(550.0f, 55.0f, 0.5f, 560.0f, 0.0));
}


void MainWindow::paintEvent(QPaintEvent *)
 {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setTransform(_transform);
    foreach(const Object& o, _objects)
    {
        drawEllipse(&p, o);
    }
    foreach(const Object& o, walls)
    {

        drawWall(&p, o);

    }
 }

//Sets value of the cell according to model
void MainWindow::fillGrid(MathNode model[][10])
{

    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            MathNode current = model[y][x];

            ui->tableWidget->setItem(y, x, new QTableWidgetItem);
            ui->tableWidget->item(y, x)->setText(current.value);
            ui->tableWidget->item(y, x)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

//Add math node to current formula
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    //retrieve value from math node
    //append to formula string

    qDebug() << "Start expression" << column << row;
    //ui->tableWidget->setItem(row,column,item);

    //validate formula before removing math ball
    removeBallAt(column, row);
}

//Same thing here but for drag
//Do not register diagonal nodes?
void MainWindow::on_tableWidget_cellEntered(int row, int column)
{
    //validate formula before removing math ball
    //removeBallAt(column, row);

}

//End string on mouse release
//Send current formula to game model to evaluate
//If valid expression, replace math nodes with new nodes from game model
//else return nodes to grid
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableWidget->viewport())
    {
        //if (event->type() == QEvent::MouseButtonRelease)
           // qDebug() << "End expression";
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_entry()
{

}

//Draw walls for grid
void MainWindow::drawWall(QPainter *p, const Object& o){
    float32 x = o.body->GetPosition().x;
    float32 y = o.body->GetPosition().y;
    float32 angle = o.body->GetAngle();
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(o.fixture->GetShape());
    float32 hx = shape->GetVertex(1).x;
    float32 hy = shape->GetVertex(2).y;

    QRectF r(x-hx, y-hy, 2*hx, 2*hy);

    p->save();
    p->translate(r.center());
    p->rotate(angle*180/b2_pi);
    p->translate(-r.center());
    p->drawRect(r);
    p->fillRect(r, Qt::white);

    p->restore();
}

//Create walls for grid
Object MainWindow::createWall(float32 x, float32 y, float32 w, float32 h, float32 angle) {
    Object o;
    // body
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
    bd.angle = angle * b2_pi;
    o.body = World->CreateBody(&bd);
    // shape
    b2PolygonShape shape;
    shape.SetAsBox(w/2.0f, h/2.0f);
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    o.fixture = o.body->CreateFixture(&fd);
    o.type = WallObject;
    return o;
}

//Create ball model
Object MainWindow::createBall(const b2Vec2& pos, float32 radius) {
    Object o;    //_objects.append(createBall(b2Vec2(dx, dy), 34.4f));

    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);
    balls.append(o.body);

    // shape
    b2CircleShape shape;
    shape.m_radius = radius; //ADJUST BALL RADIUS HERE
    // fixture
    // add mass
    for(int i = 0; i < 10; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);

    }

    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;


    MathNode myNode;
    myNode.value = QString::number(qrand()%10);
    o.color = generateColor(myNode);

    return o;
}

Object MainWindow::createBall(const b2Vec2& pos, float32 radius, int index)
{
    Object o;    //_objects.append(createBall(b2Vec2(dx, dy), 34.4f));

    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);
    balls.insert(index, o.body);

    // shape
    b2CircleShape shape;
    shape.m_radius = radius; //ADJUST BALL RADIUS HERE
    // fixture
    // add mass
    for(int i = 0; i < 10; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);

    }

    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;


    MathNode myNode;
    myNode.value = QString::number(qrand()%10);
    o.color = generateColor(myNode);

    return o;
}


//Draw ball model
//Work on getting getting a nice circle bg drawn
void MainWindow::drawEllipse(QPainter *p, const Object& o)
{
    // This gets called too many times????

    // Fills nodes
    QPen pen(Qt::black, 2);
    p->setPen(pen);
    //QBrush brush(Qt::green);
    //p->setBrush(brush);

    float32 x = o.body->GetPosition().x;
    float32 y = o.body->GetPosition().y;
    float32 r = o.fixture->GetShape()->m_radius;

    QLinearGradient gradient(0, 0, x, y);
    gradient.setColorAt(0.0, Qt::blue);
    gradient.setColorAt(1.0, o.color);
    p->setBrush(gradient);

    p->drawEllipse(QPointF(x, y), r, r);
}

//Start simulator
void MainWindow::start() {
    //if(!_timerId) {
        _timerId = startTimer(1000/60); // 60fps
    //}
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if(event->timerId() == _timerId) {
        World->Step(1.0f/60.0f, 8, 3);
        update();
    }
}

// Returns a QColor based on the input MathNode's ending value
QColor MainWindow::generateColor(MathNode currentNode)
{
    if (currentNode.value.endsWith("1"))
    {
        return Qt::green;
    }
    if (currentNode.value.endsWith("2"))
    {
        return Qt::blue;
    }
    if (currentNode.value.endsWith("3"))
    {
        return Qt::yellow;
    }
    if (currentNode.value.endsWith("4"))
    {
        return Qt::black;
    }
    if (currentNode.value.endsWith("5"))
    {
        return Qt::cyan;
    }
    if (currentNode.value.endsWith("6"))
    {
        return Qt::magenta;
    }
    if (currentNode.value.endsWith("7"))
    {
        return QColor(237, 145, 33);
    }
    if (currentNode.value.endsWith("8"))
    {
        return QColor(159, 0, 197);
    }
    if (currentNode.value.endsWith("9"))
    {
        return QColor(222, 82, 133);
    }
    else
    {
        return Qt::red;
    }
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    //qDebug() << "add expression" << currentColumn << currentRow;
    if (begin == false)
    {
        QPair<int, int> rowAndCol;
        rowAndCol.first = currentRow;
        rowAndCol.second = currentColumn;
        coordinates.append(rowAndCol);
        emit current_positions(coordinates);
    }
    begin = false;
}

void MainWindow::on_bombButton_pressed()
{


}

void MainWindow::removeBallAt(float32 column, float32 row)
{
    qDebug() << "removing ball at column: " <<column << " row: " << row;
    int index = getIndex((int)column, (int)row);
    qDebug() << "index: " << index ;

    b2Body *body = balls.at(index);

    World->DestroyBody(body);

    spawnBallAt(column, index);

    updateIndex(index);
}

void MainWindow::spawnBallAt(float32 column, int index)
{
    //incorrect index
    _objects.insert(index, createBall(b2Vec2((column * 70) + 95, 10.0f ), radius, index));

}

int MainWindow::getIndex(int column, int row)
{
    int index = row * 8 + column;
    return index;
}

//for trickling down
void MainWindow::updateIndex(int index)
{
    //current ball
    int current = index;
    //ball right above it
    int previous = current - 8;


    while(current > 7)
    {
        Object temp = _objects.at(previous);
        b2Body *body = balls.at(previous);


        _objects.removeAt(current);
        balls.removeAt(current);

        _objects.insert(current, temp);
        balls.insert(current, body);

        current = current - 8;
        previous = previous - 8;
    }

}

Object MainWindow::createBallTrickle(const b2Vec2& pos, float32 radius)
{
    Object o;    //_objects.append(createBall(b2Vec2(dx, dy), 34.4f));

    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);
    balls.insert(0, o.body);

    // shape
    b2CircleShape shape;
    shape.m_radius = radius; //ADJUST BALL RADIUS HERE
    // fixture
    // add mass
    for(int i = 0; i < 10; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);

    }

    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;


    MathNode myNode;
    myNode.value = QString::number(qrand()%10);
    o.color = generateColor(myNode);

    return o;
}



void MainWindow::on_shuffleButton_pressed()
{
    while(!_objects.isEmpty())
    {
        _objects.removeFirst();
    }

    foreach(b2Body* b, balls)
    {
        balls.removeOne(b);
        World->DestroyBody(b);

    }

    int offsetX = 95.0f;
    int offsetY = 90.0f;
    int dropheight = 10.0f;
    for (int i=7; i>-1; i--)
    {
        for (int j=7; j>-1; j--)
        {
            int dx = offsetX + j*70;
            int dy = offsetY + i*70;
            _objects.prepend(createBallTrickle(b2Vec2(dx, dropheight), radius));


            delay(50);
        }
        dropheight = dropheight - 70.0f;
    }

}

void MainWindow::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
