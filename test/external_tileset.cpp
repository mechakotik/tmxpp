#include <gtest/gtest.h>
#include <tmxpp.hpp>

class ExternalTilesetTest : public testing::Test {
protected:
    ExternalTilesetTest() { map.parseFromFile("assets/pf1_external.tmx"); }

    tmx::Map map;
};

TEST_F(ExternalTilesetTest, Tileset) {
    // Tileset exists
    ASSERT_EQ(map.tilesets().size(), 1);
    tmx::Tileset tileset = map.tilesets()[0];

    // Set values
    EXPECT_EQ(tileset.source(), "pf1_external.tsx");
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
