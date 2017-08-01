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
extern void savePointCheck(Character *p, vector<SavePoint>& sp);
extern void selection(Game *game, int x, int y, int h, int w);
extern void start(Game *game);
extern void loadGameover(Game *game);
extern void loadLoading(Game *game);
extern void loading(Game *game);
extern void loadStart(Game *game);
extern void loadSpikes(Game *game);
extern void prepSpike(Game *game);
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
extern void checkLoading(Game *game);
extern void checkGameOver(Game *game);
extern void charHurt(Game *game, Character *p);
extern void mouseClick(Game *game, int action, int x, int y);
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
extern void background_music(int choice);
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
extern void setPauseTime();
extern void setDeathTime();
extern void resumeTime();
extern void outputCurrentScore(Game *game);
extern void loadFlames(Game *game);

//declare player state
PlayerState playerState;

//declare game object
Game gm;

//declare level object
Level lev;

//declare button object
Button button[MAXBUTTONS];

//vectors
vector<Enemy> enemies;
vector<SavePoint> savePoints;
vector<SpriteAnimation> decorations;
vector<Upgrade> upgrade;

//spears
Spear s1, s2;

//Fireball
Fireball fireball(19,27,-50,65);
//sprite testing
SpriteAnimation runAnimation((char*)"player.png", 1, 12, 12, 1, 6, 
	46, 50, 0.1, true); 
SpriteAnimation idleAnimation((char*)"player.png", 1, 12, 12, 0, 0, 
	46, 50, 0.1, true);
SpriteAnimation jumpAnimation((char*)"player.png", 1, 12, 12, 8, 8,
	46, 50, 0.1, true);
SpriteAnimation attackAnimation((char*)"player.png", 1, 12, 12, 8, 10,
	46, 50, 0.1, false);
SpriteAnimation runAnimation2((char*)"greenArmor.png", 1, 12, 12, 1, 5, 
	36, 40, 0.1, true); 
SpriteAnimation idleAnimation2((char*)"greenArmor.png", 1, 12, 12, 0, 0, 
	36, 40, 0.1, true);
SpriteAnimation jumpAnimation2((char*)"greenArmor.png", 1, 12, 12, 8, 8,
	36, 40, 0.1, true);
SpriteAnimation attackAnimation2((char*)"greenArmor.png", 1, 12, 12, 8, 10,
	36, 40, 0.1, false);

// selection
bool hasSelection = false;
int sx, sy, sh, sw;

bool newLevel = false;
bool debug = false;

int main(void)
{
	spawnEntities(0, enemies, savePoints, decorations, upgrade);
	fireball.initAnimations();
	s1.initAnimations();
	s2.initAnimations();
	SavePoint sp1(100, 59, false);
	SavePoint sp2(100, 59, false);
	SavePoint sp3(100, 59, false);
	SavePoint sp4(100, 59, false);
	SavePoint sp5(1100, 59, false);
	savePoints.push_back(sp1);
	savePoints.push_back(sp2);
	savePoints.push_back(sp3);
	savePoints.push_back(sp4);
	savePoints.push_back(sp5);

	savePoints.at(0).initAnimations();
	savePoints.at(1).initAnimations();
	savePoints.at(2).initAnimations();
	savePoints.at(3).initAnimations();
	savePoints.at(4).initAnimations();
	Upgrade u1(0, 300, 48, true, "Jumping Armor", "Allows you to double jump");
	Upgrade u2(1, 800, 760, true, "Sprinting Boots", "Allows you to spring");
	upgrade.push_back(u1);
	upgrade.push_back(u2);
	upgrade.at(0).initAnimation();
	upgrade.at(1).initAnimation();
	#ifdef USE_OPENAL_SOUND
	initialize_sound();
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
		if (newLevel) {
			spawnEntities(lev.levelID, enemies, savePoints, decorations, upgrade);
			//spawnEnemies(lev.levelID, enemies);
			newLevel = false;
		}
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
	loadStart(&gm);
}

void makeCharacter(Game *game, int x, int y)
{
	//position of character
	Character *p = &game->character;
	
	p->s.center.x = p->hurt.center.x = x;
	p->s.center.y = p->hurt.center.y = y;
	for (unsigned int i = 0; i < savePoints.size(); i++) {
		if (savePoints.at(i).checkIsEnabled()) {
			movePlayer(game->character, savePoints.at(i).getX(),
				savePoints.at(i).getY());
			if (getSavePointLevel(savePoints) > 0) {
				lev.levelID = getSavePointLevel(savePoints);
				loadLevel(game, &lev);
			}
		}
	}
	fireball.move(-50,fireball.getY());
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
	if (p->upgrade)
		p->jumpMax = 2;
	else
		p->jumpMax = 1;
	p->soundChk = true;
	game->n++;
	p->isLeft = false;
	playerState = STATE_IDLE;
	newLevel = true;
}

