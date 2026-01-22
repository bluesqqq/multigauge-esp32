#include "Element.h"

bool Element::init() { return true; }

void Element::draw(Graphics &g) const { for (auto const& c : children) { c->draw(g); } }

void Element::update(int deltaTime) { for (auto& c : children) c->update(deltaTime); }
