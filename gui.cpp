#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Button.H>
#include <Fl/gl.H>

#include <cmath>
#include <cstdlib>

#include "math.h"
#include "decor.h"
#include "creature.h"
#include "nature.h"
#include "gui.h"

mywin::mywin(int x, int y, int w, int h, const char *label) : Fl_Gl_Window(x, y, w, h, label)
{
    center.x = 0;
    center.y = 0;
    k = 0.001;
    flag2 = flag = 0;
    able_to_conf = 0;
    
    Bwall = new Fl_Check_Button(int(Fl::w()/50), 200, 50, 20, "wall");
    Btree = new Fl_Check_Button(int(Fl::w()/50) * 4, 200, 50, 20, "tree");
    Bcrea = new Fl_Check_Button(int(Fl::w()/50) * 7, 200, 50, 20, "crea");
    Bwall->type(FL_RADIO_BUTTON);
    Btree->type(FL_RADIO_BUTTON);
    Bcrea->type(FL_RADIO_BUTTON);
    Bcreate = new Fl_Check_Button(int(Fl::w()/50) * 4, 250, 60, 20, "create");
    Bconftree = new Fl_Check_Button(int(Fl::w()/50) * 7, 250, 50, 20, "conf");
    Bwall->callback(but, this); Btree->callback(but, this); Bcrea->callback(but, this); Bcreate->callback(but, this); Bconftree->callback(but, this); 
    Bwall->value(0); Btree->value(0); Bcrea->value(0); Bcreate->value(0); Bconftree->value(0);
    Bwall->show(); Btree->show(); Bcrea->show(); Bcreate->show(); Bconftree->hide();
    
    Itree[0] = new Fl_Int_Input(int(Fl::w()/50) * 2, 300, 50, 30, "freq");
    Itree[1] = new Fl_Int_Input(int(Fl::w()/50) * 6, 300, 50, 30, "nutr");
    Itree[0]->value("100");
    Itree[1]->value("100");
    
    int f = 0, c = 40;
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "a"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "b"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "xleg"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "yleg"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "eyer"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "inte"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "maxa"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "vmod"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "agra"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "mult"));
    Icrea.push_back(new Fl_Int_Input(int(Fl::w()/50) * 2, 300 + c*f++, 40, 30, "type"));
    Icrea[0]->value("15");
    Icrea[1]->value("12");
    Icrea[2]->value("20");
    Icrea[3]->value("10");
    Icrea[4]->value("600");
    Icrea[5]->value("10");
    Icrea[6]->value("100");
    Icrea[7]->value("10");
    Icrea[8]->value("2");
    Icrea[9]->value("4");
    Icrea[10]->value("1");
    
    f = 0;
    for(int i = 0; i < Icrea.size() - 1; i++)
    {
        vector<Fl_Check_Button*> n;
        Ccrea.push_back(n);
        Ccrea[i].push_back(new Fl_Check_Button(int(Fl::w()/50) * 5, 300 + c*f, 20, 20, ""));
        Ccrea[i].push_back(new Fl_Check_Button(int(Fl::w()/50) * 7, 300 + c*f++, 20, 20, ""));
        Ccrea[i][0]->value(0); Ccrea[i][1]->value(0);
    }
    Ccrea[1][0]->value(1);
    Ccrea[1][1]->value(1);

    
    Sconftree[0] = new Fl_Hor_Slider(int(Fl::w()/50), 300, int(Fl::w()*8/50), 30, "freq");
    Sconftree[1] = new Fl_Hor_Slider(int(Fl::w()/50), 350, int(Fl::w()*8/50), 30, "nutr");
    Sconftree[0]->bounds(0.0, 100.0);
    Sconftree[0]->callback(conf_slider0, this);
    Sconftree[1]->bounds(0.0, 1000.0);
    Sconftree[1]->callback(conf_slider1, this);
    
    for(int i = 0; i < 2; i++)
        Sconftree[i]->hide();
    for(int i = 0; i < 2; i++)
        Itree[i]->hide();
    for(int i = 0; i < Icrea.size(); i++)
    {
        Icrea[i]->hide();
        for(int j = 0; j < Ccrea[i].size(); j++)
            Ccrea[i][j]->hide();
    }
}

void conf_slider0(Fl_Widget*b, void* w)
{
    if(((mywin*)w)->able_to_conf)
        ((mywin*)w)->to_conf->freq = int(((Fl_Hor_Slider*)b)->value());
}

