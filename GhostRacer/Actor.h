#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//don't include "StudentWorld.h"! circular dependency,
//would cause calling GhostRacer in StudentWorld fail!!
#include <cmath>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor:public GraphObject {
public:
    Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir, double size, unsigned int depth): GraphObject(imageID, startX, startY,dir,size,depth), m_sw(sw)
    //dir, size, depth place in GraphObject or not?
    {
        setDirection(dir);
        setSize(size);
        m_ID=imageID;
        m_alive=true;
        v_speed=-4;
    }
 //   virtual ~Actor();
    StudentWorld* getWorld() {
        return m_sw;
    }
    
    virtual void doSomething()=0; //can't compile if not defined
    
    bool isAlive() {
        return m_alive;
    }
    void setAlive(bool alive) {
        m_alive=alive;
    }
    void set_vert_speed(double spd) {
        v_speed=spd;
    }
    double get_vert_speed() {
        return v_speed;
    }
    int getID() const{
        return m_ID;
    }
    virtual void default_move();
    bool overlap();
    virtual bool beSprayedIfAppropriate(){return false;}
    virtual bool isCollisionAvoidanceWorthy() const{return false;}
    
private:
    StudentWorld* m_sw;
    bool m_alive;
    int m_ID;
    double v_speed;
};

class BorderLine:public Actor {
public:
  //  BorderLineY(int imageID, double startX, double startY, int dir, double size, unsigned int depth): Actor(IID_YELLOW_BORDER_LINE, startX, startY, dir, size, depth)
    //vertical speed=-4
    BorderLine(StudentWorld* sw, int imageID, double x, double y):Actor(sw,imageID,x,y,0,2,2){
    //    set_vert_speed(orig_speed);//initializa as -4
    }
    BorderLine(StudentWorld* sw, int imageID, double x, double y, double speed):Actor(sw,imageID,x,y,0,2,2){
        set_vert_speed(speed);//initializa as -4
    }
    virtual void doSomething();
   
};


class Agent : public Actor
{
public:
    //all depth 0????
    Agent(StudentWorld* sw, int imageID, double x, double y, int dir, double size, int hp):Actor(sw,imageID,x,y,dir,size,0) {m_hp=hp;}
  //  virtual bool isCollisionAvoidanceWorthy() const;

      // Get hit points. //get health point
    int getHP() const {return m_hp;}

      // Increase hit points by hp.
    void increaseHP(int hp){m_hp+=hp;}

      // Do what the spec says happens when hp units of damage is inflicted.
      // Return true if this agent dies as a result, otherwise false.
  //  virtual bool takeDamageAndPossiblyDie(int hp);

      // What sound should play when this agent is damaged but does not die?
  //  virtual int soundWhenHurt();

      // What sound should play when this agent is damaged and dies?
  //  virtual int soundWhenDie();
    virtual bool isCollisionAvoidanceWorthy() const{return true;}
    
private:
    int m_hp;
};

class GhostRacer:public Agent {
public:
    GhostRacer(StudentWorld* sw):Agent(sw, IID_GHOST_RACER,128,32,90,4,100){
        set_vert_speed(0);
    }
    virtual void doSomething();
    
    virtual void moveTo() {
      //  Actor::moveTo((cur_x+delta_x), cur_y);
        Actor::moveTo(getX()+4*cos(getDirection()*M_PI/180), getY());
    }

    int getNumSprays() const{return num_spray;}
    void increaseSprays(int amt) {num_spray+=amt;}
    
    virtual int soundWhenDie() const {return SOUND_PLAYER_DIE;}
    
    
private:
    /*
    const double max_shift_per_tick=4.0;
    int m_dir=getDirection();
    double delta_x=cos(m_dir)*max_shift_per_tick;
    double cur_x=getX();
    double cur_y=getY();
     */
    int num_spray=10;

};

class ZombieCab : public Agent
{
public:
    ZombieCab(StudentWorld* sw, double x, double y, double v_speed):Agent(sw, IID_ZOMBIE_CAB,x,y,90,4,3) {
        set_vert_speed(v_speed);
        speed_now=v_speed;
    }
    virtual void doSomething();
  //  virtual bool beSprayedIfAppropriate();
    int getHorizSpeed() const{return h_speed;}
    void setHorizSpeed(int s){h_speed=s;}
    bool has_overlapped(){return olap;}
private:
    double h_speed=0;
    bool olap=false;
    int distance=0;
    int len=0;
    double speed_now;
};


