#include "math.h"
#include "decor.h"
#include "creature.h"
#include "nature.h"

#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdio>

void nature::check_bd()
{
    double cbiomass = 0, fbiomass = 0;
    for(int i = 0; i < fcon.size(); i++)
        fbiomass += fcon[i]->nut_val;
    for(int i = 0; i < ccon.size(); i++)
        cbiomass += ccon[i]->health;
    genotype to_add; 
    for(int i = 0; i < to_add.chrom.size(); i++)
        *(to_add.chrom[i]) = 0.0;
    to_add.type = 0.0;
    if(ccon.size() != 0)
    {
        for(int i = 0; i < ccon.size(); i++)
            to_add = to_add + ccon[i]->gen;
        to_add = to_add/ccon.size();
    }
    fprintf(output, "%f %f %f %f %f %f %f %f %f %f %f %f %f %d ", fbiomass, cbiomass, to_add.a, to_add.b, to_add.xleg, to_add.yleg, to_add.eye_r, to_add.max_age, to_add.intellect, to_add.v_mod, to_add.agra, to_add.mult, to_add.type, ccon.size());
}
            
void cell::add(cell g, int h)
{//h = 1 then per, 0 then center, 2 then only food
    for(int i = 0; i < g.f.size(); i++)
        f.push_back(g.f[i]);
    if(h == 0)
        for(int i = 0; i < g.center.size(); i++)
            center.push_back(g.center[i]);
    else if(h == 1)
        for(int i = 0; i < g.center.size(); i++)
            per.push_back(g.center[i]);
}

nature::nature()
{
	mut_freq = 10;
	speed = 100;
	resolution.x = 10000; 
	resolution.y = 10000;
	sq = 50;
	output = fopen("output.txt", "w");
	//debug = fopen("debug.txt", "w");
	/*for(int i = 0; i < 100; i++)
	   bmbd.push_back(1000.0);*/
}

void nature::boost()
{   
    int l[3];
        
    int sq_con[3];
    sq_con[0] = sq;
        
    int a = clock();
    exe();
    l[0] = clock() - a;
        
    for(int i = sq_con[0] + 1; i <= int(resolution.x); i++)
        if(int(resolution.x) % i == 0)
        {
            sq_con[1] = sq = i;
            break;
        }
        
    a = clock();
    exe();
    l[1] = clock() - a;
        
    for(int i = sq_con[0] - 1; i >= 1; i--)
        if(int(resolution.x) % i == 0)
        {
            sq_con[2] = sq = i;
            break;
        }
    a = clock();
    exe();
    l[2] = clock() - a;
        
    int min_val = l[0], min_num = 0;
    for(int i = 0; i < 3; i++)
        if(l[i] < min_val)
        {
            min_val = l[i];
            min_num = i;
        }
            
    sq = sq_con[min_num];
}

void nature::two_cre_interact(creature* a, creature* b)
{
    if((a->loc - b->loc).module() <= a->gen.xleg + b->gen.xleg + a->gen.a + b->gen.a)
	{
        if(a->gen.type != b->gen.type && a->health > a->gen.mult*a->S && b->health > b->gen.mult*b->S/* && a->loc == b->loc*/)
		{
            /*if(a->gen.type == 0)
                if(!(a->gen > b->gen))
                    return;
            else
                if(!(b->gen > a->gen))
                    return;*/
     		add(create(a, b, mut_freq));
			return;
		}
		else
		{
			if(a->if_hit(b) && a->health < a->gen.agra*a->S && b->health > b->S)
				b->health -= a->S/b->S * b->S;
			if(b->if_hit(a) && b->health < b->gen.agra*b->S && a->health > a->S)
				a->health -= b->S/a->S * a->S;
		}
	}
}

void nature::cell_exe(cell a)
{
    for(int i = 0; i < a.center.size(); i++)
    {
        for(int j = i + 1; j < a.center.size(); j++)
            two_cre_interact(a.center[i], a.center[j]);
        for(int j = 0; j < a.per.size(); j++)
            two_cre_interact(a.center[i], a.per[j]);
		
		for(int j = 0; j < a.f.size(); j++)
            if(a.center[i]->if_dot_belongs_to_ellipse(a.f[j]->loc))
            {
                bool d = 0;
                for(int l = 0; l < tokillf.size(); l++)
                    if(a.f[j] == tokillf[l])
                    {
                        d = 1; break;
                    }
                if(d)
                    continue;
                a.center[i]->health += a.f[j]->nut_val;
				kill(a.f[j]);
                //break;
            }
    }
}
            
void nature::kill(food* f)
{
    tokillf.push_back(f);
}

void nature::add(food* f)
{
    tocrf.push_back(f);
}

