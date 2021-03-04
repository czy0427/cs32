#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::default_move() {
    if (getX()<0 || getX()>VIEW_WIDTH || getY()<0 || getY()>VIEW_HEIGHT) {
        setAlive(false);
        return;
    }
    //*******an error here**********
    //set_vert_speed(get_vert_speed() - getWorld()->getGR()->get_vert_speed());
    //which decrement speed every tick!!
    //std::cout<<"borderline speed"<<get_vert_speed()<<std::endl;
    set_vert_speed(-4 - getWorld()->getGR()->get_vert_speed());
    double new_y=getY()+get_vert_speed();
    moveTo(getX(),new_y);
}

bool Actor::overlap() {
    double delta_x=abs(getWorld()->getGR()->getX() - getX());
    double delta_y=abs(getWorld()->getGR()->getY() - getY());
    double rad_sum=getRadius()+getWorld()->getGR()->getRadius();
    if(delta_x<rad_sum*0.25 && delta_y<rad_sum*0.6) {
        return true;
    }
    return false;
}

void BorderLine::doSomething() {
    default_move();
}

void GhostRacer::doSomething() {
    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_LEFT:
                if(getDirection()<114) {
                    setDirection(getDirection()+8);
                }
                break;
            case KEY_PRESS_RIGHT:
                if(getDirection()>66) {
                    setDirection(getDirection()-8);
                }
                break;
            case KEY_PRESS_UP:
                if(get_vert_speed()<5) {
                    set_vert_speed(get_vert_speed()+1);
                }
                break;
            case KEY_PRESS_DOWN:
                if(get_vert_speed()>-1) {
                    set_vert_speed(get_vert_speed()-1);
                }
                break;
            case KEY_PRESS_SPACE:
                if(getNumSprays()>=1) {
                    double x; double y;
                    getPositionInThisDirection(getDirection(), SPRITE_HEIGHT, x, y);
                    getWorld()->addActor(new Spray(getWorld(),x,y,getDirection()));
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    num_spray--;
                }
                break;
        }
    }
   
    if (getX()<ROAD_CENTER - ROAD_WIDTH/2) {
        if (getDirection()>90)
            increaseHP(-10);
        setDirection(82);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
        
    }
    if (getX()>ROAD_CENTER + ROAD_WIDTH/2) {
        if (getDirection()<90)
            increaseHP(-10);
        setDirection(98);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    if (getHP()<=0) {
        getWorld()->playSound(soundWhenDie());
        setAlive(false);
        return;
    }
    //immediately set to dead if reach 0 hp, do I still play the crash sound before it dies???
    
    moveTo();
}

void ZombieCab::doSomething() {
  //  if(getHP()!=3)
  //  std::cout<<getHP()<<std::endl;
    if (isAlive()==false)
        return;
    if(overlap() && !has_overlapped()) {
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
        getWorld()->getGR()->increaseHP(-20);
        if (getX()<=getWorld()->getGR()->getX()) {
            setHorizSpeed(-5);
            setDirection(120+rand()%20);
        }
        if (getX()>getWorld()->getGR()->getX()) {
            setHorizSpeed(5);
            setDirection(60+rand()%20);
        }
        olap=true;
    }
    
    //move
  //  std::cout<<"vert speed: "<<get_vert_speed()<<std::endl;
    set_vert_speed(speed_now - getWorld()->getGR()->get_vert_speed());
  //  std::cout<<"vert speed: "<<get_vert_speed()<<std::endl;
    double new_y=getY()+get_vert_speed();
    double new_x=getX()+getHorizSpeed();
  //  std::cout<<"orig pos: "<<getY()<<std::endl;
    moveTo(new_x,new_y);
    std::cout<<"new pos: "<<getY()<<std::endl;
    if (getX()<0 || getX()>VIEW_WIDTH || getY()<0 || getY()>VIEW_HEIGHT) {
        setAlive(false);
        return;
    }
    //if sprayed
    if(getWorld()->overlap_with_spray(this)) {
        increaseHP(-1);
      //  std::cout<<"life"<<getHP()<<std::endl;
        getWorld()->playSound(SOUND_VEHICLE_HURT);
    }
    
    if(getHP()<=0) {
        setAlive(false);
        getWorld()->playSound(SOUND_VEHICLE_DIE);
        int chance=rand()%5;
        if(chance==0)
            getWorld()->addActor(new OilSlick(getWorld(),getX(),getY()));
        getWorld()->increaseScore(200);
        return;
    }
    
    //movement plan
    distance--;
    if(distance>0)
        return;
    else {
        len=rand()%29+4;
        set_vert_speed(get_vert_speed()+rand()%5-2);
        speed_now=get_vert_speed();
        distance=len;
    }
    
    //step4-
    if(getWorld()->yes_front() && get_vert_speed()>getWorld()->getGR()->get_vert_speed()) {
        set_vert_speed(get_vert_speed()-0.5);
        speed_now=get_vert_speed();
        return;
    }
    if(getWorld()->yes_back() && get_vert_speed()<=getWorld()->getGR()->get_vert_speed()) {
        set_vert_speed(get_vert_speed()+0.5);
        speed_now=get_vert_speed();
        return;
    }
}

