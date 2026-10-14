#pragma once
#include <algorithm>
#include <charconv>
#include <string>
#include <string_view>
namespace triples {
struct Settings {
    int volume = 100;
    bool english = false;
    bool dark = true;
    bool tips_seen = false;
    bool tate = false;
    std::string serialize() const { return "volume=" + std::to_string(volume) + "\nlanguage=" + (english ? "en" : "es") + "\ntheme=" + (dark ? "dark" : "light") + "\ntips_seen=" + (tips_seen ? "1" : "0") + "\ntate=" + (tate ? "1" : "0") + "\n"; }
    static Settings parse(std::string_view text) {
        Settings result;
        while (!text.empty()) {
            auto end = text.find('\n');
            auto line = text.substr(0, end);
            if (line.starts_with("volume=")) {
                int value; auto digits = line.substr(7);
                auto parsed = std::from_chars(digits.data(), digits.data() + digits.size(), value);
                if (parsed.ec == std::errc{} && parsed.ptr == digits.data() + digits.size()) result.volume = std::clamp(value, 0, 100);
            }
            if (line == "language=en") result.english = true;
            if (line == "language=es") result.english = false;
            if (line == "theme=dark") result.dark = true;
            if (line == "theme=light") result.dark = false;
            if (line == "tips_seen=1") result.tips_seen = true;
            if (line == "tate=1") result.tate = true;
            if (end == std::string_view::npos) break;
            text.remove_prefix(end + 1);
        }
        return result;
    }
};
struct Menu {
    bool open = false;
    bool credits = false;
    int selected = 0;
    int credit_page = 0;
};
inline const char* translate(const Settings& settings, const char* spanish, const char* english) {
    return settings.english ? english : spanish;
}
}
