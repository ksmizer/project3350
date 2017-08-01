// Cody Wright 
// 6/21/2017
// ===============================================================
// Purpose
// ===============================================================
// Create sound effects for the physics in the game.
// 	-background music
// 	-death sound
// 	-jump sound
//	-landing sound
// 	-explosion
// 	-NPC collision 
//	-spikes
// 	-throw spear
// 	-flames
// ===============================================================
// Keep track of player stats
// 	-time
//	-death count
// 	-kill count 
// ===============================================================

#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "fonts.h"
#include "codyG.h"
#include "kyleS.h"
#include "derrickA.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctime>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

int seconds = 0;
int minutes = 0;
int totalSeconds = 0;
int totalMinutes = 0;
int pauseSeconds = 0;
int pauseMinutes = 0;
int deaths = 0;
int kills = 0;
clock_t startTime;
clock_t pauseTime;
clock_t deadTime;
clock_t thisTime;
clock_t clockTicksTaken;
double timeInSeconds;
bool start = true;
bool finish = false;	

extern void prepPlat(Game *g);
extern void prepBox(Game *g);
extern void prepSpike(Game *g);

class Sound {
	public:
		ALuint alBuffer;
		ALuint alSource;
		ALuint alBuffer_one[2];
		ALuint alSource_one[2];
		ALuint alBuffer_two[2];
		ALuint alSource_two[2];
		ALuint alBuffer_three;
		ALuint alSource_three;
		ALuint alBuffer_four;
		ALuint alSource_four;
		ALuint alBuffer_five;
		ALuint alSource_five;
		ALuint alBuffer_six;
		ALuint alSource_six;
		ALuint alBuffer_seven;
		ALuint alSource_seven;
		ALuint alBuffer_eight;
		ALuint alSource_eight;
		ALuint alBuffer_nine;
		ALuint alSource_nine;
};

void initialize_sound()
{
	alutInit(0, NULL);

	//Clear error state.
	//alGetError();
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
}

void finish_sound()
{
	Sound s;
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &s.alSource);
	alDeleteSources(1, &s.alSource_one[0]);
	alDeleteSources(1, &s.alSource_one[1]);
	alDeleteSources(1, &s.alSource_two[0]);
	alDeleteSources(1, &s.alSource_two[1]);
	alDeleteSources(1, &s.alSource_three);
	alDeleteSources(1, &s.alSource_four);
	alDeleteSources(1, &s.alSource_five);
	alDeleteSources(1, &s.alSource_six);
	alDeleteSources(1, &s.alSource_seven);
	alDeleteSources(1, &s.alSource_eight);
	alDeleteSources(1, &s.alSource_nine);
	//Delete the buffer.
	alDeleteBuffers(1, &s.alBuffer);
	alDeleteBuffers(1, &s.alBuffer_one[0]);
	alDeleteBuffers(1, &s.alBuffer_one[1]);
	alDeleteBuffers(1, &s.alBuffer_two[0]);
	alDeleteBuffers(1, &s.alBuffer_two[1]);
	alDeleteBuffers(1, &s.alBuffer_three);
	alDeleteBuffers(1, &s.alBuffer_four);
	alDeleteBuffers(1, &s.alBuffer_five);
	alDeleteBuffers(1, &s.alBuffer_six);
	alDeleteBuffers(1, &s.alBuffer_seven);
	alDeleteBuffers(1, &s.alBuffer_eight);
	alDeleteBuffers(1, &s.alBuffer_nine);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context
	alcMakeContextCurrent(NULL);
	//Release context(s)
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
}

void thump()
{
	Sound s;

	s.alBuffer = alutCreateBufferFromFile("./thump.wav");
	
	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource);
	alSourcei(s.alSource, AL_BUFFER, s.alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource, AL_GAIN, 0.1f);
	alSourcef(s.alSource, AL_PITCH, 1.0f);
	alSourcei(s.alSource, AL_LOOPING, AL_FALSE);
	alSourcePlay(s.alSource);
}

