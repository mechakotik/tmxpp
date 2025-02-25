#include <tinyxml2.h>
#include <tmxpp.hpp>

struct tmx::Tile::Data {
    int id = 0;
    std::string className;
    IntPoint position;
    int width;
    int height;

    Image image;
    ObjectGroup objectGroup;
    std::vector<AnimationFrame> animation;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, Tile)

int tmx::Tile::id() const { return d->id; }
std::string tmx::Tile::className() const { return d->className; }
tmx::IntPoint tmx::Tile::position() const { return d->position; }
int tmx::Tile::width() const { return d->width; }
int tmx::Tile::height() const { return d->height; }

tmx::Image tmx::Tile::image() const { return d->image; }
const tmx::ObjectGroup& tmx::Tile::objectGroup() const { return d->objectGroup; }
const std::vector<tmx::Tile::AnimationFrame>& tmx::Tile::animation() const { return d->animation; }

void tmx::Tile::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing tile element");
    }

    if(root->Attribute("type") != nullptr) {
        d->className = root->Attribute("type");
    }

    root->QueryIntAttribute("id", &d->id);
    root->QueryIntAttribute("x", &d->position.x);
    root->QueryIntAttribute("y", &d->position.y);
    root->QueryIntAttribute("width", &d->width);
    root->QueryIntAttribute("height", &d->height);

    if(root->FirstChildElement("image") != nullptr) {
        d->image.parse(root->FirstChildElement("image"));
    }
    if(root->FirstChildElement("objectgroup") != nullptr) {
        d->objectGroup.parse(root->FirstChildElement("objectgroup"));
    }
    if(root->FirstChildElement("animation") != nullptr) {
        parseAnimation(root->FirstChildElement("animation"));
    }
    Properties::parse(root->FirstChildElement("properties"));
}

void tmx::Tile::parseAnimation(tinyxml2::XMLElement* root) {
    tinyxml2::XMLElement* element = root->FirstChildElement("frame");
    while(element != nullptr) {
        AnimationFrame frame;
        element->QueryIntAttribute("tileid", &frame.id);
        element->QueryIntAttribute("duration", &frame.duration);
        d->animation.push_back(frame);
        element = element->NextSiblingElement("frame");
    }
}
