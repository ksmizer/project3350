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
//checkGameOver:
// check if state is GAMEOVER & create Game Over screen
//
//savePointCheck:
// check collision with save point


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
#include <algorithm>
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
extern void explosion();
extern void timer(int mode);

void makeWeapon(Game *game, Character *p);
void start(Game *game);
void loading(Game *game);

void movement(Game *game, Character *p, PlayerState ps, char keys[])
{
	//Character movement check
	if (keys[XK_Left] || keys[XK_A] || keys[XK_a]) {
		p->isLeft = true;
		if (p->jumpCurrent > 0) {
			if (p->velocity.x > -WALK)
				p->velocity.x -= WALK / 10;
			if (keys[XK_Shift_L] || keys[XK_Shift_R]) {
				if (p->velocity.x > -RUN && p->upgrade2)
					p->velocity.x -= WALK / 5;
			}
		}
		else {
			if (keys[XK_Shift_L] || keys[XK_Shift_R]) {
				if(p->upgrade2)
					p->velocity.x = -RUN;
				else
					p->velocity.x = -WALK;
			}
			else
				p->velocity.x = -WALK;
		}
	}
	if (keys[XK_Right] || keys[XK_D] || keys[XK_d]) {
		p->isLeft = false;
		if (p->jumpCurrent > 0) {
			if (p->velocity.x < WALK)
				p->velocity.x += WALK / 10;
			if (keys[XK_Shift_L] || keys[XK_Shift_R]) {
				if (p->velocity.x < RUN && p->upgrade2)
					p->velocity.x += WALK / 5;
			}
		}
		else {
			if (keys[XK_Shift_L] || keys[XK_Shift_R])
			{
				if (p->upgrade2)
					p->velocity.x = RUN;
				else
					p->velocity.x = WALK;
			}
			else
				p->velocity.x = WALK;
		}
	}
	if (keys[XK_Up] ||  keys[XK_W] || keys[XK_w]) {
		if (p->jumpCurrent < p->jumpMax && p->velocity.y <= (3/4 * JUMP)) {
			p->velocity.y = JUMP;
			++p->jumpCurrent;
			p->hurtJump = true;
			jump();
			p->soundChk = !p->soundChk;
		}
		if (game->state == STATE_STARTMENU) {
			if (game->button.center.x >= game->yres+0.6) {
				game->button.center.x += 100;
			}
		}
	}
	if (keys[XK_Down] || keys[XK_S] || keys[XK_s]) {
		if (game->state == STATE_STARTMENU) {
			if (game->button.center.x <= game->yres+0.4) {
				game->button.center.x -= 100;
			}
		}
	}
	if (keys[XK_Left] + keys[XK_Right] + keys[XK_a]
			+ keys[XK_A] + keys[XK_d] + keys[XK_D] == 0) {
		if (p->velocity.x <= WALK / 10 && p->velocity.x >= -WALK / 10) {
			p->velocity.x = 0;
		}
		if (p->velocity.x > 0) {
			p->velocity.x -= WALK / 10;
		}
		if (p->velocity.x < 0) {
			p->velocity.x += WALK / 10;
		}
	}
	if (keys[XK_j] || keys[XK_J]) {
		makeWeapon(game, p);
	}
	return;
}

void charHurtUpdate(Game *game, Character *p)
{
	// Character hurtbox shift for jumping
	if (p->hurtJump == true) {
		p->hurt.height = p->s.height - 15;
		//p->hurt.width = p->s.width + 5;
	}
	if (p->hurtJump == false) {
		p->hurt.height = p->s.height;
		//p->hurt.width = p->s.width;
	}		
}

