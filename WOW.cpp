
//
// Chaser
//
// C++ test
// Copyright® Chaser™
//
// World Of Warcraft.Final Edition

#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
using namespace std;
int creatureassume[5];//iceman dragon lion ninja wolf
int creatureattack[5];//iceman dragon lion ninja wolf
int arrowharm,basehealth,citynumber,lion_loyaty_decrease,MAX_t;
int t;
float sishewuru(float& x){
    if(x*100-(int)(x*100)>0.5)
        x=(int)(x*100+1)/100.0;
    else if(x*100-(int)(x*100)==0.5)
    {
        if((int)(x*100)%2==0)
            x=(int)(x*100)/100.0;
        else
            x=(int)(x*100+1)/100.0;
    }
    else x=(int)(x*100)/100.0;
    return x;
}
class sword
{
public:
    int harm;
    sword(int hurt):harm(hurt/5){};
};
class arrow
{
public:
    int timeleft;
    arrow():timeleft(3){};
};
class bomb
{};
class creature
{
public:
    creature(){};
    ~creature(){};
    string name;
    int label;
    int health;
    int hurt;
    bool side;//false=red,true=blue
    sword* its_sword;
    arrow* its_arrow;
    bomb* its_bomb;
    bool steptime;//iceman
    bool its_attack;//dragon
    bool won;//dragon
    float morale;//dragon
    int loyalty;//lion
    bool arrowshot=false;
    bool tohowl=false;
    bool fighted;
    int formalhealth;
    virtual bool escape(){return false;};//lion
    virtual void lower(){};//dragon
    virtual void ifwon(creature& enemy){};//dragon
    virtual void go(int citydest){
        //000:10 red iceman 1 marched to city 1 with 20 elements and force 30
        cout<<setfill('0')<<setw(3)<<t<<":10 ";
        if(side) cout<<"blue "; else cout<<"red ";
        cout<<name<<' '<<label<<" marched to city "<<citydest<<" with "<<health<<" elements and force "<<hurt<<endl;
    };//iceman
    virtual void rob_weapon(creature& enemy){};//wolf
    virtual void creature_init(bool group,int n){};
    virtual void equip(int number){
        switch (number%3){
            case 0:
                its_sword=new sword(hurt);
                its_bomb=NULL;
                its_arrow=NULL;
                break;
            case 2:
                its_arrow=new arrow;
                its_bomb=NULL;
                its_sword=NULL;
                break;
            case 1:
                its_bomb=new bomb;
                its_sword=NULL;
                its_arrow=NULL;
                break;
        }
    };
    virtual void howl(int citylabel){};
    virtual bool die(int citylabel,creature* enemy){
        if(health<=0)
        {
            if(!arrowshot){
                //report death.
                cout<<setfill('0')<<setw(3)<<t<<":40 ";
                if(side) cout<<"blue ";
                else cout<<"red ";
                cout<<name<<' '<<label<<" was killed in city "<<citylabel+1<<endl;
            }
            if(enemy!=NULL&&enemy->name=="wolf")
                enemy->rob_weapon(*this);
            if(enemy!=NULL&&name=="lion")
            {
                enemy->health+=max(0,formalhealth);
            }
            if(enemy){
                enemy->won=true;
            }
            if(arrowshot)
            this->~creature();
            return true;
        }
        else return false;
    };
    virtual void attack(creature& enemy,int citylabel){
        won=false;
        arrowshot=false;
        fighted=enemy.fighted=true;
        if(health>0&&enemy.health>0){
            if(its_sword!=NULL){
                enemy.health-=((*its_sword).harm+hurt);
                its_sword->harm=its_sword->harm*4/5;
            }
            else enemy.health-=(hurt);
            enemy.arrowshot=false;
            //report fight message.
            //000:40 red iceman 1 attacked blue lion 1 in city 1 with 20 elements and force 30
            cout<<setfill('0')<<setw(3)<<t<<":40 ";
            if(side) cout<<"blue ";
            else cout<<"red ";
            cout<<name<<' '<<label<<" attacked ";
            if(enemy.side) cout<<"blue ";
            else cout<<"red ";
            cout<<enemy.name<<' '<<enemy.label<<" in city "<<citylabel<<" with "<<health<<" elements and force "<<hurt<<endl;
            abandon_sword();
        }
    }
    virtual void fightback(creature& enemy,int citylabel){
        won=false;
        if(health>0){
            if(its_sword!=NULL){
                enemy.health-=(hurt/2+its_sword->harm);
                its_sword->harm=its_sword->harm*4/5;
            }
            else enemy.health-=hurt/2;
            enemy.arrowshot=false;
            //report fightback message.
            //001:40 blue dragon 2 fought back against red lion 2 in city 1
            cout<<setfill('0')<<setw(3)<<t<<":40 ";
            if(side) cout<<"blue ";
            else cout<<"red ";
            cout<<name<<' '<<label<<" fought back against ";
            if(enemy.side)
                cout<<"blue ";
            else cout<<"red ";
            cout<<enemy.name<<' '<<enemy.label<<" in city "<<citylabel<<endl;
            abandon_sword();
        }
    }
    void abandon_sword(){
        if(its_sword&&its_sword->harm==0) {
            delete its_sword;
            its_sword=NULL;
        }
    }
    void arrow_attempt(creature* enemy){
        if(its_arrow!=NULL&&enemy!=NULL)
        {
            if(enemy!=NULL)
            {
                //report arrow usage.
                //000:35 blue dragon 1 shot   //not killed
                //000:35 blue dragon 1 shot and killed red lion 4
                cout<<setfill('0')<<setw(3)<<t<<":35 ";
                if(side) cout<<"blue ";
                else cout<<"red ";
                cout<<name<<' '<<label<<" shot";
                enemy->health-=arrowharm;
                if(enemy->health<=0){
                    enemy->arrowshot=true;
                    cout<<" and killed ";
                    if(enemy->side)
                        cout<<"blue ";
                    else cout<<"red ";
                    cout<<enemy->name<<' '<<enemy->label;
                }
                (its_arrow->timeleft)-=1;
                abandon_arrow();
                cout<<endl;
            }
        }
    }
    void abandon_arrow(){
        if(its_arrow&&its_arrow->timeleft==0) {
            delete its_arrow;
            its_arrow=NULL;
        }
    };
    bool attempt_bomb(creature* enemy,bool fir){
        if(health>0&&enemy!=NULL&&its_bomb!=NULL&&enemy->health>0){
            int t1;
            if(enemy->its_sword==NULL) t1=0;
            else t1=enemy->its_sword->harm;
            int t2;
            if(its_sword==NULL) t2=0;
            else t2=its_sword->harm;
            int hit;
            if((fir&&!side)||(!fir&&side)){
                hit=enemy->hurt;
            }
            else {
                if(enemy->health<=hurt+t2)
                    return false;
                hit=enemy->hurt/2;
            if(enemy->name=="ninja")
                hit=t1=0;
            }
            if(health<=(hit+t1))
            {
                //report bomb usage.
                //000:38 blue dragon 1 used a bomb and killed red lion 7
                cout<<setfill('0')<<setw(3)<<t<<":38 ";
                if(side) cout<<"blue"; else cout<<"red";
                cout<<' '<<name<<' '<<label<<" used a bomb and killed ";
                if(enemy->side) cout<<"blue"; else cout<<"red";
                cout<<' '<<enemy->name<<' '<<enemy->label<<endl;
                enemy->exit();
                exit();
                return true;
            }
            else
                return false;
        }
        else return false;
    }
        void report(){
            //000:55 blue wolf 2 has arrow(2),bomb,sword(23)
            //000:55 blue wolf 4 has no weapon
            //000:55 blue wolf 5 has sword(20)
            cout<<setfill('0')<<setw(3)<<t<<":55 ";
            bool equiped=false;
            if(side) cout<<"blue "; else cout<<"red ";
            cout<<name<<' '<<label<<" has ";
            if(its_arrow==NULL&&its_bomb==NULL&&its_sword==NULL)
                cout<<"no weapon";
            if(its_arrow){
                cout<<"arrow("<<its_arrow->timeleft<<')';
                equiped=true;
            }
            if(its_bomb){
                if(equiped) cout<<',';
                cout<<"bomb";
                equiped=true;
            }
            if(its_sword){
                if(equiped) cout<<',';
                cout<<"sword("<<its_sword->harm<<')';
            }
            cout<<endl;
        }
        void reached(){
            steptime=!steptime;
            if(name=="iceman"&&!steptime){
                health=max(health-9,1);
                hurt+=20;
            }
            cout<<setfill('0')<<setw(3)<<t<<":10 ";
            if(side) cout<<"blue "; else cout<<"red ";
            cout<<name<<' '<<label<<" reached ";
            if(side) cout<<"red "; else cout<<"blue ";
            cout<<"headquarter with "<<health<<" elements and force "<<hurt<<endl;
        }
        void exit(){
            if(its_bomb) its_bomb->~bomb();
            if(its_arrow) its_arrow->~arrow();
            if(its_sword) its_sword->~sword();
            this->~creature();
        }
    };