void conf_slider1(Fl_Widget*b, void* w)
{
    if(((mywin*)w)->able_to_conf)
        ((mywin*)w)->to_conf->nut_val = int(((Fl_Hor_Slider*)b)->value());
}

void but(Fl_Widget*b, void* w)
{
    if(((mywin*)w)->Btree->value())
    {
        ((mywin*)w)->Bconftree->show();
        
        if(((Fl_Check_Button*)b) == ((mywin*)w)->Btree && ((mywin*)w)->Bcreate->value())
            ((mywin*)w)->Bconftree->value(0);
        
        if(((mywin*)w)->Bcreate->value() && ((Fl_Check_Button*)b) == ((mywin*)w)->Bcreate)
            ((mywin*)w)->Bconftree->value(0);
        else if(!(((Fl_Check_Button*)b) == ((mywin*)w)->Btree))
            ((mywin*)w)->Bcreate->value(0);
    }
    else
        ((mywin*)w)->Bconftree->hide();
        
    if(((mywin*)w)->Btree->value() && ((mywin*)w)->Bcreate->value())
    {
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Sconftree[i]->hide();
        for(int i = 0; i < ((mywin*)w)->Icrea.size(); i++)
        {
            ((mywin*)w)->Icrea[i]->hide();
            for(int j = 0; j < ((mywin*)w)->Ccrea[i].size(); j++)
                ((mywin*)w)->Ccrea[i][j]->hide();
        }
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Itree[i]->show();
    }
    else if(((mywin*)w)->Bcrea->value() && ((mywin*)w)->Bcreate->value())
    {
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Sconftree[i]->hide();
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Itree[i]->hide();
        for(int i = 0; i < ((mywin*)w)->Icrea.size(); i++)
        {
            ((mywin*)w)->Icrea[i]->show();
            for(int j = 0; j < ((mywin*)w)->Ccrea[i].size(); j++)
                ((mywin*)w)->Ccrea[i][j]->show();
        }
    }
    else if(((mywin*)w)->Btree->value() && ((mywin*)w)->Bconftree->value())
    {
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Itree[i]->hide();
        for(int i = 0; i < ((mywin*)w)->Icrea.size(); i++)
        {
            ((mywin*)w)->Icrea[i]->hide();
            for(int j = 0; j < ((mywin*)w)->Ccrea[i].size(); j++)
                ((mywin*)w)->Ccrea[i][j]->hide();
        }
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Sconftree[i]->show();
    }
    else
    {
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Sconftree[i]->hide();
        for(int i = 0; i < 2; i++)
            ((mywin*)w)->Itree[i]->hide();
        for(int i = 0; i < ((mywin*)w)->Icrea.size(); i++)
        {
            ((mywin*)w)->Icrea[i]->hide();
            for(int j = 0; j < ((mywin*)w)->Ccrea[i].size(); j++)
                ((mywin*)w)->Ccrea[i][j]->hide();
        }
    }
}

void mywin::draw_creature(creature* c)
{
	glLoadIdentity();
	
	glScalef(k, k, k);
	
	glTranslatef(c->loc.x + center.x, c->loc.y + center.y, 0.0);
	glRotatef(c->angle, 0.0, 0.0, 1.0);
	
	if(c->gen.type == 0)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 0.0, 1.0);
	
	
    glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0, 0.0);
	for(int i = 0; i <= 20; i++)
	{
		float an = float(i)/10 * M_PI;
		glVertex2f(cos(an)*c->gen.a, sin(an)*c->gen.b);
	}
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2f(-(c->gen.a + c->gen.xleg), 0.0); glVertex2f(c->gen.a + c->gen.xleg, 0.0);
	glVertex2f(0.0, -(c->gen.b + c->gen.yleg)); glVertex2f(0.0, c->gen.b + c->gen.yleg);
	glEnd();
} 

