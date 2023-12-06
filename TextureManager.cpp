#include "TextureManager.h"
#include <assert.h>

TextureManager& TextureManager::getInstance()
{
	static TextureManager instance; // This will be created only once.
	return instance;
}

bool TextureManager::loadTexture(const std::string& id, const std::string& filepath)
{
	sf::Texture texture;
	if(!texture.loadFromFile(filepath))
	{
		return false;
	}
	assert(!textureExists(id));  // id should not already exist.
	textures[id] = texture;
	return true;
}

const sf::Texture* TextureManager::getTexture(const std::string& id) const
{
	auto it = textures.find(id);
	if(it != textures.end())
	{
		return &it->second;
	}
	assert(false);
	return nullptr;  // Texture does not exist.
}

bool TextureManager::textureExists(const std::string& id) const
{
	return textures.find(id) != textures.end();
}

void TextureManager::removeTexture(const std::string& id)
{
	auto it = textures.find(id);
	if(it != textures.end())
	{
		textures.erase(it);
	}
}

void TextureManager::clear()
{
	textures.clear();
}

TextureManager::TextureManager()
{
	bool result;

	std::string landArmyPath = "res/textures/land army texture.png";
	std::string landArmyID = "landArmy";
	result = loadTexture(landArmyID, landArmyPath);
	assert(result);

	std::string portPath = "res/textures/anchor texture.png";
	std::string portID = "anchor";
	result = loadTexture(portID, portPath);
	assert(result);
}
