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

extern void prepPlat(Game *g);
extern void prepBox(Game *g);
extern void prepSpike(Game *g);
extern void setLevel7(Game *gm, Level *lev);
extern void drawLevel7(Game *gm, Level *lev);

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
	//glColor3ub(90,140,90);
	for (int i = 0; i < 6; i++) {
		s = &g->box[i];
		glPushMatrix();
		glTranslatef(s->center.x, s->center.y, s->center.z);
		w = s->width;
		h = s->height;
		prepBox(g);
		glBegin(GL_QUADS);
			glTexCoord2f(g->tex.xB[0], g->tex.yB[0]);
				glVertex2i(-w,-h);
			glTexCoord2f(g->tex.xB[0], g->tex.yB[1]);
				glVertex2i(-w, h);
			glTexCoord2f(g->tex.xB[1], g->tex.yB[1]);
				glVertex2i( w, h);
			glTexCoord2f(g->tex.xB[1], g->tex.yB[0]);
				glVertex2i( w,-h);
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
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
	else if (lev->levelID > 1) {
		setLRDoor(g);
	}
	else if (lev->levelID == 1) {
		setRightDoor(g);
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
		glColor4f(0.0, 0.0, 0.0, 0.0);
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
        gm->plat[2].center.y = 120;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = 900;
        gm->plat[3].center.y = 140;

        //test spikes
      //  gm->spike[0].width = 25;
      //  gm->spike[0].height = 15;
    //    gm->spike[0].center.x = 1000;
  //      gm->spike[0].center.y = 60;

	//wall blocking door	
	gm->box[4].width = 15;
	gm->box[4].height = gm->yres - 700;
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
	//glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	
	/*	
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
	*/

        //Draw test platform 2
	Shape *test2;
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);


	//Draw test platform 3
	Shape *test3;
	test3 = &gm->plat[2];
	glPushMatrix();
	glTranslatef(test3->center.x, test3->center.y, test3->center.z);
	w = test3->width;
	h = test3->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//Draw test platform 4
	Shape *test4;
	test4 = &gm->plat[3];
	glPushMatrix();
	glTranslatef(test4->center.x, test4->center.y, test4->center.z);
	w = test4->width;
	h = test4->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
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
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	
	//set up spike	
	Shape *spike;
	glColor3ub(80,110,70);
	spike = &gm->spike[0];
	glPushMatrix();
	glTranslatef(spike->center.x, spike->center.y, spike->center.z);
	w = spike->width;
	h = spike->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);


	//Draw test platform 3
	Shape *test3;
	glColor3ub(80,110,70);
	test3 = &gm->plat[2];
	glPushMatrix();
	glTranslatef(test3->center.x, test3->center.y, test3->center.z);
	w = test3->width;
	h = test3->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//Draw test platform 4
	Shape *test4;
	glColor3ub(80,110,70);
	test4 = &gm->plat[3];
	glPushMatrix();
	glTranslatef(test4->center.x, test4->center.y, test4->center.z);
	w = test4->width;
	h = test4->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
    
	//Draw test platform 5
	Shape *test5;
	glColor3ub(80,110,70);
	test5 = &gm->plat[4];
	glPushMatrix();
	glTranslatef(test5->center.x, test5->center.y, test5->center.z);
	w = test5->width;
	h = test5->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	}



}


void setLevel3(Game *gm, Level *lev)
{
	if (lev->levelID == 3) {
        //test platforms
        gm->plat[0].width = 50;
        gm->plat[0].height = 15;
        gm->plat[0].center.x = 200;
        gm->plat[0].center.y = 70;

        //test platforms
        gm->plat[1].width = 50;
        gm->plat[1].height = 15;
        gm->plat[1].center.x = 50;
        gm->plat[1].center.y = 140;

        //test platforms
        gm->plat[2].width = 50;
        gm->plat[2].height = 15;
        gm->plat[2].center.x = 200;
        gm->plat[2].center.y = 210;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = gm->xres/2 - 80;
        gm->plat[3].center.y = 200;

        //test platforms
        gm->plat[4].width = 50;
        gm->plat[4].height = 15;
        gm->plat[4].center.x = gm->xres/2 + 160;
        gm->plat[4].center.y = 200;

        //test spikes
        gm->spike[0].width = 350;
        gm->spike[0].height = 15;
        gm->spike[0].center.x = gm->xres - 600;
        gm->spike[0].center.y = 110;

	//wall blocking right door	
	gm->box[4].width = 15;
	gm->box[4].height = gm->yres - 800;
	gm->box[4].center.x = gm->xres - 150;
	gm->box[4].center.y = 0;

	//wall blocking left door	
	gm->box[5].width = 15;
	gm->box[5].height = gm->yres - 600;
	gm->box[5].center.x = 250;
	gm->box[5].center.y = 0;		

	}


}

