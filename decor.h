//#include "helplib.h"

class wall{
    public:
        vec begin, end;
        vec perpendicular(vec dot);
};

class food{
	public:
		double nut_val;
		vec loc;
		food(int, vec);
};

class tree{
    public:
        vec loc;
        int rad;
        int freq;
        int nut_val;
        food* apple();          
};
