// CMPS 3350 Final Project
// Editor: Kyle Smizer-Muldoon

//movement:
// character movement update & refresh sound check
//
//charHurtUpdate:
// update character's current hitbox to be more accurate to current state
//
//charHurt:
// check if character's hurtbox collides with enemy/hazard hitboxes
//
//enemyHurt:
// check if enemy's hurtbox collides with weapon/character hitboxes
//
//charCollision:
// character collision detection with shapes/environment
//
//makeWeapon:
// weapon creation and update
//
//checkPause:
// check if pause key is pressed & create pause screen
//
//checkStart:
// check if state is STARTMENU & create start menu
//
//chekcGameOver:
// check if state is GAMEOVER & create Game Over screen


#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "fonts.h"
#include "codyG.h"
#include "kyleS.h"
//#include "derrickA.h"
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

extern void thump();
extern void hit();
extern void jump();
extern void throw_spear();
extern void spikes();
extern void death();
extern void flames();

void makeWeapon(Game *game, Character *p);

void movement(Game *game, Character *p, PlayerState ps, char keys[])
{
	//Character movement check
	if (keys[XK_Left] || keys[XK_A] || keys[XK_a]) {
		p->isLeft = true;
		if (keys[XK_Shift_L] || keys[XK_Shift_R])
			p->velocity.x = WALK * -2;
		else
			p->velocity.x = -WALK;
	}
	if (keys[XK_Right] || keys[XK_D] || keys[XK_d]) {
		p->isLeft = false;
		if (keys[XK_Shift_L] || keys[XK_Shift_R])
			p->velocity.x = WALK * 2;
		else
			p->velocity.x = WALK;
	}
	if (keys[XK_Up] ||  keys[XK_W] || keys[XK_w]) {
		if (p->jumpCurrent < p->jumpMax && p->velocity.y <= (3/4 * JUMP)) {
			p->velocity.y = JUMP;
			++p->jumpCurrent;
			p->hurtJump = true;
			jump();
			p->soundChk = !p->soundChk;
		}
	}
	if (keys[XK_Left] + keys[XK_Right] + keys[XK_a]
			+ keys[XK_A] + keys[XK_d] + keys[XK_D] == 0) {
		p->velocity.x = 0;
	}
	if (keys[XK_j] || keys[XK_J]) {
		makeWeapon(game, p);
	}
	return;
}

void charHurtUpdate(Game *game, Character *p, Enemy *e)
{
	// Character hurtbox shift for jumping
	if (p->hurtJump == true) {
		p->hurt.height = p->s.height - 15;
		p->hurt.width = p->s.width + 10;
	}
	if (p->hurtJump == false) {
		p->hurt.height = p->s.height;
		p->hurt.height = p->s.width;
	}		
}

void charHurt(Game *game, Character *p, Enemy *e)
{
	charHurtUpdate(game, p, e);
	// Spike death detection
	int spikeTop[5], spikeBottom[5], spikeLeft[5], spikeRight[5];
	for (int i = 0; i < 5; i++) {
		Shape *s = &game->spike[i];
		spikeTop[i] = s->center.y + s->height + p->s.height;
		spikeBottom[i] = s->center.y - s->height - p->s.height;
		spikeLeft[i] = s->center.x - s->width - p->s.width;
		spikeRight[i] = s->center.x + s->width + p->s.width;
		if (p->s.center.y < spikeTop[i] && p->s.center.y > spikeBottom[i]) {
			if (p->s.center.x > spikeLeft[i] && p->s.center.x < spikeRight[i]) {
				//Top collision detection
				if (p->s.center.y < spikeTop[i]
					&& p->s.center.y > spikeTop[i] - OFFSET
						&& p->s.center.x < spikeRight[i]
							&& p->s.center.x > spikeLeft[i]
								&& p->velocity.y < 0) {
					if (p->soundChk == true) {
						p->soundChk = !p->soundChk;
					}
					p->s.center.y = spikeTop[i];
					p->velocity.y = 0;
					p->jumpCurrent = 2;
					death();
					game->state = STATE_GAMEOVER;
				}
			}
		}
	}
	// Enemy collision
	int enemyHit[4];
	enemyHit[0] = e->s.center.y + e->hitbox.height + p->hurt.height;
	enemyHit[1] = e->s.center.y - e->hitbox.height - p->hurt.height;
	enemyHit[2] = e->s.center.x + e->hitbox.width + p->hurt.width;
	enemyHit[3] = e->s.center.x - e->hitbox.width - p->hurt.width;
	if (p->s.center.y < enemyHit[0] && p->s.center.y > enemyHit[1]) {
		if (p->s.center.x < enemyHit[2] && p->s.center.x > enemyHit[3]) {
			p->velocity.y = 0;
			p->jumpMax = 0;
			death();
			game->state = STATE_GAMEOVER;
		}
	}
}

