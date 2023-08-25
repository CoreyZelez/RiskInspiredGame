#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager {
public:
	static TextureManager& getInstance();

	bool loadTexture(const std::string& id, const std::string& filepath);
	const sf::Texture* getTexture(const std::string& id) const;
	bool textureExists(const std::string& id) const;
	void removeTexture(const std::string& id);
	void clear();

private:
	TextureManager(); // Private constructor to prevent external instantiation.

	// Prevent copy construction and assignment.
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	std::map<std::string, sf::Texture> textures;
};

