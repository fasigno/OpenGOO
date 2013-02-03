#include "og_world.h"
#include "flags.h"

#include <QPainter>
#include <QTime>

extern bool _E404;
extern QTime _time;
extern OGButton _buttonMenu;
extern QList<OGBall*> _balls;
extern QList<OGStrand*> _strands;
extern QList<OGStrand> _tmpStrands;

namespace visual_debug
{
    const qreal K = 10.0;
}

using namespace visual_debug;

void calculateFPS(QPainter* painter, qreal zoom)
{
    int x, y;

    static int fps = 0;
    static int cur_fps = 0;

    cur_fps++;

    if (_time.elapsed() >= 1000)
    {
        fps = cur_fps;
        cur_fps = 0;
        _time.restart();
    }

    x = painter->window().x() + 20.0/zoom;
    y = painter->window().y() + 20.0/zoom;
    painter->setPen(Qt::white);
    painter->setFont(QFont("Verdana", qRound(12.0/zoom), QFont::Bold));
    painter->drawText(x, y, QString::number(fps));
}

void drawStrands(QPainter* painter)
{    
    int x1, y1, x2, y2, b1, b2;

    for (int i=0; i < _strands.size(); i++)
    {
        if (_strands.at(i)->gb1 != -1 && _strands.at(i)->gb2 != -1)
        {
            b1 = _strands.at(i)->gb1;
            b2 = _strands.at(i)->gb2;
            x1 = _balls.at(b1)->ball->body->GetPosition().x*K;
            y1 = _balls.at(b1)->ball->body->GetPosition().y*K*(-1.0);
            x2 = _balls.at(b2)->ball->body->GetPosition().x*K;
            y2 = _balls.at(b2)->ball->body->GetPosition().y*K*(-1.0);

            painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
        }
    }

    for (int i=0; i < _tmpStrands.size(); i++)
    {
        painter->drawLine(_tmpStrands.at(i).line);
    }
}

void drawBalls(QPainter* painter)
{
    qreal x, y, radius;

    for (int i=0; i < _balls.size(); i++)
    {
        x = _balls.at(i)->ball->body->GetPosition().x*K;
        y = _balls.at(i)->ball->body->GetPosition().y*K*(-1.0);
        radius = _balls.at(i)->ball->shape->GetRadius()*K;

        painter->drawEllipse(QPointF(x, y), radius, radius);
    }
}

void visualDebug(QPainter* painter, OGWorld* world, qreal zoom)
{
    qreal winX = painter->window().x();
    qreal winY = painter->window().y();

    QPen pen(Qt::yellow,  2.0/zoom);

    painter->setOpacity(1);
    painter->setPen(pen);

    // menu
    qreal btnW = _buttonMenu.size().width()/zoom;
    qreal btnH = _buttonMenu.size().height()/zoom;
    qreal btnX = winX + _buttonMenu.position().x()/zoom;
    qreal btnY = winY + _buttonMenu.position().y()/zoom;
    QRectF rect(btnX, btnY, btnW, btnH);

    painter->drawRect(rect);
    painter->drawEllipse(QPointF(0, 0), 10.0/zoom, 10.0/zoom); // center of word

    // level exit
    if (world->leveldata()->levelexit)
    {
        qreal x, y;

        x = world->leveldata()->levelexit->pos.x();
        y = world->leveldata()->levelexit->pos.y()*(-1.0);
        QRectF rect(x, y, 10.0 , 10.0);

        rect.moveCenter(rect.topLeft());
        painter->fillRect(rect, Qt::yellow);
    }

    // pipe
    for (int i=0; i < world->leveldata()->pipe.vertex.size()-1; i++)
    {
        qreal x1, x2, y1, y2;

        x1 = world->leveldata()->pipe.vertex.at(i).x();
        y1 = world->leveldata()->pipe.vertex.at(i).y()*(-1.0);
        x2 = world->leveldata()->pipe.vertex.at(i+1).x();
        y2 = world->leveldata()->pipe.vertex.at(i+1).y()*(-1.0);

        painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    for (int i=0; i < world->scenedata()->circle.size(); i++)
    {
        qreal x, y, radius;

        x = world->scenedata()->circle.at(i)->position.x();
        y = world->scenedata()->circle.at(i)->position.y()*(-1.0);
        radius = world->scenedata()->circle.at(i)->radius;

        painter->drawEllipse(QPointF(x, y), radius, radius);
    }

    drawBalls(painter);
    drawStrands(painter);

    if (_E404)
    {
        painter->setOpacity(0.25);
        painter->fillRect(painter->window(), Qt::black);
        painter->setOpacity(1);
        painter->setPen(Qt::white);
        painter->setFont(QFont("Times", 36, QFont::Bold));
        painter->drawText(painter->window()
                          , Qt::AlignCenter
                          , "UNIMPLEMENTED!!!\nEsc to return"
                          );
    }

    if (flag == FPS)
    {
        calculateFPS(painter, zoom);
    }
}