void drawLevel4(Game *gm, Level *lev)
{

	
	float w, h;
	if (lev->levelID == 4) {

        //Draw test platform
	Shape *test;
	glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//set up spike	
	Shape *spike;
	glColor3ub(80,110,70);
	spike = &gm->spike[0];
	glPushMatrix();
	glTranslatef(spike->center.x, spike->center.y, spike->center.z);
	w = spike->width;
	h = spike->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	

	//set up spike	2
	Shape *spike2;
	glColor3ub(80,110,70);
	spike2 = &gm->spike[1];
	glPushMatrix();
	glTranslatef(spike2->center.x, spike2->center.y, spike2->center.z);
	w = spike2->width;
	h = spike2->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//set up spike	3
	Shape *spike3;
	glColor3ub(80,110,70);
	spike3 = &gm->spike[2];
	glPushMatrix();
	glTranslatef(spike3->center.x, spike3->center.y, spike3->center.z);
	w = spike3->width;
	h = spike3->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
		prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);


	//Draw test platform 3
	Shape *test3;
	glColor3ub(80,110,70);
	test3 = &gm->plat[2];
	glPushMatrix();
	glTranslatef(test3->center.x, test3->center.y, test3->center.z);
	w = test3->width;
	h = test3->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);	
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//Draw test platform 4
	Shape *test4;
	glColor3ub(80,110,70);
	test4 = &gm->plat[3];
	glPushMatrix();
	glTranslatef(test4->center.x, test4->center.y, test4->center.z);
	w = test4->width;
	h = test4->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);		
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	
	//Draw test platform 5
	Shape *test5;
	glColor3ub(80,110,70);
	test5 = &gm->plat[4];
	glPushMatrix();
	glTranslatef(test5->center.x, test5->center.y, test5->center.z);
	w = test5->width;
	h = test5->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);	
			glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);


        //Draw test platform 6
	Shape *test6;
	glColor3ub(80,110,70);
	test6 = &gm->plat[5];
	glPushMatrix();
	glTranslatef(test6->center.x, test6->center.y, test6->center.z);
	w = test6->width;
	h = test6->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);	
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



        //Draw test platform 7
	Shape *test7;
	glColor3ub(80,110,70);
	test7 = &gm->plat[6];
	glPushMatrix();
	glTranslatef(test7->center.x, test7->center.y, test7->center.z);
	w = test2->width;
	h = test2->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);	
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);


	//Draw test platform 8
	Shape *test8;
	glColor3ub(80,110,70);
	test8 = &gm->plat[7];
	glPushMatrix();
	glTranslatef(test8->center.x, test8->center.y, test8->center.z);
	w = test8->width;
	h = test8->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);	
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//Draw test platform 9
	Shape *test9;
	glColor3ub(80,110,70);
	test9 = &gm->plat[8];
	glPushMatrix();
	glTranslatef(test9->center.x, test9->center.y, test9->center.z);
	w = test9->width;
	h = test9->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	
	//Draw test platform 10
	Shape *test10;
	glColor3ub(80,110,70);
	test10 = &gm->plat[9];
	glPushMatrix();
	glTranslatef(test10->center.x, test10->center.y, test10->center.z);
	w = test10->width;
	h = test10->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);	
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	}

	



}


