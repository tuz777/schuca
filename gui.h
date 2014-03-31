#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Hor_Slider.H>

class mywin : public Fl_Gl_Window{
    public: 
        /*Fl_Window* mainwin;
        vec mw_sz;*/
        
        bool flag, flag2; double k; 
        vec m_pos, m_pos2, temp, center;
        
        tree* to_conf;
        nature* world;
        bool able_to_conf;
        
        Fl_Check_Button* Bwall;
        Fl_Check_Button* Btree;
        Fl_Check_Button* Bcrea;
        Fl_Check_Button* Bcreate;
        Fl_Check_Button* Bconftree;
        
        Fl_Int_Input* Itree[2];
        vector<Fl_Int_Input*> Icrea;
        vector< vector<Fl_Check_Button*> > Ccrea;
        
        Fl_Hor_Slider* Sconftree[2];
        
        mywin(int, int, int, int, const char*);
        
        void mouse_org();
        void draw_creature(creature*);
        void draw_food(food*);
        void draw_wall(wall);
        void draw_line(vec, vec);
        vec mouse();
        
        void draw();
        
        //void eq(mywin*);
}; 

void call(void* p);
//void mut_slider(void* p);
void speed_slider(Fl_Widget*, void*);
void zoom_slider(Fl_Widget*, void*);
void mut_slider(Fl_Widget*, void*);
void conf_slider0(Fl_Widget*, void*);
void conf_slider1(Fl_Widget*, void*);

void but(Fl_Widget*, void*);

/*void wbut(Fl_Widget*, void*);
void tbut(Fl_Widget*, void*);
void cbut(Fl_Widget*, void*);
void crbut(Fl_Widget*, void*);*/

int gui(nature* world);
