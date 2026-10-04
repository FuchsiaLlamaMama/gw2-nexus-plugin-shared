// gw2-nexus-plugin-shared/theme — the cross-addon native-look theme layer.
//
// The default GW2-native look every addon inherits, so each plugin reads as
// part of one UI rather than reinventing its own chrome. Token values come from
// the reference design; comments give each token's source hex/rgba.
//
// NOTE: a block of left-pane / category tokens lives further down, clearly
// demarcated. They were added ahead of a full palette pass so those views can
// draw without hardcoding colours.
//
// This header is deliberately ImGui-free and pure C++17: the palette is plain
// data so it unit-tests off-game (macOS/clang), and the ImGui glue in the addon
// DLLs (Windows-only) converts Color -> ImU32/ImVec4 at the draw site. See
// theme_imgui.h for that adapter and nine_slice.h for the frame geometry.

#pragma once

#include <string>

namespace shared::theme {

// 8-bit RGBA color, 0-255 per channel. Alpha carries the design's translucency
// (e.g. panel fill is 90% opaque -> a = 230). Kept ImGui-free on purpose.
struct Color
{
    unsigned char r = 0, g = 0, b = 0, a = 255;

    constexpr bool operator==(const Color& o) const
    {
        return r == o.r && g == o.g && b == o.b && a == o.a;
    }
    constexpr bool operator!=(const Color& o) const { return !(*this == o); }
};

// Convert a 0.0-1.0 opacity to an 8-bit alpha with round-to-nearest. constexpr
// so palette entries are compile-time constants (0.90 -> 230, not 229).
constexpr unsigned char alpha8(double opacity)
{
    return static_cast<unsigned char>(opacity * 255.0 + 0.5);
}

// The layered gold/bronze frame the design builds from stacked box-shadow
// rings. The primitive-drawn default frame (theme_imgui.h) reproduces it as
// concentric ImDrawList rects, outermost first.
struct FrameRings
{
    Color outer_glow;   // rgba(120,96,56,0.32) — soft gold halo
    Color bronze_band;  // rgba(74,58,34,0.7)   — structural bronze ring
    Color separator;    // rgba(18,15,9,0.9)    — near-black seam
    Color inner_bevel;  // rgba(226,196,124,0.22) — top gold highlight
};

// Named palette — the tokens a themed panel consumes.
struct Palette
{
    // Surfaces
    Color panel_bg;      // panel #191611 @ 0.90 (translucent overlay)
    Color card_bg;       // card-top #211e18 @ 0.96 (card, gradient top)
    Color card_bottom;   // card-bottom #1a1712 @ 0.96 (card, gradient bottom)
    Color form_top;      // form-top #262421 @ 0.92 (editor form, gradient top)
    Color form_bottom;   // form-bottom #1c1913 @ 0.92 (editor form, gradient bottom)
    Color input_bg;      // rgba(0,0,0,0.32)
    Color titlebar_bg;   // rgba(30,27,21,0.90)

    // Border / trim
    Color border;        // panel border: gold-line #b4965a @ 0.40
    Color trim_line;     // rgba(150,120,70,0.28) — separators, hairlines
    FrameRings rings;

    // Text
    Color text;          // #cfc7b6 — body
    Color text_title;    // #ecdcae — card/section titles
    Color text_gold;     // #e6c86a — headings, primary trim
    Color text_muted;    // rgba(200,192,174,0.85) — brightened from (190,180,160,0.6)
    Color text_muted_2;  // #d2c3a0 — icon-button glyphs (muted-2)

    // Tack pin (radial: gold-btn -> pin-dark)
    Color tack_light;    // #f0d78a — tack highlight
    Color tack_dark;     // #8a6f2e — tack shadow

    // Danger (delete-confirm strip + delete button)
    Color danger_line;   // #c86e5a — confirm strip border (drawn @ ~40%)
    Color danger_fill;   // #782820 — Delete button fill
    Color danger_bg;     // #3c1814 — confirm strip background
    Color danger_text;   // #e8b8ac — confirm copy
    Color danger_text_2; // #ffd8cd — Delete button label

    // Interactive (primary button)
    Color button;        // rgba(74,62,38,0.9)
    Color button_hovered;// rgba(90,74,44,0.95) — brighter bronze on hover
    Color button_active; // rgba(44,37,22,0.9)  — gradient bottom / pressed
    Color button_text;   // #f0d78a
    Color button_border; // rgba(180,150,90,0.55)

    // Scrollbar
    Color scrollbar_bg;      // rgba(0,0,0,0.3)
    Color scrollbar_grab;    // rgba(180,150,90,0.28)
    Color scrollbar_grab_hovered; // rgba(180,150,90,0.45)

    // Accents (semantic colors reused across addons)
    Color accent_teal;   // #7fd0d6 — coordinates / links
    Color accent_green;  // #9fd8b0 — per-character scope; Gathering category (char_green)
    Color accent_blue;   // #a7c4ea — map/zone scope; Dailies category (zone_blue)
    Color accent_danger; // #e8998a — delete / danger

