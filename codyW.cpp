// Cody Wright 
// 6/21/2017
// Purpose: Create sound effects for certain funcitons 
// in the main file. 

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
#define SIZE 40
#define HOST "www.google.com"
#define PAGE "/"
#define PORT 80
#define USERAGENT "HTMLGET 1.0"
int seconds = 0;
int minutes = 0;
int deaths = 0;
int kills = 0;
clock_t startTime;
clock_t clockTicksTaken;
double timeInSeconds;

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
	//Buffer holds the sound information.
	s.alBuffer = alutCreateBufferFromFile("./thump.wav");

	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &s.alSource);
	alSourcei(s.alSource, AL_BUFFER, s.alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(s.alSource, AL_GAIN, 0.1f);
	alSourcef(s.alSource, AL_PITCH, 1.0f);
	alSourcei(s.alSource, AL_LOOPING, AL_FALSE);
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
	alSourcePlay(s.alSource);
}

void flames()
{
	Sound s;

	s.alBuffer_one[0] = alutCreateBufferFromFile("./test.wav");

	s.alBuffer_one[1] = alutCreateBufferFromFile("./background.wav");

	alGenSources(2, s.alSource_one);

	alSourcei(s.alSource_one[0], AL_BUFFER, s.alBuffer_one[0]);
	alSourcei(s.alSource_one[1], AL_BUFFER, s.alBuffer_one[1]);

	alSourcef(s.alSource_one[0], AL_GAIN, 0.0f);
	alSourcef(s.alSource_one[0], AL_PITCH, 0.0f);
	alSourcei(s.alSource_one[0], AL_LOOPING, AL_FALSE);
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}

	alSourcef(s.alSource_one[1], AL_GAIN, 0.5f);
	alSourcef(s.alSource_one[1], AL_PITCH, 1.0f);
	//for longer fire set looping to AL_TRUE
	alSourcei(s.alSource_one[1], AL_LOOPING, AL_FALSE);
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}

	alSourcePlay(s.alSource_one[1]);
	//flames will not loop until after the first source plays
	for (int i=0; i<10; i++) {
		alSourcePlay(s.alSource_one[0]);
	}
	return;
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
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}

	alSourcef(s.alSource_two[1], AL_GAIN, 0.1f);
	alSourcef(s.alSource_two[1], AL_PITCH, 1.0f);
	//for longer fire set looping to AL_TRUE
	alSourcei(s.alSource_two[1], AL_LOOPING, AL_TRUE);
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}

	alSourcePlay(s.alSource_two[1]);
	//flames will not loop until after the first source plays
	for (int i=0; i<10; i++) {
		alSourcePlay(s.alSource_two[0]);
	}
	return;
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
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
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
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
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
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
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
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
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
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
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
	
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
	alSourcePlay(s.alSource_eight);
}

void initializeTime()
{
	startTime = clock();
}

void countDeath()
{
	deaths++;
}

void killCount()
{
	kills++;
}

void timer(int mode)
{
	if (mode == 1) {
		clockTicksTaken = clock() - startTime;
		timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
		seconds = timeInSeconds * 10;
		minutes = seconds / 60;
		seconds = seconds % 60;
	}
}

void outputScore(Game *gm)
{
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_GAMEOVER) {
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
		r.bot = gm->yres/2 + 40;
		r.left = gm->xres/2.3;
		r.center = .5;
		if (seconds < 10) {
			ggprint8b(&r, 16, c, "Time: %d:0%d", minutes, seconds);
		} else {
			ggprint8b(&r, 16, c, "Time: %d:%d", minutes, seconds);
		}
		ggprint8b(&r, 16, c, "Deaths: %d", deaths);
		//ggprint8b(&r, 16, c, "Kills: %d", kills);
	}
}

void outputCurrentScore(Game *gm)
{
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_GAMEPLAY) {
		timer(1);
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
		r.bot = gm->yres/2 + 250;
		r.left = gm->xres/45;
		r.center = .5;
		if (seconds < 10) {
			ggprint8b(&r, 16, c, "Time: %d:0%d", minutes, seconds);
		} else {
			ggprint8b(&r, 16, c, "Time: %d:%d", minutes, seconds);
		}
		ggprint8b(&r, 16, c, "Deaths: %d", deaths);
		//ggprint8b(&r, 16, c, "Kills: %d", kills);
	}
}

