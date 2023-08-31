#include "Actor.h"
#include "StudentWorld.h"


using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//ACTOR CODE
Actor::Actor(StudentWorld* world, int imageID, int initialX, int initialY, Direction d, double size, int depth): GraphObject(imageID, initialX, initialY, d, size, depth), m_world(world), m_alive(true), m_imageID(imageID){
    setVisible(true);
};
void Actor::getAnnoyed(int amount){};
void Actor::doSomething(){};
bool Actor::isAlive(){
    return m_alive;
}
int Actor::getImageID(){
    return m_imageID;
}
void Actor::dies(){
    m_alive = false;
};
Actor::~Actor(){
}
StudentWorld* Actor::getWorld(){
    return m_world;
}

void Actor::addGold(){};

//EARTH STUFF
Earth::Earth(int startX, int startY, StudentWorld* world): Actor(world, TID_EARTH, startX, startY, right, 0.25, 3){
        setVisible(true);
    }
Earth::~Earth(){
}

//TUNNELMAN CODE
TunnelMan::TunnelMan(StudentWorld* world): Actor(world, TID_PLAYER, 30, 60, right, 1.0, 0), health(10), numberOfWater(5), numberOfSonar(1), numberOfGold(0){
    setVisible(true);
}
void TunnelMan::doSomething(){
//    if (isAlive() == false){
//        return;
//    }
    //DIGGING EARTH HELP
  //  cout << getWorld() << endl;
  //cout << "I'm here" << endl;
  //cout << getWorld() << endl;
    int key;
    bool success = getWorld()->getKey(key);
    //std::cout << key;
        if (key == KEY_PRESS_UP){
            if (getDirection() != up){
                setDirection(up);
            }
            else{
                if (getY() == 60){
                    return;
                }
                if(getWorld()->canActorMoveHere(this, getX(), getY()+1)){
                    moveTo(getX(), getY()+1);
                }
            }
        }
        if (key == KEY_PRESS_DOWN){
            if (getDirection() != down){
                setDirection(down);
            }
            else{
                if (getY() == 0){
                    return;
                }
                if(getWorld()->canActorMoveHere(this, getX(), getY()-1)){
                    moveTo(getX(), getY()-1);
                }
            }
        }
        if (key == KEY_PRESS_RIGHT){
            if (getDirection() != right){
                setDirection(right);
            }
            else{
                if (getX() == 60){
                    return;
                }
                if(getWorld()->canActorMoveHere(this, getX()+1, getY())){
                    moveTo(getX()+1, getY());
                }
            }
        }
        if (key == KEY_PRESS_LEFT){
            if (getDirection() != left){
                setDirection(left);
            }
            else{
                if (getX() == 0){
                    return;
                }
                if(getWorld()->canActorMoveHere(this, getX()-1, getY())){
                    moveTo(getX()-1, getY());
                }
            }
        }
    if (key == KEY_PRESS_ESCAPE){
        dies();
    }
    
    if(key == 'Z' || key == 'z'){
        getWorld()->activateSonar();
    }
    
    if(key == KEY_PRESS_SPACE){
        waterSquirt();
    }

    if(key == KEY_PRESS_TAB){
        getWorld()->dropGold();
    }
    
    
    
    if(getWorld()->dig(getX(), getY()) == 1)
        getWorld()->playSound(SOUND_DIG);
}

TunnelMan::~TunnelMan(){
}

//FUNCTIONS FOR FINDING DIFFERENT ITEMS AND UPDATIGN MEMBER VARIABLE COUNT
void TunnelMan::addGold(){
    numberOfGold++;
};
void TunnelMan::addWater(){
    numberOfWater += 5;
};
void TunnelMan::addSonar(){
    numberOfSonar++;
};

void TunnelMan::loseGold(){
    numberOfGold--;
}
void TunnelMan::loseWater(){
    numberOfWater--;
}
void TunnelMan::loseSonar(){
    numberOfSonar--;
}

void TunnelMan::getAnnoyed(int amount){
    health = health - amount;
    if(health <= 0){
        dies();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    }
}

//GETTERS
int TunnelMan::getGold(){
    return(numberOfGold);
};
int TunnelMan::getWater(){
    return(numberOfWater);
};
int TunnelMan::getSonar(){
    return(numberOfSonar);
};

int TunnelMan::getHealth(){
    return health;
}

void TunnelMan::waterSquirt(){
    if(getWater() > 0){             //if no more water is left
        Direction dir = getDirection();     //get the player's direction
        if(dir == up){
            getWorld()->addActor(new Squirt(getWorld(), getX(), getY()+4, up));
            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
            numberOfWater--;
        }
        else if(dir == down){
            getWorld()->addActor(new Squirt(getWorld(), getX(), getY()-4, down));
            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
            numberOfWater--;
        }
        else if(dir == left){
            getWorld()->addActor(new Squirt(getWorld(), getX()-4, getY(), left));
            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
            numberOfWater--;
        }
        else if(dir == right){
            getWorld()->addActor(new Squirt(getWorld(), getX()+4, getY(), right));
            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
            numberOfWater--;
        }
    }
    return;
}