void check_mouse(XEvent *e)
{
        static int savex = 0;
        static int savey = 0;
        static int n = 0;
        if (e->type == ButtonRelease) {
			return;
        }
        if (e->type == ButtonPress) {
                if (e->xbutton.button == 1) {
                	//Left button was pressed
					mouseClick(&gm, 1, e->xbutton.x, e->xbutton.y);
                }
                if (e->xbutton.button == 3) {
                	//Right button was pressed
					mouseClick(&gm, 2, e->xbutton.x, e->xbutton.y);
                }
        }
        //Did the mouse move?
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		int x, y;
		savex = x = e->xbutton.x;
		savey = y = e->xbutton.y;
		if (++n < 10)
			return;
		if (gm.state == STATE_STARTMENU) {
			int w, h;
			if (y < gm.yres*0.49 && y > gm.yres*0.42) {
				if (x > gm.xres*0.1 && x < gm.xres*0.23) {
					w = gm.xres*0.23 - gm.xres*0.1;
					h = gm.yres*0.3 - gm.yres*0.2;
					//selection(&gm, x, y, h, w);
					hasSelection = true;
					sw = w;
					sh = h;
					sx = gm.xres*0.11 + w/2;
					sy = gm.yres*0.55;
				}
			}
			else if (y < gm.yres*0.69 && y > gm.yres*0.60) {
				if (x > gm.xres*0.08 && x < gm.xres*0.26) {
					w = gm.xres*0.26 - gm.xres*0.08;
					h = gm.yres*0.45 - gm.yres*0.37;
					//selection(&gm, x, y, h, w);
					hasSelection = true;
					sw = w;
					sh = h;
					sx = gm.xres*0.09;
					sy = gm.yres*0.37;
				}
			}
			else if (y < gm.yres*0.84 && y > gm.yres*.76) {
				if (x > gm.xres*0.11 && x < gm.xres*0.22) {
					w = gm.xres*0.22 - gm.xres*0.11;
					h = gm.yres*0.63 - gm.yres*0.55;
					//selection(&gm, x, y, h, w);
					hasSelection = true;
					sw = w;
					sh = h;
					sx = gm.xres*0.1 + w/2;
					sy = gm.yres*0.2;
				}
			}
			else
				hasSelection = false;

		}
		if (gm.state == STATE_GAMEOVER) {
			int w, h;
			if (y < gm.yres*0.9 && y > gm.yres*0.82) {
				if (x > gm.xres*0.41 && x < gm.xres*0.59) {
					w = gm.xres*0.59 - gm.xres*0.41;
					h = gm.yres*0.72 - gm.yres*0.62;
					//selection(&gm, x, y, h, w);
					hasSelection = true;
					sw = w;
					sh = h / 2;
					sx = gm.xres*0.42 + w/2;
					sy = gm.yres*0.14;
				}
			}
			else
				hasSelection = false;
		}
	}
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
				}
		if (gm.state == STATE_GAMEOVER) {
					if (gm.keys[XK_r] || gm.keys[XK_R]) {
                                resumeTime();
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
                        setPauseTime();
                    if (gm.state == STATE_PAUSE) {
                        gm.state = STATE_GAMEPLAY;
                        resumeTime();
                    }
					else if (gm.state == STATE_CONTROLS)
						gm.state = STATE_STARTMENU;
                    else
                        gm.state = STATE_PAUSE;
                    break;
				case XK_m:
					if (gm.state == STATE_CONTROLS)
						gm.state = STATE_STARTMENU;
					break;
				case XK_i:
					//toggle savepoint
					if (!debug)
						break;
					if (savePoints.at(0).checkIsEnabled())
						savePoints.at(0).disable();
					else {
						savePoints.at(0).enable();
					}
					break;
				case XK_o:
					//move player to savepoint if enabled
					if (!debug)
						break;
					for (unsigned int i = 0; i < savePoints.size(); i++) {
						if (savePoints.at(i).checkIsEnabled())
						{
							lev.levelID = 1;
							loadLevel(&gm, &lev);
							newLevel = true;
							movePlayer(gm.character, savePoints.at(i).getX(), 
							savePoints.at(i).getY());
						}
					}
					break;
				case XK_p:
                    initializeTime();
                    if (gm.state == STATE_STARTMENU) {
                        gm.state = STATE_NONE;
						loadLoading(&gm);
					}
                    break;
				case XK_j:
					playerState = STATE_ATTACK;
						if (gm.character.l[0].s.center.x < 0 || 
							gm.character.l[0].s.center.x > gm.xres) {
							s1.initSpearDirection(gm.character);
							s2.initSpearDirection(gm.character);
						}
					break;
				case XK_n:
					debug = !debug;
					break;
				case XK_t:
					if (!debug)
						break;
					if (enemies.size() > 0)
						enemies.at(0).stateUnitTest();
					break;
				case XK_v:
					//move enemy to position
					if (!debug)
						break;
					if (enemies.size() > 0)
						moveEnemy(enemies.at(0), 601, 48);
					break;

				case XK_z:
					if (!debug)
						break;
					gm.character.upgrade2 = true;
					break;
				case XK_0:
					if (!debug)
						break;
					nextLevel(&gm, &lev);
					spawnEntities(lev.levelID, enemies, savePoints, decorations, upgrade);
					break;
				case XK_9:
					if (!debug)
						break;
					previousLevel(&gm, &lev);
					spawnEntities(lev.levelID, enemies, savePoints, decorations, upgrade);
					break;
					
        }
}


