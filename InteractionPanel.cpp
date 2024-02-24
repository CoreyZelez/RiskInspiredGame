#include "InteractionPanel.h"
#include <iostream>
#include <assert.h>

InteractionPanel::InteractionPanel(std::vector<std::vector<std::unique_ptr<UIEntity>>> UIEntities)
	: UIEntities(std::move(UIEntities))
{
	init();
}

InteractionPanel::InteractionPanel(sf::Color backgroundColor, std::vector<std::vector<std::unique_ptr<UIEntity>>> UIEntities)
    : UIPanel(backgroundColor), UIEntities(std::move(UIEntities))
{
	// We construct the panel at the origin to determine its dimensions.
	float right = 0;  // Right most x value of UI entites.
	float bottom = 0;  // Bottom most y value of UI entites.
	sf::Vector2f position(0, borderPadding);  // Position of current UI entity.
	for(auto &row : this->UIEntities)
	{
		float rowRight = 0;  // Right most point of entities in current row.
		float rowBottom = bottom;  // Bottom most point of entities in current row.

		position.x = 0;

		// Position of UI entity.
		for(auto &entity : row)
		{
			if(rowRight == 0)
			{
				position.x = borderPadding;
			}
			else
			{
				position.x = rowRight + horizontalEntityPadding;
			}
			entity.get()->setPosition(position);

			assert(entity.get()->getDimensions().x > 0);
			rowRight = entity.get()->getDimensions().x + position.x;  // Right position of current entity.
			const float entityBottom = entity.get()->getDimensions().y + position.y;
			rowBottom = std::max(rowBottom, entityBottom);
		}

		right = std::max(right, rowRight);
		bottom = rowBottom;
		position.y = bottom + verticalEntityPadding;
	}

	resizeBackground(right + borderPadding, bottom + borderPadding);
}

void InteractionPanel::update()
{
	for(auto &row : UIEntities)
	{
		for(auto &UIEntity : row)
		{
			UIEntity.get()->update();
		}
	}
}

void InteractionPanel::draw(sf::RenderWindow &window) const
{
	UIPanel::draw(window);
	for(const auto &row : UIEntities)
	{
		for(const auto &entity : row)
		{
			entity.get()->draw(window);
		}
	}
}

void InteractionPanel::handleButtonDown(sf::Mouse::Button button, sf::Vector2f position)
{
	for(auto &row : UIEntities)
	{
		for(auto &entity : row)
		{
			entity.get()->handleButtonDown(button, position);
		}
	}
}

void InteractionPanel::setPosition(sf::Vector2f position, bool center)
{
	UIPanel::setPosition(position, center);
	// Adjust position to top left.
	if(center)
	{
		position.x -= getDimensions().x / 2;
		position.y -= getDimensions().y / 2;
	}

	float bottom = position.y;  // Bottom most y value of UI entites kept track of.
	sf::Vector2f entityPosition(position.x, position.y + borderPadding);  // Position of current UI entity.
	for(auto &row : this->UIEntities)
	{
		float rowRight = position.x;  // Right most point of entities in current row.
		float rowBottom = bottom;  // Bottom most point of entities in current row.

		// Set positions of UI entities.
		for(auto &entity : row)
		{
			if(rowRight == position.x)
			{
				entityPosition.x = position.x + borderPadding;
			}
			else
			{
				entityPosition.x = rowRight + horizontalEntityPadding;
			}
			entity.get()->setPosition(entityPosition);

			assert(entity.get()->getDimensions().x > 0);
			rowRight = entity.get()->getDimensions().x + entityPosition.x;  // Right position of current entity.
			const float entityBottom = entity.get()->getDimensions().y + entityPosition.y;
			rowBottom = std::max(rowBottom, entityBottom);
		}

		bottom = rowBottom;
		entityPosition.y = bottom + verticalEntityPadding;
	}
}

void InteractionPanel::init()
{
	// We construct the panel at the origin its dimensions and component relative positions.
	float right = 0;  // Right most x value of UI entites.
	float bottom = 0;  // Bottom most y value of UI entites.
	sf::Vector2f position(0, borderPadding);  // Position of current UI entity.
	for(auto &row : this->UIEntities)
	{
		float rowRight = 0;  // Right most point of entities in current row.
		float rowBottom = bottom;  // Bottom most point of entities in current row.

		position.x = 0;

		// Position of UI entity.
		for(auto &entity : row)
		{
			if(rowRight == 0)
			{
				position.x = borderPadding;
			}
			else
			{
				position.x = rowRight + horizontalEntityPadding;
			}
			entity.get()->setPosition(position);

			assert(entity.get()->getDimensions().x > 0);
			rowRight = entity.get()->getDimensions().x + position.x;  // Right position of current entity.
			const float entityBottom = entity.get()->getDimensions().y + position.y;
			rowBottom = std::max(rowBottom, entityBottom);
		}

		right = std::max(right, rowRight);
		bottom = rowBottom;
		position.y = bottom + verticalEntityPadding;
	}

	resizeBackground(right + borderPadding, bottom + borderPadding);
}
