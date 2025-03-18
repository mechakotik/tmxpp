#ifndef TMXPP_HPP
#define TMXPP_HPP

#include <exception>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#define __TMXPP_CLASS_HEADER_DEF__(Type) \
    Type();                              \
    Type(const Type& other);             \
    Type(Type&& other) noexcept;         \
    Type& operator=(const Type&);        \
    Type& operator=(Type&&) noexcept;    \
    ~Type();

#define __TMXPP_CLASS_HEADER_IMPL__(Namespace, Type)                                   \
    Namespace::Type::Type() = default;                                                 \
    Namespace::Type::Type(const Namespace::Type&) = default;                           \
    Namespace::Type::Type(Namespace::Type&&) noexcept = default;                       \
    Namespace::Type& Namespace::Type::operator=(const Namespace::Type&) = default;     \
    Namespace::Type& Namespace::Type::operator=(Namespace::Type&&) noexcept = default; \
    Namespace::Type::~Type() = default;

namespace tinyxml2 {
    class XMLElement;
}

namespace tmx {
    struct Point;
    struct IntPoint;
    struct Ellipse;
    struct Color;

    using Polygon = std::vector<Point>;
    using Polyline = std::vector<Point>;

    Color colorFromString(const std::string& str);

    enum class Type : unsigned char;

    using LoaderType = std::function<std::string(std::filesystem::path)>;

    class Exception;
    class PropertyValue;
    class Properties;
    class Map;
    class Tileset;
    class Tile;
    class Image;
    class TileLayer;
    class ObjectGroup;
    class Object;
    class Layer;

    namespace internal {
        template <typename T>
        class DPointer;

        class AbstractLayer;
        class DataBlock;
    }
} // namespace tmx

struct tmx::Point {
    double x = 0;
    double y = 0;
};

struct tmx::IntPoint {
    int x = 0;
    int y = 0;
};

struct tmx::Ellipse {
    Point center;
    Point size;
};

struct tmx::Color {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;
};

class tmx::Exception : public std::exception {
public:
    explicit Exception(std::string error) : error(std::move(error)) {}
    [[nodiscard]] const char* what() const noexcept override { return error.c_str(); }

private:
    std::string error;
};

template <typename T>
class tmx::internal::DPointer {
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

enum class tmx::Type : unsigned char { EMPTY, STRING, INT, FLOAT, BOOL, COLOR, FILE, OBJECT, CLASS };

class tmx::PropertyValue {
    friend class Properties;

public:
    __TMXPP_CLASS_HEADER_DEF__(PropertyValue)

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
    internal::DPointer<Data> d;
};

class tmx::Properties {
public:
    __TMXPP_CLASS_HEADER_DEF__(Properties)

    [[nodiscard]] bool hasProperty(const std::string& name) const;
    [[nodiscard]] const PropertyValue& property(const std::string& name) const;
    [[nodiscard]] const std::map<std::string, PropertyValue>& properties() const;

protected:
    void parse(tinyxml2::XMLElement* root);

private:
    void parseProperty(tinyxml2::XMLElement* property);

    struct Data;
    internal::DPointer<Data> d;
};

class tmx::Map : public Properties {
public:
    enum class Orientation : unsigned char { ORTHOGONAL, ISOMETRIC, STAGGERED, HEXAGONAL };
    enum class RenderOrder : unsigned char { RIGHT_DOWN, RIGHT_UP, LEFT_DOWN, LEFT_UP };
    enum class StaggerAxis : unsigned char { X_AXIS, Y_AXIS };
    enum class StaggerIndex : unsigned char { EVEN, ODD };

    __TMXPP_CLASS_HEADER_DEF__(Map)

    void parseFromData(const std::string& data);
    void parseFromFile(std::filesystem::path path, LoaderType loader = nullptr);

    [[nodiscard]] std::string version() const;
    [[nodiscard]] std::string tiledVersion() const;
    [[nodiscard]] std::string className() const;
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

    [[nodiscard]] const std::vector<Tileset>& tilesets() const;
    [[nodiscard]] const std::vector<Layer>& layers() const;

private:
    void parse(tinyxml2::XMLElement* root);
    void parseTilesets(tinyxml2::XMLElement* root);
    void parseLayers(tinyxml2::XMLElement* root);

    struct Data;
    internal::DPointer<Data> d;
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

    __TMXPP_CLASS_HEADER_DEF__(Tileset)

    void parseFromData(const std::string& data);
    void parseFromFile(std::filesystem::path path, LoaderType loader = nullptr);

    [[nodiscard]] int firstGID() const;
    [[nodiscard]] std::string source() const;
    [[nodiscard]] std::string name() const;
    [[nodiscard]] std::string className() const;
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

    [[nodiscard]] const Image& image() const;
    [[nodiscard]] const std::vector<Tile>& tiles() const;

private:
    void parse(tinyxml2::XMLElement* root);
    void parseTiles(tinyxml2::XMLElement* root);

    struct Data;
    internal::DPointer<Data> d;
};

class tmx::Tile : public Properties {
    friend class Tileset;

public:
    struct AnimationFrame {
        int id = 0;
        int duration = 0;
    };