void setLevel4(Game *gm, Level *lev)
{
	if (lev->levelID == 4) {
        //test platforms
        gm->plat[0].width = 50;
        gm->plat[0].height = 15;
        gm->plat[0].center.x = 200;
        gm->plat[0].center.y = 70;

        //test platforms
        gm->plat[1].width = 50;
        gm->plat[1].height = 15;
        gm->plat[1].center.x = 50;
        gm->plat[1].center.y = 140;

        //test platforms
        gm->plat[2].width = 50;
        gm->plat[2].height = 15;
        gm->plat[2].center.x = 200;
        gm->plat[2].center.y = 210;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = 50;
        gm->plat[3].center.y = 280;

        //test platforms
        gm->plat[4].width = 50;
        gm->plat[4].height = 15;
        gm->plat[4].center.x = 200;
        gm->plat[4].center.y = 350;

        //test platforms
        gm->plat[5].width = 50;
        gm->plat[5].height = 15;
        gm->plat[5].center.x = 50;
        gm->plat[5].center.y = 420;

        //test platforms
        gm->plat[6].width = 50;
        gm->plat[6].height = 15;
        gm->plat[6].center.x = 200;
        gm->plat[6].center.y = 490;

        //test platforms
        gm->plat[7].width = 50;
        gm->plat[7].height = 15;
        gm->plat[7].center.x = 50;
        gm->plat[7].center.y = 560;

        //test platforms
        gm->plat[8].width = 50;
        gm->plat[8].height = 15;
        gm->plat[8].center.x = 200;
        gm->plat[8].center.y = 220;

        //test platforms
        gm->plat[9].width = 50;
        gm->plat[9].height = 15;
        gm->plat[9].center.x = 550;
        gm->plat[9].center.y = 480;

        //test spikes
        gm->spike[0].width = 100;
        gm->spike[0].height = 15;
        gm->spike[0].center.x = 415;
        gm->spike[0].center.y = 520;

        //test spikes 2
        gm->spike[1].width = 200;
        gm->spike[1].height = 15;
        gm->spike[1].center.x = 600;
        gm->spike[1].center.y = 400;

        //test spikes 3
        gm->spike[2].width = 350;
        gm->spike[2].height = 15;
        gm->spike[2].center.x = gm->xres -600;
        gm->spike[2].center.y = 110;

	//wall blocking right door	
	gm->box[4].width = 15;
	gm->box[4].height = gm->yres - 800;
	gm->box[4].center.x = gm->xres - 150;
	gm->box[4].center.y = 0;

	//wall blocking left door	
	gm->box[5].width = 15;
	gm->box[5].height = gm->yres - 300;
	gm->box[5].center.x = 250;
	gm->box[5].center.y = 0;		

	}
}


void drawLevel5(Game *gm, Level *lev)
{

	
	float w, h;
	if (lev->levelID == 5) {

        //Draw test platform
	Shape *test;
	glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
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
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	
	//set up spike	2
	Shape *spike2;
	glColor3ub(80,110,70);
	spike2 = &gm->spike[1];
	glPushMatrix();
	glTranslatef(spike2->center.x, spike2->center.y, spike2->center.z);
	w = spike2->width;
	h = spike2->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//set up spike	3
	Shape *spike3;
	glColor3ub(80,110,70);
	spike3 = &gm->spike[2];
	glPushMatrix();
	glTranslatef(spike3->center.x, spike3->center.y, spike3->center.z);
	w = spike3->width;
	h = spike3->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
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
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
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
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();
	
	//Draw test platform 5
	Shape *test5;
	glColor3ub(80,110,70);
	test5 = &gm->plat[4];
	glPushMatrix();
	glTranslatef(test5->center.x, test5->center.y, test5->center.z);
	w = test5->width;
	h = test5->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();


        //Draw test platform 6
	Shape *test6;
	glColor3ub(80,110,70);
	test6 = &gm->plat[5];
	glPushMatrix();
	glTranslatef(test6->center.x, test6->center.y, test6->center.z);
	w = test6->width;
	h = test6->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();



        //Draw test platform 7
	Shape *test7;
	glColor3ub(80,110,70);
	test7 = &gm->plat[6];
	glPushMatrix();
	glTranslatef(test7->center.x, test7->center.y, test7->center.z);
	w = test2->width;
	h = test2->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();


	//Draw test platform 8
	Shape *test8;
	glColor3ub(80,110,70);
	test8 = &gm->plat[7];
	glPushMatrix();
	glTranslatef(test8->center.x, test8->center.y, test8->center.z);
	w = test8->width;
	h = test8->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

	//Draw test platform 9
	Shape *test9;
	glColor3ub(80,110,70);
	test9 = &gm->plat[8];
	glPushMatrix();
	glTranslatef(test9->center.x, test9->center.y, test9->center.z);
	w = test9->width;
	h = test9->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();
	
	//Draw test platform 10
	Shape *test10;
	glColor3ub(80,110,70);
	test10 = &gm->plat[9];
	glPushMatrix();
	glTranslatef(test10->center.x, test10->center.y, test10->center.z);
	w = test10->width;
	h = test10->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

	}

	



}


