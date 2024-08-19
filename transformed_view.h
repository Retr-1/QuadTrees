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
	void zoom_in(const olc::vf2d& mouse) {
		auto mouse_world_before = screen_to_world(mouse);
		scale -= zoom_sens;
		auto mouse_screen_after = world_to_screen(mouse_world_before);
		translate(mouse-mouse_screen_after);
	}

	void zoom_out(const olc::vf2d& mouse) {
		auto mouse_world_before = screen_to_world(mouse);
		scale += zoom_sens;
		auto mouse_screen_after = world_to_screen(mouse_world_before);
		translate(mouse - mouse_screen_after);
	}

	void translate(const olc::vf2d& direction) {
		offset_screen -= direction;
	}

	olc::vf2d screen_to_world(const olc::vf2d& pos) const {
		return (pos + offset_screen) * scale;
		// world = (screen + offset) * scale
	}

	olc::vf2d world_to_screen(const olc::vf2d& pos) const {
		return pos / scale - offset_screen;
	}
};