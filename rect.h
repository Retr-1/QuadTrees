#pragma once
#include "olcPixelGameEngine.h"
#include "transformed_view.h"

class Rect {
public:
	Rect(olc::vf2d& pos, olc::vf2d& size) : pos(pos), size(size) {}
	Rect() {}

	olc::vf2d pos;
	olc::vf2d size;

	bool contains(const Rect& rect) {
		return pos.x <= rect.pos.x && pos.y <= rect.pos.y && pos.x + size.x >= rect.pos.x + rect.size.x && pos.y + size.y >= rect.pos.y + rect.size.y;
	}

	bool contains(const olc::vf2d& pos2) {
		return pos.x <= pos2.x && pos.y <= pos2.y && pos.x + size.x >= pos2.x && pos.y + size.y >= pos2.y;
	}

	bool overlaps(const Rect& rect) {
		return contains(rect.pos) || contains(rect.pos + rect.size);
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