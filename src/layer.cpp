#include <neotmx.hpp>
#include <variant>

struct tmx::Layer::Data {
    Type type = Type::EMPTY;
    std::variant<TileLayer> layer;
};

__NEOTMX_CLASS_HEADER_IMPL__(tmx, Layer)

tmx::Layer::Layer(TileLayer&& layer) : Layer() {
    d->type = Type::TILE;
    d->layer = std::move(layer);
}

tmx::Layer::Type tmx::Layer::type() const { return d->type; }

const tmx::TileLayer& tmx::Layer::tileLayer() const {
    if(d->type != Type::TILE) {
        throwWrongType(Type::TILE);
    }
    return std::get<TileLayer>(d->layer);
}

void tmx::Layer::throwWrongType(Type wanted) const {
    throw Exception("Attempt to extract " + typeName(wanted) + " while storing " + typeName(d->type));
}

std::string tmx::Layer::typeName(Type type) {
    switch(type) {
        case Type::EMPTY:
            return "nothing";
        case Type::TILE:
            return "tile layer";
        default:
            return "unknown";
    }
}
