#include <tinyxml2.h>
#include <neotmx.hpp>

struct tmx::Tile::Data {
    int id = 0;
    std::string className;
    IntPoint position;
    int width;
    int height;

    Image image;
};

__NEOTMX_CLASS_HEADER_IMPL__(tmx, Tile)

int tmx::Tile::id() const { return d->id; }
std::string tmx::Tile::className() const { return d->className; }
tmx::IntPoint tmx::Tile::position() const { return d->position; }
float tmx::Tile::width() const { return d->width; }
float tmx::Tile::height() const { return d->height; }

tmx::Image tmx::Tile::image() const { return d->image; }

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

    Properties::parse(root->FirstChildElement("properties"));
}