void setLevel5(Game *gm, Level *lev)
{
	if (lev->levelID == 5) {
        //test platforms
        gm->plat[0].width = 50;
        gm->plat[0].height = 15;
        gm->plat[0].center.x = 200;
        gm->plat[0].center.y = 70;

        //test platforms
        gm->plat[1].width = 50;
        gm->plat[1].height = 15;
        gm->plat[1].center.x = 50;
        gm->plat[1].center.y = 140;

        //test platforms
        gm->plat[2].width = 50;
        gm->plat[2].height = 15;
        gm->plat[2].center.x = 200;
        gm->plat[2].center.y = 210;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = 500;
        gm->plat[3].center.y = 180;

        //test platforms
        gm->plat[4].width = 50;
        gm->plat[4].height = 15;
        gm->plat[4].center.x = 800;
        gm->plat[4].center.y = 160;

        //test platform above rdoor
        gm->plat[5].width = 50;
        gm->plat[5].height = 15;
        gm->plat[5].center.x = gm->xres - 50;
        gm->plat[5].center.y = 240;

        //test platforms above traps
        gm->plat[6].width = 50;
        gm->plat[6].height = 15;
        gm->plat[6].center.x = 1000;
        gm->plat[6].center.y = 550;

        //test platforms
        gm->plat[7].width = 50;
        gm->plat[7].height = 15;
        gm->plat[7].center.x = 900;
        gm->plat[7].center.y = 400;

        //test platforms wide
        gm->plat[8].width = 400;
        gm->plat[8].height = 15;
        gm->plat[8].center.x = 200;
        gm->plat[8].center.y = 650;

        //test platforms
        gm->plat[9].width = 50;
        gm->plat[9].height = 15;
        gm->plat[9].center.x = 750;
        gm->plat[9].center.y = 680;

        //test spikes top
        gm->spike[0].width = 400;
        gm->spike[0].height = 15;
        gm->spike[0].center.x = 415;
        gm->spike[0].center.y = 520;

        //test spikes 2 mid
        gm->spike[1].width = 300;
        gm->spike[1].height = 15;
        gm->spike[1].center.x = 700;
        gm->spike[1].center.y = 350;

        //test spikes 3 bottom
        gm->spike[2].width = 350;
        gm->spike[2].height = 15;
        gm->spike[2].center.x = gm->xres -600;
        gm->spike[2].center.y = 110;

	//wall blocking right door	
	gm->box[4].width = 15;
	gm->box[4].height = gm->yres - 800;
	gm->box[4].center.x = gm->xres - 150;
	gm->box[4].center.y = 0;

	//wall blocking left door	
	gm->box[5].width = 15;
	gm->box[5].height = 280;
	gm->box[5].center.x = 250;
	gm->box[5].center.y = 0;		

	}
}

void setLevel6(Game *gm, Level *lev)
{
	if (lev->levelID == 6) {
        //test platforms
        gm->plat[0].width = 50;
        gm->plat[0].height = 15;
        gm->plat[0].center.x = 200;
        gm->plat[0].center.y = 70;

        //test platforms
        gm->plat[1].width = 50;
        gm->plat[1].height = 15;
        gm->plat[1].center.x = 50;
        gm->plat[1].center.y = 140;

        //test platforms
        gm->plat[2].width = 50;
        gm->plat[2].height = 15;
        gm->plat[2].center.x = 200;
        gm->plat[2].center.y = 210;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = 200;
        gm->plat[3].center.y = 290;

        //test platforms
        gm->plat[4].width = 50;
        gm->plat[4].height = 15;
        gm->plat[4].center.x = 200;
        gm->plat[4].center.y = 370;

        //test platforms
        gm->plat[5].width = 50;
        gm->plat[5].height = 15;
        gm->plat[5].center.x = 200;
        gm->plat[5].center.y = 450;

        //test platforms
        gm->plat[6].width = 50;
        gm->plat[6].height = 15;
        gm->plat[6].center.x = 200;
        gm->plat[6].center.y = 530;

        //test platforms
        gm->plat[7].width = 50;
        gm->plat[7].height = 15;
        gm->plat[7].center.x = gm->xres - 300;
        gm->plat[7].center.y = 500;
	
        //test platforms long
        gm->plat[8].width = 400;
        gm->plat[8].height = 15;
        gm->plat[8].center.x = 1200;
        gm->plat[8].center.y = 250;

        //test platforms
        gm->plat[9].width = 50;
        gm->plat[9].height = 15;
        gm->plat[9].center.x = 800;
        gm->plat[9].center.y = 150;

        //test platforms
        gm->plat[10].width = 50;
        gm->plat[10].height = 15;
        gm->plat[10].center.x = gm->xres - 300;
        gm->plat[10].center.y = 400;

        //test platforms long
        gm->plat[11].width = 200;
        gm->plat[11].height = 15;
        gm->plat[11].center.x = gm->xres - 300;
        gm->plat[11].center.y = 730;

        //test platforms
        gm->plat[12].width = 50;
        gm->plat[12].height = 15;
        gm->plat[12].center.x = gm->xres - 300;
        gm->plat[12].center.y = 600;

        //test spikes
        gm->spike[0].width = 350;
        gm->spike[0].height = 15;
        gm->spike[0].center.x = gm->xres - 600;
        gm->spike[0].center.y = 110;

	//wall top of stair	
	//gm->box[4].width = 350;
	//gm->box[4].height = 15;
	//gm->box[4].center.x = 500;
	//gm->box[4].center.y = 0;

	//wall blocking left door	
	gm->box[5].width = 15;
	gm->box[5].height = 600;
	gm->box[5].center.x = 250;
	gm->box[5].center.y = 0;		

	}
}




