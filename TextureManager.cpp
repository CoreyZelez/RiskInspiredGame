#include "TextureManager.h"
#include <assert.h>

TextureManager::~TextureManager() 
{
	for(auto& pair : textures) 
	{
		pair.second.~Texture();
	}
}

bool TextureManager::loadTexture(const std::string& id, const std::string& filename) 
{
	sf::Texture texture;
	if(!texture.loadFromFile(filename)) 
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