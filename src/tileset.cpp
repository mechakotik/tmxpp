#include <tinyxml2.h>
#include <neotmx.hpp>
#include <string>

struct tmx::Tileset::Data {
    int firstGID = 1;
    std::string source;
    std::string name;
    std::string tilesetClass;
    int tileWidth = 0;
    int tileHeight = 0;
    int spacing = 0;
    int margin = 0;
    int tileCount = 0;
    int columns = 0;
    ObjectAlignment objectAlignment = ObjectAlignment::UNSPECIFIED;
    TileRenderSize tileRenderSize = TileRenderSize::TILE;
    FillMode fillMode = FillMode::STRETCH;

    IntPoint tileOffset;
    GridOrientation gridOrientation = GridOrientation::ORTHOGONAL;
    int gridWidth = 0;
    int gridHeight = 0;
};

int tmx::Tileset::firstGID() const { return d->firstGID; }
std::string tmx::Tileset::source() const { return d->source; }
std::string tmx::Tileset::name() const { return d->name; }
std::string tmx::Tileset::tilesetClass() const { return d->tilesetClass; }
int tmx::Tileset::tileWidth() const { return d->tileWidth; }
int tmx::Tileset::tileHeight() const { return d->tileHeight; }
int tmx::Tileset::spacing() const { return d->spacing; }
int tmx::Tileset::margin() const { return d->margin; }
int tmx::Tileset::tileCount() const { return d->tileCount; }
int tmx::Tileset::columns() const { return d->columns; }
tmx::Tileset::ObjectAlignment tmx::Tileset::objectAlignment() const { return d->objectAlignment; }
tmx::Tileset::TileRenderSize tmx::Tileset::tileRenderSize() const { return d->tileRenderSize; }
tmx::Tileset::FillMode tmx::Tileset::fillMode() const { return d->fillMode; }

tmx::IntPoint tmx::Tileset::tileOffset() const { return d->tileOffset; }
tmx::Tileset::GridOrientation tmx::Tileset::gridOrientation() const { return d->gridOrientation; }
int tmx::Tileset::gridWidth() const { return d->gridWidth; }
int tmx::Tileset::gridHeight() const { return d->gridHeight; }

void tmx::Tileset::parseFromData(const std::string& data) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.Parse(data.c_str());
    if(error != 0) {
        throw Exception("XML parse failed (error code " + std::to_string(error) + ")");
    }
    parse(doc.RootElement()->FirstChildElement("tileset"));
}

void tmx::Tileset::parseFromFile(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if(error != 0) {
        throw Exception("XML parse failed (error code " + std::to_string(error) + ")");
    }
    parse(doc.RootElement()->FirstChildElement("tileset"));
}

void tmx::Tileset::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing tileset root element");
    }

    if(root->Attribute("firstgid") != nullptr) {
        d->firstGID = root->IntAttribute("firstgid");
    }
    if(root->Attribute("source") != nullptr) {
        d->source = root->Attribute("source");
        return;
    }

    d->name = root->Attribute("name");
    d->tilesetClass = root->Attribute("class", "");
    d->tileWidth = root->IntAttribute("tilewidth");
    d->tileHeight = root->IntAttribute("tileheight");
    d->spacing = root->IntAttribute("spacing");
    d->margin = root->IntAttribute("margin");
    d->tileCount = root->IntAttribute("tilecount");
    d->columns = root->IntAttribute("columns");

    if(root->Attribute("objectalignment")) {
        std::string value = root->Attribute("objectalignment");
        if(value == "unspecified") {
            d->objectAlignment = ObjectAlignment::UNSPECIFIED;
        } else if(value == "topleft") {
            d->objectAlignment = ObjectAlignment::TOP_LEFT;
        } else if(value == "top") {
            d->objectAlignment = ObjectAlignment::TOP;
        } else if(value == "topright") {
            d->objectAlignment = ObjectAlignment::TOP_RIGHT;
        } else if(value == "left") {
            d->objectAlignment = ObjectAlignment::LEFT;
        } else if(value == "center") {
            d->objectAlignment = ObjectAlignment::CENTER;
        } else if(value == "right") {
            d->objectAlignment = ObjectAlignment::RIGHT;
        } else if(value == "bottomleft") {
            d->objectAlignment = ObjectAlignment::BOTTOM_LEFT;
        } else if(value == "bottom") {
            d->objectAlignment = ObjectAlignment::BOTTOM;
        } else if(value == "bottomright") {
            d->objectAlignment = ObjectAlignment::BOTTOM_RIGHT;
        } else {
            throw Exception("Invalid object alignment " + value);
        }
    }

    if(root->Attribute("tilerendersize")) {
        std::string value = root->Attribute("tilerendersize");
        if(value == "tile") {
            d->tileRenderSize = TileRenderSize::TILE;
        } else if(value == "grid") {
            d->tileRenderSize = TileRenderSize::GRID;
        } else {
            throw Exception("Invalid tile render size " + value);
        }
    }

    if(root->Attribute("fillmode")) {
        std::string value = root->Attribute("fillmode");
        if(value == "stretch") {
            d->fillMode = FillMode::STRETCH;
        } else if(value == "preserve-aspect-fit") {
            d->fillMode = FillMode::PRESERVE_ASPECT_FIT;
        }
    }

    if(root->FirstChildElement("tileoffset") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("tileoffset");
        d->tileOffset.x = element->IntAttribute("x");
        d->tileOffset.y = element->IntAttribute("y");
    }

    if(root->FirstChildElement("grid") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("grid");
        if(element->Attribute("orientation") != nullptr) {
            std::string value = element->Attribute("orientation");
            if(value == "orthogonal") {
                d->gridOrientation = GridOrientation::ORTHOGONAL;
            } else if(value == "isometric") {
                d->gridOrientation = GridOrientation::ISOMETRIC;
            } else {
                throw Exception("Invalid grid orientation " + value);
            }
        }
        d->gridWidth = element->IntAttribute("width");
        d->gridHeight = element->IntAttribute("height");
    }
}