void charHurt(Game *game, Character *p, vector<Enemy> &enemies)
{
		
    	//Enemy *e = &enemies.at(0);
	charHurtUpdate(game, p);
	// Spike death detection
	int spikeTop[5], spikeBottom[5], spikeLeft[5], spikeRight[5];
	for (int i = 0; i < 5; i++) {
		Shape *s = &game->spike[i];
		spikeTop[i] = s->center.y + s->height + p->s.height;
		spikeBottom[i] = s->center.y - s->height - p->s.height;
		spikeLeft[i] = s->center.x - s->width - p->s.width;
		spikeRight[i] = s->center.x + s->width + p->s.width;
		if (p->s.center.y < spikeTop[i] - 10 && p->s.center.y > spikeBottom[i] + 15) {
			if (p->s.center.x > spikeLeft[i] + 10 && p->s.center.x < spikeRight[i] - 10) {
				p->velocity.y = 0;
				p->jumpCurrent = 2;
				spikes();
				death();
				game->state = STATE_GAMEOVER;
			}
		}
	}
	// Enemy collision
	for (unsigned int i = 0; i < enemies.size(); i++) {
	    	Enemy *e = &enemies.at(i);
		int enemyHit[4];
		enemyHit[0] = e->s.center.y + e->hitbox.height + p->hurt.radius;
		enemyHit[1] = e->s.center.y - e->hitbox.height - p->hurt.radius;
		enemyHit[2] = e->s.center.x + e->hitbox.width + p->hurt.radius;
		enemyHit[3] = e->s.center.x - e->hitbox.width - p->hurt.radius;
		if (p->s.center.y < enemyHit[0] && p->s.center.y > enemyHit[1]) {
			if (p->s.center.x < enemyHit[2] && p->s.center.x > enemyHit[3]) {
				p->velocity.y = 0;
				p->jumpMax = 0;
				death();
				game->state = STATE_GAMEOVER;
			}
		}
	}
}

bool enemyHurt(Game *game, Character *p, Enemy enemy)
{
    	bool kill = false;
	Enemy *e = &enemy;
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
				e->s.center.y = spikeTop[i];
				e->velocity.y = 0;
				death();
			}
		}
	}
	//Lance death detection
	for (int i = 0; i < 1; i++) {
		int lance[i][4];
		lance[i][0] = p->l[i].s.center.y + p->l[i].hit.height + e->s.height;
		lance[i][1] = p->l[i].s.center.y - p->l[i].hit.height - e->s.height;
		lance[i][2] = p->l[i].s.center.x + p->l[i].hit.width + e->s.width;
		lance[i][3] = p->l[i].s.center.x - p->l[i].hit.width - e->s.width;
		if (e->s.center.y < lance[i][0] && e->s.center.y > lance[i][1]) {
			if (e->s.center.x < lance[i][2] && e->s.center.x > lance[i][3]) {
				e->velocity.x = 0;
				e->killEnemy();
				kill = true;
				p->l[i].s.center.x = -50;
				p->l[i].velocity.x = 0;
			}
		}
	}
	return kill;
}

