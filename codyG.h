/* 
 *Name: Cody Graves
 *Last Modified: 7/8/17
 *Project: Dungeon Escape
 *
 */

#include "kyleS.h"
#include <vector>
#include <string>
#include "ppm.h"
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "timers.h"

using namespace std;

//prototypes
extern unsigned char* buildAlphaData(Ppmimage *img);

enum PlayerState
{
	STATE_IDLE, STATE_RUN, STATE_JUMP, STATE_ATTACK
}; 

class SpriteAnimation
{
	protected: 
	bool enabled, loop;
	char* sheetName;
	char sheetNamePpm[256];
	int startFrame, endFrame, currentFrame, maxFrame;
	int maxRow, maxColumn, startRow, startColumn, currentRow, currentColumn;
	double frameWidth, frameHeight, delay, timeSpan;
	struct timespec time;
	GLuint texture;
	Ppmimage *spriteSheet;

	public:
	SpriteAnimation();
	SpriteAnimation(char* name, int numFrames, int maxr, int maxc, 
		int sFrame, int eFrame, double framew, double frameh, 
		double d, bool l);
	void enable();
	void disable();
	bool isEnabled();
	bool isLoop();
	int getCurrentFrame();
	int getCurrentRow();
	int getMaxRow();
	int getMaxColumn();
	double getFrameWidth();
	double getFrameHeight();
	GLuint* getTexture();
	Ppmimage *getSpriteSheet();
	void nextRow();
	void convertToPpm();
	void createTexture();
	void updateAnimation();
};

class Enemy
{
	protected:
	Vec direction;
	Vec velocity;
	int leftStop, rightStop;
	bool isLeft;
	public:
	Shape s;
	Hitbox hitbox;
	Enemy(Flt w, Flt h, Flt cenX, Flt cenY, Flt hitW, Flt hitH, Flt dirX, 
		Flt dirY, Flt velX, Flt velY, int left, int right, bool l);
	void checkState();
	bool checkIsLeft();
	Flt getX();
	Flt getY();
	void move();
	void stateUnitTest();
	void destroy();
};

void renderSprite(SpriteAnimation anim, int x, int y, bool left);
PlayerState getPlayerState(Character *p, char keys[]);
