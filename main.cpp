
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "transformed_view.h"
#include "Random.h"

class Rect {
public:
	olc::vf2d pos;
	olc::vf2d size;
	olc::Pixel color;

	bool contains(const Rect& rect) {
		return pos.x <= rect.pos.x && pos.y <= rect.pos.y && pos.x + size.x >= rect.pos.x + rect.size.x && pos.y + size.y >= rect.pos.y + rect.size.y;
	}

	bool contains(const olc::vf2d& pos2) {
		return pos.x <= pos2.x && pos.y <= pos2.y && pos.x + size.x >= pos2.x && pos.y + size.y >= pos2.y;
	}

	bool overlaps(const Rect& rect) {
		return contains(rect.pos) || contains(rect.pos + rect.size);
	}

	void draw(olc::PixelGameEngine& canvas) {
		canvas.FillRect(pos, size, color);
	}

	void draw(olc::PixelGameEngine& canvas, const TranformedView& tv) {
		olc::vi2d start = tv.world_to_screen(pos);
		auto end_world = pos + size;
		olc::vi2d end = tv.world_to_screen(end_world);
		canvas.FillRect(start, end-start, color);
	}
};

// Override base class with your custom functionality
class Window : public olc::PixelGameEngine
{
	int n_rects = 100;
	std::vector<Rect> rectangles;
	TranformedView tv;
	olc::vi2d prev_mouse;

public:
	Window()
	{
		// Name your application
		sAppName = "Window";
	}

public:
	bool OnUserCreate() override
	{
		init_random();
		// Called once at the start, so create things here
		for (int i = 0; i < n_rects; i++) {
			Rect rect;
			rect.pos = { random() * 1000, random() * 1000 };
			rect.size = { random() * 200, random() * 200 };
			rect.color = olc::Pixel(randint(0, 255), randint(0, 255), randint(0, 255));
			rectangles.push_back(rect);
		}
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		if (GetMouse(0).bHeld) {
			olc::vf2d delta = GetMousePos() - prev_mouse;
			tv.translate(delta);
		}
		if (GetKey(olc::UP).bPressed) {
			tv.zoom_in(GetMousePos());
		}
		if (GetKey(olc::DOWN).bPressed) {
			/*olc::vf2d m = GetMousePos();
			tv.zoom_out(m);*/
			tv.zoom_out(GetMousePos());
		}
		prev_mouse = GetMousePos();
		
		Clear(olc::BLACK);
		for (Rect& rect : rectangles) {
			rect.draw(*this, tv);
		}

		return true;
	}
};

int main()
{
	Window win;
	if (win.Construct(800, 800, 1, 1))
		win.Start();
	return 0;
}