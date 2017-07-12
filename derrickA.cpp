//created by: Derrick Alden
//Load level works with the Level class ID
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cstring>
#include "kyleS.h"
#include "derrickA.h"
#include "fonts.h"
#include <fcntl.h>

//game box array to spawn sealed room
void setFrame(Game *g) 
{

		//top frame box
		g->box[0].width = g->xres;
		g->box[0].height = 15;
		g->box[0].center.x = g->xres;
		g->box[0].center.y = g->yres;

		//bottom frame box
		g->box[1].width = g->xres;
		g->box[1].height = 15;
		g->box[1].center.x = 0;
		g->box[1].center.y = 0;

		//left frame box
		g->box[2].width = 15;
		g->box[2].height = g->yres;
		g->box[2].center.x = 0;
		g->box[2].center.y = 0;

		//right frame box
		g->box[3].width = 15;
		g->box[3].height = g->yres;
		g->box[3].center.x = g->xres;
		g->box[3].center.y = 0;		

}


//game box array to spawn room with right door
void setLeftDoor(Game *g)
{
		//top frame box
		g->box[0].width = g->xres;
		g->box[0].height = 15;
		g->box[0].center.x = g->xres;
		g->box[0].center.y = g->yres;

		//bottom frame box
		g->box[1].width = g->xres;
		g->box[1].height = 15;
		g->box[1].center.x = 0;
		g->box[1].center.y = 0;

		//left frame box
		g->box[2].width = 15;
		g->box[2].height = g->yres - 150;
		g->box[2].center.x = 0;
		g->box[2].center.y = g->yres;

		//right frame box
		g->box[3].width = 15;
		g->box[3].height = g->yres;
		g->box[3].center.x = g->xres;
		g->box[3].center.y = 0;		

}

//game box array to spawn room with left door
void setRightDoor(Game *g)
{

		//top frame box
		g->box[0].width = g->xres;
		g->box[0].height = 15;
		g->box[0].center.x = g->xres;
		g->box[0].center.y = g->yres;

		//bottom frame box
		g->box[1].width = g->xres;
		g->box[1].height = 15;
		g->box[1].center.x = 0;
		g->box[1].center.y = 0;

		//left frame box
		g->box[2].width = 15;
		g->box[2].height = g->yres;
		g->box[2].center.x = 0;
		g->box[2].center.y = 0;

		//right frame box
		g->box[3].width = 15;
		g->box[3].height = g->yres - 150;
		g->box[3].center.x = g->xres;
		g->box[3].center.y = g->yres;		

}

//game box array to spawn room with left door
void setLRDoor(Game *g) 
{

		//top frame box
		g->box[0].width = g->xres;
		g->box[0].height = 15;
		g->box[0].center.x = g->xres;
		g->box[0].center.y = g->yres;

		//bottom frame box
		g->box[1].width = g->xres;
		g->box[1].height = 15;
		g->box[1].center.x = 0;
		g->box[1].center.y = 0;

		//left frame box
		g->box[2].width = 15;
		g->box[2].height = g->yres - 150;
		g->box[2].center.x = 0;
		g->box[2].center.y = g->yres;

		//right frame box
		g->box[3].width = 15;
		g->box[3].height = g->yres - 150;
		g->box[3].center.x = g->xres;
		g->box[3].center.y = g->yres;		

}

//used in games render function to draw the boxes on the screen
void levelDrawBox(Game *g) 
{
	float w, h;
	Shape *s;
	glColor3ub(90,140,90);
	for (int i = 0; i < 5; i++) {
		s = &g->box[i];
		glPushMatrix();
		glTranslatef(s->center.x, s->center.y, s->center.z);
		w = s->width;
		h = s->height;
		glBegin(GL_QUADS);
			glVertex2i(-w,-h);
			glVertex2i(-w, h);
			glVertex2i( w, h);
			glVertex2i( w,-h);
		glEnd();
		glPopMatrix();
	}
}

void loadLevel(Game *g, Level *lev) 
{
	Character *p;
	p = &g->character;

	if (p->s.center.x < 0.1) {
		p->s.center.x = WINDOW_WIDTH - 20;
		lev->levelID--;
		if(lev->levelID == 1) {
		setRightDoor(g);
		}
	}
	if (p->s.center.x > WINDOW_WIDTH - 0.1) {
		p->s.center.x = 20;
		lev->levelID++;
		if(lev->levelID > 1) {
		setLRDoor(g);
		}
	}
	else {
//	setFrame(g);
	}

}

void levelText(Game *gm, Level *lev)
{
        Flt h, w;
        Rect r;
        int c = 0xffffffff;
if (gm->state == STATE_GAMEPLAY) {
		h = 25.0;
		w = 200.0;
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0, 0.0, 1.0, 0.9);
		glTranslated(gm->xres/2, gm->yres/2, 0);
		glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, +h);
			glVertex2i(+w, +h);
			glVertex2i(+w, -h);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		r.bot = gm->yres - 25;
		r.left = gm->xres/2;
		r.center = gm->yres;
		ggprint8b(&r, 16, c, "LEVEL: %i ", lev->levelID);
	}
        
}

//sets the level ID back to 1 on game over. Switch to checkpoint in the future.
void gameOverLevelRestart(Game *gm, Level *lev)
{
	if (gm->state == STATE_GAMEOVER) {
		lev->levelID = 1;
	//	clearLevel(gm);
		setRightDoor(gm);      
	}
}

