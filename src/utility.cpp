#include <neotmx.hpp>

tmx::Color tmx::colorFromString(const std::string& str) {
    auto convert = [str](char c) {
        if('0' <= c && c <= '9') {
            return c - '0';
        }
        if('A' <= c && c <= 'F') {
            return 10 + (c - 'A');
        }
        throw Exception("Unknown color format" + str);
    };
    auto convert2 = [convert](char a, char b) { return (convert(a) * 10) + convert(b); };

    if((str.size() == 7 || str.size() == 9) && str[0] != '#') {
        throw Exception("Unknown color format" + str);
    }

    Color color;
    color.a = 255;

    switch(str.size()) {
        case 6:
            color.r = convert2(str[0], str[1]);
            color.g = convert2(str[2], str[3]);
            color.b = convert2(str[4], str[5]);
            break;
        case 7:
            color.r = convert2(str[1], str[2]);
            color.g = convert2(str[3], str[4]);
            color.b = convert2(str[5], str[6]);
            break;
        case 8:
            color.a = convert2(str[0], str[1]);
            color.r = convert2(str[2], str[3]);
            color.g = convert2(str[4], str[5]);
            color.b = convert2(str[6], str[7]);
            break;
        case 9:
            color.a = convert2(str[1], str[2]);
            color.r = convert2(str[3], str[4]);
            color.g = convert2(str[5], str[6]);
            color.b = convert2(str[7], str[8]);
            break;
        default:
            throw Exception("Unknown color format " + str);
    }

    return color;
}
