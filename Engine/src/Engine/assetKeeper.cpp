#include "assetKeeper.h"

#include "resource.h"

AssetKeeper::AssetKeeper()
{

	category = 0;
	selection = 0;

}

AssetKeeper::~AssetKeeper() {

	database.clear();

}

void AssetKeeper::archive(const int& category, Resources* resource) {

	database.insert({ category, resource });

}

void AssetKeeper::dump(const int& category) {

	database.erase(category);

}

void AssetKeeper::remove() {

	iterator = database.find(category);

	std::advance(iterator, selection);

	database.erase(iterator);

}

void AssetKeeper::ImmidiateRemove(const int& category, const int& selection) {

	iterator = database.find(category);

	std::advance(iterator, selection);

	database.erase(iterator);

}

void AssetKeeper::browse(const int& category) {

	this->category = category;

}

void AssetKeeper::select(const int& selection) {

	this->selection = selection;

}

Resources* const AssetKeeper::view() {

	iterator = database.find(category);

	std::advance(iterator, selection);

	return iterator->second;

}

Resources* const AssetKeeper::immidiateView(const int& category, const int& selection) {

	iterator = database.find(category);

	std::advance(iterator, selection);

	return iterator->second;

}

std::vector<Resources*> AssetKeeper::viewCategory() {

	std::vector<Resources*> range;

	auto iterator = database.equal_range(category);

	for (auto i = iterator.first; i != iterator.second; ++i) {

		range.push_back(i->second);

	}

	return range;

}
