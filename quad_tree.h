#pragma once
#include "rect.h"
#include <array>
#include <memory>
#include <list>
#include <unordered_map>

constexpr size_t max_depth = 10;

template <typename Data>
class StaticQuadTree {

	struct Item {
		Data data;
		Rect area;
	};

	std::array<Rect, 4> areas;
	std::array<std::unique_ptr<StaticQuadTree<Data>>, 4> children{};
	std::vector<Item> items;
	size_t depth = 0;
	

	void search(Rect& area, std::vector<Data>& data) {
		for (int i = 0; i < 4; i++) {
			if (children[i])
			{
				if (area.contains(areas[i]))
					children[i]->dump_items(data);

				else if (area.overlaps(areas[i]))
					children[i]->search(area, data);
			}
		}

		for (Item& m : items) {
			if (area.overlaps(m.area))
				data.push_back(m.data);
		}
	}

	void dump_items(std::vector<Data>& data) {
		for (Item& itm : items) {
			data.push_back(itm.data);
		}
		for (int i = 0; i < 4; i++) {
			if (children[i])
				children[i]->dump_items(data);
		}
	}

public:

	StaticQuadTree(const Rect& area, size_t depth) : depth(depth) {
		olc::vf2d half_size = area.size / 2;
		areas[0].pos = area.pos;
		areas[1].pos = { area.pos.x + half_size.x, area.pos.y };
		areas[2].pos = { area.pos.x, area.pos.y + half_size.y };
		areas[3].pos = area.pos + half_size;

		for (int i = 0; i < 4; i++) {
			areas[i].size = half_size;
		}
	}

	std::vector<Data> search(Rect& area) {
		std::vector<Data> data;
		search(area, data);
		return data;
	}

	void insert(Rect& area, Data& data) {
		if (depth < max_depth) 
		{
			for (int i = 0; i < 4; i++) {
				if (areas[i].contains(area)) {
					if (!children[i]) {
						children[i] = std::make_unique<StaticQuadTree<Data>>(areas[i], depth + 1);
					}
					return children[i]->insert(area, data);
				}
			}
		}

		items.push_back({ data, area });
	}
};

template <typename Data>
class StaticQuadTreeContainer {
	std::list<Data> items;
	StaticQuadTree<Data*> sqt;

public:
	StaticQuadTreeContainer(const Rect& area) : sqt(area, 0) {}
	//StaticQuadTreeContainer(Rect&& area) : sqt(area, 0) {}

	std::vector<Data*> search(Rect& area) {
		return sqt.search(area);
	}

	void insert(Rect& area, Data& data) {
		items.push_back(data);
		Data* element = &items.back();
		sqt.insert(area, element);
	}
};

template <typename Data>
struct Item {
	Data data;
	Rect area;
};

template <typename Item>
struct ItemLocation {
	std::list<Item>* container;
	std::list<Item>::iterator position;
};

template <typename Data>
class DynamicQuadTree {
public:
	typedef Item<Data> Item;
	typedef ItemLocation<Item> ItemLocation;

private:
	std::array<Rect, 4> areas;
	std::array<std::unique_ptr<DynamicQuadTree<Data>>, 4> children{};
	std::list<Item> items;
	size_t depth = 0;


	void search(Rect& area, std::vector<Data>& data) {
		for (int i = 0; i < 4; i++) {
			if (children[i])
			{
				if (area.contains(areas[i]))
					children[i]->dump_items(data);

				else if (area.overlaps(areas[i]))
					children[i]->search(area, data);
			}
		}

		for (Item& m : items) {
			if (area.overlaps(m.area))
				data.push_back(m.data);
		}
	}

	void dump_items(std::vector<Data>& data) {
		for (Item& itm : items) {
			data.push_back(itm.data);
		}
		for (int i = 0; i < 4; i++) {
			if (children[i])
				children[i]->dump_items(data);
		}
	}

public:

	DynamicQuadTree(const Rect& area, size_t depth) : depth(depth) {
		olc::vf2d half_size = area.size / 2;
		areas[0].pos = area.pos;
		areas[1].pos = { area.pos.x + half_size.x, area.pos.y };
		areas[2].pos = { area.pos.x, area.pos.y + half_size.y };
		areas[3].pos = area.pos + half_size;

		for (int i = 0; i < 4; i++) {
			areas[i].size = half_size;
		}
	}

	std::vector<Data> search(Rect& area) {
		std::vector<Data> data;
		search(area, data);
		return data;
	}

	 ItemLocation insert(const Rect& area, const Data& data) {
		if (depth < max_depth)
		{
			for (int i = 0; i < 4; i++) {
				if (areas[i].contains(area)) {
					if (!children[i]) {
						children[i] = std::make_unique<DynamicQuadTree<Data>>(areas[i], depth + 1);
					}
					return children[i]->insert(area, data);
				}
			}
		}

		items.push_back({ data, area });
		return { &items, std::prev(items.end()) };
	}
};


template <typename Data>
class DynamicQuadTreeContainer {
public:
	struct ItemContainer {
		Data data;
		std::list<ItemContainer>::iterator itr;
		DynamicQuadTree<ItemContainer*>::ItemLocation dqt_loc;
	};

private:
	std::list<ItemContainer> items;
	DynamicQuadTree<ItemContainer*> dqt;

public:
	DynamicQuadTreeContainer(const Rect& area) : dqt(area, 0) {}

	std::vector<ItemContainer*> search(Rect& area) {
		return dqt.search(area);
	}

	void insert(Rect& area, Data& data) {
		items.push_back(ItemContainer());
		ItemContainer& ic = items.back();
		ic.data = data;
		ic.itr = std::prev(items.end());
		const DynamicQuadTree<ItemContainer*>::ItemLocation& dqt_loc = dqt.insert(area, &ic);
		ic.dqt_loc = dqt_loc;
	}

	void remove(ItemContainer& ic) {
		ic.dqt_loc.container->erase(ic.dqt_loc.position);
		items.erase(ic.itr);
	}
};