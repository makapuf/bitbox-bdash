/* TODO 
sons
joystick max shoot

fin level (FX - tt ce qui n'est pas rocher explose, pause et qq temps apres passe niveau suivant)
rock qui roule : commencer a G de l'ecran, varier les hauteurs a chaque fois 
temps/diam qui s'agite a la fin
affiche 
decalage bg changement ecran
load levels

start level sound 'warp)'

fireflies
(blob?)
level design
pierre tombe sur firefly
magic wall ?
bomb 
+ petits niveaux ?

*/

#include <bitbox.h>
#include <blitter.h>
#include <string.h>
#include <sampler.h>

#include "bdash.h"

#define lvl_w 128
#define lvl_h 64

extern const uint32_t clock_spr[];
extern const uint32_t diams_spr[];
extern const uint32_t rock_spr[]; // big rock

extern const int8_t snd_diam_raw[], snd_dig1_raw[], snd_music1_raw[];
extern const unsigned int snd_diam_raw_len, snd_dig1_raw_len, snd_music1_raw_len;

const int nb_levels=2;
const int level_time[] =  {0,150,100};
const int level_diams[] = {0, 12, 20};

object *bg, *clock, *diams, *rock;

uint8_t vram[lvl_w*lvl_h];

int diamonds, date_death,date_start, level;

void start_level(int l)
{
	// load level
	level = l;
	memcpy(vram, bdash_tmap[level], sizeof(vram));
	
	date_start = vga_frame;
	date_death = vga_frame+60*level_time[level]; // time left in seconds

	clock->y=level==bdash_start?-30:7; // show or hide clock 
	diams->y=level==bdash_start?-30:7; // show or hide diams 

	diamonds= 0;

	bg->x=0;bg->y=0;

	rock->y=1024; // hide by default
}

void game_init() {
	blitter_init();
	bg = tilemap_new(bdash_tset, lvl_w*32, lvl_h*32, bdash_header, &vram);
	clock = sprite_new((uint32_t*)&clock_spr, 10, 10, -1);

	diams = sprite_new((uint32_t*)&diams_spr, 50, 10, -1);

	rock = sprite_new((uint32_t*)&rock_spr, 0, 0, -1);
	rock->z = -1;

	start_level(bdash_start);
}


