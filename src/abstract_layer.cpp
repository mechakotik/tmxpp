#include <tinyxml2.h>
#include <iostream>
#include <neotmx.hpp>
#include <sstream>
#include <string>
#include <vector>

struct tmx::internal::AbstractLayer::Data {
    int id = 0;
    std::string name;
    std::string className;
    float opacity = 1;
    bool visible = true;
    Color tintColor;
    Point offset;
    Point parallaxFactor;
};

__NEOTMX_CLASS_HEADER_IMPL__(tmx::internal, AbstractLayer)

int tmx::internal::AbstractLayer::id() const { return d->id; }
const std::string& tmx::internal::AbstractLayer::name() const { return d->name; }
const std::string& tmx::internal::AbstractLayer::className() const { return d->className; }
float tmx::internal::AbstractLayer::opacity() const { return d->opacity; }
bool tmx::internal::AbstractLayer::visible() const { return d->visible; }
tmx::Color tmx::internal::AbstractLayer::tintColor() const { return d->tintColor; }
tmx::Point tmx::internal::AbstractLayer::offset() const { return d->offset; }
tmx::Point tmx::internal::AbstractLayer::parallaxFactor() const { return d->parallaxFactor; }

void tmx::internal::AbstractLayer::parse(tinyxml2::XMLElement* root) {
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
