#include "StudentWorld.h"
#include "Actor.h"
#include <algorithm>
#include <string>
#include <queue>
#include <utility>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir): GameWorld(assetDir){};

StudentWorld::~StudentWorld(){};


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::init(){
//    for (int i = 0; i < 30; i++){
//        for (int j = 0; j < VIEW_HEIGHT-4; j++){
//            m_arena[i][j] = new Earth(i, j, this);
//        }
//    }
//    for (int i = 33; i < VIEW_WIDTH; i++){
//        for (int j = 0; j < VIEW_HEIGHT-4; j++){
//            m_arena[i][j] = new Earth(i, j, this);
//        }
//    }
    probabilityHardCore = min(90, (int)getLevel() * 10 + 30);
    probabilityOfItem = (int)getLevel() * 25 + 300;
    itemSpawnTime = max(100, 300-10*((int)getLevel()));
    m_numTicks = 0;
    m_numTicksBeforeProtester = max(25, 200-(int)getLevel());
    m_numProtesters = 0;
    m_numTargetProtesters = min(15, 2+(int)((int)getLevel()*1.5));
    
    for(int i = 0; i < VIEW_WIDTH; i++){
        for(int k = 0; k < VIEW_HEIGHT ; k++){
            if(i >= 30 && i <= 33){
                if (k >= 4){
                    m_arena[i][k] = nullptr;
                }
                else{
                    m_arena[i][k] = new Earth(i, k, this);
                }
            }
            else if(k > 59){
                m_arena[i][k] = nullptr;
            }
            else{
                m_arena[i][k] = new Earth(i, k, this);
            }
        }
    }
    
    
//    for(int i = 0; i < VIEW_WIDTH; i++){
//        m_arena[i][0] = new Earth(i, 15, this);
//        m_arena[i][4] = new Earth(i, 15, this);
//
//    }
//    m_arena[0][0] = new Earth(0, 0, this);
    m_tunnelman = new TunnelMan(this);
    spawnTime = max(100, 300-10*((int)getLevel()));
    
    
    
    int B = min((int)getLevel() / 2 + 2, 9);
    for(int i = 0; i < B; i++){
        int x, y;
        randomLocation(x, y, 0, 60, 20, 56);
        while(canActorGoHerePlacement(x, y) == false){
            randomLocation(x, y, 0, 60, 20, 56);
        }
        addActor(new Boulder(this, x, y));
    }
    
    int L = min(2 + (int)getLevel(), 21);
    for(int i = 0; i < L; i++){
        int x,y;
        randomLocation(x, y, 0, 60, 0, 56);
        while(canActorGoHerePlacement(x, y) == false){
            randomLocation(x, y, 0, 60, 0, 56);
        }
        addActor(new BarrelsOfOil(x, y, this));
    }
    m_numBarrels = L;
    
    int G = max(5-(int)getLevel() / 2, 2);
    for(int i = 0; i < G; i++){
        int x,y;
        randomLocation(x, y, 0, 60, 0, 56);
        while(canActorGoHerePlacement(x, y) == false){
            randomLocation(x, y, 0, 60, 0, 56);
        }
        addActor(new Gold(x, y, this, false));
    }
    return GWSTATUS_CONTINUE_GAME;
}