void enemyHurt(Game *game, Character *p, Enemy *e)
{
	//Spike death detection
	int spikeTop[5], spikeBottom[5], spikeLeft[5], spikeRight[5];
	for (int i = 0; i < 5; i++) {
		Shape *s = &game->spike[i];
		spikeTop[i] = s->center.y + s->height + e->s.height;
		spikeBottom[i] = s->center.y - s->height - e->s.height;
		spikeLeft[i] = s->center.x - s->width - e->s.width;
		spikeRight[i] = s->center.x + s->width + e->s.width;
		if (e->s.center.y < spikeTop[i] && e->s.center.y > spikeBottom[i]) {
			if (e->s.center.x > spikeLeft[i] && e->s.center.x < spikeRight[i]) {
				//Top collision detection
				if (e->s.center.y < spikeTop[i]
					&& e->s.center.y > spikeTop[i] - OFFSET
						&& e->s.center.x < spikeRight[i]
							&& e->s.center.x > spikeLeft[i]) {
					e->s.center.y = spikeTop[i];
					//e->destroy();
				}
			}
		}
	}
	//Lance death detection
	for (int i = 0; i < 2; i++) {
		int lance[i][4];
		lance[i][0] = p->l[i].s.center.y + p->l[i].hit.height + e->s.height;
		lance[i][1] = p->l[i].s.center.y - p->l[i].hit.height - e->s.height;
		lance[i][2] = p->l[i].s.center.x + p->l[i].hit.width + e->s.width;
		lance[i][3] = p->l[i].s.center.x - p->l[i].hit.width - e->s.width;
		if (e->s.center.y < lance[i][0] && e->s.center.y > lance[i][1]) {
			if (e->s.center.x < lance[i][2] && e->s.center.x > lance[i][3]) {
				//e->destroy();
			}
		}
	}
}

void charCollision(Game *game, Character *p, Enemy *e)
{
	int boxTop[5], boxBottom[5], boxLeft[5], boxRight[5];
	for (int i = 0; i < 5; i++) {
		Shape *s = &game->box[i];
		boxTop[i] = s->center.y + s->height + p->s.height;
		boxBottom[i] = s->center.y - s->height - p->s.height;
		boxLeft[i] = s->center.x - s->width - p->s.width;
		boxRight[i] = s->center.x + s->width + p->s.width;
		if (p->s.center.y < boxTop[i] && p->s.center.y > boxBottom[i]) {
			if (p->s.center.x > boxLeft[i]	&& p->s.center.x < boxRight[i]) {
				//Top collision detection
				if (p->s.center.y < boxTop[i]
						&& p->s.center.y > boxTop[i] - OFFSET
							&& p->s.center.x < boxRight[i] - OFFSET
								&& p->s.center.x > boxLeft[i] + OFFSET) {
					if (p->velocity.y < 0) {
						if (p->soundChk == true) {
							thump();
							p->soundChk = !p->soundChk;
						}
						p->s.center.y = boxTop[i];
						p->velocity.y = 0;
						p->jumpCurrent = 0;
						p->hurtJump = false;
					}
				}
				//Bottom collision detection
				if (p->s.center.y > boxBottom[i]
						&& p->s.center.y < boxBottom[i] + OFFSET
							&& p->s.center.x < boxRight[i] - OFFSET
								&& p->s.center.x > boxLeft[i] + OFFSET) {
					p->s.center.y = boxBottom[i];
					if (p->velocity.y > 0)
						p->velocity.y = 0;
				}
				//Right collision detection
				if (p->s.center.x < boxRight[i]
						&& p->s.center.x > s->center.x
							&& p->s.center.y < boxTop[i] - OFFSET
								&& p->s.center.y > boxBottom[i] + OFFSET) {
					p->s.center.x = boxRight[i];
					if (p->velocity.x < 0)
						p->velocity.x = 0;
				}
				//Left Collision detection
				if (p->s.center.x > boxLeft[i]
						&& p->s.center.x < s->center.x
							&& p->s.center.y < boxTop[i] - OFFSET
								&& p->s.center.y > boxBottom[i] + OFFSET) {
					p->s.center.x = boxLeft[i];
					if (p->velocity.x > 0)
						p->velocity.x = 0;
				}
			}
		}
	}
	int platTop[5], platBottom[5], platLeft[5], platRight[5];
	for (int i = 0; i < 5; i++) {
		Shape *s = &game->plat[i];
		platTop[i] = s->center.y + s->height + p->s.height;
		platBottom[i] = s->center.y - s->height - p->s.height;
		platLeft[i] = s->center.x - s->width - p->s.width;
		platRight[i] = s->center.x + s->width + p->s.width;
		if (p->s.center.y < platTop[i] && p->s.center.y > platBottom[i]) {
			if (p->s.center.x > platLeft[i]	&& p->s.center.x < platRight[i]) {
				//Top collision detection
				if (p->s.center.y < platTop[i]
					&& p->s.center.y > platTop[i] - OFFSET
						&& p->s.center.x < platRight[i]
							&& p->s.center.x > platLeft[i]
								&& p->velocity.y < 0) {
					if (p->soundChk == true) {
						thump();
						p->soundChk = !p->soundChk;
					}
					p->s.center.y = platTop[i];
					p->velocity.y = 0;
					p->jumpCurrent = 0;
				}
			}
		}
	}
	// weapon update
	for (int i = 0; i < 2; i++) {
		if (p->l[i].s.center.x > 0) {
			if (abs(p->l[i].initThrow.x - p->l[i].s.center.x)
					> p->s.width * 2) {
				p->l[i].s.center.x = -10;
				p->l[i].hit.center.x = p->l[i].s.center.x;
			}
		}
	}
	enemyHurt(game, p, e);
	charHurt(game, p, e);
}

