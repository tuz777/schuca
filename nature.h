#include <vector>
#include <cstdio>

using namespace std;

class cell{
    public: 
        vector<food*> f;
        vector<creature*> center;
        vector<creature*> per;
        void add(cell, int);
};

class nature{
	public:
        
        void boost();
		void exe();
		int speed;
		
		vector<wall> wcon;
		vector<tree> tcon;
		vector<food*> fcon;
		vector<creature*> ccon;
		
		vector<creature*> tokillc;
		vector<creature*> children;
		vector<food*> tokillf;
		vector<food*> tocrf;
		
		FILE* output;
		void check_bd();
		
        vec resolution;
        int sq;
        void cell_exe(cell);
        void two_cre_interact(creature*, creature*);
        
        void kill(food*);
        void add(food*);
		void kill(creature*);
		void add(creature*);
		void orgall();
		
		int mut_freq;
		nature();
};
