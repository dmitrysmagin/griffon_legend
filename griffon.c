/*
 in game scripts
   2 - find master key
   3 - find crystal
   4 - find shield - obj 8
   5 - find sword - obj 9
   6 - regular key chest
   7 - blue flask
   8 - garden// s master key
   9 - lightning bomb
  10 - blue flask chest
  11 - lightning chest
  12 - armour chest
  13 - citadel master key
  14 - end of game
  15 - get sword3
  16 - shield3
  17 - armour3
  20 key chest 1
  60-lever

 monsters
 1 - baby dragon
 2 - one wing
 3 - boss 1
 4 - black knight
 5 - fire hydra
 6 - red dragon
 7 - priest
 8 - yellow fire dragon
 9 - two wing
10 - dragon2
11 - final boss
12 - bat kitty

 chests
  0 - regular flask
 11 - key chest
 14 - blue flask chest
 15 - lightning chest
 16 - armour chest
 17 - citadel master key
 18 - sword3
 19 - shield3
 20 - armour3

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#define MAXNPC		32
#define MAXFLOAT	32
#define MAXSPELL	32
#define ice		0
#define steel		1
#define wood		2
#define rock		3
#define fire		4

// inventory items
//#define flask		0
//#define doubleflask	1
//#define shock		2
//#define normalkey	3
//#define masterkey	4

#define sndbite		0
#define sndcrystal	1
#define snddoor		2
#define sndenemyhit	3
#define sndice		4
#define sndlever	5
#define sndlightning	6
#define sndmetalhit	7
#define sndpowerup	8
#define sndrocks	9
#define sndswordhit	10
#define sndthrow	11
#define sndchest	12
#define sndfire		13
#define sndbeep		14

typedef struct {
	float	px;
	float	py;
	float	opx;
	float	opy;
	int	walkdir;
	float	walkframe;
	float	walkspd;
	float	attackframe;
	float	attackspd;

	int	hp;
	int	maxhp;
	float	hpflash;
	int	hpflashb;
	int	level;
	int	sword;
	int	shield;
	int	armour;
	int	foundspell[6];
	float	spellcharge[6];
	int	flasks;
	int	foundcrystal;
	float	crystalcharge;
	float	attackstrength;
	float	spellstrength;
	int	spelldamage;
	int	sworddamage;

	int	masterkey;

	int	exp;
	int	nextlevel;

	int	windowloc;
	int	pause;

	float	itemselshade;
	int	ysort;
} PLAYERTYPE;

typedef struct {
	float	x;
	float	y;
	int	parentID;
	int	isbase;
	int	sprite;
	int	bonelength;	// the // bone//  that connects the body sections
} BODYSECTIONTYPE;


typedef struct {
	float	x;
	float	y;
	int	spriteset;
	int	x1;		// patrol area
	int	y1;
	int	x2;
	int	y2;
	int	attitude;
	int	hp;

	int	maxhp;
	int	item1;
	int	item2;
	int	item3;
	int	script;
	float	frame;
	float	frame2;		// end boss specific
	int	cframe;
	int	onmap;		// is this npc set to be genned in the mapfile

	int	ticks;
	int	pause;
	int	shake;

	int	movementmode;
	int	walkdir;
	float	walkspd;
	int	movingdir;
	int	moving;

	int	attacking;
	float	attackframe;
	int	cattackframe;
	float	attackspd;
	int	attackdelay;
	int	attacknext;
	int	attackattempt;

	int	spelldamage;
	int	attackdamage;


	// one wing and firehydra specific
	BODYSECTIONTYPE bodysection[31];
	float	swayangle;
	float	swayspd;
	float	headtargetx[4];
	float	headtargety[4];
	int	castpause;

	// firehydra specific
	int	attacknext2[4];
	int	attacking2[4];
	int	attackframe2[4];

	// dragon2 specific
	float	floating;
} NPCTYPE;


typedef struct {
	int	spellnum;
	float	homex;
	float	homey;
	float	enemyx;
	float	enemyy;

	float	frame;

	int	damagewho;	// 0 = npc, 1 = player

	// for earthslide
	float	rocky[9];
	int	rockimg[9];
	int	rockdeflect[9];

	float	strength;

	// fire
	int	legalive[5];

	// spell 6 specific
	float	fireballs[7][4];	// x,y,targetx, targety
	int	nfballs;
	int	ballon[7];

	int	npc;
} SPELLTYPE;



typedef struct {
	int	x;	// xyloc on spriteimageset
	int	y;
	int	xofs;	// the actual place to paste the sprite in reference to the bodypart loc on screen
	int	yofs;
	int	w;	// w/h of the sprite in the imageset
	int	h;
} ANIMSET2TYPE;


void game_addFloatIcon(int ico, float xloc, float yloc);
void game_addFloatText(char *stri, float xloc, float yloc, int col);
void game_attack();
void game_castspell(int spellnum, float homex, float homey, float enemyx, float enemyy, int damagewho);
void game_checkhit();
void game_checkinputs();
void game_configmenu();
void game_damagenpc(int npcnum, int damage, int spell);
void game_damageplayer(int damage);
void game_drawanims(int Layer);
void game_drawhud();
void game_drawnpcs(int mode);
void game_drawover(int modx, int mody);
void game_drawplayer();
void game_drawview();
void game_endofgame();
void game_eventtext(char *stri);
void game_handlewalking();
void game_loadmap(int mapnum);
void game_main();
void game_newgame();
void game_playgame();
void game_processtrigger(int trignum);
void game_saveloadnew();
void game_showlogos();
void game_swash();
void game_theend();
void game_title(int mode);
void game_updanims();
void game_updatey();
void game_updmusic();
void game_updnpcs();
void game_updspells();
void game_updspellsunder();

void sys_initialize();
void sys_line(SDL_Surface *buffer, int x1, int y1, int x2, int y2, int col);
void sys_LoadAnims();
void sys_LoadFont();
void sys_LoadItemImgs();
void sys_LoadTiles();
void sys_LoadTriggers();
void sys_print(SDL_Surface *buffer, char *stri, int xloc, int yloc, int col);
void sys_progress(int w, int wm);
void sys_LoadObjectDB();
void sys_setupAudio();
void sys_update();

// system
SDL_Surface *video, *videobuffer, *videobuffer2, *videobuffer3;
SDL_Surface *titleimg, *titleimg2, *inventoryimg;
SDL_Surface *logosimg, *theendimg;
SDL_Event event;
int SCR_WIDTH, SCR_HEIGHT, SCR_BITS, SCR_TOPX, SCR_TOPY;

SDL_Surface *mapbg, *clipbg, *clipbg2;
unsigned int clipsurround[4][4];
int fullscreen;
//int walklimits[7] = {5 * 16, 5 * 16, 14 * 16, 9 * 16, 320, 144};
Uint8 *keys;
float animspd;
int rampdata[40][24];
//JOYINFO joystickinfo;
int curmap;
SDL_Surface *fontchr[224][5]; // 256 - 32
SDL_Surface *itemimg[21], *windowimg;
SDL_Surface *spellimg;

int itemselon, curitem, itemticks;
float itemyloc;
int selenemyon, curenemy, forcepause;
int roomlock; // set to disable any room jumps while in the room
int scriptflag[100][10], saveslot;	// script, flag

// timer related - move to local later
int ticks, tickspassed, nextticks;
float fp, fps, fpsr;
int secsingame, secstart;

extern char *story[38];
SDL_Surface *mapimg[4];
extern int invmap[4][7][13];
extern char *story2[27];

// options
int opfullscreen, opmusic = 1, opeffects = 1, opmusicvol, opeffectsvol;

SDL_Rect rc, rc2, rcSrc, rcDest, rcSrc2;


int HWACCEL, HWSURFACE, maxlevel;

// inventory
int inventoryalpha, inventory[6];

// -----------special case
int dontdrawover;	// used in map24 so that the candles dont draw over the boss, default set to 0

// saveload info
SDL_Surface *saveloadimg;


// post info
float postinfo[21][3];
int nposts;

// cloud info
SDL_Surface *cloudimg;
float clouddeg = 0;
int cloudson;

// spell info
SPELLTYPE spellinfo[MAXSPELL+1];


// player info
int movingup, movingdown, movingleft, movingright;
PLAYERTYPE player;
int attacking;
PLAYERTYPE playera;


// tile info
SDL_Surface *tiles[4];
int tileinfo[3][40][24][3]; // maplayer, x, y, tiledata (tile, tilelayer)

extern int elementmap[15][20];


// animation info
SDL_Surface *anims[100];
		// id number 0&1 = players
SDL_Surface *animsa[100];
		// attack anims
float playerattackofs[4][16][3];
		// [dir] [frame] [x,y ofs, completed(0/1)]

float floattext[MAXFLOAT+1][4];
		// [id] [framesleft, x, y, col]
char *floatstri[MAXFLOAT+1];

float  floaticon[MAXFLOAT+1][4];
		// [id] [framesleft, x, y, ico]

// special for animset2
ANIMSET2TYPE animset2[7], animset9[7];

// object info
float objectframe[256][2];
int lastobj;
		// frame!, curframe
int objectinfo[33][6];
		// nframes,xtiles,ytiles,speed,type,script, update?
int objecttile[33][9][3][3][2];
		// [objnum] [frame] [x] [y] [tile/layer]
int objmap[21][15];

int objmapf[1000][21][15];
		// [mapnum] x, y  set to 1 to make this objmap spot stay at -1

// trigger info
int triggers[10000][9];
		// [map#][index], [var]
		// map#,x,y
int triggerloc[320][240], ntriggers;

// npc info
NPCTYPE npcinfo[MAXNPC+1];
int lastnpc;

// music info
Mix_Chunk *mgardens, *mgardens2, *mgardens3, *mgardens4, *mboss, *mmenu, *mendofgame;
int menabled=1, musicchannel = -1, menuchannel = -1;
int pgardens, pboss, ptown, pacademy, pcitadel;
int loopseta = 0;

Mix_Chunk *sfx[21];

// room locks
int roomlocks[201], saidlocked, canusekey, locktype, roomtounlock, saidjammed;
// set to 1 for normal key, set to 2 for master, set to 0 if unlocked

// dialog
int dialogflags[1000];

// ysort
int ysort[2401], lasty, firsty;

int pmenu;

#undef main
int main()
{
	sys_initialize();
	game_showlogos();
	game_main();

	return 0;
}

// element tile locations
int elementmap[15][20] = {
	{  2, 2, 2, 2,-1,-1,-1, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1 },
	{  2,-1,-1,-1,-1,-1,-1, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1 },
	{  2,-1, 2, 2,-1,-1,-1, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1 },
	{  2,-1, 2,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{  2, 2, 2, 2, 2,-1,-1,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1, 2,-1, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1, 0, 0, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }
};

char *story[38] = {
	"Ever since I was a child",
	"I remember being told the",
	"Legend of the Griffon Knights,",
	"who rid the world of the",
	"Dragon Empire.  These great",
	"heroes inspired us to become",
	"knights as well.",
	" ",
	"Now, 500 years after the war",
	"ended, the Dragons have",
	"returned.  Cities are falling",
	"from the lack of knights to",
	"protect them.",
	" ",
	"We never saw it coming.",
	" ",
	"And now, here I am, making",
	"my way into the lower town",
	"of Fidelis, a small city on",
	"the main continent. The rest",
	"of my men have died over",
	"the last couple days from",
	"aerial attacks.",
	" ",
	"We believed we could find",
	"shelter here, only to find",
	"every last griffon dead,",
	"the town burned to the ground,",
	"and transformed into a garrison",
	"for the Dragon forces.",
	" ",
	"In these dark times, I try to",
	"draw strength from the stories",
	"of those knights that risked",
	"everything to protect their homeland,",
	" ",
	"and hope that I can die",
	"with that honor as well."
};

char *story2[27] = {
	"After the fall of Margrave Gradius,",
	"All the dragons, struck with panic,",
	"evacuated the city immediately.",
	" ",
	"It\'s funny how without a leader",
	"everyone is so weak.",
	" ",
	" ",
	"But yet another leader will rise,",
	"and another city will fall.",
	" ",
	" ",
	"I should return home to Asherton",
	"It\'s time to leave this place",
	"and cleanse this blood stained",
	"life of mine.",
	" ",
	"No one should have to see as much",
	"death as I have.",
	" ",
	" ",
	"Before, I said that I wanted",
	"to die an honorable death.",
	" ",
	"Now I say that I have lived an",
	"honorable life,",
	"and I am free to die as I please."
};

// map in inventory menu
int invmap[4][7][13] = {
	// map 0
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,43,44,45,46,0,0,0,0},
		{0,0,0,0,0,42,0,0,0,0,0,0,0},
		{0,0,0,0,3,2,0,0,0,0,0,0,0},
		{0,0,0,0,4,5,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	// map 1
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,24,0,0,0,0,0,0,0,0,0},
		{0,0,19,20,21,22,0,0,0,27,0,0,0},
		{0,0,16,17,18,0,0,0,29,30,31,0,0},
		{0,0,12,0,13,14,0,32,33,34,35,36,0},
		{0,8,7,6,9,10,0,37,38,39,40,41,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	// map 2
	{
		{0,0,0,0,0,0,67,0,0,0,0,0,0},
		{0,0,0,0,0,0,66,0,0,0,0,0,0},
		{0,0,0,0,0,63,64,65,0,0,0,0,0},
		{0,0,0,0,58,59,60,61,62,0,0,0,0},
		{0,0,0,0,0,55,56,57,0,0,0,0,0},
		{0,0,0,0,50,51,52,53,54,0,0,0,0},
		{0,0,0,0,0,48,47,49,0,0,0,0,0}
	},

	// map 3
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,82,0,0,0,0,0,0,0,0,0},
		{0,0,0,79,80,81,0,74,72,0,0,0,0},
		{0,0,0,78,0,0,0,73,70,69,68,0,0},
		{0,0,77,76,75,0,0,0,71,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0}
	}
};

// HELPER MACRO ---------------------------------------------------------------
#define PRINT(A,B) 			\
do {					\
	char line[256];			\
	sprintf(line, A "\n", B);	\
	fputs(line, fp);		\
} while(0)

#define INPUT(A, B)			\
do {					\
	char line[256];			\
	fgets(line, sizeof(line), fp);	\
	sscanf(line, A, B);		\
} while(0)

// CODE GOES HERE -------------------------------------------------------------

// copypaste from hRnd_CRT()
static float RND()
{
	/* return between 0 and 1 (but never 1) */
	return (float)rand() * (1.0 / ((float)RAND_MAX + 1.0));
}

void game_addFloatIcon(int ico, float xloc, float yloc)
{
	int i = 0;
	do {
		if(floaticon[i][0] == 0) {
			floaticon[i][0] = 32;
			floaticon[i][1] = xloc;
			floaticon[i][2] = yloc;
			floaticon[i][3] = ico;
			return;
		}
		i++;
		if(i == MAXFLOAT+1) break;
	} while(1);
}

void game_addFloatText(char *stri, float xloc, float yloc, int col)
{
	int i = 0;
	do {
		if(floattext[i][0] == 0) {
			floattext[i][0] = 32;
			floattext[i][1] = xloc;
			floattext[i][2] = yloc;
			floattext[i][3] = col;
			strcpy(floatstri[i], stri);
			return;
		}
		i++;
		if(i == MAXFLOAT+1) break;
	} while(1);
}

void game_attack()
{
	float npx, npy;

	npx = player.px + 12;
	npy = player.py + 20;

	int lx = (int)npx / 16; //(int)npx / 16; //(npx - (npx % 16)) / 16;
	int ly = (int)npy / 16; //(int)npy / 16; //(npy - (npy % 16)) / 16;

	// if facing up
	if(player.walkdir == 0) {
		if(ly > 0) {
			int o2 = 0; // ??
			int o = objmap[lx][ly - 1];
			if(ly > 1 && curmap == 58) o2 = objmap[lx][ly - 2];
			if(ly > 1 && curmap == 54) o2 = objmap[lx][ly - 2];

			// cst
			if((objectinfo[o][4] == 1 && (o == 0 || o > 4)) || (objectinfo[o2][4] == 0 && o2 == 10)) {
				if(o2 == 10) o = 10;

				int oscript = objectinfo[o][5];
				if(oscript == 0 && inventory[0] < 9) {
					inventory[0] = inventory[0] + 1;
					if(inventory[0] > 9) inventory[0] = 9;
					game_addFloatIcon(6, lx * 16, (ly - 1) * 16);

					objmapf[curmap][lx][ly - 1] = 1;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

					game_eventtext("Found Flask!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 0 && inventory[0] == 9) {
					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndchest], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					game_eventtext("Cannot Carry any more Flasks!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 2) {
					inventory[4] = inventory[4] + 1;

					game_addFloatIcon(14, lx * 16, (ly - 1) * 16);

					itemticks = ticks + 215;

					if(curmap == 34) scriptflag[2][0] = 2;
					if(curmap == 62) scriptflag[8][0] = 2;
					if(curmap == 81) scriptflag[13][0] = 2;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;
					game_eventtext("Found the Temple Key!");
					return;
				}

				if(oscript == 3) {
					player.foundcrystal = 1;
					player.crystalcharge = 0;

					game_addFloatIcon(7, lx * 16, (ly - 1) * 16);

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

					game_eventtext("Found the Infinite Crystal!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 4 && player.shield == 1) {
					player.shield = 2;

					game_addFloatIcon(4, lx * 16, (ly - 1) * 16);

					itemticks = ticks + 215;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

					game_eventtext("Found the Obsidian Shield!");
					objmapf[4][1][2] = 1;
					return;
				}

				if(oscript == 5 && player.sword == 1) {
					player.sword = 2;

					game_addFloatIcon(3, lx * 16, (ly - 1) * 16);

					itemticks = ticks + 215;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;
					game_eventtext("Found the Fidelis Sword!");
					return;
				}

				if(oscript == 6) {
					if(inventory[3] < 9) {
						inventory[3] = inventory[3] + 1;

						for(int s = 20; s <= 23; s++) {
							if(scriptflag[s][0] == 1) {
								scriptflag[s][0] = 2;
							}
						}

						if(menabled == 1 && opeffects == 1) {
							int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
							Mix_Volume(snd, opeffectsvol);
						}

						objmapf[curmap][lx][ly - 1] = 1;

						if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

						game_eventtext("Found Key");
						game_addFloatIcon(16, lx * 16, (ly - 1) * 16);
					} else {
						if(menabled == 1 && opeffects == 1) {
							int snd = Mix_PlayChannel(-1, sfx[sndchest], 0);
							Mix_Volume(snd, opeffectsvol);
						}

						game_eventtext("Cannot Carry Any More Keys");
					}
				}

				if(oscript == 7 && inventory[1] < 9) {
					inventory[1] = inventory[1] + 1;
					if(inventory[1] > 9) inventory[1] = 9;
					game_addFloatIcon(12, lx * 16, (ly - 1) * 16);

					objmapf[curmap][lx][ly - 1] = 1;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

					game_eventtext("Found Mega Flask!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 7 && inventory[1] == 9) {
					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndchest], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					game_eventtext("Cannot Carry any more Mega Flasks!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 10 && inventory[1] < 9) {
					inventory[1] = inventory[1] + 1;
					if(inventory[1] > 9) inventory[1] = 9;
					game_addFloatIcon(12, lx * 16, (ly - 1) * 16);

					objmapf[curmap][lx][ly - 1] = 1;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

					game_eventtext("Found Mega Flask!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 10 && inventory[1] == 9) {
					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndchest], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					game_eventtext("Cannot Carry any more Mega Flasks!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 11 && inventory[2] < 9) {
					inventory[2] = inventory[2] + 1;
					if(inventory[2] > 9) inventory[2] = 9;
					game_addFloatIcon(17, lx * 16, (ly - 1) * 16);

					objmapf[curmap][lx][ly - 1] = 1;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

					game_eventtext("Found Lightning Bomb!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 11 && inventory[2] == 9) {
					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndchest], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					game_eventtext("Cannot Carry any more Lightning Bombs!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 12 && player.armour == 1) {
					player.armour = 2;

					game_addFloatIcon(5, lx * 16, (ly - 1) * 16);

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;

					game_eventtext("Found the Fidelis Mail!");
					itemticks = ticks + 215;
					return;
				}

				if(oscript == 60) {
					if(curmap == 58 && scriptflag[60][0] == 0) {
						scriptflag[60][0] = 1;

						if(menabled == 1 && opeffects == 1) {
							int snd = Mix_PlayChannel(-1, sfx[sndlever], 0);
							Mix_Volume(snd, opeffectsvol);
						}

					} else if(curmap == 58 && scriptflag[60][0] > 0) {
						if(menabled == 1 && opeffects == 1) {
							int snd = Mix_PlayChannel(-1, sfx[snddoor], 0);
							Mix_Volume(snd, opeffectsvol);
						}

						game_eventtext("It's stuck!");
					}

					if(curmap == 54 && scriptflag[60][0] == 1) {
						if(menabled == 1 && opeffects == 1) {
							int snd = Mix_PlayChannel(-1, sfx[sndlever], 0);
							Mix_Volume(snd, opeffectsvol);
						}

						scriptflag[60][0] = 2;
					} else if(curmap == 54 && scriptflag[60][0] > 1) {
						if(menabled == 1 && opeffects == 1) {
							int snd = Mix_PlayChannel(-1, sfx[snddoor], 0);
							Mix_Volume(snd, opeffectsvol);
						}

						game_eventtext("It's stuck!");
					}

				}

				if(oscript == 15 && player.sword < 3) {
					player.sword = 3;

					game_addFloatIcon(18, lx * 16, (ly - 1) * 16);

					itemticks = ticks + 215;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;
					game_eventtext("Found the Blood Sword!");
					objmapf[4][1][2] = 1;
					return;
				}

				if(oscript == 16 && player.shield < 3) {
					player.shield = 3;
					game_addFloatIcon(19, lx * 16, (ly - 1) * 16);
					itemticks = ticks + 215;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;
					game_eventtext("Found the Entropy Shield!");
					objmapf[4][1][2] = 1;
					return;
				}

				if(oscript == 17 && player.armour < 3) {
					player.armour = 3;
					game_addFloatIcon(20, lx * 16, (ly - 1) * 16);
					itemticks = ticks + 215;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					if(objectinfo[o][4] == 1) objmap[lx][ly - 1] = 3;
					game_eventtext("Found the Rubyscale Armour!");
					objmapf[4][1][2] = 1;
					return;
				}

			}
		}
	}

	attacking = 1;
	player.attackframe = 0;
	movingup = 0;
	movingdown = 0;
	movingleft = 0;
	movingright = 0;

	for(int i = 0; i <= 15; i++) {
		for(int a = 0; a <= 3; a++) {
			playerattackofs[a][i][2] = 0;
		}
	}
}

void game_castspell(int spellnum, float homex, float homey, float enemyx, float enemyy, int damagewho)
{
	// spellnum 7 = sprite 6 spitfire
	int i = 0;
	do {
		if(spellinfo[i].frame == 0) {
			spellinfo[i].homex = homex;
			spellinfo[i].homey = homey;
			spellinfo[i].enemyx = enemyx;
			spellinfo[i].enemyy = enemyy;
			spellinfo[i].spellnum = spellnum;
			int dw = 0;
			int npc = 0;
			if(damagewho > 0) {
				dw = 1;
				npc = damagewho;
			}

			spellinfo[i].damagewho = dw;
			spellinfo[i].npc = npc;

			spellinfo[i].frame = 32;
			if(damagewho == 0) {
				spellinfo[i].strength = player.spellstrength / 100;
				if(player.spellstrength == 100) spellinfo[i].strength = 1.5;
			}

			// set earthslide vars
			if(spellnum == 2) {
				for(int f = 0; f <= 8; f++) {
					spellinfo[i].rocky[f] = 0;
					spellinfo[i].rockimg[f] = (int)(RND() * 4);
					spellinfo[i].rockdeflect[f] = ((int)(RND() * 128) - 64) * 1.5;
				}
			}

			// set fire vars
			if(spellnum == 3) {
				for(int f = 0; f <= 4; f++) {
					spellinfo[i].legalive[f] = 32;
				}
			}


			// room fireball vars
			if(spellnum == 6) {
				int nballs = 0;
				for(int x = 0; x <= 19; x++) {
					for(int y = 0; y <= 14; y++) {
						if((objmap[x][y] == 1 || objmap[x][y] == 2) && nballs < 5 && (int)(RND() * 4) == 0) {
							int ax = x * 16;
							int ay = y * 16;
							/*int bx = player.px + 4; // useless code ??
							int by = player.py + 4;
							float d = sqr((bx - ax) ^ 2 + (by - ay) ^ 2);

							float tx = (bx - ax) / d;
							float ty = (by - ay) / d;*/

							spellinfo[i].fireballs[nballs][0] = ax;
							spellinfo[i].fireballs[nballs][1] = ay;
							spellinfo[i].fireballs[nballs][2] = 0;
							spellinfo[i].fireballs[nballs][3] = 0;

							spellinfo[i].ballon[nballs] = 1;
							nballs = nballs + 1;
						}
					}
				}
				spellinfo[i].nfballs = nballs;
			}

			if(menabled == 1 && opeffects == 1) {
				if(spellnum == 1) {
					int snd = Mix_PlayChannel(-1, sfx[sndthrow], 0);
					Mix_Volume(snd, opeffectsvol);
				} else if(spellnum == 5) {
					int snd = Mix_PlayChannel(-1, sfx[sndcrystal], 0);
					Mix_Volume(snd, opeffectsvol);
				} else if(spellnum == 8 || spellnum == 9) {
					int snd = Mix_PlayChannel(-1, sfx[sndlightning], 0);
					Mix_Volume(snd, opeffectsvol);
				}
			}

			return;
		}

		i = i + 1;
		if(i == MAXSPELL+1) break;
	} while(1);
}

void game_checkhit()
{
	/*unsigned int *temp, bgc;*/
	float opx, opy, npx, npy;
	float damage;

	if(attacking == 1) {
		for(int i = 1; i <= lastnpc; i++) {
			if(npcinfo[i].hp > 0 && npcinfo[i].pause < ticks && (int)(RND() * 2) == 0) {
				npx = npcinfo[i].x;
				npy = npcinfo[i].y;

				opx = npx;
				opy = npy;

				float xdif = player.px - npx;
				float ydif = player.py - npy;

				float ps = player.sword;
				if(ps > 1) ps = ps * 0.75;
					damage = (float)player.sworddamage * (1.0 + RND() * 1.0) * player.attackstrength / 100.0 * ps;
					if(player.attackstrength == 100) damage = damage * 1.5;

					int hit = 0;
					if(player.walkdir == 0) {
						if(abs(xdif) <= 8 && ydif >= 0 && ydif < 8) hit = 1;
					} else if(player.walkdir == 1) {
						if(abs(xdif) <= 8 && ydif <= 0 && ydif > -8) hit = 1;
					} else if(player.walkdir == 2) {
						if(abs(ydif) <= 8 && xdif >= -8 && xdif < 8) hit = 1;
					} else if(player.walkdir == 3) {
						if(abs(ydif) <= 8 && xdif <= 8 && xdif > -8) hit = 1;
					}

					if(hit == 1) {
						if(menabled == 1 && opeffects == 1) {
							int snd = Mix_PlayChannel(-1, sfx[sndswordhit], 0);
							Mix_Volume(snd, opeffectsvol);
						}

						game_damagenpc(i, damage, 0);
					}
			}
		}
	}
}

void game_checkinputs()
{
	int ntickdelay;
	//JoyGetPos (0, &joystickinfo)

	ntickdelay = 175;

	SDL_PollEvent(&event);
	keys = SDL_GetKeyState(NULL);

	nposts = 0;

	for(int i = 0; i <= 20; i++) {
		postinfo[i][0] = 0;
		postinfo[i][1] = 0;
	}

	for(int x = 0; x <= 19; x++) {
		for(int y = 0; y <= 14; y++) {
			int o = objmap[x][y];
			if(objectinfo[o][4] == 3) {
				postinfo[nposts][0] = x * 16;
				postinfo[nposts][1] = y * 16;
				nposts = nposts + 1;
			}
		}
	}

	if(attacking == 1 || (forcepause == 1 && itemselon == 0)) return;

	if(event.type == SDL_KEYDOWN) {
		switch(event.key.keysym.sym) {
		case SDLK_ESCAPE:
			if(itemticks < ticks) game_title(1);
			break;
		case SDLK_RETURN:
			if(keys[SDLK_LALT] || keys[SDLK_RALT]) {
				if(fullscreen & SDL_FULLSCREEN) {
					fullscreen = HWACCEL | HWSURFACE;
				} else {
					fullscreen = SDL_FULLSCREEN | HWACCEL | HWSURFACE;
				}

				opfullscreen = fullscreen & SDL_FULLSCREEN;

				video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen);
				SDL_UpdateRect(video, 0, 0, SCR_WIDTH, SCR_HEIGHT);
			}
			break;

		case SDLK_SPACE:
			if(itemselon == 0 && itemticks < ticks) game_attack();

			//SDL_SaveBMP(video, "shot.bmp");

			if(itemselon == 1 && itemticks < ticks) {
				if(curitem == 0 && inventory[0] > 0) {
					itemticks = ticks + ntickdelay;

					int heal = 50;
					int maxh = player.maxhp - player.hp;

					if(heal > maxh) heal = maxh;

					player.hp = player.hp + heal;

					//t$ = "+" + ltrim$(rtrim$(str$(heal)], 0);
					char text[256];
					sprintf(text, "+%i", heal);
					game_addFloatText(text, player.px + 16 - 4 * strlen(text), player.py + 16, 5);

					inventory[0] = inventory[0] - 1;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					itemselon = 0;
					forcepause = 0;
				}

				if(curitem == 1 && inventory[1] > 0) {
					itemticks = ticks + ntickdelay;

					int heal = 200;
					int maxh = player.maxhp - player.hp;

					if(heal > maxh) heal = maxh;

					player.hp = player.hp + heal;

					//t$ = "+" + ltrim$(rtrim$(str$(heal)], 0);
					char text[256];
					sprintf(text, "+%i", heal);
					game_addFloatText(text, player.px + 16 - 4 * strlen(text), player.py + 16, 5);

					inventory[1] = inventory[1] - 1;

					if(menabled == 1 && opeffects == 1) {
						int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
						Mix_Volume(snd, opeffectsvol);
					}

					itemselon = 0;
					forcepause = 0;

				}

				if(curitem == 2 && inventory[2] > 0) {
					game_castspell(8, player.px, player.py, npcinfo[curenemy].x, npcinfo[curenemy].y, 0);

					forcepause = 1;

					inventory[2] = inventory[2] - 1;

					itemticks = ticks + ntickdelay;
					selenemyon = 0;
					itemselon = 0;

				}

				if(curitem == 3 && inventory[3] > 0 && canusekey == 1 && locktype == 1) {
					roomlocks[roomtounlock] = 0;
					game_eventtext("UnLocked!");

					inventory[3] = inventory[3] - 1;

					itemticks = ticks + ntickdelay;
					selenemyon = 0;
					itemselon = 0;
					return;
				}

				if(curitem == 4 && inventory[4] > 0 && canusekey == 1 && locktype == 2) {
					roomlocks[roomtounlock] = 0;
					game_eventtext("UnLocked!");

					inventory[4] = inventory[4] - 1;

					itemticks = ticks + ntickdelay;
					selenemyon = 0;
					itemselon = 0;
					return;
				}

				if(curitem == 5 && player.crystalcharge == 100) {
					game_castspell(5, player.px, player.py, npcinfo[curenemy].x, npcinfo[curenemy].y, 0);

					player.crystalcharge = 0;

					forcepause = 1;

					itemticks = ticks + ntickdelay;
					selenemyon = 0;
					itemselon = 0;
				}

				if(curitem > 5 && selenemyon == 1) {
					if(curenemy <= lastnpc) {
						game_castspell(curitem - 6, player.px, player.py, npcinfo[curenemy].x, npcinfo[curenemy].y, 0);
					} else {
						int pst = curenemy - lastnpc - 1;
						game_castspell(curitem - 6, player.px, player.py, postinfo[pst][0], postinfo[pst][1], 0);
					}

					player.spellcharge[curitem - 6] = 0;

					player.spellstrength = 0;

					itemticks = ticks + ntickdelay;
					selenemyon = 0;
					itemselon = 0;
					forcepause = 0;
				}

				if(curitem > 5 && selenemyon == 0 && itemselon == 1) {
					if(player.spellcharge[curitem - 6] == 100) {
						itemticks = ticks + ntickdelay;

						selenemyon = 1;

						int i = 0;
						do {
							if(npcinfo[i].hp > 0) {
								curenemy = i;
								goto __exit_do;
							}
							i = i + 1;
							if(i == lastnpc + 1) {
								selenemyon = 0;
								goto __exit_do;
							}
						} while(1);
					__exit_do:

						if(nposts > 0 && selenemyon == 0) {
							selenemyon = 1;
							curenemy = lastnpc + 1;
						}
					}

				}
			}
			break;

		case SDLK_LCTRL:
			if(itemticks < ticks) {
				selenemyon = 0;
				if(itemselon == 1) {
					itemselon = 0;
					itemticks = ticks + 220;
					forcepause = 0;
				} else {
					itemselon = 1;
					itemticks = ticks + 220;
					forcepause = 1;
					player.itemselshade = 0;
				}
			}
			break;

		case SDLK_RCTRL:
			if(itemticks < ticks) {
				selenemyon = 0;
				if(itemselon == 1) {
					itemselon = 0;
					itemticks = ticks + 220;
					forcepause = 0;
				} else {
					itemselon = 1;
					itemticks = ticks + 220;
					forcepause = 1;
					player.itemselshade = 0;
				}
			}
			break;

		default:
			;
		}
	}

	if(itemselon == 0) {
		movingup = 0;
		movingdown = 0;
		movingleft = 0;
		movingright = 0;
		if(keys[SDLK_UP]) movingup = 1;
		if(keys[SDLK_DOWN]) movingdown = 1;
		if(keys[SDLK_LEFT]) movingleft = 1;
		if(keys[SDLK_RIGHT]) movingright = 1;
	} else {
		movingup = 0;
		movingdown = 0;
		movingleft = 0;
		movingright = 0;

		if(selenemyon == 1) {
			if(itemticks < ticks) {
				if(keys[SDLK_LEFT]) {
					int origin = curenemy;
					do {
						curenemy = curenemy - 1;
						if(curenemy < 1) curenemy = lastnpc + nposts;
						if(curenemy == origin) break;
						if(curenemy <= lastnpc && npcinfo[curenemy].hp > 0) break;
						if(curenemy > lastnpc) break;
					} while(1);
					itemticks = ticks + ntickdelay;
				}
				if(keys[SDLK_RIGHT]) {
					int origin = curenemy;
					do {
						curenemy = curenemy + 1;
						if(curenemy > lastnpc + nposts) curenemy = 1;
						if(curenemy == origin) break;
						if(curenemy <= lastnpc && npcinfo[curenemy].hp > 0) break;
						if(curenemy > lastnpc) break;
					} while(1);
					itemticks = ticks + ntickdelay;
				}


				if(curenemy > lastnpc + nposts) curenemy = 1;
				if(curenemy < 1) curenemy = lastnpc + nposts;
			}
		} else {
			if(itemticks < ticks) {
				if(keys[SDLK_UP]) {
					curitem = curitem - 1;
					itemticks = ticks + ntickdelay;
					if(curitem == 4) curitem = 9;
					if(curitem == -1) curitem = 4;
				}
				if(keys[SDLK_DOWN]) {
					curitem = curitem + 1;
					itemticks = ticks + ntickdelay;
					if(curitem == 5) curitem = 0;
					if(curitem == 10) curitem = 5;
				}
				if(keys[SDLK_LEFT]) {
					curitem = curitem - 5;
					itemticks = ticks + ntickdelay;
				}
				if(keys[SDLK_RIGHT]) {
					curitem = curitem + 5;
					itemticks = ticks + ntickdelay;
				}

				if(curitem > 9) curitem = curitem - 10;
				if(curitem < 0) curitem = curitem + 10;
			}
		}
	}
}

void game_checktrigger()
{
	int npx, npy, lx, ly;

	npx = player.px + 12;
	npy = player.py + 20;

	lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
	ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

	canusekey = 0;

	if(triggerloc[lx][ly] > -1) game_processtrigger(triggerloc[lx][ly]);
}

void game_configmenu()
{
	SDL_Surface *configwindow;
	int cursel, curselt, ofullscreen;
	int tickwait, keypause, ticks1;
	int la = 0, lb = 0;

	cursel = 0;

	ticks = SDL_GetTicks();
	tickwait = 100;
	keypause = ticks + tickwait;

	configwindow = SDL_DisplayFormat(videobuffer);

	configwindow = IMG_Load("art/configwindow.bmp");
	SDL_SetColorKey(configwindow, SDL_SRCCOLORKEY, SDL_MapRGB(configwindow->format, 255, 0, 255));
	SDL_SetAlpha(configwindow, SDL_SRCALPHA, 160);

	ticks1 = ticks;
	do {
		rc.x = 0;
		rc.y = 0;

		rc2.x = SCR_TOPX;
		rc2.y = SCR_TOPY;
		rc2.w = 320;
		rc2.h = 240;

		SDL_FillRect(videobuffer, NULL, 0);

		rcDest.x = 256 + 256 * cos(3.141592 / 180 * clouddeg * 40);
		rcDest.y = 192 + 192 * sin(3.141592 / 180 * clouddeg * 40);
		rcDest.w = 320;
		rcDest.h = 240;

		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 128);
		SDL_BlitSurface(cloudimg, &rcDest, videobuffer, NULL);
		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 64);

		rcDest.x = 256;
		rcDest.y = 192;
		rcDest.w = 320;
		rcDest.h = 240;

		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 128);
		SDL_BlitSurface(cloudimg, &rcDest, videobuffer, NULL);
		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 64);

		SDL_BlitSurface(configwindow, NULL, videobuffer, NULL);

		int sy = 38 + (240 - 38) / 2 - 88;

		for(int i = 0; i <= 21; i++) {
			static char *vr[22] = {
				"Resolution:", "",
				"Bit Depth:", "", "", "",
				"Start Fullscreen:", "", "",
				"Music:", "", "",
				"Sound Effects:", "", "",
				"Music Volume:", "",
				"Effects Volume:", "", "", "", ""
			};
			static char *vl[22] = {
				"320x240", "640x480",
				"16", "24", "32", "",
				"Yes", "No", "",
				"On", "Off", "",
				"On", "Off", "",
				"[----------]", "",
				"[----------]", "",
				"Exit + Save", "",
				"Exit"
			};
			static char line[24];

			if(i == 15 || i == 17) {
				int vol = (i == 15 ? opmusicvol : opeffectsvol) * 9 / 255;
				if(vol < 0) vol = 0;
				if(vol > 9) vol = 9;

				strcpy(line, "[----------]");
				line[vol+1] = 'X';
				vl[i] = line;
			}

			int cl = 3;
			if(i == 0 && SCR_WIDTH == 320) cl = 0;
			if(i == 1 && SCR_WIDTH == 640) cl = 0;
			if(i == 2 && SCR_BITS == 16) cl = 0;
			if(i == 3 && SCR_BITS == 24) cl = 0;
			if(i == 4 && SCR_BITS == 32) cl = 0;
			if(i == 6 && opfullscreen != 0) cl = 0;
			if(i == 7 && opfullscreen == 0) cl = 0;
			if(i == 9 && opmusic == 1) cl = 0;
			if(i == 10 && opmusic == 0) cl = 0;
			if(i == 12 && opeffects == 1) cl = 0;
			if(i == 13 && opeffects == 0) cl = 0;

			if(i > 18) cl = 0;

			sys_print(videobuffer, vr[i], 156 - 8 * strlen(vr[i]), sy + i * 8, 0);
			sys_print(videobuffer, vl[i], 164, sy + i * 8, cl);
		}

		//int y = 156;
		//int x = 160 - 9 * 4;

		curselt = cursel;
		if(cursel > 4) curselt = curselt + 1;
		if(cursel > 6) curselt = curselt + 1;
		if(cursel > 8) curselt = curselt + 1;
		if(cursel > 10) curselt = curselt + 1;
		if(cursel > 11) curselt = curselt + 1;
		if(cursel > 12) curselt = curselt + 1;
		if(cursel > 13) curselt = curselt + 1;

		rc.x = 148 + 3 * cos(3.14159 * 2 * itemyloc / 16.0);
		rc.y = sy + 8 * curselt - 4;

		SDL_BlitSurface(itemimg[15], NULL, videobuffer, &rc);

		rc.x = 0;
		rc.y = 0;
		rc.w = 320;
		rc.h = 240;

		float yy = 255.0;
		if(ticks < ticks1 + 1000) {
			yy = 255.0 * ((float)(ticks - ticks1) / 1000.0);
			if(yy < 0.0) yy = 0.0;
			if(yy > 255.0) yy = 255.0;
		}

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, (int)yy);
		SDL_BlitSurface(videobuffer, &rc, video, &rc2);
		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		itemyloc += 0.75 * fpsr;
		while(itemyloc >= 16) itemyloc -= 16;

		if(keypause < ticks) {
			SDL_PollEvent(&event);
			keys = SDL_GetKeyState(NULL);

			if(event.type == SDL_KEYDOWN) {
				keypause = ticks + tickwait;

				if(keys[SDLK_ESCAPE]) break;
				if(cursel == 11 || cursel == 12) {
					la = Mix_Volume(menuchannel, -1);
					lb = Mix_Volume(musicchannel, -1);

					if(keys[SDLK_LEFT]) {
						if(cursel == 11) {
							opmusicvol = opmusicvol - 25;
							if(opmusicvol < 0) opmusicvol = 0;

							Mix_Volume(musicchannel, opmusicvol);
							Mix_Volume(menuchannel, opmusicvol);
						} else if(cursel == 12) {
							opeffectsvol = opeffectsvol - 25;
							if(opeffectsvol < 0) opeffectsvol = 0;

							Mix_Volume(-1, opeffectsvol);
							Mix_Volume(musicchannel, opmusicvol);
							Mix_Volume(menuchannel, opmusicvol);

							if(menabled == 1 && opeffects == 1) {
								int snd = Mix_PlayChannel(-1, sfx[snddoor], 0);
								Mix_Volume(snd, opeffectsvol);
							}
						}
					}
					if(keys[SDLK_RIGHT]) {
						if(cursel == 11) {
							opmusicvol = opmusicvol + 25;
							if(opmusicvol > 255) opmusicvol = 255;

							Mix_Volume(musicchannel, opmusicvol);
							Mix_Volume(menuchannel, opmusicvol);
						} else if(cursel == 12) {
							opeffectsvol = opeffectsvol + 25;
							if(opeffectsvol > 255) opeffectsvol = 255;

							Mix_Volume(-1, opeffectsvol);
							Mix_Volume(musicchannel, opmusicvol);
							Mix_Volume(menuchannel, opmusicvol);

							if(menabled == 1 && opeffects == 1) {
								int snd = Mix_PlayChannel(-1, sfx[snddoor], 0);
								Mix_Volume(snd, opeffectsvol);
							}
						}
					}
				}

				if(keys[SDLK_UP]) cursel = cursel - 1;
				if(keys[SDLK_DOWN]) cursel = cursel + 1;
				//if(event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) {
				if(keys[SDLK_SPACE] || keys[SDLK_RETURN]) {
					if(cursel == 0) {
						fullscreen = opfullscreen | HWACCEL | HWSURFACE;

						video = SDL_SetVideoMode(320, 240, SCR_BITS, fullscreen);
						if(video == 0) {
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen);
						} else {
							SCR_WIDTH = 320;
							SCR_HEIGHT = 240;
							SCR_TOPX = 0;
							SCR_TOPY = 0;
						}

						SDL_UpdateRect(video, 0, 0, SCR_WIDTH, SCR_HEIGHT);
					}
					if(cursel == 1) {
						fullscreen = opfullscreen | HWACCEL | HWSURFACE;

						video = SDL_SetVideoMode(640, 480, SCR_BITS, fullscreen);
						if(video == 0) {
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen);
						} else {
							SCR_WIDTH = 640;
							SCR_HEIGHT = 480;
							SCR_TOPX = 160;
							SCR_TOPY = 120;
						}

						SDL_UpdateRect(video, 0, 0, SCR_WIDTH, SCR_HEIGHT);
					}
					if(cursel == 2 || cursel == 3 || cursel == 4) {
						fullscreen = opfullscreen | HWACCEL | HWSURFACE;

						int b = 16;
						if(cursel == 3) b = 24;
						if(cursel == 4) b = 32;
						video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, b, fullscreen);
						if(video == 0) {
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen);
						} else {
							SCR_BITS = b;
						}

						SDL_UpdateRect(video, 0, 0, SCR_WIDTH, SCR_HEIGHT);
					}
					if(cursel == 5) {
						ofullscreen = opfullscreen | HWACCEL | HWSURFACE;
						fullscreen = SDL_FULLSCREEN | HWACCEL | HWSURFACE;

						video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen);
						if(video == 0) {
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, ofullscreen);
						} else {
							opfullscreen = SDL_FULLSCREEN;
						}

						SDL_UpdateRect(video, 0, 0, SCR_WIDTH, SCR_HEIGHT);
					}
					if(cursel == 6) {
						ofullscreen = opfullscreen | HWACCEL | HWSURFACE;
						fullscreen = 0 | HWACCEL | HWSURFACE;

						video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen);
						if(video == 0) {
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, ofullscreen);
						} else {
							opfullscreen = 0;
						}

						SDL_UpdateRect(video, 0, 0, SCR_WIDTH, SCR_HEIGHT);
					}
					if(cursel == 7 && opmusic == 0) {
						opmusic = 1;
						if(menabled == 1) {
							menuchannel = Mix_PlayChannel(-1, mmenu, -1);
							Mix_Volume(menuchannel, opmusicvol);
						}
					}
					if(cursel == 8 && opmusic == 1) {
						opmusic = 0;
						Mix_HaltChannel(musicchannel);
						Mix_HaltChannel(menuchannel);
					}
					if(cursel == 9 && opeffects == 0) {
						opeffects = 1;
						if(menabled == 1) {
							int snd = Mix_PlayChannel(-1, sfx[snddoor], 0);
							Mix_Volume(snd, opeffectsvol);
						}
					}

					if(cursel == 10 && opeffects == 1) opeffects = 0;

					if(cursel == 13) {
						FILE *fp = fopen("config.ini", "w");

						PRINT("%s", "SCR_WIDTH:");
						PRINT("%i", SCR_WIDTH);
						PRINT("%s", "SCR_HEIGHT:");
						PRINT("%i", SCR_HEIGHT);
						PRINT("%s", "SCR_BITS:");
						PRINT("%i", SCR_BITS);
						PRINT("%s", HWACCEL ? "HWACCEL:YES" : "HWACCEL:NO");
						PRINT("%s", HWSURFACE ? "HWSURFACE:YES" : "HWSURFACE:NO");
						PRINT("%s", opfullscreen ? "FULLSCREEN:YES" : "FULLSCREEN:NO");
						PRINT("%s", opmusic ? "MUSIC:YES" : "MUSIC:NO");
						PRINT("%s", opeffects ? "SNDEFFECTS:YES" : "SNDEFFECTS:NO");
						PRINT("%s", "opmusicvol:");
						PRINT("%i", opmusicvol);
						PRINT("%s", "opeffectsvol:");
						PRINT("%i", opeffectsvol);
						fclose(fp);

						break;
					}

					if(cursel == 14) {
						// reset keys to avoid returning
						keys[SDLK_SPACE] = keys[SDLK_RETURN] = 0;
						break;
					}
				}

				if(cursel == -1) cursel = 14;
				if(cursel == 15) cursel = 0;
			}
		}

		clouddeg += 0.01 * fpsr;
		while(clouddeg >= 360) clouddeg = clouddeg - 360;

		SDL_Delay(10);
	} while(1);

	SDL_FreeSurface(configwindow);
	itemticks = ticks + 210;

	SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 64);
}