void charCollision(Game *game, Character *p, vector<Enemy> &enemies)
{
	int boxTop[7], boxBottom[7], boxLeft[7], boxRight[7];
	for (int i = 0; i < 7; i++) {
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
						if (p->velocity.x <= WALK / 5 &&
								p->velocity.x >= -WALK / 5) {
							p->velocity.x = 0;
						}
						if (p->velocity.x > 0) {
							p->velocity.x -= WALK / 5;
						}
						if (p->velocity.x < 0) {
							p->velocity.x += WALK / 5;
						}
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
	int platTop[20], platBottom[20], platLeft[20], platRight[20];
	for (int i = 0; i < 20; i++) {
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
	// weapon box collision update
	for (unsigned int i = 0; i < 10; i++) {
		for (int j = 0; j < 1; j++) {
			Shape *s = &game->box[i];
			boxTop[i] = s->center.y + s->height + W_HEIGHT;
			boxBottom[i] = s->center.y - s->height - W_HEIGHT;
			boxLeft[i] = s->center.x - s->width - W_WIDTH;
			boxRight[i] = s->center.x + s->width + W_WIDTH;
			if (p->l[j].s.center.x > 0) {
				if (p->l[j].s.center.y < boxTop[i]
						&& p->l[j].s.center.y > boxBottom[i]) {
					if (p->l[j].s.center.x > boxLeft[i]
							&& p->l[j].s.center.x < boxRight[i]) {
						//Right collision detection
						if (p->l[j].s.center.x < boxRight[i]
								&& p->l[j].s.center.x > s->center.x) {
							p->l[j].velocity.x = 0;
							p->l[j].s.center.x = -50;
						}
						//Left Collision detection
						if (p->l[j].s.center.x > boxLeft[i]
								&& p->l[j].s.center.x < s->center.x) {
							p->l[j].velocity.x = 0;
							p->l[j].s.center.x = -50;
						}
					}
				}
			}
		}
	}
	// save point collision check
	
	// player falling check
	if (p->velocity.y < -GRAVITY * 2) {
		if (p->jumpCurrent < 1) {
			p->jumpCurrent = 1;
		}
	}
	charHurt(game, p, enemies);
}

void enemyCollision(Game *game, Character *p, vector<Enemy> &enemies)
{
    for (unsigned int j = 0; j < enemies.size(); j++) {
	Enemy *e = &enemies.at(j);
	int boxTop[7], boxBottom[7], boxLeft[7], boxRight[7];
	for (int i = 0; i < 7; i++) {
		Shape *s = &game->box[i];
		boxTop[i] = s->center.y + s->height + e->s.height;
		boxBottom[i] = s->center.y - s->height - e->s.height;
		boxLeft[i] = s->center.x - s->width - e->s.width;
		boxRight[i] = s->center.x + s->width + e->s.width;
		if (e->s.center.y < boxTop[i] && e->s.center.y > boxBottom[i]) {
			if (e->s.center.x > boxLeft[i]	&& e->s.center.x < boxRight[i]) {
				//Top collision detection
				if (e->s.center.y < boxTop[i]
						&& e->s.center.y > boxTop[i] - OFFSET
							&& e->s.center.x < boxRight[i] - OFFSET
								&& e->s.center.x > boxLeft[i] + OFFSET) {
					if (e->velocity.y < 0) {
						thump();
						e->s.center.y = boxTop[i];
						e->velocity.y = 0;
					}
				}
				//Bottom collision detection
				if (e->s.center.y > boxBottom[i]
						&& e->s.center.y < boxBottom[i] + OFFSET
							&& e->s.center.x < boxRight[i] - OFFSET
								&& e->s.center.x > boxLeft[i] + OFFSET) {
					e->s.center.y = boxBottom[i];
					if (e->velocity.y > 0)
						e->velocity.y = 0;
				}
				//Right collision detection
				if (e->s.center.x < boxRight[i]
						&& e->s.center.x > s->center.x
							&& e->s.center.y < boxTop[i] - OFFSET
								&& e->s.center.y > boxBottom[i] + OFFSET) {
					e->s.center.x = boxRight[i];
					if (e->velocity.x < 0)
						e->flipDirection();
				}
				//Left Collision detection
				if (e->s.center.x > boxLeft[i]
						&& e->s.center.x < s->center.x
							&& e->s.center.y < boxTop[i] - OFFSET
								&& e->s.center.y > boxBottom[i] + OFFSET) {
					e->s.center.x = boxLeft[i];
					if (e->velocity.x > 0)
						e->flipDirection();
				}
			}
		}
	}
	int platTop[10], platBottom[10], platLeft[10], platRight[10];
	for (int i = 0; i < 10; i++) {
		Shape *s = &game->plat[i];
		platTop[i] = s->center.y + s->height + e->s.height;
		platBottom[i] = s->center.y - s->height - e->s.height;
		platLeft[i] = s->center.x - s->width - e->s.width;
		platRight[i] = s->center.x + s->width + e->s.width;
		if (e->s.center.y < platTop[i] && e->s.center.y > platBottom[i]) {
			if (e->s.center.x > platLeft[i]	&& e->s.center.x < platRight[i]) {
				//Top collision detection
				if (e->s.center.y < platTop[i]
					&& e->s.center.y > platTop[i] - OFFSET
						&& e->s.center.x <= platRight[i]
							&& e->s.center.x >= platLeft[i]
								&& e->velocity.y < 0) {
					thump();
					e->s.center.y = platTop[i];
					e->velocity.y = 0;
					if (e->s.center.x == platRight[i] ||
							e->s.center.x == platLeft[i]) {
						e->velocity.x = -e->velocity.x;
					}
				}
			}
		}
	}

	if (enemyHurt(game, p, enemies.at(j))) {
	    enemies.erase(enemies.begin() + j);
	    break;
	}
    }
}

void makeWeapon(Game *game, Character *p)
{
	for (int i = 0; i < 2; i++) {
		if (p->l[i].s.center.x < 0 || p->l[i].s.center.x > game->xres) {
			if (p->l[i].thrown == false) {
				throw_spear();
				p->l[i].s.center.x = p->s.center.x;
				p->l[i].s.center.y = p->s.center.y + p->s.height*1/3;
				p->l[i].hit.center.x = p->l[i].initThrow.x = p->s.center.x;
				p->l[i].hit.center.y = p->l[i].s.center.y;
				p->l[i].hit.width = W_WIDTH;
				p->l[i].hit.height = W_HEIGHT;
				if (!p->isLeft)
					p->l[i].velocity.x = WALK*4;
				else
					p->l[i].velocity.x = -WALK*4;
			}
		}
	}
}

void savePointCheck(Character *p, vector<SavePoint> &sp)
{
	for (unsigned int i = 0; i < sp.size(); i++) {
		if (sp.at(i).animations.at(0).isEnabled() ||
			sp.at(i).animations.at(1).isEnabled())
		{
			int x = sp.at(i).getX();
			int y = sp.at(i).getY();
			int charTop, charBottom, charLeft, charRight;
			charTop = y + p->s.height;
			charBottom = y - p->s.height;
			charLeft = x - p->s.width;
			charRight = x + p->s.width;
			if (p->s.center.y < charTop && p->s.center.y > charBottom) {
				if (p->s.center.x > charLeft && p->s.center.x < charRight) {
					sp.at(i).enable();
					for (unsigned int j = 0; j < sp.size(); j++)
						if (j != i)
							sp.at(j).disable();
				}
			}
		}
	}
}

void selection(Game *gm, int x, int y, int h, int w)
{
	Vec *c = &gm->button.center;
	c->x = x;
	c->y = y;
	h = gm->button.height;
	w = gm->button.width;
	if (gm->state == STATE_STARTMENU) {
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0, 1.0, 1.0, 0.5);
		glBegin(GL_QUADS);
			glVertex2i(c->x-w, c->y-h);
			glVertex2i(c->x-w, c->y+h);
			glVertex2i(c->x+w, c->y+h);
			glVertex2i(c->x+w, c->y-h);
		glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
	}
}

void checkControl(Game *gm)
{
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_CONTROLS) {
		h = 100.0;
		w = 200.0;
		glPushMatrix();
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0, 0.0, 0.0, 0.9);
		glTranslated(gm->xres/2, gm->yres/2, 0);
		glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w, +h);
			glVertex2i(+w, +h);
			glVertex2i(+w, -h);
		glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		r.bot = gm->yres/2 + 80;
		r.left = gm->xres/2;
		r.center = 1;
		ggprint8b(&r, 16, c, "CONTROLS");
		r.center = 0;
		r.left = gm->xres/2 - 100;
		ggprint8b(&r, 16, c, "M -> Return to Menu");
		ggprint8b(&r, 16, c, "TAB -> Pause / Resume Play");
		ggprint8b(&r, 16, c, "SHIFT -> Run");
		ggprint8b(&r, 16, c, "Right Arrow or A -> Walk right");
		ggprint8b(&r, 16, c, "Left Arrow or D -> Walk left");
		ggprint8b(&r, 16, c, "Up Arrow or W -> Jump");
		ggprint8b(&r, 16, c, "J - Throw Spear");
		ggprint8b(&r, 16, c, "V - Enemy Move Test");
		ggprint8b(&r, 16, c, "T - Enemy Unit Test");
		ggprint8b(&r, 16, c, "I - Toggle Save Point");
		ggprint8b(&r, 16, c, "O - Test Save Point");
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
		ggprint8b(&r, 16, c, "J - Throw Spear");
		ggprint8b(&r, 16, c, "T - Enemy Unit Test");
		ggprint8b(&r, 16, c, "V - Enemy Move Test");
		ggprint8b(&r, 16, c, "I - Toggle Save Point");
		ggprint8b(&r, 16, c, "O - Test Save Point");
	}
}

