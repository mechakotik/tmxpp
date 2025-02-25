#include <gtest/gtest.h>
#include <neotmx.hpp>

class BasicTest : public testing::Test {
protected:
    BasicTest() { map.parseFromFile("assets/pf1.tmx"); }

    tmx::Map map;
};

TEST_F(BasicTest, Map) {
    // Set values
    EXPECT_EQ(map.version(), "1.10");
    EXPECT_EQ(map.tiledVersion(), "1.11.0");
    EXPECT_EQ(map.orientation(), tmx::Map::Orientation::ORTHOGONAL);
    EXPECT_EQ(map.renderOrder(), tmx::Map::RenderOrder::RIGHT_DOWN);
    EXPECT_EQ(map.width(), 128);
    EXPECT_EQ(map.height(), 28);
    EXPECT_EQ(map.tileWidth(), 16);
    EXPECT_EQ(map.tileHeight(), 16);
    EXPECT_EQ(map.infinite(), false);

    // Default values
    EXPECT_EQ(map.className(), "");
    EXPECT_EQ(map.compressionLevel(), -1);
    EXPECT_FLOAT_EQ(map.parallaxOrigin().x, 0);
    EXPECT_FLOAT_EQ(map.parallaxOrigin().y, 0);

    EXPECT_EQ(map.backgroundColor().r, 0);
    EXPECT_EQ(map.backgroundColor().g, 0);
    EXPECT_EQ(map.backgroundColor().b, 0);
    EXPECT_EQ(map.backgroundColor().a, 0);

    // No properties
    EXPECT_EQ(map.properties().size(), 0);
    EXPECT_EQ(map.hasProperty("test"), false);
    EXPECT_EQ(map.property("test").type(), tmx::Type::EMPTY);
}

TEST_F(BasicTest, Tileset) {
    // Tileset exists
    ASSERT_EQ(map.tilesets().size(), 1);
    tmx::Tileset tileset = map.tilesets()[0];

    // Set values
    EXPECT_EQ(tileset.name(), "tiles");
    EXPECT_EQ(tileset.firstGID(), 1);
    EXPECT_EQ(tileset.tileWidth(), 16);
    EXPECT_EQ(tileset.tileHeight(), 16);
    EXPECT_EQ(tileset.tileCount(), 132);
    EXPECT_EQ(tileset.columns(), 12);

    // No properties
    EXPECT_EQ(tileset.properties().size(), 0);
    EXPECT_EQ(tileset.hasProperty("test"), false);
    EXPECT_EQ(tileset.property("test").type(), tmx::Type::EMPTY);

    // Default values
    EXPECT_EQ(tileset.source(), "");
    EXPECT_EQ(tileset.className(), "");
    EXPECT_EQ(tileset.spacing(), 0);
    EXPECT_EQ(tileset.margin(), 0);
    EXPECT_EQ(tileset.objectAlignment(), tmx::Tileset::ObjectAlignment::UNSPECIFIED);
    EXPECT_EQ(tileset.tileRenderSize(), tmx::Tileset::TileRenderSize::TILE);
    EXPECT_EQ(tileset.fillMode(), tmx::Tileset::FillMode::STRETCH);

    // Image
    tmx::Image image = tileset.image();
    EXPECT_EQ(image.type(), tmx::Image::Type::EXTERNAL);
    EXPECT_EQ(image.source(), "pf1.png");
    EXPECT_EQ(image.width(), 192);
    EXPECT_EQ(image.height(), 176);
}

TEST_F(BasicTest, Tile) {
    // Tileset and required tiles exist
    ASSERT_EQ(map.tilesets().size(), 1);
    std::vector<tmx::Tile> tiles = map.tilesets()[0].tiles();

    std::map<int, tmx::Tile> tile;
    for(tmx::Tile tiles : map.tilesets()[0].tiles()) {
        tile[tiles.id()] = tiles;
    }

    EXPECT_EQ(tile[6].id(), 6);
    EXPECT_EQ(tile[6].className(), "");
    EXPECT_EQ(tile[6].position().x, 0);
    EXPECT_EQ(tile[6].position().y, 0);
    EXPECT_EQ(tile[6].width(), 0);
    EXPECT_EQ(tile[6].height(), 0);

    EXPECT_EQ(tile[7].id(), 7);
    EXPECT_EQ(tile[8].id(), 8);
    EXPECT_EQ(tile[10].id(), 10);
    EXPECT_EQ(tile[11].id(), 11);

    auto animation = tile[120].animation();
    EXPECT_EQ(animation.size(), 3);
    EXPECT_EQ(animation[0].id, 120);
    EXPECT_EQ(animation[1].id, 121);
    EXPECT_EQ(animation[2].id, 122);
    EXPECT_EQ(animation[0].duration, 166);
    EXPECT_EQ(animation[1].duration, 166);
    EXPECT_EQ(animation[2].duration, 166);
}

