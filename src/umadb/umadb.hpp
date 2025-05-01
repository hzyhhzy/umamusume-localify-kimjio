#pragma once
#include <unordered_map>

namespace UmaDatabase {

	void setBundleHandleTargetCache(std::wstring bundlePath, void* target);
	void* getBundleHandleTargetCache(std::wstring bundlePath, bool isDbPath=false);
	int get_head_id_from_dress_id(int dressId);
	bool get_dress_have_mini(int dressId);

	void executeQuerySkillData();

	extern std::unordered_map<int, std::unordered_map<std::string, int>> umaSkillInfo;
}