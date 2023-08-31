#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include <vector>
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <vector>
#include <queue>

class Actor;
class Earth;
class TunnelMan;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    int dig(int x, int y);
    double distance(int objectLocationX, int PersonLocationX, int objectLocationY, int PersonLocationY);
    double nearby(Actor* actor);
    bool canGoTo(int x, int y);
    void barrelFound();
    void goldFound();
    void goldPickedUp();
    void waterPickedUp();
    void sonarPickedUp();
    void activateSonar();
    int getSpawnTime();
    void setDisplayText();
    void decrementProtester();
    void addActor(Actor* actor);
    bool canActorGoHerePlacement(int x, int y);
    bool canActorMoveHere(Actor* actor, int x, int y);
    void randomLocation(int &x, int &y, int xBottom, int xTop, int yBottom, int yTop);
    std::string formatDisplayText(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);
    bool beatLevel;
    void placingEarths();
    std::vector<std::vector<int>> bfsMaze(std::vector<std::vector<int>>& maze, Actor* actor);
    GraphObject::Direction directionToTarget(Actor* actor);
    std::vector<std::vector<int>> arrayToVector(int arr[61][61]);
    bool facingPlayer(Actor* actor);
    bool annoyTunnelman(Actor* actor, int amount, double radius);
    bool annoyProtester(Actor* actor, int amount, double radius);
    TunnelMan* getTunnelman();
    bool legalDirectionMove(Actor* actor, GraphObject::Direction dir);
    bool addItem();
    int whichItem();
    bool poolCheck(int x, int y);
    bool bribe(Actor* actor, int radius);
    void dropGold();
private:
    TunnelMan* m_tunnelman;
    Earth* m_arena[VIEW_WIDTH][VIEW_HEIGHT];
    std::vector <Actor*> actor_vector;
    int m_numBarrels;
    int m_numGold;
    int spawnTime;
    int m_numOfProtesters;
    int m_mazeSteps[61][61];
    int m_numTicks;
    int m_numTicksBeforeProtester;
    int m_numProtesters;
    int m_numTargetProtesters;
    int itemSpawnTime;
    int probabilityOfItem;
    int probabilityHardCore;
    
};

#endif // STUDENTWORLD_H_
