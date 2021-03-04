#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iostream>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class GhostRacer;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld () {
        cleanUp();
    }
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    double wl=LEFT_EDGE + ROAD_WIDTH/3;
    double wr=RIGHT_EDGE - ROAD_WIDTH/3;
    
    virtual int init();
    virtual int move();
    GhostRacer* getGR() {
        return m_gr;
    }
    virtual void cleanUp();

          // Add an actor to the world.
    void addActor(Actor* a) {m_actor.push_back(a);}

          // Record that a soul was saved.
    void recordSoulSaved() {savedsouls++;}

          // If actor a overlaps some live actor that is affected by a holy water
          // projectile, inflict a holy water spray on that actor and return true;
          // otherwise, return false.  (See Actor::beSprayedIfAppropriate.)
     //  bool sprayFirstAppropriateActor(Actor* a);

          // Return true if actor a1 overlaps actor a2, otherwise false.
   // bool overlaps(const Actor* a1, const Actor* a2) const;
    bool overlap_with_spray(const Actor* a1) const;
   // int get_num_sprayed(){return num_sprayed;}
    
    bool yes_front(){return f;}
    bool yes_back(){return b;}
    

          // If actor a overlaps this world's GhostRacer, return a pointer to the
          // GhostRacer; otherwise, return nullptr
      //  GhostRacer* getOverlappingGhostRacer(Actor* a) const;

private:
    GhostRacer* m_gr;
    std::vector<Actor*> m_actor;
    double new_border_y=VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y=32; //VIEW_HEIGHT-last_white_y
    int souls2save;//2*getLevel()+5;
    int savedsouls;
    bool b=false;
    bool f=false;
    int bonus;
  //  int num_sprayed=0;
};

#endif // STUDENTWORLD_H_
