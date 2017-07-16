#ifndef _KYLES_H_
#define _KYLES_H_

#include <cstring>
//types
typedef float Flt;
typedef Flt Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)

//constants
const Flt timeslice = 1.0f;
const Flt gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
#define WALK 2
#define JUMP 4
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define GRAVITY 0.1
#define OFFSET 15
#define C_HEIGHT 2
#define C_WIDTH 10
#define W_HEIGHT 3
#define W_WIDTH 10

//structs
struct Vec {
	Flt x, y, z;
};

struct Shape {
	Flt width, height;
	Flt radius;
	Vec center;
};

struct Hurtbox {
	Flt width, height;
	Vec center;
};

struct Hitbox {
	Flt width, height;
	Vec center;
};


enum State {
	STATE_NONE,
	STATE_STARTMENU,
	STATE_GAMEPLAY,
	STATE_PAUSE,
	STATE_GAMEOVER
};

class Weapon {
public:
	Shape s;
	Hitbox hit;
	Vec velocity;
	Vec initThrow;
	bool weaponSnd;
};

class Character {
public:
	Shape s;
	Weapon l[2];
	Hurtbox hurt;
	Vec dir;
	Vec pos;
	Vec velocity;
	int jumpCurrent;
	int jumpMax;
	bool soundChk;
	bool hurtJump;
	Flt angle;
	Flt color[3];
	bool isLeft;
};

class Game {
public:
	Shape box[7];
	Shape plat[10];
	Shape spike[10];
	Character character;
	State state;
	int n;
	int xres, yres;
	int done;
	char keys[65536];
	bool swtch;
	Game() {
		n = 0;
		state = STATE_STARTMENU;
		xres = WINDOW_WIDTH;
		yres = WINDOW_HEIGHT;
		done = 0;
		memset(keys, 0, 65536);
	}
};

#endif //_KYLES_H_