void nature::kill(creature* kal)
{
    if(kal->health > 0)
    {
        food* corpse = new food(int(kal->health), kal->loc);
        add(corpse);
    }
	tokillc.push_back(kal);
}

void nature::add(creature* ch)
{
	children.push_back(ch);
}

void nature::orgall()
{
    for(int i = 0; i < tokillf.size(); i++)
        for(int j = 0; ; j++)
        {
            if(tokillf[i] == fcon[j])
            {
                fcon.erase(fcon.begin() + j);
                delete tokillf[i];
                break;
            }
        }
        
    for(int i = 0; i < tocrf.size(); i++)
        fcon.push_back(tocrf[i]);

    tokillf.clear();
    tocrf.clear();
    
	for(int i = 0; i < tokillc.size(); i++)
        for(int j = 0; j < ccon.size(); j++)
			if(ccon[j] == tokillc[i])
			{
			    ccon.erase(ccon.begin() + j);
			    delete tokillc[i];
			    break;
			}

	for(int i = 0; i < children.size(); i++)
	    ccon.push_back(children[i]);
	children.clear();
	tokillc.clear();
}

void nature::exe()
{  
    Sleep(speed);
         
	for(int i = 0; i < ccon.size(); i++)
	    ccon[i]->exe(ccon, fcon, wcon);
	    
	for(int i = 0; i < tcon.size(); i++)
        if(random(5) == 0)
        {
            for(int l = 0; l < tcon[i].freq; l++)
            {
                food* f = tcon[i].apple();
                if(abs(int(f->loc.x)) <= resolution.x/2 && abs(int(f->loc.y)) <= resolution.y/2)
                    add(f);
            }
        }//execute all objects
    
    for(int i = 0; i < ccon.size(); i++)
	{
        for(int l = 0; l < wcon.size(); l++) 
            if(ccon[i]->creature_in_wall(wcon[l]))
            {
                ccon[i]->loc = ccon[i]->pre_loc;
                ccon[i]->health = ccon[i]->S;
            }
                
        if(ccon[i]->age >= ccon[i]->gen.max_age || ccon[i]->health <= ccon[i]->S)
			kill(ccon[i]);
    }
    
	orgall();//check if some of them are dead

    cell** field = new cell*[int(resolution.x/sq) + 2];
    for(int i = 0; i < int(resolution.x/sq) + 2; i++)
        field[i] = new cell[int(resolution.y/sq) + 2];
    
    vec dl, ur;
    dl.x = resolution.x/2; dl.y = resolution.y/2; ur.x = -resolution.x/2; ur.y = -resolution.y/2;
    for(int i = 0; i < ccon.size(); i++)
    {
        if(ccon[i]->loc.x > ur.x)
            ur.x = ccon[i]->loc.x;
        if(ccon[i]->loc.y > ur.y)
            ur.y = ccon[i]->loc.y;
        if(ccon[i]->loc.x < dl.x)
            dl.x = ccon[i]->loc.x;
        if(ccon[i]->loc.y < dl.y)
            dl.y = ccon[i]->loc.y;
        field[int((ccon[i]->loc.x + resolution.x/2)/sq) + 1][int((ccon[i]->loc.y + resolution.y/2)/sq) + 1].center.push_back(ccon[i]);
    }//na 1 vpered vpravo idet smesheniye chtoby ne vylazit' za ramki
    for(int i = 0; i < fcon.size(); i++)
        field[int((fcon[i]->loc.x + resolution.x/2)/sq) + 1][int((fcon[i]->loc.y + resolution.y/2)/sq) + 1].f.push_back(fcon[i]);
    
    for(int i = int((dl.x + resolution.x/2)/sq) + 1; i <= int((ur.x + resolution.x/2)/sq) + 1; i++)
        for(int j = int((dl.y + resolution.y/2)/sq) + 1; j <= int((ur.y + resolution.y/2)/sq) + 1; j++)
        {
            if(field[i][j].center.size() != 0)
            {
                cell to_exe;
                to_exe.add(field[i][j], 0);
                to_exe.add(field[i + 1][j], 1);
                to_exe.add(field[i - 1][j + 1], 1);
                to_exe.add(field[i][j + 1], 1);
                to_exe.add(field[i + 1][j + 1], 1);
                to_exe.add(field[i - 1][j], 2);
                to_exe.add(field[i - 1][j - 1], 2);
                to_exe.add(field[i][j - 1], 2);
                to_exe.add(field[i + 1][j - 1], 2);
                cell_exe(to_exe);
            }
        }
        
    for(int i = 0; i < int(resolution.x/sq) + 2; i++)
        delete [] field[i];
    delete [] field; //check the dependancies between alive
    
    //orgall();
}
