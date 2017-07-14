/* 
 *Name: Cody Graves
 *Last Modified: 7/8/17
 *Project: Dungeon Escape
 *Week 4: Created SpriteAnimation class and functionality to add generic
 * sprite animations in game. 
 * USAGE: Create SpriteAnimation object somewhere in program, in
 * void init_opengl(void) call animation.convertToPpm(); and 
 * animation.createTexture();, in void movement(Game *game) call
 * animation.enable() and animation.updateAnimation();
 * in void render(Game *game) call renderSprite(animation, x, y);
 * where x and y are the positions you want to render the sprite.
 *Week 5: Added PlayerState to control player animations. Added control 
 * for facing left to render sprite reversed. 
 *Week 6: Added Enemy class. Has no functionality with collision, simply moves
 * left and right while cycling through walk animation. Added Unit Test for
 * enemy which verifies it is between left and right stop points.
 *Week 7: Added sprite vector for enemy class. Added moveEnemy function to move
 * position of enemies.
 */

#include "codyG.h"

Timers timers;

//---------------------------------------------------------
//SpriteAnimation Class

SpriteAnimation::SpriteAnimation ()
{
	enabled = false;
	spriteSheet = NULL;
	startFrame=currentFrame=maxFrame = -1;
	delay = 0.1;
}

//class constructor
SpriteAnimation::SpriteAnimation(char* name, int maxr, int maxc,
	int numFrames, int sFrame, int eFrame, double framew, double frameh,
	double d, bool l)
{
	enabled = false;
	loop = l;
	sheetName = name;
	spriteSheet = NULL;
	maxRow = maxr;
	maxColumn = maxc;
	maxFrame = numFrames;
	startFrame = sFrame;
	currentFrame = sFrame;
	endFrame = eFrame;;
	frameWidth = framew;
	frameHeight = frameh;
	delay = d;
	loop = l;

	//calculate current row/column
	int tmpFrame = 0;
	for (int i = 0; i < maxRow; i++) {
		for (int j = 0; j < maxColumn; j++) {
			if (tmpFrame == startFrame) {
				startColumn = currentColumn = j;
				startRow = currentRow = i;
				return;
			}
			tmpFrame ++;
		}
	}
}

//class destructor
SpriteAnimation::~SpriteAnimation() { }

//convert image to usable format
void SpriteAnimation::convertToPpm()
{
	char command[128];

	strcpy(command, "convert ./images/");
	strcat(command, sheetName);
	strcat(command, " ");

	strcat(sheetNamePpm, sheetName);
	sheetNamePpm[strlen(sheetNamePpm) - 4] = 0;
	strcat(sheetNamePpm, ".ppm");
	strcat(command, sheetNamePpm);

	system(command);
	spriteSheet = ppm6GetImage(sheetNamePpm);
}

//creates texture using ppm image
void SpriteAnimation::createTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	unsigned char *tmpData = buildAlphaData(spriteSheet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spriteSheet->width,
		spriteSheet->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmpData);
	free(tmpData);
	unlink(sheetNamePpm);
}

//cycles animation through start frame to end frame
void SpriteAnimation::updateAnimation()
{
	if (!enabled)
		return;
	timers.recordTime(&timers.timeCurrent);
	timeSpan = timers.timeDiff(&time, &timers.timeCurrent);
	if (timeSpan > delay) {
		++ currentFrame;
		if (currentFrame > endFrame) {
			if (!loop)
				disable();
			if (startFrame != endFrame)
				currentFrame = startFrame;
			else
				currentFrame = startFrame;
			currentRow = startRow;
			currentColumn = startColumn;
		}
		timers.recordTime(&time);
	}
}

void SpriteAnimation::nextRow() { currentRow += 1; }

void SpriteAnimation::enable() { enabled = true; }

void SpriteAnimation::disable() { enabled = false; }

bool SpriteAnimation::isEnabled() {	return enabled; }

bool SpriteAnimation::isLoop() { return loop; }

int SpriteAnimation::getCurrentFrame() { return currentFrame; }

int SpriteAnimation::getCurrentRow() { return currentRow; }

int SpriteAnimation::getMaxRow() { return maxRow; }

int SpriteAnimation::getMaxColumn() { return maxColumn; }

double SpriteAnimation::getFrameWidth() { return frameWidth; }

double SpriteAnimation::getFrameHeight() { return frameHeight; }

GLuint* SpriteAnimation::getTexture() { return &texture; }

Ppmimage* SpriteAnimation::getSpriteSheet() { return spriteSheet; }

//---------------------------------------------------------
//Enemy Class

