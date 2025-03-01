#include <gtest/gtest.h>
#include <tmxpp.hpp>

#ifdef TMXPP_BASE64

class Base64Test : public testing::Test {
protected:
    Base64Test() { map.parseFromFile("assets/pf1_base64.tmx"); }

    tmx::Map map;
};

TEST_F(Base64Test, Layer) {
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
    EXPECT_EQ(layer1.encoding(), "base64");
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
    EXPECT_EQ(layer2.encoding(), "base64");
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
    EXPECT_EQ(layer3.encoding(), "base64");
    EXPECT_EQ(layer3.compression(), "");
    EXPECT_EQ(layer3.at(0, 0), 0);
    EXPECT_EQ(layer3.at(127, 0), 0);
    EXPECT_EQ(layer3.at(115, 27), 3);
    EXPECT_EQ(layer3.at(127, 27), 0);
    EXPECT_EQ(layer3.properties().size(), 0);
}

#endif
