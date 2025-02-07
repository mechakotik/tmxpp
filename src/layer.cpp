#include <tinyxml2.h>
#include <iostream>
#include <neotmx.hpp>
#include <sstream>
#include <string>
#include <vector>

struct tmx::Layer::Data {
    int id = 0;
    std::string name;
    std::string className;
    int width = 0;
    int height = 0;
    float opacity = 1;
    bool visible = true;
    Color tintColor;
    Point offset;
    Point parallaxFactor;

    std::vector<std::vector<int>> data;
    std::string encoding;
    std::string compression;
};

__NEOTMX_CLASS_HEADER_IMPL__(Layer)

int tmx::Layer::id() const { return d->id; }
const std::string& tmx::Layer::name() const { return d->name; }
const std::string& tmx::Layer::className() const { return d->className; }
int tmx::Layer::width() const { return d->width; }
int tmx::Layer::height() const { return d->height; }
float tmx::Layer::opacity() const { return d->opacity; }
bool tmx::Layer::visible() const { return d->visible; }
tmx::Color tmx::Layer::tintColor() const { return d->tintColor; }
tmx::Point tmx::Layer::offset() const { return d->offset; }
tmx::Point tmx::Layer::parallaxFactor() const { return d->parallaxFactor; }

const std::vector<std::vector<int>>& tmx::Layer::data() const { return d->data; }
std::string tmx::Layer::encoding() const { return d->encoding; }
std::string tmx::Layer::compression() const { return d->compression; }

int tmx::Layer::at(int x, int y) const {
    if(x < 0 || x >= d->width) {
        throw Exception(
            "Index x = " + std::to_string(x) + " is out of range (width = " + std::to_string(d->width) + ")");
    }
    if(y < 0 || y >= d->height) {
        throw Exception(
            "Index y = " + std::to_string(y) + " is out of range (height = " + std::to_string(d->height) + ")");
    }
    return d->data[y][x];
}

void tmx::Layer::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing layer root element");
    }

    if(root->Attribute("name") != nullptr) {
        d->name = root->Attribute("name");
    }
    if(root->Attribute("className") != nullptr) {
        d->className = root->Attribute("className");
    }

    root->QueryIntAttribute("id", &d->id);
    root->QueryIntAttribute("width", &d->width);
    root->QueryIntAttribute("height", &d->height);
    root->QueryFloatAttribute("opacity", &d->opacity);
    root->QueryBoolAttribute("visible", &d->visible);

    if(root->Attribute("tintcolor") != nullptr) {
        d->tintColor = colorFromString(root->Attribute("tintcolor"));
    }

    root->QueryFloatAttribute("offsetx", &d->offset.x);
    root->QueryFloatAttribute("offsety", &d->offset.y);
    root->QueryFloatAttribute("parallaxx", &d->parallaxFactor.x);
    root->QueryFloatAttribute("parallaxy", &d->parallaxFactor.y);

    parseData(root->FirstChildElement("data"));
    Properties::parse(root->FirstChildElement("properties"));
}

void tmx::Layer::parseData(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing layer data element for " + d->name);
    }

    if(root->Attribute("encoding") == nullptr) {
        throw Exception("Layer data is stored as XML elements, this is deprecated and unsupported by neotmx");
    }
    d->encoding = root->Attribute("encoding");

    if(root->Attribute("compression") != nullptr) {
        d->compression = root->Attribute("compression");
    }

    d->data.assign(d->height, std::vector<int>(d->width, 0));
    if(d->encoding == "csv") {
        parseCSVData(root->GetText());
    }

    // TODO: base64, compression
}

void tmx::Layer::parseCSVData(const std::string& str) {
    std::stringstream ss;
    ss << str;
    for(int y = 0; y < d->height; y++) {
        for(int x = 0; x < d->width; x++) {
            int value = 0;
            if(!(ss >> value)) {
                throw Exception("Wrong data format for layer " + d->name);
            }
            d->data[y][x] = value;
            if(x != d->width - 1 || y != d->height - 1) {
                char temp = '\0';
                if(!(ss >> temp)) {
                    throw Exception("Wrong data format for layer " + d->name);
                }
            }
        }
    }
}
