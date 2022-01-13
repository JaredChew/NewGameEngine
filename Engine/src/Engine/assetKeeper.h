#pragma once

#include <unordered_map>
#include <vector>

class Resources;

class AssetKeeper {

private:

	std::unordered_multimap<int, Resources*> database;
	std::unordered_multimap<int, Resources*>::iterator iterator;

	int category;
	int selection;

public:
	AssetKeeper();
	~AssetKeeper();

	void archive(const int& category, Resources* resource);
	void dump(const int& category);
	void remove();
	void ImmidiateRemove(const int& category, const int& selection);

	void browse(const int& category);
	void select(const int& selection);

	Resources* const view();
	Resources* const immidiateView(const int& category, const int& selection);

	std::vector<Resources*> viewCategory();

};