void drawLevel6(Game *gm, Level *lev)
{
	
	float w, h;
	if (lev->levelID == 6) {

        //Draw test platform
	Shape *test;
	glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
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
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
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
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
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
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();
	
	//Draw test platform 5
	Shape *test5;
	glColor3ub(80,110,70);
	test5 = &gm->plat[4];
	glPushMatrix();
	glTranslatef(test5->center.x, test5->center.y, test5->center.z);
	w = test5->width;
	h = test5->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

	//Draw test platform 6
	Shape *test6;
	glColor3ub(80,110,70);
	test6 = &gm->plat[5];
	glPushMatrix();
	glTranslatef(test6->center.x, test6->center.y, test6->center.z);
	w = test6->width;
	h = test6->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

	//Draw test platform 5
	Shape *test7;
	glColor3ub(80,110,70);
	test7 = &gm->plat[6];
	glPushMatrix();
	glTranslatef(test7->center.x, test7->center.y, test7->center.z);
	w = test7->width;
	h = test7->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

		//Draw test platform 5
	Shape *test8;
	glColor3ub(80,110,70);
	test8 = &gm->plat[7];
	glPushMatrix();
	glTranslatef(test8->center.x, test8->center.y, test8->center.z);
	w = test8->width;
	h = test8->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

		//Draw test platform 5
	Shape *test9;
	glColor3ub(80,110,70);
	test9 = &gm->plat[8];
	glPushMatrix();
	glTranslatef(test9->center.x, test9->center.y, test9->center.z);
	w = test9->width;
	h = test9->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

		//Draw test platform 5
	Shape *test10;
	glColor3ub(80,110,70);
	test10 = &gm->plat[9];
	glPushMatrix();
	glTranslatef(test10->center.x, test10->center.y, test10->center.z);
	w = test10->width;
	h = test10->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

		//Draw test platform 5
	Shape *test11;
	glColor3ub(80,110,70);
	test11 = &gm->plat[10];
	glPushMatrix();
	glTranslatef(test11->center.x, test11->center.y, test11->center.z);
	w = test11->width;
	h = test11->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

		//Draw test platform 5
	Shape *test12;
	glColor3ub(80,110,70);
	test12 = &gm->plat[11];
	glPushMatrix();
	glTranslatef(test12->center.x, test12->center.y, test12->center.z);
	w = test12->width;
	h = test12->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

		//Draw test platform 5
	Shape *test13;
	glColor3ub(80,110,70);
	test13 = &gm->plat[12];
	glPushMatrix();
	glTranslatef(test13->center.x, test13->center.y, test13->center.z);
	w = test13->width;
	h = test13->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
			glEnd();	
			glPopMatrix();

	}

}

