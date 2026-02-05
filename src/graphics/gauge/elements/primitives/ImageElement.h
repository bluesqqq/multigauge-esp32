#pragma once

#include "graphics/gauge/Element.h"
#include "graphics/colors/Color.h"

class ImageElement : public Element {
    private:
        std::string path;

        Image image;

    public:
        explicit ImageElement(YGConfigRef config);

        ImageElement(YGConfigRef config, const rapidjson::Value::ConstObject json);

        bool init(AssetManager& assetManager) override;
        void draw(Graphics& g) const override;
};
