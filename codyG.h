/* 
 *Name: Cody Graves
 *Last Modified: 7/15/17
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
#include <iostream> 

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
	SpriteAnimation(char* name, int maxr, int maxc, int numFrames,
		int sFrame, int eFrame, double framew, double frameh, 
		double d, bool l);
	~SpriteAnimation();
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
	int type;
	Vec direction;
	int leftStop, rightStop;
	bool isLeft;
	public:
	vector<SpriteAnimation> animations;
	Shape s;
	Vec velocity;
	Hitbox hitbox;
	Enemy(int t, Flt w, Flt h, Flt cenX, Flt cenY, Flt hitW, Flt hitH, 
		Flt dirX, Flt dirY, Flt velX, Flt velY, int left, int right, bool l);
	~Enemy();
	void checkState();
	bool checkIsLeft();
	Flt getX();
	Flt getY();
	void setX(Flt x);
	void setY(Flt y);
	void move();
	void stateUnitTest();
	void initAnimations();
};

class SavePoint
{
	protected:
	int xPos, yPos;
	bool enabled;
	public:
	vector<SpriteAnimation> animations;
	SavePoint(int x, int y, bool e);
	~SavePoint();
	void initAnimations();
	int getX();
	int getY();
	bool checkIsEnabled();
	void enable();
	void disable();
};

void movePlayer(Character &c, int xpos, int ypos);
void moveEnemy(Enemy &e, int xpos, int ypos);
void renderSprite(SpriteAnimation anim, int x, int y, Flt modifier, bool left);
PlayerState getPlayerState(Character *p, char keys[]);
