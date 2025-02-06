#ifndef NEOTMX_HPP
#define NEOTMX_HPP

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <utility>

#define __NEOTMX_CLASS_HEADER_DEF__(Type) \
    Type(); \
    Type(const Type& other); \
    Type(Type&& other) noexcept; \
    Type& operator=(const Type&); \
    Type& operator=(Type&&) noexcept; \
    ~Type();

#define __NEOTMX_CLASS_HEADER_IMPL__(Type) \
    tmx::Type::Type() = default; \
    tmx::Type::Type(const tmx::Type&) = default; \
    tmx::Type::Type(tmx::Type&&) = default; \
    tmx::Type& tmx::Type::operator=(const tmx::Type&) = default; \
    tmx::Type& tmx::Type::operator=(tmx::Type&&) = default; \
    tmx::Type::~Type() = default;

namespace tinyxml2 {
    class XMLElement;
}

namespace tmx {
    struct Point;
    struct IntPoint;
    struct Color;

    template<typename T>
    class DPointer;

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

template<typename T>
class tmx::DPointer {
public:
    DPointer() : ptr(std::make_unique<T>()) {}
    explicit DPointer(const T& data) : ptr(std::make_unique<T>(data)) {}
    explicit DPointer(T&& data) : ptr(std::make_unique<T>(std::move(data))) {}
    DPointer(const DPointer& other) : ptr(std::make_unique<T>(*other.ptr)) {}
    DPointer(DPointer&& other) noexcept : ptr(std::move(other.ptr)) {}
    ~DPointer() = default;

    DPointer& operator=(const DPointer& other) {
        if(&other != this) {
            ptr = std::make_unique<T>(*other.ptr);
        }
        return *this;
    }

    DPointer& operator=(DPointer&& other) noexcept {
        if(&other != this) {
            ptr = other.ptr;
        }
        return *this;
    }

    [[nodiscard]] const T* get() const noexcept { return ptr.get(); }
    [[nodiscard]] const T* operator->() const noexcept { return get(); }
    [[nodiscard]] const T& operator*() const noexcept { return *get(); }

    T* get() noexcept { return ptr.get(); }
    T* operator->() noexcept { return get(); }
    T& operator*() noexcept { return *get(); }

private:
    std::unique_ptr<T> ptr;
};

class tmx::Exception : public std::exception {
public:
    explicit Exception(std::string error) : error(std::move(error)) {}
    [[nodiscard]] const char* what() const noexcept override { return error.c_str(); }

private:
    std::string error;
};

enum class tmx::Type : unsigned char { EMPTY, STRING, INT, FLOAT, BOOL, COLOR, FILE, OBJECT, CLASS };

class tmx::PropertyValue {
    friend class Properties;

public:
    __NEOTMX_CLASS_HEADER_DEF__(PropertyValue)

    [[nodiscard]] Type type() const;
    [[nodiscard]] const std::string& stringValue() const;
    [[nodiscard]] int intValue() const;
    [[nodiscard]] float floatValue() const;
    [[nodiscard]] bool boolValue() const;
    [[nodiscard]] Color colorValue() const;
    [[nodiscard]] const std::string& fileValue() const;
    [[nodiscard]] int objectValue() const;
    [[nodiscard]] const Properties& classValue() const;

private:
    struct Data;
    DPointer<Data> d;
};

class tmx::Properties {
public:
    __NEOTMX_CLASS_HEADER_DEF__(Properties)

    [[nodiscard]] bool hasProperty(const std::string& name) const;
    [[nodiscard]] const PropertyValue& property(const std::string& name) const;
    [[nodiscard]] const std::map<std::string, PropertyValue>& properties() const;

protected:
    void parse(tinyxml2::XMLElement* root);

private:
    void parseProperty(tinyxml2::XMLElement* property);

    struct Data;
    DPointer<Data> d;
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
