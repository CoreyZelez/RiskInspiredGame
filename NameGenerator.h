#pragma once
#include <vector>
#include <string>
#include <set>

class NameGenerator 
{
public:
	NameGenerator(); // Constructor reads files internally.
	std::string generateRandomName();

private:
	std::vector<std::string> prefixes;
	std::vector<std::string> midfixes;
	std::vector<std::string> suffixes;

	std::vector<std::string> readWordsFromFile(const std::string& filename) const;
	std::set<std::string> usedNames;
};
