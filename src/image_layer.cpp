#include <tmxpp.hpp>
#include "tinyxml2.h"

struct tmx::ImageLayer::Data {
    Image image;
    bool repeatX = false;
    bool repeatY = false;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, ImageLayer)

const tmx::Image& tmx::ImageLayer::image() const { return d->image; }
bool tmx::ImageLayer::repeatX() const { return d->repeatX; }
bool tmx::ImageLayer::repeatY() const { return d->repeatY; }

void tmx::ImageLayer::parse(tinyxml2::XMLElement* root) {
    AbstractLayer::parse(root);

    root->QueryBoolAttribute("repeatx", &d->repeatX);
    root->QueryBoolAttribute("repeatY", &d->repeatY);

    if(root->FirstChildElement("image") != nullptr) {
        d->image.parse(root->FirstChildElement("image"));
    }
}
