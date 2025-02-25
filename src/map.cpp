#include <tinyxml2.h>
#include <tmxpp.hpp>
#include <string>
#include <vector>

struct tmx::Map::Data {
    std::string version;
    std::string tiledVersion;
    std::string className;
    Orientation orientation = Orientation::ORTHOGONAL;
    RenderOrder renderOrder = RenderOrder::RIGHT_DOWN;
    int compressionLevel = -1;
    int width = 0;
    int height = 0;
    int tileWidth = 0;
    int tileHeight = 0;
    int hexSideLength = 0;
    StaggerAxis staggerAxis = StaggerAxis::X_AXIS;
    StaggerIndex staggerIndex = StaggerIndex::EVEN;
    Point parallaxOrigin;
    Color backgroundColor;
    bool infinite = false;

    std::vector<Tileset> tilesets;
    std::vector<Layer> layers;

    std::filesystem::path path;
    LoaderType loader = nullptr;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, Map)

std::string tmx::Map::version() const { return d->version; }
std::string tmx::Map::tiledVersion() const { return d->tiledVersion; }
std::string tmx::Map::className() const { return d->className; }
tmx::Map::Orientation tmx::Map::orientation() const { return d->orientation; }
tmx::Map::RenderOrder tmx::Map::renderOrder() const { return d->renderOrder; }
int tmx::Map::compressionLevel() const { return d->compressionLevel; }
int tmx::Map::width() const { return d->width; }
int tmx::Map::height() const { return d->height; }
int tmx::Map::tileWidth() const { return d->tileWidth; }
int tmx::Map::tileHeight() const { return d->tileHeight; }
int tmx::Map::hexSideLength() const { return d->hexSideLength; }
tmx::Map::StaggerAxis tmx::Map::staggerAxis() const { return d->staggerAxis; }
tmx::Map::StaggerIndex tmx::Map::staggerIndex() const { return d->staggerIndex; }
tmx::Point tmx::Map::parallaxOrigin() const { return d->parallaxOrigin; }
tmx::Color tmx::Map::backgroundColor() const { return d->backgroundColor; }
bool tmx::Map::infinite() const { return d->infinite; }

const std::vector<tmx::Tileset>& tmx::Map::tilesets() const { return d->tilesets; }
const std::vector<tmx::Layer>& tmx::Map::layers() const { return d->layers; }

void tmx::Map::parseFromData(const std::string& data) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.Parse(data.c_str());
    if(error != 0) {
        throw Exception("XML parse failed (error code " + std::to_string(error) + ")");
    }
    parse(doc.FirstChildElement("map"));
}

void tmx::Map::parseFromFile(std::filesystem::path path, LoaderType loader) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = tinyxml2::XML_SUCCESS;
    if(loader == nullptr) {
        error = doc.LoadFile(path.string().c_str());
    } else {
        error = doc.Parse(loader(path).c_str());
    }
    if(error != 0) {
        throw Exception("XML parse failed (error code " + std::to_string(error) + ")");
    }
    d->path = path;
    d->loader = loader;
    parse(doc.FirstChildElement("map"));
}

void tmx::Map::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing map root element");
    }

    if(root->Attribute("version") != nullptr) {
        d->version = root->Attribute("version");
    }
    if(root->Attribute("tiledversion") != nullptr) {
        d->tiledVersion = root->Attribute("tiledversion");
    }
    if(root->Attribute("mapClass") != nullptr) {
        d->className = root->Attribute("mapClass");
    }

    if(root->Attribute("orientation") != nullptr) {
        std::string value = root->Attribute("orientation");
        if(value == "orthogonal") {
            d->orientation = Orientation::ORTHOGONAL;
        } else if(value == "isometric") {
            d->orientation = Orientation::ISOMETRIC;
        } else if(value == "staggered") {
            d->orientation = Orientation::STAGGERED;
        } else if(value == "hexagonal") {
            d->orientation = Orientation::HEXAGONAL;
        } else {
            throw Exception("Invalid orientation " + value);
        }
    }

    if(root->Attribute("renderorder") != nullptr) {
        std::string value = root->Attribute("renderorder");
        if(value == "right-down") {
            d->renderOrder = RenderOrder::RIGHT_DOWN;
        } else if(value == "right-up") {
            d->renderOrder = RenderOrder::RIGHT_UP;
        } else if(value == "left-down") {
            d->renderOrder = RenderOrder::LEFT_DOWN;
        } else if(value == "left-up") {
            d->renderOrder = RenderOrder::LEFT_UP;
        } else {
            throw Exception("Invalid render order " + value);
        }
    }

    root->QueryIntAttribute("compressionlevel", &d->compressionLevel);
    root->QueryIntAttribute("width", &d->width);
    root->QueryIntAttribute("height", &d->height);
    root->QueryIntAttribute("tilewidth", &d->tileWidth);
    root->QueryIntAttribute("tileheight", &d->tileHeight);
    root->QueryIntAttribute("hexsidelength", &d->hexSideLength);

    if(root->Attribute("staggeraxis") != nullptr) {
        std::string value = root->Attribute("staggeraxis");
        if(value == "x") {
            d->staggerAxis = StaggerAxis::X_AXIS;
        } else if(value == "y") {
            d->staggerAxis = StaggerAxis::Y_AXIS;
        } else {
            throw Exception("Invalid stagger axis " + value);
        }
    }

    if(root->Attribute("staggerindex") != nullptr) {
        std::string value = root->Attribute("staggerindex");
        if(value == "even") {
            d->staggerIndex = StaggerIndex::EVEN;
        } else if(value == "odd") {
            d->staggerIndex = StaggerIndex::ODD;
        } else {
            throw Exception("Invalid stagger index " + value);
        }
    }

    root->QueryDoubleAttribute("parallaxoriginx", &d->parallaxOrigin.x);
    root->QueryDoubleAttribute("parallaxoriginy", &d->parallaxOrigin.y);

    if(root->Attribute("backgroundcolor") != nullptr) {
        d->backgroundColor = colorFromString(root->Attribute("backgroundcolor"));
    }

    parseTilesets(root);
    parseLayers(root);
    Properties::parse(root->FirstChildElement("properties"));
}

void tmx::Map::parseTilesets(tinyxml2::XMLElement* root) {
    tinyxml2::XMLElement* element = root->FirstChildElement("tileset");
    while(element != nullptr) {
        Tileset tileset;
        tileset.parse(element);
        if(!tileset.source().empty()) {
            tileset.parseFromFile(d->path.parent_path() / tileset.source(), d->loader);
        }
        d->tilesets.push_back(tileset);
        element = element->NextSiblingElement("tileset");
    }
}

void tmx::Map::parseLayers(tinyxml2::XMLElement* root) {
    tinyxml2::XMLElement* element = root->FirstChildElement("layer");
    while(element != nullptr) {
        TileLayer layer;
        layer.parse(element);
        d->layers.emplace_back(std::move(layer));
        element = element->NextSiblingElement("layer");
    }
}