void makeWeapon(Game *game, Character *p)
{
	for (int i = 0; i < 2; i++) {
		if (p->l[i].s.center.x < 0) {
			p->l[i].s.center.x = p->s.center.x;
			p->l[i].s.center.y = p->s.center.y;
			p->l[i].hit.center.x = p->l[i].initThrow.x = p->s.center.x;
			p->l[i].hit.center.y = p->l[i].s.center.y;
			p->l[i].hit.width = W_WIDTH;
			p->l[i].hit.height = W_HEIGHT;
			if (!p->isLeft)
				p->l[i].velocity.x = WALK*2;
			else
				p->l[i].velocity.x = -WALK*2;
		}
	}
}

void checkPause(Game *gm)
{
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_PAUSE) {
		h = 100.0;
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
		r.bot = gm->yres/2 + 80;
		r.left = gm->xres/2;
		r.center = 1;
		ggprint8b(&r, 16, c, "PAUSE SCREEN");
		r.center = 0;
		r.left = gm->xres/2 - 100;
		ggprint8b(&r, 16, c, "TAB -> Resume play");
		ggprint8b(&r, 16, c, "SHIFT -> Run");
		ggprint8b(&r, 16, c, "Right Arrow or A -> Walk right");
		ggprint8b(&r, 16, c, "Left Arrow or D -> Walk left");
		ggprint8b(&r, 16, c, "Up Arrow or W -> Jump");
		ggprint8b(&r, 16, c, "T - Enemy Unit Test");
	}
}

void checkStart(Game *gm)
{
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_STARTMENU) {
		h = 100.0;
		w = 200.0;
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0, 0.0, 1.0, 0.7);
		glTranslated(gm->xres/2, gm->yres/2, 0);
		glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, +h);
			glVertex2i(+w, +h);
			glVertex2i(+w, -h);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		r.bot = gm->yres/2 + 80;
		r.left = gm->xres/2;
		r.center = 1;
		ggprint8b(&r, 16, c, "START MENU");
		r.center = 0;
		r.left = gm->xres/2 - 100;
		ggprint8b(&r, 16, c, "P - Play");
		ggprint8b(&r, 16, c, "TAB - Pause");
	}
}

void checkGameOver(Game *gm)
{
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_GAMEOVER) {
		h = 100.0;
		w = 200.0;
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0, 1.0, 1.0, 0.7);
		glTranslated(gm->xres/2, gm->yres/2, 0);
		glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, +h);
			glVertex2i(+w, +h);
			glVertex2i(+w, -h);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		r.bot = gm->yres/2 + 80;
		r.left = gm->xres/2;
		r.center = 1;
		ggprint8b(&r, 16, c, "GAME OVER");
		r.center = 0;
		r.left = gm->xres/2 - 100;
		ggprint8b(&r, 16, c, "R - Restart");
	}
}
