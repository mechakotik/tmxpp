#include <tinyxml2.h>
#include <sstream>
#include <string>
#include <tmxpp.hpp>
#include <variant>
#include <vector>

struct tmx::Object::Data {
    int id = 0;
    std::string name;
    std::string className;
    Point position;
    Point size;
    double rotation = 0;
    int gid = 0;
    bool visible = true;

    Type type = Type::EMPTY;
    std::variant<Ellipse, Point, std::vector<Point>, Text> shape;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, Object)

int tmx::Object::id() const { return d->id; }
std::string tmx::Object::name() const { return d->name; }
std::string tmx::Object::className() const { return d->className; }
tmx::Point tmx::Object::position() const { return d->position; }
tmx::Point tmx::Object::size() const { return d->size; }
double tmx::Object::rotation() const { return d->rotation; }
int tmx::Object::gid() const { return d->gid; }
bool tmx::Object::visible() const { return d->visible; }

tmx::Object::Type tmx::Object::type() const { return d->type; }

tmx::Ellipse tmx::Object::ellipse() const {
    ensureType(Type::ELLIPSE);
    return std::get<Ellipse>(d->shape);
}

tmx::Point tmx::Object::point() const {
    ensureType(Type::POINT);
    return std::get<Point>(d->shape);
}

const tmx::Polygon& tmx::Object::polygon() const {
    ensureType(Type::POLYGON);
    return std::get<Polygon>(d->shape);
}

const tmx::Polyline& tmx::Object::polyline() const {
    ensureType(Type::POLYLINE);
    return std::get<Polyline>(d->shape);
}

const tmx::Text& tmx::Object::text() const {
    ensureType(Type::TEXT);
    return std::get<Text>(d->shape);
}

void tmx::Object::ensureType(Type type) const {
    if(d->type != type) {
        throw Exception("Attempt to extract " + typeName(type) + " while storing " + typeName(d->type));
    }
}

std::string tmx::Object::typeName(Type type) {
    switch(type) {
        case Type::EMPTY:
            return "nothing";
        case Type::ELLIPSE:
            return "ellipse";
        case Type::POINT:
            return "point";
        case Type::POLYGON:
            return "polygon";
        case Type::POLYLINE:
            return "polyline";
        case Type::TEXT:
            return "text";
        default:
            return "unknown";
    }
}

void tmx::Object::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing object root element");
    }

    if(root->Attribute("name") != nullptr) {
        d->name = root->Attribute("name");
    }
    if(root->Attribute("className") != nullptr) {
        d->className = root->Attribute("className");
    }

    root->QueryIntAttribute("id", &d->id);
    root->QueryDoubleAttribute("x", &d->position.x);
    root->QueryDoubleAttribute("y", &d->position.y);
    root->QueryDoubleAttribute("width", &d->size.x);
    root->QueryDoubleAttribute("height", &d->size.y);
    root->QueryDoubleAttribute("rotation", &d->rotation);
    root->QueryIntAttribute("gid", &d->gid);
    root->QueryBoolAttribute("visible", &d->visible);
    // TODO: object templates

    if(root->FirstChildElement("ellipse") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("ellipse");
        Ellipse ellipse;
        element->QueryDoubleAttribute("x", &ellipse.center.x);
        element->QueryDoubleAttribute("y", &ellipse.center.y);
        element->QueryDoubleAttribute("width", &ellipse.size.x);
        element->QueryDoubleAttribute("height", &ellipse.size.y);
        d->shape = ellipse;
        d->type = Type::ELLIPSE;
    } else if(root->FirstChildElement("point") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("point");
        Point point;
        element->QueryDoubleAttribute("x", &point.x);
        element->QueryDoubleAttribute("y", &point.y);
        d->shape = point;
        d->type = Type::POINT;
    } else if(root->FirstChildElement("polygon") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("polygon");
        d->shape = parsePoints(element->Attribute("points"));
        d->type = Type::POLYGON;
    } else if(root->FirstChildElement("polyline") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("polyline");
        d->shape = parsePoints(element->Attribute("points"));
        d->type = Type::POLYLINE;
    } else if(root->FirstChildElement("text") != nullptr) {
        tinyxml2::XMLElement* element = root->FirstChildElement("text");
        d->shape = tmx::Text();
        std::get<Text>(d->shape).parse(element);
        d->type = Type::TEXT;
    }

    Properties::parse(root->FirstChildElement("properties"));
}

std::vector<tmx::Point> tmx::Object::parsePoints(const std::string& str) {
    std::vector<Point> res;
    std::stringstream ss;
    ss << str;

    Point point;
    char temp = '\0';
    while(ss >> point.x >> temp >> point.y) {
        res.push_back({point.x, point.y});
    }

    return res;
}
