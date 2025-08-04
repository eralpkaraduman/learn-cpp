#include "pdnewlib.h"
#include <memory>
#include <string>
#include <string_view>

extern "C" {
#include "pd_api.h"
}

constexpr std::string_view HELLO_TEXT = "Hello";

/**
 * Game class - contains all game logic
 */
class Game {
public:
  explicit Game(PlaydateAPI *pd)
      : pd_(pd), fontpath_("/System/Fonts/Asheville-Sans-14-Bold.pft"),
        font_(load_font(pd, fontpath_)), dx_(2), dy_(1) {

    // Calculate text dimensions and center position
    if (!font_) {
      pd_->system->error("Failed to load font from: %s", fontpath_.c_str());
    }

    pd_->graphics->setFont(font_);
    text_width_ = pd_->graphics->getTextWidth(
        font_, HELLO_TEXT.data(), HELLO_TEXT.size(), kASCIIEncoding, 0);
    text_height_ = pd_->graphics->getFontHeight(font_);

    x_ = (LCD_COLUMNS - text_width_) / 2;
    y_ = (LCD_ROWS - text_height_) / 2;

    // Set refresh rate
    pd_->display->setRefreshRate(50);
  }

  void update() {
    // Clear screen
    pd_->graphics->clear(kColorWhite);

    // Draw bounding box as filled black rectangle
    pd_->graphics->fillRect(x_, y_, text_width_, text_height_, kColorBlack);

    // Draw text in white over the black box
    pd_->graphics->setFont(font_);
    pd_->graphics->setDrawMode(kDrawModeInverted);
    pd_->graphics->drawText(HELLO_TEXT.data(), HELLO_TEXT.size(),
                            kASCIIEncoding, x_, y_);
    pd_->graphics->setDrawMode(kDrawModeCopy);

    // Update position
    x_ += dx_;
    y_ += dy_;

    // Bounce off edges using structured bindings
    auto [left_bound, right_bound] = std::pair{0, LCD_COLUMNS - text_width_};
    auto [top_bound, bottom_bound] = std::pair{0, LCD_ROWS - text_height_};

    if (x_ < left_bound || x_ > right_bound) {
      dx_ = -dx_;
    }

    if (y_ < top_bound || y_ > bottom_bound) {
      dy_ = -dy_;
    }

    // Draw FPS
    pd_->system->drawFPS(0, 0);
  }

private:
  static LCDFont* load_font(PlaydateAPI* pd, std::string_view path) {
    const char* err = nullptr;
    return pd->graphics->loadFont(path.data(), &err);
  }

  PlaydateAPI *pd_;
  std::string fontpath_;
  LCDFont* font_;
  int x_, y_, dx_, dy_;
  int text_width_, text_height_;
};

/**
 * Global game instance using smart pointer for automatic cleanup
 */
std::unique_ptr<Game> game;

/**
 * Update callback function - must be C-style for Playdate API
 */
static auto gameTick(void *userdata) -> int {
  [[maybe_unused]] auto *pd = static_cast<PlaydateAPI *>(userdata);
  if (game) {
    game->update();
  }
  return 1;
}

/**
 * Main event handler - entry point for all Playdate applications
 * Must have C linkage for the Playdate runtime
 */
extern "C" {
auto eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg) -> int {
  // Essential: Call this first for C++ runtime initialization
  eventHandler_pdnewlib(pd, event, arg);

  switch (event) {
  case kEventInit:
    // Create our game instance
    game = std::make_unique<Game>(pd);
    // Set the update callback to turn off Lua mode
    pd->system->setUpdateCallback(gameTick, pd);
    break;

  case kEventTerminate:
    pd->system->logToConsole("Shutting down...");
    game.reset(); // Clean up using reset() instead of nullptr
    break;

  default:
    break;
  }

  return 0;
}
}