#include "FontManager.h"
#include <assert.h>

FontManager& FontManager::getInstance()
{
	static FontManager instance; // This will be created only once.
	return instance;
}

bool FontManager::loadFont(const std::string& id, const std::string& filepath)
{
	sf::Font font;
	if(!font.loadFromFile(filepath))
	{
		return false;
	}
	fonts[id] = font;
	return true;
}

const sf::Font* FontManager::getFont(const std::string& id) const
{
	auto it = fonts.find(id);
	if(it != fonts.end())
	{
		return &it->second;
	}
	assert(false);
	return nullptr;  // Texture does not exist.
}

bool FontManager::fontExists(const std::string& id) const
{
	return fonts.find(id) != fonts.end();
}

void FontManager::removeFont(const std::string& id)
{
	auto it = fonts.find(id);
	if(it != fonts.end())
	{
		fonts.erase(it);
	}
}

void FontManager::clear()
{
	fonts.clear();
}

FontManager::FontManager()
{
	std::string militaryFontPath = "res/fonts/ChunkFive-Regular.otf";
	std::string militaryFontID = "militaryFont";
	loadFont(militaryFontID, militaryFontPath);

	std::string UIFont1Path = "res/fonts/ChunkFive-Regular.otf";
	std::string UIFont1ID = "UIFont1";
	loadFont(UIFont1ID, UIFont1Path);
}