    __TMXPP_CLASS_HEADER_DEF__(Tile)

    [[nodiscard]] int id() const;
    [[nodiscard]] std::string className() const;
    [[nodiscard]] IntPoint position() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

    [[nodiscard]] Image image() const;
    [[nodiscard]] const ObjectGroup& objectGroup() const;
    [[nodiscard]] const std::vector<AnimationFrame>& animation() const;

private:
    void parse(tinyxml2::XMLElement* root);
    void parseAnimation(tinyxml2::XMLElement* root);

    struct Data;
    internal::DPointer<Data> d;
};

class tmx::Image {
    friend class Tileset;
    friend class Tile;

public:
    enum class Type : unsigned char { EMPTY, EXTERNAL, EMBEDDED };

    __TMXPP_CLASS_HEADER_DEF__(Image)

    [[nodiscard]] Type type() const;
    [[nodiscard]] const std::string& source() const;
    [[nodiscard]] Color transparentColor() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

private:
    void parse(tinyxml2::XMLElement* root);

    struct Data;
    internal::DPointer<Data> d;
};

class tmx::internal::AbstractLayer : public Properties {
public:
    __TMXPP_CLASS_HEADER_DEF__(AbstractLayer)

    [[nodiscard]] int id() const;
    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] const std::string& className() const;
    [[nodiscard]] double opacity() const;
    [[nodiscard]] bool visible() const;
    [[nodiscard]] Color tintColor() const;
    [[nodiscard]] Point offset() const;
    [[nodiscard]] Point parallaxFactor() const;

protected:
    void parse(tinyxml2::XMLElement* root);

private:
    struct Data;
    internal::DPointer<Data> d;
};

class tmx::TileLayer : public internal::AbstractLayer {
    friend class Map;

public:
    __TMXPP_CLASS_HEADER_DEF__(TileLayer)

    [[nodiscard]] const std::vector<std::vector<unsigned int>>& data() const;
    [[nodiscard]] int at(int x, int y) const;
    [[nodiscard]] bool flipHorizontal(int x, int y) const;
    [[nodiscard]] bool flipVertical(int x, int y) const;
    [[nodiscard]] bool flipDiagonal(int x, int y) const;
    [[nodiscard]] bool rotateHex120(int x, int y) const;

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] std::string encoding() const;
    [[nodiscard]] std::string compression() const;

private:
    void parse(tinyxml2::XMLElement* root);
    void parseData(tinyxml2::XMLElement* root);
    void parseCSVData(const std::string& str);
    static std::string decompressData(std::string& str, std::string compression);
    void parseBase64Data(const std::string& str);
    void checkBounds(int x, int y) const;

    struct Data;
    internal::DPointer<Data> d;
};

class tmx::ObjectGroup : public internal::AbstractLayer {
    friend class Map;
    friend class Tile;

public:
    enum class DrawOrder : unsigned char { TOPDOWN, INDEX };

    __TMXPP_CLASS_HEADER_DEF__(ObjectGroup)

    [[nodiscard]] Color color() const;
    [[nodiscard]] DrawOrder drawOrder() const;
    [[nodiscard]] const std::vector<Object>& objects() const;

private:
    void parse(tinyxml2::XMLElement* root);

    struct Data;
    internal::DPointer<Data> d;
};

class tmx::Object : public Properties {
    friend class ObjectGroup;

public:
    enum class Type { EMPTY, ELLIPSE, POINT, POLYGON, POLYLINE, TEXT };

    __TMXPP_CLASS_HEADER_DEF__(Object)

    [[nodiscard]] int id() const;
    [[nodiscard]] std::string name() const;
    [[nodiscard]] std::string className() const;
    [[nodiscard]] Point position() const;
    [[nodiscard]] Point size() const;
    [[nodiscard]] double rotation() const;
    [[nodiscard]] int gid() const;
    [[nodiscard]] bool visible() const;

    [[nodiscard]] Type type() const;
    [[nodiscard]] Ellipse ellipse() const;
    [[nodiscard]] Point point() const;
    [[nodiscard]] const Polygon& polygon() const;
    [[nodiscard]] const Polyline& polyline() const;

private:
    void parse(tinyxml2::XMLElement* root);
    static std::vector<Point> parsePoints(const std::string& str);
    void ensureType(Type type) const;
    static std::string typeName(Type type);

    struct Data;
    internal::DPointer<Data> d;
};

class tmx::Layer {
public:
    enum class Type : uint8_t { EMPTY, TILE, OBJECT };

    __TMXPP_CLASS_HEADER_DEF__(Layer)

    explicit Layer(TileLayer&& layer);
    explicit Layer(ObjectGroup&& layer);

    [[nodiscard]] Type type() const;
    [[nodiscard]] const TileLayer& tileLayer() const;
    [[nodiscard]] const ObjectGroup& objectGroup() const;

private:
    void ensureType(Type type) const;
    static std::string typeName(Type type);

    struct Data;
    internal::DPointer<Data> d;
};

#endif // TMXPP_HPP