void flames()
{
	Sound s;

	s.alBuffer_one[0] = alutCreateBufferFromFile("./test.wav");

	s.alBuffer_one[1] = alutCreateBufferFromFile("./flames.wav");

	alGenSources(2, s.alSource_one);

	alSourcei(s.alSource_one[0], AL_BUFFER, s.alBuffer_one[0]);
	alSourcei(s.alSource_one[1], AL_BUFFER, s.alBuffer_one[1]);

	alSourcef(s.alSource_one[0], AL_GAIN, 0.0f);
	alSourcef(s.alSource_one[0], AL_PITCH, 0.0f);
	alSourcei(s.alSource_one[0], AL_LOOPING, AL_FALSE);
	

	alSourcef(s.alSource_one[1], AL_GAIN, 0.0f);
	alSourcef(s.alSource_one[1], AL_PITCH, 0.0f);
	alSourcei(s.alSource_one[1], AL_LOOPING, AL_FALSE);
	

	alSourcePlay(s.alSource_one[1]);
	//flames will not loop until after the first source plays
	for (int i=0; i<10; i++) {
		alSourcePlay(s.alSource_one[0]);
	}
	
}

void background_music()
{
	Sound s;

	s.alBuffer_two[0] = alutCreateBufferFromFile("./test.wav");

	s.alBuffer_two[1] = alutCreateBufferFromFile("./background.wav");

	alGenSources(2, s.alSource_two);
	
	alSourcei(s.alSource_two[0], AL_BUFFER, s.alBuffer_two[0]);
	alSourcei(s.alSource_two[1], AL_BUFFER, s.alBuffer_two[1]);

	alSourcef(s.alSource_two[0], AL_GAIN, 0.0f);
	alSourcef(s.alSource_two[0], AL_PITCH, 0.0f);
	alSourcei(s.alSource_two[0], AL_LOOPING, AL_FALSE);
	
	alSourcef(s.alSource_two[1], AL_GAIN, 0.1f);
	alSourcef(s.alSource_two[1], AL_PITCH, 1.0f);
	//for longer fire set looping to AL_TRUE
	alSourcei(s.alSource_two[1], AL_LOOPING, 1);
	alSourcePlay(s.alSource_two[1]);

	//flames will not loop until after the first source plays
	for (int i=0; i<10; i++) {
		alSourcePlay(s.alSource_two[0]);
	}
}

void hit()
{
	Sound s;
	//Buffer holds the sound information.
	s.alBuffer_three = alutCreateBufferFromFile("./hit.wav");

	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource_three);
	alSourcei(s.alSource_three, AL_BUFFER, s.alBuffer_three);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource_three, AL_GAIN, 0.1f);
	alSourcef(s.alSource_three, AL_PITCH, 1.0f);
	alSourcei(s.alSource_three, AL_LOOPING, AL_FALSE);
	
	alSourcePlay(s.alSource_three);
	
}

void jump()
{
	Sound s;
	//Buffer holds the sound information.
	s.alBuffer_four = alutCreateBufferFromFile("./jump.wav");

	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource_four);
	alSourcei(s.alSource_four, AL_BUFFER, s.alBuffer_four);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource_four, AL_GAIN, 0.1f);
	alSourcef(s.alSource_four, AL_PITCH, 1.0f);
	alSourcei(s.alSource_four, AL_LOOPING, AL_FALSE);
	
	alSourcePlay(s.alSource_four);
	
}

void throw_spear()
{
	Sound s;
	//Buffer holds the sound information.
	s.alBuffer_five = alutCreateBufferFromFile("./throw.wav");

	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource_five);
	alSourcei(s.alSource_five, AL_BUFFER, s.alBuffer_five);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource_five, AL_GAIN, 0.1f);
	alSourcef(s.alSource_five, AL_PITCH, 1.0f);
	alSourcei(s.alSource_five, AL_LOOPING, AL_FALSE);
	
	alSourcePlay(s.alSource_five);
	
}

