#include <tmxpp.hpp>
#include <tinyxml2.h>
#include <vector>

struct tmx::ObjectGroup::Data {
    Color color;
    DrawOrder drawOrder = DrawOrder::TOPDOWN;
    std::vector<Object> objects;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, ObjectGroup)

tmx::Color tmx::ObjectGroup::color() const {return d->color;}
tmx::ObjectGroup::DrawOrder tmx::ObjectGroup::drawOrder() const {return d->drawOrder;}
const std::vector<tmx::Object>& tmx::ObjectGroup::objects() const { return d->objects; }

void tmx::ObjectGroup::parse(tinyxml2::XMLElement* root) {
    AbstractLayer::parse(root);

    if(root->Attribute("color") != nullptr) {
        d->color = colorFromString(root->Attribute("color"));
    }

    if(root->Attribute("drawOrder") != nullptr) {
        std::string value = root->Attribute("drawOrder");
        if(value == "topdown") {
            d->drawOrder = DrawOrder::TOPDOWN;
        } else if(value == "index") {
            d->drawOrder = DrawOrder::INDEX;
        } else {
            throw Exception("Unknown object group draw order " + value);
        }
    }

    tinyxml2::XMLElement* element = root->FirstChildElement("object");
    while(element != nullptr) {
        Object object;
        object.parse(element);
        d->objects.push_back(object);
        element = element->NextSiblingElement("object");
    }
}
