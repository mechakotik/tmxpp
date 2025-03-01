#include <tinyxml2.h>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <tmxpp.hpp>

#ifdef TMXPP_BASE64
#include <base64.hpp>
#endif

struct tmx::TileLayer::Data {
    std::vector<std::vector<unsigned int>> data;
    int width = 0;
    int height = 0;
    std::string encoding;
    std::string compression;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, TileLayer)

const std::vector<std::vector<unsigned int>>& tmx::TileLayer::data() const { return d->data; }
int tmx::TileLayer::width() const { return d->width; }
int tmx::TileLayer::height() const { return d->height; }
std::string tmx::TileLayer::encoding() const { return d->encoding; }
std::string tmx::TileLayer::compression() const { return d->compression; }

static constexpr unsigned int FLIP_H = 0x80000000;
static constexpr unsigned int FLIP_V = 0x40000000;
static constexpr unsigned int FLIP_D = 0x20000000;
static constexpr unsigned int ROTATE_HEX120 = 0x10000000;

int tmx::TileLayer::at(int x, int y) const {
    checkBounds(x, y);
    return static_cast<int>(d->data[y][x] & ~(FLIP_H | FLIP_V | FLIP_D | ROTATE_HEX120));
}

bool tmx::TileLayer::flipHorizontal(int x, int y) const {
    checkBounds(x, y);
    return (d->data[y][x] & FLIP_H) != 0;
}

bool tmx::TileLayer::flipVertical(int x, int y) const {
    checkBounds(x, y);
    return (d->data[y][x] & FLIP_V) != 0;
}

bool tmx::TileLayer::flipDiagonal(int x, int y) const {
    checkBounds(x, y);
    return (d->data[y][x] & FLIP_D) != 0;
}

bool tmx::TileLayer::rotateHex120(int x, int y) const {
    checkBounds(x, y);
    return (d->data[y][x] & ROTATE_HEX120) != 0;
}

void tmx::TileLayer::checkBounds(int x, int y) const {
    if(x < 0 || x >= d->width) {
        throw Exception(
            "Index x = " + std::to_string(x) + " is out of range (width = " + std::to_string(d->width) + ")");
    }
    if(y < 0 || y >= d->height) {
        throw Exception(
            "Index y = " + std::to_string(y) + " is out of range (height = " + std::to_string(d->height) + ")");
    }
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

    d->data.assign(d->height, std::vector<unsigned int>(d->width, 0));
    if(d->encoding == "csv") {
        parseCSVData(root->GetText());
    } else if(d->encoding == "base64") {
        #ifdef TMXPP_BASE64
            std::string str = root->GetText();
            str = decompressData(str, d->encoding);
            parseBase64Data(str);
        #else
            throw Exception("Tilemap uses base64 encoding, but tmxpp was build without base64 support");
        #endif
    } else {
        throw Exception("Unknown encoding " + d->encoding);
    }
}

void tmx::TileLayer::parseCSVData(const std::string& str) {
    std::stringstream ss;
    ss << str;
    for(int y = 0; y < d->height; y++) {
        for(int x = 0; x < d->width; x++) {
            unsigned int value = 0;
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

std::string tmx::TileLayer::decompressData(const std::string& str, std::string compression) {
    return str;
}

void tmx::TileLayer::parseBase64Data(const std::string& str) {
#ifdef TMXPP_BASE64
    auto begin = str.begin(), end = str.end();
    while(*begin == ' ' || *begin == '\n' || *begin == '\t') {
        ++begin;
    }
    while(*std::prev(end, 1) == ' ' || *std::prev(end, 1) == '\n' || *std::prev(end, 1) == '\t') {
        --end;
    }
    std::string data = base64::from_base64(std::string_view(begin, end));
    if(static_cast<int>(data.size()) < d->width * d->height * 4) {
        throw Exception("Wrong data format for layer " + name());
    }

    int pos = 0;
    for(int y = 0; y < d->height; y++) {
        for(int x = 0; x < d->width; x++) {
            uint32_t value = 0;
            value |= static_cast<uint32_t>(data[pos]);
            value |= static_cast<uint32_t>(data[pos + 1]) << 8U;
            value |= static_cast<uint32_t>(data[pos + 2]) << 16U;
            value |= static_cast<uint32_t>(data[pos + 3]) << 24U;
            d->data[y][x] = value;
            pos += 4;
        }
    }
#endif
}
