<h1 align="center">tmx++</h1>

<div align="center">

![Endpoint Badge](https://img.shields.io/endpoint?url=https%3A%2F%2Fghloc.vercel.app%2Fapi%2Fmechakotik%2Ftmxpp%2Fbadge%3Fbranch%3Dmain%26filter%3D.cpp%252C.hpp%252C%2521test&label=lines%20of%20code&color=4dc71f)
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/mechakotik/tmxpp/cmake-multi-platform.yml)

</div>

tmx++ is a C++ 20 library for parsing TMX tilemaps. It supports TMX map format as of Tiled 1.11.2, with a few limitations. tmx++ is used by Tails Adventure Remake, was tested to work on Windows, Linux and Android.

## Including in CMake project

The easiest way is to add this repository as a submodule (e.g. in `external/tmxpp`) and include it in CMakeLists using `add_subdirectory`. This will just work on all platforms and have all the dependencies included. You may also disable unneeded compression methods to cut down binary size.

```cmake
# Disable compression methods you don't use
set(TMXPP_BASE64 ON)
set(TMXPP_ZLIB ON)
set(TMXPP_ZSTD ON)
add_subdirectory(external/tmxpp EXCLUDE_FROM_ALL)

target_link_libraries(myproject PRIVATE tmxpp)
target_include_directories(myproject PRIVATE external/tmxpp/include)
```

## Installing system-wide

Installation process is pretty standard for CMake projects.

```sh
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DTMXPP_INSTALL=ON -DTMXPP_VENDORED=OFF
cmake --build .
sudo cmake --install .
```

Here `TMXPP_VENDORED=OFF` enables linking to external libraries, so it requires dependent libraries to be installed in system. You may also set ``TMXPP_VENDORED=ON`` here to use included dependencies.

## Usage example

Here is a basic usage example to quickly get started. ~~I am too lazy to write the docs~~ Header file contains almost no implementation details and can be used as API reference.

```c++
tmx::Map map;
try {
    map.parseFromFile("maps/pf/pf1.tmx");
} catch(tmx::Exception& e) {
    std::cout << e.what() << std::endl;
    return;
}

tmx::Tileset tileset = map.tilesets().front();
std::filesystem::path image = tileset.image().source();

for(const tmx::Tile& tile : tileset.tiles()) {
    for(int i = 0; i < tile.animation().size(); i++) {
        int id = tile.animation().at(i).id;
        int duraion = tile.animation().at(i).duration;
    }
    if(tile.hasProperty("type") && tile.property("type").type() == tmx::Type::INT) {
        int type = tile.property("type").intValue();
    }
}

for(const tmx::Layer& layer : map.layers()) {
    if(layer.type() == tmx::Layer::Type::TILE) {
        const tmx::TileLayer& tileLayer = layer.tileLayer();
        for(int x = 0; x < tileLayer.width(); x++) {
            for(int y = 0; y < tileLayer.height(); y++) {
                int gid = tileLayer.at(x, y);
            }
        }
    } else if(layer.type() == tmx::Layer::Type::OBJECT) {
        const tmx::ObjectGroup& objectGroup = layer.objectGroup();
        for(const tmx::Object& object : objectGroup.objects()) {
            tmx::Point pos = object.position();
            if(object.type() == tmx::Object::Type::POLYGON) {
                tmx::Polygon polygon = object.polygon();
            }
        }
    }
}
```

## Unsupported

- Infinite maps (TODO)
- Image layers (TODO)
- Object templates (TODO, when I figure out a good way to implement it)
- Embedded images (not planned, as not supported by Tiled itself while being in TMX specification)
- Terrains and wang sets (not planned, used to build maps and probably irrelevant when rendering them)
