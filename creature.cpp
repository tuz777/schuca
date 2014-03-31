#include <cstdlib>
#include <vector>
#include <cmath>

#include "math.h"
#include "decor.h"
#include "creature.h"

using namespace std;

bool Genotype::operator>(Genotype snd) const
{
    double imp_prms_num = 0, better = 0;
    for(int i = 0; i < Params_SIZE; i++)
        if(get_IP(i, 0))
        {
            imp_prms_num++;
            switch(get_IP(i, 1)){
                case 1:
                    better += get_G(i) > snd.get_G(i);
                    break;
                case 0:
                    better += get_G(i) <= snd.get_G(i);
            }
        }
    return better/imp_prms_num >= 0.5;
}   

double Genotype::get_G(int i) const
{
    return genes[i];
}

bool Genotype::get_IP(int i, int j) const
{
    return imp_params[i][j];
}

bool creature::creature_in_wall(wall w)
{
    if(pre_loc == loc)
        return false;
    return lines_intersect(w.begin, w.end, loc, pre_loc);
}

void creature::exe(const vector<creature*>& nature_creature_data, const vector<food*>& nature_food_data, const vector<wall>& nature_wall_data)
{
    pre_loc = loc;
    
    if(pepyaka >= 100)
        think(nature_creature_data, nature_food_data, nature_wall_data);
     
    if((loc - dest).module() > (loc + v - dest).module())
        loc = loc + v;
    else
        loc = dest;
	pepyaka += gen.get_G(INTELLECT);
	age += 0.1;
	health -= 0.001 * S;
}
//vec gl;
bool creature::dest_init(vec d, vector<wall> walls)
{
	for(int i = 0; i < walls.size(); i++)
	{
        if((walls[i].perpendicular(loc) - loc).module() <= gen.get_G(VIEW_R))
            if(lines_intersect(loc, d, walls[i].begin, walls[i].end))
                return false;
    }
    dest = d;
    
    if(dest == loc)
        return true;
        	
	v = (dest - loc)*(gen.get_G(SPEED)/(dest - loc).module());
	vec tmp; tmp.y = loc.y; tmp.x = (v + loc).x;
    
    double D = 1.0;
    if(sign(v.x) != sign(v.y))
	   D = -1.0;
	
    angle = D*acos((tmp - loc).module()/v.module())/M_PI * 180;

    double lar, sma;
    if(gen.get_G(W) > gen.get_G(H))
    {
        lar = gen.get_G(W); sma = gen.get_G(H);
    }
    else
    {
        lar = gen.get_G(H); sma = gen.get_G(W);
    }
        
	center[0] = v*(sqrt(lar*lar - sma*sma)/v.module());
	center[1] = center[0].reflect();

	tmp = v;
	leg[0] = tmp*((gen.get_G(W) + gen.get_G(HLEG))/tmp.module());
	double p = tmp.x; tmp.x = tmp.y; tmp.y = p;
	leg[1] = tmp*((gen.get_G(H) + gen.get_G(VLEG))/tmp.module());
	leg[2] = leg[0].reflect();
	leg[3] = leg[1].reflect();
	return true;
}

void creature::init()
{
    //BASIC_STATE
    age = 0; pepyaka = 100;
    //LEGS
	l = 2*max(gen.get_G(W), gen.get_G(H));
	S = gen.get_G(W)*gen.get_G(H)*gen.get_G(SPEED);
}

