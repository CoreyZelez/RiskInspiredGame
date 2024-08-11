#include "EstateMaker.h"
#include "InputUtility.h"
#include "EstateManager.h"
#include "NameGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <random>

EstateMaker::EstateMaker()
	: nameGenerator("estates")
{

}

void EstateMaker::load(std::string mapName, std::deque<EditorTerritory>& territories)
{
	std::ifstream file("res/maps/" + mapName + "/" + mapName + "_estates.txt");
	std::string line;

	while (std::getline(file, line))
	{
		if (line.size() == 0)  // Line is blank.
		{
			continue;
		}
		else if (line.compare(estateSaveLabel) == 0)
		{
			loadEstate(file);
		}
		else if (line.compare(baronySaveLabel) == 0)
		{
			loadBarony(file, territories);
		}
		else if (line.compare(maridomSaveLabel) == 0)
		{
			loadMaridom(file, territories);
		}
	}
}

void EstateMaker::save(std::string mapName) const
{
	std::ofstream file("res/maps/" + mapName + "/" + mapName + "_estates.txt");

	// Saves estates in order of title prestige.
	std::vector<Title> titles = getOrderedTitles();
	for (Title title : titles)
	{
		for (auto iter = estates.cbegin(); iter != estates.cend(); ++iter)
		{
			// Check that the key matches the title.
			if (iter->first != title)
			{
				continue;
			}
			// Save the estate.
			for (const EditorEstate& estate : iter->second)
			{
				estate.saveToFile(file);
				file << std::endl;  // Gap in file signifys end of individual estate info.
			}
		}
	}
}

void EstateMaker::draw(sf::RenderWindow &window)
{
	if(state == EstateMakerState::none)
	{
		drawEstates(window);
		//estateManager.draw(window);
	}
	else if(state == EstateMakerState::selectEstate)
	{
		drawEstates(window, selectedTitle);

		//estateManager.draw(window, selectedTitle);
	}
	else if(state == EstateMakerState::editEstate)
	{
		// MUST IMPLEMENT!
		///estateManager.draw(window, selectedTitle - 1);

		drawEstates(window, selectedTitle);

		//estateManager.draw(window, selectedTitle);
		selectedEstate->draw(window);
	}
}

void EstateMaker::handleInput(const sf::RenderWindow &window, sf::View &view)
{
	InputUtility &inputUtility = InputUtility::getInstance();

	handleInputForView(view);

	if(inputUtility.getButtonDown(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Select estate for modification.
		if(state == EstateMakerState::selectEstate)
		{
			// Selects estate with selectedTitle. Can have a parent estate.
			selectedEstate = getEstate(worldPos, selectedTitle, true);
			changeState(EstateMakerState::editEstate);
		}
		// Removes subfiefs from estate.
		else if(state == EstateMakerState::editEstate)
		{
			EditorEstate *subfief = getLowerEstate(worldPos, selectedTitle, true);

			// Remove subFief if it is a sub fief of estate. 
			if(subfief != nullptr)
			{
				selectedEstate->removeSubfief(subfief);
			}
		}
	}
	else if(inputUtility.getButtonDown(sf::Mouse::Left))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		// Add subfiefs to estate.
		if(state == EstateMakerState::editEstate)
		{
			// Adds parentless and lower title estate at mouse position as a subfief.
			EditorEstate *subfief = getLowerEstate(worldPos, selectedTitle, false);
			if(subfief != nullptr)
			{
				assert(subfief->hasParent() == false);
				selectedEstate->addSubfief(subfief);
			}
		}
		else if(state == EstateMakerState::editColors)
		{
			////////////////////////////////////////////////////////////////////////////////
			// NO INDIVIDUAL ESTATE COLOR EDITING CURRENTLY.
			// Estate *estate = getEstate(worldPos, selectedTitle, false);
			////////////////////////////////////////////////////////////////////////////////
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num1))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::barony;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num2))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::county;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num3))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::duchy;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num4))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::kingdom;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Num5))
	{
		if(state == EstateMakerState::none || state == EstateMakerState::selectEstate)
		{
			selectedTitle = Title::empire;
			changeState(EstateMakerState::selectEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::R))
	{
		if(state == EstateMakerState::selectEstate)
		{
			selectedEstate = &createEstate(selectedTitle);
			changeState(EstateMakerState::editEstate);
		}
	}
	else if(inputUtility.getKeyPressed(sf::Keyboard::Enter))
	{
		if(state == EstateMakerState::editEstate)
		{
			selectedEstate = nullptr;
			changeState(EstateMakerState::none);
		}
	}
	else if(inputUtility.getKeyDown(sf::Keyboard::O) && inputUtility.getKeyPressed(sf::Keyboard::P))
	{
		if(selectedTitle != Title::noTitle)
		{
			randomiseTitleColor(selectedTitle);
		}
	}

	inputClock.restart();
}