    // Left-pane / category tokens. Kept in the palette so no colour is
    // hard-coded at the draw site. The reused tokens (Crafting gold ->
    // text_gold, char_green -> accent_green, zone_blue -> accent_blue,
    // gold-line -> border) are NOT duplicated here.
    Color sheen;         // #ffffff — white selection/hover fills (used @ low alpha)
    Color brass;         // #96784b — default borders, dividers, hairlines
    Color gold_title;    // #ffeebb — titles, headings, item titles
    Color amber;         // #ffcc77 — active nav/selection text + left border
    Color nav_idle;      // #e7e2d5 — left-tree nav rows, idle
    Color note_row_idle; // #c8c2b4 — left-tree item rows, idle
    Color muted;         // #beb4a0 — secondary labels, kickers, counts (full alpha)
    Color count_idle;    // #969696 — count badge, idle
    Color legend_purple; // #c9a3e6 — Legendaries category
    Color char_orange;   // #e0a58a — Characters category
};

// Spacing / rounding metrics (px), taken from the reference design.
struct Metrics
{
    float window_rounding = 2.0f;  // panel corner radius
    float frame_rounding  = 3.0f;  // controls / cards / inputs
    float border_size     = 1.0f;  // structural border width
    float window_padding  = 15.0f; // 14-16px content padding
    float item_spacing_y  = 12.0f; // gap between cards
    float scrollbar_size  = 10.0f; // scrollbar width
    float ring_width      = 1.0f;  // per-ring thickness for the drawn frame
};

// The default GW2-native theme — always available and always shippable.
constexpr Palette gw2_palette()
{
    Palette p{};

    p.panel_bg    = {25, 22, 17, alpha8(0.90)}; // panel #191611
    p.card_bg     = {33, 30, 24, alpha8(0.96)}; // card-top #211e18
    p.card_bottom = {26, 23, 18, alpha8(0.96)}; // card-bottom #1a1712
    p.form_top    = {38, 36, 33, alpha8(0.92)}; // form-top #262421
    p.form_bottom = {28, 25, 19, alpha8(0.92)}; // form-bottom #1c1913
    p.input_bg    = {0, 0, 0, alpha8(0.32)};
    p.titlebar_bg = {30, 27, 21, alpha8(0.90)};

    p.border    = {180, 150, 90, alpha8(0.40)}; // panel border gold-line #b4965a @ 40%
    p.trim_line = {150, 120, 70, alpha8(0.28)};

    p.rings.outer_glow  = {120, 96, 56, alpha8(0.32)};
    p.rings.bronze_band = {74, 58, 34, alpha8(0.7)};
    p.rings.separator   = {18, 15, 9, alpha8(0.9)};
    p.rings.inner_bevel = {226, 196, 124, alpha8(0.22)};

    p.text        = {207, 199, 182, 255}; // #cfc7b6
    p.text_title  = {236, 220, 174, 255}; // #ecdcae
    p.text_gold   = {230, 200, 106, 255}; // #e6c86a
    // Brightened: secondary text was too subdued in-game to read.
    // Was {190,180,160} @ 0.6.
    p.text_muted  = {200, 192, 174, alpha8(0.85)};
    p.text_muted_2 = {210, 195, 160, 255}; // #d2c3a0

    p.tack_light = {240, 215, 138, 255}; // #f0d78a
    p.tack_dark  = {138, 111, 46, 255};  // #8a6f2e

    p.danger_line   = {200, 110, 90, 255};        // #c86e5a
    p.danger_fill   = {120, 40, 32, alpha8(0.90)}; // #782820
    p.danger_bg     = {60, 24, 20, alpha8(0.90)};  // #3c1814
    p.danger_text   = {232, 184, 172, 255};       // #e8b8ac
    p.danger_text_2 = {255, 216, 205, 255};       // #ffd8cd

    p.button         = {74, 62, 38, alpha8(0.9)};
    p.button_hovered = {90, 74, 44, alpha8(0.95)};
    p.button_active  = {44, 37, 22, alpha8(0.9)};
    p.button_text    = {240, 215, 138, 255}; // #f0d78a
    p.button_border  = {180, 150, 90, alpha8(0.55)};

    p.scrollbar_bg           = {0, 0, 0, alpha8(0.3)};
    p.scrollbar_grab         = {180, 150, 90, alpha8(0.28)};
    p.scrollbar_grab_hovered = {180, 150, 90, alpha8(0.45)};

    p.accent_teal   = {127, 208, 214, 255}; // #7fd0d6
    p.accent_green  = {159, 216, 176, 255}; // #9fd8b0
    p.accent_blue   = {167, 196, 234, 255}; // #a7c4ea
    p.accent_danger = {232, 153, 138, 255}; // #e8998a

    // Left-pane / category tokens.
    p.sheen         = {255, 255, 255, 255}; // #ffffff (applied at low alpha)
    p.brass         = {150, 120, 75,  255}; // #96784b
    p.gold_title    = {255, 238, 187, 255}; // #ffeebb
    p.amber         = {255, 204, 119, 255}; // #ffcc77
    p.nav_idle      = {231, 226, 213, 255}; // #e7e2d5
    p.note_row_idle = {200, 194, 180, 255}; // #c8c2b4
    p.muted         = {190, 180, 160, 255}; // #beb4a0
    p.count_idle    = {150, 150, 150, 255}; // #969696
    p.legend_purple = {201, 163, 230, 255}; // #c9a3e6
    p.char_orange   = {224, 165, 138, 255}; // #e0a58a

    return p;
}

constexpr Metrics gw2_metrics() { return Metrics{}; }

// Resolve a category's `color_token` (a category names a palette colour by
// string, never a raw RGBA) to the corresponding Palette colour, so the draw
// site never hardcodes a colour. Covers the 5 built-in tokens; an unknown/user
// token falls back to a visible default (gold).
inline Color category_color(const Palette& p, const std::string& token)
{
    if (token == "gold")          { return p.text_gold; }     // Crafting
    if (token == "char_green")    { return p.accent_green; }  // Gathering
    if (token == "legend_purple") { return p.legend_purple; } // Legendaries
    if (token == "zone_blue")     { return p.accent_blue; }   // Dailies
    if (token == "char_orange")   { return p.char_orange; }   // Characters
    return p.text_gold; // unknown token -> visible default
}

} // namespace shared::theme
