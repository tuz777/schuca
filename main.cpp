#include <ctime>
#include <cmath>
#include <process.h>
#include <vector>
#include <iostream>
//#include <allegro.h>
#include <cstdio>

#include "math.h"
#include "decor.h"
#include "creature.h"
#include "nature.h"
#include "gui.h"

using namespace std;

void guithr(void* p)
{
    gui((nature*)p);
    exit(0);
}

int main()
{
	nature* world = new nature;
	srand(time(NULL));
    /*int r = 0;
	creature* test = new creature; test->gen.a = 10; test->gen.b = 5; test->gen.xleg = 5; test->gen.yleg = 0; test->gen.eye_r = 1000; test->gen.intellect = 10; test->gen.max_age = 500;  test->gen.v_mod = 10.0;  test->gen.agra = 2, test->gen.mult = 4; test->gen.type = 1; 
    for(int i = 0; i < 10; i++)
    {
        vector<bool> n;
        test->gen.ideal.push_back(n);
        for(int j = 0; j < 2; j++)
            test->gen.ideal[i].push_back(0);
    }
    test->init();
    test->loc.x = -r; test->loc.y = -r;
    test->health = 10*test->S;
    test->dest = test->loc;
    world->add(test);
    
    creature* test1 = new creature;
    *test1 = *test;
    for(int i = 0; i < 10; i++)
    {
        vector<bool> n;
        test1->gen.ideal.push_back(n);
        for(int j = 0; j < 2; j++)
            test1->gen.ideal[i].push_back(0);
    }
    test1->gen.type = 0;
    world->add(test1);*/
    
    /*test1->gen.type = 0; test1->gen.a = 10; test1->gen.b = 5; test1->gen.xleg = 0; test1->gen.yleg = 0; test1->gen.v_mod = 10.0; test1->gen.intellect = 10; test1->gen.max_age = 52; test1->gen.eye_r = 1000;
    test1->loc.x =-r; test1->loc.y = -r;
    test1->init();
	test1->health = 10*test1->S;
    world->add(test1);
    tree t; t.nut_val = 300; t.loc.x = -r; t.loc.y = -r; t.rad = 1500; t.freq = 100;
    world->tcon.push_back(t);*/
    //t.loc.x = 2000; t.loc.y = 2000; t.freq = 100;
    //world->tcon.push_back(t);
    
    int a = int(world->resolution.x/2);
    wall d; d.begin.x = a; d.end.x = a; d.begin.y = -a; d.end.y = a;//right
    world->wcon.push_back(d);
    d.begin.x = -a; d.end.x = a; d.begin.y = a; d.end.y = a;//down
    world->wcon.push_back(d);
    d.begin.x = -a; d.end.x = -a; d.begin.y = -a; d.end.y = a;//left
    world->wcon.push_back(d);
    d.begin.x = -a; d.end.x = a; d.begin.y = -a; d.end.y = -a;//up
    world->wcon.push_back(d);
    /*d.begin.x = a + 100; d.end.x = -a + 100; d.begin.y = a; d.end.y = -a;//up
    world->wcon.push_back(d);*/
    
    _beginthread(guithr, 1, world);
    
    while(1){
        if(!(world->speed == 100.0))
        {
            for(int i = 0; i < 100; i++)
            {
		        world->exe();
            }
            world->boost();
            if(world->ccon.size() != 0)
                world->check_bd();
        }
    }
	return 0;
}
