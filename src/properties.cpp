#include <tinyxml2.h>
#include <memory>
#include <neotmx.hpp>

const tmx::Properties& tmx::PropertyValue::classValue() const {
    return *std::get<std::shared_ptr<Properties>>(value_);
}

void tmx::Properties::parseProperties(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        return;
    }
    tinyxml2::XMLElement* property = root->FirstChildElement("property");
    while(property != nullptr) {
        parseProperty(property);
        property = property->NextSiblingElement("property");
    }
}

void tmx::Properties::parseProperty(tinyxml2::XMLElement* property) {
    if(property->Attribute("name") == nullptr) {
        throw Exception("Missing property name");
    }
    if(property->Attribute("value") == nullptr) {
        throw Exception("Missing property value");
    }

    std::string name = property->Attribute("name");
    std::string type = property->Attribute("type", "string");
    if(type == "string") {
        properties_[name].set(Type::STRING, std::string(property->Attribute("value")));
    } else if(type == "int") {
        properties_[name].set(Type::INT, property->IntAttribute("value"));
    } else if(type == "float") {
        properties_[name].set(Type::FLOAT, property->FloatAttribute("value"));
    } else if(type == "bool") {
        properties_[name].set(Type::BOOL, property->BoolAttribute("value"));
    } else if(type == "color") {
        // TODO
    } else if(type == "file") {
        properties_[name].set(Type::FILE, std::string(property->Attribute("value")));
    } else if(type == "object") {
        properties_[name].set(Type::OBJECT, property->IntAttribute("value"));
    } else if(type == "class") {
        std::shared_ptr<Properties> properties = std::make_shared<Properties>();
        properties->parseProperties(property->FirstChildElement("properties"));
        properties_[name].set(Type::CLASS, properties);
    }
}
