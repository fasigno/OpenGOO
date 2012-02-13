#ifndef LEVEL_H
#define LEVEL_H

//#include <QWidget> <--To use without opengl
#include <QGLWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <Box2D/Box2D.h>

#include <QPair>


#include "goo.h"
#include "ground.h"
#include "joint.h"
#include "target.h"
#include "menu.h"
#include "object.h"

#include "levelloader.h"

#include <stickylink.h>


//Flag of the game
enum RunFlag {STANDARD, DEBUG};

//This is the Scene Widget
//It initialize evrithing
//The world (b2World) object is here
class Level : public QGLWidget //QWidget <--To use without openGL
{
    Q_OBJECT
public:
    explicit Level(QRect geometry,QString level,RunFlag flag = STANDARD,QWidget *parent = 0); //Geometry is needed to have the display dimension information, level is the level to load
    ~Level();

private:
    //Run type flag
    RunFlag flag;
    //LOADER
    LevelLoader * loader;
    //PROPERTY
    //Graphic
    QPoint center;          //Center of the display, is used to have a more human coordinate system
    QPoint translation;     //For the level translation
    bool onMenu;
    bool mooving;
    Menu* menu;
    //Physic
    b2World *world;         //world object of the box2d lib
    float step;             //it say how much long is a step
    //Game
    Ground *ground;         //Ground object
    QList<Goo*> goos;       //All the goos!
    QList<Goo*> goosToDestroy;  //GOOs to be destroyed the next update!
    QList<Joint*> joints;   //All the joints!
    QList<Joint*> jointsToDestroy; //Joints to be destroyed the next update!

    //STICKY VARIABLE
    //LIST OF THE STICKY TO BE CREATED
    QList< QPair<Goo*,QPoint> > stickyToCreate;
    //DEBUG LIST
    QList<StickyLink*> stickys;

    QList<Object*> objects;
    Target* target;         //The target object
    bool drag;              //If the player is dragging some goo
    Goo* dragged;           //The goo that is under the mouse
    //Mouse
    b2Vec2 mousePos;        //Mouse current position (is needed to calculate the speed of the mouse
    b2Vec2 mouseSpeed;      //Mouse speed (so when release the dragged goo, he take the mouse speed)
    //Level
    QString name;           //Level name
    int goal;               //Number of goo to collect to win
    int points;             //Number of goo collected
    bool catched;           //If the tower of Goos is catched
    QRect limit;            //Translation limit
    QList<QPoint> possibility;  //To show the possible joints on the dragged goo
    //METHOD
    //Level

//    void readLevel(QString path);   //Function to load the level from file
//    bool parseString(QString string);   //Split a line of the file in the two component the tag and the real information
//    bool parseInfo(QString tag,QString info); //Recognize the tag and use the info

    void createThorns(); //To initialize thorns

    Goo* getGooAt(QPoint p);    //Funciton to get (if any) a goo in a point +/- the radius of the goo

    //Function to translate the gui
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveOf(QPoint dP);

    bool makeJoint(Goo*a,Goo*b);    //Function to create a single joint between two goos
    QList<QPoint> possibleJoints(QPoint p); //Function to show all the possible joint from a point
    bool createJoints(QPoint p);    //Function to create


    //Funcion to draw background, score and win
    void paintBg(QPainter &p);
    void paintScore(QPainter &p);
    void paintWin(QPainter &p);

    //MENU BUTTON FOR TOUCHSCREEN
    void paintButton(QPainter &p);
    void clickButton(QPoint p);

protected:
    //Timer event function here the world is updated and after the scene is redr
    void timerEvent(QTimerEvent *);
    //Function that draw evrithing in the scene
    void paintEvent(QPaintEvent *);
    //Key release function (esc: to exit, up/left/down/right to move the scene
    void keyReleaseEvent(QKeyEvent *);
    //Mouse moove event
    void mouseMoveEvent(QMouseEvent *);
    //Press mouse event
    void mousePressEvent(QMouseEvent *);
    //Release mouse event
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void closing(); //When level is to close for some error reason
public slots:
private slots:
    //LEVEL LOADER SLOTS:
    void setGoal(int goal);
    void setName(QString name);
    void setLimit(QRect limit);
    void setGround(QPoint gCenter,QList<QPoint> gList);
    void setTarget(QPoint target);
    void setStartArea(int n,QRect area);
    void setJoint(QPoint a, QPoint b);

    void createSticky(QPoint p);
    void destroySticky();

    //LEVEL SLOTS:
    void destroyJoint(Joint * joint);   //Destroy a joint
    void destroyGOO();                  //Destroy a GOO!
    void gooCatched(Goo * goo);         //Target catch a goo
    void giveTarget(Goo* previous);     //Function to give a jointed goo to follow to a free goo
    void destroyJoint(Goo*a,Goo*b);
    void towerCatched();                //Actions to do when the tower is near the target
    void towerLost();                   //Actions to do when the tower in no more near the target
    void resume();                      //Close the menu
    void restart();                     //Restart the level
    void closeAll();
};

#endif // LEVEL_H
