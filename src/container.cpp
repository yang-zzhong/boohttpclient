
#include "container.hpp"

namespace boohttp
{
	Container::Container()
	{
	}

	void Container::set(string key, string value)
	{
		items[key] = value;
	}

	void Container::unset(string key)
	{
		has(key) && items.erase(key);
	}

	bool Container::has(string key)
	{
		return items.count(key) > 0;
	}

	string Container::value(string key)
	{
		auto v = items.find(key);
		if (v != items.end()) {
			return v->second;
		}
		return "";
	}

	void Container::merge(const Container * container)
	{
		map<string, string> items = container->all();
		for (auto item = items.begin(); item != items.end(); item++) {
			items[item->first] = item->second;
		}
	}

	const map<string, string> & Container::all() const
	{
		return items;
	}

	Container::~Container()
	{
		items.erase(items.begin(), items.end());
	}

	string Container::combine(vector<string> values)
	{
		return "";
	}
}
