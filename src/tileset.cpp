#include <tinyxml2.h>
#include <neotmx.hpp>
#include <string>

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
        firstGID_ = root->IntAttribute("firstgid");
    }
    if(root->Attribute("source") != nullptr) {
        source_ = root->Attribute("source");
        return;
    }

    name_ = root->Attribute("name");
    tilesetClass_ = root->Attribute("class", "");
    tileWidth_ = root->IntAttribute("tilewidth");
    tileHeight_ = root->IntAttribute("tileheight");
    spacing_ = root->IntAttribute("spacing");
    margin_ = root->IntAttribute("margin");
    tileCount_ = root->IntAttribute("tilecount");
    columns_ = root->IntAttribute("columns");

    if(root->Attribute("objectalignment")) {
        std::string value = root->Attribute("objectalignment");
        if(value == "unspecified") {
            objectAlignment_ = ObjectAlignment::UNSPECIFIED;
        } else if(value == "topleft") {
            objectAlignment_ = ObjectAlignment::TOP_LEFT;
        } else if(value == "top") {
            objectAlignment_ = ObjectAlignment::TOP;
        } else if(value == "topright") {
            objectAlignment_ = ObjectAlignment::TOP_RIGHT;
        } else if(value == "left") {
            objectAlignment_ = ObjectAlignment::LEFT;
        } else if(value == "center") {
            objectAlignment_ = ObjectAlignment::CENTER;
        } else if(value == "right") {
            objectAlignment_ = ObjectAlignment::RIGHT;
        } else if(value == "bottomleft") {
            objectAlignment_ = ObjectAlignment::BOTTOM_LEFT;
        } else if(value == "bottom") {
            objectAlignment_ = ObjectAlignment::BOTTOM;
        } else if(value == "bottomright") {
            objectAlignment_ = ObjectAlignment::BOTTOM_RIGHT;
        } else {
            throw Exception("Invalid object alignment " + value);
        }
    }

    if(root->Attribute("tilerendersize")) {
        std::string value = root->Attribute("tilerendersize");
        if(value == "tile") {
            tileRenderSize_ = TileRenderSize::TILE;
        } else if(value == "grid") {
            tileRenderSize_ = TileRenderSize::GRID;
        } else {
            throw Exception("Invalid tile render size " + value);
        }
    }

    if(root->Attribute("fillmode")) {
        std::string value = root->Attribute("fillmode");
        if(value == "stretch") {
            fillMode_ = FillMode::STRETCH;
        } else if(value == "preserve-aspect-fit") {
            fillMode_ = FillMode::PRESERVE_ASPECT_FIT;
        }
    }

    if(root->FirstChildElement("tileoffset") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("tileoffset");
        tileOffset_.x = element->IntAttribute("x");
        tileOffset_.y = element->IntAttribute("y");
    }

    if(root->FirstChildElement("grid") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("grid");
        if(element->Attribute("orientation") != nullptr) {
            std::string value = element->Attribute("orientation");
            if(value == "orthogonal") {
                gridOrientation_ = GridOrientation::ORTHOGONAL;
            } else if(value == "isometric") {
                gridOrientation_ = GridOrientation::ISOMETRIC;
            } else {
                throw Exception("Invalid grid orientation " + value);
            }
        }
        gridWidth_ = element->IntAttribute("width");
        gridHeight_ = element->IntAttribute("height");
    }
}
