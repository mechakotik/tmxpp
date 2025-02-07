#ifndef NEOTMX_HPP
#define NEOTMX_HPP

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <utility>

#define __NEOTMX_CLASS_HEADER_DEF__(Type) \
    Type();                               \
    Type(const Type& other);              \
    Type(Type&& other) noexcept;          \
    Type& operator=(const Type&);         \
    Type& operator=(Type&&) noexcept;     \
    ~Type();

#define __NEOTMX_CLASS_HEADER_IMPL__(Type)                           \
    tmx::Type::Type() = default;                                     \
    tmx::Type::Type(const tmx::Type&) = default;                     \
    tmx::Type::Type(tmx::Type&&) noexcept = default;                 \
    tmx::Type& tmx::Type::operator=(const tmx::Type&) = default;     \
    tmx::Type& tmx::Type::operator=(tmx::Type&&) noexcept = default; \
    tmx::Type::~Type() = default;

namespace tinyxml2 {
    class XMLElement;
}

namespace tmx {
    struct Point;
    struct IntPoint;
    struct Color;

    template <typename T>
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

template <typename T>
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
            std::swap(ptr, other.ptr);
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

    __NEOTMX_CLASS_HEADER_DEF__(Map)

    void parseFromData(const std::string& data);
    void parseFromFile(const std::string& filename);

    [[nodiscard]] std::string version() const;
    [[nodiscard]] std::string tiledVersion() const;
    [[nodiscard]] std::string mapClass() const;
    [[nodiscard]] Orientation orientation() const;
    [[nodiscard]] RenderOrder renderOrder() const;
    [[nodiscard]] int compressionLevel() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] int tileWidth() const;
    [[nodiscard]] int tileHeight() const;
    [[nodiscard]] int hexSideLength() const;
    [[nodiscard]] StaggerAxis staggerAxis() const;
    [[nodiscard]] StaggerIndex staggerIndex() const;
    [[nodiscard]] Point parallaxOrigin() const;
    [[nodiscard]] Color backgroundColor() const;
    [[nodiscard]] bool infinite() const;

private:
    void parse(tinyxml2::XMLElement* root);

    struct Data;
    DPointer<Data> d;
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

    __NEOTMX_CLASS_HEADER_DEF__(Tileset)

    void parseFromData(const std::string& data);
    void parseFromFile(const std::string& filename);

    [[nodiscard]] int firstGID() const;
    [[nodiscard]] std::string source() const;
    [[nodiscard]] std::string name() const;
    [[nodiscard]] std::string tilesetClass() const;
    [[nodiscard]] int tileWidth() const;
    [[nodiscard]] int tileHeight() const;
    [[nodiscard]] int spacing() const;
    [[nodiscard]] int margin() const;
    [[nodiscard]] int tileCount() const;
    [[nodiscard]] int columns() const;
    [[nodiscard]] ObjectAlignment objectAlignment() const;
    [[nodiscard]] TileRenderSize tileRenderSize() const;
    [[nodiscard]] FillMode fillMode() const;

    [[nodiscard]] IntPoint tileOffset() const;
    [[nodiscard]] GridOrientation gridOrientation() const;
    [[nodiscard]] int gridWidth() const;
    [[nodiscard]] int gridHeight() const;

private:
    void parse(tinyxml2::XMLElement* root);

    struct Data;
    DPointer<Data> d;
};

#endif // NEOTMX_HPP
