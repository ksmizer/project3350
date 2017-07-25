//cs3350 Spring 2017 Final Project
//author: Group Dungeon Escape
//date: 6/25/17
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "kyleS.h"
#include "derrickA.h"

#include "codyG.h"
#include "ppm.h"
#include "timers.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//Function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_mouse(XEvent *e);
void check_keys(XEvent *e);
void makeCharacter(Game *game, int x, int y);
void physics(Game *game, PlayerState ps);
void render(Game *game);

//Extern function calls
extern void movement(Game *game, Character *p, PlayerState ps, char keys[]);
extern void charCollision(Game *game, Character *p, vector<Enemy> &enemies);
extern void enemyCollision(Game *game, Character *p, vector<Enemy> &enemies);
extern void savePointCheck(Character *p, SavePoint *sp);
//extern void buttonInit(Game *game);
extern void loadBoxes(Game *game);
extern void prepBox(Game *game);
extern void loadBackground(Game *game);
extern void background(Game *game);
extern void loadPlatforms(Game *game);
extern void prepPlat(Game *game);
extern void platBind(Game *game);
extern void checkPause(Game *game);
extern void checkControl(Game *game);
extern void checkStart(Game *game);
extern void checkGameOver(Game *game);
extern void charHurt(Game *game, Character *p);
extern void mouseClick(Game *game, int ibutton, int action, int x, int y);
extern void setFrame(Game *g);
extern void setLRDoor(Game *g);
extern void levelText(Game *game, Level *lev);
extern void levelDrawBox(Game *g);
extern void setLevelSwitch(Game *g, Level *lev);
extern void nextLevel(Game *g, Level *lev);
extern void previousLevel(Game *g, Level *lev);
extern void gameOverLevelRestart(Game *game, Level *lev);
extern void setLeftDoor(Game *g);
extern void setRightDoor(Game *g);
extern void loadLevel(Game *g, Level *lev);
extern void thump();
extern void initialize_sound();
extern void finish_sound();
extern void background_music();
extern void death();
extern void loadLevel(Game *g);
extern void initialize_sound();
extern void finish_sound();
extern void background_music();
extern void currentTimer(int mode);
extern void totalTimer(int mode);
extern void initializeTime();
extern void resetTime();
extern void countDeath();
extern void outputScore(Game *game);
extern void outputCurrentScore(Game *game);

//declare player state
PlayerState playerState;

//declare game object
Game gm;

//declare level object
Level lev;

//declare button object
Button button[MAXBUTTONS];

//test enemy
vector<Enemy> enemies;
vector <SavePoint> savePoints;

//spears
Spear s1, s2;

//zombie sprites
//vector<SpriteAnimation> zombieAnimations;

//sprite testing
SpriteAnimation runAnimation((char*)"player.png", 1, 12, 12, 1, 6, 
	46, 50, 0.1, true); 
SpriteAnimation idleAnimation((char*)"player.png", 1, 12, 12, 0, 0, 
	46, 50, 0.1, true);
SpriteAnimation jumpAnimation((char*)"player.png", 1, 12, 12, 8, 8,
	46, 50, 0.1, true);
SpriteAnimation attackAnimation((char*)"player.png", 1, 12, 12, 8, 10,
	46, 50, 0.1, false);
//SpriteAnimation zombieAnimation((char*)"zombie.png", 1, 5, 5, 0, 3,
	//27, 40, 0.1, true);

int main(void)
{
	//initialize enemies
	initializeTime();
	Enemy testEnemy(0, 27, 40, 400, 48, 15, 40, 0, 0, 1, 0, 0, 1200, false);
	enemies.push_back(testEnemy);
		//initialize sprites
	for (unsigned int i = 0; i < enemies.size(); i++) 
		enemies.at(i).initAnimations();
	s1.initAnimations();
	s2.initAnimations();
	
	SavePoint sp1(200, 59, false);
	savePoints.push_back(sp1);
	savePoints.at(0).initAnimations();
	#ifdef USE_OPENAL_SOUND
	initialize_sound();
	background_music();
	srand(time(NULL));
	initXWindows();
	init_opengl();
	gm.n = 0; 
	
	//builds solid frame around level with box array
	//setFrame(&gm);
	
	//builds frame around level with left door
	setRightDoor(&gm);	

	//set ups spikes and plats for level 2
	//setLevel2(&gm, &lev);

	//play background_music
	background_music();
	
	//start animation
	while (!gm.done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_mouse(&e);
			check_keys(&e);
		}
		if (gm.state == STATE_GAMEPLAY)
			physics(&gm, playerState);

		render(&gm);
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	finish_sound();
	#endif
	return 0;
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "3350 Final Project");
}

