#include <gtest/gtest.h>
#include <neotmx.hpp>
#include <fstream>

class BasicTest : public testing::Test {
protected:
    BasicTest() {
        map.parseFromFile("assets/pf1.tmx");
    }

    tmx::Map map;
};

TEST_F(BasicTest, TilemapAttributes) {
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

    // No properties
    EXPECT_EQ(map.properties().size(), 0);
    EXPECT_EQ(map.hasProperty("test"), false);
    EXPECT_EQ(map.property("test").type(), tmx::Type::EMPTY);

    // Default values
    EXPECT_EQ(map.mapClass(), "");
    EXPECT_EQ(map.compressionLevel(), -1);
    EXPECT_FLOAT_EQ(map.parallaxOrigin().x, 0);
    EXPECT_FLOAT_EQ(map.parallaxOrigin().y, 0);
}