void spikes()
{
	Sound s;
	//Buffer holds the sound information.
	s.alBuffer_six = alutCreateBufferFromFile("./spikes.wav");

	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource_six);
	alSourcei(s.alSource_six, AL_BUFFER, s.alBuffer_six);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource_six, AL_GAIN, 0.1f);
	alSourcef(s.alSource_six, AL_PITCH, 1.0f);
	alSourcei(s.alSource_six, AL_LOOPING, AL_FALSE);
	
	alSourcePlay(s.alSource_six);
	
}

void death()
{
	Sound s;
	//Buffer holds the sound information.
	
	s.alBuffer_seven = alutCreateBufferFromFile("./death.wav");

	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource_seven);
	alSourcei(s.alSource_seven, AL_BUFFER, s.alBuffer_seven);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource_seven, AL_GAIN, 0.1f);
	alSourcef(s.alSource_seven, AL_PITCH, 1.0f);
	alSourcei(s.alSource_seven, AL_LOOPING, AL_FALSE);
	
	alSourcePlay(s.alSource_seven);
}

void explosion()
{
	Sound s;
	//Buffer holds the sound information.
	
	s.alBuffer_eight = alutCreateBufferFromFile("./explosion.wav");

	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource_eight);
	alSourcei(s.alSource_eight, AL_BUFFER, s.alBuffer_eight);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource_eight, AL_GAIN, 0.1f);
	alSourcef(s.alSource_eight, AL_PITCH, 1.0f);
	alSourcei(s.alSource_eight, AL_LOOPING, AL_FALSE);
	
	alSourcePlay(s.alSource_eight);
	
}

// reset the clock
void resumeTime() { thisTime = clock(); }

void countDeath() { deaths++; }

void killCount() { kills++; }

void setDeathTime() { deadTime = clock(); }

// at the begginning of game start timer on impact
void setTime()
{
	if (start == true) {
		thisTime = clock();
		start = false;
	}
}

// if all times are 0 start the timer
void initializeTime()
{
	if (totalMinutes == 0 && totalSeconds == 0) {
		if (seconds == 0 && minutes == 0) {
			thisTime = clock();
		}
	}
}

void resetTime()
{
	int s;
	int m;

		
	if (seconds >= 60)
	{
		totalMinutes = totalMinutes + (seconds / 60);
		totalSeconds = totalSeconds + (seconds % 60);
			
	} else {
		totalSeconds = totalSeconds + seconds;
		totalMinutes = totalMinutes + minutes;
	}
	if (totalSeconds >= 60) {
		m = (totalSeconds / 60);
		s = (totalSeconds % 60);
		totalMinutes = totalMinutes + m;
		totalSeconds = s;
	}
	seconds = 0;
	minutes = 0;
	resumeTime();
}

void setPauseTime()
{
	// reset pause time if its not 0
	if (pauseSeconds != 0) {
		pauseSeconds = 0;
	}
	if (pauseMinutes != 0) {
		pauseMinutes = 0;
	}
	// set pausetime when player hits tab
	pauseSeconds = seconds;
	pauseMinutes = minutes;
}

void totalTimer()
{
	// current time minus the time when the clock started
	clockTicksTaken = clock() - thisTime;
	timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
	// seconds = timeInSeconds * 20 for csub computers
	seconds = timeInSeconds * 21.5;
	minutes = seconds / 60;
	seconds = seconds % 60;
	// running total of pause time
	minutes = minutes + pauseMinutes;
	seconds = seconds + pauseSeconds;
}

void outputScore(Game *gm)
{
	finish = true;
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	h = 50;
	w = 50;
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
	r.bot = gm->yres/2 + 800;
	r.left = gm->xres/2.3;
	r.center = .5;
	if (totalSeconds < 10) {
		ggprint8b(&r, 16, c, "Total Time: %d:0%d", totalMinutes, totalSeconds);
	} else {
		ggprint8b(&r, 16, c, "Total Time: %d:%d", totalMinutes, totalSeconds);
	}
	ggprint8b(&r, 16, c, "Deaths: %d", deaths);
	ggprint8b(&r, 16, c, "Kills: %d", kills);
}

