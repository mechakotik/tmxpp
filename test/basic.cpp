#include <gtest/gtest.h>
#include <fstream>
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
}

TEST_F(BasicTest, Layers) {
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
