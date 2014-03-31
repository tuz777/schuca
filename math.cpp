#include <cstdlib>
#include <cmath>



#include "math.h"


Vec::Vec(){
    ptr[X] = &x; 
    ptr[Y] = &y;
}

Vec::Vec(double a, double b) : x(a), y(b)
{
    Vec();
}

int random(int m)
{
	return rand() % (m + 1);
}

double smult(Vec a, Vec b)
{
    return a.x*b.x + a.y*b.y;
} 

double vmult(Vec a, Vec b)
{
    return a.x*b.y - a.y*b.x;
}

inline bool sides(Vec a, Vec b, Vec dot)
{
    return smult(a-dot, b-dot) <= 0;
}

inline bool eq_zero(double t)
{
    return abs(t) <= EPS;
}

bool segments_cross(Vec l0b, Vec l0e, Vec l1b, Vec l1e)
{
    Vec a0 = l0b, 
        a1 = l1b, 
        b0 = l0e-l0b, 
        b1 = l1e-l1b;
    
    if(eq_zero(vmult(b1, b0)))
        return (eq_zero(vmult(a1 - a0, b0)) && 
               (
               sides(a0, a0+b0, a1) || 
               sides(a0, a0+b0, a1+b1) ||
               sides(a1, a1+b1, a0) || 
               sides(a1, a1+b1, a0+b0)
               ));
    
    Vec dot = b1*((vmult(b0, a0)+vmult(a1, b0))/vmult(b0, b1)) + a1;
    
    return (eq_zero((dot-(a0+b0)).mod() + (dot - a0).mod() - b0.mod()) && 
           eq_zero((dot-(a1+b1)).mod() + (dot - a1).mod() - b1.mod()));
}

double Vec::mod(){
    return sqrt(x*x + y*y);
}

Vec rand_vec(Vec center, int rad)
{
	Vec l; 
    int sign = 1;
    for(int i = 0; i < Axis_SIZE; i++)
    {
	   if(random(1))
	       sign *= -1;
	   *(l.ptr[i]) = sign * random(rad);
    }
    
	return l + center;
}

Vec rand_vec(Vec border)
{
	Vec l; 
    
    int sign = 1;
    
    for(int i = 0; i < Axis_SIZE; i++)
    {
	   if(random(1))
	       sign *= -1;
	    *(l.ptr[i]) = sign * random((int)*(border.ptr[i]));
    }
    
	return l;
}

Vec Vec::reflect()
{
	return Vec(-x, -y);
}

inline int cl_rn(double t)
{
    return int(t + 0.5);
}

bool Vec::operator==(Vec g)
{
	return cl_rn(x) == cl_rn(g.x) && cl_rn(y) == cl_rn(g.y);
}

Vec Vec::operator+(Vec g)
{
	return Vec(g.x + x, g.y + y);
}

Vec Vec::operator/(double g)
{
	return Vec(x/g, y/g);
}

Vec Vec::operator*(double g)
{
	return Vec(x*g, y*g);
}

Vec Vec::operator-(Vec g)
{
    return Vec(x - g.x, y - g.y);
}
