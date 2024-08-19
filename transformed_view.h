#pragma once
#include "olcPixelGameEngine.h"

class TranformedView {
public:
	//olc::vf2d scale = { 1,1 };
	float scale = 1;
	olc::vf2d offset_screen = { 0,0 };
	//olc::vf2d screen_size;
	float zoom_sens = 0.1f;

	/*void init_screen_size(olc::vf2d& size) {
		screen_size = size;
		scale = size;
	}*/
	void zoom_in(olc::vf2d& mouse) {
		scale += zoom_sens;
	}

	void zoom_out(olc::vf2d& mouse) {
		scale -= zoom_sens;
	}

	void translate(olc::vf2d& direction) {
		offset_screen -= direction;
	}

	olc::vf2d screen_to_world(olc::vf2d& pos) const {
		return (pos + offset_screen) * scale;
		// world = (screen + offset) * scale
	}

	olc::vf2d world_to_screen(olc::vf2d& pos) const {
		return pos / scale - offset_screen;
	}
};