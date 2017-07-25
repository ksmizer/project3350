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

void movement(Game *game, Character *p, PlayerState ps, char keys[])
{
	//Character movement check
	if (keys[XK_Left] || keys[XK_A] || keys[XK_a]) {
		p->isLeft = true;
		if (p->jumpCurrent > 0) {
			if (p->velocity.x > -WALK)
				p->velocity.x -= WALK / 10;
			if (keys[XK_Shift_L] || keys[XK_Shift_R]) {
				if (p->velocity.x > -WALK * 2)
					p->velocity.x -= WALK / 5;
			}
		}
		else {
			if (keys[XK_Shift_L] || keys[XK_Shift_R])
				p->velocity.x = -WALK * 2;
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
				if (p->velocity.x < WALK * 2)
					p->velocity.x += WALK / 5;
			}
		}
		else {
			if (keys[XK_Shift_L] || keys[XK_Shift_R])
				p->velocity.x = WALK * 2;
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

void charHurtUpdate(Game *game, Character *p, Enemy *e)
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
					spikes();
					death();
					game->state = STATE_GAMEOVER;
				}
				//Bottom collision detection
				if (p->s.center.y > spikeBottom[i]
						&& p->s.center.y < spikeBottom[i] + OFFSET
							&& p->s.center.x < spikeRight[i] - OFFSET
								&& p->s.center.x > spikeLeft[i] + OFFSET) {
					p->s.center.y = spikeBottom[i];
					p->velocity.y = 0;
					p->jumpCurrent = 2;
					spikes();
					death();
					game->state = STATE_GAMEOVER;
				}
				//Right collision detection
				if (p->s.center.x < spikeRight[i]
						&& p->s.center.x > s->center.x
							&& p->s.center.y < spikeTop[i] - OFFSET
								&& p->s.center.y > spikeBottom[i] + OFFSET) {
					p->s.center.y = spikeRight[i];
					p->velocity.y = 0;
					p->jumpCurrent = 2;
					spikes();
					death();
					game->state = STATE_GAMEOVER;
				}
				//Left Collision detection
				if (p->s.center.x > spikeLeft[i]
						&& p->s.center.x < s->center.x
							&& p->s.center.y < spikeTop[i] - OFFSET
								&& p->s.center.y > spikeBottom[i] + OFFSET) {
					p->s.center.y = spikeLeft[i];
					p->velocity.y = 0;
					p->jumpCurrent = 2;
					spikes();
					death();
					game->state = STATE_GAMEOVER;
				}
			}
		}
	}
	// Enemy collision
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
					e->velocity.y = 0;
					death();
				}
				//Bottom collision detection
				if (e->s.center.y > spikeBottom[i]
						&& e->s.center.y < spikeBottom[i] + OFFSET
							&& e->s.center.x < spikeRight[i] - OFFSET
								&& e->s.center.x > spikeLeft[i] + OFFSET) {
					e->s.center.y = spikeBottom[i];
					e->velocity.y = 0;
					death();
				}
				//Right collision detection
				if (e->s.center.x < spikeRight[i]
						&& e->s.center.x > s->center.x
							&& e->s.center.y < spikeTop[i] - OFFSET
								&& e->s.center.y > spikeBottom[i] + OFFSET) {
					e->s.center.y = spikeRight[i];
					e->velocity.y = 0;
					death();
				}
				//Left Collision detection
				if (e->s.center.x > spikeLeft[i]
						&& e->s.center.x < s->center.x
							&& e->s.center.y < spikeTop[i] - OFFSET
								&& e->s.center.y > spikeBottom[i] + OFFSET) {
					e->s.center.y = spikeLeft[i];
					e->velocity.y = 0;
					death();
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
				e->velocity.x = 0;
				e->killEnemy();
				//death();
			}
		}
	}
}