int is_round(int pos)
{
	if (vram[pos]%16==bdash_diamond || vram[pos]%16==bdash_rock ) // diamond or rock
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

int is_rockford(int pos) {

	switch (vram[pos]%16) {
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

void explode(int pos)
{	
	for (int j=-(int)lvl_w;j<=lvl_w;j+=lvl_w)
		for (int i=pos+j-1;i<=pos+j+1;i++)
		{
			if (vram[i]!=bdash_block && vram[i]!=bdash_block2 && vram[i] != bdash_block3)
				vram[i]=bdash_explode;
		}
}


uint16_t old_gamepad;
uint16_t gamepad_pressed;

void handle_rockford(int pos)
{
	static int moved=0; // have we moved this frame
	// try different directions

	// die ?
	if (vga_frame>=date_death)  {
		explode(pos);
		return;
	}

	for (int i=0;i<4;i++) {
		// move ?
		if (
			((vga_frame-moved>=16) && gamepad_buttons[0]&v[i][0]) || // still pressed
			(vga_frame != moved    &&  gamepad_pressed&v[i][0]) // just pressed
			)  {

			moved=vga_frame;

			switch (vram[pos+v[i][1]])
			{
				case bdash_diamond : 
				case bdash_diamond+16 : 
				case bdash_diamond+32 : 
				case bdash_diamond+48 : 
					diamonds += 1;
					play_sample(snd_diam_raw, snd_diam_raw_len, 256, -1,40,40);
				
					vram[pos+v[i][1]] = v[i][2];
					vram[pos] = bdash_empty;
					break;

				case bdash_soil : 
					play_sample(snd_dig1_raw, snd_dig1_raw_len, 256, -1,40,40);
					vram[pos+v[i][1]] = v[i][2];
					vram[pos] = bdash_empty;
					break;

				case bdash_empty : 
					vram[pos+v[i][1]] = v[i][2];
					vram[pos] = bdash_empty;
				break;

				case bdash_rock : 
					// _horizontal_ push=> i = 0 or 1 
					if (i<2 && vram[pos+v[i][1]*2]==bdash_empty) 
					{
						vram[pos] = bdash_empty;
						vram[pos+v[i][1]]=v[i][2];
						vram[pos+v[i][1]*2]=bdash_rock;
					}
				break;
				case bdash_out_open :
					vram[pos]=bdash_empty;
					// Win ! make a nice animation ...
					start_level(level<nb_levels?level+1:bdash_start);

				break;
			}


		// shift background accordingly
		if (pos%lvl_w + bg->x/32 >= 16 && bg->x>-(int)bg->w+VGA_H_PIXELS) bg->x -=32; 
		if (pos%lvl_w + bg->x/32 <= 3 && bg->x<0 ) bg->x +=32;
		if (pos/lvl_w + bg->y/32 >= 12 ) bg->y -=32; // && bg->x>bg->w-VGA_V_PIXELS
		if (pos/lvl_w + bg->y/32 <=2 && bg->y<0 ) bg->y +=32;


		} else if (vga_frame != moved && vga_frame-moved<16 && gamepad_buttons[0]&v[i][0]) { // still pressed ? animate
			vram[pos]=v[i][2]+(vga_frame-moved)/4*16;
		}
	}

	if ( vga_frame-moved>32 ) { // idle
		vram[pos]=bdash_rockford_idle+((vga_frame-moved-32)/16)%4*16;
	}
}

void death() 
// makes player die
{
	message("aargh\n");
	start_level(0); // restart
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
	clock->fr = vga_frame<date_death?(vga_frame-date_start)*9/(level_time[level]*60):8; // clock
	diams->fr = diamonds<level_diams[level]?diamonds*9/level_diams[level]:8; // diams

}


void game_frame()
// memorize only "interesting" pos ?

{
	kbd_emulate_gamepad();
	gamepad_pressed = gamepad_buttons[0] & ~old_gamepad;
	static int snd_handle = -1;

	if (level==bdash_start) {
		if (snd_handle<0) // launch looping sample
			snd_handle = play_sample(snd_music1_raw, snd_music1_raw_len,250,0, 200,200);

		vram[lvl_w*6+10]=bdash_rockford_idle+((vga_frame-32)/16)%4*16;
		int fr=((vga_frame-32)/8)%16-12;
		vram[lvl_w*6+11]=bdash_diamond+(fr<0?0:fr)*16;

		if ((vga_frame%256)<128) {
			rock->x  = (vga_frame%256)*6;
			rock->y  = 300;
			rock->fr = (vga_frame/4)%8;
		}	


		if (gamepad_pressed & (gamepad_A | gamepad_start)) {
			stop_sample(snd_handle);
			start_level(1);
		}
		return;
	} 


	update_displays();

	for (int pos=sizeof(vram)-1;pos>=0;pos--) { // en remontant ...
		if (is_rockford(pos)) { 
			handle_rockford(pos);
		} else switch (vram[pos]) { 
			case bdash_rock : 
				falls(pos, bdash_rock_fall); 
			break;
			
			case bdash_rock_fall :
				if (vga_frame%8) break;
				if (is_rockford(pos+lvl_w))
					date_death = vga_frame; // die now

				if (!falls(pos, bdash_rock_fall)) // free fall
					// if did not move 
					vram[pos]=bdash_rock;
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
				if (is_rockford(pos+lvl_w))
					date_death = vga_frame;
					
				// falls through
			case bdash_diamond+48 : 
				if (vga_frame%8==0) 
					vram[pos]=bdash_diamond; // next frame
				falls(pos, bdash_diamond_fall);
			break;

			case bdash_explode : 
			case bdash_explode+1 : 
			case bdash_explode+2 : 
			case bdash_explode+3 : 
			case bdash_explode+4 : 
				if (vga_frame%8==0) 
					vram[pos]+=1;
			break;
			case bdash_explode+5 : 
				if (vga_frame%16==0)
					vram[pos]=bdash_empty;
			break;

			case bdash_out_closed : 
				if (diamonds>=level_diams[level]) 
					vram[pos]=bdash_out_open;
			break;

		}
    	
    }
    old_gamepad = gamepad_buttons[0];

    if (vga_frame>=date_death+200) {
    	start_level(bdash_start); // start page
    }
} 
