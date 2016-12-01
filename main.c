/* 

TODO 

start at last level / choose lvl seen in session
title screens: a tile16 at same vram and same tileset than tilemap ! 
end level (FX - tt ce qui n'est pas rocher explose, pause et qq temps apres passe niveau suivant)
temps/diam qui s'agite a la fin / SFX (attn : last frame only when all finished)
SFX open door
lives
start level sound 'warp'
blob
magic wall 

*/

#include <string.h>
#include <bitbox.h>
#include "lib/blitter/blitter.h"
#include "lib/chiptune/chiptune.h"

#include "bdash.h"


#define lvl_w 40
#define lvl_h 22

#define NB_OBJECTS 32

enum ObjTypes {
	obj_none, 
	obj_firefly_up, 
	obj_firefly_down, 
	obj_firefly_left, 
	obj_firefly_right, 
	obj_butterfly_left,
	obj_butterfly_right,
	obj_butterfly_up,
	obj_butterfly_down,
};
struct MovingObject {	
	uint8_t type; // none, firefly, butterfly, moving rock, rockford, ...
	uint8_t n; // up, down, left, right ...
	int pos;
};

struct Level {
	char *title;
	int points, extra; // points per diamond / extra diamond
	int diamonds; // needed diamonds to end level
	int time; // seconds
	char *msg;
	char map[22*40]; 
};

extern struct ChipSong bdash_chipsong;

extern const uint32_t clock_spr[];
extern const uint32_t diams_spr[];
extern const uint32_t rock_spr[]; // big rock

#include "levels.h" // defines levels[]


object *bg, *clock, *diams, *rock;

uint8_t vram[lvl_w*lvl_h];

int diamonds, date_death,date_start, level;
int level_time;
int level_diams;
int last_level=1;

struct MovingObject moving_objects[NB_OBJECTS];
int nb_moving_objects;

int pos_rockford;

uint16_t old_gamepad;
uint16_t gamepad_pressed;

static const uint8_t lvlmap[]={
	[' '-' ']=bdash_empty,
	['W'-' ']=bdash_block2,
	['w'-' ']=bdash_block,
	['d'-' ']=bdash_diamond,
	['.'-' ']=bdash_soil,
	['r'-' ']=bdash_rock,
	['X'-' ']=bdash_rockford_idle,
	['P'-' ']=bdash_out_closed,
	['q'-' ']=bdash_firefly,
	['B'-' ']=bdash_butterfly,
	['b'-' ']=bdash_butterfly,
};

void load_level(int l) 
{
	const struct Level *lvl=&levels[l];

	nb_moving_objects=0;
	for (int j=0;j<22;j++)
		for (int i=0;i<40;i++) {
			uint8_t c=lvlmap[lvl->map[j*40+i]-' '];
			vram[j*lvl_w+i]=c;

			if ( c==bdash_firefly || c==bdash_butterfly ) {
				struct MovingObject *m=&moving_objects[nb_moving_objects];
				m->pos=j*lvl_w+i;
				m->n=0;
				m->type=c==bdash_firefly ? obj_firefly_left : obj_butterfly_right;
				nb_moving_objects++;
			} else if (c==bdash_rockford_idle) {
				pos_rockford=j*lvl_w+i;
			}
		}

	for (int i=nb_moving_objects;i<NB_OBJECTS;i++)
		moving_objects[i].type=obj_none;

	level_time = lvl->time;
	level_diams = lvl->diamonds;
}

void start_level(int l)
{
	// load level
	level = l;
	if (last_level<level) last_level=level;

	if (l) {
		load_level(l-1);
		date_start = vga_frame;
		date_death = vga_frame+60*level_time; // time left in seconds

		clock->y=7; // show or hide clock 
		diams->y=7; // show or hide diams 

		diamonds= 0;

		// bg position
		bg->x=0;bg->y=0;
		
		while (pos_rockford%lvl_w + bg->x/32 >= 16 && bg->x>-(int)bg->w+VGA_H_PIXELS) bg->x -=32; 
		while (pos_rockford/lvl_w + bg->y/32 >= 12 && bg->y>-(int)bg->h+VGA_V_PIXELS) bg->y -=32; 

		rock->y=1024; // hide by default
	} else { // title
		memcpy(vram, bdash_tmap[level], sizeof(vram));
		bg->x=0;bg->y=0;
		clock->y=1024; // show or hide clock 
		diams->y=1024; // show or hide diams 
	}

}

