#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class FontManager {
public:
	static FontManager& getInstance();

	bool loadFont(const std::string& id, const std::string& filepath);
	const sf::Font* getFont(const std::string& id) const;
	bool fontExists(const std::string& id) const;
	void removeFont(const std::string& id);
	void clear();

private:
	FontManager(); // Private constructor to prevent external instantiation.

	// Prevent copy construction and assignment.
	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;

	std::map<std::string, sf::Font> fonts;
};

