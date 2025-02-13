#include <tinyxml2.h>
#include <neotmx.hpp>

struct tmx::Image::Data {
    Type type = Type::EMPTY;
    std::string source;
    Color transparentColor;
    int width = 0;
    int height = 0;
};

__NEOTMX_CLASS_HEADER_IMPL__(tmx, Image)

tmx::Image::Type tmx::Image::type() const { return d->type; }
const std::string& tmx::Image::source() const { return d->source; }
tmx::Color tmx::Image::transparentColor() const { return d->transparentColor; }
int tmx::Image::width() const { return d->width; }
int tmx::Image::height() const { return d->height; }

void tmx::Image::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing image element");
    }

    if(root->Attribute("source") != nullptr) {
        d->type = Type::EXTERNAL;
        d->source = root->Attribute("source");
    } else {
        throw Exception("Embedded images are currently unsupported");
    }

    if(root->Attribute("trans") != nullptr) {
        d->transparentColor = colorFromString(root->Attribute("trans"));
    }

    root->QueryIntAttribute("width", &d->width);
    root->QueryIntAttribute("height", &d->height);
}