void game_init() {
	blitter_init();
	bg = tilemap_new(bdash_tset, 0,0, bdash_header, &vram);
	clock = sprite_new((uint32_t*)&clock_spr, 10, 10, -1);

	diams = sprite_new((uint32_t*)&diams_spr, 50, 10, -1);

	rock = sprite_new((uint32_t*)&rock_spr, 0, 0, -1);
	rock->z = -1;

	start_level(bdash_start);
}


int is_round(int pos)
{
	if (16+vram[pos]%16==bdash_diamond || 16+vram[pos]%16==bdash_rock ) // diamond or rock
		return 1;
	else if (vram[pos]== bdash_block)
		return 1;
	return 0;
}
// make what is on the cell at pos fall if (incase of left)
// left is empty, left/down is empty, down is a rock, a block or a diamond
// left_right is +1 for right, -1 for left
// returns 1 if moved, 0 else
int fall_if_empty(int pos,int left_right, int falling_block)
{
	if (vram[pos+lvl_w+left_right] == bdash_empty && vram[pos+left_right] == bdash_empty )
		if (is_round(pos+lvl_w)) {
			vram[pos+lvl_w+left_right]=falling_block;
			vram[pos]=bdash_empty;
			return 1;
		}
	return 0;
}

int falls(int pos, int falling_block)
{
	// returns 1 if falled 
	if (vga_frame%8) return 0;
	// tombe dessous
	if (vram[pos+lvl_w] == bdash_empty) { 
		vram[pos+lvl_w]=falling_block;
		vram[pos]=bdash_empty;
		return 1;
	};

	if (fall_if_empty(pos, -1, falling_block)) 
		return 1;
	if (fall_if_empty(pos, +1, falling_block)) 
	  	return 1;
	 return 0;
}

int is_rockford(int pos) 
{
	switch (16+vram[pos]%16) {
		case bdash_rockford_idle :
		case bdash_rockford_R : 
		case bdash_rockford_L : 
		case bdash_rockford_D : 
		case bdash_rockford_U : 
			return 1;
			break;
	}
	return 0;
}

const int v[][3] = { // button, relative position, first column
	{gamepad_right,1,	bdash_rockford_R},
	{gamepad_left,-1,	bdash_rockford_L},
	{gamepad_up,-lvl_w,	bdash_rockford_U},
	{gamepad_down,lvl_w,bdash_rockford_D},
};

void explode(int pos, int wat)
{	
	for (int j=-(int)lvl_w;j<=lvl_w;j+=lvl_w)
		for (int i=pos+j-1;i<=pos+j+1;i++) {
			if (vram[i]!=bdash_block2 && vram[i] != bdash_block3)
				vram[i]=wat;
		}
}

