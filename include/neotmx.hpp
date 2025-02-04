#ifndef NEOTMX_HPP
#define NEOTMX_HPP

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>

namespace tinyxml2 {
    class XMLElement;
}

namespace tmx {
    struct Point;
    struct IntPoint;
    struct Color;

    enum class Type : unsigned char;

    class Exception;
    class PropertyValue;
    class Properties;
    class Map;
    class Tileset;
} // namespace tmx

struct tmx::Point {
    double x = 0;
    double y = 0;
};

struct tmx::IntPoint {
    int x = 0;
    int y = 0;
};

struct tmx::Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;
};

class tmx::Exception : public std::exception {
public:
    explicit Exception(std::string what) : what_(std::move(what)) {}
    [[nodiscard]] const char* what() const noexcept override { return what_.c_str(); }

private:
    std::string what_;
};

enum class tmx::Type : unsigned char { EMPTY, STRING, INT, FLOAT, BOOL, COLOR, FILE, OBJECT, CLASS };

class tmx::PropertyValue {
public:
    [[nodiscard]] Type type() const { return type_; }
    [[nodiscard]] const std::string& stringValue() const { return std::get<std::string>(value_); }
    [[nodiscard]] int intValue() const { return std::get<int>(value_); }
    [[nodiscard]] float floatValue() const { return std::get<float>(value_); }
    [[nodiscard]] bool boolValue() const { return std::get<bool>(value_); }
    [[nodiscard]] Color colorValue() const { return std::get<Color>(value_); }
    [[nodiscard]] const std::string& fileValue() const { return std::get<std::string>(value_); }
    [[nodiscard]] int objectValue() const { return std::get<int>(value_); }
    [[nodiscard]] const Properties& classValue() const;

    template <typename T>
    void set(Type type, const T& value) {
        type_ = type;
        value_ = value;
    }

private:
    std::variant<std::string, int, float, bool, Color, std::shared_ptr<Properties>> value_;
    Type type_ = Type::EMPTY;
};

class tmx::Properties {
public:
    [[nodiscard]] bool hasProperty(const std::string& name) const { return properties_.contains(name); }
    [[nodiscard]] const PropertyValue& property(const std::string& name) { return properties_[name]; }
    [[nodiscard]] const std::map<std::string, PropertyValue>& properties() const { return properties_; }

protected:
    void parseProperties(tinyxml2::XMLElement* root);

private:
    void parseProperty(tinyxml2::XMLElement* property);

    std::map<std::string, PropertyValue> properties_;
};

class tmx::Map : public Properties {
public:
    enum class Orientation : unsigned char { ORTHOGONAL, ISOMETRIC, STAGGERED, HEXAGONAL };
    enum class RenderOrder : unsigned char { RIGHT_DOWN, RIGHT_UP, LEFT_DOWN, LEFT_UP };
    enum class StaggerAxis : unsigned char { X_AXIS, Y_AXIS };
    enum class StaggerIndex : unsigned char { EVEN, ODD };

    void parseFromData(const std::string& data);
    void parseFromFile(const std::string& filename);

    [[nodiscard]] std::string version() const { return version_; }
    [[nodiscard]] std::string tiledVersion() const { return tiledVersion_; }
    [[nodiscard]] std::string mapClass() const { return mapClass_; }
    [[nodiscard]] Orientation orientation() const { return orientation_; }
    [[nodiscard]] RenderOrder renderOrder() const { return renderOrder_; }
    [[nodiscard]] int compressionLevel() const { return compressionLevel_; }
    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] int tileWidth() const { return tileWidth_; }
    [[nodiscard]] int tileHeight() const { return tileHeight_; }
    [[nodiscard]] int hexSideLength() const { return hexSideLength_; }
    [[nodiscard]] StaggerAxis staggerAxis() const { return staggerAxis_; }
    [[nodiscard]] StaggerIndex staggerIndex() const { return staggerIndex_; }
    [[nodiscard]] Point parallaxOrigin() const { return parallaxOrigin_; }
    [[nodiscard]] Color backgroundColor() const { return backgroundColor_; }
    [[nodiscard]] bool infinite() const { return infinite_; }

private:
    void parse(tinyxml2::XMLElement* root);

    std::string version_;
    std::string tiledVersion_;
    std::string mapClass_;
    Orientation orientation_ = Orientation::ORTHOGONAL;
    RenderOrder renderOrder_ = RenderOrder::RIGHT_DOWN;
    int compressionLevel_ = -1;
    int width_ = 0;
    int height_ = 0;
    int tileWidth_ = 0;
    int tileHeight_ = 0;
    int hexSideLength_ = 0;
    StaggerAxis staggerAxis_ = StaggerAxis::X_AXIS;
    StaggerIndex staggerIndex_ = StaggerIndex::EVEN;
    Point parallaxOrigin_;
    Color backgroundColor_;
    bool infinite_ = false;
};

class tmx::Tileset : public Properties {
    friend class Map;

public:
    enum class ObjectAlignment : unsigned char {
        UNSPECIFIED,
        TOP_LEFT,
        TOP,
        TOP_RIGHT,
        LEFT,
        CENTER,
        RIGHT,
        BOTTOM_LEFT,
        BOTTOM,
        BOTTOM_RIGHT
    };

    enum class TileRenderSize : unsigned char { TILE, GRID };
    enum class FillMode : unsigned char { STRETCH, PRESERVE_ASPECT_FIT };
    enum class GridOrientation : unsigned char { ORTHOGONAL, ISOMETRIC };

    void parseFromData(const std::string& data);
    void parseFromFile(const std::string& filename);

    [[nodiscard]] int firstGID() const { return firstGID_; }
    [[nodiscard]] std::string source() const { return source_; }
    [[nodiscard]] std::string name() const { return name_; }
    [[nodiscard]] std::string tilesetClass() const { return tilesetClass_; }
    [[nodiscard]] int tileWidth() const { return tileWidth_; }
    [[nodiscard]] int tileHeight() const { return tileHeight_; }
    [[nodiscard]] int spacing() const { return spacing_; }
    [[nodiscard]] int margin() const { return margin_; }
    [[nodiscard]] int tileCount() const { return tileCount_; }
    [[nodiscard]] int columns() const { return columns_; }
    [[nodiscard]] ObjectAlignment objectAlignment() const { return objectAlignment_; }
    [[nodiscard]] TileRenderSize tileRenderSize() const { return tileRenderSize_; }
    [[nodiscard]] FillMode fillMode() const { return fillMode_; }

    [[nodiscard]] IntPoint tileOffset() const { return tileOffset_; }
    [[nodiscard]] GridOrientation gridOrientation() const { return gridOrientation_; }
    [[nodiscard]] int gridWidth() const { return gridWidth_; }
    [[nodiscard]] int gridHeight() const { return gridHeight_; }

private:
    void parse(tinyxml2::XMLElement* root);

    int firstGID_ = 1;
    std::string source_;
    std::string name_;
    std::string tilesetClass_;
    int tileWidth_ = 0;
    int tileHeight_ = 0;
    int spacing_ = 0;
    int margin_ = 0;
    int tileCount_ = 0;
    int columns_ = 0;
    ObjectAlignment objectAlignment_ = ObjectAlignment::UNSPECIFIED;
    TileRenderSize tileRenderSize_ = TileRenderSize::TILE;
    FillMode fillMode_ = FillMode::STRETCH;

    IntPoint tileOffset_;
    GridOrientation gridOrientation_ = GridOrientation::ORTHOGONAL;
    int gridWidth_ = 0;
    int gridHeight_ = 0;
};

#endif // NEOTMX_HPP