//ITEMS CODE
Items::Items(StudentWorld* world, int imageID, int initialX, int initialY):Actor(world, imageID, initialX, initialY, right, 1.0, 2), m_discovered(false){
    //initially 1.0 and 2 for size and depth, respectively; most objects have these specs but have to change depth for boulder and squirt which have depth = 1
};
       
Items::~Items(){
};

void Items::doSomething(){
    if(getWorld()->nearby(this) <= 4.0){
        setVisible(true);
    }
    if(getWorld()->nearby(this) <= 3.0){
        found();
        dies();
    }
}



//BARRELSOFOIL CODE
BarrelsOfOil::BarrelsOfOil(int initialX, int initialY, StudentWorld* world):Items(world, TID_BARREL, initialX, initialY){
    setVisible(false);
}

BarrelsOfOil::~BarrelsOfOil(){}

void BarrelsOfOil::found(){
    getWorld()->playSound(SOUND_FOUND_OIL);
    getWorld()->increaseScore(1000);
    getWorld()->barrelFound();
    dies();
}

//GOLD CODE
Gold::Gold(int initialX, int initialY, StudentWorld* world, bool dropped):Items(world, TID_GOLD, initialX, initialY), ticksGold(0), wasDropped(dropped){
    if (wasDropped == true){
        setVisible(true);
    }
    else{
        setVisible(false);
    }
}
Gold::~Gold(){}
void Gold::doSomething(){
    if(!isAlive())
        return;
    if(wasDropped == false){
        if(getWorld()->nearby(this) < 4.0)
            setVisible(true);
        if(getWorld()->nearby(this) < 3.0){
            found();
            dies();
        }
    }
    if(wasDropped == true){
        if(getWorld()->bribe(this, 3.0))
            dies();
        if(ticksGold == 100)
            dies();
        else
            ticksGold++;
    }
}


void Gold::found(){
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(10);
    getWorld()->goldFound();
    getWorld()->goldPickedUp();
};




//SONAR CODE
Sonar::Sonar(StudentWorld* world):Items(world, TID_SONAR, 0, 60){
    setVisible(true);
    numberOfSonarTicks = 0;
}
Sonar::~Sonar(){};


void Sonar::doSomething(){
    if (!isAlive()){
        return;
    }
    if(numberOfSonarTicks == getWorld()->getSpawnTime()){
        dies();
        return;
    }
    //IF NEAR PLAYER, PICK UP(CALL FOUND), SET TO DEAD AND RETURN
    if(getWorld()->nearby(this) < 3.0){
        found();
        dies();
    }
    else{
        numberOfSonarTicks++;
    }
};

void Sonar::found(){
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->sonarPickedUp();
    getWorld()->increaseScore(75);
};


//BOULDER CODE
Boulder::Boulder(StudentWorld* world, int initialX, int initialY):Actor(world, TID_BOULDER, initialX, initialY, down, 1.0, 1),
m_ticksWaiting(0){
    getWorld()->dig(initialX, initialY);
    setVisible(true);
}

Boulder::~Boulder(){};

void Boulder::doSomething(){
    if (!isAlive())
        return;
    if (isStable(getX(), getY()) == true){
        return;
    }
        if (isStable(getX(), getY()) == false){
            m_ticksWaiting++;
            if(m_ticksWaiting >= 30){
                if(isStable(getX(), getY()) == false){
                    moveTo(getX(), getY()-1);
                    getWorld()->playSound(SOUND_FALLING_ROCK);
                    getWorld()->annoyTunnelman(this, 100, 3.0);
                    getWorld()->annoyProtester(this, 100, 3.0);
                }
                if(isStable(getX(), getY()) == true){
                    dies();
                }
            }
        }
    }

bool Boulder::isStable(int x, int y){
    if (getWorld()->canGoTo(getX(), getY()-1) == true){
        return false;
    }
    return true;
}

WaterPool::WaterPool(StudentWorld* world, int initialX, int initialY):Items(world, TID_WATER_POOL, initialX, initialY), m_ticksWaiting(0){
    setVisible(true);
}

WaterPool::~WaterPool(){}

void WaterPool::doSomething(){
    if(!isAlive()){
        return;
    }
    if (m_ticksWaiting == getWorld()->getSpawnTime()){
        dies();
        return;
    }
    if(getWorld()->nearby(this) < 3.0){
        found();
        dies();
    }
    else{
        m_ticksWaiting++;
    }
}

void WaterPool::found(){
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->waterPickedUp();
    getWorld()->increaseScore(100);
}