void handle_rockford()
{

	static int moved=0; // have we moved this frame
	// try different directions

	if (pos_rockford<0) return;
	// die ?
	if (vga_frame>=date_death) {
		explode(pos_rockford, bdash_explode);			
		pos_rockford=-1;
		return;
	}

	int oldpos=pos_rockford;
	int new_rocktile=0;

	for (int i=0;i<4;i++) {
		// move ?
		if (
			((vga_frame-moved>=8) && gamepad_buttons[0]&v[i][0]) || // still pressed
			(vga_frame != moved    &&  gamepad_pressed&v[i][0]) // just pressed
			)  {

			moved=vga_frame;

			switch (vram[pos_rockford+v[i][1]])
			{
				case bdash_diamond : 
				case bdash_diamond+16 : 
				case bdash_diamond+32 : 
				case bdash_diamond+48 : 
					diamonds += 1;
					chip_note(3, 1, 4);
					pos_rockford += v[i][1];
					new_rocktile  = v[i][2];
					message("%d diamonds\n",diamonds);

					break;

				case bdash_soil : 
					chip_note(3, 1, 3);
					pos_rockford += v[i][1];
					new_rocktile  = v[i][2];
					break;

				case bdash_empty : 
					pos_rockford += v[i][1];
					new_rocktile  = v[i][2];
					break;

				case bdash_rock : 
					// _horizontal_ push=> i = 0 or 1 
					if (i<2 && vram[pos_rockford+v[i][1]*2]==bdash_empty) {
						vram[pos_rockford+v[i][1]*2]=bdash_rock;

						pos_rockford += v[i][1];
						new_rocktile  = v[i][2];

						chip_note(3, 1, 5);
					}
					break;

				case bdash_out_open :
					vram[pos_rockford]=bdash_empty;
					// Win ! make a nice animation ... / title
					start_level(level<NB_LEVELS?level+1:bdash_start);
					break;
			}

			if (pos_rockford != oldpos) {
				vram[oldpos]=bdash_empty;
				vram[pos_rockford]=new_rocktile;
			}


		} else if (vga_frame != moved && vga_frame-moved<8 && gamepad_buttons[0]&v[i][0]) { // still pressed ? animate
			vram[pos_rockford]=v[i][2]+(vga_frame-moved)/4*16;
		}
	}

	if ( vga_frame-moved>32 ) { // idle
		vram[pos_rockford]=bdash_rockford_idle+((vga_frame-moved-32)/16)%4*16;
	}

	if (pos_rockford%lvl_w + bg->x/32 >= 16 && bg->x+bg->w > VGA_H_PIXELS) bg->x-=4; 
	if (pos_rockford%lvl_w + bg->x/32 <=  3 && bg->x<0 ) bg->x+=4;

	if (pos_rockford/lvl_w + bg->y/32 >= 12 && bg->y+bg->h > VGA_V_PIXELS) bg->y-=4; 
	if (pos_rockford/lvl_w + bg->y/32 <=  2 && bg->y<0 ) bg->y+=4;

}


// structure to determine movement for common structures
const struct {
	int pos; 
	enum ObjTypes turn_id;
	int if_killed; 
} obj_movement [] = {
	[obj_firefly_left ]={    -1, obj_firefly_up,      bdash_explode},
	[obj_firefly_right]={     1, obj_firefly_down,        bdash_explode},
	[obj_firefly_down ]={ lvl_w, obj_firefly_left,     bdash_explode},
	[obj_firefly_up   ]={-lvl_w, obj_firefly_right,      bdash_explode},

	[obj_butterfly_left ]={    -1, obj_butterfly_down,    bdash_diamond},
	[obj_butterfly_right]={     1, obj_butterfly_up,  bdash_diamond},
	[obj_butterfly_down ]={ lvl_w, obj_butterfly_right,  bdash_diamond},
	[obj_butterfly_up   ]={-lvl_w, obj_butterfly_left, bdash_diamond},
};

void move_objects(void)
{
	for (int i=0;i<nb_moving_objects;i++) {
		struct MovingObject *m=&moving_objects[i];
		if (m->type==obj_none) continue;

		const int oldpos=m->pos;

		// object hit by boulder ?
	
		int next_pos;
		// check collisions each frame 
		if ( vram[m->pos-lvl_w]==bdash_rock_fall || vram[m->pos-lvl_w]==bdash_diamond_fall ) {
			// kill it 
			explode(m->pos, obj_movement[m->type].if_killed);
			m->type=obj_none;
			
		}

		if (vga_frame%4==0) {
			vram[oldpos]=vram[oldpos]%64+16; // advance animation
		}

		if (vga_frame%8==0) { // moving speed 
			next_pos = m->pos+obj_movement[m->type].pos;
			if (vram[next_pos]==bdash_empty) {
				m->pos=next_pos;
			} else if (next_pos==pos_rockford) {
				date_death=vga_frame;
				m->type = obj_none;
			} else {
				m->type = obj_movement[m->type].turn_id;
			}
		}

		if (oldpos!=m->pos) {
			vram[m->pos]=vram[oldpos];
			vram[oldpos]=bdash_empty;
		}
	}
}