void game_damagenpc(int npcnum, int damage, int spell)
{
	float npx, npy;
	int lx, ly, cx, cy, alive;
	char line[256];
	float ratio;
	int fcol, heal, ff;

	if(damage == 0) {
		strcpy(line, "miss!");
		fcol = 2;
	} else {
		ratio = 0;
		heal = 0;
		if(damage < 0) heal = 1;
		if(damage < 0) damage = -damage;

		if(heal == 0) {
			if(damage > npcinfo[npcnum].hp) {
				ratio = (damage - npcinfo[npcnum].hp) / damage;
				damage = npcinfo[npcnum].hp;
			}
		} else {
			if(damage > npcinfo[npcnum].maxhp - npcinfo[npcnum].hp) damage = npcinfo[npcnum].maxhp - npcinfo[npcnum].hp;
		}

		npcinfo[npcnum].pause = ticks + 900;
		if(npcinfo[npcnum].spriteset == 11) npcinfo[npcnum].pause = ticks + 900;

		if(heal == 0) npcinfo[npcnum].hp = npcinfo[npcnum].hp - damage;
		if(heal == 1) npcinfo[npcnum].hp = npcinfo[npcnum].hp + damage;

		if(npcinfo[npcnum].hp > npcinfo[npcnum].maxhp) npcinfo[npcnum].hp = npcinfo[npcnum].maxhp;

		sprintf(line, "-%i", damage);
		fcol = 1;
		if(spell == 0) player.attackstrength = ratio * 100;

		if(heal == 1) {
			sprintf(line, "+%i", damage);
			fcol = 5;
		}
	}

	game_addFloatText(line, npcinfo[npcnum].x + 12 - 4 * strlen(line), npcinfo[npcnum].y + 16, fcol);


	if(npcinfo[npcnum].spriteset == 12) game_castspell(9, npcinfo[npcnum].x, npcinfo[npcnum].y, player.px, player.py, npcnum);

	if(npcinfo[npcnum].hp < 0) npcinfo[npcnum].hp = 0;

	if(npcinfo[npcnum].hp == 0) {
		player.exp = player.exp + npcinfo[npcnum].maxhp;

		if(npcinfo[npcnum].spriteset == 1 || npcinfo[npcnum].spriteset == 7 || npcinfo[npcnum].spriteset == 6) {
			ff = (int)(RND() * player.level * 3);
			if(ff == 0) {
				npx = npcinfo[npcnum].x + 12;
				npy = npcinfo[npcnum].y + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(objmap[lx][ly] == -1) objmap[lx][ly] = 4;
			}
		}

		if(npcinfo[npcnum].spriteset == 2 || npcinfo[npcnum].spriteset == 9 || npcinfo[npcnum].spriteset == 4 || npcinfo[npcnum].spriteset == 5) {
			ff = (int)(RND() * player.level);
			if(ff == 0) {
				npx = npcinfo[npcnum].x + 12;
				npy = npcinfo[npcnum].y + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(objmap[lx][ly] == -1) objmap[lx][ly] = 12;
			}
		}

		if(npcinfo[npcnum].spriteset == 9 || npcinfo[npcnum].spriteset == 10 || npcinfo[npcnum].spriteset == 5) {
			ff = (int)(RND() * player.level * 2);
			if(ff == 0) {
				npx = npcinfo[npcnum].x + 12;
				npy = npcinfo[npcnum].y + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(objmap[lx][ly] == -1) objmap[lx][ly] = 13;
			}
		}

		// academy master key chest script
		if(npcinfo[npcnum].script == 2) {
			cx = 9;
			cy = 7;

			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				objmap[cx][cy] = 5;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
				scriptflag[2][0] = 1;
			}
		}

		// academy crystal chest script
		if(npcinfo[npcnum].script == 3) {
			cx = 9;
			cy = 7;

			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				objmap[cx][cy] = 6;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				scriptflag[3][0] = 1;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
			}
		}

		// tower shield chest script
		if(npcinfo[npcnum].script == 4 && scriptflag[4][0] == 0) {
				triggerloc[9][7] = 5004;

				int curtile = 40;
				int curtilel = 0;
				int curtilex = curtile % 20;
				int curtiley = (curtile - curtilex) / 20;

				int l = 0; // ?? not defined in original code
				tileinfo[l][9][7][0] = curtile + 1;
				tileinfo[l][9][7][1] = 0;

				rcSrc.x = curtilex * 16;
				rcSrc.y = curtiley * 16;
				rcSrc.w = 16;
				rcSrc.h = 16;

				rcDest.x = 9 * 16;
				rcDest.y = 7 * 16;
				rcDest.w = 16;
				rcDest.h = 16;

				SDL_BlitSurface(tiles[curtilel], &rcSrc, mapbg, &rcDest);
		}

		// firehydra sword chest
		if(npcinfo[npcnum].script == 5) {
			cx = 9;
			cy = 6;

			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				objmap[cx][cy] = 9;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				scriptflag[5][0] = 1;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
			}

		}

		// gardens master key script
		if(npcinfo[npcnum].script == 8 && scriptflag[6][0] == 0) {
			cx = 13;
			cy = 7;

			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				objmap[cx][cy] = 5;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
				scriptflag[8][0] = 1;
			}
		}

		// regular key chest 1
		for(int s = 20; s <= 23; s++) {
			if(npcinfo[npcnum].script == s && scriptflag[s][0] < 2) {
				cx = 9;
				cy = 7;

				alive = 0;
				for(int i = 1; i <= lastnpc; i++) {
					if(npcinfo[i].hp > 0) alive = 1;
				}

				if(alive == 0) {
					objmap[cx][cy] = 11;

					rcDest.x = cx * 8;
					rcDest.y = cy * 8;
					rcDest.w = 8;
					rcDest.h = 8;

					npx = player.px + 12;
					npy = player.py + 20;

					lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
					ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

					if(lx == cx && ly == cy) player.py = player.py + 16;
					scriptflag[s][0] = 1;
					SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
				}
			}
		}

		// pickup lightning bomb
		if(npcinfo[npcnum].script == 9 && (curmap == 41 && scriptflag[9][1] == 0)) {
			cx = 9;
			cy = 7;

			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				objmap[cx][cy] = 13;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
			}
		}

		// citadel armour chest
		if(npcinfo[npcnum].script == 12) {
			cx = 8;
			cy = 7;

			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				objmap[cx][cy] = 16;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				scriptflag[12][0] = 1;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
			}
		}

		// citadel master key script
		if(npcinfo[npcnum].script == 13 && scriptflag[13][0] == 0) {
			cx = 11;
			cy = 10;

			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				objmap[cx][cy] = 5;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
				scriptflag[13][0] = 1;
			}
		}

		// max ups
		if(npcinfo[npcnum].script == 15 && scriptflag[15][0] == 0) {
			alive = 0;
			for(int i = 1; i <= lastnpc; i++) {
				if(npcinfo[i].hp > 0) alive = 1;
			}

			if(alive == 0) {
				cx = 6;
				cy = 8;

				objmap[cx][cy] = 18;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
				scriptflag[15][0] = 1;

				cx = 9;
				cy = 8;

				objmap[cx][cy] = 19;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));

				scriptflag[16][0] = 1;

				cx = 12;
				cy = 8;

				objmap[cx][cy] = 20;

				rcDest.x = cx * 8;
				rcDest.y = cy * 8;
				rcDest.w = 8;
				rcDest.h = 8;

				npx = player.px + 12;
				npy = player.py + 20;

				lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
				ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

				if(lx == cx && ly == cy) player.py = player.py + 16;
				SDL_FillRect(clipbg2, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
				scriptflag[17][0] = 1;
			}
		}

		if(npcinfo[npcnum].script == 14) game_endofgame();
	}
}

void game_damageplayer(int damage)
{
	char line[256];

	player.hp -= damage;
	if(player.hp < 0) player.hp = 0;

	sprintf(line, "-%i", damage);
	if(damage == 0) strcpy(line, "miss!");

	game_addFloatText(line, player.px + 12 - 4 * strlen(line), player.py + 16, 4);

	player.pause = ticks + 1000;
}

void game_drawanims(int Layer)
{
	for(int sx = 0; sx <= 19; sx++) {
		for(int sy = 0; sy <= 14; sy++) {
			int o = objmap[sx][sy];

			if(o > -1) {
				int xtiles = objectinfo[o][1];
				int ytiles = objectinfo[o][2];
				int cframe = objectframe[o][1];

				for(int x = 0; x <= xtiles - 1; x++) {
					for(int y = 0; y <= ytiles - 1; y++) {
						int x1 = (sx + x) * 16;
						int y1 = (sy + y) * 16;

						if(objecttile[o][cframe][x][y][1] == Layer) {
							int c = objecttile[o][cframe][x][y][0];
							c = c - 1;
							int curtilel = 3;
							int curtilex = c % 20;
							int curtiley = (c - curtilex) / 20;

							if(curmap == 58 && scriptflag[60][0] > 0) curtilex = 1;
							if(curmap == 54 && scriptflag[60][0] > 1) curtilex = 1;
							rcSrc.x = curtilex * 16;
							rcSrc.y = curtiley * 16;
							rcSrc.w = 16;
							rcSrc.h = 16;

							rcDest.x = x1;
							rcDest.y = y1;
							rcDest.w = 16;
							rcDest.h = 16;

							SDL_BlitSurface(tiles[curtilel], &rcSrc, videobuffer, &rcDest);
						}

						if(Layer == 1) {
							for(int l = 1; l <= 2; l++) {
								int c = tileinfo[l][sx + x][sy + y][0];
								if(c > 0) {
									int cl = tileinfo[l][sx + x][sy + y][1];

									c = c - 1;
									int curtile = c;
									int curtilel = cl;
									int curtilex = c % 20;
									int curtiley = (c - curtilex) / 20;

									rcSrc.x = curtilex * 16;
									rcSrc.y = curtiley * 16;
									rcSrc.w = 16;
									rcSrc.h = 16;

									rcDest.x = (sx + x) * 16;
									rcDest.y = (sy + y) * 16;
									rcDest.w = 16;
									rcDest.h = 16;

									int pass = 1;
									if(curtilel == 1) {
										for(int ff = 0; ff <= 5; ff++) {
											int ffa = 20 * 5 - 1 + ff * 20;
											int ffb = 20 * 5 + 4 + ff * 20;
											if(curtile > ffa && curtile < ffb) pass = 0;
										}
									}

									if(pass == 1) SDL_BlitSurface(tiles[curtilel], &rcSrc, videobuffer, &rcDest);
								}
							}
						}
					}
				}
			}
		}
	}
}

void game_drawhud()
{
	char line[128];
	sprintf(line, "fps: %i, map: %i, fl: %i", (int)fps, curmap, scriptflag[60][0]);
	sys_print(videobuffer, line, 0, 0, 0);

	sprintf(line, "px: %f, py: %f", player.px, player.py);
	sys_print(videobuffer, line, 0, 8, 0);

	long ccc;

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = 320;
	rcSrc.h = 240;

	SDL_FillRect(videobuffer2, &rcSrc, 0);

	for(int i = 0; i <= MAXFLOAT; i++) {
		if(floattext[i][0] > 0) {
			int fc = (int)floattext[i][3];

			if(fc == 1 || fc == 3) {
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) - 1, 2);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) + 1, 2);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) - 1, (int)(floattext[i][2]) + 0, 2);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 1, (int)(floattext[i][2]) + 0, 2);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]), (int)(floattext[i][2]), fc);
			}
			if(fc == 2) {
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) - 1, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) + 1, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) - 1, (int)(floattext[i][2]) + 0, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 1, (int)(floattext[i][2]) + 0, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]), (int)(floattext[i][2]), fc);
			}

			if(fc == 4) {
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) - 1, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) + 1, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) - 1, (int)(floattext[i][2]) + 0, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 1, (int)(floattext[i][2]) + 0, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]), (int)(floattext[i][2]), 1);
			}

			if(fc == 5) {
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) - 1, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 0, (int)(floattext[i][2]) + 1, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) - 1, (int)(floattext[i][2]) + 0, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]) + 1, (int)(floattext[i][2]) + 0, 3);
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]), (int)(floattext[i][2]), 0);
			}


			if(fc == 0) {
				sys_print(videobuffer, floatstri[i], (int)(floattext[i][1]), (int)(floattext[i][2]), fc);
			}
		}

		if(floaticon[i][0] > 0) {
			int ico = floaticon[i][3];
			int ix = floaticon[i][1];
			int iy = floaticon[i][2];

			rcDest.x = ix;
			rcDest.y = iy;

			if(ico != 99) SDL_BlitSurface(itemimg[ico], NULL, videobuffer, &rcDest);
			if(ico == 99) {
				SDL_SetAlpha(spellimg, SDL_SRCALPHA, (int)(RND() * 96) + 96);

				rcSrc.x = 16 * (int)(RND() * 2);
				rcSrc.y = 80;
				rcSrc.w = 16;
				rcSrc.h = 16;

				rcDest.x = ix;
				rcDest.y = iy;

				SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);

				SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);
			}
		}
	}

	if(itemselon == 0) {
		int sy = 211;


		int nx = 19 * 8 + 13;
		rcSrc.x = nx - 17 + 48;
		rcSrc.y = sy;

		if(player.foundcrystal == 1) {
			rcSrc.x = rcSrc.x + 17;

			SDL_BlitSurface(itemimg[7], NULL, videobuffer, &rcSrc);

			ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32);

			rcSrc2.x = rcSrc.x;
			rcSrc2.y = sy + 16;
			rcSrc2.w = 16;
			rcSrc2.h = 4;

			SDL_FillRect(videobuffer, &rcSrc2, ccc);

			ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64);
			if(player.crystalcharge == 100) ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32);

			int mx = player.crystalcharge / 100 * 14;
			if(mx > 14) mx = 14;

			rcSrc2.x = rcSrc.x + 1;
			rcSrc2.y = sy + 17;
			rcSrc2.w = mx;
			rcSrc2.h = 2;

			SDL_FillRect(videobuffer, &rcSrc2, ccc);


			for(int i = 0; i <= 4; i++) {
				rcSrc.x = rcSrc.x + 17;

				if(player.foundspell[i] == 1) {
					SDL_BlitSurface(itemimg[8 + i], NULL, videobuffer, &rcSrc);

					ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32);

					rcSrc2.x = rcSrc.x;
					rcSrc2.y = sy + 16;
					rcSrc2.w = 16;
					rcSrc2.h = 4;

					SDL_FillRect(videobuffer, &rcSrc2, ccc);

					ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64);
					if(player.spellcharge[i] == 100) ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32);

					mx = player.spellcharge[i] / 100 * 14;
					if(mx > 14) mx = 14;

					rcSrc2.x = rcSrc.x + 1;
					rcSrc2.y = sy + 17;
					rcSrc2.w = mx;
					rcSrc2.h = 2;

					SDL_FillRect(videobuffer, &rcSrc2, ccc);
				}
			}
		}
		return;
	}

	if(selenemyon == 0) {
		rcDest.x = 0;
		rcDest.y = 0;
		rcDest.w = 320;
		rcDest.h = 240;
		rc2.x = 0;
		rc2.y = 0;
		SDL_SetAlpha(videobuffer2, SDL_SRCALPHA, (int)(player.itemselshade * 4));
		SDL_FillRect(videobuffer2, &rcDest, 0);
		SDL_BlitSurface(videobuffer2, NULL, videobuffer, &rc2);

		int sy = 202;
		rcSrc.x = 46;
		rcSrc.y = 46;

		SDL_SetAlpha(inventoryimg, SDL_SRCALPHA, 160); // 128
		SDL_BlitSurface(inventoryimg, NULL, videobuffer, &rcSrc);
		SDL_SetAlpha(inventoryimg, SDL_SRCALPHA, 255);

		int sx = 54;
		sy = 55;

		// draw map 9,77
		rcDest.x = 46 + 9;
		rcDest.y = 46 + 77;

		int amap = 0;
		if(curmap > 46) amap = 2;
		if(curmap > 67) amap = 3;
		if(curmap > 5 && curmap < 42) amap = 1;
		SDL_BlitSurface(mapimg[amap], NULL, videobuffer, &rcDest);

		ccc = SDL_MapRGB(videobuffer->format, 128 + 127 * sin(3.141592 * 2 * itemyloc / 16), 0, 0);

		for(int b = 0; b <= 6; b++) {
			for(int a = 0; a <= 12; a++) {
				if(invmap[amap][b][a] == curmap) {
					rcSrc2.x = 46 + 9 + a * 9 + 2;
					rcSrc2.y = 46 + 77 + b * 9 + 1;
					rcSrc2.w = 6;
					rcSrc2.h = 6;
				}
			}
		}

		SDL_FillRect(videobuffer, &rcSrc2, ccc);

		if(amap == 1) {
			sys_print(videobuffer, "L1", 46 + 9, 46 + 77, 0);
			sys_print(videobuffer, "L2", 46 + 9 + 7 * 9, 46 + 77, 0);
		}

		int cc = 0;
		if(player.hp <= player.maxhp * 0.25) cc = (int)(player.hpflash);

		sprintf(line, "Health: %i/%i", player.hp, player.maxhp);
		sys_print(videobuffer, line, sx, sy, cc);

		sprintf(line, "Level : %i", player.level);
		if(player.level == 22) strcpy(line, "Level : MAX");
		sys_print(videobuffer, line, sx, sy + 8, 0);

		int mx = player.exp / player.level * 14;
		if(mx > 14) mx = 14;

		ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32);

		rcSrc2.x = sx + 64;
		rcSrc2.y = sy + 16;
		rcSrc2.w = 16;
		rcSrc2.h = 4;

		SDL_FillRect(videobuffer, &rcSrc2, ccc);

		ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64);

		rcSrc2.x = sx + 65;
		rcSrc2.y = sy + 17;
		rcSrc2.w = mx;
		rcSrc2.h = 2;

		SDL_FillRect(videobuffer, &rcSrc2, ccc);

		mx = player.attackstrength / 100 * 54;
		if(mx > 54) mx = 54;

		int mx2 = player.spellstrength / 100 * 54;
		if(mx2 > 54) mx2 = 54;

		ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32);

		rcSrc2.x = sx;
		rcSrc2.y = sy + 16;
		rcSrc2.w = 56;
		rcSrc2.h = 6;

		SDL_FillRect(videobuffer, &rcSrc2, ccc);

		ccc = SDL_MapRGB(videobuffer->format, 0, 64, 224);
		if(player.attackstrength == 100) ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32);

		rcSrc2.x = sx + 1;
		rcSrc2.y = sy + 17;
		rcSrc2.w = mx;
		rcSrc2.h = 2;

		SDL_FillRect(videobuffer, &rcSrc2, ccc);

		ccc = SDL_MapRGB(videobuffer->format, 128, 0, 224);
		if(player.spellstrength == 100) ccc = SDL_MapRGB(videobuffer->format, 224, 0, 0);

		rcSrc2.x = sx + 1;
		rcSrc2.y = sy + 19;
		rcSrc2.w = mx2;
		rcSrc2.h = 2;

		SDL_FillRect(videobuffer, &rcSrc2, ccc);

		// time
		int ase = secstart + secsingame;
		int h = ((ase - (ase % 3600)) / 3600);
		ase = (ase - h * 3600);
		int m = ((ase - (ase % 60)) / 60);
		int s = (ase - m * 60);

		sprintf(line, "%02i:%02i:%02i", h, m, s);
		sys_print(videobuffer, line, 46 + 38 - strlen(line) * 4, 46 + 49, 0);

		sys_print(videobuffer, "Use", 193, 55, 0);
		sys_print(videobuffer, "Cast", 236, 55, 0);

		rcSrc.x = 128;
		rcSrc.y = 91;

		int ss = (player.sword - 1) * 3;
		if(player.sword == 3) ss = 18;
		SDL_BlitSurface(itemimg[ss], NULL, videobuffer, &rcSrc);

		rcSrc.x = rcSrc.x + 16;
		ss = (player.shield - 1) * 3 + 1;
		if(player.shield == 3) ss = 19;
		SDL_BlitSurface(itemimg[ss], NULL, videobuffer, &rcSrc);

		rcSrc.x = rcSrc.x + 16;
		ss = (player.armour - 1) * 3 + 2;
		if(player.armour == 3) ss = 20;
		SDL_BlitSurface(itemimg[ss], NULL, videobuffer, &rcSrc);

		for(int i = 0; i <= 4; i++) {
			sx = 188;
			sy = 70 + i * 24;
			rcSrc.x = sx;
			rcSrc.y = sy;
			if(i == 0) SDL_BlitSurface(itemimg[6], NULL, videobuffer, &rcSrc);
			if(i == 1) SDL_BlitSurface(itemimg[12], NULL, videobuffer, &rcSrc);
			if(i == 2) SDL_BlitSurface(itemimg[17], NULL, videobuffer, &rcSrc);
			if(i == 3) SDL_BlitSurface(itemimg[16], NULL, videobuffer, &rcSrc);
			if(i == 4) SDL_BlitSurface(itemimg[14], NULL, videobuffer, &rcSrc);

			sprintf(line, "x%i", inventory[i]);
			sys_print(videobuffer, line, sx + 17, sy + 7, 0);
		}

		if(player.foundcrystal == 1) {
			rcSrc.x = 243;
			rcSrc.y = 67;
			sy = 67;

			SDL_BlitSurface(itemimg[7], NULL, videobuffer, &rcSrc);

			ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32);

			rcSrc2.x = rcSrc.x;
			rcSrc2.y = sy + 16;
			rcSrc2.w = 16;
			rcSrc2.h = 4;

			SDL_FillRect(videobuffer, &rcSrc2, ccc);

			ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64);
			if(player.crystalcharge == 100) ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32);

			mx = player.crystalcharge / 100 * 14;
			if(mx > 14) mx = 14;

			rcSrc2.x = rcSrc.x + 1;
			rcSrc2.y = sy + 17;
			rcSrc2.w = mx;
			rcSrc2.h = 2;

			SDL_FillRect(videobuffer, &rcSrc2, ccc);


			for(int i = 0; i <= 3; i++) {
				rcSrc.x = 243;
				rcSrc.y = 91 + i * 24;
				sy = rcSrc.y;

				if(player.foundspell[i] == 1) {
					SDL_BlitSurface(itemimg[8 + i], NULL, videobuffer, &rcSrc);

					ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32);

					rcSrc2.x = rcSrc.x;
					rcSrc2.y = sy + 16;
					rcSrc2.w = 16;
					rcSrc2.h = 4;

					SDL_FillRect(videobuffer, &rcSrc2, ccc);

					ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64);
					if(player.spellcharge[i] == 100) ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32);

					mx = player.spellcharge[i] / 100 * 14;
					if(mx > 14) mx = 14;

					rcSrc2.x = rcSrc.x + 1;
					rcSrc2.y = sy + 17;
					rcSrc2.w = mx;
					rcSrc2.h = 2;

					SDL_FillRect(videobuffer, &rcSrc2, ccc);
				}
			}
		}

		if(itemselon == 1) {
			for(int i = 0; i <= 4; i++) {
				if(curitem == 5 + i) {
					rcDest.x = (float)(243 - 12 + 3 * sin(3.141592 * 2 * itemyloc / 16));
					rcDest.y = 67 + 24 * i;
					SDL_BlitSurface(itemimg[15], NULL, videobuffer, &rcDest);
				}

				if(curitem == i) {
					rcDest.x = (float)(189 - 12 + 3 * sin(3.141592 * 2 * itemyloc / 16));
					rcDest.y = 70 + 24 * i;
					SDL_BlitSurface(itemimg[15], NULL, videobuffer, &rcDest);
				}
			}
		}
	}

	if(selenemyon == 1) {
		if(curenemy > lastnpc) {
			int pst = curenemy - lastnpc - 1;
			rcDest.x = postinfo[pst][0];
			rcDest.y = (float)(postinfo[pst][1] - 4 - sin(3.141592 / 8 * itemyloc));
		} else {
			rcDest.x = npcinfo[curenemy].x + 4;
			rcDest.y = (float)(npcinfo[curenemy].y + 4 - 16 - sin(3.141592 / 8 * itemyloc));
		}

		SDL_BlitSurface(itemimg[13], NULL, videobuffer, &rcDest);
	}
}

