#include <neotmx.hpp>
#include <tinyxml2.h>
#include <sstream>

struct tmx::TileLayer::Data {
    std::vector<std::vector<int>> data;
    int width = 0;
    int height = 0;
    std::string encoding;
    std::string compression;
};

__NEOTMX_CLASS_HEADER_IMPL__(TileLayer)

const std::vector<std::vector<int>>& tmx::TileLayer::data() const { return d->data; }
int tmx::TileLayer::width() const { return d->width; }
int tmx::TileLayer::height() const { return d->height; }
std::string tmx::TileLayer::encoding() const { return d->encoding; }
std::string tmx::TileLayer::compression() const { return d->compression; }

int tmx::TileLayer::at(int x, int y) const {
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

void tmx::TileLayer::parse(tinyxml2::XMLElement* root) {
    AbstractLayer::parse(root);
    root->QueryIntAttribute("width", &d->width);
    root->QueryIntAttribute("height", &d->height);
    parseData(root->FirstChildElement("data"));
}

void tmx::TileLayer::parseData(tinyxml2::XMLElement* root) {
    if(root == nullptr) {
        throw Exception("Missing layer data element for " + name());
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

void tmx::TileLayer::parseCSVData(const std::string& str) {
    std::stringstream ss;
    ss << str;
    for(int y = 0; y < d->height; y++) {
        for(int x = 0; x < d->width; x++) {
            int value = 0;
            if(!(ss >> value)) {
                throw Exception("Wrong data format for layer " + name());
            }
            d->data[y][x] = value;
            if(x != d->width - 1 || y != d->height - 1) {
                char temp = '\0';
                if(!(ss >> temp)) {
                    throw Exception("Wrong data format for layer " + name());
                }
            }
        }
    }
}