void update_displays(void) 
{
	// falling displays at start
	const float os = 2.0f; // overshoot
	float t; // time 0.-1.

	if (vga_frame-date_start<32+32) {
		t = (vga_frame-date_start-32)/32.f-1.f;
		clock->y = -40 + (t*t*((os+1)*t + os)+1)*50.; //		(s+1)*t^3 - s*t^2
	}
	if (vga_frame-date_start<32+64) {
		t = (vga_frame-date_start-64)/32.f-1.f;
		diams->y = -40 + (t*t*((os+1)*t + os)+1)*50.; //		(s+1)*t^3 - s*t^2
	}

	// set to frame corresponding to level
	clock->fr = vga_frame<date_death?(vga_frame-date_start)*9/(level_time*60):8; // clock
	diams->fr = diamonds<level_diams?diamonds*9/level_diams:8; // diams

}


void game_frame()
{
	// memorize only "interesting" pos ?
	gamepad_pressed = gamepad_buttons[0] & ~old_gamepad;

	if (level==bdash_start) {
		if (!chip_song_playing())
			chip_play(&bdash_chipsong);

		vram[lvl_w*6+10]=bdash_rockford_idle+((vga_frame-32)/16)%4*16;
		int fr=((vga_frame-32)/8)%16-12;
		vram[lvl_w*6+11]=bdash_diamond+(fr<0?0:fr)*16;

		if ((vga_frame%256)<128) {
			rock->x  = (vga_frame%256)*6;
			rock->y  = 300;
			rock->fr = (vga_frame/4)%8;
		}	

		if (gamepad_pressed & (gamepad_A | gamepad_start)) {
			chip_play(0);
			start_level(last_level);
		}
		return;
	} 


	update_displays();

	handle_rockford();

	for (int pos=sizeof(vram)-1;pos>=0;pos--) { // going up
		switch (vram[pos]) { 
			case bdash_rock : 
				falls(pos, bdash_rock_fall); 
			break;
			
			case bdash_rock_fall :
				if (vga_frame%8) break;
				if (pos+lvl_w==pos_rockford)
					date_death = vga_frame; // die now

				if (!falls(pos, bdash_rock_fall)) {// free fall
					// if did not move 
					vram[pos]=bdash_rock;
					// sfx bump on ground
					chip_note(3, 1, 5);
				}
				break;


			case bdash_diamond : 
				if (vga_frame%64==0) // each 64, but stays 32 (32 is spent as shining)
					vram[pos]+=16; // next frame

				falls(pos, bdash_diamond_fall);
			break;

			case bdash_diamond+16 : 
			case bdash_diamond+32 : 
				if (vga_frame%8==0) 
					vram[pos]+=16; // next frame
				falls(pos, bdash_diamond_fall);
			break;

			case bdash_diamond_fall : // reset 
				if (pos+lvl_w==pos_rockford)
					date_death = vga_frame;
			// falls through

			case bdash_diamond+48 : 
				if (vga_frame%8==0) 
					vram[pos]=bdash_diamond; // next frame
				falls(pos, bdash_diamond_fall);
			break;

			case bdash_explode : 
			case bdash_explode+16 : 
			case bdash_explode+32 : 
			case bdash_explode+1 : 
				if (vga_frame%8==0) 
					vram[pos]+=16;
			break;

			case bdash_explode+48 : 
				if (vga_frame%8==0) 
					vram[pos]=bdash_explode+1;
			break;

			case bdash_explode+16+1 : 
				if (vga_frame%8==0) 
					vram[pos]=bdash_empty;
			break;

			case bdash_out_closed : 
				if (diamonds>=level_diams) 
					vram[pos]=bdash_out_open;
			break;

		}
    	
    }

	move_objects();

    old_gamepad = gamepad_buttons[0];

    if (vga_frame>=date_death+200) { // pre reset
    	start_level(bdash_start); // start page
    }
} 
