#include "NameGenerator.h"
#include <fstream>
#include <random>
#include <ctime>

NameGenerator::NameGenerator() {
	// Seed the random number generator
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Read words from text files into vectors
	prefixes = readWordsFromFile("res/generators/names/estates/prefixes.txt");
	midfixes = readWordsFromFile("res/generators/names/estates/midfixes.txt");
	suffixes = readWordsFromFile("res/generators/names/estates/suffixes.txt");
}

std::vector<std::string> NameGenerator::readWordsFromFile(const std::string& filename) const {
	std::vector<std::string> words;
	std::ifstream file(filename);
	if(file.is_open()) {
		std::string word;
		while(std::getline(file, word)) {
			words.push_back(word);
		}
		file.close();
	}
	return words;
}

std::string NameGenerator::generateRandomName()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> prefixDist(0, prefixes.size() - 1);
	std::uniform_int_distribution<int> midfixDist(0, midfixes.size() - 1);
	std::uniform_int_distribution<int> suffixDist(0, suffixes.size() - 1);

	std::string name;

	// Construct the name.
	name += prefixes[prefixDist(gen)];
	if(std::rand() % 2 == 0) {
		name += midfixes[midfixDist(gen)];
	}
	name += suffixes[suffixDist(gen)];

	if(usedNames.count(name) > 0)
	{
		return generateRandomName();
	}

	usedNames.insert(name);
	return name;
}