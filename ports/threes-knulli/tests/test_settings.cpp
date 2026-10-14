#include "test_main.hpp"
#include "ui.hpp"
using namespace triples;
TEST(settings_roundtrip_and_language) {
    Settings value; value.volume = 40; value.english = true; value.dark = false; value.tips_seen = true;
    auto loaded = Settings::parse(value.serialize());
    CHECK_EQ(loaded.volume, 40); CHECK(loaded.english);
    CHECK(!loaded.dark);
    CHECK(loaded.tips_seen);
    CHECK(!Settings::parse("volume=90\nlanguage=en\n").tips_seen);
    CHECK(Settings::parse("volume=90\nlanguage=en\n").dark);
    CHECK_EQ(std::string(translate(loaded, "Continuar", "Resume")), "Resume");
}
TEST(settings_corrupt_and_out_of_range) {
    CHECK_EQ(Settings::parse("volume=oops\nlanguage=fr\n").volume, 100);
    CHECK(!Settings::parse("language=fr\n").english);
    CHECK_EQ(Settings::parse("volume=-30\n").volume, 0);
    CHECK_EQ(Settings::parse("volume=500\n").volume, 100);
    CHECK_EQ(Settings::parse("volume=40junk\n").volume, 100);
    CHECK_EQ(Settings::parse("volume=99999999999999999999\n").volume, 100);
    CHECK_EQ(Settings::parse("volume=0\nlanguage=es\n").volume, 0);
}
