#include <vector>
#include <cstdio>

using namespace std;

enum Params{W, H, HLEG, VLEG, VIEW_R, INTELLECT, LIFE_LENGTH, SPEED, HUNTL, BREEDL, SEX,    Params_SIZE};  

class Genotype{
        vector<double> genes;
        vector< vector<bool> > imp_params;
        bool operator>(Genotype) const;
        public:
            double get_G(int) const;
            bool get_IP(int, int) const;
};

class creature{
        Genotype gen;
        vec leg[4], center[2], dest;
        double l, S, angle;
	    double health, age, pepyaka;
	    vec loc, pre_loc, v;
	void init();
	bool dest_init(vec, vector<wall>);
	void exe(const vector<creature*>&, const vector<food*>&, const vector<wall>&);
	vector<creature*> isee(const vector<creature*>&);
	vector<food*> isee(const vector<food*>&);
    bool if_hit(creature*);
    void think(const vector<creature*>&, const vector<food*>&, const vector<wall>&);
	bool if_dot_belongs_to_ellipse(vec);
	bool creature_in_wall(wall w);
};

creature* create(creature*, creature*, int mfr); 
        
class A{
vector< vector<int> > f_;
A() : f(f_)
{
f_.resize(W, vector<int>(H));
}
public:
const vector< vector<int> >& f;
};
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
