#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    souls2save=2*getLevel()+5;
    savedsouls=0;
    bonus=5000;
    //borderline
    int N=VIEW_HEIGHT / SPRITE_HEIGHT;
    int M=VIEW_HEIGHT / (4*SPRITE_HEIGHT);
  //  double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
  //  double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
  //  double wl=LEFT_EDGE + ROAD_WIDTH/3;
  //  double wr=RIGHT_EDGE - ROAD_WIDTH/3;
    
    for (int i=0;i<N;i++) {
        m_actor.push_back(new BorderLine(this,IID_YELLOW_BORDER_LINE, LEFT_EDGE,i*SPRITE_HEIGHT));
    }
    for (int i=0;i<N;i++) {
        m_actor.push_back(new BorderLine(this,IID_YELLOW_BORDER_LINE, RIGHT_EDGE,i*SPRITE_HEIGHT));
    }
    for (int i=0;i<M;i++) {
        m_actor.push_back(new BorderLine(this,IID_WHITE_BORDER_LINE, wl,i*4*SPRITE_HEIGHT));
    }
    for (int i=0;i<M;i++) {
        m_actor.push_back(new BorderLine(this,IID_WHITE_BORDER_LINE, wr,i*4*SPRITE_HEIGHT));
    }
    
    //ghostracer
    m_gr=new GhostRacer(this);
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //==================check level======================
    if (m_gr->isAlive()==false) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if (souls2save-savedsouls==0) {
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //===================display text==================
    ostringstream oss;
    oss<<"Score: "<<getScore();
    oss<<setw(7)<<"Lvl: ";
    oss<<getLevel()<< "  Souls2Save: "<<souls2save-savedsouls;
    oss<<"  Lives: "<<getLives()<<"  Health: "<<getGR()->getHP();
    oss<< "  Sprays: " <<getGR()->getNumSprays()<< "  Bonus: " << bonus;
    string text=oss.str();
    setGameStatText(text);
    
    //==================do something======================
    m_gr->doSomething();
    for (int i=0; i<m_actor.size();i++) {
        if (m_actor[i]->isAlive()==true) {
            m_actor[i]->doSomething();
        }
    }
    bonus--;
    
    //==================borderline movement======================
    /*
    double last_white_y=0;
    for (int i=int(m_actor.size())-1; i>=0; i--) {
        if (m_actor[i]->getID()==IID_WHITE_BORDER_LINE) {
            last_white_y=m_actor[i]->getY();
            break;
       }
    }
     */
    // double new_border_y=VIEW_HEIGHT - SPRITE_HEIGHT; //256-8=248
  //   double delta_y=new_border_y - last_white_y;
   
    delta_y-=(-4-getGR()->get_vert_speed());

    if (delta_y>=SPRITE_HEIGHT) {
        m_actor.push_back(new BorderLine(this,IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2,new_border_y));
        m_actor.push_back(new BorderLine(this,IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2,new_border_y));
    }
    if(delta_y>=4*SPRITE_HEIGHT) {
        m_actor.push_back(new BorderLine(this,IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3,new_border_y));
        m_actor.push_back(new BorderLine(this,IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3,new_border_y));
        delta_y=0;
    }
    
    //==================lost soul======================
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    int ChanceOfLostSoul=100;
    int num_soul=rand()%ChanceOfLostSoul;
    double r_x= LEFT_EDGE + (double)(rand()) / ((double)(RAND_MAX/(RIGHT_EDGE - LEFT_EDGE)));
    if (num_soul==0)
        m_actor.push_back(new SoulGoodie(this,r_x, VIEW_HEIGHT));
    
    //==================oil slick======================
    int ChanceOfOilSlick=max(150-getLevel()*10,40);
    int num_oil=rand()%ChanceOfOilSlick;
    if (num_oil==0)
        m_actor.push_back(new OilSlick(this,LEFT_EDGE + (double)(rand()) / ((double)(RAND_MAX/(RIGHT_EDGE - LEFT_EDGE))), VIEW_HEIGHT));
    
    //==================Holy Water======================
    int ChanceOfHolyWater=100+10*getLevel();
    int num_water=rand()%ChanceOfHolyWater;
    if (num_water==0)
        m_actor.push_back(new HolyWaterGoodie(this,LEFT_EDGE + (double)(rand()) / ((double)(RAND_MAX/(RIGHT_EDGE - LEFT_EDGE))), VIEW_HEIGHT));
    
    //==================Human Ped======================
    int ChanceHumanPed=max(200-getLevel()*10,30);
    int num_human=rand()%ChanceHumanPed;
    if (num_human==0)
        m_actor.push_back(new HumanPedestrian(this,(double)(rand()%(VIEW_WIDTH+1)),VIEW_HEIGHT));
    
    //==================Zombie Ped======================
    int ChanceZombiePed=max(100-getLevel()*10,20);
    int num_zombie=rand()%ChanceZombiePed;
    if (num_zombie==0)
        m_actor.push_back(new ZombiePedestrian(this,(double)(rand()%(VIEW_WIDTH+1)),VIEW_HEIGHT));
    
    //==================Zombie cab======================
    double left=ROAD_CENTER - ROAD_WIDTH/3;
    double middle=ROAD_CENTER;
    double right=ROAD_CENTER + ROAD_WIDTH/3;
    double lanes[3]={left,middle,right};
    double bound1[3]={LEFT_EDGE,wl,wr};
    double bound2[3]={wl,wr,RIGHT_EDGE};
    int chosen[3]={-1,-1,-1};
    int ChanceZombieCab=max(100-getLevel()*10,20);
    int num_cab=rand()%ChanceZombieCab;
    if (num_cab==0) {
        int j=0;
        while(j!=3) {
            int pick=rand()%3;
            if(pick!=chosen[0] && pick!=chosen[1] && pick!=chosen[2]) {
                chosen[j]=pick;
                j++;
            }
        }
 
        //repeat for each lane
        for(int x=0;x<3;x++) {
            int k=chosen[x];
            double min=-1;
            double max=-1;
            for(int i=0;i<m_actor.size();i++) {
                if(m_actor[i]->isCollisionAvoidanceWorthy()
                   && m_actor[i]->getX()>bound1[k] && m_actor[i]->getX()<bound2[k]) {
                    if(min==-1 || m_actor[i]->getY() < min)
                        min=m_actor[i]->getY();
                    if(max==-1 || m_actor[i]->getY() > max)
                        max=m_actor[i]->getY();
                }
            }
            if((min==-1 || min>VIEW_HEIGHT/3)
               && (getGR()->getX()<bound1[k] || getGR()->getX()>bound2[k])) {
                int cur_lane=lanes[k];
                double spd=getGR()->get_vert_speed()+rand()%3+2;
          //      cout<<speed<<endl;
                m_actor.push_back(new ZombieCab(this,cur_lane,SPRITE_HEIGHT/2, spd));
           //     m_actor.back()->set_vert_speed(getGR()->get_vert_speed()+rand()%3+2);
                //===============check front and back closet CAW Actor===================
                int f_close=-1;
                int b_close=-1;
                for(int i=0;i<m_actor.size();i++) {
                    if(m_actor[i]->isCollisionAvoidanceWorthy()
                       &&m_actor[i]->getX()>bound1[k]
                       && m_actor[i]->getX()<bound2[k]) {
                        //in front
                        if(m_actor[i]->getY()>m_actor.back()->getY()) {
                            double delta=m_actor[i]->getY()-m_actor.back()->getY();
                            if(f_close==-1 || delta<f_close)
                                f_close=delta;
                        }
                        if(m_actor[i]->getY()<m_actor.back()->getY()) {
                            double delta_b=m_actor.back()->getY()-m_actor[i]->getY();
                            if(b_close==-1 || delta_b<b_close)
                                b_close=delta_b;
                        }
                    }
                }
                if(f_close>=0 &&f_close<96)
                    f=true;
                if(b_close>=0 && b_close<96)
                    b=true;
                
                break;
            }
            
            else if(max==-1 || max<VIEW_HEIGHT*2/3) {
                int cur_lane=lanes[k];
                double speed=getGR()->get_vert_speed()-(rand()%3+2);
                m_actor.push_back(new ZombieCab(this,cur_lane,VIEW_HEIGHT-SPRITE_HEIGHT/2,speed));
          //      cout<<speed<<endl;
                int f_close=-1;
                int b_close=-1;
                for(int i=0;i<m_actor.size();i++) {
                    if(m_actor[i]->isCollisionAvoidanceWorthy()
                       &&m_actor[i]->getX()>bound1[k]
                       && m_actor[i]->getX()<bound2[k]) {
                        //in front
                        if(m_actor[i]->getY()>m_actor.back()->getY()) {
                            double delta=m_actor[i]->getY()-m_actor.back()->getY();
                            if(f_close==-1 || delta<f_close)
                                f_close=delta;
                        }
                        if(m_actor[i]->getY()<m_actor.back()->getY()) {
                            double delta_b=m_actor.back()->getY()-m_actor[i]->getY();
                            if(b_close==-1 || delta_b<b_close)
                                b_close=delta_b;
                        }
                    }
                }
                if(f_close>=0 &&f_close<96)
                    f=true;
                if(b_close>=0 && b_close<96)
                    b=true;
                
                break;
            }
        }
        
    }
      //  m_actor.push_back(new ZombieCab(this,(double)(rand()%(VIEW_WIDTH+1)),VIEW_HEIGHT));

    
    //===============erase out-of-border objects==================
    for (int i=0; i<m_actor.size();i++) {
        if (m_actor[i]->isAlive()==false) {
            delete m_actor[i];
            m_actor.erase(m_actor.begin()+i);
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_gr;
    
  //  while(!v.empty()) {
   //         delete v.back();
   //         v.pop_back();
   //     }
    
    for (int i=0; i<m_actor.size();i++) {
    //***don't use "delete m_actor.back()" here!!***
    //might get an error "memory being freed is not allocated"
    //relate to the "erase out-of-border objects" function above!
    //not all vector locations store objects!
       // if (m_actor[i]) {
            delete m_actor[i];
       // }
    }
    m_actor.clear();
  //  cout<<m_actor.size()<<endl; //size would turn 0
   // cout<<m_actor.capacity()<<endl; //capacity 256?? leave it???
}

bool StudentWorld::overlap_with_spray(const Actor* a1) const {
    for(int i=0;i<m_actor.size();i++) {
        if (m_actor[i]->getID()==IID_HOLY_WATER_PROJECTILE) {
            Actor* a2=m_actor[i];
            double delta_x=abs(a1->getX() - a2->getX());
            double delta_y=abs(a1->getY() - a2->getY());
            double rad_sum=a1->getRadius()+a2->getRadius();
            if(delta_x<rad_sum*0.25 && delta_y<rad_sum*0.6) {
                m_actor[i]->setAlive(false);
              //  delete m_actor[i];
                return true;
            }
        }
    }
    return false;
}