void game_drawnpcs(int mode)
{
	unsigned int ccc;

	ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32);

	int lok = 0;
	if(mode == 1) lok = 1;

	int fst = firsty;
	int lst = lasty;

	if(mode == 0) lst = player.ysort;
	if(mode == 1) fst = player.ysort;

	for(int yy = fst; yy <= lst; yy++) {

		if(ysort[yy] > 0) {
			int i = ysort[yy];

			if(npcinfo[i].hp > 0) {
				int npx = (int)(npcinfo[i].x);
				int npy = (int)(npcinfo[i].y);

				// if((npy <= player.py AND mode = 0) OR (npy > player.py AND mode = 1)) {

				int sprite = npcinfo[i].spriteset;

				int wdir = npcinfo[i].walkdir;

				// spriteset1 specific
				if(npcinfo[i].spriteset == 1) {

					if(npcinfo[i].attacking == 0) {

						int cframe = npcinfo[i].cframe;

						rcSrc.x = (int)(cframe / 4) * 24;
						rcSrc.y = wdir * 24;
						rcSrc.w = 24;
						rcSrc.h = 24;

						rcDest.x = npx;
						rcDest.y = npy;
						rcDest.w = 24;
						rcDest.h = 24;

						if(npcinfo[i].pause > ticks && npcinfo[i].shake < ticks) {
							npcinfo[i].shake = ticks + 50;
							rcDest.x = rcDest.x + (int)(RND() * 3) - 1;
							rcDest.y = rcDest.y + (int)(RND() * 3) - 1;
						}

						SDL_BlitSurface(anims[sprite], &rcSrc, videobuffer, &rcDest);
					} else {
						int cframe = npcinfo[i].cattackframe;

						rcSrc.x = (int)(cframe / 4) * 24;
						rcSrc.y = wdir * 24;
						rcSrc.w = 24;
						rcSrc.h = 24;

						rcDest.x = npx;
						rcDest.y = npy;
						rcDest.w = 24;
						rcDest.h = 24;

						SDL_BlitSurface(animsa[sprite], &rcSrc, videobuffer, &rcDest);
					}

				}

				// onewing
				if(npcinfo[i].spriteset == 2) {
					for(int f = 0; f <= 7; f++) {
						int s = npcinfo[i].bodysection[f].sprite;
						rcSrc.x = animset2[s].x;
						rcSrc.y = animset2[s].y;
						rcSrc.w = animset2[s].w;
						rcSrc.h = animset2[s].h;

						rcDest.x = npcinfo[i].bodysection[f].x - animset2[s].xofs;
						rcDest.y = npcinfo[i].bodysection[f].y - animset2[s].yofs + 2;

						SDL_BlitSurface(anims[2], &rcSrc, videobuffer, &rcDest);
					}

				}

				// twowing
				if(npcinfo[i].spriteset == 9) {
					for(int f = 0; f <= 7; f++) {
						int yp = 0;

						if(f == 0 && (curmap == 53 || curmap == 57 || curmap == 61 || curmap == 65 || curmap == 56 || curmap > 66) && scriptflag[60][0] > 0) yp = 16;
						int s = npcinfo[i].bodysection[f].sprite;
						rcSrc.x = animset9[s].x;
						rcSrc.y = animset9[s].y + yp;
						rcSrc.w = animset9[s].w;
						rcSrc.h = animset9[s].h;

						rcDest.x = npcinfo[i].bodysection[f].x - animset9[s].xofs;
						rcDest.y = npcinfo[i].bodysection[f].y - animset9[s].yofs + 2;

						SDL_BlitSurface(anims[9], &rcSrc, videobuffer, &rcDest);
					}

				}


				//  boss 1
				if(npcinfo[i].spriteset == 3) {
					if(npcinfo[i].attacking == 0) {
						int cframe = npcinfo[i].cframe;

						rcSrc.x = (int)(cframe / 4) * 24;
						rcSrc.y = 0;
						rcSrc.w = 24;
						rcSrc.h = 48;

						rcDest.x = npx - 2;
						rcDest.y = npy - 24;

						SDL_BlitSurface(anims[3], &rcSrc, videobuffer, &rcDest);
					} else {
						rcSrc.x = 4 * 24;
						rcSrc.y = 0;
						rcSrc.w = 24;
						rcSrc.h = 48;

						rcDest.x = npx - 2;
						rcDest.y = npy - 24;

						SDL_BlitSurface(anims[3], &rcSrc, videobuffer, &rcDest);
					}

				}

				// black knight
				if(npcinfo[i].spriteset == 4) {
					if(npcinfo[i].attacking == 0) {
						int cframe = npcinfo[i].cframe;

						rcSrc.x = (int)(cframe / 4) * 24;
						rcSrc.y = 0;
						rcSrc.w = 24;
						rcSrc.h = 48;

						rcDest.x = npx - 2;
						rcDest.y = npy - 24;

						SDL_BlitSurface(anims[4], &rcSrc, videobuffer, &rcDest);
					} else {
						rcSrc.x = 4 * 24;
						rcSrc.y = 0;
						rcSrc.w = 24;
						rcSrc.h = 48;

						rcDest.x = npx - 2;
						rcDest.y = npy - 24;

						SDL_BlitSurface(anims[4], &rcSrc, videobuffer, &rcDest);
					}
				}


				// firehydra
				if(npcinfo[i].spriteset == 5) {
					for(int ff = 0; ff <= 2; ff++) {
						if(npcinfo[i].hp > 10 * ff * 20) {
							rcSrc.x = 16 * (int)(RND() * 2);
							rcSrc.y = 80;
							rcSrc.w = 16;
							rcSrc.h = 16;

							rcDest.x = npcinfo[i].bodysection[10 * ff].x - 8;
							rcDest.y = npcinfo[i].bodysection[10 * ff].y - 8;

							int x = 192 + ((int)(itemyloc + ff * 5) % 3) * 64;
							if(x > 255) x = 255;
							SDL_SetAlpha(spellimg, SDL_SRCALPHA, x);
							SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
							SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);

							for(int f = 1; f <= 8; f++) {
								rcSrc.x = 16 * (int)(RND() * 2);
								rcSrc.y = 80;
								rcSrc.w = 16;
								rcSrc.h = 16;

								rcDest.x = npcinfo[i].bodysection[ff * 10 + f].x - 8 + (int)(RND() * 3) - 1;
								rcDest.y = npcinfo[i].bodysection[ff * 10 + f].y - 8 + (int)(RND() * 3) - 1;

								x = 192 + f % 3 * 64;
								if(x > 255) x = 255;
								SDL_SetAlpha(spellimg, SDL_SRCALPHA, x);
								SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
								SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);
							}

							rcSrc.x = 0;
							rcSrc.y = 0;
							rcSrc.w = 42;
							rcSrc.h = 36;

							rcDest.x = npcinfo[i].bodysection[10 * ff + 9].x - 21;
							rcDest.y = npcinfo[i].bodysection[10 * ff + 9].y - 21;

							SDL_SetAlpha(spellimg, SDL_SRCALPHA, 192);
							SDL_BlitSurface(anims[5], &rcSrc, videobuffer, &rcDest);
							SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);

						}

					}

				}

				// red dragon
				if(npcinfo[i].spriteset == 6) {
					int cframe = npcinfo[i].cframe;

					rcSrc.x = (int)(cframe / 4) * 24;
					rcSrc.y = wdir * 24;
					rcSrc.w = 24;
					rcSrc.h = 24;

					rcDest.x = npx;
					rcDest.y = npy;
					rcDest.w = 24;
					rcDest.h = 24;

					if(npcinfo[i].pause > ticks && npcinfo[i].shake < ticks) {
						npcinfo[i].shake = ticks + 50;
						rcDest.x = rcDest.x + (int)(RND() * 3) - 1;
						rcDest.y = rcDest.y + (int)(RND() * 3) - 1;
					}

					SDL_BlitSurface(anims[sprite], &rcSrc, videobuffer, &rcDest);
				}

				// wizard
				if(npcinfo[i].spriteset == 7) {
					// if(npcinfo[i].attacking == 0) {
						int cframe = npcinfo[i].cframe;

						rcSrc.x = (int)(cframe / 4) * 24;
						rcSrc.y = wdir * 24;
						rcSrc.w = 24;
						rcSrc.h = 24;

						rcDest.x = npx;
						rcDest.y = npy;
						rcDest.w = 24;
						rcDest.h = 24;

						if(npcinfo[i].pause > ticks && npcinfo[i].shake < ticks) {
							npcinfo[i].shake = ticks + 50;
							rcDest.x = rcDest.x + (int)(RND() * 3) - 1;
							rcDest.y = rcDest.y + (int)(RND() * 3) - 1;
						}
						SDL_BlitSurface(anims[sprite], &rcSrc, videobuffer, &rcDest);
					// } else {
						//cframe = npcinfo[i].cattackframe;

						//rcSrc.x = (int)(cframe / 4) * 24;
						//rcSrc.y = wdir * 24;
						//rcSrc.w = 24;
						//rcSrc.h = 24;

						//rcDest.x = npx;
						//rcDest.y = npy;
						//rcDest.w = 24;
						//rcDest.h = 24;
					// SDL_BlitSurface(animsa(sprite), &rcSrc, videobuffer, &rcDest);
					// }
				}


				// yellow dragon
				if(npcinfo[i].spriteset == 8) {
					int cframe = npcinfo[i].cframe;

					rcSrc.x = (int)(cframe / 4) * 24;
					rcSrc.y = wdir * 24;
					rcSrc.w = 24;
					rcSrc.h = 24;

					rcDest.x = npx;
					rcDest.y = npy;
					rcDest.w = 24;
					rcDest.h = 24;

					if(npcinfo[i].pause > ticks && npcinfo[i].shake < ticks) {
						npcinfo[i].shake = ticks + 50;
						rcDest.x = rcDest.x + (int)(RND() * 3) - 1;
						rcDest.y = rcDest.y + (int)(RND() * 3) - 1;
					}
					SDL_BlitSurface(anims[sprite], &rcSrc, videobuffer, &rcDest);
				}


				// dragon2
				if(npcinfo[i].spriteset == 10) {
					if(npcinfo[i].attacking == 0) {
						npcinfo[i].floating = npcinfo[i].floating + 0.25 * fpsr;
						while(npcinfo[i].floating >= 16) npcinfo[i].floating = npcinfo[i].floating - 16;

						float frame = npcinfo[i].frame;
						int cframe = npcinfo[i].cframe;

						frame = frame + 0.5 * fpsr;
						while(frame >= 16)
							frame = frame - 16;

						cframe = (int)(frame);
						if(cframe > 16) cframe = 16 - 1;
						if(cframe < 0) cframe = 0;

						npcinfo[i].frame = frame;
						npcinfo[i].cframe = cframe;

						cframe = npcinfo[i].cframe;

						rcSrc.x = 74 * wdir;
						rcSrc.y = (int)(cframe / 4) * 48;
						rcSrc.w = 74;
						rcSrc.h = 48;

						rcDest.x = npx + 12 - 37;
						rcDest.y = (float)(npy + 12 - 32 - 3 * sin(3.141592 * 2 * npcinfo[i].floating / 16));
						rcDest.w = 24;
						rcDest.h = 24;

						if(npcinfo[i].pause > ticks && npcinfo[i].shake < ticks) {
							npcinfo[i].shake = ticks + 50;
							rcDest.x = rcDest.x + (int)(RND() * 3) - 1;
							rcDest.y = rcDest.y + (int)(RND() * 3) - 1;
						}

						SDL_BlitSurface(anims[sprite], &rcSrc, videobuffer, &rcDest);
					} else {
						npcinfo[i].floating = npcinfo[i].floating + 0.25 * fpsr;
						while(npcinfo[i].floating >= 16) npcinfo[i].floating = npcinfo[i].floating - 16;

						int cframe = npcinfo[i].cattackframe;

						rcSrc.x = 74 * wdir;
						rcSrc.y = (int)(cframe / 4) * 48;
						rcSrc.w = 74;
						rcSrc.h = 48;

						rcDest.x = npx + 12 - 37;
						rcDest.y = (float)(npy + 12 - 32 - 3 * sin(3.141592 * 2 * npcinfo[i].floating / 16));
						rcDest.w = 24;
						rcDest.h = 24;

						SDL_BlitSurface(animsa[sprite], &rcSrc, videobuffer, &rcDest);
					}
				}

				// end boss
				if(npcinfo[i].spriteset == 11) {

					npcinfo[i].floating = npcinfo[i].floating + .3 * fpsr;
					while(npcinfo[i].floating >= 16) npcinfo[i].floating = npcinfo[i].floating - 16;


					float frame = npcinfo[i].frame2;
					int cframe = npcinfo[i].cframe;

					frame = frame + 0.5 * fpsr;
					while(frame >= 16)
						frame = frame - 16;

					npcinfo[i].frame2 = frame;

					cframe = (int)(frame);

					int sx = npx + 12 - 40;
					int sy = (float)(npy + 12 - 50 - 3 * sin(3.141592 * 2 * npcinfo[i].floating / 16));

					for(int fr = 0; fr <= 3; fr++) {
						SDL_SetAlpha(spellimg, SDL_SRCALPHA, 128 + (int)(RND() * 96));

						rcSrc.x = 16 * (int)(RND() * 2);
						rcSrc.y = 80;
						rcSrc.w = 16;
						rcSrc.h = 16;

						rcDest.x = sx + 32 + (int)(RND() * 3) - 1;
						rcDest.y = sy - (int)(RND() * 6);

						SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
					}

					for(int ii = 0; ii <= 8; ii++) {
						for(int i2 = 0; i2 <= 3; i2++) {
							rcSrc.x = 16 * (int)(RND() * 2);
							rcSrc.y = 80;
							rcSrc.w = 16;
							rcSrc.h = 16;

							float fr3 = frame - 3 + i2;
							if(fr3 < 0) fr3 = fr3 + 16;

							rcDest.x = (float)(sx + 36 + ii * 8 - ii * cos(3.14159 * 2 * (fr3 - ii) / 16) * 2);
							rcDest.y = (float)(sy + 16 + ii * sin(3.14159 * 2 * (fr3 - ii) / 16) * 3 - ii); //  * 4

							SDL_SetAlpha(spellimg, SDL_SRCALPHA, i2 / 3 * 224);

							SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);

							int xloc = rcDest.x;
							int yloc = rcDest.y;
							int xdif = (xloc + 8) - (player.px + 12);
							int ydif = (yloc + 8) - (player.py + 12);

							if((abs(xdif) < 8 && abs(ydif) < 8) && player.pause < ticks) {
								float damage = (float)npcinfo[i].spelldamage * (1.0 + RND() * 0.5);

								if(player.hp > 0) {
									game_damageplayer(damage);
									if(menabled == 1 && opeffects == 1) {
										int snd = Mix_PlayChannel(-1, sfx[sndfire], 0);
										Mix_Volume(snd, opeffectsvol);
									}
								}

							}


							rcDest.x = (float)(sx + 36 - ii * 8 + ii * cos(3.14159 * 2 * (fr3 - ii) / 16) * 2);
							rcDest.y = (float)(sy + 16 + ii * sin(3.14159 * 2 * (fr3 - ii) / 16) * 3 - ii); //  * 4

							SDL_SetAlpha(spellimg, SDL_SRCALPHA, i2 / 3 * 224);

							SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);

							xloc = rcDest.x;
							yloc = rcDest.y;
							xdif = (xloc + 8) - (player.px + 12);
							ydif = (yloc + 8) - (player.py + 12);

							if((abs(xdif) < 8 && abs(ydif) < 8) && player.pause < ticks) {
								float damage = (float)npcinfo[i].spelldamage * (1.0 + RND() * 0.5);

								if(player.hp > 0) {
									game_damageplayer(damage);
									if(menabled == 1 && opeffects == 1) {
										int snd = Mix_PlayChannel(-1, sfx[sndfire], 0);
										Mix_Volume(snd, opeffectsvol);
									}
								}
							}
						}
					}

					SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);

					if(npcinfo[i].attacking == 0) {
						int cframe = (int)(frame);
						rcSrc.x = 0;
						rcSrc.y = 72 * (int)(cframe / 4);
						rcSrc.w = 80;
						rcSrc.h = 72;

						rcDest.x = sx;
						rcDest.y = sy;

						if(npcinfo[i].pause > ticks && npcinfo[i].shake < ticks) {
							npcinfo[i].shake = ticks + 50;
							rcDest.x = rcDest.x + (int)(RND() * 3) - 1;
							rcDest.y = rcDest.y + (int)(RND() * 3) - 1;
						}

						SDL_BlitSurface(anims[sprite], &rcSrc, videobuffer, &rcDest);
					} else {
						int cframe = (int)(npcinfo[i].cattackframe);

						rcSrc.x = 0;
						rcSrc.y = 72 * (int)(cframe / 4);
						rcSrc.w = 80;
						rcSrc.h = 72;

						rcDest.x = sx;
						rcDest.y = sy;

						SDL_BlitSurface(animsa[sprite], &rcSrc, videobuffer, &rcDest);
					}
				}

				// bat kitty
				if(npcinfo[i].spriteset == 12) {
					npcinfo[i].floating = npcinfo[i].floating + 1 * fpsr;
					while(npcinfo[i].floating >= 16) npcinfo[i].floating = npcinfo[i].floating - 16;

					float frame = npcinfo[i].frame;
					int cframe = npcinfo[i].cframe;

					frame = frame + 0.5 * fpsr;
					while(frame >= 16)
						frame = frame - 16;

					cframe = (int)(frame);
					if(cframe > 16) cframe = 16 - 1;
					if(cframe < 0) cframe = 0;

					npcinfo[i].frame = frame;
					npcinfo[i].cframe = cframe;

					cframe = npcinfo[i].cframe;

					rcSrc.x = 0;
					rcSrc.y = 0;
					rcSrc.w = 99;
					rcSrc.h = 80;

					rcDest.x = npx + 12 - 50;
					rcDest.y = (float)(npy + 12 - 64 + 2 * sin(3.141592 * 2 * npcinfo[i].floating / 16));
					rcDest.w = 99;
					rcDest.h = 80;

					if(npcinfo[i].pause > ticks && npcinfo[i].shake < ticks) {
						npcinfo[i].shake = ticks + 50;
						rcDest.x = rcDest.x + (int)(RND() * 3) - 1;
						rcDest.y = rcDest.y + (int)(RND() * 3) - 1;
					}

					SDL_BlitSurface(anims[sprite], &rcSrc, videobuffer, &rcDest);
				}

				rcDest.x = npx + 4;
				rcDest.y = npy + 22;
				rcDest.w = 16;
				rcDest.h = 4;

				SDL_FillRect(videobuffer, &rcDest, 0);

				rcDest.x = npx + 5;
				rcDest.y = npy + 23;


				int ww = 14 * npcinfo[i].hp / npcinfo[i].maxhp;
				if(ww > 14) ww = 14;
				if(ww < 1) ww = 1;

				rcDest.w = ww;
				rcDest.h = 2;


				SDL_FillRect(videobuffer, &rcDest, ccc);

				int pass = 1;

				if(npcinfo[i].spriteset == 3) pass = 0;
				if(pass == 1) game_drawover(npx, npy);

			}
		}
	}
}

void game_drawover(int modx, int mody)
{
	int npx = modx + 12;
	int npy = mody + 20;

	int lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
	int ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

	for(int xo = -1; xo <= 1; xo++) {
		for(int yo = -1; yo <= 1; yo++) {

			int sx = lx + xo;
			int sy = ly + yo;

			int sx2 = sx * 16;
			int sy2 = sy * 16;

			if(sx > -1 && sx < 40 && sy > -1 && sy < 24) {

				int curtile = tileinfo[2][sx][sy][0];
				int curtilel = tileinfo[2][sx][sy][1];

				if(curtile > 0) {
					curtile = curtile - 1;
					int curtilex = curtile % 20;
					int curtiley = (curtile - curtilex) / 20;

					rcSrc.x = curtilex * 16;
					rcSrc.y = curtiley * 16;
					rcSrc.w = 16;
					rcSrc.h = 16;

					rcDest.x = sx2;
					rcDest.y = sy2;
					rcDest.w = 16;
					rcDest.h = 16;

					int pass = 1;
					if(curtilel == 1) {
						for(int ff = 0; ff <= 5; ff++) {
							int ffa = 20 * 5 - 1 + ff * 20;
							int ffb = 20 * 5 + 4 + ff * 20;
							if(curtile > ffa && curtile < ffb) pass = 0;
						}
					}

					if(pass == 1) SDL_BlitSurface(tiles[curtilel], &rcSrc, videobuffer, &rcDest);
				}
			}
		}
	}
}

void game_drawplayer()
{
	long ccc;

	int f = 0;
	if(player.armour == 3) f = 13;

	if(attacking == 0) {
		rcSrc.x = (int)(player.walkframe / 4) * 24;
		rcSrc.y = player.walkdir * 24;
		rcSrc.w = 24;
		rcSrc.h = 24;

		rcDest.x = (int)(player.px);
		rcDest.y = (int)(player.py);
		rcDest.w = 24;
		rcDest.h = 24;

		SDL_BlitSurface(anims[f], &rcSrc, videobuffer, &rcDest);
	} else {
		rcSrc.x = (int)(player.attackframe / 4) * 24;
		rcSrc.y = player.walkdir * 24;
		rcSrc.w = 24;
		rcSrc.h = 24;

		rcDest.x = (int)(player.px);
		rcDest.y = (int)(player.py);
		rcDest.w = 24;
		rcDest.h = 24;

		SDL_BlitSurface(animsa[f], &rcSrc, videobuffer, &rcDest);

	}

	ccc = SDL_MapRGB(videobuffer->format, 224, 224, 64);

	int pass = 0;
	if(player.hp <= player.maxhp * 0.25) pass = 1;

	if(pass == 1) {
		ccc = SDL_MapRGB(videobuffer->format, 255, 255, 255);
		if((int)(player.hpflash) == 1) ccc = SDL_MapRGB(videobuffer->format, 255, 0, 0);
	}

	int sss = 6;
	if(player.foundcrystal) sss = 8;
	int npx = player.px;
	int npy = player.py;
	rcDest.x = npx + 4;
	rcDest.y = npy + 22;
	rcDest.w = 16;
	rcDest.h = sss;

	SDL_FillRect(videobuffer, &rcDest, 0);

	rcDest.x = npx + 5;
	rcDest.y = npy + 23;


	int ww = 14 * player.hp / player.maxhp;
	if(ww > 14) ww = 14;
	if(ww < 1) ww = 1;

	rcDest.w = ww;
	rcDest.h = 2;

	SDL_FillRect(videobuffer, &rcDest, ccc);

	ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64);
	if(player.attackstrength == 100) ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32);

	ww = 14 * player.attackstrength / 100;
	if(ww > 14) ww = 14;

	int ww2 = 14 * player.spellstrength / 100;
	if(ww2 > 14) ww2 = 14;

	rcDest.w = ww;
	rcDest.h = 2;
	rcDest.y = rcDest.y + 2;

	SDL_FillRect(videobuffer, &rcDest, ccc);

	ccc = SDL_MapRGB(videobuffer->format, 128, 0, 224);
	if(player.spellstrength == 100) ccc = SDL_MapRGB(videobuffer->format, 224, 0, 0);

	rcDest.w = ww2;
	rcDest.h = 2;
	rcDest.y = rcDest.y + 2;

	SDL_FillRect(videobuffer, &rcDest, ccc);
}

void game_drawview()
{
	SDL_BlitSurface(mapbg, NULL, videobuffer, NULL);

	game_updspellsunder();

	game_drawanims(0);

	// ------dontdrawover = special case to make boss work right in room 24
	if(dontdrawover == 1) game_drawanims(1);
	game_drawnpcs(0);

	game_drawplayer();

	game_drawnpcs(1);
	if(dontdrawover == 0) game_drawanims(1);

	game_drawover((int)player.px, (int)player.py);

	game_updspells();

	if(cloudson == 1) {
		rcDest.x = (float)(256 + 256 * cos(3.141592 / 180 * clouddeg));
		rcDest.y = (float)(192 + 192 * sin(3.141592 / 180 * clouddeg));
		rcDest.w = 320;
		rcDest.h = 240;

		SDL_BlitSurface(cloudimg, &rcDest, videobuffer, NULL);
	}

	game_drawhud();

	rcDest.x = SCR_TOPX;
	rcDest.y = SCR_TOPY;

	SDL_BlitSurface(videobuffer, NULL, video, &rcDest);
}

void game_endofgame()
{
	float xofs = 0;
	int ticks1, bticks, pauseticks, keypause;

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = 320;
	rcSrc.h = 240;

	rc.x = 0;
	rc.y = 0;
	rc.w = 320;
	rc.h = 240;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;
	rc2.w = 320;
	rc2.h = 240;

	//x = 160 - 4 * len(stri$)

	ticks = SDL_GetTicks();
	pauseticks = ticks + 500;
	bticks = ticks;

	keypause = ticks + 220;

	float spd = 0.2;

	if(menabled == 1 && opmusic == 1) {
		Mix_HaltChannel(-1);
		musicchannel = Mix_PlayChannel(-1, mendofgame, -1);
		Mix_Volume(musicchannel, 0);
	}

	ticks1 = ticks;
	int ya = 0;

	SDL_FillRect(videobuffer2, &rc, 0);
	SDL_FillRect(videobuffer3, &rc, 0);
	SDL_BlitSurface(videobuffer, &rc, videobuffer2, &rc);

	float ld = 0;
	int ldstop = 0;

	do {
		ld = ld + 4 * fpsr;
		if(ld > opmusicvol) ld = opmusicvol;
		if(menabled == 1 && ldstop == 0) {
			Mix_Volume(musicchannel, (int)ld);
			if((int)ld == opmusicvol) ldstop = 1;
		}

		ya = 0;
		if(ticks < ticks1 + 1500) {
			ya = (255 * (ticks - ticks1)) / 1500;
			if(ya < 0) ya = 0;
			if(ya > 255) ya = 255;
		} else {
			break;
		}

		SDL_FillRect(videobuffer, &rc, 0);

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, ya);
		SDL_BlitSurface(videobuffer2, &rc, videobuffer3, &rc);
		SDL_BlitSurface(videobuffer, &rc, videobuffer3, &rc);
		SDL_BlitSurface(videobuffer3, &rc, video, &rc2);
		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}
	} while(1);

	ticks1 = ticks;
	ya = 0;
	float y = 140;

	do {
 		rc.x = -xofs;
		rc.y = 0;

		SDL_BlitSurface(titleimg, &rcSrc, videobuffer, &rc);

 		rc.x = -xofs + 320;
		rc.y = 0;

		SDL_BlitSurface(titleimg, &rcSrc, videobuffer, &rc);

 		rc.x = 0;
		rc.y = 0;

		y = y - spd * fpsr;
		for(int i = 0; i <= 26; i++) {
			int yy = y + i * 10;
			if(yy > -8 && yy < 240) {
				int x = 160 - strlen(story2[i]) * 4;
				sys_print(videobuffer, story2[i], x, yy, 4);
			}

			if(yy < 10 && i == 25) break;
		}

		rc.x = 0;
		rc.y = 0;
		rc.w = 320;
		rc.h = 240;

		int ya = 255;
		if(ticks < ticks1 + 1000) {
			ya = 255 * (ticks - ticks1) / 1000;
			if(ya < 0) ya = 0;
			if(ya > 255) ya = 255;
		}

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, ya);
		SDL_BlitSurface(videobuffer, &rc, video, &rc2);
		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		float add = 0.5 * fpsr;
		if(add > 1) add = 1;
		xofs = xofs + add;
		if(xofs >= 320) xofs = xofs - 320;

		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);

		if(event.type == SDL_KEYDOWN) spd = 1;
		if(event.type == SDL_KEYUP) spd = 0.2;

		if(keys[SDLK_ESCAPE]) break;
	} while(1);


	rc.x = 0;
	rc.y = 0;
	rc.w = 320;
	rc.h = 240;

	ticks1 = ticks;
	y = 0;

	SDL_BlitSurface(videobuffer, &rc, videobuffer2, &rc);

	do {
		if(ticks < ticks1 + 1500) {
			int y = 255 * (ticks - ticks1) / 1500;
			if(y < 0) y = 0;
			if(y > 255) y = 255;
		else
			break;
		}

		SDL_FillRect(videobuffer, &rc, 0);

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, y);
		SDL_BlitSurface(videobuffer2, &rc, videobuffer3, &rc);
		SDL_BlitSurface(videobuffer, &rc, videobuffer3, &rc);
		SDL_BlitSurface(videobuffer3, &rc, video, &rc2);
		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}
	} while(1);


	int keywait = 2000 + ticks;

	ticks1 = ticks;
	y = 0;
	do {

		SDL_BlitSurface(theendimg, &rcSrc, videobuffer, &rc);

		y = 255;
		if(ticks < ticks1 + 1000) {
			y = 255 * (ticks - ticks1) / 1000;
			if(y < 0) y = 0;
			if(y > 255) y = 255;
		}

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, y);
		SDL_BlitSurface(videobuffer, &rc, video, &rc2);
		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);

			SDL_Flip(video);
			SDL_PumpEvents();

			tickspassed = ticks;
			ticks = SDL_GetTicks();

			tickspassed = ticks - tickspassed;
			fpsr = (float)tickspassed / 24;

			fp++;
			if(ticks > nextticks) {
				nextticks = ticks + 1000;
				fps = fp;
				fp = 0;
			}

			SDL_PollEvent(&event);
			keys = SDL_GetKeyState(NULL);

			if(event.type == SDL_KEYDOWN && keywait < ticks) break;

	} while(1);

	SDL_FillRect(videobuffer2, &rc, 0);
	SDL_FillRect(videobuffer3, &rc, 0);

	game_theend();

}

