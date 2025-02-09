#include <tinyxml2.h>
#include <iostream>
#include <neotmx.hpp>
#include <sstream>
#include <string>
#include <vector>

struct tmx::AbstractLayer::Data {
    int id = 0;
    std::string name;
    std::string className;
    float opacity = 1;
    bool visible = true;
    Color tintColor;
    Point offset;
    Point parallaxFactor;
};

__NEOTMX_CLASS_HEADER_IMPL__(AbstractLayer)

int tmx::AbstractLayer::id() const { return d->id; }
const std::string& tmx::AbstractLayer::name() const { return d->name; }
const std::string& tmx::AbstractLayer::className() const { return d->className; }
float tmx::AbstractLayer::opacity() const { return d->opacity; }
bool tmx::AbstractLayer::visible() const { return d->visible; }
tmx::Color tmx::AbstractLayer::tintColor() const { return d->tintColor; }
tmx::Point tmx::AbstractLayer::offset() const { return d->offset; }
tmx::Point tmx::AbstractLayer::parallaxFactor() const { return d->parallaxFactor; }

void tmx::AbstractLayer::parse(tinyxml2::XMLElement* root) {
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
    root->QueryFloatAttribute("opacity", &d->opacity);
    root->QueryBoolAttribute("visible", &d->visible);

    if(root->Attribute("tintcolor") != nullptr) {
        d->tintColor = colorFromString(root->Attribute("tintcolor"));
    }

    root->QueryFloatAttribute("offsetx", &d->offset.x);
    root->QueryFloatAttribute("offsety", &d->offset.y);
    root->QueryFloatAttribute("parallaxx", &d->parallaxFactor.x);
    root->QueryFloatAttribute("parallaxy", &d->parallaxFactor.y);

    Properties::parse(root->FirstChildElement("properties"));
}