void setLevel8(Game *gm, Level *lev)
{
	if (lev->levelID == 8) {
        //test platforms
        gm->plat[0].width = 50;
        gm->plat[0].height = 15;
        gm->plat[0].center.x = 200;
        gm->plat[0].center.y = 70;

        //test platforms
        gm->plat[1].width = 50;
        gm->plat[1].height = 15;
        gm->plat[1].center.x = 50;
        gm->plat[1].center.y = 140;

        //test platforms
        gm->plat[2].width = 50;
        gm->plat[2].height = 15;
        gm->plat[2].center.x = 250;
        gm->plat[2].center.y = 210;

        //test platforms
        gm->plat[3].width = 50;
        gm->plat[3].height = 15;
        gm->plat[3].center.x = 250;
        gm->plat[3].center.y = 290;

        //test platforms
        gm->plat[4].width = 50;
        gm->plat[4].height = 15;
        gm->plat[4].center.x = 250;
        gm->plat[4].center.y = 370;

        //test platforms
        gm->plat[5].width = 50;
        gm->plat[5].height = 15;
        gm->plat[5].center.x = 250;
        gm->plat[5].center.y = 450;

        //test platforms below spike
        gm->plat[6].width = 50;
        gm->plat[6].height = 15;
        gm->plat[6].center.x = 600;
        gm->plat[6].center.y = 600;

        //test platforms ground floor
        gm->plat[7].width = 50;
        gm->plat[7].height = 15;
        gm->plat[7].center.x = 250;
        gm->plat[7].center.y = 100;
	
        //test platforms long bottom
        gm->plat[8].width = 250;
        gm->plat[8].height = 15;
        gm->plat[8].center.x = 850;
        gm->plat[8].center.y = 250;

        //test platforms
        gm->plat[9].width = 50;
        gm->plat[9].height = 15;
        gm->plat[9].center.x = 800;
        gm->plat[9].center.y = 150;

        //test platforms
        gm->plat[10].width = 50;
        gm->plat[10].height = 15;
        gm->plat[10].center.x = gm->xres - 300;
        gm->plat[10].center.y = 400;

        //test platforms long top
        gm->plat[11].width = 200;
        gm->plat[11].height = 15;
        gm->plat[11].center.x = 1000;
        gm->plat[11].center.y = 545;

        //test platforms
        gm->plat[12].width = 50;
        gm->plat[12].height = 15;
        gm->plat[12].center.x = gm->xres - 300;
        gm->plat[12].center.y = 400;

        //test spikes
        gm->spike[0].width = 200;
        gm->spike[0].height = 15;
        gm->spike[0].center.x = 300;
        gm->spike[0].center.y = 740;

        //test spikes 2
        gm->spike[1].width = 100;
        gm->spike[1].height = 15;
        gm->spike[1].center.x = 950;
        gm->spike[1].center.y = 265;

        //test spikes 3
        gm->spike[2].width = 100;
        gm->spike[2].height = 15;
        gm->spike[2].center.x = 600;
        gm->spike[2].center.y = 265;

	//wall top of mid	
	gm->box[4].width = 50;
	gm->box[4].height = 350;
	gm->box[4].center.x = 650;
	gm->box[4].center.y = 350;

	//wall blocking left door	
	gm->box[5].width = 15;
	gm->box[5].height = 500;
	gm->box[5].center.x = 250;
	gm->box[5].center.y = 0;		

	}
}




