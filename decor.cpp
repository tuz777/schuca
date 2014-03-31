#include "math.h"
#include "decor.h"

vec wall::perpendicular(vec dot)
{
	vec ret;
	double a = (begin - dot).module(), b = (end - begin).module(), c = (end - dot).module(), T;
	T = -(c*c - a*a - b*b)/(2*b);
	ret = ((end - begin) * (T/b)) + begin;
	return ret;
}
food::food(int nut, vec l)
{
	loc = l;
    nut_val = nut;
}

food* tree::apple()
{
    food* neu = new food(nut_val, rand_vec(rad, rad) + loc);
    return neu;
}