void outputCurrentScore(Game *gm)
{
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_GAMEPLAY && start == false && finish == false) {
		totalTimer();
		h = 50;
		w = 50;
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
		r.bot = gm->yres/2 + 400;
		r.left = gm->xres/45;
		r.center = .5;
		if (seconds < 10) {
			ggprint8b(&r, 16, c, "Time: %d:0%d", minutes, seconds);
		} else {
			ggprint8b(&r, 16, c, "Time: %d:%d", minutes, seconds);
		}
		// use this code for testing timer on smaller 
		// computer monitors or laptops
		ggprint8b(&r, 16, c, "Deaths: %d", deaths);
		ggprint8b(&r, 16, c, "Kills: %d", kills);
		/*r.bot = gm->yres/2 + 200;
		r.left = gm->xres/45;
		r.center = .5;
		if (seconds < 10) {
			ggprint8b(&r, 16, c, "Time: %d:0%d", minutes, seconds);
		} else {
			ggprint8b(&r, 16, c, "Time: %d:%d", minutes, seconds);
		}
		ggprint8b(&r, 16, c, "Deaths: %d", deaths);
		ggprint8b(&r, 16, c, "Kills: %d", kills);*/
	}
}

void loadFlames(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/flames.png tmp.ppm");
	gm->tex.flames = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
    	glGenTextures(1, &p->tex.flamesTexture);
	int w = gm->tex.flames->width;
	int h = gm->tex.flames->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.flamesTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, gm->tex.flames->data);
	unlink("./tmp.ppm");
	gm->tex.xf[0] = 0.0;
	gm->tex.xf[1] = 1.0;
	gm->tex.yf[0] = 0.0;
	gm->tex.yf[1] = 1.0;

}

void prepFlames(Game*gm)
{
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.flamesTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

}