void EstateMaker::changeState(EstateMakerState state)
{
	if(state == EstateMakerState::selectEstate)
	{
		makeColored(selectedTitle, false);
	}
	else if(state == EstateMakerState::editEstate)
	{
		makeColored(selectedTitle, true);  

		// Change the color of selected estate to be unique of other estates.
		const sf::Color selectedEstateColor(240, 240, 0);
		selectedEstate->getGrid().setColor(selectedEstateColor);
	}

	this->state = state;
}

void EstateMaker::handleInputForView(sf::View &view) const
{
	InputUtility &inputUtility = InputUtility::getInstance();

	// Handle view movement.
	const float speed = 3000.0f;
	const float effectiveSpeed = speed * inputClock.getElapsedTime().asSeconds();
	if(inputUtility.getKeyDown(sf::Keyboard::W))
	{
		view.move(0, -effectiveSpeed);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::A))
	{
		view.move(-effectiveSpeed, 0);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::S))
	{
		view.move(0, effectiveSpeed);
	}
	if(inputUtility.getKeyDown(sf::Keyboard::D))
	{
		view.move(effectiveSpeed, 0);
	}

	// Handle view zoom.
	const float zoom = 0.1f;
	if(inputUtility.getMouseScrollDirection() == 1)
	{
		view.zoom(1 - zoom);
	}
	else if(inputUtility.getMouseScrollDirection() == -1)
	{
		view.zoom(1 + zoom);
	}
}

