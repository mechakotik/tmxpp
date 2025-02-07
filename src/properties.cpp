#include <tinyxml2.h>
#include <map>
#include <neotmx.hpp>
#include <string>
#include <variant>

struct tmx::PropertyValue::Data {
    Type type = Type::EMPTY;
    std::variant<std::string, int, float, bool, Color, Properties> value;
};

__NEOTMX_CLASS_HEADER_IMPL__(PropertyValue)

tmx::Type tmx::PropertyValue::type() const { return d->type; }
const std::string& tmx::PropertyValue::stringValue() const { return std::get<std::string>(d->value); }
int tmx::PropertyValue::intValue() const { return std::get<int>(d->value); }
float tmx::PropertyValue::floatValue() const { return std::get<float>(d->value); }
bool tmx::PropertyValue::boolValue() const { return std::get<bool>(d->value); }
tmx::Color tmx::PropertyValue::colorValue() const { return std::get<Color>(d->value); }
const std::string& tmx::PropertyValue::fileValue() const { return std::get<std::string>(d->value); }
int tmx::PropertyValue::objectValue() const { return std::get<int>(d->value); }
const tmx::Properties& tmx::PropertyValue::classValue() const { return std::get<Properties>(d->value); }

struct tmx::Properties::Data {
    std::map<std::string, PropertyValue> properties;
};

__NEOTMX_CLASS_HEADER_IMPL__(Properties)

bool tmx::Properties::hasProperty(const std::string& name) const { return d->properties.contains(name); }

const tmx::PropertyValue& tmx::Properties::property(const std::string& name) const {
    if(!hasProperty(name)) {
        static PropertyValue emptyValue = PropertyValue();
        return emptyValue;
    }
    return d->properties.at(name);
}

const std::map<std::string, tmx::PropertyValue>& tmx::Properties::properties() const { return d->properties; }

void tmx::Properties::parse(tinyxml2::XMLElement* root) {
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
    std::string type = "string";
    if(property->Attribute("type") != nullptr) {
        type = property->Attribute("type");
    }
    PropertyValue::Data data;

    if(type == "string") {
        data = {.type = Type::STRING, .value = property->Attribute("value")};
    } else if(type == "int") {
        data = {.type = Type::INT, .value = property->IntAttribute("value")};
    } else if(type == "float") {
        data = {.type = Type::FLOAT, .value = property->FloatAttribute("value")};
    } else if(type == "bool") {
        data = {.type = Type::BOOL, .value = property->BoolAttribute("value")};
    } else if(type == "color") {
        // TODO
    } else if(type == "file") {
        data = {.type = Type::FILE, .value = std::string(property->Attribute("value"))};
    } else if(type == "object") {
        data = {.type = Type::OBJECT, .value = property->IntAttribute("value")};
    } else if(type == "class") {
        Properties properties;
        properties.parse(property->FirstChildElement("properties"));
        data = {.type = Type::CLASS, .value = properties};
    }

    PropertyValue value;
    value.d = DPointer(data);
    d->properties[name] = value;
}
