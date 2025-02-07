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
    EXPECT_EQ(map.version(), "1.10");
    EXPECT_EQ(map.tiledVersion(), "1.11.0");
    EXPECT_EQ(map.orientation(), tmx::Map::Orientation::ORTHOGONAL);
    EXPECT_EQ(map.renderOrder(), tmx::Map::RenderOrder::RIGHT_DOWN);
    EXPECT_EQ(map.width(), 128);
    EXPECT_EQ(map.height(), 28);
    EXPECT_EQ(map.tileWidth(), 16);
    EXPECT_EQ(map.tileHeight(), 16);
    EXPECT_EQ(map.infinite(), false);
}