void Pedestrian::default_move(){
    if (getX()<0 || getX()>VIEW_WIDTH || getY()<0 || getY()>VIEW_HEIGHT) {
        setAlive(false);
        return;
    }
    set_vert_speed(-4 - getWorld()->getGR()->get_vert_speed());
    double new_y=getY()+get_vert_speed();
    double new_x=getX()+getHorizSpeed();
   // std::cout<<"new_x"<<new_x<<std::endl;
    moveTo(new_x,new_y);
}

void Pedestrian::moveAndPossiblyPickPlan(){
    //========how to understand "distance" vs. "len"??
    default_move(); //do I need the Pedestrian:: here??
    //==========this is one understanding========
  //  mp_distance-=abs(getHorizSpeed());
    mp_distance--;
  //  std::cout<<"distance"<<mp_distance<<std::endl;
    if (mp_distance>0)
        return;
    int range[6]={-3,-2,-1,1,2,3};
    int hspeed=range[rand()%6];
    setHorizSpeed(hspeed);
    len=rand()%29+4;
  //  std::cout<<"len"<<len<<std::endl;
    mp_distance=len;
    if (getHorizSpeed()<0)
        setDirection(180);
    else
        setDirection(0);
 //   std::cout<<"dir"<<getDirection()<<std::endl;
}

void HumanPedestrian::doSomething() {
    moveAndPossiblyPickPlan();
    if (overlap()) {
        setAlive(false);
        getWorld()->getGR()->setAlive(false);
    }
    if(getWorld()->overlap_with_spray(this)) {
        setDirection(getDirection()+180);
        setHorizSpeed(-1*getHorizSpeed());
        getWorld()->playSound(SOUND_PED_HURT);
    }
}

void ZombiePedestrian::doSomething() {
    if(isAlive()==false)
        return;
    //overlap
  //  std::cout<<getHP()<<std::endl;
    if (overlap()) {
        getWorld()->getGR()->increaseHP(-5);
        increaseHP(-2);
        setAlive(false);
     //   std::cout<<getHP()<<std::endl;
    }
    //if damaged
    if(getWorld()->overlap_with_spray(this)) {
        increaseHP(-1);
     //   std::cout<<getHP()<<std::endl;
        if(getHP()>0)
            getWorld()->playSound(SOUND_PED_HURT);
    }
    
    //if close to GR
    if(abs(getX()-getWorld()->getGR()->getX())<=30
       && getY()>getWorld()->getGR()->getY()) {
        setDirection(270);
        if (getX()<getWorld()->getGR()->getX())
            setHorizSpeed(1);
        else if(getX()>getWorld()->getGR()->getX())
            setHorizSpeed(-1);
        else
            setHorizSpeed(0);
        
        tick--;
        if (tick<=0) {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            tick=20;
        }
    }
    
    //check dead
    if(getHP()<=0) {
        setAlive(false);
        getWorld()->playSound(SOUND_PED_DIE);
        getWorld()->increaseScore(150);
        if(!overlap()) {
            int chance=rand()%5;
            if(chance==0)
                getWorld()->addActor(new HealingGoodie(getWorld(),getX(),getY()));
        }
    }
    
    moveAndPossiblyPickPlan();
}

void SoulGoodie::doSomething() {
    default_move();
    
    //rotate
    setDirection(getDirection()-10);
    
    //if overlaps
    if(overlap()) {
        getWorld()->playSound(getSound());
        setAlive(false);
        getWorld()->increaseScore(100);
        getWorld()->recordSoulSaved();
    }
    
}

void OilSlick::doSomething(){
    default_move();
    if(overlap()) {
        getWorld()->playSound(getSound());
        for(;;) {
            int rand_turn=rand()%16+5;
            int multiplier[2]={-1,1};
            int m=multiplier[rand()%2];
            int new_d=getWorld()->getGR()->getDirection()+rand_turn*m;
            if (new_d>=60 && new_d <=120) {
                getWorld()->getGR()->setDirection(new_d);
                break;
            }
        }
    }
}

void HolyWaterGoodie::doSomething(){
    default_move();
    if (overlap()) {
        setAlive(false);
        getWorld()->getGR()->increaseSprays(10);
        getWorld()->increaseScore(50);
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

void HealingGoodie::doSomething(){
    default_move();
    if(overlap()) {
        setAlive(false);
        if(getWorld()->getGR()->getHP()<=90)
            getWorld()->getGR()->increaseHP(10);
        getWorld()->playSound(getSound());
        getWorld()->increaseScore(250);
    }
}

void Spray::doSomething() {
    if(isAlive()==false)
        return;
    if (getX()<0 || getX()>VIEW_WIDTH || getY()<0 || getY()>VIEW_HEIGHT) {
        setAlive(false);
        return;
    }
    moveForward(SPRITE_HEIGHT);
    travel_d+=SPRITE_HEIGHT;
    if(travel_d>=160)
        setAlive(false);
}
