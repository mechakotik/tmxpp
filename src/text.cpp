#include <tmxpp.hpp>
#include "tinyxml2.h"

struct tmx::Text::Data {
    std::string text;
    std::string fontFamily = "sans-serif";
    int pixelSize = 16;
    bool wrap = false;
    Color color;
    bool bold = false;
    bool italic = false;
    bool underline = false;
    bool strikeout = false;
    bool kerning = true;
    HAlign halign = HAlign::LEFT;
    VAlign valign = VAlign::TOP;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, Text)

std::string tmx::Text::text() const { return d->text; }
std::string tmx::Text::fontFamily() const { return d->fontFamily; }
int tmx::Text::pixelSize() const { return d->pixelSize; }
bool tmx::Text::wrap() const { return d->wrap; }
tmx::Color tmx::Text::color() const { return d->color; }
bool tmx::Text::bold() const { return d->bold; }
bool tmx::Text::italic() const { return d->italic; }
bool tmx::Text::underline() const { return d->underline; }
bool tmx::Text::strikeout() const { return d->strikeout; }
bool tmx::Text::kerning() const { return d->kerning; }
tmx::Text::HAlign tmx::Text::halign() const { return d->halign; }
tmx::Text::VAlign tmx::Text::valign() const { return d->valign; }

void tmx::Text::parse(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing text element");
    }

    if(root->Value() != nullptr) {
        d->text = root->Value();
    }
    if(root->Attribute("fontfamily") != nullptr) {
        d->fontFamily = root->Attribute("fontFamily");
    }

    root->QueryIntAttribute("pixelsize", &d->pixelSize);
    root->QueryBoolAttribute("wrap", &d->wrap);

    if(root->Attribute("color") != nullptr) {
        d->color = colorFromString(root->Attribute("color"));
    }

    root->QueryBoolAttribute("bold", &d->bold);
    root->QueryBoolAttribute("italic", &d->italic);
    root->QueryBoolAttribute("underline", &d->underline);
    root->QueryBoolAttribute("strikeout", &d->strikeout);
    root->QueryBoolAttribute("kerning", &d->kerning);

    if(root->Attribute("halign") != nullptr) {
        std::string halign = root->Attribute("halign");
        if(halign == "left") {
            d->halign = HAlign::LEFT;
        } else if(halign == "center") {
            d->halign = HAlign::CENTER;
        } else if(halign == "right") {
            d->halign = HAlign::RIGHT;
        } else if(halign == "justify") {
            d->halign = HAlign::JUSTIFY;
        } else {
            throw Exception("Unknown text halign " + halign);
        }
    }

    if(root->Attribute("valign") != nullptr) {
        std::string valign = root->Attribute("valign");
        if(valign == "top") {
            d->valign = VAlign::TOP;
        } else if(valign == "center") {
            d->valign = VAlign::CENTER;
        } else if(valign == "bottom") {
            d->valign = VAlign::BOTTOM;
        } else {
            throw Exception("Unknown text valign " + valign);
        }
    }
}