void cleanupXWindows(void)
{
	//do not change
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void initXWindows(void)
{
	//do not change
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w=WINDOW_WIDTH, h=WINDOW_HEIGHT;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask | PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
		InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
	//Set the screen background color
	glClearColor(0.1, 0.1, 0.1, 1.0);
	//Allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	
	//Ppm textures
	loadBackground(&gm);
	loadPlatforms(&gm);

	//Sprites
	runAnimation.convertToPpm();
	runAnimation.createTexture();
	idleAnimation.convertToPpm();
	idleAnimation.createTexture();
	jumpAnimation.convertToPpm();
	jumpAnimation.createTexture();
	attackAnimation.convertToPpm();
	attackAnimation.createTexture();
	s1.sprite.convertToPpm();
	s1.sprite.createTexture();
	s2.sprite.convertToPpm();
	s2.sprite.createTexture();
	for (unsigned int i = 0; i < enemies.size(); i++) {
		for (unsigned int j = 0; j < enemies.at(i).animations.size(); j++) {
			enemies.at(i).animations.at(j).convertToPpm();
			enemies.at(i).animations.at(j).createTexture();
		}
	}
	for (unsigned int j = 0; j < savePoints.size(); j++) {
		for (unsigned int i = 0; i < 2; i++) {
			savePoints[j].animations.at(i).convertToPpm();
			savePoints[j].animations.at(i).createTexture();
		}
	}
}

void makeCharacter(Game *game, int x, int y)
{
	resetTime();
	//position of character
	Character *p = &game->character;
	
	p->s.center.x = p->hurt.center.x = x;
	p->s.center.y = p->hurt.center.y = y;
	for (unsigned int i = 0; i < savePoints.size(); i++) {
		if (savePoints.at(i).checkIsEnabled())
			movePlayer(game->character, savePoints.at(i).getX(),
				savePoints.at(i).getY());
	}
	p->velocity.y = 0;
	p->velocity.x = 0;
	p->s.height = p->hurt.height = runAnimation.getFrameHeight() - 10;// * 0.4;
	p->s.width = p->hurt.width = runAnimation.getFrameWidth() - 20;// * 0.16;
	p->hurt.radius = (p->s.width-5)/2;
	p->hurtJump = false;
	p->l[0].s.center.x = -50;
	p->l[1].s.center.x = -50;
	p->l[0].thrown = false;
	p->l[1].thrown = false;
	p->jumpCurrent = 0;
	p->jumpMax = 2;
	p->soundChk = true;
	game->n++;
	p->isLeft = false;
	playerState = STATE_IDLE;
}

void check_mouse(XEvent *e)
{
        static int savex = 0;
        static int savey = 0;
        static int n = 0;
		gm.lbutton = 0;
		gm.rbutton = 0;
        if (e->type == ButtonRelease) {
			mouseClick(&gm, e->xbutton.button, 2, e->xbutton.x, e->xbutton.y);
			return;
        }
        if (e->type == ButtonPress) {
                if (e->xbutton.button == 1) {
                        //Left button was pressed
						gm.lbutton = 1;
                }
                if (e->xbutton.button == 3) {
                        //Right button was pressed
						gm.rbutton = 1;
                }
        }
        //Did the mouse move?
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
                savex = e->xbutton.x;
                savey = e->xbutton.y;
                if (++n < 10)
                        return;
        }
		//Check for mouse over any buttons
		for (int i = 0; i < gm.nbuttons; i++) {
			gm.button[i].over = 0;
			gm.button[i].down = 0;
			if (savex >= button[i].r.left &&
					savex <= gm.button[i].r.right &&
					savey >= gm.button[i].r.bot &&
					savey <= gm.button[i].r.top) {
				button[i].over = 1;
				break;
			}
		}
		if (gm.lbutton)
			mouseClick(&gm, 1, 1, savex, savey);
		if (gm.rbutton)
			mouseClick(&gm, 2, 1, savex, savey);
}

