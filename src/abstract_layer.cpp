#include <tinyxml2.h>
#include <string>
#include <tmxpp.hpp>

struct tmx::internal::AbstractLayer::Data {
    int id = 0;
    std::string name;
    std::string className;
    double opacity = 1;
    bool visible = true;
    Color tintColor;
    Point offset;
    Point parallaxFactor{.x = 1, .y = 1};
};

__TMXPP_CLASS_HEADER_IMPL__(tmx::internal, AbstractLayer)

int tmx::internal::AbstractLayer::id() const { return d->id; }
const std::string& tmx::internal::AbstractLayer::name() const { return d->name; }
const std::string& tmx::internal::AbstractLayer::className() const { return d->className; }
double tmx::internal::AbstractLayer::opacity() const { return d->opacity; }
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
    root->QueryDoubleAttribute("opacity", &d->opacity);
    root->QueryBoolAttribute("visible", &d->visible);

    if(root->Attribute("tintcolor") != nullptr) {
        d->tintColor = colorFromString(root->Attribute("tintcolor"));
    }

    root->QueryDoubleAttribute("offsetx", &d->offset.x);
    root->QueryDoubleAttribute("offsety", &d->offset.y);
    root->QueryDoubleAttribute("parallaxx", &d->parallaxFactor.x);
    root->QueryDoubleAttribute("parallaxy", &d->parallaxFactor.y);

    Properties::parse(root->FirstChildElement("properties"));
}