void checkStart(Game *gm)
{
	if (gm->state == STATE_STARTMENU) {
		start(gm);
	}
}

void checkLoading(Game *gm)
{
	if (gm->state == STATE_NONE) {
		loading(gm);
		gm->state = STATE_LOADING;
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

void loadBackground(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/background.png tmp.ppm");
	gm->tex.background = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
	glGenTextures(1, &p->tex.backTexture);
	int w = gm->tex.background->width;
	int h = gm->tex.background->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
					GL_RGB, GL_UNSIGNED_BYTE, gm->tex.background->data);
	unlink("./tmp.ppm");
	gm->tex.xb[0] = 0.0;
	gm->tex.xb[1] = 1.0;
	gm->tex.yb[0] = 0.0;
	gm->tex.yb[1] = 1.0;
}

void loadGameover(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/died.png tmp.ppm");
	gm->tex.died = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
	glGenTextures(1, &p->tex.diedTexture);
	int w = gm->tex.died->width;
	int h = gm->tex.died->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.diedTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
					GL_RGB, GL_UNSIGNED_BYTE, gm->tex.died->data);
	unlink("./tmp.ppm");
	gm->tex.xd[0] = 0.0;
	gm->tex.xd[1] = 1.0;
	gm->tex.yd[0] = 0.0;
	gm->tex.yd[1] = 1.0;
	glPushMatrix();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.diedTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xd[0], gm->tex.yd[1]);
			glVertex2i(0, 0); 
		glTexCoord2f(gm->tex.xd[0], gm->tex.yd[0]);
			glVertex2i(0, gm->yres);
		glTexCoord2f(gm->tex.xd[1], gm->tex.yd[0]);
			glVertex2i(gm->xres, gm->yres);
		glTexCoord2f(gm->tex.xd[1], gm->tex.yd[1]);
			glVertex2i(gm->xres, 0); 
	glEnd();
	glPopMatrix();
}