void game_eventtext(char *stri)
{
	int x, fr, pauseticks, bticks;

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = 320;
	rcSrc.h = 240;

	SDL_FillRect(videobuffer2, &rcSrc, 0);
	SDL_FillRect(videobuffer3, &rcSrc, 0);

	rc.x = 0;
	rc.y = 0;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;
	rc2.w = 320;
	rc2.h = 240;

	x = 160 - 4 * strlen(stri);

	ticks = SDL_GetTicks();
	pauseticks = ticks + 500;
	bticks = ticks;

	SDL_BlitSurface(videobuffer, NULL, videobuffer3, NULL);
	SDL_BlitSurface(video, &rc2, videobuffer2, NULL);

	do {
		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);

		if(event.type == SDL_KEYDOWN && pauseticks < ticks) break;
		SDL_BlitSurface(videobuffer2, NULL, videobuffer, NULL);

		fr = 192;

		if(pauseticks > ticks) fr = 192 * (ticks - bticks) / 500;
		if(fr > 192) fr = 192;

		SDL_SetAlpha(windowimg, SDL_SRCALPHA, fr);

		SDL_BlitSurface(windowimg, NULL, videobuffer, &rc);
		if(pauseticks < ticks) sys_print(videobuffer, stri, x, 15, 0);

		SDL_SetAlpha(windowimg, SDL_SRCALPHA, 255);
		SDL_BlitSurface(videobuffer, NULL, video, &rc2);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24.0;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		SDL_Delay(10);
	} while(1);

	SDL_BlitSurface(videobuffer3, NULL, videobuffer, NULL);

	itemticks = ticks + 210;
}


void game_handlewalking()
{
	unsigned int *temp/*, c*/, bgc;
	float spd, /*ppx, ppy,*/ px, py, opx, opy;
	float nx, ny, npx, npy;

	int xmax = 20 * 16 - 25;
	int ymax = 15 * 16 - 25;

	px = player.px;
	py = player.py;
	opx = px;
	opy = py;

	spd = player.walkspd * fpsr;

	nx = (px / 2 + 6);
	ny = (py / 2 + 10);

	npx = px + 12;
	npy = py + 20;
	int lx = (int)npx / 16; //(npx - ((int)npx % 16)) / 16;
	int ly = (int)npy / 16; //(npy - ((int)npy % 16)) / 16;

	int ramp = rampdata[lx][ly];
	if(ramp == 1 && movingup) spd = spd * 2;
	if(ramp == 1 && movingdown) spd = spd * 2;

	if(ramp == 2 && movingleft) movingup = 1;
	if(ramp == 2 && movingright) movingdown = 1;

	if(ramp == 3 && movingright) movingup = 1;
	if(ramp == 3 && movingleft) movingdown = 1;

	for(int x = -1; x <= 1; x++) {
		for(int y = -1; y <= 1; y++) {
			int sx = nx + x;
			int sy = ny + y;

			clipsurround[x + 1][y + 1] = 0;
			if(sx > -1 && sx < 320 && sy > -1 && sy < 192) {
				temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
				clipsurround[x + 1][y + 1] = *temp;
			}
		}
	}

	if(movingup) player.walkdir = 0;
	if(movingdown) player.walkdir = 1;
	if(movingleft) player.walkdir = 2;
	if(movingright) player.walkdir = 3;

	if(movingup && clipsurround[1][0] == 0) {
		py = py - spd;
		player.walkdir = 0;
	} else if(movingup && clipsurround[1][0] > 0) {
		// move upleft
		if(movingright == 0 && clipsurround[0][0] == 0) {
			py = py - spd;
			px = px - spd;
		}

		// move upright
		if(movingleft == 0 && clipsurround[2][0] == 0) {
			py = py - spd;
			px = px + spd;
		}
	}
	if(movingdown && clipsurround[1][2] == 0) {
		py = py + spd;
		player.walkdir = 1;
	} else if(movingdown && clipsurround[1][2] > 0) {
		// move downleft
		if(movingright == 0 && clipsurround[0][2] == 0) {
			py = py + spd;
			px = px - spd;
		}

		// move downright
		if(movingleft == 0 && clipsurround[2][2] == 0) {
			py = py + spd;
			px = px + spd;
		}
	}
	if(movingleft && clipsurround[0][1] == 0) {
		px = px - spd;
		player.walkdir = 2;
	} else if(movingleft && clipsurround[0][1] > 0) {
		// move leftup
		if(movingdown == 0 && clipsurround[0][0] == 0) {
			py = py - spd;
			px = px - spd;
		}

		// move leftdown
		if(movingup == 0 && clipsurround[0][2] == 0) {
			py = py + spd;
			px = px - spd;
		}
	}
	if(movingright && clipsurround[2][1] == 0) {
		px = px + spd;
		player.walkdir = 3;
	} else if(movingright && clipsurround[2][1] > 0) {
		// move rightup
		if(movingdown == 0 && clipsurround[2][0] == 0) {
			px = px + spd;
			py = py - spd;
		}

		// move rightdown
		if(movingup == 0 && clipsurround[2][2] == 0) {
			py = py + spd;
			px = px + spd;
		}
	}

	if(px < -8) px = -8;
	if(px > xmax) px = xmax;
	if(py < -8) py = -8;
	if(py > ymax) py = ymax;

	int pass = 1;

	int sx = (px / 2 + 6);
	int sy = (py / 2 + 10);
	temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
	bgc = *temp;
	if(bgc > 0 && bgc != 1000) {
		px = opx;
		py = opy;
		pass = 0;
	}

	// push npc
	if(pass == 1) {
		for(int i = 1; i <= lastnpc; i++) {
			if(npcinfo[i].hp > 0) {
				npx = npcinfo[i].x;
				npy = npcinfo[i].y;

				opx = npx;
				opy = npy;

				int xdif = player.px - npx;
				int ydif = player.py - npy;

				if(player.walkdir == 0) {
					if(abs(xdif) <= 8 && ydif > 0 && ydif < 8) npcinfo[i].y = npcinfo[i].y - spd;
				} else if(player.walkdir == 1) {
					if(abs(xdif) <= 8 && ydif < 0 && ydif > -8) npcinfo[i].y = npcinfo[i].y + spd;
				} else if(player.walkdir == 2) {
					if(abs(ydif) <= 8 && xdif > 0 && xdif < 8) npcinfo[i].x = npcinfo[i].x - spd;
				} else if(player.walkdir == 3) {
					if(abs(ydif) <= 8 && xdif < 0 && xdif > -8) npcinfo[i].x = npcinfo[i].x + spd;
				}

				npx = npcinfo[i].x;
				npy = npcinfo[i].y;

				sx = (int)(npx / 2 + 6);
				sy = (int)(npy / 2 + 10);
				temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
				bgc = *temp;

				if(bgc > 0) {
					npcinfo[i].x = opx;
					npcinfo[i].y = opy;
				}
			}
		}
	}

	player.opx = player.px;
	player.opy = player.py;
	player.px = px;
	player.py = py;

	if(player.px != player.opx || player.py != player.opy) player.walkframe = player.walkframe + animspd * fpsr;
	if(player.walkframe >= 16) player.walkframe = player.walkframe - 16;

	// walking over items to pickup :::
	int o = objmap[lx][ly];

	if(o > -1) {
		// fsk
		if(objectinfo[o][4] == 2 && inventory[0] < 9) {
			objmap[lx][ly] = -1;

			inventory[0] = inventory[0] + 1;
			if(inventory[0] > 9) inventory[0] = 9;
			game_addFloatIcon(6, lx * 16, ly * 16);

			objmapf[curmap][lx][ly] = 1;

			if(menabled == 1 && opeffects == 1) {
				int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
				Mix_Volume(snd, opeffectsvol);
			}
		}

		if(objectinfo[o][5] == 7 && inventory[1] < 9) {
			objmap[lx][ly] = -1;

			inventory[1] = inventory[1] + 1;
			if(inventory[1] > 9) inventory[1] = 9;
			game_addFloatIcon(12, lx * 16, ly * 16);

			objmapf[curmap][lx][ly] = 1;

			if(menabled == 1 && opeffects == 1) {
				int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
				Mix_Volume(snd, opeffectsvol);
			}
		}

		if(objectinfo[o][5] == 9 && inventory[2] < 9 && (curmap == 41 && scriptflag[9][1] == 0)) {
			objmap[lx][ly] = -1;

			inventory[2] = inventory[2] + 1;
			if(inventory[2] > 9) inventory[2] = 9;
			game_addFloatIcon(17, lx * 16, ly * 16);

			objmapf[curmap][lx][ly] = 1;
			if(curmap == 41) scriptflag[9][1] = 1;

			if(menabled == 1 && opeffects == 1) {
				int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
				Mix_Volume(snd, opeffectsvol);
			}

		}

		if(objectinfo[o][5] == 9 && inventory[2] < 9) {
			objmap[lx][ly] = -1;

			inventory[2] = inventory[2] + 1;
			if(inventory[2] > 9) inventory[2] = 9;
			game_addFloatIcon(17, lx * 16, ly * 16);

			objmapf[curmap][lx][ly] = 1;

			if(menabled == 1 && opeffects == 1) {
				int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
				Mix_Volume(snd, opeffectsvol);
			}

		}
	}
}

void game_loadmap(int mapnum)
{
	unsigned int ccc;
	SDL_Rect trect;
	FILE *fp;
	char name[256];
	int tempmap[320][200];

	ccc = SDL_MapRGB(clipbg->format, 255,255,255);

	curmap = mapnum;

	trect.x = 0;
	trect.y	= 0;
	trect.w = 320;
	trect.h = 240;
	SDL_FillRect(mapbg, &trect, 0);
	SDL_FillRect(clipbg, &trect, ccc);
	SDL_FillRect(clipbg2, &trect, ccc);

	forcepause = 0;
	cloudson = 0;
	if(mapnum < 6) cloudson = 1;
	if(mapnum > 41) cloudson = 1;
	if(mapnum > 47) cloudson = 0;
	if(mapnum == 52) cloudson = 1;
	if(mapnum == 60) cloudson = 1;
	if(mapnum == 50) cloudson = 1;
	if(mapnum == 54) cloudson = 1;
	if(mapnum == 58) cloudson = 1;
	if(mapnum == 62) cloudson = 1;
	if(mapnum == 83) cloudson = 1;

	// -----------special case
	dontdrawover = 0;
	if(mapnum == 24) dontdrawover = 1;

	if((mapnum == 53 || mapnum == 57 || mapnum == 61 || mapnum == 65 || mapnum == 62) && scriptflag[60][0] > 0) mapnum = mapnum + 100;
	if((mapnum == 161 || mapnum == 162) && scriptflag[60][0] == 2) mapnum = mapnum + 100;

	for(int i = 0; i <= MAXSPELL; i++)
		spellinfo[i].frame = 0;

	roomlock = 0;

	// read *.map file
	sprintf(name, "mapdb/%04i.map", mapnum); printf("Reading %s\n", name);
	fp = fopen(name, "r");
	for(int x = 0; x <= 319; x++)
		for(int y = 0; y <= 199; y++) {
			INPUT("%i", &tempmap[x][y]);
		}
	fclose(fp);

	for(int x = 0; x <= 319; x++)
		for(int y = 0; y <= 239; y++) {
			triggerloc[x][y] = -1;
		}

	// read *.trg file
	sprintf(name, "mapdb/%04i.trg", mapnum); printf("Reading %s\n", name);
	fp = fopen(name, "r");

	INPUT("%i", &ntriggers);

	for(int i = 0; i < ntriggers; i++) {
		int mapx, mapy, trig;

		INPUT("%i", &mapx);
		INPUT("%i", &mapy);
		INPUT("%i", &trig);

		triggerloc[mapx][mapy] = trig;
	}
	fclose(fp);


	for(int y = 0; y <= 23; y++)
		for(int x = 0; x <= 39; x++)
			rampdata[x][y] = tempmap[3 * 40 + x][y + 40];


	for(int y = 0; y <= 23; y++) 
		for(int x = 0; x <= 39; x++)
			for(int l = 0; l <= 2; l++)
				for(int a = 0; a <= 2; a++)
					tileinfo[l][x][y][a] = 0;

	if(scriptflag[4][0] == 1 && curmap == 4) {
		triggerloc[9][7] = 5004;
		tempmap[9][7] = 41;
		tempmap[9][7 + 40] = 0;
	}

	for(int y = 0; y <= 23; y++) {
		for(int x = 0; x <= 39; x++) {
			for(int l = 0; l <= 2; l++) {
				int lx, ly, curtile, curtilelayer;
				int curtilel, curtilex, curtiley;
				int pass, ffa, ffb;

				ly = y;
				lx = x + l * 40;

				// tile
				curtile = tempmap[lx][ly];
				curtilelayer = tempmap[lx][ly + 40];

				if(curtile > 0) {
					curtile = curtile - 1;
					curtilel = curtilelayer;
					curtilex = curtile % 20;
					curtiley = (curtile - curtilex) / 20;

					tileinfo[l][x][y][0] = curtile + 1;
					tileinfo[l][x][y][1] = curtilelayer;

					rcSrc.x = curtilex * 16;
					rcSrc.y = curtiley * 16;
					rcSrc.w = 16;
					rcSrc.h = 16;

					rcDest.x = x * 16;
					rcDest.y = y * 16;
					rcDest.w = 16;
					rcDest.h = 16;

					pass = 0;

					if(l == 2 && curtilel == 1) {
						for(int ff = 0; ff <= 5; ff++) {
							ffa = 20 * 5 - 1 + ff * 20;
							ffb = 20 * 5 + 4 + ff * 20;
							if(curtile > ffa && curtile < ffb) {
								SDL_SetAlpha(tiles[curtilel], SDL_SRCALPHA, 128);
								pass = 1;
							}
						}
					}
					if(l == 1 && curtilel == 2) {
						for(int ff = 0; ff <= 4; ff++) {
							ffa = 20 * (5 + ff) + 3;
							if(curtile == ffa) {
								SDL_SetAlpha(tiles[curtilel], SDL_SRCALPHA, 192);
								pass = 1;
							}
						}
					}

					SDL_BlitSurface(tiles[curtilel], &rcSrc, mapbg, &rcDest);
					SDL_SetAlpha(tiles[curtilel], SDL_SRCALPHA, 255);

					rcDest.x = x * 8;
					rcDest.y = y * 8;
					rcDest.w = 8;
					rcDest.h = 8;

					SDL_FillRect(clipbg, &rcDest, 0);
				}

			}
		}
	}


	for(int x = 0; x <= 39; x++) {
		for(int y = 0; y <= 23; y++) {

			int d = tempmap[3 * 40 + x][y];

			int clip = 0;
			int npc = 0;
			int obj = 0;

			if(scriptflag[4][0] == 1 && x == 9 && y == 7) d = 99;

			if(d > 0) {
				clip = d % 2;
				d = (d - clip) / 2;
				npc = d % 2;
				d = (d - npc) / 2;
				obj = d % 2;

				if(d == 99 && x == 9 && y == 7) clip = 1;

				if(clip) {
					if(d != 99) d = tempmap[6 * 40 + x][y];
					if(d == 99) d = 1;

					int x1 = x * 8;
					int y1 = y * 8;

					if(d == 1) {
						for(int i = 0; i <= 7; i++) {
							sys_line(clipbg, x1, y1 + i, x1 + 7 - i, y1 + i, ccc);
						}
					} else if(d == 2) {
						sys_line(clipbg, x1, y1, x1 + 7, y1, ccc);
						sys_line(clipbg, x1, y1 + 1, x1 + 7, y1 + 1, ccc);
					} else if(d == 3) {
						for(int i = 0; i <= 7; i++) {
							sys_line(clipbg, x1 + i, y1 + i, x1 + 7, y1 + i, ccc);
						}
					} else if(d == 4) {
						sys_line(clipbg, x1, y1, x1, y1 + 7, ccc);
						sys_line(clipbg, x1 + 1, y1, x1 + 1, y1 + 7, ccc);
					} else if(d == 5) {
						rcDest.x = x1;
						rcDest.y = y1;
						rcDest.w = 8;
						rcDest.h = 8;
						SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
					} else if(d == 6) {
						sys_line(clipbg, x1 + 7, y1, x1 + 7, y1 + 7, ccc);
						sys_line(clipbg, x1 + 6, y1, x1 + 6, y1 + 7, ccc);
					} else if(d == 7) {
						for(int i = 0; i <= 7; i++) {
							sys_line(clipbg, x1, y1 + i, x1 + i, y1 + i, ccc);
						}
					} else if(d == 8) {
						sys_line(clipbg, x1, y1 + 7, x1 + 7, y1 + 7, ccc);
						sys_line(clipbg, x1, y1 + 7, x1 + 6, y1 + 6, ccc);
					} else if(d == 9) {
						for(int i = 0; i <= 7; i++) {
							sys_line(clipbg, x1 + 7 - i, y1 + i, x1 + 7, y1 + i, ccc);
						}
					}
				}
			}
		}
	}

	lastobj = 0;
	lastnpc = 0;

	for(int i = 0; i <= MAXNPC; i++)
		npcinfo[i].onmap = 0;

	for(int x = 0; x <= 19; x++) {
		for(int y = 0; y <= 19; y++) {
			int d = tempmap[3 * 40 + x][y];

			int clip = 0;
			int npc = 0;
			int obj = 0;
			if(d > 0) {
				clip = d % 2;
				d = (d - clip) / 2;
				npc = d % 2;
				d = (d - npc) / 2;
				obj = d % 2;
			}

			objmap[x][y] = -1;

			if(obj == 1) {

				int o = tempmap[5 * 40 + x][y];

				if(objmapf[curmap][x][y] == 0) {
					objmap[x][y] = o;

					if(objectinfo[o][0] > 1) {
						if(o > lastobj) lastobj = o;
					}

					int x1 = x * 8;
					int y1 = y * 8;

					rcDest.x = x1;
					rcDest.y = y1;
					rcDest.w = 8;
					rcDest.h = 8;

					if(objectinfo[o][4] == 1) SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
					if(objectinfo[o][4] == 3) SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
				}
			}
			if(npc == 1) {
				int o = tempmap[4 * 40 + x][y];

				if(o > lastnpc) lastnpc = o;

				npcinfo[o].x = x * 16 - 4;
				npcinfo[o].y = y * 16 - 5;

				npcinfo[o].walkdir = 1;
				npcinfo[o].onmap = 1;
			}
		}
	}


	if(curmap == 62 && scriptflag[8][0] > 0) lastnpc = 0;
	if(curmap == 73 && scriptflag[12][0] > 0) lastnpc = 0;
	if(curmap == 81 && scriptflag[13][0] > 0) lastnpc = 0;

	if(curmap == 73 && scriptflag[12][0] == 0) roomlock = 1;
	if(curmap == 81 && scriptflag[13][0] == 0) roomlock = 1;
	if(curmap == 83 && scriptflag[15][0] == 0) roomlock = 1;
	if(curmap == 82) roomlock = 1;

	// read *.npc file
	sprintf(name, "mapdb/%04i.npc", mapnum); printf("Reading %s\n", name);
	fp = fopen(name, "r");

	for(int i = 0; i <= MAXNPC; i++) {
		INPUT("%i", &npcinfo[i].spriteset);
		INPUT("%i", &npcinfo[i].x1);
		INPUT("%i", &npcinfo[i].y1);
		INPUT("%i", &npcinfo[i].x2);
		INPUT("%i", &npcinfo[i].y2);
		INPUT("%i", &npcinfo[i].movementmode);
		INPUT("%i", &npcinfo[i].hp);
		INPUT("%i", &npcinfo[i].item1);
		INPUT("%i", &npcinfo[i].item2);
		INPUT("%i", &npcinfo[i].item3);
		INPUT("%i", &npcinfo[i].script);

		// baby dragon
		if(npcinfo[i].spriteset == 1) {
			npcinfo[i].hp = 12;
			npcinfo[i].attackdelay = 2000;

			npcinfo[i].attackdamage = 2;
			npcinfo[i].spelldamage = 0;

			npcinfo[i].walkspd = 1;

			if(RND() * 5 == 0) npcinfo[i].hp = 0;
		}

		// onewing
		if(npcinfo[i].spriteset == 2) {
			npcinfo[i].hp = 200;
			npcinfo[i].attackdelay = 2000;
			npcinfo[i].swayspd = 1;

			npcinfo[i].attackdamage = 24;
			npcinfo[i].spelldamage = 30;

			npcinfo[i].walkspd = 1.4;
			npcinfo[i].castpause = ticks;
		}

		// boss1
		if(npcinfo[i].spriteset == 3) {
			npcinfo[i].hp = 300;
			npcinfo[i].attackdelay = 2200;

			npcinfo[i].attackdamage = 0;
			npcinfo[i].spelldamage = 30;

			npcinfo[i].walkspd = 1.2;
		}

		// black knights
		if(npcinfo[i].spriteset == 4) {
			npcinfo[i].hp = 200;
			npcinfo[i].attackdelay = 2800;

			npcinfo[i].attackdamage = 0;
			npcinfo[i].spelldamage = 30;

			npcinfo[i].walkspd = 1;
		}

		// boss2 firehydra
		if(npcinfo[i].spriteset == 5) {
			npcinfo[i].hp = 600;
			npcinfo[i].attackdelay = 2200;

			npcinfo[i].attackdamage = 50;
			npcinfo[i].spelldamage = 30;

			npcinfo[i].walkspd = 1.3;

			npcinfo[i].swayangle = 0;
		}

		// baby fire dragon
		if(npcinfo[i].spriteset == 6) {
			npcinfo[i].hp = 20;
			npcinfo[i].attackdelay = 1500;

			npcinfo[i].attackdamage = 0;
			npcinfo[i].spelldamage = 12;

			npcinfo[i].walkspd = 1;

			if(RND() * 5 == 0) npcinfo[i].hp = 0;
		}

		// priest1
		if(npcinfo[i].spriteset == 7) {
			npcinfo[i].hp = 40;
			npcinfo[i].attackdelay = 5000;

			npcinfo[i].attackdamage = 0;
			npcinfo[i].spelldamage = 8;

			npcinfo[i].walkspd = 1;

			if(RND() * 8 == 0) npcinfo[i].hp = 0;
		}

		// yellow fire dragon
		if(npcinfo[i].spriteset == 8) {
			npcinfo[i].hp = 100;
			npcinfo[i].attackdelay = 1500;

			npcinfo[i].attackdamage = 0;
			npcinfo[i].spelldamage = 24;

			npcinfo[i].walkspd = 1;

			if(RND() * 5 == 0) npcinfo[i].hp = 0;
		}

		// twowing
		if(npcinfo[i].spriteset == 9) {
			npcinfo[i].hp = 140;
			npcinfo[i].attackdelay = 2000;
			npcinfo[i].swayspd = 1;

			npcinfo[i].attackdamage = 30;
			npcinfo[i].spelldamage = 0;

			npcinfo[i].walkspd = 1;

			npcinfo[i].castpause = 0;
		}

		// dragon2
		if(npcinfo[i].spriteset == 10) {
			npcinfo[i].hp = 80;
			npcinfo[i].attackdelay = 1500;

			npcinfo[i].attackdamage = 24;
			npcinfo[i].spelldamage = 0;

			npcinfo[i].walkspd = 1;

			npcinfo[i].floating = RND() * 16;
		}

		// end boss
		if(npcinfo[i].spriteset == 11) {
			npcinfo[i].hp = 1200;
			npcinfo[i].attackdelay = 2000;

			npcinfo[i].attackdamage = 100;
			npcinfo[i].spelldamage = 60;

			npcinfo[i].walkspd = 1;

			npcinfo[i].floating = RND() * 16;
		}

		// bat kitty
		if(npcinfo[i].spriteset == 12) {
			npcinfo[i].hp = 800;
			npcinfo[i].attackdelay = 2000;

			npcinfo[i].attackdamage = 100;
			npcinfo[i].spelldamage = 50;

			npcinfo[i].walkspd = 1;

			npcinfo[i].floating = RND() * 16;
		}

		if(npcinfo[i].onmap == 0) npcinfo[i].hp = 0;

		npcinfo[i].maxhp = npcinfo[i].hp;

		npcinfo[i].attacking = 0;
		npcinfo[i].attackframe = 0;
		npcinfo[i].cattackframe = 0;
		npcinfo[i].attackspd = 1.5;
		npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay * (1 + RND() * 2);

		if(npcinfo[i].spriteset == 2 || npcinfo[i].spriteset == 9) {
			npcinfo[i].bodysection[0].sprite = 0;
			npcinfo[i].bodysection[1].sprite = 1;
			npcinfo[i].bodysection[2].sprite = 2;
			npcinfo[i].bodysection[3].sprite = 3;
			npcinfo[i].bodysection[4].sprite = 4;
			npcinfo[i].bodysection[5].sprite = 3;
			npcinfo[i].bodysection[6].sprite = 3;
			npcinfo[i].bodysection[7].sprite = 5;

			npcinfo[i].bodysection[0].bonelength = 8;
			npcinfo[i].bodysection[1].bonelength = 7;
			npcinfo[i].bodysection[2].bonelength = 6;
			npcinfo[i].bodysection[3].bonelength = 4;
			npcinfo[i].bodysection[4].bonelength = 4;
			npcinfo[i].bodysection[5].bonelength = 4;
			npcinfo[i].bodysection[6].bonelength = 4;

			for(int f = 0; f <= 7; f++) {
				npcinfo[i].bodysection[f].x = npcinfo[i].x + 12;
				npcinfo[i].bodysection[f].y = npcinfo[i].y + 14;
			}

			npcinfo[i].headtargetx[0] = npcinfo[i].x + 12;
			npcinfo[i].headtargety[0] = npcinfo[i].y + 14;

		}

		if(npcinfo[i].spriteset == 5) {
			for(int f = 0; f <= 29; f++) {
				npcinfo[i].bodysection[f].x = npcinfo[i].x + 12;
				npcinfo[i].bodysection[f].y = npcinfo[i].y + 14;
			}

			for(int f = 0; f <= 2; f++) {
				npcinfo[i].headtargetx[f] = npcinfo[i].x + 12;
				npcinfo[i].headtargety[f] = npcinfo[i].y + 14;

				npcinfo[i].attacking2[f] = 0;
				npcinfo[i].attackframe2[f] = 0;
			}
		}

		if(npcinfo[i].script == 2) {
			roomlock = 1;
			if(scriptflag[2][0] > 0) {
				roomlock = 0;
				npcinfo[i].hp = 0;
			}
		}

		if(npcinfo[i].script == 3) {
			roomlock = 1;
			if(scriptflag[3][0] > 0) {
				roomlock = 0;
				npcinfo[i].hp = 0;
			}
		}

		if(npcinfo[i].script == 5) {
			roomlock = 1;
			if(scriptflag[5][0] > 0) {
				roomlock = 0;
				npcinfo[i].hp = 0;
			}
		}

		if(npcinfo[i].script == 15) {
			roomlock = 1;
			if(scriptflag[15][0] > 0) {
				roomlock = 0;
				npcinfo[i].hp = 0;
			}
		}

		npcinfo[i].pause = ticks;
	}

	fclose(fp);


	int cx, cy, npx, npy, lx, ly;

	// academy master key
	if(curmap == 34 && scriptflag[2][0] == 1) {
		cx = 9;
		cy = 7;

		objmap[cx][cy] = 5;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}

	// academy crystal
	if(curmap == 24 && player.foundcrystal == 0 && scriptflag[3][0] == 1) {
		cx = 9;
		cy = 7;

		objmap[cx][cy] = 6;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}

	// gardens master key
	if(curmap == 62 && scriptflag[8][0] == 1) {
		cx = 13;
		cy = 7;

		objmap[cx][cy] = 5;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}

	// gardens fidelis sword
	if(curmap == 66 && scriptflag[5][0] == 1 && player.sword == 1) {
		cx = 9;
		cy = 6;

		objmap[cx][cy] = 9;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}

	// citadel armour
	if(curmap == 73 && scriptflag[12][0] == 1 && player.armour == 1) {
		cx = 8;
		cy = 7;

		objmap[cx][cy] = 16;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}

	// citadel master key
	if(curmap == 81 && scriptflag[13][0] == 1) {
		cx = 11;
		cy = 10;

		objmap[cx][cy] = 5;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}


	// max ups
	if(curmap == 83 && scriptflag[15][0] == 1 && player.sword < 3) {
		cx = 6;
		cy = 8;

		objmap[cx][cy] = 18;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}

	if(curmap == 83 && scriptflag[16][0] == 1 && player.shield < 3) {
		cx = 9;
		cy = 8;

		objmap[cx][cy] = 19;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
	}

	if(curmap == 83 && scriptflag[17][0] == 1 && player.armour < 3) {
		cx = 12;
		cy = 8;

		objmap[cx][cy] = 20;

		rcDest.x = cx * 8;
		rcDest.y = cy * 8;
		rcDest.w = 8;
		rcDest.h = 8;

		npx = player.px + 12;
		npy = player.py + 20;

		lx = (int)npx / 16; //(npx - (npx % 16)) / 16;
		ly = (int)npy / 16; //(npy - (npy % 16)) / 16;

		if(lx == cx && ly == cy) player.py = player.py + 16;

		SDL_FillRect(clipbg, &rcDest, SDL_MapRGB(clipbg->format, 255,255,255));
		}

	SDL_BlitSurface(clipbg, NULL, clipbg2, NULL);
}

void game_main()
{
	game_title(0);
	game_saveloadnew();
}