void charCollision(Game *game, Character *p, Enemy *e)
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
	// weapon update
	/*
	for (int i = 0; i < 2; i++) {
		if (p->l[i].s.center.x > 0) {
			if (abs(p->l[i].initThrow.x - p->l[i].s.center.x)
					> p->s.width * 2) {
				p->l[i].s.center.x = -10;
				p->l[i].hit.center.x = p->l[i].s.center.x;
				p->l[i].velocity.x = 0;
			}
		}
	}*/
	// save point collision check
	
	// player falling check
	if (p->velocity.y < -GRAVITY * 2) {
		if (p->jumpCurrent < 1) {
			p->jumpCurrent = 1;
		}
	}
	charHurt(game, p, e);
}

void enemyCollision(Game *game, Character *p, Enemy *e)
{
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
	enemyHurt(game, p, e);
}

void makeWeapon(Game *game, Character *p)
{
	for (int i = 0; i < 2; i++) {
		if (p->l[i].s.center.x < 0 || p->l[i].s.center.x > W_WIDTH) {
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

void savePointCheck(Character *p, SavePoint *sp)
{
	int x = sp->getX();
	int y = sp->getY();
	int charTop, charBottom, charLeft, charRight;
	charTop = y + p->s.height;
	charBottom = y - p->s.height;
	charLeft = x - p->s.width;
	charRight = x + p->s.width;
	if (p->s.center.y < charTop && p->s.center.y > charBottom) {
		if (p->s.center.x > charLeft && p->s.center.x < charRight) {
			sp->enable();
		}
	}
}
/*
void buttonInit(Game *gm)
{
	if (gm->state == STATE_STARTMENU) {
		int nbuttons = gm->nbuttons = 0;
		//===========//
		//Play button//
		//===========//
		gm->button[nbuttons].r.width = 100;
		gm->button[nbuttons].r.height = 50;
		gm->button[nbuttons].r.left =
			gm->xres / 2 - gm->button[nbuttons].r.width / 2;
		gm->button[nbuttons].r.bot =
			gm->yres / 2 - gm->button[nbuttons].r.width / 2;
		gm->button[nbuttons].r.right =
			gm->button[nbuttons].r.left + gm->button[nbuttons].r.width;
		gm->button[nbuttons].r.top =
			gm->button[nbuttons].r.bot + gm->button[nbuttons].r.height;
		gm->button[nbuttons].r.centerx =
			(gm->button[nbuttons].r.left + gm->button[nbuttons].r.right) / 2;
		gm->button[nbuttons].r.centery =
			(gm->button[nbuttons].r.bot + gm->button[nbuttons].r.top) / 2;
		strcpy(gm->button[nbuttons].text, "Play");
		gm->button[nbuttons].down = 0;
		gm->button[nbuttons].click = 0;
		gm->button[nbuttons].color[0] = 0.4f;
		gm->button[nbuttons].color[1] = 0.4f;
		gm->button[nbuttons].color[2] = 0.7f;
		gm->button[nbuttons].dcolor[0] = gm->button[nbuttons].color[0] * 0.5f;
		gm->button[nbuttons].dcolor[1] = gm->button[nbuttons].color[1] * 0.5f;
		gm->button[nbuttons].dcolor[2] = gm->button[nbuttons].color[2] * 0.5f;
		gm->button[nbuttons].text_color = 0x00ffffff;
		gm->nbuttons++;
		//===============//
		//Controls button//
		//===============//
		gm->button[nbuttons].r.width = 100;
		gm->button[nbuttons].r.height = 50;
		gm->button[nbuttons].r.left =
			gm->xres / 2 - gm->button[nbuttons].r.width / 2;
		gm->button[nbuttons].r.bot =	gm->yres / 2 -
			gm->button[nbuttons].r.height - gm->button[nbuttons].r.width / 2;
		gm->button[nbuttons].r.right =
			gm->button[nbuttons].r.left + gm->button[nbuttons].r.width;
		gm->button[nbuttons].r.top =
			gm->button[nbuttons].r.bot + gm->button[nbuttons].r.height;
		gm->button[nbuttons].r.centerx =
			(gm->button[nbuttons].r.left + gm->button[nbuttons].r.right) / 2;
		gm->button[nbuttons].r.centery =
			(gm->button[nbuttons].r.bot + gm->button[nbuttons].r.top) / 2;
		strcpy(gm->button[nbuttons].text, "Controls");
		gm->button[nbuttons].down = 0;
		gm->button[nbuttons].click = 0;
		gm->button[nbuttons].color[0] = 0.4f;
		gm->button[nbuttons].color[1] = 0.4f;
		gm->button[nbuttons].color[2] = 0.7f;
		gm->button[nbuttons].dcolor[0] = gm->button[nbuttons].color[0] * 0.5f;
		gm->button[nbuttons].dcolor[1] = gm->button[nbuttons].color[1] * 0.5f;
		gm->button[nbuttons].dcolor[2] = gm->button[nbuttons].color[2] * 0.5f;
		gm->button[nbuttons].text_color = 0x00ffffff;
		gm->nbuttons++;
		//===========//
		//Exit button//
		//===========//
		gm->button[nbuttons].r.width = 100;
		gm->button[nbuttons].r.height = 50;
		gm->button[nbuttons].r.left =
			gm->xres / 2 - gm->button[nbuttons].r.width / 2;
		gm->button[nbuttons].r.bot =	gm->yres / 2 -
			gm->button[nbuttons].r.height*2 - gm->button[nbuttons].r.width / 2;
		gm->button[nbuttons].r.right =
			gm->button[nbuttons].r.left + gm->button[nbuttons].r.width;
		gm->button[nbuttons].r.top =
			gm->button[nbuttons].r.bot + gm->button[nbuttons].r.height;
		gm->button[nbuttons].r.centerx =
			(gm->button[nbuttons].r.left + gm->button[nbuttons].r.right) / 2;
		gm->button[nbuttons].r.centery =
			(gm->button[nbuttons].r.bot + gm->button[nbuttons].r.top) / 2;
		strcpy(gm->button[nbuttons].text, "Exit");
		gm->button[nbuttons].down = 0;
		gm->button[nbuttons].click = 0;
		gm->button[nbuttons].color[0] = 0.4f;
		gm->button[nbuttons].color[1] = 0.4f;
		gm->button[nbuttons].color[2] = 0.7f;
		gm->button[nbuttons].dcolor[0] = gm->button[nbuttons].color[0] * 0.5f;
		gm->button[nbuttons].dcolor[1] = gm->button[nbuttons].color[1] * 0.5f;
		gm->button[nbuttons].dcolor[2] = gm->button[nbuttons].color[2] * 0.5f;
		gm->button[nbuttons].text_color = 0x00ffffff;
		gm->nbuttons++;
	}
}
*/
void mouseClick(Game *gm, int ibutton, int action, int x, int y)
{
	if (action == 1) {
		for (int i = 0; i < gm->nbuttons; i++) {
			if (gm->button[i].over) {
				gm->button[i].down = 1;
				gm->button[i].click = 1;
				if (i == 0) {
					//Pressed Play button
					gm->state = STATE_GAMEPLAY;
				}
				if (i == 1) {
					//Pressed Controls button
					gm->state = STATE_CONTROLS;
				}
				if (i == 2) {
					//Pressed Quit button
					gm->done = 1;
				}
			}
		}
	}
	if (action == 2) {
		for (int i = 0; i < gm->nbuttons; i++) {
			gm->button[i].down = 0;
			gm->button[i].click = 0;
		}
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
		ggprint8b(&r, 16, c, "CONTROLS");
		r.center = 0;
		r.left = gm->xres/2 - 100;
		ggprint8b(&r, 16, c, "TAB -> Resume play");
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
	Flt h, w;
	Rect r;
	int c = 0xffffffff;
	if (gm->state == STATE_STARTMENU) {
		h = 100.0;
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
