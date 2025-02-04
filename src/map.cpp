#include <tinyxml2.h>
#include <neotmx.hpp>
#include <string>

void tmx::Map::parseFromData(const std::string& data) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.Parse(data.c_str());
    if(error != 0) {
        throw Exception("XML parse failed (error code " + std::to_string(error) + ")");
    }
    parse(doc.RootElement()->FirstChildElement("map"));
}

void tmx::Map::parseFromFile(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
    if(error != 0) {
        throw Exception("XML parse failed (error code " + std::to_string(error) + ")");
    }
    parse(doc.RootElement()->FirstChildElement("map"));
}

void tmx::Map::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing map root element");
    }

    version_ = root->Attribute("version", "1.0");
    tiledVersion_ = root->Attribute("tiledVersion", "1.0");
    mapClass_ = root->Attribute("class", "");

    if(root->Attribute("orientation") != nullptr) {
        std::string value = root->Attribute("orientation");
        if(value == "orthogonal") {
            orientation_ = Orientation::ORTHOGONAL;
        } else if(value == "isometric") {
            orientation_ = Orientation::ISOMETRIC;
        } else if(value == "staggered") {
            orientation_ = Orientation::STAGGERED;
        } else if(value == "hexagonal") {
            orientation_ = Orientation::HEXAGONAL;
        } else {
            throw Exception("Invalid orientation " + value);
        }
    }

    if(root->Attribute("renderorder") != nullptr) {
        std::string value = root->Attribute("renderorder");
        if(value == "right-down") {
            renderOrder_ = RenderOrder::RIGHT_DOWN;
        } else if(value == "right-up") {
            renderOrder_ = RenderOrder::RIGHT_UP;
        } else if(value == "left-down") {
            renderOrder_ = RenderOrder::LEFT_DOWN;
        } else if(value == "left-up") {
            renderOrder_ = RenderOrder::LEFT_UP;
        } else {
            throw Exception("Invalid render order " + value);
        }
    }

    compressionLevel_ = root->IntAttribute("compressionlevel", -1);
    width_ = root->IntAttribute("width", 0);
    height_ = root->IntAttribute("height", 0);
    tileWidth_ = root->IntAttribute("tileWidth", 0);
    tileHeight_ = root->IntAttribute("tileHeight", 0);
    hexSideLength_ = root->IntAttribute("hexSideLength", 0);

    if(root->Attribute("staggeraxis") != nullptr) {
        std::string value = root->Attribute("staggeraxis");
        if(value == "x") {
            staggerAxis_ = StaggerAxis::X_AXIS;
        } else if(value == "y") {
            staggerAxis_ = StaggerAxis::Y_AXIS;
        } else {
            throw Exception("Invalid stagger axis " + value);
        }
    }

    if(root->Attribute("staggerindex") != nullptr) {
        std::string value = root->Attribute("staggerindex");
        if(value == "even") {
            staggerIndex_ = StaggerIndex::EVEN;
        } else if(value == "odd") {
            staggerIndex_ = StaggerIndex::ODD;
        } else {
            throw Exception("Invalid stagger index " + value);
        }
    }

    parallaxOrigin_.x = root->IntAttribute("parallaxoriginx", 0);
    parallaxOrigin_.y = root->IntAttribute("parallaxoriginx", 0);

    // TODO: background color

    parseProperties(root->FirstChildElement("properties"));
}
