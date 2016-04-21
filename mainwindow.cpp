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
    ui->tableWidget->hide();

    // Connect slots and signals
    QObject::connect(ui->shuffleButton, SIGNAL(clicked(bool)), this, SLOT(on_entry()));

    //Create world
    b2Vec2 gravity(0.0f, 12.0f); //normal earth gravity, 9.8 m/s/s straight down!
    World = new b2World(gravity);



    //release the mock balls
    for(int i=0; i<5; i++) {
        int dx = 620 - qrand() % 560;
        int dy = qrand() % 2;
        _objects.append(createBall(b2Vec2(dx, 25.0f), 1.0f));
    }


    //Create walls for grid
    _objects.append(createWall(60.0f, 615.0f, 560.0f, 1.0f, 0.0)); //ground
    _objects.append(createWall(60.0f, 55.0f, 1.0f, 560.0f, 0.0));  //left border
    _objects.append(createWall(620.0f, 55.0f, 1.0f, 560.0f, 0.0));  //right border

    //inner borders
//    _objects.append(createWall(130.0f, 55.0f, 1.0f, 560.0f, 0.0));
//    _objects.append(createWall(200.0f, 55.0f, 1.0f, 560.0f, 0.0));
//    _objects.append(createWall(270.0f, 55.0f, 1.0f, 560.0f, 0.0));
//    _objects.append(createWall(340.0f, 55.0f, 1.0f, 560.0f, 0.0));
//    _objects.append(createWall(410.0f, 55.0f, 1.0f, 560.0f, 0.0));
//    _objects.append(createWall(480.0f, 55.0f, 1.0f, 560.0f, 0.0));
//    _objects.append(createWall(550.0f, 55.0f, 1.0f, 560.0f, 0.0));






}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
 {
    //what does this do?
//     QStyleOption opt;
//     opt.init(this);
//     QPainter p(this);
//     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setTransform(_transform);
    foreach(const Object& o, _objects) {
        switch(o.type) {
        case BallObject:
            drawEllipse(&p, o);


            break;
        case WallObject:
            drawWall(&p, o);

        }

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
    _objects.append(createWall(60.0f, 55.0f, 1.0f, 560.0f, 0.0));  //left border
    _objects.append(createWall(620.0f, 55.0f, 1.0f, 560.0f, 0.0));  //right border

}

//Same thing here but for drag
//Do not register diagonal nodes?
void MainWindow::on_tableWidget_cellEntered(int row, int column)
{

}

//End string on mouse release
//Send current formula to game model to evaluate
//If valid expression, replace math nodes with new nodes from game model
//else return nodes to grid
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableWidget->viewport())
    {
        if (event->type() == QEvent::MouseButtonRelease)
            qDebug() << "End expression";
    }
    return QMainWindow::eventFilter(obj, event);
}

//
void MainWindow::on_entry()
{

}

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

Object MainWindow::createBall(const b2Vec2& pos, float32 radius) {
    Object o;
    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);
    // shape
    b2CircleShape shape;
    shape.m_radius = radius;
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.friction = 1.0f;
    fd.restitution = 0.6f;
    o.fixture = o.body->CreateFixture(&fd);
    o.type = BallObject;
    return o;
}

void MainWindow::drawEllipse(QPainter *p, const Object& o) {
    float32 x = o.body->GetPosition().x;
    float32 y = o.body->GetPosition().y;
    float32 r = o.fixture->GetShape()->m_radius + 20;
    p->drawEllipse(QPointF(x, y), r, r);
}

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




