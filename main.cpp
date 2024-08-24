
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "transformed_view.h"
#include "Random.h"
#include "rect.h"
#include <chrono>
#include "quad_tree.h"

// Override base class with your custom functionality
class Window : public olc::PixelGameEngine
{
	int n_rects = 10000;
	std::vector<DrawingRect> rectangles;
	TranformedView tv;
	olc::vi2d prev_mouse;
	DynamicQuadTreeContainer<DrawingRect> dqt;

public:
	Window() : dqt(Rect())
	{
		// Name your application
		sAppName = "Window";
	}

public:
	bool OnUserCreate() override
	{
		init_random();
		// Called once at the start, so create things here
		Rect visible;
		visible.pos = { 0,0 };
		visible.size = { 100000,100000 };
		
		dqt = DynamicQuadTreeContainer<DrawingRect>(visible);
		
		for (int i = 0; i < n_rects; i++) {
			DrawingRect rect;
			rect.pos = { random() * visible.size.x, random() * visible.size.y };
			rect.size = { random() * 300, random() * 300 };
			rect.color = olc::Pixel(randint(0, 255), randint(0, 255), randint(0, 255));
			rectangles.push_back(rect);
			dqt.insert(rect, rect);
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
		if (GetKey(olc::UP).bHeld) {
			tv.zoom_in(GetMousePos());
		}
		if (GetKey(olc::DOWN).bHeld) {
			/*olc::vf2d m = GetMousePos();
			tv.zoom_out(m);*/
			tv.zoom_out(GetMousePos());
		}
		prev_mouse = GetMousePos();
		
		Clear(olc::BLACK);
		Rect screen;
		screen.pos = tv.screen_to_world({ 0,0 });
		screen.size = tv.screen_to_world(GetScreenSize()) - screen.pos;
		int c = 0;
		auto start_time = std::chrono::system_clock::now();
		for (auto& ic : dqt.search(screen)) {
			ic->data.draw(*this, tv);
			c++;
		}
		std::chrono::duration<float> time_delta = std::chrono::system_clock::now() - start_time;
		DrawString({ 0,0 }, "rectangles: " + std::to_string(c), olc::WHITE, 2U);
		DrawString({ 400, 0 }, "QuadTree: " + std::to_string(time_delta.count()), olc::WHITE, 2U);
		
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