TEST_F(BasicTest, Layer) {
    ASSERT_EQ(map.layers().size(), 3);
    ASSERT_EQ(map.layers()[0].type(), tmx::Layer::Type::TILE);
    ASSERT_EQ(map.layers()[1].type(), tmx::Layer::Type::TILE);
    ASSERT_EQ(map.layers()[2].type(), tmx::Layer::Type::TILE);

    tmx::TileLayer layer1 = map.layers()[0].tileLayer();
    EXPECT_EQ(layer1.id(), 3);
    EXPECT_EQ(layer1.name(), "layer2");
    EXPECT_EQ(layer1.width(), 128);
    EXPECT_EQ(layer1.height(), 28);
    EXPECT_EQ(layer1.className(), "");
    EXPECT_EQ(layer1.encoding(), "csv");
    EXPECT_EQ(layer1.compression(), "");
    EXPECT_EQ(layer1.at(0, 0), 121);
    EXPECT_EQ(layer1.at(127, 27), 121);
    EXPECT_EQ(layer1.properties().size(), 0);

    tmx::TileLayer layer2 = map.layers()[1].tileLayer();
    EXPECT_EQ(layer2.id(), 2);
    EXPECT_EQ(layer2.name(), "layer1");
    EXPECT_EQ(layer2.width(), 128);
    EXPECT_EQ(layer2.height(), 28);
    EXPECT_EQ(layer2.className(), "");
    EXPECT_EQ(layer2.encoding(), "csv");
    EXPECT_EQ(layer2.compression(), "");
    EXPECT_EQ(layer2.at(0, 0), 16);
    EXPECT_EQ(layer2.at(127, 0), 5);
    EXPECT_EQ(layer2.at(0, 27), 3);
    EXPECT_EQ(layer2.at(127, 27), 3);

    EXPECT_EQ(layer2.at(2, 0), 3);
    EXPECT_EQ(layer2.flipHorizontal(2, 0), true);
    EXPECT_EQ(layer2.flipVertical(2, 0), false);
    EXPECT_EQ(layer2.flipDiagonal(2, 0), false);
    EXPECT_EQ(layer2.at(5, 0), 3);
    EXPECT_EQ(layer2.flipHorizontal(5, 0), true);
    EXPECT_EQ(layer2.flipVertical(5, 0), true);
    EXPECT_EQ(layer2.flipDiagonal(5, 0), false);
    EXPECT_EQ(layer2.at(1, 2), 3);
    EXPECT_EQ(layer2.flipHorizontal(1, 2), true);
    EXPECT_EQ(layer2.flipVertical(1, 2), false);
    EXPECT_EQ(layer2.flipDiagonal(1, 2), true);

    EXPECT_EQ(layer2.properties().size(), 1);
    EXPECT_EQ(layer2.hasProperty("collision"), true);
    EXPECT_EQ(layer2.property("collision").type(), tmx::Type::BOOL);
    EXPECT_EQ(layer2.property("collision").boolValue(), true);
    EXPECT_EQ(layer2.hasProperty("test"), false);
    EXPECT_EQ(layer2.property("test").type(), tmx::Type::EMPTY);

    tmx::TileLayer layer3 = map.layers()[2].tileLayer();
    EXPECT_EQ(layer3.id(), 1);
    EXPECT_EQ(layer3.name(), "layer0");
    EXPECT_EQ(layer3.width(), 128);
    EXPECT_EQ(layer3.height(), 28);
    EXPECT_EQ(layer3.className(), "");
    EXPECT_EQ(layer3.encoding(), "csv");
    EXPECT_EQ(layer3.compression(), "");
    EXPECT_EQ(layer3.at(0, 0), 0);
    EXPECT_EQ(layer3.at(127, 0), 0);
    EXPECT_EQ(layer3.at(115, 27), 3);
    EXPECT_EQ(layer3.at(127, 27), 0);
    EXPECT_EQ(layer3.properties().size(), 0);
}

TEST_F(BasicTest, PolygonObject) {
    ASSERT_EQ(map.tilesets().size(), 1);
    std::vector<tmx::Tile> tiles = map.tilesets()[0].tiles();

    std::map<int, tmx::Tile> tile;
    for(tmx::Tile tiles : map.tilesets()[0].tiles()) {
        tile[tiles.id()] = tiles;
    }

    std::vector<tmx::Object> objects = tile[6].objectGroup().objects();
    ASSERT_EQ(objects.size(), 1);
    ASSERT_EQ(objects[0].type(), tmx::Object::Type::POLYGON);

    tmx::Polygon polygon = objects[0].polygon();
    ASSERT_EQ(polygon.size(), 4);
    ASSERT_DOUBLE_EQ(polygon[0].x, 0);
    ASSERT_DOUBLE_EQ(polygon[0].y, 0);
    ASSERT_DOUBLE_EQ(polygon[1].x, 16);
    ASSERT_DOUBLE_EQ(polygon[1].y, 0);
    ASSERT_DOUBLE_EQ(polygon[2].x, 16);
    ASSERT_DOUBLE_EQ(polygon[2].y, 16);
    ASSERT_DOUBLE_EQ(polygon[3].x, 0);
    ASSERT_DOUBLE_EQ(polygon[3].y, 16);
}