void mywin::draw_food(food* f)
{
    glLoadIdentity();
	
	glScalef(k, k, k);
	
    glTranslatef(f->loc.x + center.x, f->loc.y + center.y, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    double rad = 3.0;
    glVertex2f(rad, rad);
    glVertex2f(rad, -rad);
    glVertex2f(-rad, -rad);
    glVertex2f(-rad, rad);
    glEnd();
}

void mywin::draw_wall(wall w)
{
    glLoadIdentity();
	
	glScalef(k, k, k);
	
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(center.x + w.begin.x, center.y + w.begin.y);
	glVertex2f(center.x + w.end.x, center.y + w.end.y);
	glEnd();
} 

void mywin::draw_line(vec a, vec b)
{
    glLoadIdentity();
	
	glScalef(k, k, k);
	
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(center.x + a.x, center.y + a.y);
	glVertex2f(center.x + b.x, center.y + b.y);
	glEnd();
}

vec mywin::mouse()
{
    vec pos; 
    pos.x = (Fl::event_x() - 0.6*Fl::w())/(Fl::w()*0.4)/k;
    pos.y = -(Fl::event_y() - 0.5*Fl::h())/(Fl::h()*0.5)/k;
    return pos;
}

void mywin::draw()
{
    glClearColor (1.0f, 1.0f, 1.0f, 0.0f);
    glClear (GL_COLOR_BUFFER_BIT);
            
    glMatrixMode(GL_MODELVIEW);
    
    mouse_org();
    
    for(int i = 0; i < world->ccon.size(); i++)
        draw_creature(world->ccon[i]);
    for(int i = 0; i < world->fcon.size(); i++)
        draw_food(world->fcon[i]);
    for(int i = 0; i < world->wcon.size(); i++)
        draw_wall(world->wcon[i]);
        
    /*double biomass = 0;
    for(int i = 0; i < world->fcon.size(); i++)
        biomass += world->fcon[i]->nut_val;
    for(int i = 0; i < world->ccon.size(); i++)
        biomass += world->ccon[i]->health;
    glLoadIdentity();
    glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f((-biomass/2)*0.000001, 0.99);
	glVertex2f((biomass/2)*0.000001, 0.99);
	glEnd();*/

    /*if(world->bmbd.size() > 100)
	{
        vec an, bn; an.x =0; an.y = 0; bn.y = 0; bn.x = world->bmbd.size()*0.0000001;
        draw_line(an, bn);
        for(int i = 0; i < world->bmbd.size(); i += int(double(world->bmbd.size())/100))
	    {
            vec an, bn; an.x = i/(int(double(world->bmbd.size())/100)); an.y = 0; bn.x = an.x; bn.y = world->bmbd[i]*0.000001;
            draw_line(an, bn);
        }
    }*/
    /*vec a, b; a.x = 0; a.y = 0; b.x = world->bmbd.size(); b.y = 0;
    draw_line(a, b);*/
    
	/*vec tmp = world->dl; tmp.y = world->ur.y;
	draw_line(world->dl, tmp);
	tmp = world->dl; tmp.x = world->ur.x;
	draw_line(world->dl, tmp);
	tmp = world->ur; tmp.y = world->dl.y;
	draw_line(world->ur, tmp);
	tmp = world->ur; tmp.x = world->dl.x;
	draw_line(world->ur, tmp);*/
	
	/*for(int i = 0; i < world->resolution.x; i += world->sq)
	{
        vec beg; beg.x = i - world->resolution.x/2; beg.y = world->resolution.x/2;
        vec end; end.x = beg.x; end.y = -beg.y;
        draw_line(beg, end);
        beg.x = world->resolution.x/2; beg.y = i - world->resolution.x/2;
        end.x = -beg.x; end.y = beg.y;
        draw_line(beg, end);
    }*/
}

void mywin::mouse_org()
{
    if(Fl::event_x() >= Fl::w()/5 && Fl::event_state(FL_BUTTON1) && !flag)
    {
        m_pos = mouse();
        flag = 1;
    }
    else if(Fl::event_x() >= Fl::w()/5 && Fl::event_state(FL_BUTTON1) && flag)
    {
        vec curr_m_pos = mouse();
        center = center + (curr_m_pos - m_pos);
        m_pos = curr_m_pos;
    }
    else
        flag = 0;
    //pervaya knopka
    if(Fl::event_state(FL_BUTTON3) && !flag2)
    {
        m_pos2 = mouse() - center;
        if(!(m_pos2 == temp))
            flag2 = 1;
        if(Bcrea->value())
        {
            if(Bcreate->value() && !(temp == m_pos2))
            {
                creature* to_add = new creature;
                for(int i = 0; i < to_add->gen.chrom.size(); i++)
                    *(to_add->gen.chrom[i]) = atoi(Icrea[i]->value());
                for(int i = 0; i < Ccrea.size(); i++)
                {
                    vector<bool> n;
                    to_add->gen.ideal.push_back(n);
                    for(int j = 0; j < Ccrea[i].size(); j++)
                        to_add->gen.ideal[i].push_back(Ccrea[i][j]->value());
                }
                to_add->gen.type = atoi(Icrea[Icrea.size() - 1]->value());
                if(to_add->gen.type != 0.0)
                    to_add->gen.type = 1.0;
                
                to_add->init();
                to_add->health = 3*to_add->S;
                to_add->loc = m_pos2;
                vector<wall> w; vec d = to_add->loc; d.x++;
                to_add->dest_init(d, w);
                
                world->children.push_back(to_add);
                
                /*for(int i = 0; i < to_add->gen.ideal.size(); i++)
                    for(int j = 0; j < to_add->gen.ideal[i].size(); j++)
                        fprintf(world->debug, "%d", int(to_add->gen.ideal[i][j]));*/
                
                temp = m_pos2;
            }
            else if(!Bcreate->value())
            {
                for(int i = 0; i < world->ccon.size(); i++)
                {
                    /*vec a, b; b = a = world->ccon[i]->loc;
                        a.x += 1000; b.x -=1000;
                        draw_line(a, b);*/
                    if(world->ccon[i]->if_dot_belongs_to_ellipse(m_pos2))
                    {
                        for(int l = 0; l < 10000000; l++);
                        world->tokillc.push_back(world->ccon[i]);
                    }
                }
            }
            flag2 = 0;
        }
    }
    else if(Fl::event_state(FL_BUTTON3) && flag2)
    {
        temp = mouse() - center;
        if(!(temp == m_pos2))
        {
            if(Bwall->value() && Bcreate->value())
            {
                wall w; w.begin = m_pos2; w.end = temp;
                world->wcon.push_back(w);
            }
            else if(Bwall->value() && !Bcreate->value())
            {
                for(int i = 4; i < world->wcon.size(); i++)
                    if(lines_intersect(world->wcon[i].begin, world->wcon[i].end, m_pos2, temp))
                        world->wcon.erase(world->wcon.begin() + i);
            }
            else if(Btree->value() && Bcreate->value())
            {
                tree to_add; int it0 = atoi(Itree[0]->value()), it1 = atoi(Itree[1]->value());
                if(it0 > 100)
                    it0 = 100;
                if(it1 > 1000)
                    it1 = 1000;
                to_add.freq = it0;
                to_add.nut_val = it1;
                to_add.loc = m_pos2;
                to_add.rad = int((m_pos2 - temp).module());
                world->tcon.push_back(to_add);
            }
            else if(Btree->value() && Bconftree->value())
            {
                able_to_conf = 0;
                for(int i = 0; i < world->tcon.size(); i++)
                    if((m_pos2 - world->tcon[i].loc).module() < world->tcon[i].rad || (temp - world->tcon[i].loc).module() < world->tcon[i].rad)
                    {
                        able_to_conf = 1;
                        to_conf = &world->tcon[i];
                        break;
                    }
                if(able_to_conf)
                {
                    Sconftree[0]->value(to_conf->freq);
                    Sconftree[1]->value(to_conf->nut_val);
                }
            }
            else if(Btree->value() && !Bcreate->value())
            {
                for(int i = 0; i < world->tcon.size(); i++)
                    if((m_pos2 - world->tcon[i].loc).module() < world->tcon[i].rad || (temp - world->tcon[i].loc).module() < world->tcon[i].rad)
                        world->tcon.erase(world->tcon.begin() + i);
            }
            flag2 = 0;
        }
    }
    else if(flag2)
        draw_line(m_pos2, mouse() - center);
            
    /*if(wheel_state != Fl::event_dy())
    {
        if(k - 0.00001*wheel_state > 0.00001)
            k -= 0.00001*wheel_state;
        wheel_state = Fl::event_dy();
    }*/
}

/*void mywin::eq(mywin* m)
{
    mw_sz = m->mw_sz;
        
    flag = m->flag;
    flag2 = m->flag2; 
    k = m->k; 
    m_pos = m->m_pos;
    m_pos2 = m->m_pos2; 
    temp = m->temp;
    center = m->center;
        
    to_conf = m->to_conf;
    world = m->world;
    Bwall = m->Bwall;
    Btree = m->Btree;
    Bcrea = m->Bcrea;
    Bcreate = m->Bcreate;
    Bconftree = m->Bconftree;
    
    for(int i = 0; i < 2; i++)
    {
        Itree[i] = m->Itree[i];
        Sconftree[i] = m->Sconftree[i];
    }
    for(int i = 0; i < 9; i++)
        Icrea[i] = m->Icrea[i];
}*/

void call(void* p)
{   
    /*if(((mywin*)p)->mainwin->w() != ((mywin*)p)->mw_sz.x || ((mywin*)p)->mainwin->h() != ((mywin*)p)->mw_sz.y)
    {
        //((mywin*)p)->resize(((mywin*)p)->mainwin->w()/5, 0, int(((mywin*)p)->mainwin->w()*0.6), ((mywin*)p)->mainwin->h());
        ((mywin*)p)->mainwin->begin();
        mywin* neu = new mywin(((mywin*)p)->mainwin->w()/5, 0, int(((mywin*)p)->mainwin->w()*0.8), ((mywin*)p)->mainwin->h(), "test");
        ((mywin*)p)->mainwin->end();
        neu->eq((mywin*)p);
        delete (mywin*)p;
        ((mywin*)p)->mw_sz.x = ((mywin*)p)->mainwin->w(); ((mywin*)p)->mw_sz.y = ((mywin*)p)->mainwin->h();
        neu->redraw();
        Fl::repeat_timeout(1.0/100, call, neu);
    }
    else
    {*/
    ((mywin*)p)->redraw();
    Fl::repeat_timeout(1.0/100, call, p);
}  

void zoom_slider(Fl_Widget* s, void* p)
{
    ((mywin*)p)->k = ((Fl_Hor_Slider*)s)->value();
}
void speed_slider(Fl_Widget* s, void* p)
{
    ((nature*)p)->speed = int(((Fl_Hor_Slider*)s)->value());
}
void mut_slider(Fl_Widget* s, void* p)
{
    ((nature*)p)->mut_freq = int(((Fl_Hor_Slider*)s)->value());
}

/*void wbut(Fl_Widget* b, void* p){
    if(((Fl_Check_Button*)b)->value())
    {
        ((mywin*)p)->wb = 1;
        ((mywin*)p)->tb = ((mywin*)p)->cb = 0;
    }
}
void tbut(Fl_Widget* b, void* p){
    if(((Fl_Check_Button*)b)->value())
    {
        ((mywin*)p)->tb = 1;
        ((mywin*)p)->wb = ((mywin*)p)->cb = 0;
    }   
}
void cbut(Fl_Widget* b, void* p){
    if(((Fl_Check_Button*)b)->value())
    {
        ((mywin*)p)->cb = 1;
        ((mywin*)p)->tb = ((mywin*)p)->wb = 0;
    }    
}
void crbut(Fl_Widget* b, void* p){
    ((mywin*)p)->crb = ((Fl_Check_Button*)b)->value();
}*/


int gui(nature* world)
{    
    Fl_Window *window;
    window = new Fl_Window (Fl::w(), Fl::h());
    
    mywin* gl = new mywin(int(Fl::w()/5), 0, int(Fl::w()*0.8), Fl::h(), "test");
    
    gl->world = world;
    /*gl->mainwin = window;
    gl->mw_sz.x = window->w(); gl->mw_sz.y = window->h();*/
    
    Fl_Hor_Slider speed(int(Fl::w()/50), 30, int(Fl::w()*8/50), 30, "speed");
    speed.bounds(100.0, 0.0);
    speed.callback(speed_slider, world);
    speed.value(100.0);
    
    Fl_Hor_Slider zoom(int(Fl::w()/50), 80, int(Fl::w()*8/50), 30, "zoom");
    zoom.bounds(0.0001, 0.005);
    zoom.value(0.001);
    zoom.callback(zoom_slider, gl);
    
    Fl_Hor_Slider mut(int(Fl::w()/50), 130, int(Fl::w()*8/50), 30, "mut_freq");
    mut.bounds(30.0, 0.0);
    mut.value(10.0);
    mut.callback(mut_slider, world);
    
    /*Fl_Int_Input freq(int(Fl::w()/50) * 2, 300, 50, 25, "freq");
    Fl_Int_Input nutr(int(Fl::w()/50) * 6, 300, 50, 25, "nutr");*/
    
    window->fullscreen();
    //window->add_resizable(gl);
    //window->resizable(window);
    //gl->resizable(gl);
    window->show();
    Fl::add_timeout(1.0/25, call, gl);

    return(Fl::run());
}