void game_newgame()
{
	float xofs = 0;
	float ld = 0, add;
	int ticks, bticks, pauseticks, cnt = 0;

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = 320;
	rcSrc.h = 240;

	SDL_FillRect(videobuffer2, &rcSrc, 0);
	SDL_FillRect(videobuffer3, &rcSrc, 0);


	rc.x = 0;
	rc.y = 0;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;
	rc2.w = 320;
	rc2.h = 240;


	//x = 160 - 4 * len(stri$) // ??

	ticks = SDL_GetTicks();
	pauseticks = ticks + 500;
	bticks = ticks;


	SDL_BlitSurface(videobuffer, NULL, videobuffer3, NULL);
	SDL_BlitSurface(video, &rc2, videobuffer2, NULL);

	//cursel = 0;

	//keypause = ticks + 220;

	fpsr = 0.0;
	int y = 140;

	if(menabled == 1 && opmusic == 1) {
		Mix_HaltChannel(-1);
		musicchannel = Mix_PlayChannel(-1, mendofgame, -1);
		Mix_Volume(musicchannel, 0);
	}

	secsingame = 0;
	secstart = 0;

	int ldstop = 0;

	do {
		ld += 4 * fpsr;
		if((int)ld > opmusicvol) ld = opmusicvol;
		if(menabled == 1 && ldstop == 0) {
			Mix_Volume(musicchannel, (int)ld);
			if((int)ld == opmusicvol) ldstop = 1;
		}

 		rc.x = -xofs;
		rc.y = 0;

		SDL_BlitSurface(titleimg, NULL, videobuffer, &rc);

 		rc.x = -xofs + 320;
		rc.y = 0;

		SDL_BlitSurface(titleimg, NULL, videobuffer, &rc);

 		rc.x = 0;
		rc.y = 0;

		if(++cnt >= 6) {
			cnt = 0;
			y--;
		}

		for(int i = 0; i <= 37; i++) {
			int yy, x;

			yy = y + i * 10;
			if(yy > -8 && yy < 240) {
				x = 160 - strlen(story[i]) * 4;
				sys_print(videobuffer, story[i], x, yy, 4);
			}

			if(yy < 10 && i == 37) goto __exit_do;
		}

		rc.x = 0;
		rc.y = 0;
		rc.w = 320;
		rc.h = 240;

		SDL_BlitSurface(videobuffer, &rc, video, &rc2);
		SDL_Flip(video);

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24.0;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		add = 0.5 * fpsr;
		if(add > 1) add = 1;
		xofs += add;
		if(xofs >= 320) xofs = xofs - 320;

		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);

		if(event.type == SDL_KEYDOWN) cnt = 6;
		if(keys[SDLK_ESCAPE]) goto __exit_do;

		SDL_Delay(10);
	} while(1);
__exit_do:

	player.px = 0;
	player.py = 0;
	player.opx = 0;
	player.opy = 0;
	player.walkdir = 0;
	player.walkframe = 0;
	player.walkspd = 0;
	player.attackframe = 0;
	player.attackspd = 0;
	player.hp = 0;
	player.maxhp = 0;
	player.hpflash = 0;
	player.level = 0;
	player.sword = 0;
	player.shield = 0;
	player.armour = 0;
	for(int i = 0; i <= 5; i++) {
		player.foundspell[i] = 0;
		player.spellcharge[i] = 0;
	}
	player.flasks = 0;
	player.foundcrystal = 0;
	player.crystalcharge = 0;
	player.attackstrength = 0;
	player.spelldamage = 0;
	player.sworddamage = 0;
	player.masterkey = 0;
	player.exp = 0;
	player.nextlevel = 0;

	memset(scriptflag, 0, sizeof(scriptflag));
	memset(objmapf, 0, sizeof(objmapf));
	memset(inventory, 0, sizeof(inventory));
	memset(roomlocks, 0, sizeof(roomlocks));

	roomlocks[66] = 2;
	roomlocks[24] = 2;
	roomlocks[17] = 1;
	roomlocks[34] = 1;
	roomlocks[50] = 1;
	roomlocks[73] = 1;
	roomlocks[82] = 2;

	player.walkspd = 1.1;
	animspd = 0.5;
	attacking = 0;
	player.attackspd = 1.5;

	player.sword = 1;
	player.level = 1;
	player.nextlevel = 50;
	player.shield = 1;
	player.armour = 1;
	player.hp = 14;
	player.maxhp = player.hp;

	player.sworddamage = player.level * 2;
	player.spelldamage = player.level * 1.5;

	player.px = 15*16 - 4;
	player.py = 6*16 - 4;
	player.walkdir = 1;

	pgardens = 0;
	ptown = 0;
	pboss = 0;
	pacademy = 0;
	pcitadel = 0;

	game_loadmap(2);

	game_playgame();
}

void game_playgame()
{
	game_swash();

	if(pmenu == 1 && menabled == 1) {
		Mix_HaltChannel(menuchannel);
		pmenu = 0;
	}

	do {
		if(forcepause == 0) {
			game_updanims();
			game_updnpcs();
		}

		game_checktrigger();
		game_checkinputs();

		if(forcepause == 0) game_handlewalking();

		game_updatey();
		game_drawview();

		game_updmusic();

		sys_update();
	} while(1);
}

void game_processtrigger(int trignum)
{
	int trigtype, tx, ty, tmap, tjumpstyle, tsx, tsy;

	trigtype = triggers[trignum][0];

	if(roomlock == 1) return;
	// map jump------------------------------
	if(trigtype == 0) {
		tx = triggers[trignum][1];
		ty = triggers[trignum][2];
		tmap = triggers[trignum][3];
		tjumpstyle = triggers[trignum][4];

		if(roomlocks[tmap] > 0) {
			if(saidlocked == 0) game_eventtext("Locked");
			saidlocked = 1;
			canusekey = 1;
			locktype = roomlocks[tmap];
			roomtounlock = tmap;
			return;
		}

		if(tmap == 1) {
			if(saidjammed == 0) game_eventtext("Door Jammed!");
			saidjammed = 1;
			return;
		}

		saidlocked = 0;
		saidjammed = 0;

		// loc-sxy+oldmaploc
		if(tjumpstyle == 0) {

			tsx = triggers[trignum][5];
			tsy = triggers[trignum][6];

			player.px += (tx - tsx) * 16;
			player.py += (ty - tsy) * 16;

			// HACKFIX
			if(player.px < 0) player.px = 0;
			if(player.py < 0) player.py = 0;

			if(tmap > 0) {
				if(menabled == 1 && opeffects == 1) {
					int snd = Mix_PlayChannel(-1, sfx[snddoor], 0);
					Mix_Volume(snd, opeffectsvol);
				}

				game_loadmap(tmap);
				game_swash();
			}
		}
	}

	for(int i = 0; i <= MAXFLOAT; i++) {
		floattext[i][0] = 0;
		floaticon[i][0] = 0;
	}
}

void game_saveloadnew()
{
	float y; int yy;
	int currow, curcol, lowerlock;
	int ticks, ticks1, tickpause;
	int plevel, sx, sy;

	clouddeg = 0;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;

	SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);
	SDL_SetAlpha(saveloadimg, SDL_SRCALPHA, 192);

	currow = 0;
	curcol = 0;

	lowerlock = 0;

	ticks = SDL_GetTicks();
	ticks1 = ticks;
	tickpause = ticks + 150;

	do {
		SDL_FillRect(videobuffer, NULL, 0);

		y = y + 1 * fpsr;

		rcDest.x = 256 + 256 * cos(3.141592 / 180 * clouddeg * 40);
		rcDest.y = 192 + 192 * sin(3.141592 / 180 * clouddeg * 40);
		rcDest.w = 320;
		rcDest.h = 240;

		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 128);
		SDL_BlitSurface(cloudimg, &rcDest, videobuffer, NULL);
		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 64);


		rcDest.x = 256;
		rcDest.y = 192;
		rcDest.w = 320;
		rcDest.h = 240;

		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 128);
		SDL_BlitSurface(cloudimg, &rcDest, videobuffer, NULL);
		SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 64);

		SDL_BlitSurface(saveloadimg, NULL, videobuffer, NULL);

		SDL_PollEvent(&event);
		keys = SDL_GetKeyState(NULL);

		if(tickpause < ticks) {
			if(event.type == SDL_KEYDOWN) {
				itemticks = ticks + 220;

				if(keys[SDLK_RETURN] || keys[SDLK_SPACE]) {
					// QUIT - non existent :)
					if(currow == 0 && curcol == 4) {
						exit(1);
					}
					// RETURN
					if(currow == 0 && curcol == 3) {
						// reset keys to avoid returning
						keys[SDLK_RETURN] = keys[SDLK_SPACE] = 0;
						return;
					}
					// NEW GAME
					if(currow == 0 && curcol == 0) game_newgame();

					// LOAD GAME
					if(currow == 0 && curcol == 1) {
						lowerlock = 1;
						currow = 1 + saveslot;
						tickpause = ticks + 125;
					}
					// SAVE GAME
					if(currow == 0 && curcol == 2) {
						lowerlock = 1;
						currow = 1;
						tickpause = ticks + 125;
					}


					if(lowerlock == 1 && curcol == 1 && tickpause < ticks) {
						FILE *fp;
						char line[256];

						sprintf(line, "data/player%i.sav", currow - 1);
						fp = fopen(line, "w");

						PRINT("%i", player.level);

						PRINT("%i", (secstart + secsingame));
						PRINT("a", 0);

						PRINT("%f", player.px);
						PRINT("%f", player.py);
						PRINT("%f", player.opx);
						PRINT("%f", player.opy);
						PRINT("%i", player.walkdir);
						PRINT("%f", player.walkframe);
						PRINT("%f", player.walkspd);
						PRINT("%f", player.attackframe);
						PRINT("%f", player.attackspd);
						PRINT("%i", player.hp);
						PRINT("%i", player.maxhp);
						PRINT("%f", player.hpflash);
						PRINT("%i", player.level);
						PRINT("%i", player.sword);
						PRINT("%i", player.shield);
						PRINT("%i", player.armour);
						for(int i = 0; i <= 5; i++) {
							PRINT("%i", player.foundspell[i]);
							PRINT("%f", player.spellcharge[i]);
						}
						for(int a = 0; a <= 4; a++) {
							PRINT("%i", inventory[a]);
						}
						PRINT("%i", player.foundcrystal);
						PRINT("%f", player.crystalcharge);
						PRINT("%f", player.attackstrength);
						PRINT("%i", player.spelldamage);
						PRINT("%i", player.sworddamage);
						PRINT("%i", player.masterkey);
						PRINT("%i", player.exp);
						PRINT("%i", player.nextlevel);
						for(int a = 0; a <= 99; a++) {
							for(int b = 0; b <= 9; b++) {
								PRINT("%i", scriptflag[a][b]);
							}
						}
						PRINT("%i", curmap);

						for(int a = 0; a <= 999; a++) {
							for(int b = 0; b <= 20; b++) {
								for(int c = 0; c <= 14; c++) {
									PRINT("%i", objmapf[a][b][c]);
								}
							}
						}

						for(int a = 0; a <= 200; a++) {
							PRINT("%i", roomlocks[a]);
						}

						PRINT("%f", player.spellstrength);

						fclose(fp);

						secstart = secstart + secsingame;
						secsingame = 0;
						lowerlock = 0;
						saveslot = currow - 1;
						currow = 0;
					}

					if(lowerlock == 1 && curcol == 2 && tickpause < ticks) {
						FILE *fp;
						char line[256];

						sprintf(line, "data/player%i.sav", currow - 1);
						fp = fopen(line, "r");
	
						INPUT("%i", &player.level);
						INPUT("%i", &secstart);
						INPUT("%s", line);

						INPUT("%f", &player.px);
						INPUT("%f", &player.py);
						INPUT("%f", &player.opx);
						INPUT("%f", &player.opy);
						INPUT("%i", &player.walkdir);
						INPUT("%f", &player.walkframe);
						INPUT("%f", &player.walkspd);
						INPUT("%f", &player.attackframe);
						INPUT("%f", &player.attackspd);
						INPUT("%i", &player.hp);
						INPUT("%i", &player.maxhp);
						INPUT("%f", &player.hpflash);
						INPUT("%i", &player.level);
						INPUT("%i", &player.sword);
						INPUT("%i", &player.shield);
						INPUT("%i", &player.armour);
						for(int i = 0; i <= 5; i++) {
							INPUT("%i", &player.foundspell[i]);
							INPUT("%f", &player.spellcharge[i]);
						}
						for(int a = 0; a <= 4; a++) {
							INPUT("%i", &inventory[a]);
						}

						INPUT("%i", &player.foundcrystal);
						INPUT("%f", &player.crystalcharge);
						INPUT("%f", &player.attackstrength);
						INPUT("%i", &player.spelldamage);
						INPUT("%i", &player.sworddamage);
						INPUT("%i", &player.masterkey);
						INPUT("%i", &player.exp);
						INPUT("%i", &player.nextlevel);
						for(int a = 0; a <= 99; a++) {
							for(int b = 0; b <= 9; b++) {
								INPUT("%i", &scriptflag[a][b]);
							}
						}
						INPUT("%i", &curmap);

						for(int a = 0; a <= 999; a++) {
							for(int b = 0; b <= 20; b++) {
								for(int c = 0; c <= 14; c++) {
									INPUT("%i", &objmapf[a][b][c]);
								}
							}
						}

						for(int a = 0; a <= 200; a++) {
							INPUT("%i", &roomlocks[a]);
						}

						INPUT("%f", &player.spellstrength);

						player.walkspd = 1.1;
						animspd = .5;
						attacking = 0;
						player.attackspd = 1.5;

						pgardens = 0;
						ptown = 0;
						pboss = 0;
						pacademy = 0;
						pcitadel = 0;


						fclose(fp);

						Mix_HaltChannel(-1);

						secsingame = 0;
						saveslot = currow - 1;
						game_loadmap(curmap);
						game_playgame();
					}

					tickpause = ticks + 125;
				}

				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					if(lowerlock == 0) return;
					lowerlock = 0;
					currow = 0;
					tickpause = ticks + 125;
					break;
				case SDLK_DOWN:
					if(lowerlock == 1) {
						currow = currow + 1;
						if(currow == 5) currow = 1;
						tickpause = ticks + 125;
					}
					break;

				case SDLK_UP:
					if(lowerlock == 1) {
						currow = currow - 1;
						if(currow == 0) currow = 4;
						tickpause = ticks + 125;
					}
					break;

				case SDLK_LEFT:
					if(lowerlock == 0) {
						curcol = curcol - 1;
						if(curcol == -1) curcol = 3;
						tickpause = ticks + 125;
					}
					break;

				case SDLK_RIGHT:
					if(lowerlock == 0) {
						curcol = curcol + 1;
						if(curcol == 4) curcol = 0;
						tickpause = ticks + 125;
					}
					break;
				default:
					;
				}
			}
		}


		// savestates---------------------------------
		// read it only when needed!

		for(int ff = 0; ff <= 3; ff++) {
			FILE *fp;
			char line[256];
			int asecstart;

			sprintf(line,"data/player%i.sav", ff);

			fp = fopen(line, "r");

			INPUT("%i", &plevel);

			if(plevel > 0) {
				playera.level = plevel;

				INPUT("%i", &asecstart);
				INPUT("%s", line);

				INPUT("%f", &playera.px);
				INPUT("%f", &playera.py);
				INPUT("%f", &playera.opx);
				INPUT("%f", &playera.opy);
				INPUT("%i", &playera.walkdir);
				INPUT("%f", &playera.walkframe);
				INPUT("%f", &playera.walkspd);
				INPUT("%f", &playera.attackframe);
				INPUT("%f", &playera.attackspd);
				INPUT("%i", &playera.hp);
				INPUT("%i", &playera.maxhp);
				INPUT("%f", &playera.hpflash);
				INPUT("%i", &playera.level);
				INPUT("%i", &playera.sword);
				INPUT("%i", &playera.shield);
				INPUT("%i", &playera.armour);
				for(int i = 0; i <= 5; i++) {
					INPUT("%i", &playera.foundspell[i]);
					INPUT("%f", &playera.spellcharge[i]);
				}
				for(int a = 0; a <= 4; a++) {
					INPUT("%i", &inventory[a]);
				}
				INPUT("%i", &playera.foundcrystal);
				INPUT("%f", &playera.crystalcharge);
				INPUT("%f", &playera.attackstrength);
				INPUT("%i", &playera.spelldamage);
				INPUT("%i", &playera.sworddamage);
				INPUT("%i", &playera.masterkey);
				INPUT("%i", &playera.exp);
				INPUT("%i", &playera.nextlevel);
			}
			fclose(fp);

			if(plevel > 0) {
				int sx, sy, cc, ss, nx, ccc;

				sx = 8;
				sy = 57 + ff * 48;

				// time
				int ase = asecstart;
				int h = ((ase - (ase % 3600)) / 3600);
				ase = (ase - h * 3600);
				int m = ((ase - (ase % 60)) / 60);
				int s = (ase - m * 60);

				sprintf(line, "Game Time: %02i:%02i:%02i", h, m, s);
				sys_print(videobuffer, line, 160 - strlen(line) * 4, sy, 0);

				sx  = 12;
				sy = sy + 11;
				cc = 0;

				sprintf(line, "Health: %i/%i", playera.hp, playera.maxhp);
				sys_print(videobuffer, line, sx, sy, cc);

				if(playera.level == 22)
					strcpy(line, "Level: MAX");
				else
					sprintf(line, "Level: %i", playera.level);

				sys_print(videobuffer, line, sx, sy + 11, 0);

				rcSrc.x = sx + 15 * 8 + 24;
				rcSrc.y = sy + 1;

				ss = (playera.sword - 1) * 3;
				if(playera.sword == 3) ss = 18;
				SDL_BlitSurface(itemimg[ss], NULL, videobuffer, &rcSrc);

				rcSrc.x = rcSrc.x + 16;
				ss = (playera.shield - 1) * 3 + 1;
				if(playera.shield == 3) ss = 19;
				SDL_BlitSurface(itemimg[ss], NULL, videobuffer, &rcSrc);

				rcSrc.x = rcSrc.x + 16;
				ss = (playera.armour - 1) * 3 + 2;
				if(playera.armour == 3) ss = 20;
				SDL_BlitSurface(itemimg[ss], NULL, videobuffer, &rcSrc);

				nx = rcSrc.x + 13 + 3 * 8;
				rcSrc.x = nx - 17;

				if(playera.foundcrystal == 1) {
					rcSrc.x = rcSrc.x + 17;

					SDL_BlitSurface(itemimg[7], NULL, videobuffer, &rcSrc);

					ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32);

					for(int i = 0; i <= 4; i++) {
						rcSrc.x = rcSrc.x + 17;
						if(playera.foundspell[i] == 1) SDL_BlitSurface(itemimg[8 + i], NULL, videobuffer, &rcSrc);
					}
				}
			} else {
				sx = 10;
				sy = 57 + ff * 48;
				sys_print(videobuffer, "Empty", 160 - 5 * 4, sy, 0);
			}
		}
		// ------------------------------------------


		if(currow == 0) {
			rcDest.y = 18;
			if(curcol == 0) rcDest.x = 10;
			if(curcol == 1) rcDest.x = 108;
			if(curcol == 2) rcDest.x = 170;
			if(curcol == 3) rcDest.x = 230;
			rcDest.x = (float)(rcDest.x + 2 + 2 * sin(3.14159 * 2 * itemyloc / 16));
		}

		if(currow > 0) {
			rcDest.x = (float)(0 + 2 * sin(3.14159 * 2 * itemyloc / 16));
			rcDest.y = (float)(53 + (currow - 1) * 48);
		}

		SDL_BlitSurface(itemimg[15], NULL, videobuffer, &rcDest);


		if(lowerlock == 1) {
			rcDest.y = 18;
			if(curcol == 1) rcDest.x = 108;
			if(curcol == 2) rcDest.x = 170;
			rcDest.x = rcDest.x; // + 2 + 2 * sin(-3.14159 * 2 * itemyloc / 16)

			SDL_BlitSurface(itemimg[15], NULL, videobuffer, &rcDest);
		}

		yy = 255;
		if(ticks < ticks1 + 1000) {
			yy = 255 * (ticks - ticks1) / 1000;
			if(yy < 0) yy = 0;
			if(yy > 255) yy = 255;
		}

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, (int)yy);

		SDL_BlitSurface(videobuffer, NULL, video, &rc2);
		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		clouddeg += 0.01 * fpsr;
		while(clouddeg >= 360) clouddeg -= 360;

		itemyloc += 0.6 * fpsr;
		while(itemyloc >= 16) itemyloc -= 16;

		SDL_Delay(10);
	} while(1);

	SDL_SetAlpha(cloudimg, SDL_SRCALPHA, 64);
}

void game_showlogos()
{
	float y;
	int ticks1;

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = 320;
	rcSrc.h = 240;

	rc.x = 0;
	rc.y = 0;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;
	rc2.w = 320;
	rc2.h = 240;

	ticks = SDL_GetTicks();
	ticks1 = ticks;

	y = 0.0;

	do {
		rc.x = 0;
		rc.y = 0;
		rc.w = 320;
		rc.h = 240;

		y = 255.0;
		if(ticks < ticks1 + 1000) {
			y = 255.0 * ((float)(ticks - ticks1) / 1000.0);
			if(y < 0.0) y = 0.0;
			if(y > 255.0) y = 255.0;
		}

		if(ticks > ticks1 + 3000) {
			y = 255.0 - 255.0 * ((float)(ticks - ticks1 - 3000.0) / 1000.0);
			if(y < 0.0) y = 0.0;
			if(y > 255.0) y = 255.0;
		}

		SDL_FillRect(videobuffer, &rc, 0);

		SDL_SetAlpha(logosimg, SDL_SRCALPHA, (int)y);
		SDL_BlitSurface(logosimg, NULL, videobuffer, NULL);
		SDL_SetAlpha(logosimg, SDL_SRCALPHA, 255);


		SDL_BlitSurface(videobuffer, &rc, video, &rc2);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		SDL_Delay(10);
		if(ticks > ticks1 + 4000) break;
	} while(1);
}


void game_swash()
{
	float y;

	rcDest.x = 0;
	rcDest.y = 0;
	rcDest.w = 320;
	rcDest.h = 240;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;

	y = 0;

	do {
		y = y + 1 * fpsr;

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, (int)y);
		SDL_FillRect(videobuffer, &rcDest, 0);
		SDL_BlitSurface(videobuffer, &rcDest, video, &rc2);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24.0;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		clouddeg += 0.01 * fpsr;
		while(clouddeg >= 360) clouddeg = clouddeg - 360;

		if(y > 10) break;
	} while(1);

	y = 0;
	do {
		y = y + 1 * fpsr;

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, (int)(y * 25));
		SDL_BlitSurface(mapbg, NULL, videobuffer, NULL);

		if(cloudson == 1) {
			rcDest.x = (float)(256 + 256 * cos(3.141592 / 180 * clouddeg));
			rcDest.y = (float)(192 + 192 * sin(3.141592 / 180 * clouddeg));
			rcDest.w = 320;
			rcDest.h = 240;

			SDL_BlitSurface(cloudimg, &rcDest, videobuffer, NULL);
		}

		SDL_BlitSurface(videobuffer, NULL, video, &rc2);
		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24.0;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		clouddeg += 0.01 * fpsr;
		while(clouddeg >= 360) clouddeg = clouddeg - 360;

		if(y > 10) break;
	} while(1);


	SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);
}

void game_theend()
{
	float y;

	rcDest.x = 0;
	rcDest.y = 0;
	rcDest.w = 320;
	rcDest.h = 240;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;


	y = 0;
	for(int i = 0; i <= MAXFLOAT; i++) {
		floattext[i][0] = 0;
		floaticon[i][0] = 0;
	}

	do {
		y = y + 1 * fpsr;

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, (int)y);
		SDL_FillRect(videobuffer, &rcDest, 0);
		SDL_BlitSurface(videobuffer, &rcDest, video, &rc2);

		SDL_Flip(video);
		SDL_PumpEvents();

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24.0;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		if(y > 100) break;
	} while(1);

	game_title(0);
}

void game_title(int mode)
{
	float xofs = 0;
	int ticks, ticks1, keypause;
	int cursel, ldstop;
	int x, y;

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = 320;
	rcSrc.h = 240;

	SDL_FillRect(videobuffer2, &rcSrc, 0);
	SDL_FillRect(videobuffer3, &rcSrc, 0);

	rc.x = 0;
	rc.y = 0;

	rc2.x = SCR_TOPX;
	rc2.y = SCR_TOPY;
	rc2.w = 320;
	rc2.h = 240;

	//x = 160 - 4 * len(stri$); // ??

	//pauseticks = ticks + 500; // ??
	ticks = SDL_GetTicks();

	SDL_BlitSurface(videobuffer, NULL, videobuffer3, NULL);
	SDL_BlitSurface(video, &rc2, videobuffer2, NULL);

	cursel = 0;

	keypause = ticks + 220;

	ticks1 = ticks;

	if(menabled == 1 && opmusic == 1) {
		Mix_Volume(musicchannel, 0);
		Mix_Pause(musicchannel);

		menuchannel = Mix_PlayChannel(-1, mmenu, -1);
		Mix_Volume(menuchannel, opmusicvol);
		pmenu = 1;
	}

	ldstop = 0;

	float ld = 0;
	do {

		ld += 4.0 * fpsr;
		if(ld > opmusicvol) ld = opmusicvol;
		if(menabled == 1 && ldstop == 0) {
			Mix_Volume(menuchannel, (int)ld);
			if((int)ld == opmusicvol) ldstop = 1;
		}

		rc.x = -xofs;
		rc.y = 0;

		SDL_BlitSurface(titleimg, NULL, videobuffer, &rc);

		rc.x = -xofs + 320.0;
		rc.y = 0;

		SDL_BlitSurface(titleimg, NULL, videobuffer, &rc);

		rc.x = 0;
		rc.y = 0;

		SDL_BlitSurface(titleimg2, NULL, videobuffer, &rc);

		y = 172;
		x = 160 - 14 * 4;

		sys_print(videobuffer, "new game/save/load", x, y, 4);
		sys_print(videobuffer, "options", x, y + 16, 4);
		sys_print(videobuffer, "quit game", x, y + 32, 4);
		if(mode == 1) sys_print(videobuffer, "return", x, y + 48, 4);

		rc.x = (float)(x - 16 - 4 * cos(3.14159 * 2 * itemyloc / 16));
		rc.y = (float)(y - 4 + 16 * cursel);

		SDL_BlitSurface(itemimg[15], NULL, videobuffer, &rc);

		rc.x = 0;
		rc.y = 0;
		rc.w = 320;
		rc.h = 240;

		float yf = 255.0;
		if(ticks < ticks1 + 1000) {
			yf = 255.0 * ((float)(ticks - ticks1) / 1000.0);
			if(y < 0.0) yf = 0.0;
			if(y > 255.0) yf = 255.0;
		}

		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, (int)yf);
		SDL_BlitSurface(videobuffer, &rc, video, &rc2);
		SDL_SetAlpha(videobuffer, SDL_SRCALPHA, 255);

		tickspassed = ticks;
		ticks = SDL_GetTicks();

		tickspassed = ticks - tickspassed;
		fpsr = (float)tickspassed / 24.0;

		fp++;
		if(ticks > nextticks) {
			nextticks = ticks + 1000;
			fps = fp;
			fp = 0;
		}

		float add = 0.5 * fpsr;
		if(add > 1) add = 1;

		xofs = xofs + add;
		if(xofs >= 320.0) xofs -= 320.0;

		itemyloc += 0.75 * fpsr;

		while(itemyloc >= 16)
			itemyloc = itemyloc - 16;

		SDL_PollEvent(&event);

		if(keypause < ticks) {
			keys = SDL_GetKeyState(NULL);

			if(event.type == SDL_KEYDOWN) {
				keypause = ticks + 150;

				if(keys[SDLK_ESCAPE] && mode == 1) break;
				else if(keys[SDLK_UP]) {
					cursel--;
					if(cursel < 0) cursel = (mode == 1 ? 3 : 2);
				} else if(keys[SDLK_DOWN]) {
					cursel++;
					if(cursel >= (mode == 1 ? 4 : 3)) cursel = 0;
				} else if(keys[SDLK_SPACE] || keys[SDLK_RETURN]) {
					if(cursel == 0) {
						game_saveloadnew();
						ticks = SDL_GetTicks();
						keypause = ticks + 150;
						ticks1 = ticks;
					} else if(cursel == 1) {
						game_configmenu();
						ticks = SDL_GetTicks();
						keypause = ticks + 150;
						ticks1 = ticks;
					} else if(cursel == 2) {
						exit(1);
					} else if(cursel == 3) break;
				}
			}
		}

		SDL_Flip(video);
		SDL_Delay(10);
	} while(1);

	itemticks = ticks + 210;

	if(menabled == 1 && opmusic == 1) {
		Mix_HaltChannel(menuchannel);
		Mix_Resume(musicchannel);
		Mix_Volume(musicchannel, opmusicvol);
		pmenu = 0;
	}
}

void game_updanims()
{
	for(int i = 0; i <= lastobj; i++) {
		int nframes = objectinfo[i][0];
		int oanimspd = objectinfo[i][3];
		float frame = objectframe[i][0];
		int cframe = objectframe[i][1];
		// objectinfo[i][6] = 0; // ?? out of bounds

		if(nframes > 1) {
			frame = frame + oanimspd / 50 * fpsr;
			while(frame >= nframes)
				frame = frame - nframes;

			cframe = (int)frame; // truncate fractional part
			if(cframe > nframes) cframe = nframes - 1;
			if(cframe < 0) cframe = 0;

			objectframe[i][0] = frame;
			objectframe[i][1] = cframe;
		}
	}
}

void game_updatey()
{
	int yy, ff;

	for(int i = 0; i <= 2400; i++)
		ysort[i] = -1;

	ff = (int)(player.py * 10);
	if(ff < 0) ff = 0; // HACKFIX or ysort[yy] may go out of bounds
	player.ysort = ff;
	ysort[ff] = 0;

	firsty = 2400;
	lasty = 0;

	for(int i = 1; i <= lastnpc; i++) {
		yy = (int)(npcinfo[i].y * 10);

		do {
			if(ysort[yy] == -1 || yy == 2400) break;
			yy = yy + 1;
		} while(1);

		ysort[yy] = i;
		if(yy < firsty) firsty = yy;
		if(yy > lasty) lasty = yy;
	}
}

void game_updmusic()
{
	Mix_Chunk *iplaysound = NULL;

	if(menabled == 1 && opmusic == 1) {

		// if(curmap > 5 && curmap < 42) iplaysound = macademy;
		// if(curmap > 47) iplaysound = mgardens;
		iplaysound = mgardens;
		if(roomlock == 1) iplaysound = mboss;

		if(iplaysound == mboss && pboss) iplaysound = NULL;
		if(iplaysound == mgardens && pgardens) iplaysound = NULL;

		if(iplaysound != NULL) {
			Mix_HaltChannel(musicchannel);

			pboss = 0;
			pgardens = 0;
			ptown = 0;
			pacademy = 0;
			pcitadel = 0;

			if(iplaysound == mboss) pboss = 1;
			if(iplaysound == mgardens) pgardens = 1;

			musicchannel = Mix_PlayChannel(-1, iplaysound, -1);
			Mix_Volume(musicchannel, opmusicvol);
		} else {
			if(!Mix_Playing(musicchannel)) {
				loopseta = loopseta + 1;
				if(loopseta == 4) loopseta = 0;

				if(pgardens == 1) {
					Mix_HaltChannel(musicchannel);
					if(pgardens == 1 && loopseta == 0) musicchannel = Mix_PlayChannel(-1, mgardens, 0);
					if(pgardens == 1 && loopseta == 1) musicchannel = Mix_PlayChannel(-1, mgardens2, 0);
					if(pgardens == 1 && loopseta == 2) musicchannel = Mix_PlayChannel(-1, mgardens3, 0);
					if(pgardens == 1 && loopseta == 3) musicchannel = Mix_PlayChannel(-1, mgardens4, 0);
				}

				Mix_Volume(musicchannel, opmusicvol);
			}
		}
	}
}