void drawLevel8(Game *gm, Level *lev)
{
	
	float w, h;
	if (lev->levelID == 8) {

        //Draw test platform
	Shape *test;
	glColor3ub(80,110,70);
	test = &gm->plat[0];
	glPushMatrix();
	glTranslatef(test->center.x, test->center.y, test->center.z);
	w = test->width;
	h = test->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//set up spike	
	Shape *spike;
	glColor3ub(80,110,70);
	spike = &gm->spike[0];
	glPushMatrix();
	glTranslatef(spike->center.x, spike->center.y, spike->center.z);
	w = spike->width;
	h = spike->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);

	//set up spike2	
	Shape *spike2;
	glColor3ub(80,110,70);
	spike2 = &gm->spike[1];
	glPushMatrix();
	glTranslatef(spike2->center.x, spike2->center.y, spike2->center.z);
	w = spike2->width;
	h = spike2->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	
	//set up spike3	
	Shape *spike3;
	glColor3ub(80,110,70);
	spike3 = &gm->spike[2];
	glPushMatrix();
	glTranslatef(spike3->center.x, spike3->center.y, spike3->center.z);
	w = spike3->width;
	h = spike3->height;
	prepSpike(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
	

        //Draw test platform 2
	Shape *test2;
	glColor3ub(80,110,70);
	test2 = &gm->plat[1];
	glPushMatrix();
	glTranslatef(test2->center.x, test2->center.y, test2->center.z);
	w = test2->width;
	h = test2->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);


	//Draw test platform 3
	Shape *test3;
	glColor3ub(80,110,70);
	test3 = &gm->plat[2];
	glPushMatrix();
	glTranslatef(test3->center.x, test3->center.y, test3->center.z);
	w = test3->width;
	h = test3->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



	//Draw test platform 4
	Shape *test4;
	glColor3ub(80,110,70);
	test4 = &gm->plat[3];
	glPushMatrix();
	glTranslatef(test4->center.x, test4->center.y, test4->center.z);
	w = test4->width;
	h = test4->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);


	
	//Draw test platform 5
	Shape *test5;
	glColor3ub(80,110,70);
	test5 = &gm->plat[4];
	glPushMatrix();
	glTranslatef(test5->center.x, test5->center.y, test5->center.z);
	w = test5->width;
	h = test5->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



	//Draw test platform 6
	Shape *test6;
	glColor3ub(80,110,70);
	test6 = &gm->plat[5];
	glPushMatrix();
	glTranslatef(test6->center.x, test6->center.y, test6->center.z);
	w = test6->width;
	h = test6->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



	//Draw test platform 5
	Shape *test7;
	glColor3ub(80,110,70);
	test7 = &gm->plat[6];
	glPushMatrix();
	glTranslatef(test7->center.x, test7->center.y, test7->center.z);
	w = test7->width;
	h = test7->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



		//Draw test platform 5
	Shape *test8;
	glColor3ub(80,110,70);
	test8 = &gm->plat[7];
	glPushMatrix();
	glTranslatef(test8->center.x, test8->center.y, test8->center.z);
	w = test8->width;
	h = test8->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



		//Draw test platform 5
	Shape *test9;
	glColor3ub(80,110,70);
	test9 = &gm->plat[8];
	glPushMatrix();
	glTranslatef(test9->center.x, test9->center.y, test9->center.z);
	w = test9->width;
	h = test9->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



		//Draw test platform 5
	Shape *test10;
	glColor3ub(80,110,70);
	test10 = &gm->plat[9];
	glPushMatrix();
	glTranslatef(test10->center.x, test10->center.y, test10->center.z);
	w = test10->width;
	h = test10->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



		//Draw test platform 5
	Shape *test11;
	glColor3ub(80,110,70);
	test11 = &gm->plat[10];
	glPushMatrix();
	glTranslatef(test11->center.x, test11->center.y, test11->center.z);
	w = test11->width;
	h = test11->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



		//Draw test platform 5
	Shape *test12;
	glColor3ub(80,110,70);
	test12 = &gm->plat[11];
	glPushMatrix();
	glTranslatef(test12->center.x, test12->center.y, test12->center.z);
	w = test12->width;
	h = test12->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



		//Draw test platform 5
	Shape *test13;
	glColor3ub(80,110,70);
	test13 = &gm->plat[12];
	glPushMatrix();
	glTranslatef(test13->center.x, test13->center.y, test13->center.z);
	w = test13->width;
	h = test13->height;
	prepPlat(gm);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[0]);
			glVertex2i(-w,-h);
		glTexCoord2f(gm->tex.xp[0], gm->tex.yp[1]);
			glVertex2i(-w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[1]);
			glVertex2i( w, h);
		glTexCoord2f(gm->tex.xp[1], gm->tex.yp[0]);
			glVertex2i( w,-h);
	glEnd();	
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);



	}

}




