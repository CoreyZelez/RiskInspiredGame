#include "NameGenerator.h"
#include <fstream>
#include <random>
#include <ctime>

NameGenerator::NameGenerator(const std::string &folderName)
{
	// Seed the random number generator
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	readFixes(folderName);
}

std::vector<std::string> readWordsFromFile(const std::string& filename)
{
	std::vector<std::string> words;
	std::ifstream file(filename);
	if(file.is_open()) {
		std::string word;
		while(std::getline(file, word)) 
		{
			words.push_back(word);
		}
		file.close();
	}
	return words;
}

void NameGenerator::readFixes(const std::string folderName)
{
	// Read words from text files into vectors
	prefixes = readWordsFromFile("res/generators/names/" + folderName + "/prefixes.txt");
	midfixes = readWordsFromFile("res/generators/names/" + folderName + "/midfixes.txt");
	suffixes = readWordsFromFile("res/generators/names/"+ folderName + "/suffixes.txt");
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
	if(std::rand() % 2 == 0) 
	{
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