//constructor
Enemy::Enemy(int t, Flt w, Flt h, Flt cenX, Flt cenY, Flt hitW, Flt hitH, 
	Flt dirX, Flt dirY, Flt velX, Flt velY, int left, int right, bool l)
{
	type = t;
	s.width = w;
	s.height = h;
	s.center.x = cenX;
	s.center.y = cenY;
	s.center.z = 0.0;
	hitbox.width = hitW;
	hitbox.height = hitH;
	direction.x = dirX;
	direction.y = dirY;
	velocity.x = velX;
	velocity.y = velY;
	leftStop = left;
	rightStop = right;
	isLeft = l;
}

//destructor
Enemy::~Enemy() { }

//checks when enemy should turn around
void Enemy::checkState()
{
	if (s.center.x >= rightStop || s.center.x <= leftStop) {
		velocity.x = -velocity.x;
		isLeft = !isLeft;
	}
}

bool Enemy::checkIsLeft() { return isLeft; }

Flt Enemy::getX() { return s.center.x; }

Flt Enemy::getY() { return s.center.y; }

void Enemy::setX(Flt x) { s.center.x = x; }

void Enemy::setY(Flt y) { s.center.y = y; }

void Enemy::move() 
{ 
	s.center.x += velocity.x;
	hitbox.center.x = s.center.x;
}

void Enemy::initAnimations()
{
	animations.clear();
	if (type == 0) {
		SpriteAnimation anim((char*)"zombie.png", 1, 5, 5, 0, 3, 27, 40, 0.1, true);
		animations.push_back(anim);
	}
}

//checks if enemy is between left and right stop values
void Enemy::stateUnitTest()
{
	if (s.center.x > rightStop || s.center.x < leftStop)
		printf("Failure...\n");
	else
		printf("Success!\n");
	printf("Current Position: %f\n", s.center.x);
	printf("Expected Position Between: %i and %i\n", leftStop, rightStop);
}
//---------------------------------------------------------
// FUNCTIONS
void moveEnemy(Enemy &e, int xpos, int ypos)
{
	e.setX(xpos);
	e.setY(ypos);	
}

void renderSprite(SpriteAnimation sprite, int xposition, 
	int yposition, bool left)
{
	if (sprite.isEnabled()) {
		int ix;
		float tx, ty, h, w;
		float xPer, yPer;
		SpriteAnimation *tmp = &sprite;
		h = tmp->getFrameHeight();
		w = tmp->getFrameWidth();
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, *(tmp->getTexture()));
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		ix = tmp->getCurrentFrame() % tmp->getMaxColumn();
		//end of current column, shift down to next row
		if (tmp->getCurrentFrame() > (tmp->getMaxColumn() - 1) * 
			(tmp->getCurrentRow() + 1))
			tmp->nextRow();
		tx = float(ix / float(tmp->getMaxColumn()));
		ty = float(tmp->getCurrentRow() / float(tmp->getMaxRow()));
		xPer = 1.0 / tmp->getMaxColumn();
		yPer = 1.0 / tmp->getMaxRow();
		glBegin(GL_QUADS);
		if (left) {
			glTexCoord2f(tx + xPer,                ty + yPer);
			glVertex2i(w * -1 + xposition,  h * -1 + yposition);
			glTexCoord2f(tx + xPer,                ty);
			glVertex2i(w * -1 + xposition,  h + yposition);
			glTexCoord2f(tx,         ty);
			glVertex2i(w + xposition, h + yposition);
			glTexCoord2f(tx,         ty + yPer);
			glVertex2i(w + xposition, h * -1 + yposition);
		}
		else {
			glTexCoord2f(tx,                ty + yPer);
			glVertex2i(w * -1 + xposition,  h * -1 + yposition);
			glTexCoord2f(tx,                ty);
			glVertex2i(w * -1 + xposition,  h + yposition);
			glTexCoord2f(tx + xPer,         ty);
			glVertex2i(w + xposition, h + yposition);
			glTexCoord2f(tx + xPer,         ty + yPer);
			glVertex2i(w + xposition, h * -1 + yposition);
		}
		glEnd();
		glPopMatrix();
		glDisable(GL_ALPHA_TEST);
	}
}

PlayerState getPlayerState(Character *p, char keys[])
{
	PlayerState tmp = STATE_IDLE;
	if (keys[XK_Right] || keys[XK_D] || keys[XK_d] ||
		keys[XK_Left] || keys[XK_A] || keys[XK_a])
		tmp = STATE_RUN;
	if (p->jumpCurrent > 0)
		tmp = STATE_JUMP;
	return tmp;
}