int StudentWorld::move()
{
    setDisplayText();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

    if(m_numTicks == 0 && m_numProtesters < m_numTargetProtesters){
        int random = rand() % 100;
        cout << random << std::endl;
        if(random < probabilityHardCore)
            addActor(new HardCoreProtester(this, TID_HARD_CORE_PROTESTER, 20, 250));
        else
            addActor(new Protester(this, TID_PROTESTER, 5, 100));
        m_numProtesters++;
        m_numTicks = m_numTicksBeforeProtester;
        }
    else if(m_numTicks > 0){
        m_numTicks--;
    }
        m_tunnelman->doSomething();
        
        if(addItem() == true){
            if(whichItem() == 1)
                addActor(new Sonar(this));
            else{
                int x,y;
                randomLocation(x, y, 0, VIEW_WIDTH-4, 0, VIEW_HEIGHT-4);
                while(poolCheck(x, y) == false){
                     randomLocation(x, y, 0, VIEW_WIDTH-4, 0, VIEW_HEIGHT-4);
                }
                addActor(new WaterPool(this, x, y));
            }
        }
        for(int i = 0; i < actor_vector.size(); i++){
            if(actor_vector[i]->isAlive()){
                actor_vector[i]->doSomething();
                if(m_numBarrels == 0){
                    playSound(SOUND_FINISHED_LEVEL);
                    return GWSTATUS_FINISHED_LEVEL;
                }
            }
        }

    
    if(m_tunnelman->isAlive() == false){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    int count_protestors = 0;
    for(vector<Actor*>::iterator it = actor_vector.begin(); it != actor_vector.end();){
        if (!(*it)->isAlive()){
            delete *it;
            it = actor_vector.erase(it);
        }
        else{
            if ((*it)->getID() == TID_PROTESTER || TID_HARD_CORE_PROTESTER)
                count_protestors ++;
            it++;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    for (int i = 0; i < 64; i++){
        for (int j = 0; j < 64; j++){
            delete m_arena[i][j];
            m_arena[i][j] = nullptr;
        }
    }
    delete m_tunnelman;
    m_tunnelman = nullptr;
    vector<Actor*>::iterator it = actor_vector.begin();
    while(it != actor_vector.end()){
        delete *it;
        it = actor_vector.erase(it);
    }
};


int StudentWorld::dig(int x, int y){
    int result = 0;
    for(int i = x; i < x+4 && i <= 64; i++){
        for(int j = y; j < y+4 && j <= 60; j++){
            if(i <= 64 && j <= 60){
                if(m_arena[i][j] != nullptr){
                    result = 1;
                    delete m_arena[i][j];
                    m_arena[i][j] = nullptr;
                }
            }
        }
    }
    return result;
}

double StudentWorld::distance(int objectLocationX, int PersonLocationX, int objectLocationY, int PersonLocationY){
    int x = objectLocationX - PersonLocationX;
    int y = objectLocationY - PersonLocationY;
    double distance = sqrt((x*x)+(y*y));
    return distance;
}

double StudentWorld::nearby(Actor* actor){
    return distance(actor->getX(), m_tunnelman->getX(), actor->getY(), m_tunnelman->getY());
}

bool StudentWorld::canGoTo(int x, int y){
    for (int i = x; i < x+4 && i < 64; i++){
        for (int j = y; j < y+4 && j < 64; j++){
            if (m_arena[i][j] != nullptr){
                return false;
            }
        }
    }
    return true;
}

void StudentWorld::barrelFound(){
    m_numBarrels--;
}

void StudentWorld::goldFound(){
    m_numGold--;
}

int StudentWorld::getSpawnTime(){
    return spawnTime;
}

void StudentWorld::goldPickedUp(){
    m_tunnelman->addGold();
}
void StudentWorld::waterPickedUp(){
    m_tunnelman->addWater();

}
void StudentWorld::sonarPickedUp(){
    m_tunnelman->addSonar();
}


void StudentWorld::setDisplayText(){
    //get the stats
    int level = getLevel();
    int lives = getLives();
    int health = m_tunnelman->getHealth();
    int squirts = m_tunnelman->getWater();
    int gold = m_tunnelman->getGold();
    int barrelsLeft = m_numBarrels;
    int sonar = m_tunnelman->getSonar();
    int score = getScore();
    
    //format the stats
    string s = formatDisplayText(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
    //display the stats
    setGameStatText(s);
}

std::string StudentWorld::formatDisplayText(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score){
    string scoreString = to_string(score);
    while(scoreString.size() != 6){
        scoreString = "0"+scoreString;
    }
    return ("Lvl: "+to_string(level)+"  Lives: "+to_string(lives)+"  Hlth: "+to_string(health*10)+"%"+"  Wtr: "+to_string(squirts)+"  Gld: "+to_string(gold)+"  Oil Left: "+to_string(barrelsLeft)+"  Sonar: "+to_string(sonar)+"  Scr: "+scoreString);
}

void StudentWorld::addActor(Actor* actor){
    actor_vector.push_back(actor);
}

bool StudentWorld::canActorGoHerePlacement(int x, int y){
    if(x >= 26 && x <= 34)
        return false;
    vector<Actor*>::iterator it = actor_vector.begin();
    while(it != actor_vector.end()){
        if(distance(x, (*it)->getX(), y, (*it)->getY()) <= 6)
            return false;
        it++;
    }
    return true;
}

void StudentWorld::randomLocation(int &x, int &y, int xBottom, int xTop, int yBottom, int yTop){
    x = rand() % (1 + xTop - xBottom) + xBottom;
    y = rand() % (1 + yTop - yBottom) + yBottom;
}

bool StudentWorld::canActorMoveHere(Actor* actor, int x, int y){
    if(canGoTo(x,y) == false & actor != m_tunnelman){
        return false;
    }
    for(vector<Actor*>::iterator it = actor_vector.begin(); it != actor_vector.end(); it++){
        if((x < 0 || x > 60 || y < 0 || y > 60) || ((*it)->getImageID() == TID_BOULDER && distance(x, (*it)->getX(), y, (*it)->getY()) <= 3)){
            return false;
        }
    }
    return true;
}
    
void StudentWorld::activateSonar(){
    if(m_tunnelman->getSonar() > 0){
        playSound(SOUND_SONAR);
        for(vector<Actor*>::iterator it = actor_vector.begin(); it != actor_vector.end(); it++){
            if(nearby(*it) < 12){
                (*it)->setVisible(true);
            }
        }
        m_tunnelman->loseSonar();
    }
    if(m_tunnelman->getSonar() == 0){
        return;
    }
}

bool StudentWorld::annoyTunnelman(Actor* actor, int amount, double radius){
    if (nearby(actor) < radius){
        m_tunnelman->getAnnoyed(amount);
        return true;
    }
    return false;
}

bool StudentWorld::annoyProtester(Actor* actor, int amount, double radius){
    bool annoyed = false;
    for(vector<Actor*>::iterator it = actor_vector.begin(); it != actor_vector.end(); it++){
        if((distance((*it)->getX(), actor->getX(), (*it)->getY(), actor->getY()) <= radius) && ((*it)->getImageID() == TID_PROTESTER || (*it)->getImageID() == TID_HARD_CORE_PROTESTER))
            (*it)->getAnnoyed(amount);
            annoyed = true;
    }
    return annoyed;
}

void StudentWorld::decrementProtester(){
    m_numProtesters--;
}

TunnelMan* StudentWorld::getTunnelman(){
    return m_tunnelman;
}

void StudentWorld::placingEarths(){
    for (int i = 0; i < 61; i++){
        for (int j = 0; j < 61; j++){
            if(m_arena[i][j] == nullptr){
                m_mazeSteps[i][j] = -2;
            }
            if((m_arena[i][j] != nullptr)){
                m_mazeSteps[i][j] = -1;
            }
            //check for boulders
            if((canActorMoveHere(m_tunnelman, i, j)) == false){
                m_mazeSteps[i][j] = -1;
            }
        }
    }
}

vector<vector<int>> StudentWorld::bfsMaze(vector<vector<int>>& maze, Actor* actor) {
    int rows = 61;
    int cols = 61;

    // Directions for Up, Down, Left, Right
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Using a queue for BFS with starting point and its distance as 0
    // Queue contains tuple of (row, col, distance)
    queue<pair<pair<int, int>, int>> q;
    if (actor == m_tunnelman){
        q.push({{actor->getX(), actor->getY()}, 0});
    }
    else{
        q.push({{60, 60}, 0});
    }
    while (!q.empty()) {
        int current_row = q.front().first.first;
        int current_col = q.front().first.second;
        int distance = q.front().second;
        q.pop();

        // If the current spot is valid
        if (maze[current_row][current_col] == -2) {
            maze[current_row][current_col] = distance;

            for (int i = 0; i < 4; ++i) {
                int new_row = current_row + directions[i][0];
                int new_col = current_col + directions[i][1];
                // Check for out-of-bounds or if it's a wall or already visited spot
                if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols && maze[new_row][new_col] == -2) {
                    q.push({{new_row, new_col}, distance + 1});
                }
            }
        }
    }

    return maze;
}

GraphObject::Direction StudentWorld::directionToTarget(Actor* actor){
    placingEarths();
    std::vector<std::vector<int>> reformattedMaze = arrayToVector(m_mazeSteps);
    
    bfsMaze(reformattedMaze, actor);
    int distance = 10000;
    GraphObject::Direction d;
    
    if(actor->getX()-1 >= 0){
        if (reformattedMaze[actor->getX()-1][actor->getY()]>=0 && reformattedMaze[actor->getX()-1][actor->getY()] < distance && legalDirectionMove(actor, GraphObject::left)){
            distance = reformattedMaze[actor->getX()-1][actor->getY()];
            d = GraphObject::left;
        }
    }
    if(actor->getX()+1 <= 60){
        if (reformattedMaze[actor->getX()+1][actor->getY()]>=0 && reformattedMaze[actor->getX()+1][actor->getY()] < distance && legalDirectionMove(actor, GraphObject::right)){
            distance = reformattedMaze[actor->getX()+1][actor->getY()];
            d = GraphObject::right;
        }
    }
    if(actor->getY()+1 <= 60){
        if (reformattedMaze[actor->getX()][actor->getY()+1]>=0 && reformattedMaze[actor->getX()][actor->getY()+1] < distance  && legalDirectionMove(actor, GraphObject::up)){
            distance = reformattedMaze[actor->getX()][actor->getY()+1];
            d = GraphObject::up;
        }
    }
    if(actor->getY()-1 >= 0){
        if (reformattedMaze[actor->getX()][actor->getY()-1]>=0 && reformattedMaze[actor->getX()][actor->getY()-1] < distance  && legalDirectionMove(actor, GraphObject::down)){
            distance = reformattedMaze[actor->getX()][actor->getY()-1];
            d = GraphObject::down;
        }
    }
    return d;
}

std::vector<std::vector<int>> StudentWorld::arrayToVector(int arr[61][61]){
    std::vector<std::vector<int>> result(61, std::vector<int>(61));
    for (int i = 0; i < 61; ++i) {
        for (int j = 0; j < 61; ++j) {
            result[i][j] = arr[i][j];
        }
    }
    return result;
}

bool StudentWorld::facingPlayer(Actor* actor){
    GraphObject::Direction d = actor->getDirection();
    if (d == GraphObject::up && actor->getX() == m_tunnelman->getX() && actor->getY() < m_tunnelman->getY()){
        return true;
    }
    if (d == GraphObject::down && actor->getX() == m_tunnelman->getX() && actor->getY() > m_tunnelman->getY()){
        return true;
    }
    if (GraphObject::right && actor->getX() < m_tunnelman->getX() && actor->getY() == m_tunnelman->getY()){
        return true;
    }
    if (GraphObject::left && actor->getX() > m_tunnelman->getX() && actor->getY() == m_tunnelman->getY()){
        return true;
    }
    return false;
}


// Plan For Maze so our protestors can leave:
// will use two 2d arrays; will first create a 2-d array of places where earth is and where earth isn't. Then, in the second 2-d array, we will assign values to all the nonEarth places. In doing, so we will mark all of the places without Earth and an annoyed protestor simply has to look to the four spots around him to determine, which was is most efficient.

bool StudentWorld::legalDirectionMove(Actor* actor, GraphObject::Direction dir){
    GraphObject::Direction direction = dir;
    if (direction == GraphObject::up){
        if(canActorMoveHere(actor, actor->getX(), actor->getY()+1)){
            return true;
        }
    }
    else if (direction == GraphObject::down){
        if(canActorMoveHere(actor, actor->getX(), actor->getY()-1)){
            return true;
        }
    }
    else if (direction == GraphObject::left){
        if(canActorMoveHere(actor, actor->getX()-1, actor->getY())){
            return true;
        }
    }
    else if (direction == GraphObject::right){
        if(canActorMoveHere(actor, actor->getX()+1, actor->getY())){
            return true;
        }
    }
    return false;
}

bool StudentWorld::addItem(){
    int d = rand() % probabilityOfItem;
    if(d == 0){
        return true;
    }
    return false;
}

int StudentWorld::whichItem(){
    //check if a goodie needs to be added
    int e = rand() % 5;
    if(e == 0)
        return 1;
    return 2;
}

bool StudentWorld::poolCheck(int x, int y){
    for(int i = x; i < x+4 && i<64; i++)
        for(int j = y; j < y+4 && j<64; j++)
            if(m_arena[i][j]!=nullptr)
                return false;
    for(vector<Actor*>::iterator it = actor_vector.begin(); it != actor_vector.end(); it++){
        if(distance(x, (*it)->getX(), y, (*it)->getY()) <= 6)
            return false;
    }
    return true;
}

bool StudentWorld::bribe(Actor* actor, int radius){
    for (vector<Actor*>::iterator it = actor_vector.begin(); it != actor_vector.end(); it++){
        if(distance(actor->getX(), (*it)->getX(), actor->getY(), (*it)->getY()) <= radius){
            if ((*it)->getImageID() == TID_PROTESTER || (*it)->getImageID() == TID_HARD_CORE_PROTESTER){
                (*it)->addGold();
                return true;
            }
        }
    }
        return false;
}

void StudentWorld::dropGold(){
    if (m_tunnelman->getGold() > 0){
        addActor(new Gold(m_tunnelman->getX(), m_tunnelman->getY(), this, true));
        m_tunnelman->loseGold();
    }
    return;
}