//===========Pedestrian============
class Pedestrian : public Agent
{
public:
    Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size):Agent(sw, imageID, x ,y, 0, size,2){ h_speed=0; mp_distance=0;}
  //  virtual int soundWhenHurt() const;
 //   virtual int soundWhenDie() const;

      // Get the pedestrian's horizontal speed
    int getHorizSpeed() const{return h_speed;}

      // Set the pedestrian's horizontal speed
    void setHorizSpeed(int s){h_speed=s;}

      // Move the pedestrian.  If the pedestrian doesn't go off screen and
      // should pick a new movement plan, pick a new plan.
    void moveAndPossiblyPickPlan();
    virtual void default_move();
    
private:
    int h_speed;
    int mp_distance;
    int len=0;
};

class HumanPedestrian : public Pedestrian
{
public:
    HumanPedestrian(StudentWorld* sw, double x, double y):Pedestrian(sw,IID_HUMAN_PED, x,y,2){}
    virtual void doSomething();
  //  virtual bool beSprayedIfAppropriate();
  //  virtual bool takeDamageAndPossiblyDie(int hp);
    virtual bool isCollisionAvoidanceWorthy() const{return true;}
};

class ZombiePedestrian : public Pedestrian
{
public:
    ZombiePedestrian(StudentWorld* sw, double x, double y):Pedestrian(sw,IID_ZOMBIE_PED, x,y,3){}
    virtual void doSomething();
   // virtual bool beSprayedIfAppropriate();
private:
    int tick=0;
};


//============Goodies============
class GhostRacerActivatedObject : public Actor
{
public:
    //all depth of 2?
    GhostRacerActivatedObject(StudentWorld* sw, int imageID, double x, double y, double size, int dir):Actor(sw, imageID, x, y, dir, size, 2) {}
    
   // virtual bool beSprayedIfAppropriate();

      // Do the object's special activity (increase health, spin Ghostracer, etc.)
  //  virtual void doActivity(GhostRacer* gr) = 0;

      // Return the object's increase to the score when activated.
  //  virtual int getScoreIncrease() const = 0;

      // Return the sound to be played when the object is activated.
    virtual int getSound() const=0;

      // Return whether the object dies after activation.
//    virtual bool selfDestructs() const = 0;

      // Return whether the object is affected by a holy water projectile.
 //   virtual bool isSprayable() const = 0;
    
private:
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(StudentWorld* sw, double x, double y):GhostRacerActivatedObject(sw, IID_SOUL_GOODIE, x, y, 4, 0){}
    virtual void doSomething();
 //   virtual void doActivity(GhostRacer* gr);
//    virtual int getScoreIncrease() const;
    virtual int getSound() const {return SOUND_GOT_SOUL;}
 //   virtual bool selfDestructs() const;
 //   virtual bool isSprayable() const;
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(StudentWorld* sw, double x, double y):GhostRacerActivatedObject(sw, IID_OIL_SLICK, x, y, (double)(rand()%4+2), 0){}
    virtual void doSomething();
  //  virtual void doActivity(GhostRacer* gr);
  //  virtual int getScoreIncrease() const;
    virtual int getSound() const {return SOUND_OIL_SLICK;}
  //  virtual bool selfDestructs() const;
  //  virtual bool isSprayable() const;
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(StudentWorld* sw, double x, double y):GhostRacerActivatedObject(sw, IID_HOLY_WATER_GOODIE, x, y, 2, 90){}
    virtual void doSomething();
    virtual int getSound() const {return SOUND_GOT_GOODIE;}
  //  virtual void doActivity(GhostRacer* gr);
  //  virtual int getScoreIncrease() const;
  //  virtual bool selfDestructs() const;
  //  virtual bool isSprayable() const;
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(StudentWorld* sw, double x, double y):GhostRacerActivatedObject(sw, IID_HEAL_GOODIE, x, y, 1, 0){}
    virtual void doSomething();
    virtual int getSound() const {return SOUND_GOT_GOODIE;}
 //   virtual void doActivity(GhostRacer* gr);
  //  virtual int getScoreIncrease() const;
  //  virtual bool selfDestructs() const;
  //  virtual bool isSprayable() const;
};

class Spray : public Actor
{
public:
    Spray(StudentWorld* sw, double x, double y, int dir):Actor(sw,IID_HOLY_WATER_PROJECTILE,x,y,dir,1,1){

    }
    virtual void doSomething();
    
private:
    double max_travel=160;
    double travel_d=0;
};
#endif // ACTOR_H_