void drawLevel7(Game *gm, Level *lev)
{
	float w, h;
	if (lev->levelID == 7) {

		//set up spike 1	
		Shape *spike;
		//glColor3ub(80,110,70);
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
		glDisable(GL_BLEND);
        	
		//set up spike 2
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
		glDisable(GL_BLEND);
			
		//Draw test platform 1
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
		
	        //Draw test platform 2
		Shape *test2;
		//glColor3ub(80,110,70);
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
		//glColor3ub(80,110,70);
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
		//glColor3ub(80,110,70);
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
		
		//set up spike 3
		Shape *spike3;	
		glColor3ub(80,110,70);
		spike3 = &gm->spike[2];
		glPushMatrix();
		glTranslatef(spike3->center.x, spike3->center.y, spike3->center.z);
		w = spike3->width;
		h = spike3->height;
		prepFlames(gm);
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
		glDisable(GL_BLEND);
		
		//Draw test platform 5
		Shape *test5;
		//glColor3ub(80,110,70);
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
	
		//set up spike 4	
		Shape *spike4;
		glColor3ub(80,110,70);
		spike4 = &gm->spike[3];
		glPushMatrix();
		glTranslatef(spike4->center.x, spike4->center.y, spike4->center.z);
		w = spike4->width;
		h = spike4->height;
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
		glDisable(GL_BLEND);
		
		//Draw test platform 6
		Shape *test6;
		//glColor3ub(80,110,70);
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
		//glColor3ub(80,110,70);
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
		
		//set up spike 5	
		Shape *spike5;
		//Color3ub(80,110,70);
		spike5 = &gm->spike[4];
		glPushMatrix();
		glTranslatef(spike5->center.x, spike5->center.y, spike5->center.z);
		w = spike5->width;
		h = spike5->height;
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
		glDisable(GL_BLEND);
		
		//Draw test platform 8
		Shape *test8;
		//glColor3ub(80,110,70);
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
		//glColor3ub(80,110,70);
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
		//glColor3ub(80,110,70);
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

		//Draw test platform 11
		Shape *test11;
		//glColor3ub(80,110,70);
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
	
		//spike 6	
		Shape *spike6;	
		//glColor3ub(80,110,70);
		spike6 = &gm->spike[5];
		glPushMatrix();
		glTranslatef(spike6->center.x, spike6->center.y, spike6->center.z);
		w = spike6->width;
		h = spike6->height;
		prepFlames(gm);
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

		//spike 7	
		Shape *spike7;	
		//glColor3ub(80,110,70);
		spike7 = &gm->spike[6];
		glPushMatrix();
		glTranslatef(spike7->center.x, spike7->center.y, spike7->center.z);
		w = spike7->width;
		h = spike7->height;
		prepFlames(gm);
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
		
		//spike 8	
		Shape *spike8;	
		glColor3ub(80,110,70);
		spike8 = &gm->spike[7];
		glPushMatrix();
		glTranslatef(spike8->center.x, spike8->center.y, spike8->center.z);
		w = spike8->width;
		h = spike8->height;
		prepFlames(gm);
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
		
		//spike 9	
		Shape *spike9;	
		glColor3ub(80,110,70);
		spike9 = &gm->spike[8];
		glPushMatrix();
		glTranslatef(spike9->center.x, spike9->center.y, spike9->center.z);
		w = spike9->width;
		h = spike9->height;
		prepFlames(gm);
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
		
		//spike 10	
		Shape *spike10;	
		glColor3ub(80,110,70);
		spike10 = &gm->spike[9];
		glPushMatrix();
		glTranslatef(spike10->center.x, spike10->center.y, spike10->center.z);
		w = spike10->width;
		h = spike10->height;
		prepFlames(gm);
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
		
		//spike 11
		Shape *spike11;	
		glColor3ub(80,110,70);
		spike11 = &gm->spike[10];
		glPushMatrix();
		glTranslatef(spike11->center.x, spike11->center.y, spike11->center.z);
		w = spike11->width;
		h = spike11->height;
		prepFlames(gm);
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
		
		//spike 12	
		Shape *spike12;	
		glColor3ub(80,110,70);
		spike12 = &gm->spike[11];
		glPushMatrix();
		glTranslatef(spike12->center.x, spike12->center.y, spike12->center.z);
		w = spike12->width;
		h = spike12->height;
		prepFlames(gm);
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
		
		//spike 13	
		Shape *spike13;	
		glColor3ub(80,110,70);
		spike13 = &gm->spike[12];
		glPushMatrix();
		glTranslatef(spike13->center.x, spike13->center.y, spike13->center.z);
		w = spike13->width;
		h = spike13->height;
		prepFlames(gm);
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
			
		//Draw test platform 12
		Shape *test12;
		//glColor3ub(80,110,70);
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
	}
}