void physics(Game *game, PlayerState ps)
{
	
	Character *p;

	if (game->n <= 0) {
		makeCharacter(&gm, gm.xres/2, gm.yres/2);
		game->character.jumpMax = 1;
	}
	
	
	p = &game->character;
	
	//Gravity and velocity update
	p->velocity.y -= GRAVITY;
	p->s.center.x += p->velocity.x;
	p->s.center.y += p->velocity.y;
	for (unsigned int i = 0; i < enemies.size(); i++) {
		enemies.at(i).move();
		enemies.at(i).checkState();
	}

	//kyleS.cpp	
	movement(game, p, ps, gm.keys);
	charCollision(game, p, enemies);
	enemyCollision(game, p, enemies);
	savePointCheck(p, savePoints);
	upgradeCheck(p, upgrade);	

	checkFireball(game, p, fireball);
	
	//check for the character is off-screen to load next level
	if (p->s.center.y < 0.1 || p->s.center.y > gm.yres) {
		loadLevel(&gm, &lev);
		newLevel = true;
		//spawnEnemies(lev.levelID, enemies);
		//game->n--;
	}
	if (p->s.center.x < 0.1 || p->s.center.x > gm.xres) {
		loadLevel(&gm, &lev);
		//spawnEnemies(lev.levelID, enemies);
		newLevel = true;
		//game->n--;
	}

	//Sprite
	//check for player state, enable that animation
	if (playerState != STATE_ATTACK)
	{
		playerState = getPlayerState(p, gm.keys);
	}
	if (playerState == STATE_IDLE) {
		if (!game->character.upgrade)
			idleAnimation.enable();
		else
			idleAnimation2.enable();
		runAnimation.disable();
		jumpAnimation.disable();
		attackAnimation.disable();
		runAnimation2.disable();
		jumpAnimation2.disable();
		attackAnimation2.disable();

		idleAnimation.updateAnimation();
		idleAnimation2.updateAnimation();
	}
	if (playerState == STATE_RUN) {
		idleAnimation.disable();
		idleAnimation2.disable();
		if (!game->character.upgrade)
			runAnimation.enable();
		else
			runAnimation2.enable();
		jumpAnimation.disable();
		attackAnimation.disable();
		jumpAnimation2.disable();
		attackAnimation2.disable();

		runAnimation.updateAnimation();
		runAnimation2.updateAnimation();
	}
	if (playerState == STATE_JUMP) {
		idleAnimation.disable();
		idleAnimation2.disable();
		runAnimation.disable();
		runAnimation2.disable();
		if (!game->character.upgrade)
			jumpAnimation.enable();
		else
			jumpAnimation2.enable();
		attackAnimation.disable();
		attackAnimation2.disable();
		jumpAnimation.updateAnimation();
		jumpAnimation2.updateAnimation();
	}	
	if (playerState == STATE_ATTACK) {
		idleAnimation.disable();
		idleAnimation2.disable();
		runAnimation.disable();
		runAnimation2.disable();
		jumpAnimation.disable();
		jumpAnimation2.disable();
		if (!game->character.upgrade)
			attackAnimation.enable();
		else
			attackAnimation2.enable();
		attackAnimation.updateAnimation();
		attackAnimation2.updateAnimation();
		if (!attackAnimation.isEnabled() && !attackAnimation2.isEnabled())
			playerState = STATE_IDLE;
	}
	for (unsigned int i = 0; i < enemies.size(); i++) {	
		for (unsigned int j = 0; j < enemies.at(i).animations.size(); j++) {
			enemies.at(i).animations.at(j).enable();
			enemies.at(i).animations.at(j).updateAnimation();
			if (enemies.at(i).getType() == 2 && 
				enemies.at(i).animations.at(j).getCurrentFrame() == 4)
				enemies.at(i).attack(fireball);
		}
	}
	
	fireball.sprite.enable();
	fireball.sprite.updateAnimation();
	fireball.updatePosition();

	s1.sprite.enable();
	s2.sprite.enable();
	s1.sprite.updateAnimation();
//	s2.sprite.updateAnimation();
	updateSpear(&game->character);

	for (unsigned int i = 0; i < savePoints.size(); i++) {
		for (unsigned int j = 0; j < savePoints.at(i).animations.size(); j++) {
			savePoints.at(i).animations.at(j).updateAnimation();
			//if (savePoints.at(i).checkIsEnabled()) {
			//}
		}
	}
	for (unsigned int i = 0; i < decorations.size(); i++) {
		decorations.at(i).enable();
		decorations.at(i).updateAnimation();
	}

	checkSavePoints(lev.levelID, savePoints);
	checkUpgrade(lev.levelID, upgrade);
	for (unsigned int i = 0; i < upgrade.size(); i++) {
		upgrade.at(i).sprite.updateAnimation();
	}
	
	if (gm.state == STATE_GAMEOVER) {
		resetTime();
		setDeathTime();
		countDeath();
	} 
	if (gm.state == STATE_PAUSE) {
		setPauseTime();
	}
}

