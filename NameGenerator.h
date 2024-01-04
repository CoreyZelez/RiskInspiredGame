#pragma once
#include <vector>
#include <string>
#include <set>

class NameGenerator
{
public:
	NameGenerator(const std::string &folderName);

	void readFixes(const std::string folderName);
	std::string generateRandomName();

private:

	std::vector<std::string> prefixes;
	std::vector<std::string> midfixes;
	std::vector<std::string> suffixes;

	std::set<std::string> usedNames;
};

std::vector<std::string> readWordsFromFile(const std::string& filename);