//SQUIRT CODE
Squirt::Squirt(StudentWorld* world, int initialX, int initialY, Direction d):Actor(world, TID_WATER_SPURT, initialX, initialY, d, 1.0, 1),distanceTravelled(0){
    setVisible(true);
}

Squirt::~Squirt(){};

void Squirt::doSomething(){
    if(distanceTravelled == 4)
        dies();
    if(getDirection() == up){
        if(getWorld()->canActorMoveHere(this, getX(), getY()+1) != true || getWorld()->annoyProtester(this, 2, 3.0)){
            dies();
            return;
        }
        else
           moveTo(getX(), getY()+1);
    }
    else if(getDirection() == down){
        if(getWorld()->canActorMoveHere(this, getX(), getY()-1) != true || getWorld()->annoyProtester(this, 2, 3.0)){
            dies();
            return;
        }
        else
            moveTo(getX(), getY()-1);
    }
    else if(getDirection() == left){
        if(getWorld()->canActorMoveHere(this, getX()-1, getY()) != true || getWorld()->annoyProtester(this, 2, 3.0)){
            dies();
            return;
        }
        else
            moveTo(getX()-1, getY());
    }
    else if(getDirection() == right){
        
        if(getWorld()->canActorMoveHere(this, getX()+1, getY()) != true || getWorld()->annoyProtester(this, 2, 3.0)){
            dies();
            return;
        }
        else
            moveTo(getX()+1, getY());
    }
    distanceTravelled++;
}
    //GET HELP CHECKING IF THERE IS EARTH OR BOULDER THERE

Protester::Protester(StudentWorld* world, int imageId, int health, int score):Actor(world, imageId, 60, 60, left, 1.0, 1), m_health(health),m_lookingForWayOut(false), m_numRestTicks(0), m_numPerpendicularTicks(0), m_ticksSinceShout(0){
    setVisible(true);
    ticksToWaitBetweenMoves = max(0, 3 - (int)getWorld()->getLevel()/4);
    m_numStepsinCurrDir = numSquaresToMoveInCurrentDirection();
    m_numOfStunTicks = max(50, 100 - (int)getWorld()->getLevel() * 10);
}

Protester::~Protester(){};

int Protester::numSquaresToMoveInCurrentDirection(){
    int a = rand() % (53) + 8;
    return a;
}

void Protester::addGold(){
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    m_lookingForWayOut = true;
}

void Protester::doSomething(){
    if(!isAlive()){
        return;
    }
    if(m_numRestTicks >= 0){
        m_numRestTicks--;
        return;
    }
    m_numRestTicks = ticksToWaitBetweenMoves;
    if (m_lookingForWayOut == true){
        if(getX() == 60 && getY() == 60){
            getWorld()->decrementProtester();
            Actor::dies();
        }
        else{
            Direction d = getWorld()->directionToTarget(this);
            setDirection(d);
            if (d == up){
                moveTo(getX(), getY()+1);
            }
            if (d == down){
                moveTo(getX(), getY()-1);
            }
            if (d == right){
                moveTo(getX()+1, getY());
            }
            if (d == left){
                moveTo(getX()-1, getY());
            }
        }
        return;
    }
    
    if(m_ticksSinceShout > 0){
        m_ticksSinceShout--;
        //return;
    }
    
    if(m_numPerpendicularTicks < 200){
      //cout << "a";
        m_numPerpendicularTicks++;
    }
    
    
    if (getWorld()->nearby(this) < 4.0 && getWorld()->facingPlayer(this) == true && m_ticksSinceShout == 0){
        m_ticksSinceShout = 15;
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->annoyTunnelman(this, 2, 4.0);
        return;
    }
    
    
    
    Direction d = canSeePlayer();
    if(d != none){
      //cout << "b";
        if(d == down){
            setDirection(down);
            moveTo(getX(), getY()-1);
        }
        if(d == up){
                setDirection(up);
                moveTo(getX(), getY()+1);
        }
        if(d == left){
            setDirection(left);
            moveTo(getX()-1, getY());
        }
        if(d == right){
            setDirection(right);
            moveTo(getX()+1, getY());
        }
        m_numStepsinCurrDir = 0;
        return;
    }

    GraphObject::Direction perp_dir = randomPerpendicular();
    if(m_numPerpendicularTicks >= 200 && perp_dir!=none){
        m_numPerpendicularTicks = 0;
        setDirection(perp_dir);
        m_numStepsinCurrDir = numSquaresToMoveInCurrentDirection();
    }
    
    if(m_numStepsinCurrDir > 0){
        if(getDirection() == up){
            if(getWorld()->canActorMoveHere(this,getX(), getY()+1)){
                moveTo(getX(), getY()+1);
                m_numStepsinCurrDir--;
                m_numPerpendicularTicks++;
                return;
            }
            else{
                m_numStepsinCurrDir = 0;
                return;
            }
        }
        if(getDirection() == down){
            if(getWorld()->canActorMoveHere(this, getX(), getY()-1)){
                moveTo(getX(), getY()-1);
                m_numStepsinCurrDir--;
                m_numPerpendicularTicks++;
                return;
            }
            else{
                m_numStepsinCurrDir = 0;
                return;
            }
        }
        if(getDirection() == right){
            if(getWorld()->canActorMoveHere(this, getX()+1, getY())){
                moveTo(getX()+1, getY());
                m_numStepsinCurrDir--;
                m_numPerpendicularTicks++;
                return;
            }
            else{
                m_numStepsinCurrDir = 0;
                return;
           }
        }
        if(getDirection() == left){
            if(getWorld()->canActorMoveHere(this, getX()-1, getY())){
                moveTo(getX()-1, getY());
                m_numStepsinCurrDir--;
                m_numPerpendicularTicks++;
                return;
            }
            else{
                m_numStepsinCurrDir = 0;
                return;
            }
        }
    }

    
    Direction a = randomDirection();
    if(m_numStepsinCurrDir <=  0){
        while (getWorld()->legalDirectionMove(this, a) == false){
      //cout << "c";
            a = randomDirection();
        }
        setDirection(a);
        m_numStepsinCurrDir = numSquaresToMoveInCurrentDirection();
    }
    
    
}