void setLevel7(Game *gm, Level *lev)
{
	if (lev->levelID == 7) {
		//test spikes1
		gm->spike[0].width = 300;
		gm->spike[0].height = 15;
		gm->spike[0].center.x = gm->xres - 885;
		gm->spike[0].center.y = 135;
		
		//test spikes2
		gm->spike[1].width = 150;
		gm->spike[1].height = 15;
		gm->spike[1].center.x = gm->xres - 350;
		gm->spike[1].center.y = 135;
			
		//test wall1		
		gm->box[4].width = 10;
		gm->box[4].height = 600;
		gm->box[4].center.x = 1100;
		gm->box[4].center.y = 0;
	
		//test platform1
		gm->plat[0].width = 25;
		gm->plat[0].height = 15;
		gm->plat[0].center.x = 1075;
		gm->plat[0].center.y = 135;
	
		//test platforms2
		gm->plat[1].width = 15;
		gm->plat[1].height = 15;
		gm->plat[1].center.x = 820;
		gm->plat[1].center.y = 250;
	
		//test platforms3
		gm->plat[2].width = 15;
		gm->plat[2].height = 15;
		gm->plat[2].center.x = 200;
		gm->plat[2].center.y = 250;
	
		//test platforms4
		gm->plat[3].width = 30;
		gm->plat[3].height = 15;
		gm->plat[3].center.x = 7;
		gm->plat[3].center.y = 375;
	
		//test spikes3
		gm->spike[2].width = 50;
		gm->spike[2].height = 15;
		gm->spike[2].center.x = 250;
		gm->spike[2].center.y = 450;
		
		//wall 2	
		gm->box[5].width = 15;
		gm->box[5].height = 115;
		gm->box[5].center.x = 200;
		gm->box[5].center.y = 550;
		
		//test platforms5
		gm->plat[4].width = 15;
		gm->plat[4].height = 15;
		gm->plat[4].center.x = 180;
		gm->plat[4].center.y = 450;
	        
		//test spikes4
		gm->spike[3].width = 60;
		gm->spike[3].height = 15;
		gm->spike[3].center.x = 60;
		gm->spike[3].center.y = 550;
		
		//test platforms6
		gm->plat[5].width = 30;
		gm->plat[5].height = 5;
		gm->plat[5].center.x = 7;
		gm->plat[5].center.y = 565;
		
		//test platforms7
		gm->plat[6].width = 50;
		gm->plat[6].height = 10;
		gm->plat[6].center.x = 415;
		gm->plat[6].center.y = 470;
		
		//test spikes5
		gm->spike[4].width = 350;
		gm->spike[4].height = 15;
		gm->spike[4].center.x = 670;
		gm->spike[4].center.y = 655;
	        
		//test platforms8
		gm->plat[7].width = 50;
		gm->plat[7].height = 10;
		gm->plat[7].center.x = 600;
		gm->plat[7].center.y = 470;
		
		//test platforms9
		gm->plat[8].width = 50;
		gm->plat[8].height = 10;
		gm->plat[8].center.x = 800;
		gm->plat[8].center.y = 470;
	
		//test platforms10
		gm->plat[9].width = 50;
		gm->plat[9].height = 10;
		gm->plat[9].center.x = 1000;
		gm->plat[9].center.y = 470;
	        
		//test spikes 6
		gm->spike[5].width = 50;
		gm->spike[5].height = 15;
		gm->spike[5].center.x = 350;
		gm->spike[5].center.y = 450;
		
		//test spikes 7
		gm->spike[6].width = 50;
		gm->spike[6].height = 15;
		gm->spike[6].center.x = 450;
		gm->spike[6].center.y = 450;
		
		//test spikes 8
		gm->spike[7].width = 50;
		gm->spike[7].height = 15;
		gm->spike[7].center.x = 550;
		gm->spike[7].center.y = 450;
		
		//test spikes 9
		gm->spike[8].width = 50;
		gm->spike[8].height = 15;
		gm->spike[8].center.x = 650;
		gm->spike[8].center.y = 450;
		
		//test spikes 10
		gm->spike[9].width = 50;
		gm->spike[9].height = 15;
		gm->spike[9].center.x = 750;
		gm->spike[9].center.y = 450;
		
		//test spikes 11
		gm->spike[10].width = 50;
		gm->spike[10].height = 15;
		gm->spike[10].center.x = 850;
		gm->spike[10].center.y = 450;
		
		//test spikes 12
		gm->spike[11].width = 50;
		gm->spike[11].height = 15;
		gm->spike[11].center.x = 950;	
		gm->spike[11].center.y = 450;
		
		//test spikes 13
		gm->spike[12].width = 50;
		gm->spike[12].height = 15;
		gm->spike[12].center.x = 1050;
		gm->spike[12].center.y = 450;
	        
		//test platforms12
		gm->plat[11].width = 15;
		gm->plat[11].height = 15;
		gm->plat[11].center.x = 520;
		gm->plat[11].center.y = 250;
	}
}