class iceman:public creature
{
    public:
        void creature_init(bool group,int n){
            health=creatureassume[0];
            hurt=creatureattack[0];
            side=group;
            steptime=false;
            name="iceman";
            label=n;
            equip(n);
        };
        void go(int citydest){
            steptime=!steptime;
            if(steptime==false&&name=="iceman"){
                health=max(health-9,1);
                hurt+=20;
            }
            cout<<setfill('0')<<setw(3)<<t<<":10 ";
            if(side) cout<<"blue "; else cout<<"red ";
            cout<<name<<' '<<label<<" marched to city "<<citydest<<" with "<<health<<" elements and force "<<hurt<<endl;
        }
    };
    class dragon: public creature
    {
    public:
        void creature_init(bool group,int n){
            health=creatureassume[1];
            hurt=creatureattack[1];
            side=group;
            its_attack=false;
            won=false;
            fighted=false;
            name="dragon";
            label=n;
            equip(n);
        };
        void attack(creature& enemy,int citylabel){
            won=false;
            fighted=enemy.fighted=true;
            its_attack=true;
            tohowl=true;
            if(health>0&&enemy.health>0){
                fighted=enemy.fighted=true;
                if(its_sword!=NULL){
                    enemy.health-=((*its_sword).harm+hurt);
                    its_sword->harm=its_sword->harm*4/5;
                }
                else enemy.health-=(hurt);
                enemy.arrowshot=false;
                //report fight message.
                cout<<setfill('0')<<setw(3)<<t<<":40 ";
                if(side) cout<<"blue ";
                else cout<<"red ";
                cout<<name<<' '<<label<<" attacked ";
                if(enemy.side) cout<<"blue ";
                else cout<<"red ";
                cout<<enemy.name<<' '<<enemy.label<< " in city "<<citylabel<<" with "<<health<<" elements and force "<<hurt<<endl;
                abandon_sword();
            }
        }
        void howl(int citylabel){
            if(tohowl&&(name=="dragon")&&morale>=0.8){
                //report howl
                //003:40 blue dragon 2 yelled in city 4
                cout<<setfill('0')<<setw(3)<<t<<":40 ";
                if(side) cout<<"blue"; else cout<<"red";
                cout<<" dragon "<<label<<" yelled in city "<<citylabel+1<<endl;
                tohowl=false;
            }
        }
        void ifwon(creature& enemy){
            if(fighted){
                if(enemy.health<=0){
                    morale+=0.2;
                }
                else
                    morale-=0.2;
            }
        };
    };
    class lion:public creature
    {
    public:
        void creature_init(bool group,int n){
            health=creatureassume[2];
            hurt=creatureattack[2];
            side=group;
            name="lion";
            label=n;
            equip(n);
        }
        void equip(int n){
            its_arrow=NULL;its_bomb=NULL;its_sword=NULL;
        };
        void lower(){
            loyalty-=lion_loyaty_decrease;
        }
        bool escape(){
            if((name=="lion")&&loyalty<=0){
                cout<<setfill('0')<<setw(3)<<t<<":05 ";
                if(side) cout<<"blue ";
                else cout<<"red ";
                cout<<"lion "<<label<<" ran away"<<endl;
                this->~lion();
                return true;
            }
            else return false;
        }
    };
    class ninja:public creature
    {
    public:
        void creature_init(bool group,int n){
            health=creatureassume[3];
            hurt=creatureattack[3];
            side=group;
            name="ninja";
            label=n;
            equip(n);
        }
        void equip(int number){
            switch (number%3){
                case 0:
                    its_sword=new sword(hurt);
                    its_arrow=NULL;
                    its_bomb=new bomb;
                    break;
                case 1:
                    its_arrow=new arrow;
                    its_bomb=new bomb;
                    its_sword=NULL;
                    break;
                case 2:
                    its_bomb=NULL;
                    its_sword=new sword(hurt);
                    its_arrow=new arrow;
                    break;
            }
        }
        void fightback(creature& enemy,int citylabel){};
    };
    class wolf:public creature
    {
    public:
        void creature_init(bool group,int n){
            health=creatureassume[4];
            hurt=creatureattack[4];
            side=group;
            name="wolf";
            label=n;
            equip(n);
        }
        void equip(int number){
            its_sword=NULL;
            its_arrow=NULL;
            its_bomb=NULL;
        };
        void rob_weapon(creature& enemy){
            if(its_bomb==NULL&&enemy.its_bomb)
                its_bomb=new bomb;
            if(its_arrow==NULL&&enemy.its_arrow){
                its_arrow=new arrow;
                its_arrow->timeleft=enemy.its_arrow->timeleft;
            }
            if(its_sword==NULL&&enemy.its_sword)
                its_sword=new sword(enemy.its_sword->harm*5);
        }
    };
    class city
    {
    public:
        int label;
        creature* blue_warrior;
        creature* red_warrior;
        bool flaged=false;
        int lastresult=0;//1 represent blue won, -1 otherwise
        bool flag;//false=red,true=blue
        bool peace;
        int elements=0;
        void init(int number){
            label=number;
            blue_warrior=NULL;
            red_warrior=NULL;
            flaged=false;
            lastresult=0;
        }
        bool who_first(){
            if((flaged&&!flag)||((!flaged)&&(label%2==1)))
                return false;//red
            else return true;//blue
        }
        void fight(int citylabel){
            if(blue_warrior!=NULL&&red_warrior!=NULL)
            {
                red_warrior->formalhealth=red_warrior->health;
                blue_warrior->formalhealth=blue_warrior->health;
                red_warrior->fighted=blue_warrior->fighted=true;
                if(red_warrior->health<=0&&blue_warrior->health>0){
                    blue_warrior->won=true;
                    blue_warrior->fighted=false;
                    if(who_first()) blue_warrior->tohowl=true;
                    else blue_warrior->tohowl=false;
                    return;
                }
                if(blue_warrior->health<=0&&red_warrior->health>0){
                    red_warrior->won=true;
                    red_warrior->fighted=false;
                    if(!who_first()) red_warrior->tohowl=true;
                    else red_warrior->tohowl=false;
                    return;
                }
                else{
                    if((flaged&&(!flag))||((!flaged)&&(label%2==1)))
                    {
                        red_warrior->attack(*blue_warrior,citylabel+1);
                        if(blue_warrior->health>0)
                            blue_warrior->fightback(*red_warrior,citylabel+1);
                        red_warrior->ifwon(*blue_warrior);
                    }
                    else {
                        blue_warrior->attack(*red_warrior,citylabel+1);
                        if(red_warrior->health>0)
                            red_warrior->fightback(*blue_warrior,citylabel+1);
                        blue_warrior->ifwon(*red_warrior);
                    }
                }
            }
        }
        void setflag(){
            if(red_warrior!=NULL&&blue_warrior==NULL)
            {
                if(lastresult==-1)
                {
                    if(!(flaged&&(!flag))){
                    flaged=true;
                    flag=false;
                    //report setflag
                    //004:40 blue flag raised in city 4
                    cout<<setfill('0')<<setw(3)<<t<<":40 red flag raised in city "<<label<<endl;
                    }
                }
                else
                {
                    lastresult=-1;
                }
            }
            if(red_warrior==NULL&&blue_warrior!=NULL)
            {
                if(lastresult==1)
                {
                    if(!(flaged&&flag)){
                    flaged=true;
                    flag=true;
                    //report setflag
                        cout<<setfill('0')<<setw(3)<<t<<":40 blue flag raised in city "<<label<<endl;
                    }
                }
                else
                {
                    lastresult=1;
                }
            }
            if(red_warrior&&blue_warrior) lastresult=0;
        }
        void whosvictory(int& red_total,int& blue_total){//1=blue won,-1=red won
            if(blue_warrior&&!red_warrior)
            {
                //report earning element message.
                //001:40 blue dragon 2 earned 10 elements for his headquarter
                cout<<setfill('0')<<setw(3)<<t<<":30 blue "<<blue_warrior->name<<' '<<blue_warrior->label<<" earned "<<elements<<" elements for his headquarter"<<endl;
                blue_total+=elements;
                elements=0;
                peace=true;
                blue_warrior->its_attack=false;
                blue_warrior->fighted=false;
                blue_warrior->won=false;
            }
            else if(!blue_warrior&&red_warrior)
            {
                //report earning element message.
                cout<<setfill('0')<<setw(3)<<t<<":30 red "<<red_warrior->name<<' '<<red_warrior->label<<" earned "<<elements<<" elements for his headquarter"<<endl;;
                red_total+=elements;
                elements=0;
                peace=true;
                red_warrior->fighted=false;
                red_warrior->won=false;
                red_warrior->its_attack=false;
            }
            if(red_warrior&&blue_warrior) peace=false;
        }
        void battle_result(int& red_total,int& blue_total){
            if(blue_warrior&&!red_warrior)
            {
                //report earning element message.
                //001:40 blue dragon 2 earned 10 elements for his headquarter
                cout<<setfill('0')<<setw(3)<<t<<":40 blue "<<blue_warrior->name<<' '<<blue_warrior->label<<" earned "<<elements<<" elements for his headquarter"<<endl;
                blue_total+=elements;
                elements=0;
                blue_warrior->won=true;
                peace=true;
            }
            else if(!blue_warrior&&red_warrior)
            {
                //report earning element message.
                cout<<setfill('0')<<setw(3)<<t<<":40 red "<<red_warrior->name<<' '<<red_warrior->label<<" earned "<<elements<<" elements for his headquarter"<<endl;;
                red_total+=elements;
                elements=0;
                red_warrior->won=true;
                peace=true;
            }
            if(red_warrior) red_warrior->won=false;
            if(blue_warrior) blue_warrior->won=false;
            if(red_warrior&&blue_warrior) {red_warrior->loyalty-=lion_loyaty_decrease;blue_warrior->loyalty-=lion_loyaty_decrease;}
        }
        void exit(){
            if(red_warrior)
                red_warrior->exit();
            if(blue_warrior)
                blue_warrior->exit();
            this->~city();
        }
    };
    class base
    {
    public:
        bool opponent;// true=blue,false=red
        int totalhealth;
        bool occupied=false;
        bool danger=false;
        int soider_label;
        creature *warrior;
        creature *enemy;
        void init(bool s){
            totalhealth=basehealth;
            opponent=s;
            warrior=enemy=NULL;
            soider_label=1;
            occupied=false;
            danger=false;
        }
        void generate_warrior(){
            //000:00 blue dragon 1 born
            //Its morale is 23.34
            //000:00 blue lion 1 born
            //Its loyalty is 24
            warrior=NULL;
            if(opponent){//lion、dragon、ninja、iceman、wolf
                switch ((soider_label-1)%5) {
                    case 0:
                        if(totalhealth>=creatureassume[2]){
                            warrior=new lion;
                            totalhealth-=creatureassume[2];
                            cout<<setfill('0')<<setw(3)<<t<<":00 blue lion "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 1:
                        if(totalhealth>=creatureassume[1]){
                            warrior=new dragon;
                            totalhealth-=creatureassume[1];
                            cout<<setfill('0')<<setw(3)<<t<<":00 blue dragon "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 2:
                        if(totalhealth>=creatureassume[3]){
                            warrior=new ninja;
                            totalhealth-=creatureassume[3];
                            cout<<setfill('0')<<setw(3)<<t<<":00 blue ninja "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 3:
                        if(totalhealth>=creatureassume[0]){
                            warrior=new iceman;
                            totalhealth-=creatureassume[0];
                            cout<<setfill('0')<<setw(3)<<t<<":00 blue iceman "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 4:
                        if(totalhealth>=creatureassume[4]){
                            warrior=new wolf;
                            totalhealth-=creatureassume[4];
                            cout<<setfill('0')<<setw(3)<<t<<":00 blue wolf "<<soider_label<<" born"<<endl;
                        }
                        break;
                }
            }
            else {//iceman、lion、wolf、ninja、dragon
                switch ((soider_label-1)%5) {
                    case 0:
                        if(totalhealth>=creatureassume[0]){
                            warrior=new iceman;
                            totalhealth-=creatureassume[0];
                            cout<<setfill('0')<<setw(3)<<t<<":00 red iceman "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 1:
                        if(totalhealth>=creatureassume[2]){
                            warrior=new lion;
                            totalhealth-=creatureassume[2];
                            cout<<setfill('0')<<setw(3)<<t<<":00 red lion "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 2:
                        if(totalhealth>=creatureassume[4]){
                            warrior=new wolf;
                            totalhealth-=creatureassume[4];
                            cout<<setfill('0')<<setw(3)<<t<<":00 red wolf "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 3:
                        if(totalhealth>=creatureassume[3]){
                            warrior=new ninja;
                            totalhealth-=creatureassume[3];
                            cout<<setfill('0')<<setw(3)<<t<<":00 red ninja "<<soider_label<<" born"<<endl;
                        }
                        break;
                    case 4:
                        if(totalhealth>=creatureassume[1]){
                            warrior=new dragon;
                            totalhealth-=creatureassume[1];
                            cout<<setfill('0')<<setw(3)<<t<<":00 red dragon "<<soider_label<<" born"<<endl;
                        }
                        break;
                }
            }
            if(warrior!=NULL){
                warrior->creature_init(opponent,soider_label);
                warrior->loyalty=totalhealth;
                warrior->morale=(float)totalhealth/(float)creatureassume[1];
                if(warrior->name=="lion")
                    cout<<"Its loyalty is "<<warrior->loyalty<<endl;
                if(warrior->name=="dragon"){
                    warrior->morale=sishewuru(warrior->morale);
                    cout<<"Its morale is "<<fixed<<setprecision(2)<<warrior->morale<<endl;
                }
                ++soider_label;
            }
        }
        void exit(){
            if(warrior) warrior->exit();
            if(enemy) enemy->exit();
            this->~base();
        }
    };
    int main(){
        int x;
        cin>>x;
        for(int z=1;z<=x;++z){
            cout<<"Case "<<z<<':'<<endl;
            cin>>basehealth>>citynumber>>arrowharm>>lion_loyaty_decrease>>MAX_t;
            cin>>creatureassume[1]>>creatureassume[3]>>creatureassume[0]>>creatureassume[2]>>creatureassume[4];//dragon,ninja,iceman,lion,wolf
            cin>>creatureattack[1]>>creatureattack[3]>>creatureattack[0]>>creatureattack[2]>>creatureattack[4];
            base *red_base=new base;
            red_base->init(false);
            base *blue_base=new base;
            blue_base->init(true);
            city *Cityline=new city[citynumber];
            for(int i=0;i<citynumber;++i)
                Cityline[i].init(i+1);
            t=0;
            for(int ts=0;ts<=MAX_t;)
            {
                //武士降生（：00）-lion逃跑（：05）-武士前进（：10）-武士放箭（：35）-武士使用bomb（：38）-进攻、反击、战死、欢呼、获取生命元、升旗（：40）-武士抵达司令部、占领司令部（：10）-司令部报告（：50）-武士报告（：55）
                //arrow射死的武士不会报告死亡
                //:00
                //new warrior
                //
                if(ts>MAX_t)
                    break;
                red_base->generate_warrior();
                blue_base->generate_warrior();
                //:05
                //lion escape
                //
                ts+=5;
                if(ts>MAX_t)
                    break;
                if(red_base->warrior&&red_base->warrior->escape())
                    red_base->warrior=NULL;
                for(int j=0;j<citynumber;++j)
                {
                    if(Cityline[j].blue_warrior&&Cityline[j].blue_warrior->escape())
                        Cityline[j].blue_warrior=NULL;
                    if(Cityline[j].red_warrior&&Cityline[j].red_warrior->escape())
                        Cityline[j].red_warrior=NULL;
                }
                if(blue_base->warrior&&blue_base->warrior->escape())
                    blue_base->warrior=NULL;
                //:10
                //base taken
                //
                ts+=5;
                if(ts>MAX_t)
                    break;
                if(red_base->enemy&&Cityline[0].blue_warrior)
                    red_base->occupied=true;
                if(blue_base->enemy&&Cityline[citynumber-1].red_warrior)
                    blue_base->occupied=true;
                if(Cityline[0].blue_warrior){
                    red_base->enemy=Cityline[0].blue_warrior;
                }
                if(Cityline[citynumber-1].red_warrior){
                    blue_base->enemy=Cityline[citynumber-1].red_warrior;
                }
                for(int j=0;j<citynumber-1;++j){
                    Cityline[j].blue_warrior=Cityline[j+1].blue_warrior;
                    Cityline[citynumber-1-j].red_warrior=Cityline[citynumber-2-j].red_warrior;
                }
                Cityline[0].red_warrior=red_base->warrior;
                Cityline[citynumber-1].blue_warrior=blue_base->warrior;
                if(red_base->enemy&&!red_base->danger){red_base->enemy->reached();red_base->danger=true;}
                if(red_base->occupied)
                {
                    red_base->enemy->reached();
                    cout<<setfill('0')<<setw(3)<<t<<":10 red headquarter was taken"<<endl;
                }
                for(int j=0;j<citynumber;++j)
                {
                    if(Cityline[j].red_warrior)
                        Cityline[j].red_warrior->go(j+1);
                    if(Cityline[j].blue_warrior)
                        Cityline[j].blue_warrior->go(j+1);
                }
                if(blue_base->enemy&&!blue_base->danger) {blue_base->enemy->reached();blue_base->danger=true;};
                if(blue_base->occupied)
                {
                    blue_base->enemy->reached();
                    cout<<setfill('0')<<setw(3)<<t<<":10 blue headquarter was taken"<<endl;
                }
                if(red_base->occupied||blue_base->occupied)
                    break;
                ts+=20;
                if(ts>MAX_t)
                    break;
                //:30
                //victory judge
                //
                for(int j=0;j<citynumber;++j)
                {
                    Cityline[j].peace=false;
                    Cityline[j].elements+=10;
                    Cityline[j].whosvictory(red_base->totalhealth, blue_base->totalhealth);
                }
                ts+=5;
                if(ts>MAX_t)
                    break;
                //:35
                //try arrow
                //
                for(int j=0;j<citynumber;++j)
                {
                    if(Cityline[j].red_warrior&&j<citynumber-1)
                        Cityline[j].red_warrior->arrow_attempt(Cityline[j+1].blue_warrior);
                    if(Cityline[j].blue_warrior&&j>0)
                        Cityline[j].blue_warrior->arrow_attempt(Cityline[j-1].red_warrior);
                }
                ts+=3;
                if(ts>MAX_t)
                    break;
                //:38
                //try bomb
                //
                for(int j=0;j<citynumber;++j)
                {
                    if(Cityline[j].red_warrior&&Cityline[j].red_warrior->attempt_bomb(Cityline[j].blue_warrior,Cityline[j].who_first())){
                        Cityline[j].red_warrior=NULL;
                        Cityline[j].blue_warrior=NULL;
                    }
                    if(Cityline[j].blue_warrior&&Cityline[j].blue_warrior->attempt_bomb(Cityline[j].red_warrior,Cityline[j].who_first()))
                    {
                        Cityline[j].red_warrior=NULL;
                        Cityline[j].blue_warrior=NULL;
                    };
                }
                ts+=2;
                if(ts>MAX_t)
                    break;
                //:40
                //fight
                //
                for(int j=0;j<citynumber;++j){
                    Cityline[j].fight(j);
                    //:40
                    //warrior death
                    //
                    if(Cityline[j].red_warrior&&Cityline[j].red_warrior->die(j,Cityline[j].blue_warrior)) Cityline[j].red_warrior=NULL;
                    if(Cityline[j].blue_warrior&&Cityline[j].blue_warrior->die(j,Cityline[j].red_warrior)) Cityline[j].blue_warrior=NULL;
                    //:40
                    //dragon howl and reward winners
                    //
                    if(Cityline[j].red_warrior)
                        Cityline[j].red_warrior->howl(j);
                    if(Cityline[j].blue_warrior)
                        Cityline[j].blue_warrior->howl(j);
                    if(Cityline[j].blue_warrior&&!Cityline[j].red_warrior&&Cityline[j].blue_warrior->won)
                    {
                        if(blue_base->totalhealth>=8)
                        {
                            blue_base->totalhealth-=8;
                            Cityline[j].blue_warrior->health+=8;
                        }
                    }
                    if(!Cityline[j].blue_warrior&&Cityline[j].red_warrior&&Cityline[j].red_warrior->won)
                    {
                        if(red_base->totalhealth>=8)
                        {
                            red_base->totalhealth-=8;
                            Cityline[j].red_warrior->health+=8;
                        }
                    }
                    if(!Cityline[j].peace){
                        Cityline[j].battle_result(red_base->totalhealth, blue_base->totalhealth);
                        Cityline[j].setflag();
                    }
                }
                ts+=10;
                if(ts>MAX_t)
                    break;
                //:50
                //base report
                cout<<setfill('0')<<setw(3)<<t<<":50 "<<red_base->totalhealth<<" elements in red headquarter"<<endl;
                cout<<setfill('0')<<setw(3)<<t<<":50 "<<blue_base->totalhealth<<" elements in blue headquarter"<<endl;
                ts+=5;
                if(ts>MAX_t)
                    break;
                //:55
                //warrior report
                //
                for(int j=0;j<citynumber;++j)
                {
                    if(Cityline[j].red_warrior){
                        Cityline[j].red_warrior->report();
                    }
                }
                if(blue_base->enemy) blue_base->enemy->report();
                if(red_base->enemy) red_base->enemy->report();
                for(int j=0;j<citynumber;++j){
                    if(Cityline[j].blue_warrior)
                        Cityline[j].blue_warrior->report();
                }
                ts+=5;
                ++t;
            }
            red_base->exit();
            blue_base->exit();
            for(int j=0;j<citynumber;++j)
                Cityline[j].exit();
        }
        return 0;
    }
    /*
     Test Data:
     27
     200 6 12 24 733
     21 23 65 22 19
     17 23 14 33 67
     20 3 50 30 1000
     100 100 100 100 100
     200 200 200 200 200
     130 10 2 20 1000
     40 20 30 40 10
     40 50 20 10 30
     100 2 20 100 5000
     10 10 10 10 10
     5 5 5 5 5
     1000 2 10 2100 5000
     100 100 100 100 100
     5 5 5 5 5
     1000 20 25 100 998
     90 20 30 100 20
     20 50 20 100 30
     1000 20 25 50 3000
     90 20 30 10 20
     20 50 20 40 30
     1000 20 50 80 5000
     190 20 30 110 20
     20 50 20 60 30
     1000 20 50 30 5000
     80 30 30 110 20
     100 20 40 80 40
     1000 20 25 50 3000
     90 20 30 50 20
     100 30 10 100 80
     1000 20 50 30 5000
     80 30 30 210 20
     150 20 140 180 40
     1000 1 25 20 1000
     90 20 30 10 20
     20 50 20 40 30
     1000 12 100 30 1000
     190 20 30 110 20
     20 50 20 60 30
     1000 20 25 100 4659
     90 20 30 100 20
     20 50 20 100 30
     100 10 50 30 5000
     190 20 30 110 20
     20 50 20 110 30
     1000 20 25 100 998
     80 20 30 100 20
     200 100 200 300 130
     500 20 2 3 998
     50 60 45 55 35
     1 3 2 4 3
     500 20 2 3 998
     1 3 2 4 3
     50 60 45 55 35
     0 20 25 100 998
     80 20 30 100 20
     200 100 200 300 130
     1000 20 55 10 4659
     90 20 30 10 20
     20 50 20 100 30
     100 15 20 30 3000
     100 100 100 100 10
     10 100 10 100 1
     10 1 1 1 100
     1 1 1 1 1
     1 1 1 1 1
     1000 20 40 58 1000
     12 23 12 23 63
     45 64 23 12 34
     300 13 45 23 1234
     12 53 23 6 5
     23 54 23 53 18
     300 20 10 10 5000
     200 200 200 200 200
     1 2 3 4 5
     500 20 2 3 998
     50 60 45 55 35
     1 3 2 4 3
     1000 20 2 3 5000
     100 150 120 99 5
     1 3 2 4 3
     */
