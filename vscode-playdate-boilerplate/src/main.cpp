#include "pdnewlib.h"
#include <cstring>
#include <memory>
#include <string>

extern "C" {
#include "pd_api.h"
}

constexpr int TEXT_WIDTH = 16 * 2;
constexpr int TEXT_HEIGHT = 16;

/**
 * Game class - contains all game logic
 */
class Game {
public:
  explicit Game(PlaydateAPI *pd)
      : pd_(pd), fontpath_("/System/Fonts/Asheville-Sans-14-Bold.pft"),
        font_(nullptr), x_((LCD_COLUMNS - TEXT_WIDTH) / 2),
        y_((LCD_ROWS - TEXT_HEIGHT) / 2), dx_(2), dy_(1) {
    // Load font
    const char *err;
    font_ = pd_->graphics->loadFont(fontpath_.c_str(), &err);

    if (font_ == nullptr) {
      pd_->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__,
                         fontpath_.c_str(), err);
    }

    // Set refresh rate
    pd_->display->setRefreshRate(50);
  }

  void update() {
    // Clear screen
    pd_->graphics->clear(kColorWhite);
    pd_->graphics->setFont(font_);
    pd_->graphics->drawText("Hello", std::strlen("Hello"), kASCIIEncoding, x_,
                            y_);

    // Update position
    x_ += dx_;
    y_ += dy_;

    // Bounce off edges
    if (x_ < 0 || x_ > LCD_COLUMNS - TEXT_WIDTH) {
      dx_ = -dx_;
    }

    if (y_ < 0 || y_ > LCD_ROWS - TEXT_HEIGHT) {
      dy_ = -dy_;
    }

    // Draw FPS
    pd_->system->drawFPS(0, 0);
  }

private:
  PlaydateAPI *pd_;
  std::string fontpath_;
  LCDFont *font_;
  int x_, y_, dx_, dy_;
};

/**
 * Global game instance using smart pointer for automatic cleanup
 */
std::unique_ptr<Game> game;

/**
 * Update callback function - must be C-style for Playdate API
 */
static int gameTick(void *userdata) {
  (void)userdata; // unused parameter
  game->update();
  return 1;
}

/**
 * Main event handler - entry point for all Playdate applications
 * Must have C linkage for the Playdate runtime
 */
extern "C" {
int eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg) {
  // Essential: Call this first for C++ runtime initialization
  eventHandler_pdnewlib(pd, event, arg);

  if (event == kEventInit) {
    // Create our game instance
    game = std::make_unique<Game>(pd);

    // Set the update callback to turn off Lua mode
    pd->system->setUpdateCallback(gameTick, pd);
  }

  if (event == kEventTerminate) {
    pd->system->logToConsole("Shutting down...");
    game = nullptr; // Clean up
  }

  return 0;
}
}