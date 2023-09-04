#include "UIComponent.h"

UIComponent::UIComponent(const sf::Vector2f& position, float padding): padding(padding) {
	background.setPosition(position);
}

void UIComponent::addDecoration(UIDecoration &decoration)
{
	float yOffset = decorations.empty() ? 0.0f : decorations.back().getTextBounds().top + decorations.back().getTextBounds().height + padding;
	decoration.setPosition(background.getPosition().x + padding, background.getPosition().y + yOffset);
	decorations.push_back(decoration);

	// Update the background size based on the decorations
	float maxWidth = 0.0f;
	for(const UIDecoration& dec : decorations) {
		sf::FloatRect bounds = dec.getTextBounds();
		maxWidth = std::max(maxWidth, bounds.width);
	}
	background.setSize(sf::Vector2f(maxWidth + 2 * padding, yOffset + decorations.back().getTextBounds().height + padding));
}

void UIComponent::setPosition(const sf::Vector2f& position) {
	background.setPosition(position);
	// Re-position the decorations based on the new background position
	float yOffset = padding;
	for(UIDecoration& dec : decorations) {
		dec.setPosition(background.getPosition().x + padding, background.getPosition().y + yOffset);
		yOffset += dec.getTextBounds().height + padding;
	}
}

void UIComponent::draw(sf::RenderWindow& window) const {
	window.draw(background);
	for(const UIDecoration& dec : decorations) {
		dec.draw(window);
	}
}