void setLevel2(Game *gm, Level *lev)
{
	if (lev->levelID == 2) {
        //test platforms
        gm->plat[0].width = 50;
        gm->plat[0].height = 15;
        gm->plat[0].center.x = 300;
        gm->plat[0].center.y = 80;

        //test platforms
        gm->plat[1].width = 50;
        gm->plat[1].height = 15;
        gm->plat[1].center.x = 500;
        gm->plat[1].center.y = 100;

        //test platforms
        gm->plat[2].width = 50;
        gm->plat[2].height = 15;
        gm->plat[2].center.x = 700;
        gm->plat[2].center.y = 250;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = 800;
        gm->plat[3].center.y = 400;

        //test spikes
        gm->spike[0].width = 25;
        gm->spike[0].height = 15;
        gm->spike[0].center.x = 1000;
        gm->spike[0].center.y = 60;

	//wall blocking door	
	gm->box[4].width = 15;
	gm->box[4].height = gm->yres - 325;
	gm->box[4].center.x = gm->xres - 150;
	gm->box[4].center.y = 0;		

	}


}



void drawLevel2(Game *gm, Level *lev)
{
	float w, h;
	if (lev->levelID == 2) {

        //Draw test platform
	Shape *test;
	glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
		
	Shape *spike;
	glColor3ub(80,110,70);
	spike = &gm->spike[0];
	glPushMatrix();
	glTranslatef(spike->center.x, spike->center.y, spike->center.z);
	w = spike->width;
	h = spike->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();


	//Draw test platform 3
	Shape *test3;
	glColor3ub(80,110,70);
	test3 = &gm->plat[2];
	glPushMatrix();
	glTranslatef(test3->center.x, test3->center.y, test3->center.z);
	w = test3->width;
	h = test3->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 4
	Shape *test4;
	glColor3ub(80,110,70);
	test4 = &gm->plat[3];
	glPushMatrix();
	glTranslatef(test4->center.x, test4->center.y, test4->center.z);
	w = test4->width;
	h = test4->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	}
}

void drawLevel3(Game *gm, Level *lev)
{
	float w, h;
	if (lev->levelID == 3) {

        //Draw test platform
	Shape *test;
	glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	
	//set up spike	
	Shape *spike;
	glColor3ub(80,110,70);
	spike = &gm->spike[0];
	glPushMatrix();
	glTranslatef(spike->center.x, spike->center.y, spike->center.z);
	w = spike->width;
	h = spike->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();


	//Draw test platform 3
	Shape *test3;
	glColor3ub(80,110,70);
	test3 = &gm->plat[2];
	glPushMatrix();
	glTranslatef(test3->center.x, test3->center.y, test3->center.z);
	w = test3->width;
	h = test3->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 4
	Shape *test4;
	glColor3ub(80,110,70);
	test4 = &gm->plat[3];
	glPushMatrix();
	glTranslatef(test4->center.x, test4->center.y, test4->center.z);
	w = test4->width;
	h = test4->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	}



}


void setLevel3(Game *gm, Level *lev)
{
	if (lev->levelID == 3) {
        //test platforms
        gm->plat[0].width = 50;
        gm->plat[0].height = 15;
        gm->plat[0].center.x = 300;
        gm->plat[0].center.y = 80;

        //test platforms
        gm->plat[1].width = 50;
        gm->plat[1].height = 15;
        gm->plat[1].center.x = 500;
        gm->plat[1].center.y = 100;

        //test platforms
        gm->plat[2].width = 50;
        gm->plat[2].height = 15;
        gm->plat[2].center.x = 700;
        gm->plat[2].center.y = 120;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = 900;
        gm->plat[3].center.y = 140;

        //test spikes
        gm->spike[0].width = 25;
        gm->spike[0].height = 15;
        gm->spike[0].center.x = 1000;
        gm->spike[0].center.y = 60;

	//wall blocking door	
	gm->box[4].width = 15;
	gm->box[4].height = gm->yres - 600;
	gm->box[4].center.x = gm->xres - 150;
	gm->box[4].center.y = 0;		

	}


}

void clearLevel(Game *gm) 
{
	float w,h;

	for (int i = 0; i < 9; i++) {
	gm->plat[i].width = 0;
        gm->plat[i].height = 0;
        gm->plat[i].center.x = 0;
        gm->plat[i].center.y = 0;
	}


        //test spikes
        gm->spike[0].width = 0;
        gm->spike[0].height = 0;
        gm->spike[0].center.x = 0;
        gm->spike[0].center.y = 0;

        //wall blocking door    
        gm->box[4].width = 0;
        gm->box[4].height = 0;
        gm->box[4].center.x = 0;
        gm->box[4].center.y = 0;


        //Draw test platform
	Shape *test;
	glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	
	//set up spike	
	Shape *spike;
	glColor3ub(80,110,70);
	spike = &gm->spike[0];
	glPushMatrix();
	glTranslatef(spike->center.x, spike->center.y, spike->center.z);
	w = spike->width;
	h = spike->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();


	//Draw test platform 3
	Shape *test3;
	glColor3ub(80,110,70);
	test3 = &gm->plat[2];
	glPushMatrix();
	glTranslatef(test3->center.x, test3->center.y, test3->center.z);
	w = test3->width;
	h = test3->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 4
	Shape *test4;
	glColor3ub(80,110,70);
	test4 = &gm->plat[3];
	glPushMatrix();
	glTranslatef(test4->center.x, test4->center.y, test4->center.z);
	w = test4->width;
	h = test4->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();


	




}

void setLevelSwitch(Game *gm, Level *lev)
{
    switch (lev->levelID) 
    {
	case 1:
		clearLevel(gm);
		//setLevel1(&gm, &lev);
		break;
	case 2:
		clearLevel(gm);
		setLevel2(gm, lev);
		drawLevel2(gm, lev);
		break;
	case 3:
		clearLevel(gm);
		setLevel3(gm, lev);
		drawLevel3(gm, lev);
		break;
	case 4:
		clearLevel(gm);
		break;
	default:
		clearLevel(gm);
		break;
	
	

    }


}