//sets games objects to 0 and draws the newly cleared game objects to remove them
void clearLevel(Game *gm) 
{
	float w,h;

	for (int i = 0; i < 20; i++) {
		gm->plat[i].width = 0;
        	gm->plat[i].height = 0;
        	gm->plat[i].center.x = 0;
        	gm->plat[i].center.y = 0;
	}


	for (int i = 0; i < 10; i++) {
        //test spikes
 		gm->spike[i].width = 0;
 		gm->spike[i].height = 0;
 		gm->spike[i].center.x = 0;
		gm->spike[i].center.y = 0;
	}

        //wall blocking door    
        gm->box[4].width = 0;
        gm->box[4].height = 0;
        gm->box[4].center.x = 0;
        gm->box[4].center.y = 0;

        gm->box[5].width = 0;
        gm->box[5].height = 0;
        gm->box[5].center.x = 0;
        gm->box[5].center.y = 0;


        //clears test platforms
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

	//set up spike	
	Shape *spike2;
	glColor3ub(80,110,70);
	spike2 = &gm->spike[1];
	glPushMatrix();
	glTranslatef(spike2->center.x, spike2->center.y, spike2->center.z);
	w = spike2->width;
	h = spike2->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//set up spike	
	Shape *spike3;
	glColor3ub(80,110,70);
	spike3 = &gm->spike[2];
	glPushMatrix();
	glTranslatef(spike3->center.x, spike3->center.y, spike3->center.z);
	w = spike3->width;
	h = spike3->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//set up spike	
	Shape *spike4;
	glColor3ub(80,110,70);
	spike4 = &gm->spike[3];
	glPushMatrix();
	glTranslatef(spike4->center.x, spike4->center.y, spike4->center.z);
	w = spike4->width;
	h = spike4->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//set up spike	
	Shape *spike5;
	glColor3ub(80,110,70);
	spike5 = &gm->spike[4];
	glPushMatrix();
	glTranslatef(spike5->center.x, spike5->center.y, spike5->center.z);
	w = spike5->width;
	h = spike5->height;
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

	//Draw test platform 5
	Shape *test5;
	glColor3ub(80,110,70);
	test5 = &gm->plat[4];
	glPushMatrix();
	glTranslatef(test5->center.x, test5->center.y, test5->center.z);
	w = test5->width;
	h = test5->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 6
	Shape *test6;
	glColor3ub(80,110,70);
	test6 = &gm->plat[5];
	glPushMatrix();
	glTranslatef(test6->center.x, test6->center.y, test6->center.z);
	w = test6->width;
	h = test6->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 7
	Shape *test7;
	glColor3ub(80,110,70);
	test7 = &gm->plat[6];
	glPushMatrix();
	glTranslatef(test7->center.x, test7->center.y, test7->center.z);
	w = test7->width;
	h = test7->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 8
	Shape *test8;
	glColor3ub(80,110,70);
	test8 = &gm->plat[7];
	glPushMatrix();
	glTranslatef(test8->center.x, test8->center.y, test8->center.z);
	w = test8->width;
	h = test8->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 9
	Shape *test9;
	glColor3ub(80,110,70);
	test9 = &gm->plat[8];
	glPushMatrix();
	glTranslatef(test9->center.x, test9->center.y, test9->center.z);
	w = test9->width;
	h = test9->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 9
	Shape *test10;
	glColor3ub(80,110,70);
	test10 = &gm->plat[9];
	glPushMatrix();
	glTranslatef(test10->center.x, test10->center.y, test10->center.z);
	w = test10->width;
	h = test10->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 9
	Shape *test11;
	glColor3ub(80,110,70);
	test11 = &gm->plat[10];
	glPushMatrix();
	glTranslatef(test11->center.x, test11->center.y, test11->center.z);
	w = test11->width;
	h = test11->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();



	//Draw test platform 9
	Shape *test12;
	glColor3ub(80,110,70);
	test12 = &gm->plat[11];
	glPushMatrix();
	glTranslatef(test12->center.x, test12->center.y, test12->center.z);
	w = test12->width;
	h = test12->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 9
	Shape *test13;
	glColor3ub(80,110,70);
	test13 = &gm->plat[12];
	glPushMatrix();
	glTranslatef(test13->center.x, test13->center.y, test13->center.z);
	w = test13->width;
	h = test13->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();

	//Draw test platform 9
	Shape *test14;
	glColor3ub(80,110,70);
	test14 = &gm->plat[13];
	glPushMatrix();
	glTranslatef(test14->center.x, test14->center.y, test14->center.z);
	w = test14->width;
	h = test14->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();


}

//used for testing to advance the level
void nextLevel(Game *gm, Level *lev)
{
	Character *p;
	p = &gm->character;
	p->s.center.x = 20;
	lev->levelID++;
	loadLevel(gm, lev);

}
//used for testing to go back a level
void previousLevel(Game *gm, Level *lev)
{
	if (lev->levelID > 1) {
		Character *p;
		p = &gm->character;
		p->s.center.x = WINDOW_WIDTH - 20;
		lev->levelID--;
	}
}

//loads level function based off the Level classes levelID
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
		setLevel4(gm, lev);
		drawLevel4(gm, lev);
		break;
	case 5:
		clearLevel(gm);
		setLevel5(gm, lev);
		drawLevel5(gm, lev);
		break;
	case 6:
		clearLevel(gm);
		setLevel6(gm, lev);
		drawLevel6(gm, lev);
		break;
	case 7:
		clearLevel(gm);
		setLevel7(gm, lev);
		drawLevel7(gm, lev);
		break;
		
	case 8:
		clearLevel(gm);
		setLevel8(gm, lev);
		drawLevel8(gm, lev);
		break;
	
	default:
		clearLevel(gm);
		break;
	
	

    }


}



