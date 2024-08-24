#pragma once
#include "olcPixelGameEngine.h"
#include "transformed_view.h"


class Rect {
public:
	Rect(const olc::vf2d& pos, const olc::vf2d& size) : pos(pos), size(size) {}
	Rect() {}

	olc::vf2d pos;
	olc::vf2d size;

	bool contains(const Rect& rect) {
		return pos.x <= rect.pos.x && pos.y <= rect.pos.y && pos.x + size.x >= rect.pos.x + rect.size.x && pos.y + size.y >= rect.pos.y + rect.size.y;
	}

	bool contains(const olc::vf2d& pos2) {
		return pos.x <= pos2.x && pos.y <= pos2.y && pos.x + size.x >= pos2.x && pos.y + size.y >= pos2.y;
	}

	bool overlaps(const Rect& other) {
		return pos.x + size.x >= other.pos.x && pos.x <= other.pos.x + other.size.x && pos.y + size.y >= other.pos.y && pos.y < other.pos.y + other.size.y;
	}

	Rect screen_to_world(TranformedView& tv) {
		auto tl = tv.screen_to_world(pos);
		auto br = tv.screen_to_world(pos + size);
		return Rect(tl, br - tl);
	}
};

class DrawingRect : public Rect {
public:
	olc::Pixel color;

	using Rect::Rect;

	void draw(olc::PixelGameEngine& canvas) {
		canvas.FillRect(pos, size, color);
	}

	void draw(olc::PixelGameEngine& canvas, const TranformedView& tv) {
		olc::vi2d start = tv.world_to_screen(pos);
		auto end_world = pos + size;
		olc::vi2d end = tv.world_to_screen(end_world);
		canvas.FillRect(start, end - start, color);
	}
};