void game_updnpcs()
{
	float npx, npy, onpx, onpy;
	unsigned int *temp, dq, bgc;
	float nnxa = 0, nnya = 0, nnxb = 0, nnyb = 0;
	int pass = 0, checkpass;
	float wspd, xdif, ydif;
	int sx, sy;
	int dospell = 0;

	for(int i = 1; i <= lastnpc; i++) {
		if(npcinfo[i].hp > 0) {
			//  is npc walking
			pass = 0;
			if(npcinfo[i].attacking == 0) pass = 1;
			if(npcinfo[i].spriteset == 5) pass = 1;
			if(pass == 1) {
				int moveup = 0;
				int movedown = 0;
				int moveleft = 0;
				int moveright = 0;

				npx = npcinfo[i].x;
				npy = npcinfo[i].y;

				onpx = npx;
				onpy = npy;

				wspd = npcinfo[i].walkspd / 4;

				if(npcinfo[i].spriteset == 10) wspd = wspd * 2;
				int wdir = npcinfo[i].walkdir;

				int mode = npcinfo[i].movementmode;

				xdif = player.px - npx;
				ydif = player.py - npy;

				if(abs(xdif) < 4 * 16 && abs(ydif) < 4 * 16 && mode < 3) mode = 0;
				if(npcinfo[i].hp < npcinfo[i].maxhp * 0.25) mode = 3;

				if(npcinfo[i].pause > ticks) mode = -1;
				if(npcinfo[i].spriteset == 2 && npcinfo[i].castpause > ticks) mode = -1;

				if(mode == 3) {
					mode = 1;
					if(abs(xdif) < 4 * 16 && abs(ydif) < 4 * 16) mode = 3;
				}

				checkpass = 0;

				// npc  AI CODE
				// --------------

				// *** aggressive
				if(mode == 0) {
					wspd = npcinfo[i].walkspd / 2;

					xdif = player.px - npx;
					ydif = player.py - npy;

					if(abs(xdif) > abs(ydif)) {
						if(xdif < 4) wdir = 2;
						if(xdif > -4) wdir = 3;
					} else {
						if(ydif < 4) wdir = 0;
						if(ydif > -4) wdir = 1;
					}

					if(xdif < 4) moveleft = 1;
					if(xdif > -4) moveright = 1;
					if(ydif < 4) moveup = 1;
					if(ydif > -4) movedown = 1;
				}
				// *******************

				// *** defensive
				if(mode == 1) {

					int movingdir = npcinfo[i].movingdir;

					if(npcinfo[i].ticks > ticks + 100000) npcinfo[i].ticks = ticks;

					if(npcinfo[i].ticks < ticks) {
						npcinfo[i].ticks = ticks + 2000;
						movingdir = (int)(RND() * 8);
						npcinfo[i].movingdir = movingdir;
					}

					if(movingdir == 0) {
						wdir = 2; // left
						moveup = 1;
						moveleft = 1;
					} else if(movingdir == 1) {
						wdir = 0; // up
						moveup = 1;
					} else if(movingdir == 2) {
						wdir = 3; // right
						moveup = 1;
						moveright = 1;
					} else if(movingdir == 3) {
						wdir = 3; // right
						moveright = 1;
					} else if(movingdir == 4) {
						wdir = 3; // right
						moveright = 1;
						movedown = 1;
					} else if(movingdir == 5) {
						wdir = 1; // down
						movedown = 1;
					} else if(movingdir == 6) {
						wdir = 2; // left
						movedown = 1;
						moveleft = 1;
					} else if(movingdir == 7) {
						wdir = 2; // left
						moveleft = 1;
					}

					checkpass = 1;
				}
				// *******************

				// *** run away
				if(mode == 3) {
					wspd = npcinfo[i].walkspd / 2;

					xdif = player.px - npx;
					ydif = player.py - npy;

					if(abs(xdif) > abs(ydif)) {
						if(xdif < 4) wdir = 3;
						if(xdif > -4) wdir = 2;
					} else {
						if(ydif < 4) wdir = 1;
						if(ydif > -4) wdir = 0;
					}

					if(xdif < 4) moveright = 1;
					if(xdif > -4) moveleft = 1;
					if(ydif < 4) movedown = 1;
					if(ydif > -4) moveup = 1;
				}
				// *******************

				// -------------- ?? move*** vs movin***
				int movinup = 0;
				int movindown = 0;
				int movinleft = 0;
				int movinright = 0;

				float xp = (npx / 2 + 6);
				float yp = (npy / 2 + 10);

				if(npcinfo[i].spriteset == 10) wspd = wspd * 2;

				float ii = wspd * fpsr;
				if(ii < 1) ii = 1;

				SDL_LockSurface(clipbg);

				if(moveup) {
					sx = xp;
					sy = yp - ii;
					temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
					dq = *temp;
					if(npcinfo[i].spriteset == 11) dq = 0;

					if(dq == 0) movinup = 1;
					if(dq > 0) {
						sx = xp - ii;
						sy = yp - ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0) {
							movinup = 1;
							movinleft = 1;
						}
					}
					if(dq > 0) {
						sx = xp + ii;
						sy = yp - ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0) {
							movinup = 1;
							movinright = 1;
						}
					}
				}

				if(movedown) {
					sx = xp;
					sy = yp + ii;
					temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
					dq = *temp;
					if(npcinfo[i].spriteset == 11) dq = 0;
					if(dq == 0) movindown = 1;
					if(dq > 0) {
						sx = xp - ii;
						sy = yp + ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0) {
							movindown = 1;
							movinleft = 1;
						}
					}
					if(dq > 0) {
						sx = xp + ii;
						sy = yp + ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0 ) {
							movindown = 1;
							movinright = 1;
						}
					}
				}

				if(moveleft) {
					sx = xp - ii;
					sy = yp;
					temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
					dq = *temp;
					if(npcinfo[i].spriteset == 11) dq = 0;
					if(dq == 0) movinleft = 1;
					if(dq > 0) {
						sx = xp - ii;
						sy = yp - ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0) {
							movinleft = 1;
							movinup = 1;
						}
					}
					if(dq > 0) {
						sx = xp - ii;
						sy = yp + ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0) {
							movinleft = 1;
							movindown = 1;
						}
					}
				}

				if(moveright) {
					sx = xp + ii;
					sy = yp;
					temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
					dq = *temp;
					if(npcinfo[i].spriteset == 11) dq = 0;
					if(dq == 0) movinright = 1;
					if(dq > 0) {
						sx = xp + ii;
						sy = yp - ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0) {
							movinright = 1;
							movinup = 1;
						}
					}
					if(dq > 0) {
						sx = xp + ii;
						sy = yp + ii;
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;
						if(npcinfo[i].spriteset == 11) dq = 0;
						if(dq == 0) {
							movinright = 1;
							movindown = 1;
						}
					}
				}

				if(movinup) npy = npy - wspd * fpsr;
				if(movindown) npy = npy + wspd * fpsr;
				if(movinleft) npx = npx - wspd * fpsr;
				if(movinright) npx = npx + wspd * fpsr;

				if(checkpass == 1) {
					pass = 0;
					if(npx >= npcinfo[i].x1 * 16 - 8 && npx <= npcinfo[i].x2 * 16 + 8 && npy >= npcinfo[i].y1 * 16 - 8 && npy <= npcinfo[i].y2 * 16 + 8) pass = 1;
					if(pass == 0) {
						npx = onpx;
						npy = onpy;
						npcinfo[i].ticks = ticks;
					}
				}

				float aspd = wspd;

				if(npcinfo[i].spriteset == 10) aspd = wspd / 2;

				xp = (npx / 2 + 6);
				yp = (npy / 2 + 10);

				sx = xp;
				sy = yp;
				temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
				bgc = *temp;

				SDL_UnlockSurface(clipbg);

				float anpx = npx + 12;
				float anpy = npy + 20;

				int lx = (int)anpx / 16; //(anpx - (anpx % 16)) / 16;
				int ly = (int)anpy / 16; //(anpy - (anpy % 16)) / 16;

				if(triggerloc[lx][ly] > -1) bgc = 1;
				if(npcinfo[i].spriteset == 11) bgc = 0;

				int rst = 0;

				if(npcinfo[i].spriteset == 11) {
					if(npx < 40 || npx > 280 || npy < 36 || npy > 204) rst = 1;
				}

				if(bgc > 0 || rst == 1) {
					npx = onpx;
					npy = onpy;
				}

				npcinfo[i].x = npx;
				npcinfo[i].y = npy;

				npcinfo[i].walkdir = wdir;
				npcinfo[i].moving = 0;

				if(npx != onpx || npy != onpy) npcinfo[i].moving = 1;

				if(npcinfo[i].moving == 1) {
					float frame = npcinfo[i].frame;
					int cframe = npcinfo[i].cframe;

					frame = frame + aspd * fpsr;
					while(frame >= 16)
						frame = frame - 16;

					cframe = (int)(frame);
					if(cframe > 16) cframe = 16 - 1;
					if(cframe < 0) cframe = 0;

					npcinfo[i].frame = frame;
					npcinfo[i].cframe = cframe;
				}

				// spriteset1 specific
				if(npcinfo[i].spriteset == 1 && npcinfo[i].attackattempt < ticks) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
						npx = npcinfo[i].x;
						npy = npcinfo[i].y;

						xdif = player.px - npx;
						ydif = player.py - npy;

						if(abs(xdif) < 20 && abs(ydif) < 20) {
							npcinfo[i].attackattempt = ticks + 100;
							if((int)(RND() * 2) == 0) {
								if(menabled == 1 && opeffects == 1) {
									int snd = Mix_PlayChannel(-1, sfx[sndenemyhit], 0);
									Mix_Volume(snd, opeffectsvol);
								}

								npcinfo[i].attacking = 1;
								npcinfo[i].attackframe = 0;
							}
						}
					}
				}

				// onewing specific
				if(npcinfo[i].spriteset == 2) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
							npx = npcinfo[i].x;
							npy = npcinfo[i].y;

							xdif = player.px - npx;
							ydif = player.py - npy;

							if(abs(xdif) < 24 && abs(ydif) < 24) {
								float dist = sqrt(xdif * xdif + ydif * ydif);

								if((dist) < 24) {
									if(menabled == 1 && opeffects == 1) {
										int snd = Mix_PlayChannel(-1, sfx[sndbite], 0);
										Mix_Volume(snd, opeffectsvol);
									}

									npcinfo[i].attacking = 1;
									npcinfo[i].attackframe = 0;

									npcinfo[i].headtargetx[0] = player.px + 12;
									npcinfo[i].headtargety[0] = player.py - 4;
								}
							}

					}

					dospell = 0;

					if(npcinfo[i].attacking == 0 && npcinfo[i].castpause < ticks) {
						npcinfo[i].swayspd = npcinfo[i].swayspd + npcinfo[i].swayspd / 200 * fpsr;
						if(npcinfo[i].swayspd > 15) {
							dospell = 1;
							npcinfo[i].swayspd = 1;
						}

						// sway code
						npcinfo[i].swayangle = npcinfo[i].swayangle + npcinfo[i].swayspd * fpsr;
						if(npcinfo[i].swayangle >= 360) npcinfo[i].swayangle = npcinfo[i].swayangle - 360;

						npcinfo[i].headtargetx[0] = npcinfo[i].x + (24 - npcinfo[i].swayspd / 2) * sin(3.14159 / 180 * npcinfo[i].swayangle) + 12;
						npcinfo[i].headtargety[0] = npcinfo[i].y - 36 + 16 + 8 * sin(3.14159 * 2 / 180 * npcinfo[i].swayangle);
					}

					if(dospell == 1) {
						npcinfo[i].pause = ticks + 3000;
						npcinfo[i].attacknext = ticks + 4500;
						npcinfo[i].castpause = ticks + 4500;

						game_castspell(3, npcinfo[i].x, npcinfo[i].y, npcinfo[i].x, npcinfo[i].y, i);

						npcinfo[i].headtargetx[0] = npcinfo[i].x;
						npcinfo[i].headtargety[0] = npcinfo[i].y - 36 + 16;
					}

					// targethead code
					xdif = npcinfo[i].bodysection[7].x - npcinfo[i].headtargetx[0];
					ydif = npcinfo[i].bodysection[7].y - npcinfo[i].headtargety[0];


					npcinfo[i].bodysection[7].x = npcinfo[i].bodysection[7].x  - xdif * 0.4 * fpsr;
					npcinfo[i].bodysection[7].y = npcinfo[i].bodysection[7].y  - ydif * 0.4 * fpsr;


					npcinfo[i].bodysection[0].x = npcinfo[i].x + 12;
					npcinfo[i].bodysection[0].y = npcinfo[i].y + 12;

					for(int f = 6; f >= 1; f--) {
						xdif = npcinfo[i].bodysection[f + 1].x - npcinfo[i].bodysection[f - 1].x;
						ydif = npcinfo[i].bodysection[f + 1].y - npcinfo[i].bodysection[f - 1].y;

						float tx = npcinfo[i].bodysection[f - 1].x + xdif / 2;
						float ty = npcinfo[i].bodysection[f - 1].y + ydif / 2;

						npcinfo[i].bodysection[f].x = npcinfo[i].bodysection[f].x - (npcinfo[i].bodysection[f].x - tx) / 3;
						npcinfo[i].bodysection[f].y = npcinfo[i].bodysection[f].y - (npcinfo[i].bodysection[f].y - ty) / 3;
					}
				}

				// boss1 specific and blackknight
				if(npcinfo[i].spriteset == 3 || npcinfo[i].spriteset == 4) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
						npcinfo[i].attacking = 1;
						npcinfo[i].attackframe = 0;

						game_castspell(1, npcinfo[i].x, npcinfo[i].y, player.px, player.py, i);
					}

					if(npcinfo[i].castpause < ticks) {
						game_castspell(6, npcinfo[i].x, npcinfo[i].y, player.px, player.py, i);
						npcinfo[i].castpause = ticks + 12000;
					}
				}


				// firehydra specific
				if(npcinfo[i].spriteset == 5) {
					npcinfo[i].swayspd = 4;

					// sway code
					npcinfo[i].swayangle = npcinfo[i].swayangle + npcinfo[i].swayspd * fpsr;
					if(npcinfo[i].swayangle >= 360) npcinfo[i].swayangle = npcinfo[i].swayangle - 360;

					for(int ff = 0; ff <= 2; ff++) {
						if(npcinfo[i].hp > 10 * ff * 20) {
							if(npcinfo[i].pause < ticks && npcinfo[i].attacking2[ff] == 0 && npcinfo[i].attacknext2[ff] < ticks) {
								npx = npcinfo[i].x;
								npy = npcinfo[i].y;

								xdif = player.px - npx;
								ydif = player.py - npy;

								if(abs(xdif) < 48 && abs(ydif) < 48) {
									float dist = sqrt(xdif * xdif + ydif * ydif);

									if((dist) < 36) {
										if(menabled == 1 && opeffects == 1) {
											int snd = Mix_PlayChannel(-1, sfx[sndbite], 0);
											Mix_Volume(snd, opeffectsvol);
										}

										npcinfo[i].attacking = 1;
										npcinfo[i].attacking2[ff] = 1;
										npcinfo[i].attackframe2[ff] = 0;

										npcinfo[i].headtargetx[ff] = player.px + 12;
										npcinfo[i].headtargety[ff] = player.py - 4;

										npcinfo[i].swayangle = 0;
									}
								}

							}

							if(npcinfo[i].attacking2[ff] == 0) {
								npcinfo[i].headtargetx[ff] = npcinfo[i].x + 38 * sin(3.14159 / 180 * (npcinfo[i].swayangle + 120 * ff)) + 12;
								npcinfo[i].headtargety[ff] = npcinfo[i].y - 46 + 16 + 16 * sin(3.14159 * 2 / 180 * (npcinfo[i].swayangle + 120 * ff));
							}

							// targethead code
							xdif = npcinfo[i].bodysection[10 * ff + 9].x - npcinfo[i].headtargetx[ff];
							ydif = npcinfo[i].bodysection[10 * ff + 9].y - npcinfo[i].headtargety[ff];

							npcinfo[i].bodysection[10 * ff + 9].x = npcinfo[i].bodysection[10 * ff + 9].x  - xdif * 0.4 * fpsr;
							npcinfo[i].bodysection[10 * ff + 9].y = npcinfo[i].bodysection[10 * ff + 9].y  - ydif * 0.4 * fpsr;

							npcinfo[i].bodysection[10 * ff].x = npcinfo[i].x + 12 + 8 * cos(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360));
							npcinfo[i].bodysection[10 * ff].y = npcinfo[i].y + 12 + 8 * sin(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360));

							for(int f = 8; f >= 1; f--) {
								xdif = npcinfo[i].bodysection[ff * 10 + f + 1].x - npcinfo[i].bodysection[ff * 10 + f - 1].x;
								ydif = npcinfo[i].bodysection[ff * 10 + f + 1].y - npcinfo[i].bodysection[ff * 10 + f - 1].y;

								float tx = npcinfo[i].bodysection[ff * 10 + f - 1].x + xdif / 2;
								float ty = npcinfo[i].bodysection[ff * 10 + f - 1].y + ydif / 2;

								npcinfo[i].bodysection[ff * 10 + f].x = npcinfo[i].bodysection[ff * 10 + f].x - (npcinfo[i].bodysection[ff * 10 + f].x - tx) / 3;
								npcinfo[i].bodysection[ff * 10 + f].y = npcinfo[i].bodysection[ff * 10 + f].y - (npcinfo[i].bodysection[ff * 10 + f].y - ty) / 3;
							}
						}
					}
				}

				// spriteset6 specific
				if(npcinfo[i].spriteset == 6 && npcinfo[i].attackattempt < ticks) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
						npx = npcinfo[i].x;
						npy = npcinfo[i].y;

						xdif = player.px - npx;
						ydif = player.py - npy;

						pass = 0;
						if(abs(xdif) < 48 && abs(ydif) < 6) pass = 1;
						if(abs(ydif) < 48 && abs(xdif) < 6) pass = 2;

						if(pass > 0) {
							npcinfo[i].attackattempt = ticks + 100;
							if((int)(RND() * 2) == 0) {
								npcinfo[i].attacking = 1;
								npcinfo[i].attackframe = 0;

								if(pass == 1 && xdif < 0) {
									nnxa = npx - 8;
									nnya = npy + 4;
									nnxb = npx - 48 - 8;
									nnyb = npy + 4;
								} else if(pass == 1 && xdif > 0) {
									nnxa = npx + 16;
									nnya = npy + 4;
									nnxb = npx + 16 + 48;
									nnyb = npy + 4;
								} else if(pass == 2 && ydif < 0) {
									nnya = npy;
									nnxa = npx + 4;
									nnyb = npy - 48;
									nnxb = npx + 4;
								} else if(pass == 2 && ydif > 0) {
									nnya = npy + 20;
									nnxa = npx + 4;
									nnyb = npy + 20 + 48;
									nnxb = npx + 4;
								}

								game_castspell(7, nnxa, nnya, nnxb, nnyb, i);
							}
						}
					}
				}

				// wizard1 specific
				if(npcinfo[i].spriteset == 7) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
						npcinfo[i].attacking = 1;
						npcinfo[i].attackframe = 0;

						game_castspell(9, npcinfo[i].x, npcinfo[i].y, player.px, player.py, i);
					}

					if(npcinfo[i].castpause < ticks) {
						// game_castspell 6, npcinfo[i].x, npcinfo[i].y, player.px, player.py, i
						// npcinfo[i].castpause = ticks + 12000
					}

				}

				// spriteset6 specific
				if(npcinfo[i].spriteset == 8 && npcinfo[i].attackattempt < ticks) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
						npx = npcinfo[i].x;
						npy = npcinfo[i].y;

						xdif = player.px - npx;
						ydif = player.py - npy;

						pass = 0;
						if(abs(xdif) < 56 && abs(ydif) < 6) pass = 1;
						if(abs(ydif) < 56 && abs(xdif) < 6) pass = 2;

						if(pass > 0) {
							npcinfo[i].attackattempt = ticks + 100;
							if((int)(RND() * 2) == 0) {
								npcinfo[i].attacking = 1;
								npcinfo[i].attackframe = 0;

								if(pass == 1 && xdif < 0) {
									nnxa = npx - 8;
									nnya = npy + 4;
									nnxb = npx - 56 - 8;
									nnyb = npy + 4;
									npcinfo[i].walkdir = 2;
								} else if(pass == 1 && xdif > 0) {
									nnxa = npx + 16;
									nnya = npy + 4;
									nnxb = npx + 16 + 56;
									nnyb = npy + 4;
									npcinfo[i].walkdir = 3;
								} else if(pass == 2 && ydif < 0) {
									nnya = npy;
									nnxa = npx + 4;
									nnyb = npy - 56;
									nnxb = npx + 4;
									npcinfo[i].walkdir = 0;
								} else if(pass == 2 && ydif > 0) {
									nnya = npy + 20;
									nnxa = npx + 4;
									nnyb = npy + 20 + 56;
									nnxb = npx + 4;
									npcinfo[i].walkdir = 1;
								}

								game_castspell(7, nnxa, nnya, nnxb, nnyb, i);
							}
						}
					}
				}

				// twowing specific
				if(npcinfo[i].spriteset == 9) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
							npx = npcinfo[i].bodysection[7].x;
							npy = npcinfo[i].bodysection[7].y;

							xdif = player.px - npx;
							ydif = player.py - npy;

							if(abs(xdif) < 24 && abs(ydif) < 24) {
								float dist = sqrt(xdif * xdif + ydif * ydif);

								if((dist) < 24) {
									if(menabled == 1 && opeffects == 1) {
										int snd = Mix_PlayChannel(-1, sfx[sndbite], 0);
										Mix_Volume(snd, opeffectsvol);
									}

									npcinfo[i].attacking = 1;
									npcinfo[i].attackframe = 0;

									npcinfo[i].headtargetx[0] = player.px + 12;
									npcinfo[i].headtargety[0] = player.py - 4;
								}
							}

					}

					if(npcinfo[i].attacking == 0 && npcinfo[i].castpause < ticks) {
						npcinfo[i].swayspd = 4;

						// sway code
						npcinfo[i].swayangle = npcinfo[i].swayangle + npcinfo[i].swayspd * fpsr;
						if(npcinfo[i].swayangle >= 360) npcinfo[i].swayangle = npcinfo[i].swayangle - 360;

						npcinfo[i].headtargetx[0] = npcinfo[i].x + (24 - npcinfo[i].swayspd / 2) * sin(3.14159 / 180 * npcinfo[i].swayangle) + 12;
						npcinfo[i].headtargety[0] = npcinfo[i].y - 36 + 16 + 8 * sin(3.14159 * 2 / 180 * npcinfo[i].swayangle);
					}

					if(dospell == 1) {
						npcinfo[i].pause = ticks + 3000;
						npcinfo[i].attacknext = ticks + 5000;
						npcinfo[i].castpause = ticks + 3000;

						game_castspell(3, npcinfo[i].x, npcinfo[i].y, npcinfo[i].x, npcinfo[i].y, i);

						npcinfo[i].headtargetx[0] = npcinfo[i].x;
						npcinfo[i].headtargety[0] = npcinfo[i].y - 36 + 16;
					}

					// targethead code
					xdif = npcinfo[i].bodysection[7].x - npcinfo[i].headtargetx[0];
					ydif = npcinfo[i].bodysection[7].y - npcinfo[i].headtargety[0];


					npcinfo[i].bodysection[7].x = npcinfo[i].bodysection[7].x  - xdif * 0.4 * fpsr;
					npcinfo[i].bodysection[7].y = npcinfo[i].bodysection[7].y  - ydif * 0.4 * fpsr;

					npcinfo[i].bodysection[0].x = npcinfo[i].x + 12;
					npcinfo[i].bodysection[0].y = npcinfo[i].y + 12;

					for(int f = 6; f >= 1; f--) {
						xdif = npcinfo[i].bodysection[f + 1].x - npcinfo[i].bodysection[f - 1].x;
						ydif = npcinfo[i].bodysection[f + 1].y - npcinfo[i].bodysection[f - 1].y;

						float tx = npcinfo[i].bodysection[f - 1].x + xdif / 2;
						float ty = npcinfo[i].bodysection[f - 1].y + ydif / 2;

						npcinfo[i].bodysection[f].x = npcinfo[i].bodysection[f].x - (npcinfo[i].bodysection[f].x - tx) / 3;
						npcinfo[i].bodysection[f].y = npcinfo[i].bodysection[f].y - (npcinfo[i].bodysection[f].y - ty) / 3;
					}

				}

				// dragon2 specific
				if(npcinfo[i].spriteset == 10 && npcinfo[i].attackattempt < ticks) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
						npx = npcinfo[i].x;
						npy = npcinfo[i].y;

						xdif = player.px - npx;
						ydif = player.py - npy;

						if(abs(xdif) < 32 && abs(ydif) < 32) {
							npcinfo[i].attackattempt = ticks + 100;
							if((int)(RND() * 2) == 0) {
								if(menabled == 1 && opeffects == 1) {
									int snd = Mix_PlayChannel(-1, sfx[sndenemyhit], 0);
									Mix_Volume(snd, opeffectsvol);
								}

								npcinfo[i].attacking = 1;
								npcinfo[i].attackframe = 0;
							}
						}
					}
				}


				// endboss specific
				if(npcinfo[i].spriteset == 11 && npcinfo[i].attackattempt < ticks) {
					if(npcinfo[i].attacknext < ticks && npcinfo[i].pause < ticks && npcinfo[i].attacking == 0) {
						npx = npcinfo[i].x;
						npy = npcinfo[i].y;

						xdif = player.px - npx;
						ydif = player.py - npy;

						if(abs(xdif) < 38 && abs(ydif) < 38) {
							npcinfo[i].attackattempt = ticks + 100;
							if((int)(RND() * 2) == 0) {
								if(menabled == 1 && opeffects == 1) {
									int snd = Mix_PlayChannel(-1, sfx[sndice], 0);
									Mix_Volume(snd, opeffectsvol);
								}
								npcinfo[i].attacking = 1;
								npcinfo[i].attackframe = 0;
							}
						}
					}
				}
			}


			int dist; float damage;

			npx = npcinfo[i].x;
			npy = npcinfo[i].y;

			int xp = (npx / 2 + 6);
			int yp = (npy / 2 + 10);

			rcSrc.x = xp - 1;
			rcSrc.y = yp - 1;
			rcSrc.w = 3;
			rcSrc.h = 3;

			if(npcinfo[i].pause < ticks) SDL_FillRect(clipbg, &rcSrc, i);


			pass = 0;
			if(npcinfo[i].attacking == 1) pass = 1;
			if(npcinfo[i].spriteset == 5) {
				if(npcinfo[i].attacking2[0] == 1) pass = 1;
				if(npcinfo[i].attacking2[1] == 1) pass = 1;
				if(npcinfo[i].attacking2[2] == 1) pass = 1;
			}

			if(pass == 1) {
				// spriteset1 specific
				if(npcinfo[i].spriteset == 1) {
					npcinfo[i].attackframe = npcinfo[i].attackframe + npcinfo[i].attackspd * fpsr;
					if(npcinfo[i].attackframe >= 16) {
						npcinfo[i].attackframe = 0;
						npcinfo[i].attacking = 0;
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
					}

					npcinfo[i].cattackframe = (int)(npcinfo[i].attackframe);

					npx = npcinfo[i].x;
					npy = npcinfo[i].y;

					xdif = player.px - npx;
					ydif = player.py - npy;

					dist = 10;

					if(abs(xdif) < dist && abs(ydif) < dist && player.pause < ticks) {
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
						// npcinfo[i].attackframe = 0;
						// npcinfo[i].attacking = 0;

						damage = (float)npcinfo[i].attackdamage * (0.5 + RND() * 1.0);

						if(player.hp > 0) game_damageplayer(damage);
					}
				}

				if(npcinfo[i].spriteset == 2) {
					// targethead code
					xdif = npcinfo[i].bodysection[7].x - npcinfo[i].headtargetx[0];
					ydif = npcinfo[i].bodysection[7].y - npcinfo[i].headtargety[0];

					npcinfo[i].bodysection[7].x = npcinfo[i].bodysection[7].x  - xdif * 0.4 * fpsr;
					npcinfo[i].bodysection[7].y = npcinfo[i].bodysection[7].y  - ydif * 0.4 * fpsr;

					npcinfo[i].bodysection[0].x = npcinfo[i].x + 12;
					npcinfo[i].bodysection[0].y = npcinfo[i].y + 12;

					for(int f = 6; f >= 1; f--) {
						xdif = npcinfo[i].bodysection[f + 1].x - npcinfo[i].bodysection[f - 1].x;
						ydif = npcinfo[i].bodysection[f + 1].y - npcinfo[i].bodysection[f - 1].y;

						float tx = npcinfo[i].bodysection[f - 1].x + xdif / 2;
						float ty = npcinfo[i].bodysection[f - 1].y + ydif / 2;

						npcinfo[i].bodysection[f].x = npcinfo[i].bodysection[f].x - (npcinfo[i].bodysection[f].x - tx);
						npcinfo[i].bodysection[f].y = npcinfo[i].bodysection[f].y - (npcinfo[i].bodysection[f].y - ty);
					}

					npcinfo[i].attackframe = npcinfo[i].attackframe + npcinfo[i].attackspd * fpsr;
					if(npcinfo[i].attackframe >= 16) {
						npcinfo[i].attackframe = 0;
						npcinfo[i].attacking = 0;
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
					}

					npcinfo[i].cattackframe = (int)(npcinfo[i].attackframe);

					npx = npcinfo[i].bodysection[7].x;
					npy = (npcinfo[i].bodysection[7].y + 16);

					xdif = (player.px + 12) - npx;
					ydif = (player.py + 12) - npy;

					dist = 8;

					if(abs(xdif) < dist && abs(ydif) < dist && player.pause < ticks) {
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
						// npcinfo[i].attackframe = 0
						// npcinfo[i].attacking = 0
						damage = (float)npcinfo[i].attackdamage * (1.0 + (RND() * 0.5));
						if(player.hp > 0) game_damageplayer(damage);
					}
				}


				// firehydra
				if(npcinfo[i].spriteset == 5) {
					for(int ff = 0; ff <= 2; ff++) {
						if(npcinfo[i].attacking2[ff] == 1) {
							xdif = npcinfo[i].bodysection[10 * ff + 9].x - npcinfo[i].headtargetx[ff];
							ydif = npcinfo[i].bodysection[10 * ff + 9].y - npcinfo[i].headtargety[ff];

							npcinfo[i].bodysection[10 * ff + 9].x = npcinfo[i].bodysection[10 * ff + 9].x  - xdif * .2 * fpsr;
							npcinfo[i].bodysection[10 * ff + 9].y = npcinfo[i].bodysection[10 * ff + 9].y  - ydif * .2 * fpsr;

							npcinfo[i].bodysection[10 * ff].x = npcinfo[i].x + 12 + 8 * cos(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360));
							npcinfo[i].bodysection[10 * ff].y = npcinfo[i].y + 12 + 8 * sin(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360));

							for(int f = 8; f >= 1; f--) {
								xdif = npcinfo[i].bodysection[ff * 10 + f + 1].x - npcinfo[i].bodysection[ff * 10 + f - 1].x;
								ydif = npcinfo[i].bodysection[ff * 10 + f + 1].y - npcinfo[i].bodysection[ff * 10 + f - 1].y;

								float tx = npcinfo[i].bodysection[ff * 10 + f - 1].x + xdif / 2;
								float ty = npcinfo[i].bodysection[ff * 10 + f - 1].y + ydif / 2;

								npcinfo[i].bodysection[ff * 10 + f].x = npcinfo[i].bodysection[ff * 10 + f].x - (npcinfo[i].bodysection[ff * 10 + f].x - tx) / 3;
								npcinfo[i].bodysection[ff * 10 + f].y = npcinfo[i].bodysection[ff * 10 + f].y - (npcinfo[i].bodysection[ff * 10 + f].y - ty) / 3;
							}

							npcinfo[i].attackframe2[ff] = npcinfo[i].attackframe2[ff] + npcinfo[i].attackspd * fpsr;
							if(npcinfo[i].attackframe2[ff] >= 16) {
								npcinfo[i].attackframe2[ff] = 0;
								npcinfo[i].attacking2[ff] = 0;
								npcinfo[i].attacknext2[ff] = ticks + npcinfo[i].attackdelay;
							}

							npcinfo[i].cattackframe = (int)(npcinfo[i].attackframe);

							npx = npcinfo[i].bodysection[10 * ff + 9].x;
							npy = (npcinfo[i].bodysection[10 * ff + 9].y + 16);

							xdif = (player.px + 12) - npx;
							ydif = (player.py + 12) - npy;

							dist = 8;

							if(abs(xdif) < dist && abs(ydif) < dist && player.pause < ticks) {
								npcinfo[i].attacknext2[ff] = ticks + npcinfo[i].attackdelay;
								// npcinfo[i].attackframe2(ff) = 0
								// npcinfo[i].attacking2(ff) = 0
								damage = (float)npcinfo[i].attackdamage * (1.0 + RND() * 0.5);
								if(player.hp > 0) game_damageplayer(damage);
							}
						}
					}

				}

				// twowing specific
				if(npcinfo[i].spriteset == 9) {
					// targethead code
					xdif = npcinfo[i].bodysection[7].x - npcinfo[i].headtargetx[0];
					ydif = npcinfo[i].bodysection[7].y - npcinfo[i].headtargety[0];

					npcinfo[i].bodysection[7].x = npcinfo[i].bodysection[7].x - xdif * 0.4 * fpsr;
					npcinfo[i].bodysection[7].y = npcinfo[i].bodysection[7].y - ydif * 0.4 * fpsr;

					npcinfo[i].bodysection[0].x = npcinfo[i].x + 12;
					npcinfo[i].bodysection[0].y = npcinfo[i].y + 12;

					for(int f = 6; f >= 1; f--) {
						xdif = npcinfo[i].bodysection[f + 1].x - npcinfo[i].bodysection[f - 1].x;
						ydif = npcinfo[i].bodysection[f + 1].y - npcinfo[i].bodysection[f - 1].y;

						float tx = npcinfo[i].bodysection[f - 1].x + xdif / 2;
						float ty = npcinfo[i].bodysection[f - 1].y + ydif / 2;

						npcinfo[i].bodysection[f].x = npcinfo[i].bodysection[f].x - (npcinfo[i].bodysection[f].x - tx);
						npcinfo[i].bodysection[f].y = npcinfo[i].bodysection[f].y - (npcinfo[i].bodysection[f].y - ty);
					}

					npcinfo[i].attackframe = npcinfo[i].attackframe + npcinfo[i].attackspd * fpsr;
					if(npcinfo[i].attackframe >= 16) {
						npcinfo[i].attackframe = 0;
						npcinfo[i].attacking = 0;
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
					}

					npcinfo[i].cattackframe = (int)(npcinfo[i].attackframe);

					npx = npcinfo[i].bodysection[7].x;
					npy = (npcinfo[i].bodysection[7].y + 16);

					xdif = (player.px + 12) - npx;
					ydif = (player.py + 12) - npy;

					dist = 8;

					if(abs(xdif) < dist && abs(ydif) < dist && player.pause < ticks) {
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
						// npcinfo[i].attackframe = 0
						// npcinfo[i].attacking = 0
						damage = (float)npcinfo[i].attackdamage * (1.0 + RND() * 0.5);
						if(player.hp > 0) game_damageplayer(damage);
					}
				}

				// dragon 2 specific
				if(npcinfo[i].spriteset == 10) {

					npcinfo[i].attackframe = npcinfo[i].attackframe + npcinfo[i].attackspd * fpsr;
					if(npcinfo[i].attackframe >= 16) {
						npcinfo[i].attackframe = 0;
						npcinfo[i].attacking = 0;
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
					}

					npcinfo[i].cattackframe = (int)(npcinfo[i].attackframe);

					npx = npcinfo[i].x;
					npy = npcinfo[i].y;

					xdif = player.px - npx;
					ydif = player.py - npy;

					dist = 16 + npcinfo[i].attackframe;

					if(abs(xdif) < dist && abs(ydif) < dist && player.pause < ticks) {
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
						// npcinfo[i].attackframe = 0
						// npcinfo[i].attacking = 0
						damage = (float)npcinfo[i].attackdamage * (0.5 + RND() * 1.0);
						if(player.hp > 0) game_damageplayer(damage);
					}
				}

				// endboss specific
				if(npcinfo[i].spriteset == 11) {
					npcinfo[i].attackframe = npcinfo[i].attackframe + npcinfo[i].attackspd * fpsr;
					if(npcinfo[i].attackframe >= 16) {
						npcinfo[i].attackframe = 0;
						npcinfo[i].attacking = 0;
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
					}

					npcinfo[i].cattackframe = (int)(npcinfo[i].attackframe);

					npx = npcinfo[i].x;
					npy = npcinfo[i].y;

					xdif = player.px - npx;
					ydif = player.py - npy;

					dist = 36;

					if(abs(xdif) < dist && abs(ydif) < dist && player.pause < ticks) {
						npcinfo[i].attacknext = ticks + npcinfo[i].attackdelay;
						// npcinfo[i].attackframe = 0
						// npcinfo[i].attacking = 0
						damage = (float)npcinfo[i].attackdamage * (0.5 + RND() * 1.0);
						if(player.hp > 0) game_damageplayer(damage);
					}
				}
			}
			// -------end fight code
		}
	}
}