void loadLoading(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/loading.png tmp.ppm");
	gm->tex.loading = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
	glGenTextures(1, &p->tex.loadTexture);
	int w = gm->tex.loading->width;
	int h = gm->tex.loading->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.loadTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
					GL_RGB, GL_UNSIGNED_BYTE, gm->tex.loading->data);
	unlink("./tmp.ppm");
	gm->tex.xl[0] = 0.0;
	gm->tex.xl[1] = 1.0;
	gm->tex.yl[0] = 0.0;
	gm->tex.yl[1] = 1.0;
}

void loadStart(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/descape.png tmp.ppm");
	gm->tex.start = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
	glGenTextures(1, &p->tex.startTexture);
	int w = gm->tex.start->width;
	int h = gm->tex.start->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.startTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
					GL_RGB, GL_UNSIGNED_BYTE, gm->tex.start->data);
	unlink("./tmp.ppm");
	gm->tex.xS[0] = 0.0;
	gm->tex.xS[1] = 1.0;
	gm->tex.yS[0] = 0.0;
	gm->tex.yS[1] = 1.0;
}

void loadPlatforms(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/platform.png tmp.ppm");
	gm->tex.platform = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
	glGenTextures(1, &p->tex.platTexture);
	int w = gm->tex.platform->width;
	int h = gm->tex.platform->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.platTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, gm->tex.platform->data);
	unlink("./tmp.ppm");
	gm->tex.xp[0] = 0.0;
	gm->tex.xp[1] = 1.0;
	gm->tex.yp[0] = 0.0;
	gm->tex.yp[1] = 1.0;
}

void loadBoxes(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/box.png tmp.ppm");
	gm->tex.box = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
	glGenTextures(1, &p->tex.boxTexture);
	int w = gm->tex.box->width;
	int h = gm->tex.box->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.boxTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, gm->tex.box->data);
	unlink("./tmp.ppm");
	gm->tex.xB[0] = 0.0;
	gm->tex.xB[1] = 1.0;
	gm->tex.yB[0] = 0.0;
	gm->tex.yB[1] = 1.0;
}