void render(Game *game)
{
	float w, h;
//	glClearColor(0.0, 0.0, 0.0, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT);

	if (gm.state == STATE_STARTMENU) {
		loadStart(&gm);
		checkStart(&gm);
	}

	if (gm.state == STATE_LOADING) {

		//textures
		loadBackground(&gm);
		loadPlatforms(&gm);
		loadBoxes(&gm);
		loadSpikes(&gm);
		loadFlames(&gm);
	
		//Sprites
		runAnimation.convertToPpm();
		runAnimation.createTexture();
		idleAnimation.convertToPpm();
		idleAnimation.createTexture();
		jumpAnimation.convertToPpm();
		jumpAnimation.createTexture();
		attackAnimation.convertToPpm();
		attackAnimation.createTexture();
		runAnimation2.convertToPpm();
		runAnimation2.createTexture();
		idleAnimation2.convertToPpm();
		idleAnimation2.createTexture();
		jumpAnimation2.convertToPpm();
		jumpAnimation2.createTexture();
		attackAnimation2.convertToPpm();
		attackAnimation2.createTexture();
		fireball.sprite.convertToPpm();
		fireball.sprite.createTexture();
		s1.sprite.convertToPpm();
		s1.sprite.createTexture();
		s2.sprite.convertToPpm();
		s2.sprite.createTexture();
		if (enemies.size() > 0)
			printf("SIZE: %i\n", (int)enemies.at(0).animations.size());
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
		for (unsigned int i = 0; i < decorations.size(); i++) {
			decorations.at(i).convertToPpm();
			decorations.at(i).createTexture();
		}
		for (unsigned int i = 0; i < upgrade.size(); i++) {
			upgrade.at(i).sprite.convertToPpm();
			upgrade.at(i).sprite.createTexture();
		}
		gm.state = STATE_GAMEPLAY;
	}
	//draw background
	background(&gm);

	renderEntities(decorations);
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

	
	for (unsigned int i = 0; i < upgrade.size(); i++) {
		renderSprite(upgrade.at(i).sprite, upgrade.at(i).getX(), 
			upgrade.at(i).getY(), 1.0, false);
	}

	//Render Player Sprite
	if (game->character.upgrade) {
	renderSprite(idleAnimation2, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(runAnimation2, game->character.s.center.x, 
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(jumpAnimation2, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(attackAnimation2, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(s1.sprite, game->character.l[0].s.center.x, 
		game->character.l[0].s.center.y, 1.0, s1.checkIsLeft());
	renderSprite(s2.sprite, game->character.l[1].s.center.x, 
		game->character.l[1].s.center.y, 1.0, s2.checkIsLeft());
	}
	else {
	renderSprite(idleAnimation, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(runAnimation, game->character.s.center.x, 
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(jumpAnimation, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(attackAnimation, game->character.s.center.x,
		game->character.s.center.y, 1.0, game->character.isLeft);
	renderSprite(s1.sprite, game->character.l[0].s.center.x, 
		game->character.l[0].s.center.y, 1.0, s1.checkIsLeft());
	renderSprite(s2.sprite, game->character.l[1].s.center.x, 
		game->character.l[1].s.center.y, 1.0, s2.checkIsLeft());
	}
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
	renderSprite(fireball.sprite, fireball.getX(), fireball.getY(), 1.0, false);

	

	//Check Game States
	checkStart(&gm);
	checkLoading(&gm);
	checkControl(&gm);
	checkPause(&gm);
	//checkGameOver(&gm);
	outputCurrentScore(&gm);

	if (gm.state == STATE_GAMEOVER) {
		if (gm.hasDied)
			loadGameover(&gm);
	}

	if (gm.state == STATE_STARTMENU || gm.state == STATE_GAMEOVER) {
		if (hasSelection)
			selection(&gm, sx, sy, sh, sw);
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