void game_updspells()
{
	int foundel[5];
	float npx, npy;
	long cl1, cl2, cl3;
	int ll[4][2];

	float xloc = 0, yloc = 0, xst, yst, xdif, ydif;

	for(int i = 0; i <= MAXSPELL; i++) {
		if(spellinfo[i].frame > 0) {
			int spellnum = spellinfo[i].spellnum;

			// water
			if(spellnum == 0 && forcepause == 0) {
				float fr = (32 - spellinfo[i].frame);

				ll[0][0] = -2;
				ll[0][1] = -3;
				ll[1][0] = 2;
				ll[1][1] = -3;
				ll[2][0] = -4;
				ll[2][1] = -2;
				ll[3][0] = 4;
				ll[3][1] = -2;

				for(int f = 0; f <= 3; f++) {
					if(fr > f * 4 && fr < f * 4 + 16) {
						float alf = 255;

						if(fr < f * 4 + 8) {
							int fi = (int)((fr - f * 4) * 3) % 4;
							rcSrc.x = 32 + fi * 16;
							rcSrc.y = 80;
							rcSrc.w = 16;
							rcSrc.h = 16;

							xloc = spellinfo[i].enemyx + 12 + ll[f][0] * 16;
							yloc = spellinfo[i].enemyy + 16 + ll[f][1] * 16;

							rcDest.x = xloc;
							rcDest.y = yloc;

							alf = 255 * ((fr - f * 4) / 8);
						}

						if(fr >= f * 4 + 8) {
							int fi = 0; // ??

							if(f == 0 || f == 2) fi = 0;
							if(f == 1 || f == 3) fi = 1;
							rcSrc.x = 32 + fi * 16;
							rcSrc.y = 80;
							rcSrc.w = 16;
							rcSrc.h = 16;

							xst = spellinfo[i].enemyx + 12 + ll[f][0] * 16;
							yst = spellinfo[i].enemyy + 16 + ll[f][1] * 16;

							float xi = (spellinfo[i].enemyx - xst) * 2 / 8;
							float yi = (spellinfo[i].enemyy - yst) * 2 / 8;

							float fl = (fr - f * 4 - 8) / 2;
							xloc = xst + xi * fl * fl;
							yloc = yst + yi * fl * fl;

							rcDest.x = xloc;
							rcDest.y = yloc;

							alf = 255;
						}

						if(xloc > -16 && xloc < 304 && yloc > -16 && yloc < 224) {
							SDL_SetAlpha(spellimg, SDL_SRCALPHA, alf);
							SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
							SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);

							if(spellinfo[i].damagewho == 0) {
								for(int e = 1; e <= lastnpc; e++) {

									xdif = (xloc + 16) - (npcinfo[e].x + 12);
									ydif = (yloc + 16) - (npcinfo[e].y + 12);

									if((abs(xdif) < 16 && abs(ydif) < 16)) {
										float damage = (float)player.spelldamage * (1.0 + RND() * 0.5) * (float)spellinfo[i].strength;

										if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) {
											game_damagenpc(e, damage, 1);
											if(menabled == 1 && opeffects == 1) {
												int snd = Mix_PlayChannel(-1, sfx[sndice], 0);
												Mix_Volume(snd, opeffectsvol);
											}
										}
									}
								}
							}

							// check for post damage
							if(nposts > 0) {
								for(int e = 0; e <= nposts - 1; e++) {
									xdif = (xloc + 16) - (postinfo[e][0] + 8);
									ydif = (yloc + 16) - (postinfo[e][1] + 8);

									if((abs(xdif) < 16 && abs(ydif) < 16)) {
										objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
										objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

										rcSrc.x = postinfo[e][0] / 2;
										rcSrc.y = postinfo[e][1] / 2;
										rcSrc.w = 8;
										rcSrc.h = 8;

										SDL_FillRect(clipbg2, &rcSrc, 0);

										game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);

										if(menabled == 1 && opeffects == 1) {
											int snd = Mix_PlayChannel(-1, sfx[sndice], 0);
											Mix_Volume(snd, opeffectsvol);
										}
									}
								}
							}
						}
					}
				}
			}

			// metal
			if(spellnum == 1 && forcepause == 0) {
				float npx, npy, xdif, ydif;
				int npc;
				int fr = (int)((32 - spellinfo[i].frame)*4) % 3;

				rcSrc.x = fr * 48;
				rcSrc.y = 0;
				rcSrc.w = 48;
				rcSrc.h = 48;

				float c1 = (32 - spellinfo[i].frame) / 16;
				//float c = sin(3.14159 * 2 * c1);

				float halfx = (spellinfo[i].homex-12) + ((spellinfo[i].enemyx-12) - (spellinfo[i].homex-12)) / 2;
				float halfy = (spellinfo[i].homey-12) + ((spellinfo[i].enemyy-12) - (spellinfo[i].homey-12)) / 2;

				float wdth = (halfx - spellinfo[i].homex) * 1.2;
				float hight = (halfy - spellinfo[i].homey) * 1.2;

				xloc = halfx + wdth * cos(3.14159 + 3.14159 * 2 * c1);
				yloc = halfy + hight * sin(3.14159 + 3.14159 * 2 * c1);

				rcDest.x = xloc;
				rcDest.y = yloc;

				SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);

				spellinfo[i].frame = spellinfo[i].frame - 0.2 * fpsr;
				if(spellinfo[i].frame < 0) spellinfo[i].frame = 0;

				if(spellinfo[i].damagewho == 0) {
					for(int e = 1; e <= lastnpc; e++) {
						xdif = (xloc + 24) - (npcinfo[e].x + 12);
						ydif = (yloc + 24) - (npcinfo[e].y + 12);

						if((abs(xdif) < 24 && abs(ydif) < 24)) {
							float damage = (float)player.spelldamage * (1.0 + RND() * 0.5) * (float)spellinfo[i].strength;

							if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) {
								game_damagenpc(e, damage, 1);
								if(menabled == 1 && opeffects == 1) {
									int snd = Mix_PlayChannel(-1, sfx[sndmetalhit], 0);
									Mix_Volume(snd, opeffectsvol);
								}
							}
						}
					}
				}

				if(spellinfo[i].damagewho == 1) {

					// --------- boss 1 specific
					if(spellinfo[i].frame == 0 && npcinfo[spellinfo[i].npc].spriteset == 3) {
						npc = spellinfo[i].npc;
						npcinfo[npc].attackframe = 0;
						npcinfo[npc].attacking = 0;

						npcinfo[npc].pause = ticks + 1000;
						npcinfo[npc].attacknext = ticks + 4000;
					}
					// ---------------

					// --------- blackknight specific
					if(spellinfo[i].frame == 0 && npcinfo[spellinfo[i].npc].spriteset == 4) {
						npc = spellinfo[i].npc;
						npcinfo[npc].attackframe = 0;
						npcinfo[npc].attacking = 0;

						npcinfo[npc].pause = ticks + 1000;
						npcinfo[npc].attacknext = ticks + 3500;
					}
					// ---------------

					xdif = (xloc + 24) - (player.px + 12);
					ydif = (yloc + 24) - (player.py + 12);

					if((abs(xdif) < 24 && abs(ydif) < 24) && player.pause < ticks) {
						npx = player.px;
						npy = player.py;

						float damage = (float)npcinfo[spellinfo[i].npc].spelldamage * (1.0 + RND() * 0.5);

						if(player.hp > 0) {
							game_damageplayer(damage);
							if(menabled == 1 && opeffects == 1) {
								int snd = Mix_PlayChannel(-1, sfx[sndmetalhit], 0);
								Mix_Volume(snd, opeffectsvol);
							}
						}
					}
				}


				// check for(int post damage
				if(nposts > 0) {
					for(int e = 0; e <= nposts - 1; e++) {
						xdif = (xloc + 24) - (postinfo[e][0] + 8);
						ydif = (yloc + 24) - (postinfo[e][1] + 8);

						if((abs(xdif) < 24 && abs(ydif) < 24)) {
							objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
							objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

							rcSrc.x = postinfo[e][0] / 2;
							rcSrc.y = postinfo[e][1] / 2;
							rcSrc.w = 8;
							rcSrc.h = 8;

							SDL_FillRect(clipbg2, &rcSrc, 0);

							game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);

							if(menabled == 1 && opeffects == 1) {
								int snd = Mix_PlayChannel(-1, sfx[sndmetalhit], 0);
								Mix_Volume(snd, opeffectsvol);
							}
						}
					}
				}
			}

			// earth
			if(spellnum == 2 && forcepause == 0) {
				float hght = 240 - spellinfo[i].enemyy;

				for(int f = 8; f >= 0; f--) {

					float fr = (32 - spellinfo[i].frame);

					if(fr > f && fr < f + 16) {
						rcSrc.x = 32 * spellinfo[i].rockimg[f];
						rcSrc.y = 48;
						rcSrc.w = 32;
						rcSrc.h = 32;

						int scatter = 0;
						if(fr < 8 + f) {
							xloc = spellinfo[i].enemyx - 4;
							yloc = spellinfo[i].enemyy * (1 - cos(3.14159 / 2 * (fr - f) / 8)); // ^ 2;
							yloc *= yloc;
						} else {
							scatter = 1;
							xloc = spellinfo[i].enemyx - 4 - spellinfo[i].rockdeflect[f] * sin(3.14159 / 2* ((fr - f) - 8) / 8);
							yloc = spellinfo[i].enemyy + hght * (1 - cos(3.14159 / 2 * ((fr - f) - 8) / 8));
						}

						rcDest.x = xloc;
						rcDest.y = yloc;

						if(xloc > -16 && xloc < 304 && yloc > -16 && yloc < 224) {
							SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);

							if(scatter == 1) {
								if(spellinfo[i].damagewho == 0) {
									for(int e = 1; e <= lastnpc; e++) {
										xdif = (xloc + 16) - (npcinfo[e].x + 12);
										ydif = (yloc + 16) - (npcinfo[e].y + 12);

										if((abs(xdif) < 16 && abs(ydif) < 16)) {
											float damage = (float)player.spelldamage * (1.0 + RND() * 0.5) * (float)spellinfo[i].strength;

											if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) {
												game_damagenpc(e, damage, 1);
												if(menabled == 1 && opeffects == 1) {
													int snd = Mix_PlayChannel(-1, sfx[sndrocks], 0);
													Mix_Volume(snd, opeffectsvol);
												}
											}
										}
									}
								}


								// check for(int post damage
								if(nposts > 0) {
									for(int e = 0; e <= nposts - 1; e++) {
										xdif = (xloc + 16) - (postinfo[e][0] + 8);
										ydif = (yloc + 16) - (postinfo[e][1] + 8);

										if((abs(xdif) < 16 && abs(ydif) < 16)) {
											objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
											objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

											rcSrc.x = postinfo[e][0] / 2;
											rcSrc.y = postinfo[e][1] / 2;
											rcSrc.w = 8;
											rcSrc.h = 8;

											SDL_FillRect(clipbg2, &rcSrc, 0);

											game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);

											if(menabled == 1 && opeffects == 1) {
												int snd = Mix_PlayChannel(-1, sfx[sndrocks], 0);
												Mix_Volume(snd, opeffectsvol);
											}
										}
									}
								}
							}
						}
					}
				}

				spellinfo[i].frame = spellinfo[i].frame - 0.2 * fpsr;
				if(spellinfo[i].frame < 0) spellinfo[i].frame = 0;
			}

			// crystal
			if(spellnum == 5) {

				float fra = (32 - spellinfo[i].frame);
				int fr = (int)((spellinfo[i].frame) * 2) % 8;

				rcSrc.x = fr * 32;
				rcSrc.y = 96 + 48;
				rcSrc.w = 32;
				rcSrc.h = 64;

				rcDest.x = player.px - 4;
				rcDest.y = player.py + 16 - 48;

				int f = 160;
				if(fra < 8) f = 192 * fra / 8;
				if(fra > 24) f = 192 * (1 - (fra - 24) / 8);

				SDL_SetAlpha(spellimg, SDL_SRCALPHA, f);
				SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
				SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);

				spellinfo[i].frame = spellinfo[i].frame - 0.3 * fpsr;
				if(spellinfo[i].frame < 0) {
					spellinfo[i].frame = 0;
					forcepause = 0;

					npx = player.px + 12;
					npy = player.py + 20;

					int lx = (int)npx / 16; //(npx - (npx MOD 16)) / 16
					int ly = (int)npy / 16; //(npy - (npy MOD 16)) / 16

					for(int f = 0; f <= 4; f++) {
						foundel[f] = 0;
					}

					for(int xo = -2; xo <= 2; xo++) {
						for(int yo = -2; yo <= 2; yo++) {

							int sx = lx + xo;
							int sy = ly + yo;

							if(sx > -1 && sx < 20 && sy > -1 && sy < 15) {
								for(int l = 0; l <= 2; l++) {
									int curtile = tileinfo[l][sx][sy][0];
									int curtilel = tileinfo[l][sx][sy][1];

									if(curtile > 0) {
										curtile = curtile - 1;
										int curtilex = curtile % 20;
										int curtiley = (curtile - curtilex) / 20;

										int element = elementmap[curtiley][curtilex];
										if(element > -1 && curtilel == 0) foundel[element] = 1;
									}
								}

								int o = objmap[sx][sy];
								if(o > -1) {
									if(objectinfo[o][4] == 1) foundel[1] = 1;
									if(o == 1 || o == 2) {
										foundel[1] = 1;
										foundel[3] = 1;
									}
								}
							}
						}
					}

					char line[256];
					strcpy(line, "Found... nothing...");
					f = 0;
					do {
						if(foundel[f] == 1 && player.foundspell[f] == 0) {
							player.foundspell[f] = 1;
							player.spellcharge[f] = 0;
							if(f == 0) strcpy(line, "Found... Water Essence");
							if(f == 1) strcpy(line, "Found... Metal Essence");
							if(f == 2) strcpy(line, "Found... Earth Essence");
							if(f == 3) strcpy(line, "Found... Fire Essence");
							break;
						}

						f=f+1;
						if(f == 5) break;
					} while(1);

					game_eventtext(line);
				}
			}

			// room fireballs
			if(spellnum == 6 && forcepause == 0) {

				if(spellinfo[i].frame > 16) {
					float fr = (32 - spellinfo[i].frame);

					SDL_SetAlpha(spellimg, SDL_SRCALPHA, 192 * sin(3.14159 * fr / 4));

					rcSrc.x = 16 * (int)(RND() * 2);
					rcSrc.y = 80;
					rcSrc.w = 16;
					rcSrc.h = 16;

					for(int ff = 0; ff <= spellinfo[i].nfballs - 1; ff++) {

						xloc = spellinfo[i].fireballs[ff][0] + (int)(RND() * 3) - 1;
						yloc = spellinfo[i].fireballs[ff][1] + (int)(RND() * 3) - 1;

						rcDest.x = xloc;
						rcDest.y = yloc;

						SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
					}

					SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);
				} else {
					SDL_SetAlpha(spellimg, SDL_SRCALPHA, 192);

					rcSrc.x = 16 * (int)(RND() * 2);
					rcSrc.y = 80;
					rcSrc.w = 16;
					rcSrc.h = 16;

					for(int ff = 0; ff <= spellinfo[i].nfballs - 1; ff++) {
						float ax = spellinfo[i].fireballs[ff][0];
						float ay = spellinfo[i].fireballs[ff][1];
						float bx = player.px + 4;
						float by = player.py + 4;
						float d = sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay));

						float tx = (bx - ax) / d;
						float ty = (by - ay) / d;

						spellinfo[i].fireballs[ff][2] += tx * 1.2 * fpsr;
						spellinfo[i].fireballs[ff][3] += ty * 1.2 * fpsr;

						if(spellinfo[i].ballon[ff] == 1) {
							spellinfo[i].fireballs[ff][0] = ax + spellinfo[i].fireballs[ff][2] * 0.2 * fpsr;
							spellinfo[i].fireballs[ff][1] = ay + spellinfo[i].fireballs[ff][3] * 0.2 * fpsr;

							xloc = spellinfo[i].fireballs[ff][0] + (int)(RND() * 3) - 1;
							yloc = spellinfo[i].fireballs[ff][1] + (int)(RND() * 3) - 1;

							rcDest.x = xloc;
							rcDest.y = yloc;

							SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
						}

						if(xloc < -1 || yloc < -1 || xloc > 304 || yloc > 224) spellinfo[i].ballon[ff] = 0;
					}

					SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);
				}

				spellinfo[i].frame = spellinfo[i].frame - 0.2 * fpsr;
				if(spellinfo[i].frame < 0) spellinfo[i].frame = 0;

				if(spellinfo[i].damagewho == 1) {
					for(int ff = 0; ff <= spellinfo[i].nfballs - 1; ff++) {
						if(spellinfo[i].ballon[ff] == 1) {
							xloc = spellinfo[i].fireballs[ff][0] + (int)(RND() * 3) - 1;
							yloc = spellinfo[i].fireballs[ff][1] + (int)(RND() * 3) - 1;

							xdif = (xloc + 8) - (player.px + 12);
							ydif = (yloc + 8) - (player.py + 12);

							if((abs(xdif) < 8 && abs(ydif) < 8) && player.pause < ticks) {
								float damage = npcinfo[spellinfo[i].npc].spelldamage * (1 + RND() * 0.5) / 3;

								if(player.hp > 0) game_damageplayer(damage);

								if(menabled == 1 && opeffects == 1) {
									 int snd = Mix_PlayChannel(-1, sfx[sndfire], 0);
									 Mix_Volume(snd, opeffectsvol);
								}
							}
						}
					}
				}
			}

			// lightning bomb
			if(spellnum == 8) {

				cl1 = SDL_MapRGB(videobuffer->format, 0, 32, 204);
				cl2 = SDL_MapRGB(videobuffer->format, 142, 173, 191);
				cl3 = SDL_MapRGB(videobuffer->format, 240, 240, 240);

				float px = player.px + 12;
				float py = player.py + 12;

				int apx = px + (int)(RND() * 5 - 2);
				int apy = py + (int)(RND() * 5 - 2);

				for(int f = 0; f <= 0; f++) { // ?? 
					int y = apy;
					int orn = 0;
					for(int x = apx; x <= 319; x++) {
						int rn = (int)(RND() * 3);

						if(orn == 0) y = y - 1;
						if(orn == 2) y = y + 1;

						sys_line(videobuffer,x, y - 1, x, y + 2, cl1);
						sys_line(videobuffer,x, y, x, y + 1, cl3);

						if(rn == 0) sys_line(videobuffer, x, y + 1, x, y + 1, cl2);
						if(rn == 2) sys_line(videobuffer, x, y, x, y, cl2);

						orn = rn;

						if(spellinfo[i].damagewho == 0) {
							for(int e = 1; e <= lastnpc; e++) {

								xdif = (x + 16) - (npcinfo[e].x + 12);
								ydif = (y + 16) - (npcinfo[e].y + 12);

								if((abs(xdif) < 8 && abs(ydif) < 8)) {
									float damage = 30 * (1 + RND() * 0.5);

									if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) game_damagenpc(e, damage, 1);
								}
							}
						}
	
						// check for post damage
						if(nposts > 0) {
							for(int e = 0; e <= nposts - 1; e++) {
								xdif = (xloc + 16) - (postinfo[e][0] + 8);
								ydif = (yloc + 16) - (postinfo[e][1] + 8);

								if((abs(xdif) < 16 && abs(ydif) < 16)) {
									objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
									objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

									rcSrc.x = postinfo[e][0] / 2;
									rcSrc.y = postinfo[e][1] / 2;
									rcSrc.w = 8;
									rcSrc.h = 8;

									SDL_FillRect(clipbg2, &rcSrc, 0);

									game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);
								}
							}
						}
					}

					y = apy;
					orn = 0;
					for(int x = apx; x >= 0; x--) {
						int rn = (int)(RND() * 3);

						if(orn == 0) y = y - 1;
						if(orn == 2) y = y + 1;

						sys_line(videobuffer,x, y - 1, x, y + 2, cl1);
						sys_line(videobuffer,x, y, x, y + 1, cl3);

						if(rn == 0) sys_line(videobuffer, x, y + 1, x, y + 1, cl2);
						if(rn == 2) sys_line(videobuffer, x, y, x, y, cl2);

						orn = rn;

						if(spellinfo[i].damagewho == 0) {
							for(int e = 1; e <= lastnpc; e++) {

								xdif = (x + 16) - (npcinfo[e].x + 12);
								ydif = (y + 16) - (npcinfo[e].y + 12);

								if((abs(xdif) < 8 && abs(ydif) < 8)) {
									float damage = 30 * (1 + RND() * 0.5);

									if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) game_damagenpc(e, damage, 1);
								}
							}
						}

						// check for post damage
						if(nposts > 0) {
							for(int e = 0; e <= nposts - 1; e++) {
								xdif = (xloc + 16) - (postinfo[e][0] + 8);
								ydif = (yloc + 16) - (postinfo[e][1] + 8);

								if((abs(xdif) < 16 && abs(ydif) < 16)) {
									objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
									objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

									rcSrc.x = postinfo[e][0] / 2;
									rcSrc.y = postinfo[e][1] / 2;
									rcSrc.w = 8;
									rcSrc.h = 8;

									SDL_FillRect(clipbg2, &rcSrc, 0);

									game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);
								}
							}
						}
					}

					int x = apx;
					orn = 0;
					for(int y = apy; y <= 239; y++) {
						int rn = (int)(RND() * 3);

						if(orn == 0) x = x - 1;
						if(orn == 2) x = x + 1;

						sys_line(videobuffer,x - 1, y, x + 2, y, cl1);
						sys_line(videobuffer,x, y, x + 1, y, cl3);

						if(rn == 0) sys_line(videobuffer, x + 1, y, x + 1, y, cl2);
						if(rn == 2) sys_line(videobuffer, x, y, x, y, cl2);

						orn = rn;

						if(spellinfo[i].damagewho == 0) {
							for(int e = 1; e <= lastnpc; e++) {
								xdif = (x + 16) - (npcinfo[e].x + 12);
								ydif = (y + 16) - (npcinfo[e].y + 12);

								if((abs(xdif) < 8 && abs(ydif) < 8)) {
									float damage = 30 * (1 + RND() * 0.5);

									if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) game_damagenpc(e, damage, 1);
								}
							}
						}

						// check for post damage
						if(nposts > 0) {
							for(int e = 0; e <= nposts - 1; e++) {
								xdif = (xloc + 16) - (postinfo[e][0] + 8);
								ydif = (yloc + 16) - (postinfo[e][1] + 8);

								if((abs(xdif) < 16 && abs(ydif) < 16)) {
									objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
									objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

									rcSrc.x = postinfo[e][0] / 2;
									rcSrc.y = postinfo[e][1] / 2;
									rcSrc.w = 8;
									rcSrc.h = 8;

									SDL_FillRect(clipbg2, &rcSrc, 0);

									game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);
								}
							}
						}
					}

					x = apx;
					orn = 0;
					for(int y = apy; y >= 0; y--) {
						int rn = (int)(RND() * 3);

						if(orn == 0) x = x - 1;
						if(orn == 2) x = x + 1;

						sys_line(videobuffer,x - 1, y, x + 2, y, cl1);
						sys_line(videobuffer,x, y, x + 1, y, cl3);

						if(rn == 0) sys_line(videobuffer, x + 1, y, x + 1, y, cl2);
						if(rn == 2) sys_line(videobuffer, x, y, x, y, cl2);

						orn = rn;

						if(spellinfo[i].damagewho == 0) {
							for(int e = 1; e <= lastnpc; e++) {
								xdif = (x + 16) - (npcinfo[e].x + 12);
								ydif = (y + 16) - (npcinfo[e].y + 12);

								if((abs(xdif) < 8 && abs(ydif) < 8)) {
									float damage = 30 * (1 + RND() * 0.5);

									if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) game_damagenpc(e, damage, 1);
								}
							}
						}

						// check for post damage
						if(nposts > 0) {
							for(int e = 0; e <= nposts - 1; e++) {
								xdif = (xloc + 16) - (postinfo[e][0] + 8);
								ydif = (yloc + 16) - (postinfo[e][1] + 8);

								if((abs(xdif) < 16 && abs(ydif) < 16)) {
									objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
									objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

									rcSrc.x = postinfo[e][0] / 2;
									rcSrc.y = postinfo[e][1] / 2;
									rcSrc.w = 8;
									rcSrc.h = 8;

									SDL_FillRect(clipbg2, &rcSrc, 0);

									game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);
								}
							}
						}
					}
				}

				spellinfo[i].frame -= 0.5 * fpsr;
				if(spellinfo[i].frame < 0) {
					spellinfo[i].frame = 0;
					forcepause = 0;
				}
			}

			// wizard 1 lightning
			if(spellnum == 9) {

				cl1 = SDL_MapRGB(videobuffer->format, 0, 32, 204);
				cl2 = SDL_MapRGB(videobuffer->format, 142, 173, 191);
				cl3 = SDL_MapRGB(videobuffer->format, 240, 240, 240);

				int px = spellinfo[i].enemyx + 12;
				int py = spellinfo[i].enemyy + 24;

				int apx = px + (int)(RND() * 20 - 10);
				int apy = py + (int)(RND() * 20 - 10);

				int x = apx;
				int orn = 0;
				for(int y = 0; y <= apy; y++) {
					if(y < 240) {
						float xdif, ydif;
						int rn = (int)(RND() * 3);

						if(orn == 0) x = x - 1;
						if(orn == 2) x = x + 1;

						sys_line(videobuffer,x - 1, y, x + 2, y, cl1);
						sys_line(videobuffer,x, y, x + 1, y, cl3);

						if(rn == 0) sys_line(videobuffer, x + 1, y, x + 1, y, cl2);
						if(rn == 2) sys_line(videobuffer, x, y, x, y, cl2);

						orn = rn;

						if(spellinfo[i].damagewho == 1) {

							xdif = (x + 8) - (player.px + 12);
							ydif = (y + 8) - (player.py + 12);

							if((abs(xdif) < 8 && abs(ydif) < 8) && player.pause < ticks) {
								float damage = ((float)player.hp * 0.75) * (RND() * 0.5 + 0.5);
								if(damage < 5) damage = 5;

								if(npcinfo[spellinfo[i].npc].spriteset == 12) {
									if(damage < 50) damage = 40 + (int)(RND() * 40);
								}

								if(player.hp > 0) game_damageplayer(damage);
							}
						}
					}
				}

				spellinfo[i].frame -= 0.5 * fpsr;
				if(spellinfo[i].frame < 0) {
					spellinfo[i].frame = 0;

					npcinfo[spellinfo[i].npc].attacking = 0;
					npcinfo[spellinfo[i].npc].attacknext = ticks + npcinfo[spellinfo[i].npc].attackdelay;
				}
			}
		}
	}
}