void loadSpikes(Game *gm)
{
	Game *p = gm;
	//load the images file into a ppm structure.
	system("convert images/spike.png tmp.ppm");
	gm->tex.spike = ppm6GetImage("./tmp.ppm");
	//create opengl texture elements
	glGenTextures(1, &p->tex.spikeTexture);
	int w = gm->tex.spike->width;
	int h = gm->tex.spike->height;
	glBindTexture(GL_TEXTURE_2D, gm->tex.spikeTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	gm->tex.spike->data = buildAlphaData(gm->tex.spike);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, gm->tex.spike->data);
	unlink("./tmp.ppm");
	gm->tex.xs[0] = 0.0;
	gm->tex.xs[1] = 1.0;
	gm->tex.ys[0] = 0.0;
	gm->tex.ys[1] = 1.0;
}

void start(Game *gm)
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.startTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xS[0], gm->tex.yS[1]);
			glVertex2i(0, 0); 
		glTexCoord2f(gm->tex.xS[0], gm->tex.yS[0]);
			glVertex2i(0, gm->yres);
		glTexCoord2f(gm->tex.xS[1], gm->tex.yS[0]);
			glVertex2i(gm->xres, gm->yres);
		glTexCoord2f(gm->tex.xS[1], gm->tex.yS[1]);
			glVertex2i(gm->xres, 0); 
	glEnd();
	glPopMatrix();
}

void loading(Game *gm)
{
	glPushMatrix();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.loadTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xl[0], gm->tex.yl[1]);
			glVertex2i(0, 0); 
		glTexCoord2f(gm->tex.xl[0], gm->tex.yl[0]);
			glVertex2i(0, gm->yres);
		glTexCoord2f(gm->tex.xl[1], gm->tex.yl[0]);
			glVertex2i(gm->xres, gm->yres);
		glTexCoord2f(gm->tex.xl[1], gm->tex.yl[1]);
			glVertex2i(gm->xres, 0); 
	glEnd();
	glPopMatrix();
}

void background(Game *gm)
{
	glPushMatrix();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.backTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(gm->tex.xb[0], gm->tex.yb[1]);
			glVertex2i(0, 0); 
		glTexCoord2f(gm->tex.xb[0], gm->tex.yb[0]);
			glVertex2i(0, gm->yres);
		glTexCoord2f(gm->tex.xb[1], gm->tex.yb[0]);
			glVertex2i(gm->xres, gm->yres);
		glTexCoord2f(gm->tex.xb[1], gm->tex.yb[1]);
			glVertex2i(gm->xres, 0); 
	glEnd();
	glPopMatrix();
}

void prepPlat(Game *gm)
{
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.platTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
}

void prepBox(Game *gm)
{
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.boxTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
}

void prepSpike(Game *gm)
{
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, gm->tex.spikeTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
}

void mouseClick(Game *gm, int action, int x, int y)
{
	if (action == 1) {
		if (gm->state == STATE_STARTMENU) {
			if (y < gm->yres*0.3 && y > gm->yres*0.2) {
				if (x > gm->xres*0.1 && x < gm->xres*0.23) {
					gm->state = STATE_NONE;
					loadLoading(gm);
				}
			}
			if (y < gm->yres*0.45 && y > gm->yres*0.37) {
				if (x > gm->xres*0.08 && x < gm->xres*0.26) {
					gm->state = STATE_CONTROLS;
					checkControl(gm);
				}
			}
			if (y < gm->yres*0.63 && y > gm->yres*.55) {
				if (x > gm->xres*0.11 && x < gm->xres*0.22) {
					gm->done = 1;
				}
			}
		}
		if (gm->state == STATE_GAMEOVER) {
			if (y < gm->yres*0.3 && y > gm->yres*.2) {
				if (x > gm->xres*0.45 && x < gm->xres*0.55) {
					gm->state = STATE_GAMEPLAY;
				}
			}
		}
	}
	if (action == 2) {
	}
}
