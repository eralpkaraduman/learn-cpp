#include "pdnewlib.h"
#include <memory>
#include <string>
#include <string_view>

extern "C" {
#include "pd_api.h"
}

constexpr std::string_view HELLO_TEXT = "Hello";
constexpr std::string_view IMAGE_PATH = "bisey";

/**
 * Game class - contains all game logic
 */
class Game {
public:
  explicit Game(PlaydateAPI *pd)
      : pd_(pd), fontpath_("/System/Fonts/Asheville-Sans-14-Bold.pft"),
        font_(load_font(pd, fontpath_)), image_(load_image(pd, IMAGE_PATH)), dx_(2), dy_(1) {

    // Load font
    if (!font_) {
      pd_->system->error("Failed to load font from: %s", fontpath_.c_str());
    }

    // Load image
    if (!image_) {
      pd_->system->error("Failed to load image from: %s", IMAGE_PATH.data());
    }

    // Center the image (static position)
    int img_width, img_height;
    pd_->graphics->getBitmapData(image_, &img_width, &img_height, nullptr, nullptr, nullptr);
    image_x_ = (LCD_COLUMNS - img_width) / 2;
    image_y_ = (LCD_ROWS - img_height) / 2;

    // Get text dimensions for bouncing
    pd_->graphics->setFont(font_);
    text_width_ = pd_->graphics->getTextWidth(
        font_, HELLO_TEXT.data(), HELLO_TEXT.size(), kASCIIEncoding, 0);
    text_height_ = pd_->graphics->getFontHeight(font_);
    
    // Start text at top-left for bouncing
    text_x_ = 10;
    text_y_ = 10;

    // Set refresh rate
    pd_->display->setRefreshRate(50);
  }

  void update() {
    // Clear screen
    pd_->graphics->clear(kColorWhite);

    // Draw the cat image (static at center)
    pd_->graphics->drawBitmap(image_, image_x_, image_y_, kBitmapUnflipped);

    // Draw bouncing text with black bounding box
    pd_->graphics->fillRect(text_x_, text_y_, text_width_, text_height_, kColorBlack);
    
    // Draw text in white over the black box
    pd_->graphics->setFont(font_);
    pd_->graphics->setDrawMode(kDrawModeInverted);
    pd_->graphics->drawText(HELLO_TEXT.data(), HELLO_TEXT.size(),
                            kASCIIEncoding, text_x_, text_y_);
    pd_->graphics->setDrawMode(kDrawModeCopy);

    // Update text position
    text_x_ += dx_;
    text_y_ += dy_;

    // Bounce text off edges using structured bindings
    auto [left_bound, right_bound] = std::pair{0, LCD_COLUMNS - text_width_};
    auto [top_bound, bottom_bound] = std::pair{0, LCD_ROWS - text_height_};

    if (text_x_ < left_bound || text_x_ > right_bound) {
      dx_ = -dx_;
    }

    if (text_y_ < top_bound || text_y_ > bottom_bound) {
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
  
  static LCDBitmap* load_image(PlaydateAPI* pd, std::string_view path) {
    const char* err = nullptr;
    return pd->graphics->loadBitmap(path.data(), &err);
  }

  PlaydateAPI *pd_;
  std::string fontpath_;
  LCDFont* font_;
  LCDBitmap* image_;
  int text_x_, text_y_, dx_, dy_;           // Text position and movement
  int image_x_, image_y_;                   // Image position (static)
  int text_width_, text_height_;           // Text dimensions
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