void check_keys(XEvent *e) {
        //Was there input from the keyboard?
		static int shift = 0;
        int key = XLookupKeysym(&e->xkey, 0);
        if (e->type == KeyRelease) {
                gm.keys[key] = 0;
                if (gm.keys[XK_Shift_L] || gm.keys[XK_Shift_R]) {
					shift = 0;
					return;
                }
        }
        if (e->type == KeyPress) {
                gm.keys[key] = 1;
                if (gm.keys[XK_Shift_L] || gm.keys[XK_Shift_R]) {
					shift = 1;
					return;
				}
		if (gm.state == STATE_GAMEOVER) {
					if (gm.keys[XK_r] || gm.keys[XK_R]) {
							makeCharacter(&gm, gm.xres/2, gm.yres/2);
				gm.state = STATE_GAMEPLAY;
			}
                }
        } else {
			return;
		}
		if (shift) {}
        switch (key) {
                case XK_Escape:
                    gm.done = 1;
                    break;
                case XK_Tab:
                    if (gm.state == STATE_PAUSE)
                        gm.state = STATE_GAMEPLAY;
					else if (gm.state == STATE_CONTROLS)
						gm.state = STATE_STARTMENU;
                    else
                        gm.state = STATE_PAUSE;
                    break;
				
				case XK_i:
					//toggle savepoint
					if (savePoints.at(0).checkIsEnabled())
						savePoints.at(0).disable();
					else {
						savePoints.at(0).enable();
					}
					break;
				case XK_o:
					//move player to savepoint if enabled
					for (unsigned int i = 0; i < savePoints.size(); i++) {
						if (savePoints.at(i).checkIsEnabled())
						{
							lev.levelID = 1;
							loadLevel(&gm, &lev);
							movePlayer(gm.character, savePoints.at(i).getX(), 
							savePoints.at(i).getY());
						}
					}
					break;
				case XK_p:
                    if (gm.state == STATE_STARTMENU)
                        gm.state = STATE_GAMEPLAY;
                    break;
				case XK_j:
					playerState = STATE_ATTACK;
					s1.initSpearDirection(gm.character);
					s2.initSpearDirection(gm.character);
					break;
				case XK_t:
					if (enemies.size() > 0)
						enemies.at(0).stateUnitTest();
					break;
				case XK_v:
					//move enemy to position
					//enemies.erase(enemies.begin()); breaks game due to physics
					if (enemies.size() > 0)
						moveEnemy(enemies.at(0), 601, 48);
					break;
				case XK_0:
					nextLevel(&gm, &lev);
					break;
				case XK_9:
					previousLevel(&gm, &lev);
					break;
					
        }
}


void physics(Game *game, PlayerState ps)
{
	Character *p;
	Enemy *e;

	if (game->n <= 0)
		makeCharacter(&gm, gm.xres/2, gm.yres/2);
	
	
	p = &game->character;
	e = &enemies.at(0);
	
	//Gravity and velocity update
	p->velocity.y -= GRAVITY;
	p->s.center.x += p->velocity.x;
	p->s.center.y += p->velocity.y;
	
	e->move();
	e->checkState();
	

	//kyleS.cpp	
	movement(game, p, ps, gm.keys);
	charCollision(game, p, enemies);
	enemyCollision(game, p, enemies);
	savePointCheck(p, &savePoints.at(0));
	if (!gm.button[0].r.width) {
		//buttonInit(game);
	}

	//check for the character is off-screen to load next level
	if (p->s.center.y < 0.1 || p->s.center.y > gm.yres) {
		loadLevel(&gm, &lev);
		//game->n--;
	}
	if (p->s.center.x < 0.1 || p->s.center.x > gm.xres) {
		loadLevel(&gm, &lev);
		//game->n--;
	}

	//Sprite
	//check for player state, enable that animation
	if (playerState != STATE_ATTACK)
	{
		playerState = getPlayerState(p, gm.keys);
	}
	if (playerState == STATE_IDLE) {
		idleAnimation.enable();
		runAnimation.disable();
		jumpAnimation.disable();
		attackAnimation.disable();
		idleAnimation.updateAnimation();
	}
	if (playerState == STATE_RUN) {
		idleAnimation.disable();
		runAnimation.enable();
		jumpAnimation.disable();
		attackAnimation.disable();
		runAnimation.updateAnimation();
	}
	if (playerState == STATE_JUMP) {
		idleAnimation.disable();
		runAnimation.disable();
		jumpAnimation.enable();
		attackAnimation.disable();
		jumpAnimation.updateAnimation();
	}	
	if (playerState == STATE_ATTACK) {
		idleAnimation.disable();
		runAnimation.disable();
		jumpAnimation.disable();
		attackAnimation.enable();
		attackAnimation.updateAnimation();
		if (!attackAnimation.isEnabled())
			playerState = STATE_IDLE;
	}
	for (unsigned int i = 0; i < enemies.size(); i++) {	
		for (unsigned int j = 0; j < enemies.at(i).animations.size(); j++) {
			enemies.at(i).animations.at(j).enable();
			enemies.at(i).animations.at(j).updateAnimation();
		}
	}

	s1.sprite.enable();
	s2.sprite.enable();
//	s1.sprite.updateAnimation();
//	s2.sprite.updateAnimation();
	updateSpear(&game->character);
	
	if (savePoints.at(0).checkIsEnabled() && lev.levelID == 1) {
		savePoints.at(0).animations.at(0).disable();
		savePoints.at(0).animations.at(1).enable();
		savePoints.at(0).animations.at(1).updateAnimation();
	}
	else {
		savePoints.at(0).animations.at(1).disable();
		savePoints.at(0).animations.at(0).enable();
		savePoints.at(0).animations.at(0).updateAnimation();

	if (lev.levelID != 1)
		savePoints.at(0).animations.at(0).disable();
		savePoints.at(0).animations.at(1).disable();
	}
	if (gm.state == STATE_GAMEOVER) {
		countDeath();
		totalTimer(1);
	} if (gm.state == STATE_PAUSE) {
		totalTimer(1);
	}
}

