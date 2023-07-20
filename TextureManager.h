#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager {
public:
	~TextureManager();

	bool loadTexture(const std::string& id, const std::string& filename);
	const sf::Texture* getTexture(const std::string& id) const;
	bool textureExists(const std::string& id) const;
	void removeTexture(const std::string& id);
	void clear();

private:
	std::map<std::string, sf::Texture> textures;
};