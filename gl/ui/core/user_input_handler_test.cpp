#include "gl/ui/core/user_input_handler.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(UserInputTest, SimpleTest) {
  UserInputHandler handler{};

  KeyboardKey keyboard_key = KeyboardKey::kNone;
  PressState keyboard_state = PressState::kNone;

  auto OnKeyboardEvent = [&keyboard_key, &keyboard_state](KeyboardKey key,
                                                          PressState state) {
    keyboard_key = key;
    keyboard_state = state;
  };

  MouseKey mouse_key = MouseKey::kNone;
  PressState mouse_state = PressState::kNone;
  float dist_travelled_x = 0.0f;
  float dist_travelled_y = 0.0f;

  auto OnMouseEvent =
      [&mouse_key, &mouse_state, &dist_travelled_x, &dist_travelled_y](
          MouseKey key,
          PressState state,
          float increment_x,
          float increment_y) {
        mouse_key = key;
        mouse_state = state;
        dist_travelled_x = increment_x;
        dist_travelled_y = increment_y;
      };

  handler.RegisterKeyboardCallback(OnKeyboardEvent);
  handler.RegisterMouseCallback(OnMouseEvent);

  handler.DispatchKeyboardEvent(KeyboardKey::kArrowUp, PressState::kPressed);
  EXPECT_EQ(KeyboardKey::kArrowUp, keyboard_key);
  EXPECT_EQ(PressState::kPressed, keyboard_state);

  handler.DispatchMouseEvent(
      MouseKey::kLeft, PressState::kPressed, 10.0f, 42.0f);
  EXPECT_EQ(MouseKey::kLeft, mouse_key);
  EXPECT_EQ(PressState::kPressed, mouse_state);
  EXPECT_FLOAT_EQ(10.0f, dist_travelled_x);
  EXPECT_FLOAT_EQ(42.0f, dist_travelled_y);
}