void render(Game *game)
{
	float w, h;
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw background
	background(&gm);

	setLevelSwitch(&gm, &lev);
	//set up level 2 
	//setLevel2(&gm, &lev);
	//renders level 2 if on level 2
	//drawLevel2(&gm, &lev);

	//set up level 3
	//setLevel3(&gm, &lev);
	//renders level 3 if on level 3
	//drawLevel3(&gm, &lev);
	
	//draw boxs using game box array to the screen
	levelDrawBox(&gm);

	//draws level text
	levelText(&gm, &lev);

	//draw platforms & spikes
	//platforms(&gm);

	//draw character here
	glPushMatrix();
	glColor3ub(150,160,220);
	for (int i = 0; i < game->n; i++) {
		Vec *c = &game->character.s.center;
		w = 2;
		h = 2;
		glBegin(GL_QUADS);
			glVertex2i(c->x-w, c->y-h);
			glVertex2i(c->x-w, c->y+h);
			glVertex2i(c->x+w, c->y+h);
			glVertex2i(c->x+w, c->y-h);
		glEnd();
		glPopMatrix();
	}

	//Render Player Sprite
	renderSprite(idleAnimation, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(runAnimation, game->character.s.center.x, 
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(jumpAnimation, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(attackAnimation, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(s1.sprite, game->character.l[0].s.center.x, 
		game->character.l[1].s.center.y, 1.0, s1.checkIsLeft());
	renderSprite(s2.sprite, game->character.l[0].s.center.x, 
		game->character.l[1].s.center.y, 1.0, s2.checkIsLeft());
	for (unsigned int i = 0; i < enemies.size(); i++) {
		for (unsigned int j = 0; j < enemies.at(i).animations.size(); j++) {
			renderSprite(enemies.at(i).animations.at(j), enemies.at(i).getX(),
				enemies.at(i).getY(), 1.0, enemies.at(i).checkIsLeft());
		}
	}
	for (unsigned int j = 0; j < savePoints.size(); j++) {
		for (unsigned int i = 0; i < 2; i++) {
			renderSprite(savePoints.at(j).animations.at(i), savePoints.at(j).getX(),
				savePoints.at(j).getY(), 4.0, false);
		}
	}

	//Check Game States
	checkStart(&gm);
	checkControl(&gm);
	checkPause(&gm);
	checkGameOver(&gm);
	outputScore(&gm);
	outputCurrentScore(&gm);

	//Temporary spike indicator
	Rect r;
	int c = 0xffffffff;
	if (gm.state == STATE_GAMEPLAY) {
		for (int i = 0; i < 10; i++) {
			if (gm.spike[i].center.x > 0) {
				h = gm.spike[i].height;
				w = gm.spike[i].width;
				glPushMatrix();
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(0.0, 0.0, 0.0, 0.0);
				glTranslated(gm.spike[i].center.x, gm.spike[i].center.y, 0);
				glBegin(GL_QUADS);
					glVertex2i(-w, -h);
					glVertex2i(-w, +h);
					glVertex2i(+w, +h);
					glVertex2i(+w, -h);
				glEnd();
				glDisable(GL_BLEND);
				glPopMatrix();
				r.bot = gm.spike[i].center.y;
				r.left = gm.spike[i].center.x;
				r.center = 1;
				ggprint8b(&r, 16, c, "SPIKES");
			}
		}
	}
	//resets level id on game over
	gameOverLevelRestart(&gm, &lev);

}

unsigned char *buildAlphaData(Ppmimage *img)
{
	//add 4th component to RGB stream
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char*)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i = 0; i < img->width * img->height * 3; i += 3) 
	{
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a == t0 && b == t1 && c == t2)
			*(ptr + 3) = 0;
		ptr += 4;
		data += 3;
	}
	return newdata;
}