void game_updspellsunder()
{

	unsigned int dq, *temp;
	/*float npx, npy, onpx, onpy;*/

	if(forcepause == 1) return;

	for(int i = 0; i <= MAXSPELL; i++) {
		if(spellinfo[i].frame > 0) {
			int spellnum = spellinfo[i].spellnum;

			// water
			if(spellnum == 0) {
				int fra = (32 - spellinfo[i].frame);
				int fr = (int)((32 - spellinfo[i].frame) * 2) % 4;

				rcSrc.x = fr * 48;
				rcSrc.y = 96;
				rcSrc.w = 48;
				rcSrc.h = 48;

				rcDest.x = spellinfo[i].enemyx - 12;
				rcDest.y = spellinfo[i].enemyy - 8;

				int f = 160;
				if(fra < 8) f = 160 * fra / 8;
				if(fra > 24) f = 160 * (1 - (fra - 24) / 8);

				SDL_SetAlpha(spellimg, SDL_SRCALPHA, f);
				SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);
				SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);

				spellinfo[i].frame = spellinfo[i].frame - 0.2 * fpsr;
				if(spellinfo[i].frame < 0) spellinfo[i].frame = 0;


				for(int f = 1; f <= lastnpc; f++) {
					int xdif = spellinfo[i].enemyx - npcinfo[f].x;
					int ydif = spellinfo[i].enemyy - npcinfo[f].y;

					float dist = sqrt(xdif * xdif + ydif * ydif);

					if(dist > 20) dist = 20;

					if(dist > 5) {
						int xm = 1;
						if(xdif < 0) xm = -1;
						int ym = 1;
						if(ydif < 0) ym = -1;

						float ratio = (1 - dist / 25);

						float newx = npcinfo[f].x + ratio * xdif / 3 * fpsr;
						float newy = npcinfo[f].y + ratio * ydif / 3 * fpsr;

						int sx = (newx / 2 + 6);
						int sy = (newy / 2 + 10);
						temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
						dq = *temp;

						// SDL_UnLockSurface clipbg

						if(dq == 0) {
							npcinfo[f].x = newx;
							npcinfo[f].y = newy;
							// npcinfo[f].castpause = ticks + 200;
						} else {
							int xpass = 0;
							int ypass = 0;

							sx = (newx / 2 + 6);
							sy = (npcinfo[f].y / 2 + 10);
							temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
							dq = *temp;

							if(dq == 0) xpass = 1;


							sx = (npcinfo[f].x / 2 + 6);
							sy = (newy / 2 + 10);
							temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
							dq = *temp;

							if(dq == 0) ypass = 1;

							if(ypass == 1) {
								newx = npcinfo[f].x;
							} else if(xpass == 1) {
								newy = npcinfo[f].y;
							}

							if(xpass == 1 || ypass == 1) {
								npcinfo[f].x = newx;
								npcinfo[f].y = newy;
								// npcinfo[f].castpause = ticks + 200;
							}
						}
					}
				}
			}

			// fire
			if(spellnum == 3) {
				float fr = (32 - spellinfo[i].frame);

				fr = fr*fr * (1 - cos(3.14159 / 4 + 3.14159 / 4 * fr / 32));

				if(fr > 32) fr = 32;

				float s = 8;
				if(spellinfo[i].frame < 8) s = spellinfo[i].frame;

				int fra = (int)fr;

				for(int f = 0; f <= 4; f++) {
					for(int x = 0; x <= fra; x += 2) {
						if(spellinfo[i].legalive[f] >= x) {
							SDL_SetAlpha(spellimg, SDL_SRCALPHA, 192 * sin(3.14159 * x / 32) * s / 8);

							float an = 360 / 5 * f + x / 32 * 180;

							rcSrc.x = 16 * (int)(RND() * 2);
							rcSrc.y = 80;
							rcSrc.w = 16;
							rcSrc.h = 16;

							float xloc = (float)(spellinfo[i].enemyx + 4 + x * 2 * cos(3.14159 / 180 * an) + (int)(RND() * 3) - 1);
							float yloc = (float)(spellinfo[i].enemyy + 4 + x * 2 * sin(3.14159 / 180 * an) + (int)(RND() * 3) - 1);
							rcDest.x = (int)xloc;
							rcDest.y = (int)yloc;

							if(xloc > -1 && xloc < 304 && yloc > -1 && yloc < 224) {
								SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);

								int sx = (xloc / 2 + 4);
								int sy = (yloc / 2 + 8);
								temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
								dq = *temp;

								if(dq > 1000 && x > 4) spellinfo[i].legalive[f] = x;

								if(spellinfo[i].damagewho == 0) {
									for(int e = 1; e <= lastnpc; e++) {
										float xdif = (xloc + 8) - (npcinfo[e].x + 12);
										float ydif = (yloc + 8) - (npcinfo[e].y + 12);

										if((abs(xdif) < 8 && abs(ydif) < 8)) {
											float damage = (float)player.spelldamage * (1.0 + RND() * 0.5) * (float)spellinfo[i].strength;

											if(npcinfo[e].spriteset == 5) damage = -damage;
											if(npcinfo[e].spriteset == 11) damage = -damage;
											if(npcinfo[e].hp > 0 && npcinfo[e].pause < ticks) {
												game_damagenpc(e, damage, 1);
												if(menabled == 1 && opeffects == 1) {
													int snd = Mix_PlayChannel(-1, sfx[sndfire], 0);
													Mix_Volume(snd, opeffectsvol);
												}
											}
										}
									}
								}

								if(spellinfo[i].damagewho == 1) {
									float xdif = (xloc + 8) - (player.px + 12);
									float ydif = (yloc + 8) - (player.py + 12);

									if((abs(xdif) < 8 && abs(ydif) < 8) && player.pause < ticks) {
										float damage = (float)npcinfo[spellinfo[i].npc].spelldamage * (1.0 + RND() * 0.5);

										if(player.hp > 0) {
											game_damageplayer(damage);

											if(menabled == 1 && opeffects == 1) {
												int snd = Mix_PlayChannel(-1, sfx[sndfire], 0);
												Mix_Volume(snd, opeffectsvol);
											}
										}
									}
								}

								// check for post damage
								if(nposts > 0) {
									for(int e = 0; e <= nposts - 1; e++) {
										float xdif = (xloc + 8) - (postinfo[e][0] + 8);
										float ydif = (yloc + 8) - (postinfo[e][1] + 8);

										if((abs(xdif) < 8 && abs(ydif) < 8)) {
											objmapf[curmap][(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = 1;
											objmap[(int)postinfo[e][0] / 16][(int)postinfo[e][1] / 16] = -1;

											rcSrc.x = postinfo[e][0] / 2;
											rcSrc.y = postinfo[e][1] / 2;
											rcSrc.w = 8;
											rcSrc.h = 8;

											SDL_FillRect(clipbg2, &rcSrc, 0);

											if(menabled == 1 && opeffects == 1) {
												int snd = Mix_PlayChannel(-1, sfx[sndfire], 0);
												Mix_Volume(snd, opeffectsvol);
											}

											game_addFloatIcon(99, postinfo[e][0], postinfo[e][1]);
										}
									}
								}
							}
						}
					}
				}

				SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);

				spellinfo[i].frame = spellinfo[i].frame - 0.2 * fpsr;
				if(spellinfo[i].frame < 0) spellinfo[i].frame = 0;


			}


			// sprite 6 spitfire
			if(spellnum == 7) {
				float xspan = spellinfo[i].enemyx - spellinfo[i].homex;
				float yspan = spellinfo[i].enemyy - spellinfo[i].homey;
				float fr = (32 - spellinfo[i].frame);

				for(int f = 0; f <= 7; f++) {
					int alpha = 0;
					float xx = 0;
					if(fr > f * 2 && fr < f * 2 + 16) xx = fr - f * 2;
					if(xx < 8) alpha = 255 * xx / 8;
					if(xx > 8) alpha = 255 * (1 - (xx - 8) / 8);
					float yy = 16 * sin(3.141592 / 2 * xx / 16) - 8;

					if(alpha < 0) alpha = 0;
					if(alpha > 255) alpha = 255;

					SDL_SetAlpha(spellimg, SDL_SRCALPHA, alpha);

					rcSrc.x = 16 * (int)(RND() * 2);
					rcSrc.y = 80;
					rcSrc.w = 16;
					rcSrc.h = 16;

					float xloc = spellinfo[i].homex + xspan / 7 * f;
					float yloc = spellinfo[i].homey + yspan / 7 * f - yy;

					rcDest.x = xloc;
					rcDest.y = yloc;

					if(xloc > -16 && xloc < 320 && yloc > -16 && yloc < 240) {
						SDL_BlitSurface(spellimg, &rcSrc, videobuffer, &rcDest);

						if(spellinfo[i].damagewho == 1) {
							float xdif = (xloc + 8) - (player.px + 12);
							float ydif = (yloc + 8) - (player.py + 12);

							if((abs(xdif) < 8 && abs(ydif) < 8) && player.pause < ticks && alpha > 64) {
								float damage = (float)npcinfo[spellinfo[i].npc].spelldamage * (1.0 + RND() * 0.5);

								if(player.hp > 0) {
									game_damageplayer(damage);
									if(menabled == 1 && opeffects == 1) {
										int snd = Mix_PlayChannel(-1, sfx[sndfire], 0);
										Mix_Volume(snd, opeffectsvol);
									}
								}

							}
						}

					}

				}

				SDL_SetAlpha(spellimg, SDL_SRCALPHA, 255);
				spellinfo[i].frame = spellinfo[i].frame - 0.5 * fpsr;
				if(spellinfo[i].frame < 0) spellinfo[i].frame = 0;

				if(spellinfo[i].frame == 0) {
					npcinfo[spellinfo[i].npc].attacking = 0;
					npcinfo[spellinfo[i].npc].attacknext = ticks + npcinfo[spellinfo[i].npc].attackdelay;
				}
			}
		}
	}
}

void sys_initialize()
{
	int result;
	char line[128];
	char arg[128];
	FILE *fp;

	// init char *floatstri[MAXFLOAT]
	for(int i = 0; i <= MAXFLOAT; i++)
		floatstri[i] = malloc(64); // 64 bytes each string (should be enough)

	// set default values
	SCR_WIDTH = 320;
	SCR_HEIGHT = 240;
	SCR_BITS = 32;
	HWACCEL = 0;
	HWSURFACE = SDL_SWSURFACE;
	opfullscreen = 0;
	opmusic = 1;
	opeffects = 1;
	opmusicvol = 127;
	opeffectsvol = 127;

	fp = fopen("config.ini", "r");
	if(fp) {
		while(fgets(line, sizeof(line), fp) != NULL) {
			sscanf(line, "%s", arg); // eliminate eol and eof by this

			if(strcmp(arg, "SCR_WIDTH:") == 0) {
				fgets(line, sizeof(line), fp);
				sscanf(line, "%i", &SCR_WIDTH);
			} else if(strcmp(arg, "SCR_HEIGHT:") == 0) {
				fgets(line, sizeof(line), fp);
				sscanf(line, "%i", &SCR_HEIGHT);
			} else if(strcmp(arg, "SCR_BITS:") == 0) {
				fgets(line, sizeof(line), fp);
				sscanf(line, "%i", &SCR_BITS);
			} else if(strcmp(arg, "HWACCEL:YES") == 0) {
				HWACCEL = SDL_HWACCEL;
			} else if(strcmp(arg, "HWSURFACE:YES") == 0) {
				HWSURFACE = SDL_HWSURFACE;
			} else if(strcmp(arg, "FULLSCREEN:YES") == 0) {
				opfullscreen = SDL_FULLSCREEN;
			} else if(strcmp(arg, "MUSIC:YES") == 0) {
				opmusic = 1;
			} else if(strcmp(arg, "SNDEFFECTS:YES") == 0) {
				opeffects = 1;
			} else if(strcmp(arg, "opmusicvol:") == 0) {
				fgets(line, sizeof(line), fp);
				sscanf(line, "%i", &opmusicvol);
			} else if(strcmp(arg, "opeffectsvol:") == 0) {
				fgets(line, sizeof(line), fp);
				sscanf(line, "%i", &opeffectsvol);
			}
		}

		fclose(fp);
	}

	printf("SCR_WIDTH: %i\n", SCR_WIDTH);
	printf("SCR_HEIGHT: %i\n", SCR_HEIGHT);
	printf("SCR_BITS: %i\n", SCR_BITS);

	SCR_TOPX = SCR_WIDTH / 2 - 160;
	SCR_TOPY = SCR_HEIGHT / 2 - 120;

	fullscreen = opfullscreen | HWACCEL | HWSURFACE;

	result = SDL_Init(SDL_INIT_EVERYTHING);
	if(result) {
		printf("Failed to init SDL\n");
		exit(1);
	}

	atexit(SDL_Quit);

	video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen); // SDL_FULLSCREEN
	if(!video) {
		printf("Failed to init Video\n");
		exit(1);
	}


	SDL_WM_SetCaption("The Griffon Legend", NULL);

	SDL_ShowCursor(SDL_DISABLE);

	videobuffer = SDL_DisplayFormat(video);
	videobuffer2 = SDL_DisplayFormat(video);
	videobuffer3 = SDL_DisplayFormat(video);
	mapbg = SDL_CreateRGBSurface(HWSURFACE, 640, 480, SCR_BITS, video->format->Rmask, video->format->Gmask, video->format->Bmask, video->format->Amask);
	clipbg = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, SCR_BITS, video->format->Rmask, video->format->Gmask, video->format->Bmask, video->format->Amask);
	clipbg2 = SDL_DisplayFormat(video);

	for(int i = 0; i <= 3; i++) {
		char name[128];

		sprintf(name, "art/map%i.bmp", i + 1);
		mapimg[i] = SDL_DisplayFormat(video);
		mapimg[i] = IMG_Load(name);
		SDL_SetColorKey(mapimg[i], SDL_SRCCOLORKEY, SDL_MapRGB(mapimg[i]->format, 255, 0, 255));
	}

	cloudimg = IMG_Load("art/clouds.bmp");
	SDL_SetColorKey(cloudimg, SDL_SRCCOLORKEY, SDL_MapRGB(cloudimg->format, 255, 0, 255));
	SDL_SetAlpha(cloudimg, 0 | SDL_SRCALPHA, 96);


	saveloadimg = IMG_Load("art/saveloadnew.bmp");
	SDL_SetColorKey(saveloadimg, SDL_SRCCOLORKEY, SDL_MapRGB(saveloadimg->format, 255, 0, 255));
	SDL_SetAlpha(saveloadimg, 0 | SDL_SRCALPHA, 160);

	titleimg = IMG_Load("art/titleb.bmp");
	titleimg2 = IMG_Load("art/titlea.bmp");
	SDL_SetColorKey(titleimg2, SDL_SRCCOLORKEY, SDL_MapRGB(titleimg2->format, 255, 0, 255));
	//SDL_SetAlpha(titleimg2, 0 | SDL_SRCALPHA, 204);

	inventoryimg = IMG_Load("art/inventory.bmp");
	SDL_SetColorKey(inventoryimg, SDL_SRCCOLORKEY, SDL_MapRGB(inventoryimg->format, 255, 0, 255));

	logosimg = SDL_DisplayFormat(video);
	theendimg = SDL_DisplayFormat(video);
	logosimg = IMG_Load("art/logos.bmp");
	theendimg = IMG_Load("art/theend.bmp");


	sys_LoadTiles();
	sys_LoadTriggers();
	sys_LoadObjectDB();
	sys_LoadAnims();
	sys_LoadFont();
	sys_LoadItemImgs();

	fpsr = 1;
	nextticks = ticks + 1000;

	maxlevel = 22;

	for(int i = 0; i <= 15; i++) {
		playerattackofs[0][i][0] = 0; // -1// -(i + 1)
		playerattackofs[0][i][1] = -sin(3.14159 * 2 * (i + 1) / 16) * 2 - 1;

		playerattackofs[1][i][0] = 0; // i + 1
		playerattackofs[1][i][1] = -sin(3.14159 * 2 * (i + 1) / 16) * 2 + 1;

		playerattackofs[2][i][0] = -1; // -(i + 1)
		playerattackofs[2][i][1] = -sin(3.14159 * 2 * (i + 1) / 16) * 2;

		playerattackofs[3][i][0] = 1; // i + 1
		playerattackofs[3][i][1] = -sin(3.14159 * 2 * (i + 1) / 16) * 2;
	}

	sys_setupAudio();
}

void sys_line(SDL_Surface *buffer, int x1, int y1, int x2, int y2, int col)
{
	unsigned int *temp;

	SDL_LockSurface(buffer);

	int xdif = x2 - x1;
	int ydif = y2 - y1;

	if(xdif == 0) {
		for(int y = y1; y <= y2; y++) {
			temp = buffer->pixels + y * buffer->pitch + x1 * buffer->format->BytesPerPixel;
			*temp = col;
		}
	}

	if(ydif == 0) {
		for(int x = x1; x <= x2; x++) {
			temp = buffer->pixels + y1 * buffer->pitch + x * buffer->format->BytesPerPixel;
			*temp = col;
		}
	}

	SDL_UnlockSurface(buffer);
}

void sys_LoadAnims()
{
	spellimg = IMG_Load("art/spells.bmp");
	SDL_SetColorKey(spellimg, SDL_SRCCOLORKEY, SDL_MapRGB(spellimg->format, 255, 0, 255));

	anims[0] = IMG_Load("art/anims0.bmp");
	SDL_SetColorKey(anims[0], SDL_SRCCOLORKEY, SDL_MapRGB(anims[0]->format, 255, 0, 255));

	animsa[0] = IMG_Load("art/anims0a.bmp");
	SDL_SetColorKey(animsa[0], SDL_SRCCOLORKEY, SDL_MapRGB(animsa[0]->format, 255, 0, 255));

	anims[13] = IMG_Load("art/anims0x.bmp");
	SDL_SetColorKey(anims[13], SDL_SRCCOLORKEY, SDL_MapRGB(anims[13]->format, 255, 0, 255));

	animsa[13] = IMG_Load("art/anims0xa.bmp");
	SDL_SetColorKey(animsa[13], SDL_SRCCOLORKEY, SDL_MapRGB(animsa[13]->format, 255, 0, 255));

	anims[1] = IMG_Load("art/anims1.bmp");
	SDL_SetColorKey(anims[1], SDL_SRCCOLORKEY, SDL_MapRGB(anims[1]->format, 255, 0, 255));

	animsa[1] = IMG_Load("art/anims1a.bmp");
	SDL_SetColorKey(animsa[1], SDL_SRCCOLORKEY, SDL_MapRGB(animsa[1]->format, 255, 0, 255));

	anims[2] = IMG_Load("art/anims2.bmp");
	SDL_SetColorKey(anims[2], SDL_SRCCOLORKEY, SDL_MapRGB(anims[2]->format, 255, 0, 255));

	// huge
	animset2[0].xofs = 8;
	animset2[0].yofs = 7;
	animset2[0].x = 123;
	animset2[0].y = 0;
	animset2[0].w = 18;
	animset2[0].h = 16;
	// big
	animset2[1].xofs = 7;
	animset2[1].yofs = 7;
	animset2[1].x = 107;
	animset2[1].y = 0;
	animset2[1].w = 16;
	animset2[1].h = 14;
	// med
	animset2[2].xofs = 6;
	animset2[2].yofs = 6;
	animset2[2].x = 93;
	animset2[2].y = 0;
	animset2[2].w = 14;
	animset2[2].h = 13;
	// small
	animset2[3].xofs = 4;
	animset2[3].yofs = 4;
	animset2[3].x = 83;
	animset2[3].y = 0;
	animset2[3].w = 10;
	animset2[3].h = 10;
	// wing
	animset2[4].xofs = 4;
	animset2[4].yofs = 20;
	animset2[4].x = 42;
	animset2[4].y = 0;
	animset2[4].w = 41;
	animset2[4].h = 33;
	// head
	animset2[5].xofs = 20;
	animset2[5].yofs = 18;
	animset2[5].x = 0;
	animset2[5].y = 0;
	animset2[5].w = 42;
	animset2[5].h = 36;

	anims[9] = IMG_Load("art/anims9.bmp");
	SDL_SetColorKey(anims[9], SDL_SRCCOLORKEY, SDL_MapRGB(anims[9]->format, 255, 0, 255));

	// huge
	animset9[0].xofs = 8;
	animset9[0].yofs = 7;
	animset9[0].x = 154;
	animset9[0].y = 0;
	animset9[0].w = 18;
	animset9[0].h = 16;
	// big
	animset9[1].xofs = 7;
	animset9[1].yofs = 7;
	animset9[1].x = 138;
	animset9[1].y = 0;
	animset9[1].w = 16;
	animset9[1].h = 14;
	// med
	animset9[2].xofs = 6;
	animset9[2].yofs = 6;
	animset9[2].x = 93 + 31;
	animset9[2].y = 0;
	animset9[2].w = 14;
	animset9[2].h = 13;
	// small
	animset9[3].xofs = 4;
	animset9[3].yofs = 4;
	animset9[3].x = 83 + 31;
	animset9[3].y = 0;
	animset9[3].w = 10;
	animset9[3].h = 10;
	// wing
	animset9[4].xofs = 36;
	animset9[4].yofs = 20;
	animset9[4].x = 42;
	animset9[4].y = 0;
	animset9[4].w = 72;
	animset9[4].h = 33;
	// head
	animset9[5].xofs = 20;
	animset9[5].yofs = 18;
	animset9[5].x = 0;
	animset9[5].y = 0;
	animset9[5].w = 42;
	animset9[5].h = 36;

	anims[3] = IMG_Load("art/anims3.bmp");
	SDL_SetColorKey(anims[3], SDL_SRCCOLORKEY, SDL_MapRGB(anims[3]->format, 255, 0, 255));

	anims[4] = IMG_Load("art/anims4.bmp");
	SDL_SetColorKey(anims[4], SDL_SRCCOLORKEY, SDL_MapRGB(anims[4]->format, 255, 0, 255));

	anims[5] = IMG_Load("art/anims5.bmp");
	SDL_SetColorKey(anims[5], SDL_SRCCOLORKEY, SDL_MapRGB(anims[5]->format, 255, 0, 255));

	anims[6] = IMG_Load("art/anims6.bmp");
	SDL_SetColorKey(anims[6], SDL_SRCCOLORKEY, SDL_MapRGB(anims[6]->format, 255, 0, 255));

	anims[7] = IMG_Load("art/anims7.bmp");
	SDL_SetColorKey(anims[7], SDL_SRCCOLORKEY, SDL_MapRGB(anims[7]->format, 255, 0, 255));

	anims[8] = IMG_Load("art/anims8.bmp");
	SDL_SetColorKey(anims[8], SDL_SRCCOLORKEY, SDL_MapRGB(anims[8]->format, 255, 0, 255));

	anims[10] = IMG_Load("art/anims10.bmp");
	SDL_SetColorKey(anims[10], SDL_SRCCOLORKEY, SDL_MapRGB(anims[10]->format, 255, 0, 255));

	animsa[10] = IMG_Load("art/anims10a.bmp");
	SDL_SetColorKey(animsa[10], SDL_SRCCOLORKEY, SDL_MapRGB(animsa[10]->format, 255, 0, 255));

	anims[11] = IMG_Load("art/anims11.bmp");
	SDL_SetColorKey(anims[11], SDL_SRCCOLORKEY, SDL_MapRGB(anims[11]->format, 255, 0, 255));

	animsa[11] = IMG_Load("art/anims11a.bmp");
	SDL_SetColorKey(animsa[11], SDL_SRCCOLORKEY, SDL_MapRGB(animsa[11]->format, 255, 0, 255));

	anims[12] = IMG_Load("art/anims12.bmp");
	SDL_SetColorKey(anims[12], SDL_SRCCOLORKEY, SDL_MapRGB(anims[12]->format, 255, 0, 255));
}

void sys_LoadItemImgs()
{
	SDL_Surface *temp;

	temp = IMG_Load("art/icons.bmp");

	for(int i = 0; i <= 20; i++) {
		itemimg[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 16, 16, SCR_BITS, video->format->Rmask, video->format->Gmask, video->format->Bmask, video->format->Amask);
		SDL_SetColorKey(itemimg[i], SDL_SRCCOLORKEY, SDL_MapRGB(itemimg[i]->format, 255, 0, 255));

		rcSrc.x = i * 16;
		rcSrc.y = 0;
		rcSrc.w = 16;
		rcSrc.h = 16;

		SDL_BlitSurface(temp, &rcSrc, itemimg[i], NULL);
	}

	SDL_FreeSurface(temp);
}

void sys_LoadFont()
{
	SDL_Surface *font;

	font = IMG_Load("art/font.bmp");

	for(int i = 32; i <= 255; i++)
		for(int f = 0; f <= 4; f++) {
			int i2 = i - 32;

			fontchr[i2][f] = SDL_CreateRGBSurface(SDL_SWSURFACE, 8, 8, SCR_BITS, video->format->Rmask, video->format->Gmask, video->format->Bmask, video->format->Amask);
			SDL_SetColorKey(fontchr[i2][f], SDL_SRCCOLORKEY, SDL_MapRGB(fontchr[i2][f]->format, 255, 0, 255));

			int col = i2 % 40;

			int row = (i2 - col) / 40;

			rcSrc.x = col * 8;
			rcSrc.y = row * 8 + f * 48;
			rcSrc.w = 8;
			rcSrc.h = 8;

			rcDest.x = 0;
			rcDest.y = 0;
			SDL_BlitSurface(font, &rcSrc, fontchr[i2][f], &rcDest);
		}

	SDL_FreeSurface(font);
}

void sys_LoadTiles()
{
	tiles[0] = IMG_Load("art/tx.bmp");
	tiles[1] = IMG_Load("art/tx1.bmp");
	tiles[2] = IMG_Load("art/tx2.bmp");
	tiles[3] = IMG_Load("art/tx3.bmp");

	for(int i = 0; i <= 3; i++)
		SDL_SetColorKey(tiles[i], SDL_SRCCOLORKEY, SDL_MapRGB(tiles[i]->format, 255, 0, 255));

	windowimg = IMG_Load("art/window.bmp");
	SDL_SetColorKey(windowimg, SDL_SRCCOLORKEY, SDL_MapRGB(windowimg->format, 255, 0, 255));
}

void sys_LoadTriggers()
{
	FILE *fp;
	char line[512];

	fp = fopen("data/triggers.dat", "r");

	for(int i = 0; i <= 9999; i++)
		for(int a = 0; a <= 8; a++) {
			fgets(line, sizeof(line), fp);
			sscanf(line, "%i", &triggers[i][a]);
		}

	fclose(fp);
}

void sys_LoadObjectDB()
{
	FILE *fp;
	char line[512];
	// DIM SHARED objectinfo(255, 6)
				// nframes,xtiles,ytiles,speed,type,script, update?

	// DIM SHARED objecttile(255, 4, 2, 2, 1)
				// [objnum] [frame] [x] [y] [tile/layer]

	fp = fopen("objectdb.dat", "r");

	for(int a = 0; a <= 32; a++) {
		for(int b = 0; b <= 5; b++) {
			fgets(line, sizeof(line), fp);
			sscanf(line, "%i", &objectinfo[a][b]);
		}

		for(int b = 0; b <= 8; b++) {
			for(int c = 0; c <= 2; c++) {
				for(int d = 0; d <= 2; d++) {
					for(int e = 0; e <= 1; e++) {
						fgets(line, sizeof(line), fp);
						sscanf(line, "%i", &objecttile[a][b][c][d][e]);
					}
				}
			}
		}
	}

	fclose(fp);
}

void sys_print(SDL_Surface *buffer, char *stri, int xloc, int yloc, int col)
{
	int l = strlen(stri);

	for(int i = 0; i < l; i++) {
		rcDest.x = xloc + i * 8;
		rcDest.y = yloc;

		SDL_BlitSurface(fontchr[stri[i] - 32][col], NULL, buffer, &rcDest);
	}
}

void sys_progress(int w, int wm)
{
	long ccc;

	ccc = SDL_MapRGB(videobuffer->format, 0, 255, 0);

	rcDest.w = w * 74 / wm;
	SDL_FillRect(video, &rcDest, ccc);
	SDL_Flip(video);
}

void sys_setupAudio()
{
	SDL_Surface *loadimg;

	menabled = 1;

	if((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG) {
		printf("Failed to init OGG support\n");
		exit(1);
	}

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

	atexit(Mix_Quit);
	atexit(Mix_CloseAudio);

	char *stri = "Loading...";
	sys_print(video, stri, SCR_TOPX + 160 - 4 * strlen(stri), SCR_TOPY + 116, 0);

	loadimg = IMG_Load("art/load.bmp");
	SDL_SetColorKey(loadimg, SDL_SRCCOLORKEY, SDL_MapRGB(loadimg->format, 255, 0, 255));

	SDL_Flip(video);

	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = 88;
	rcSrc.h = 32;

	rcDest.x = SCR_TOPX + 160 - 44;
	rcDest.y = SCR_TOPY + 116 + 12;

	SDL_SetAlpha(loadimg, 0 | SDL_SRCALPHA, 160); // 128
	SDL_BlitSurface(loadimg, &rcSrc, video, &rcDest);
	SDL_SetAlpha(loadimg, 0 | SDL_SRCALPHA, 255);

	rcDest.x = SCR_TOPX + 160 - 44 + 7;
	rcDest.y = SCR_TOPY + 116 + 12 + 12;
	rcDest.h = 8;

	if(menabled == 1) {
		mboss = Mix_LoadWAV("music/boss.ogg");
		sys_progress(1, 21);
		mgardens = Mix_LoadWAV("music/gardens.ogg");
		sys_progress(2, 21);
		mgardens2 = Mix_LoadWAV("music/gardens2.ogg");
		sys_progress(3, 21);
		mgardens3 = Mix_LoadWAV("music/gardens3.ogg");
		sys_progress(4, 21);
		mgardens4 = Mix_LoadWAV("music/gardens4.ogg");
		sys_progress(5, 21);
		mendofgame = Mix_LoadWAV("music/endofgame.ogg");
		sys_progress(6, 21);
		mmenu = Mix_LoadWAV("music/menu.ogg");
		sys_progress(7, 21);

		sfx[0] = Mix_LoadWAV("sfx/bite.ogg");
		sys_progress(8, 21);
		sfx[1] = Mix_LoadWAV("sfx/crystal.ogg");
		sys_progress(9, 21);
		sfx[2] = Mix_LoadWAV("sfx/door.ogg");
		sys_progress(10, 21);
		sfx[3] = Mix_LoadWAV("sfx/enemyhit.ogg");
		sys_progress(11, 21);
		sfx[4] = Mix_LoadWAV("sfx/ice.ogg");
		sys_progress(12, 21);
		sfx[5] = Mix_LoadWAV("sfx/lever.ogg");
		sys_progress(13, 21);
		sfx[6] = Mix_LoadWAV("sfx/lightning.ogg");
		sys_progress(14, 21);
		sfx[7] = Mix_LoadWAV("sfx/metalhit.ogg");
		sys_progress(15, 21);
		sfx[8] = Mix_LoadWAV("sfx/powerup.ogg");
		sys_progress(16, 21);
		sfx[9] = Mix_LoadWAV("sfx/rocks.ogg");
		sys_progress(17, 21);
		sfx[10] = Mix_LoadWAV("sfx/swordhit.ogg");
		sys_progress(18, 21);
		sfx[11] = Mix_LoadWAV("sfx/throw.ogg");
		sys_progress(19, 21);
		sfx[12] = Mix_LoadWAV("sfx/chest.ogg");
		sys_progress(20, 21);
		sfx[13] = Mix_LoadWAV("sfx/fire.ogg");
		sys_progress(21, 21);
		sfx[14] = Mix_LoadWAV("sfx/beep.ogg");
	}
}

void sys_update()
{
	unsigned int *temp, bgc;
	int pa, sx, sy;
	float opx, opy, spd;

	// SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
	SDL_Flip(video);
	SDL_PumpEvents();

	tickspassed = ticks;
	ticks = SDL_GetTicks();

	tickspassed = ticks - tickspassed;
	fpsr = (float)tickspassed / 24.0;

	fp++;
	if(ticks > nextticks) {
		nextticks = ticks + 1000;
		fps = fp;
		fp = 0;
		secsingame = secsingame + 1;
	}

	SDL_LockSurface(clipbg);

	if(attacking == 1) {
		player.attackframe += player.attackspd * fpsr;
		if(player.attackframe >= 16) {
			attacking = 0;
			player.attackframe = 0;
			player.walkframe = 0;
		}

		pa = (int)(player.attackframe);

		for(int i = 0; i <= pa; i++) {
			if(playerattackofs[player.walkdir][i][2] == 0) {
				playerattackofs[player.walkdir][i][2] = 1;

				opx = player.px;
				opy = player.py;

				player.px = player.px + playerattackofs[player.walkdir][i][0];
				player.py = player.py + playerattackofs[player.walkdir][i][1];

				sx = (int)(player.px / 2 + 6);
				sy = (int)(player.py / 2 + 10);
				temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel;
				bgc = *temp;
				if(bgc > 0) {
					player.px = opx;
					player.py = opy;
				}
			}
		}

		player.opx = player.px;
		player.opy = player.py;

		game_checkhit();
	}

	for(int i = 0; i <= MAXFLOAT; i++) {
		if(floattext[i][0] > 0) {
			spd = 0.5 * fpsr;
			floattext[i][0] = floattext[i][0] - spd;
			floattext[i][2] = floattext[i][2] - spd;
			if(floattext[i][0] < 0) floattext[i][0] = 0;
		}

		if(floaticon[i][0] > 0) {
			spd = 0.5 * fpsr;
			floaticon[i][0] = floaticon[i][0] - spd;
			floaticon[i][2] = floaticon[i][2] - spd;
			if(floaticon[i][0] < 0) floaticon[i][0] = 0;
		}
	}

	if(player.level == maxlevel) player.exp = 0;

	if(player.exp >= player.nextlevel) {
		player.level = player.level + 1;
		game_addFloatText("LEVEL UP!", player.px + 16 - 36, player.py + 16, 3);
		player.exp = player.exp - player.nextlevel;
		// player.nextlevel = player.level * 75;
		player.nextlevel = player.nextlevel * 2;
		player.maxhp = player.maxhp + player.level * 3;
		if(player.maxhp > 999) player.maxhp = 999;
		player.hp = player.maxhp;

		player.sworddamage = player.level * 1.4;
		player.spelldamage = player.level * 1.3;

		if(menabled == 1 && opeffects == 1) {
			int snd = Mix_PlayChannel(-1, sfx[sndpowerup], 0);
			Mix_Volume(snd, opeffectsvol);
		}
	}

	SDL_UnlockSurface(clipbg);
	SDL_BlitSurface(clipbg2, NULL, clipbg, NULL);

	rc.x = player.px - 2;
	rc.y = player.py - 2;
	rc.w = 5;
	rc.h = 5;

	SDL_FillRect(clipbg, &rc, 1000);

	if(forcepause == 0) {
		if(player.foundcrystal == 1) player.crystalcharge += 5 * player.level * 0.001 * fpsr;
		if(player.crystalcharge > 100) player.crystalcharge = 100;

		for(int i = 0; i <= 4; i++) {
			if(player.foundspell[i] == 1) player.spellcharge[i] += 1 * player.level * 0.01 * fpsr;
			if(player.spellcharge[i] > 100) player.spellcharge[i] = 100;
		}

		player.attackstrength += (30 + 3 * (float)player.level) / 50 * fpsr;

		if(player.foundcrystal) {
			player.spellstrength += 3 * player.level * .01 * fpsr;
		}
	}

	if(player.attackstrength > 100) player.attackstrength = 100;

	if(player.spellstrength > 100) player.spellstrength = 100;

	itemyloc += 0.75 * fpsr;
	while(itemyloc >= 16) itemyloc -= 16;

	if(player.hp <= 0) game_theend();

	if(roomlock == 1) {
		roomlock = 0;
		for(int i = 1; i <= lastnpc; i++)
			if(npcinfo[i].hp > 0) roomlock = 1;
	}

	clouddeg += 0.1 * fpsr;
	while(clouddeg >= 360) clouddeg = clouddeg - 360;

	player.hpflash = player.hpflash + 0.1 * fpsr;
	if(player.hpflash >=2) {
		player.hpflash = 0;
		player.hpflashb = player.hpflashb + 1;
		if(player.hpflashb == 2) player.hpflashb = 0;
		if(menabled == 1 && opeffects == 1 && player.hpflashb == 0 && player.hp < player.maxhp / 4) {
			int snd = Mix_PlayChannel(-1, sfx[sndbeep], 0);
			Mix_Volume(snd, opeffectsvol);
		}
	}

	// cloudson = 0

	if(itemselon == 1) player.itemselshade = player.itemselshade + 2 * fpsr;
	if(player.itemselshade > 24) player.itemselshade = 24;

	for(int i = 0; i <= 4; i++)
		if(inventory[i] > 9) inventory[i] = 9;
}
