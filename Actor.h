#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <iostream>

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject{
public:
    Actor(StudentWorld* world, int imageID, int initialX, int initialY, Direction d, double size, int depth);
    bool isAlive();
    virtual void dies();
    StudentWorld* getWorld();
    virtual ~Actor();
    virtual void doSomething();
    int getImageID();
    virtual void addGold();
    virtual void getAnnoyed(int amount);
private:
    bool m_alive;
    int m_imageID;
    StudentWorld* m_world;
};

class Earth: public Actor{
public:
     Earth(int startX, int startY, StudentWorld* world);
    virtual ~Earth();
};

class TunnelMan: public Actor{
public:
    TunnelMan(StudentWorld* world);
    virtual void doSomething();
    virtual ~TunnelMan();
    int getGold();
    int getWater();
    int getSonar();
    int getHealth();
    void addGold();
    void addWater();
    void addSonar();
    void loseGold();
    void loseWater();
    void loseSonar();
    virtual void getAnnoyed(int amount);
    void waterSquirt();
    
private:
    int lives;
    int health;
    int numberOfWater;
    int numberOfSonar;
    int numberOfGold;
};

class Protester: public Actor{
public:
    Protester(StudentWorld* world, int imageId, int health, int score);
    virtual ~Protester();
    int numSquaresToMoveInCurrentDirection();
    virtual void doSomething();
    Direction canSeePlayer();
    GraphObject::Direction randomDirection();
    GraphObject::Direction randomPerpendicular();
    virtual void getAnnoyed(int amount);
    int getHealth();
    virtual void addGold();
    virtual void dies();
    void restTicksSetter(int time);
private:
    int m_health;
    int ticksToWaitBetweenMoves;
    int m_numRestTicks;
    int m_numStepsinCurrDir;
    int m_ticksSinceShout;
    int m_numOfStunTicks;
    int m_numPerpendicularTicks;
    bool m_lookingForWayOut = false;
    
};

class HardCoreProtester: public Protester{
public:
    HardCoreProtester(StudentWorld* world, int imageId, int health, int score);
    ~HardCoreProtester();
    virtual void addGold();
private:
    int stareTime;
};

class Squirt: public Actor{
public:
    Squirt(StudentWorld* world, int initialX, int initialY, Direction d);
    virtual ~Squirt();
    virtual void doSomething();
private:
    int distanceTravelled;
    Direction directionShot;
};

class Boulder: public Actor{
public:
    Boulder(StudentWorld* world, int initialX, int initialY);
    virtual ~Boulder();
    virtual void doSomething();
    bool isStable(int x, int y);
private:
    int m_ticksWaiting;
};

class Items: public Actor{
public:
    Items(StudentWorld* world, int imageID, int initialX, int initialY);
    virtual ~Items();
    virtual void doSomething();
    virtual void found() = 0;
private:
    bool m_discovered;
};




class Gold: public Items{
public:
    Gold(int startX, int startY, StudentWorld* world, bool dropped);
    virtual ~Gold();
    virtual void doSomething();
    virtual void found();
private:
    bool wasDropped;
    int ticksGold;
};

class WaterPool: public Items{
public:
    WaterPool(StudentWorld* world, int initialX, int initialY);
    virtual ~WaterPool();
    virtual void doSomething();
    virtual void found();
private:
    int m_ticksWaiting;
};



class Sonar: public Items{
public:
    Sonar(StudentWorld* world);
    virtual ~Sonar();
    virtual void doSomething();
    virtual void found();
private:
    int numberOfSonarTicks;
};


class BarrelsOfOil: public Items{
public:
    BarrelsOfOil(int startX, int startY, StudentWorld* world);
    virtual ~BarrelsOfOil();
    virtual void found();
};

//• Gold Nuggets
//• Sonar kits
//• Water refills (that can be picked up to refill the TunnelMan’s squirt gun)
//• Earth
#endif // ACTOR_H_