void EstateMaker::loadEstate(std::ifstream& file)
{
	std::string line;

	// Load title.
	std::getline(file, line);
	assert(line.compare("# title") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	Title title = static_cast<Title>(std::stoi(line));

	// Load name.
	std::string name = loadName(file);

	// Load color.
	sf::Color color = loadColor(file);

	// Load the names of the subfiefs.
	std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Create the estate.
	estates[title].emplace_back(EditorEstate(title, color));
	EditorEstate &estate = estates[title].back();
	estate.initName(name);

	// Add the subfiefs.
	bool hasSubfief = false;
	for (std::string& subfiefName : subfiefNames)
	{
		EditorEstate* subfief = getFief(subfiefName);

		// Subfief no longer exists.
		if (subfief == nullptr)
		{
			continue;
		}

		estate.addSubfief(subfief);
		hasSubfief = true;
	}

	// Remove estate if has no subfiefs.
	if (!hasSubfief)
	{
		estates[title].pop_back();
	}
}

void EstateMaker::loadBarony(std::ifstream& file, std::deque<EditorTerritory>& territories)
{
	std::string line;

	// Load title.
	std::getline(file, line);
	assert(line.compare("# title") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	assert(static_cast<Title>(std::stoi(line)) == Title::barony);
	Title title = static_cast<Title>(std::stoi(line));

	// Load barony name.
	const std::string name = loadName(file);

	// Load barony color.
	const sf::Color color = loadColor(file);

	// Load names of subfiefs of barony.
	const std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Load territory.
	std::getline(file, line);  // Label is traversed by loadSubfiefNames functions.
	const int territoryID = std::stoi(line);
	EditorTerritory* territory = nullptr;
	for (EditorTerritory& landTerritory : territories)
	{
		if (landTerritory.getType() != TerritoryType::land)
		{
			continue;
		}

		// Territory already allocated to a landed estate.
		if (landTerritory.getID() == territoryID)
		{
			assert(allocatedTerritoryIDs.count(territoryID) == 0);
			allocatedTerritoryIDs.insert(territoryID);  // Track territory id so reconcileBaronies does not create new barony with same territory.
			territory = &landTerritory;
			break;
		}
	}

	// Territory was removed so barony no longer exists.
	if (territory == nullptr)
	{
		return;
	}

	// Create the barony.
	EditorEstate barony(Title::barony, territory, color);
	barony.initName(name);

	// Add the subfiefs.
	for (const std::string& subfiefName : subfiefNames)
	{
		barony.addSubfief(getFief(subfiefName));
	}

	// Add the barony to estates.
	estates[Title::barony].emplace_back(std::move(barony));
}

void EstateMaker::loadMaridom(std::ifstream& file, std::deque<EditorTerritory>& territories)
{
	std::string line;

	// Load title.
	std::getline(file, line);
	assert(line.compare("# title") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	assert(static_cast<Title>(std::stoi(line)) == Title::maridom);
	Title title = static_cast<Title>(std::stoi(line));

	// Load maridom name.
	const std::string name = loadName(file);

	// Load maridom color.
	const sf::Color color = loadColor(file);

	// Load names of subfiefs of maridom.
	const std::vector<std::string> subfiefNames = loadSubfiefNames(file);

	// Load territory.
	std::getline(file, line);  // Label is traversed by loadSubfiefNames functions.
	const int territoryID = std::stoi(line);
	EditorTerritory* territory = nullptr;
	for (EditorTerritory& navalTerritory : territories)
	{
		if (navalTerritory.getType() != TerritoryType::naval)
		{
			continue;
		}

		// Territory already allocated to a landed estate.
		if (navalTerritory.getID() == territoryID)
		{
			assert(allocatedTerritoryIDs.count(territoryID) == 0);

			allocatedTerritoryIDs.insert(territoryID);  // Track territory id so reconcileBaronies does not create new barony with same territory.
			territory = &navalTerritory;

			break;
		}
	}

	// Territory was removed so maridom no longer exists.
	if (territory == nullptr)
	{
		return;
	}

	// Create the maridom.
	EditorEstate maridom(Title::maridom, territory, color);
	maridom.initName(name);
	// Add the subfiefs.
	for (const std::string& subfiefName : subfiefNames)
	{
		maridom.addSubfief(getFief(subfiefName));
	}
	// Add the barony to estates.
	estates[Title::maridom].emplace_back(std::move(maridom));
}

EditorEstate* EstateMaker::getFief(std::string name)
{
	for (auto title : getOrderedTitles())
	{
		for (EditorEstate& estate : estates[title])
		{
			if (estate.getName().compare(name) == 0)
			{
				return &estate;
			}
		}
	}

	return nullptr;
	// MAYBE THIS SHOULD BE DONE IN FUTURE
	//throw std::logic_error("Fief does not exist.");
}

void EstateMaker::drawEstates(sf::RenderWindow& window, Title title) const
{
	// Draw all estates not of given title. 
	for (const auto& [currTitle, titleEstates] : estates)
	{
		if (currTitle == title)
		{
			continue;
		}

		for (const EditorEstate& estate : titleEstates)
		{
			estate.draw(window);
		}
	}

	// Draw estates of given title.
	for (const auto& [currTitle, titleEstates] : estates)
	{
		if (currTitle != title)
		{
			continue;
		}

		for (const EditorEstate& estate : titleEstates)
		{
			estate.draw(window);
		}
	}
}

EditorEstate* EstateMaker::getEstate(sf::Vector2f position, Title title, bool allowParent)
{
	for (EditorEstate& estate : estates[title])
	{
		if ((allowParent || !estate.hasParent()) && estate.getGrid().containsPosition(position))
		{
			return &estate;
		}
	}

	return nullptr;
}

EditorEstate* EstateMaker::getLowerEstate(sf::Vector2f position, Title title, bool allowParent)
{
	// Following code problematic if more titles equal in prominence to Title::barony are added.
	assert(static_cast<int>(Title::barony) == 1);

	Title underTitle = title - 1;  // Title directly under parameter title.

	// OVERLOAD ++ TO INCREMENT TITLE NOT BASED ON INT. SHOULD BE EZ.

	for (Title currTitle = underTitle; currTitle >= Title::barony; currTitle--)
	{
		for (EditorEstate& estate : estates[currTitle])
		{
			if ((allowParent || !estate.hasParent()) && estate.getGrid().containsPosition(position))
			{
				return &estate;
			}
		}
	}

	return nullptr;
}

void EstateMaker::makeColored(Title title, bool setLower)
{
	setTitleGreyColor(Title::barony);
	setTitleGreyColor(Title::county);
	setTitleGreyColor(Title::duchy);
	setTitleGreyColor(Title::kingdom);
	setTitleGreyColor(Title::empire);

	if (title == Title::barony || (Title::barony < title && setLower))
	{
		setTitleDefaultColor(Title::barony);
	}
	if (title == Title::county || (Title::county < title && setLower))
	{
		setTitleDefaultColor(Title::county);

	}
	if (title == Title::duchy || (Title::duchy < title && setLower))
	{
		setTitleDefaultColor(Title::duchy);

	}
	if (title == Title::kingdom || (Title::kingdom < title && setLower))
	{
		setTitleDefaultColor(Title::kingdom);

	}
	if (title == Title::empire || (Title::empire < title && setLower))
	{
		setTitleDefaultColor(Title::empire);
	}
}

void EstateMaker::setTitleDefaultColor(Title title)
{
	for (EditorEstate& estate : estates[title])
	{
		estate.setGridColorDefault();
	}
}

void EstateMaker::setTitleGreyColor(Title title)
{
	for (auto& estate : estates[title])
	{
		estate.setGridColorGrey();
	}
}

void EstateMaker::randomiseTitleColor(Title title)
{
	for (EditorEstate& estate : estates[title])
	{
		estate.setDefaultColor(generateRandomEstateColor(title));
	}
}

void EstateMaker::drawEstates(sf::RenderWindow& window) const
{
	// Following code problematic if more titles equal in prominence to Title::barony are added.
	assert(static_cast<int>(Title::barony) == 1);

	for (const auto& [title, titleEstates] : estates)
	{
		for (const EditorEstate& estate : titleEstates)
		{
			estate.draw(window);
		}
	}
}

std::string EstateMaker::loadName(std::ifstream& file)
{
	std::string line;

	// Load name.
	std::getline(file, line);
	assert(line.compare("# name") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	std::string name = line;
	assert(allocatedEstateNames.count(name) == 0);
	// Ensure name cannot be reused.
	allocatedEstateNames.insert(name);
	return name;
}

sf::Color EstateMaker::loadColor(std::ifstream& file)
{
	std::string line;

	std::getline(file, line);
	assert(line.compare("# color") == 0);
	std::getline(file, line);
	assert(line[0] != '#');
	std::istringstream iss(line);
	int r, g, b;
	iss >> r >> g >> b;
	return sf::Color(r, g, b);
}

std::vector<std::string> EstateMaker::loadSubfiefNames(std::ifstream& file)
{
	std::vector<std::string> names;
	std::string line;

	std::getline(file, line);
	assert(line.compare("# subfiefs") == 0);

	while (std::getline(file, line) && line[0] != '#' && line.size() > 0)
	{
		names.push_back(line);
	}

	return names;
}

EditorEstate& EstateMaker::createEstate(Title title)
{
	assert(title != Title::barony);

	estates[title].emplace_back(EditorEstate(title, generateRandomEstateColor(title)));
	estates[title].back().initName(generateName());

	return estates[title].back();
}

std::string EstateMaker::generateName()
{
	std::string name;
	// LOOP POTENTIALLY NEVER TERMINATES IN PARTICULAR IF THERE IS NOT ENOUGH POSSIBLE NAMES.
	while (true)
	{
		name = nameGenerator.generateRandomName();
		if (allocatedEstateNames.count(name) == 0)
		{
			break;
		}
	}
	allocatedEstateNames.insert(name);
	return name;
}

void EstateMaker::reconcileBaronies(const std::deque<EditorTerritory>& territories)
{
	std::list<EditorEstate>& baronies = estates[Title::barony];

	for (const EditorTerritory& territory : territories)
	{
		if (territory.getType() != TerritoryType::land)
		{
			continue;
		}

		// Territory already allocated to a landed estate.
		if (allocatedTerritoryIDs.count(territory.getID()) == 1)
		{
			continue;
		}

		EditorEstate barony(Title::barony, &territory, generateRandomEstateColor(Title::barony));
		barony.initName(generateName());

		// Ensures id cannot be reused for other landed estate.
		allocatedTerritoryIDs.insert(territory.getID());

		estates[Title::barony].emplace_back(std::move(barony));
	}

	// Remove baronies with empty territory grids.
	for (auto barony = baronies.begin(); barony != baronies.end();)
	{
		assert(barony->getTitle() == Title::barony);
		
		const EditorTerritory* territory = barony->getTerritory();
		
		if (territory == nullptr || territory->getGrid().isEmpty())
		{
			barony = baronies.erase(barony);
		}
		else
		{
			++barony;
		}
	}
}

void EstateMaker::reconcileMaridoms(const std::deque<EditorTerritory>& territories)
{
	std::list<EditorEstate>& maridoms = estates[Title::maridom];

	for (const EditorTerritory& territory : territories)
	{
		if (territory.getType() != TerritoryType::naval)
		{
			continue;
		}

		// Territory already allocated to a landed estate.
		if (allocatedTerritoryIDs.count(territory.getID()) == 1)
		{
			continue;
		}

		EditorEstate maridom(Title::maridom, &territory, generateRandomEstateColor(Title::maridom));
		maridom.initName(generateName());

		// Ensures id cannot be reused for other landed estate.
		allocatedTerritoryIDs.insert(territory.getID());  

		estates[Title::maridom].emplace_back(std::move(maridom));
	}

	// Remove maridoms with empty territory grids.
	for (auto maridom = maridoms.begin(); maridom != maridoms.end();)
	{
		assert(maridom->getTitle() == Title::maridom);

		const EditorTerritory* territory = maridom->getTerritory();

		if (territory == nullptr && territory->getGrid().isEmpty())
		{
			maridom = maridoms.erase(maridom);
		}
		else
		{
			++maridom;
		}
	}
}

sf::Color generateRandomEstateColor(Title title)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	if (title == Title::maridom)
	{
		return sf::Color(120, 120, 120);
	}
	if (title == Title::barony)
	{
		std::uniform_int_distribution<int> redDist(20, 50);
		std::uniform_int_distribution<int> blueDist(200, 230);
		std::uniform_int_distribution<int> greenDist(0, 40);
		return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
	}
	else if (title == Title::county)
	{
		if (rand() % 3 == 0)
		{
			std::uniform_int_distribution<int> redDist(20, 50);
			std::uniform_int_distribution<int> blueDist(180, 220);
			std::uniform_int_distribution<int> greenDist(100, 180);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else if (rand() % 2 == 0)
		{
			std::uniform_int_distribution<int> redDist(20, 50);
			std::uniform_int_distribution<int> blueDist(150, 200);
			std::uniform_int_distribution<int> greenDist(180, 230);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else
		{
			std::uniform_int_distribution<int> redDist(20, 50);
			std::uniform_int_distribution<int> blueDist(120, 160);
			std::uniform_int_distribution<int> greenDist(110, 140);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
	}
	else if (title == Title::duchy)
	{
		if (rand() % 4 == 0)
		{
			std::uniform_int_distribution<int> redDist(20, 50);
			std::uniform_int_distribution<int> blueDist(110, 170);
			std::uniform_int_distribution<int> greenDist(180, 240);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else if (rand() % 3 == 0)
		{
			std::uniform_int_distribution<int> redDist(20, 40);
			std::uniform_int_distribution<int> blueDist(20, 80);
			std::uniform_int_distribution<int> greenDist(150, 180);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else if (rand() % 2 == 0)
		{
			std::uniform_int_distribution<int> redDist(90, 130);
			std::uniform_int_distribution<int> blueDist(20, 80);
			std::uniform_int_distribution<int> greenDist(110, 140);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else
		{
			std::uniform_int_distribution<int> redDist(20, 50);
			std::uniform_int_distribution<int> blueDist(20, 40);
			std::uniform_int_distribution<int> greenDist(190, 250);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
	}
	else if (title == Title::kingdom)
	{
		if (rand() % 4 == 0)
		{
			std::uniform_int_distribution<int> redDist(200, 240);
			std::uniform_int_distribution<int> blueDist(20, 50);
			std::uniform_int_distribution<int> greenDist(110, 150);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else if (rand() % 3 == 0)
		{
			std::uniform_int_distribution<int> redDist(220, 240);
			std::uniform_int_distribution<int> blueDist(20, 50);
			std::uniform_int_distribution<int> greenDist(180, 230);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else if (rand() % 2 == 0)
		{
			std::uniform_int_distribution<int> redDist(160, 180);
			std::uniform_int_distribution<int> blueDist(20, 50);
			std::uniform_int_distribution<int> greenDist(0, 110);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else
		{
			std::uniform_int_distribution<int> redDist(210, 250);
			std::uniform_int_distribution<int> blueDist(20, 40);
			std::uniform_int_distribution<int> greenDist(20, 50);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
	}
	else if (title == Title::empire)
	{
		if (rand() % 3 == 0)
		{
			std::uniform_int_distribution<int> redDist(180, 240);
			std::uniform_int_distribution<int> blueDist(80, 140);
			std::uniform_int_distribution<int> greenDist(10, 50);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else if (rand() % 3 == 0)
		{
			std::uniform_int_distribution<int> redDist(170, 240);
			std::uniform_int_distribution<int> blueDist(10, 50);
			std::uniform_int_distribution<int> greenDist(60, 140);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
		else
		{
			std::uniform_int_distribution<int> redDist(90, 130);
			std::uniform_int_distribution<int> blueDist(200, 255);
			std::uniform_int_distribution<int> greenDist(10, 50);
			return sf::Color(redDist(gen), greenDist(gen), blueDist(gen));
		}
	}

	throw std::logic_error("No color was specified for title.");
}