GraphObject::Direction Protester::canSeePlayer(){
    if(getY() == getWorld()->getTunnelman()->getY()){
        for(int i = getX(); i < 61; i++){
            if(getWorld()->canActorMoveHere(this, i, getY()) == false){
                break;
            }
            if(i == getWorld()->getTunnelman()->getX()){
                return GraphObject::right;
            }
        }
        for(int i = getX(); i >= 0; i--){
            if(getWorld()->canActorMoveHere(this, i, getY()) == false){
                break;
            }
            if(i == getWorld()->getTunnelman()->getX()){
                return GraphObject::left;
            }
            
        }
    }
    if(getX() == getWorld()->getTunnelman()->getX()){
        for(int i = getY(); i < 61; i++){
            if(getWorld()->canActorMoveHere(this,getX(), i) == false){
                break;
            }
            if(i == getWorld()->getTunnelman()->getY()){
                return GraphObject::up;
            }
            
        }
        for(int i = getY(); i >= 0; i--){
            if(getWorld()->canActorMoveHere(this, getX(), i) == false){
                break;
            }
            if(i == getWorld()->getTunnelman()->getY()){
                return GraphObject::down;
            }
        }
    }
    return GraphObject::none;
}

GraphObject::Direction Protester::randomDirection(){
    int a = rand() % 4;
    if (a == 0){
        return right;
    }
    if (a == 1){
        return left;
    }
    if (a == 2){
        return up;
    }
    if (a == 3){
        return down;
    }
    return none;
}

GraphObject::Direction Protester::randomPerpendicular(){
    if(getDirection() == up || getDirection() == down){
        int c = rand() % 2;
        if (c == 0 && getWorld()->canActorMoveHere(this, getX()-1, getY())){
            return left;
        }
        if (c == 1 && getWorld()->canActorMoveHere(this, getX()+1, getY())){
            return right;
        }
    }
    else if(getDirection() == left || getDirection() == right){
        int c = rand() % 2;
        if (c == 0 && getWorld()->canActorMoveHere(this, getX(), getY()-1)){
            return down;
        }
        if (c == 1 && getWorld()->canActorMoveHere(this, getX(), getY()+1)){
            return up;
        }
    }
    return none;
}
//    health = health - amount;
//if(health <= 0){
//    dies();
//    getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
//}
void Protester::getAnnoyed(int amount){
    bool annoyed = false;
    if(m_lookingForWayOut == true)
        return;
    m_health = m_health - amount;
    if(m_health <= 0){
        if(amount == 100){
            getWorld()->increaseScore(500);
        }
        if(amount == 2){
            getWorld()->increaseScore(100);
        }
        Protester::dies();
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
    }
    else if(getHealth() > 0){
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        m_numRestTicks = m_numOfStunTicks;
    }
}

int Protester::getHealth(){
    return m_health;
}
void Protester::dies(){
    m_lookingForWayOut = true;
}
HardCoreProtester::HardCoreProtester(StudentWorld* world, int imageId, int health, int score):Protester(world, TID_HARD_CORE_PROTESTER, health, score){};

HardCoreProtester::~HardCoreProtester(){};

void HardCoreProtester::addGold(){
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    int stareTime = max(50, 100-(int)getWorld()->getLevel() * 10);
    restTicksSetter(stareTime);
};

void Protester::restTicksSetter(int time){
    m_numRestTicks = time;
}

