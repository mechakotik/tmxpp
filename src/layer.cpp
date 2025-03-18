#include <tmxpp.hpp>
#include <utility>
#include <variant>

struct tmx::Layer::Data {
    Type type = Type::EMPTY;
    std::variant<TileLayer, ObjectGroup> layer;
};

__TMXPP_CLASS_HEADER_IMPL__(tmx, Layer)

tmx::Layer::Layer(TileLayer&& layer) : Layer() {
    d->type = Type::TILE;
    d->layer = std::move(layer);
}

tmx::Layer::Layer(ObjectGroup&& layer) : Layer() {
    d->type = Type::OBJECT;
    d->layer = std::move(layer);
}

tmx::Layer::Type tmx::Layer::type() const { return d->type; }

const tmx::TileLayer& tmx::Layer::tileLayer() const {
    ensureType(Type::TILE);
    return std::get<TileLayer>(d->layer);
}

const tmx::ObjectGroup& tmx::Layer::objectGroup() const {
    ensureType(Type::OBJECT);
    return std::get<ObjectGroup>(d->layer);
}

void tmx::Layer::ensureType(Type type) const {
    if(d->type != type) {
        throw Exception("Attempt to extract " + typeName(type) + " while storing " + typeName(d->type));
    }
}

std::string tmx::Layer::typeName(Type type) {
    switch(type) {
        case Type::EMPTY:
            return "nothing";
        case Type::TILE:
            return "tile layer";
        case Type::OBJECT:
            return "object group";
        default:
            return "unknown";
    }
}