creature* create(creature* a, creature* b, int mut_freq)
{
	vector<double> mut;
	for(int i = 0; i < Params_SIZE; i++)
	{
		if(random(mut_freq) == 0)
		{
            if(random(1))
			    mut.push_back(-random(5));
			else
			    mut.push_back(random(5));
        }
		else
		    mut.push_back(0);
	}

	creature* result = new creature;
	for(int i = 0; i < result->gen.chrom.size() - 2; i++)
	   *(result->gen.chrom[i]) = abs((*(a->gen.chrom[i]) + *(b->gen.chrom[i]))/2 + mut[i]);
	for(int i = result->gen.chrom.size() - 2; i < result->gen.chrom.size(); i++)
	   *(result->gen.chrom[i]) = abs((*(a->gen.chrom[i]) + *(b->gen.chrom[i]))/2 + mut[i]/10);
	   
	if(result->gen.a < 5)
	   result->gen.a = 5;
	if(result->gen.b < 5)
	   result->gen.b = 5;
	if(result->gen.v_mod < 5)
	   result->gen.v_mod = 5;
	   
	result->gen.type = double(random(1));
	
    creature* same_type;
    if(a->gen.type == result->gen.type)
        same_type = a;
    else
        same_type = b;
    for(int i = 0; i < same_type->gen.ideal.size(); i++)
    {
        vector<bool> n;
        result->gen.ideal.push_back(n);
        for(int j = 0; j < same_type->gen.ideal[i].size(); j++)
            result->gen.ideal[i].push_back(same_type->gen.ideal[i][j]);
    }      
    if(random(mut_freq) == 0)
    {
        int num;
        num = random(result->gen.ideal.size() - 1);
        for(int i = 0; i < result->gen.ideal[num].size(); i++)
            result->gen.ideal[num][i] = bool(random(1));
    }
        
    
    result->init();
    result->health = (a->health + b->health)/3;
    a->health = result->health; b->health = result->health;
    result->loc = (a->loc + b->loc)/2;
    
    vector<wall> w; vec d = result->loc; d.x++;
    result->dest_init(d, w);
	//mogut vozniknut' oshibki s initom pre_loc
	return result;
}


void creature::think(const vector<creature*>& nature_creature_data, const vector<food*>& nature_food_data, const vector<wall>& nature_wall_data)
{
	pepyaka = 0;
    vector<creature*> g1 = isee(nature_creature_data);
	vector<creature*> g2;
    for(int i = 0; i < g1.size(); i++)
    {
        if(gen.type == g1[i]->gen.type && !(gen > g1[i]->gen) && (g1[i]->loc - loc).module() < 5*gen.v_mod)
        {
            if(dest_init(((g1[i]->loc - loc).reflect() + loc), nature_wall_data))
                return;
        }
		else
			g2.push_back(g1[i]);
	}
	if(health > gen.mult*S)
	{
    	for(int i = 0; i < g2.size(); i++)
        	{
                if(g2[i]->gen.type != gen.type)
                {
                    if(gen.type == 0)
                    {
                        if(gen > g2[i]->gen && (g2[i]->loc - loc).module() < 5*gen.v_mod)
                            if(dest_init(((g2[i]->loc - loc).reflect() + loc), nature_wall_data))
                                return;
                    }
                    else
                    {
                        if(!(gen > g2[i]->gen))
                            if(dest_init(g2[i]->loc, nature_wall_data))
                                return;
                    }
                }
            }
	}
	else if(health < gen.agra*S)
	{
        for(int i = 0; i < g2.size(); i++)
            if(g2[i]->gen.type == gen.type && gen > g2[i]->gen)
		        if(dest_init(g2[0]->loc - leg[0], nature_wall_data))
			        return;
	}
	
	vector<food*> f = isee(nature_food_data);
	if(f.size() != 0)
    {
		if(dest_init(f[f.size() - 1]->loc, nature_wall_data))
		    return;
    }  
        
    if(dest_init(rand_vec(50) + loc, nature_wall_data))
        return;
    dest_init(loc, nature_wall_data);
}

bool creature::if_dot_belongs_to_ellipse(vec dot)
{
	if(l >= (dot - (loc + center[0])).module() + (dot - (loc + center[1])).module())
		return true;
	return false;
}

bool creature::if_hit(creature* f)
{
    for(int i = 0; i < 4; i++)
        if(f->if_dot_belongs_to_ellipse(leg[i] + loc))
            return true;
    /*if(f->if_dot_belongs_to_ellipse(loc))
        return true;*/
    return false;
}

vector<creature*> creature::isee(const vector<creature*>& s)
{
	vector<creature*> result;
    for(int i = 0; i < s.size(); i++)
    {
        if((s[i]->loc - loc).module() == 0)
            continue;
        if((s[i]->loc - loc).module() <= gen.eye_r)
            result.push_back(s[i]);
    }
	return result;
}

vector<food*> creature::isee(const vector<food*>& f)
{
	vector<food*> result;
	for(int i = 0; i < f.size(); i++)
	    if((loc - f[i]->loc).module() <= gen.eye_r)
	        result.push_back(f[i]);
	if(result.size() != 0)
	{
        food* nearest;
	    nearest = result[0];
        for(int i = 1; i < result.size(); i++)
        {
            if((nearest->loc - loc).module() > (result[i]->loc - loc).module())
                nearest = result[i];
        }
        result.push_back(nearest);
    }
	return result;
}
