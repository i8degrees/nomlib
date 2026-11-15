/******************************************************************************

  nomlib - C++11 cross-platform game engine

  paint.cpp

  A minimal pixel paint program demonstrating nomlib's RenderWindow, Image,
  Texture, event handling and font rendering.

  Tools: pixel pencil, eraser, outline and filled rectangles, outline and
  filled circles, flood fill (with optional two-point gradient), and a text
  stamp tool.

  Palette: up to 255 swatches shown as a horizontal bar at the bottom of the
  window.

  Shortcuts:
    1-6          Select tool
    Left click   Apply tool / start drag
    Release      Finish shape drag
    S            Save canvas.png
    L            Load canvas.png
    Z            Undo
    Y            Redo
    Esc          Quit
    Backspace    Delete last typed character while the text tool is active
    Return       Commit text stamp

  Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include <SDL.h>

#include "nomlib/core.hpp"
#include "nomlib/math.hpp"
#include "nomlib/system.hpp"
#include "nomlib/graphics.hpp"

using namespace nom;

namespace {

const std::string APP_NAME = "nom_paint";
const std::string SAVE_FILENAME = "canvas.png";
const nom::int32 WINDOW_WIDTH = 1024;
const nom::int32 WINDOW_HEIGHT = 768;
const nom::int32 TOOLBAR_WIDTH = 112;
const nom::int32 PALETTE_SWATCH_SIZE = 16;
const nom::int32 PALETTE_COLS = 16;
const nom::int32 PALETTE_ROWS = 16;
const nom::int32 PALETTE_AREA_WIDTH = PALETTE_COLS * PALETTE_SWATCH_SIZE;
const nom::int32 PALETTE_AREA_HEIGHT = PALETTE_ROWS * PALETTE_SWATCH_SIZE;
const nom::int32 MIN_TOOL_SIZE = 1;
const nom::int32 MAX_TOOL_SIZE = 40;
const double NOM_PI = 3.14159265358979323846;
const nom::int32 CANVAS_WIDTH = WINDOW_WIDTH - TOOLBAR_WIDTH;
const nom::int32 CANVAS_HEIGHT = WINDOW_HEIGHT - PALETTE_AREA_HEIGHT;

enum class Tool {
  Pencil = 0,
  Eraser,
  Rectangle,
  FilledRectangle,
  Circle,
  FilledCircle,
  FillBucket,
  Text
};

struct PlacedText {
  std::string text;
  Point2i pos;
  Color4i color;
  nom::int32 size = 20;
};

// ---------------------------------------------------------------------------
// Canvas history node
// ---------------------------------------------------------------------------
struct HistoryNode {
  HistoryNode* prev = nullptr;
  std::unique_ptr<HistoryNode> next;
  Image image;
  std::vector<PlacedText> texts;
};

// ---------------------------------------------------------------------------
// Pixel helpers operating on an unlocked nom::Image
// ---------------------------------------------------------------------------
bool inside_canvas(const Image& img, nom::int32 x, nom::int32 y)
{
  return x >= 0 && y >= 0 && x < img.width() && y < img.height();
}

Point2i screen_to_canvas(const Point2i& p)
{
  return Point2i(p.x - TOOLBAR_WIDTH, p.y);
}

Point2i canvas_to_screen(const Point2i& p)
{
  return Point2i(p.x + TOOLBAR_WIDTH, p.y);
}

bool same_color(const Color4i& a, const Color4i& b)
{
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

Color4i get_pixel(const Image& img, nom::int32 x, nom::int32 y)
{
  if( inside_canvas(img, x, y) ) {
    return img.color4i_pixel(x, y);
  }
  return Color4i::Black;
}

void put_pixel(Image& img, nom::int32 x, nom::int32 y, const Color4i& color)
{
  if( inside_canvas(img, x, y) ) {
    img.set_pixel(x, y, color);
  }
}

void draw_circle(Image& img, nom::int32 cx, nom::int32 cy, nom::int32 r,
                 const Color4i& color, bool filled);

void draw_brush(Image& img, nom::int32 cx, nom::int32 cy, nom::int32 size,
                const Color4i& color)
{
  nom::int32 radius = size / 2;
  if( radius < 1 ) {
    put_pixel(img, cx, cy, color);
  } else {
    draw_circle(img, cx, cy, radius, color, true);
  }
}

void draw_brush_line(Image& img, nom::int32 x0, nom::int32 y0,
                     nom::int32 x1, nom::int32 y1,
                     nom::int32 size, const Color4i& color)
{
  float dx = static_cast<float>(x1 - x0);
  float dy = static_cast<float>(y1 - y0);
  float dist = std::sqrt(dx * dx + dy * dy);
  float radius = size / 2.0f;
  float step = std::max(1.0f, dist / std::max(1.0f, radius * 0.5f));
  int steps = static_cast<int>(step);

  for( int i = 0; i <= steps; ++i ) {
    float t = (steps == 0) ? 0.0f : static_cast<float>(i) / steps;
    int x = static_cast<int>(x0 + dx * t);
    int y = static_cast<int>(y0 + dy * t);
    draw_brush(img, x, y, size, color);
  }
}

void draw_line(Image& img, nom::int32 x0, nom::int32 y0,
               nom::int32 x1, nom::int32 y1, const Color4i& color)
{
  nom::int32 dx = std::abs(x1 - x0);
  nom::int32 dy = std::abs(y1 - y0);
  nom::int32 sx = (x0 < x1) ? 1 : -1;
  nom::int32 sy = (y0 < y1) ? 1 : -1;
  nom::int32 err = dx - dy;

  while( true ) {
    put_pixel(img, x0, y0, color);
    if( x0 == x1 && y0 == y1 ) break;
    nom::int32 e2 = 2 * err;
    if( e2 > -dy ) {
      err -= dy;
      x0 += sx;
    }
    if( e2 < dx ) {
      err += dx;
      y0 += sy;
    }
  }
}

void draw_rect_outline(Image& img, nom::int32 x0, nom::int32 y0,
                       nom::int32 x1, nom::int32 y1, const Color4i& color)
{
  nom::int32 min_x = std::min(x0, x1);
  nom::int32 max_x = std::max(x0, x1);
  nom::int32 min_y = std::min(y0, y1);
  nom::int32 max_y = std::max(y0, y1);

  for( nom::int32 x = min_x; x <= max_x; ++x ) {
    put_pixel(img, x, min_y, color);
    put_pixel(img, x, max_y, color);
  }
  for( nom::int32 y = min_y + 1; y <= max_y - 1; ++y ) {
    put_pixel(img, min_x, y, color);
    put_pixel(img, max_x, y, color);
  }
}

void draw_rect_filled(Image& img, nom::int32 x0, nom::int32 y0,
                      nom::int32 x1, nom::int32 y1, const Color4i& color)
{
  nom::int32 min_x = std::min(x0, x1);
  nom::int32 max_x = std::max(x0, x1);
  nom::int32 min_y = std::min(y0, y1);
  nom::int32 max_y = std::max(y0, y1);

  for( nom::int32 y = min_y; y <= max_y; ++y ) {
    for( nom::int32 x = min_x; x <= max_x; ++x ) {
      put_pixel(img, x, y, color);
    }
  }
}

void draw_rect_outline_thick(Image& img, nom::int32 x0, nom::int32 y0,
                             nom::int32 x1, nom::int32 y1,
                             nom::int32 thickness, const Color4i& color)
{
  nom::int32 min_x = std::min(x0, x1);
  nom::int32 max_x = std::max(x0, x1);
  nom::int32 min_y = std::min(y0, y1);
  nom::int32 max_y = std::max(y0, y1);

  // Clamp thickness to the smaller dimension so we don't invert the rect.
  nom::int32 t = std::min(thickness,
                          std::min((max_x - min_x + 1) / 2,
                                   (max_y - min_y + 1) / 2));
  if( t < 1 ) t = 1;

  // Top and bottom bars.
  draw_rect_filled(img, min_x, min_y, max_x, min_y + t - 1, color);
  draw_rect_filled(img, min_x, max_y - t + 1, max_x, max_y, color);

  // Left and right bars.
  draw_rect_filled(img, min_x, min_y + t, min_x + t - 1, max_y - t, color);
  draw_rect_filled(img, max_x - t + 1, min_y + t, max_x, max_y - t, color);
}

void draw_circle_outline_thick(Image& img, nom::int32 cx, nom::int32 cy,
                               nom::int32 r, nom::int32 thickness,
                               const Color4i& color)
{
  // Draw concentric filled circles to build a thick ring.
  nom::int32 outer = r;
  nom::int32 inner = std::max(0, r - thickness + 1);
  for( nom::int32 radius = outer; radius >= inner; --radius ) {
    draw_circle(img, cx, cy, radius, color, true);
  }
}

void draw_circle(Image& img, nom::int32 cx, nom::int32 cy, nom::int32 r,
                 const Color4i& color, bool filled)
{
  if( r <= 0 ) return;

  if( filled ) {
    // Distance-based fill avoids the scanline gaps of the midpoint method.
    nom::int32 r2 = r * r;
    for( nom::int32 y = cy - r; y <= cy + r; ++y ) {
      nom::int32 dy = y - cy;
      nom::int32 dx_limit = static_cast<nom::int32>(
        std::sqrt(r2 - dy * dy));
      for( nom::int32 x = cx - dx_limit; x <= cx + dx_limit; ++x ) {
        put_pixel(img, x, y, color);
      }
    }
    return;
  }

  // Outline with the midpoint circle algorithm.
  nom::int32 x = 0;
  nom::int32 y = r;
  nom::int32 d = 3 - 2 * r;

  auto draw_octants = [&]() {
    put_pixel(img, cx + x, cy + y, color);
    put_pixel(img, cx - x, cy + y, color);
    put_pixel(img, cx + x, cy - y, color);
    put_pixel(img, cx - x, cy - y, color);
    put_pixel(img, cx + y, cy + x, color);
    put_pixel(img, cx - y, cy + x, color);
    put_pixel(img, cx + y, cy - x, color);
    put_pixel(img, cx - y, cy - x, color);
  };

  draw_octants();

  while( y >= x ) {
    ++x;
    if( d > 0 ) {
      --y;
      d = d + 4 * (x - y) + 10;
    } else {
      d = d + 4 * x + 6;
    }
    draw_octants();
  }
}

// ---------------------------------------------------------------------------
// Flood fill with optional two-point linear gradient
// ---------------------------------------------------------------------------
void flood_fill(Image& img, nom::int32 sx, nom::int32 sy,
                const Color4i& fill_color, const Color4i& grad_end_color,
                bool use_gradient)
{
  if( ! inside_canvas(img, sx, sy) ) return;

  Color4i target = img.color4i_pixel(sx, sy);
  if( same_color(target, fill_color) &&
      (!use_gradient || same_color(fill_color, grad_end_color)) )
  {
    return;
  }

  // Bounds for the filled region; used to compute gradient endpoints.
  nom::int32 min_x = std::numeric_limits<nom::int32>::max();
  nom::int32 max_x = std::numeric_limits<nom::int32>::min();
  nom::int32 min_y = std::numeric_limits<nom::int32>::max();
  nom::int32 max_y = std::numeric_limits<nom::int32>::min();

  std::vector<Point2i> stack;
  stack.reserve(1024);
  stack.emplace_back(sx, sy);

  // First pass: mark/paint the filled area with the start color and record
  // the bounding box of the region.
  while( !stack.empty() ) {
    Point2i p = stack.back();
    stack.pop_back();

    if( !inside_canvas(img, p.x, p.y) ) continue;
    if( !same_color(img.color4i_pixel(p.x, p.y), target) ) continue;

    img.set_pixel(p.x, p.y, fill_color);

    min_x = std::min(min_x, p.x);
    max_x = std::max(max_x, p.x);
    min_y = std::min(min_y, p.y);
    max_y = std::max(max_y, p.y);

    stack.emplace_back(p.x + 1, p.y);
    stack.emplace_back(p.x - 1, p.y);
    stack.emplace_back(p.x, p.y + 1);
    stack.emplace_back(p.x, p.y - 1);
  }

  // Second pass: apply gradient across the bounding box if requested.
  if( use_gradient ) {
    nom::int32 span_x = std::max(1, max_x - min_x);
    nom::int32 span_y = std::max(1, max_y - min_y);

    for( nom::int32 y = min_y; y <= max_y; ++y ) {
      for( nom::int32 x = min_x; x <= max_x; ++x ) {
        if( !inside_canvas(img, x, y) ) continue;
        if( !same_color(img.color4i_pixel(x, y), fill_color) ) continue;

        float t = static_cast<float>(y - min_y) / span_y;
        t = std::min(1.0f, std::max(0.0f, t));

        uint8 r = static_cast<uint8>(
          (1.0f - t) * fill_color.r + t * grad_end_color.r);
        uint8 g = static_cast<uint8>(
          (1.0f - t) * fill_color.g + t * grad_end_color.g);
        uint8 b = static_cast<uint8>(
          (1.0f - t) * fill_color.b + t * grad_end_color.b);
        uint8 a = static_cast<uint8>(
          (1.0f - t) * fill_color.a + t * grad_end_color.a);

        img.set_pixel(x, y, Color4i(r, g, b, a));
      }
    }
  }
}

// ---------------------------------------------------------------------------
// Palette generation
// ---------------------------------------------------------------------------
std::vector<Color4i> generate_html_palette()
{
  // The 16 basic HTML colors as RGB triples.
  const std::array<std::array<uint8, 3>, 16> html_colors = {{
    {{255, 255, 255}}, // White
    {{192, 192, 192}}, // Silver
    {{128, 128, 128}}, // Gray
    {{  0,   0,   0}}, // Black
    {{255,   0,   0}}, // Red
    {{128,   0,   0}}, // Maroon
    {{255, 255,   0}}, // Yellow
    {{128, 128,   0}}, // Olive
    {{  0, 255,   0}}, // Lime
    {{  0, 128,   0}}, // Green
    {{  0, 255, 255}}, // Aqua
    {{  0, 128, 128}}, // Teal
    {{  0,   0, 255}}, // Blue
    {{  0,   0, 128}}, // Navy
    {{255,   0, 255}}, // Fuchsia
    {{128,   0, 128}}  // Purple
  }};

  // Tile the 16-color set across the full 256 swatch grid.
  std::vector<Color4i> palette;
  palette.reserve(256);
  for( nom::size_type i = 0; i < 256; ++i ) {
    const auto& c = html_colors[i % html_colors.size()];
    palette.push_back(Color4i(c[0], c[1], c[2], 255));
  }
  return palette;
}

std::vector<Color4i> generate_palette(nom::size_type count)
{
  std::vector<Color4i> palette;
  palette.reserve(count);

  // First entry is always the "erase to" / background color.
  palette.push_back(Color4i(255, 255, 255, 255));

  // Generate the remaining colors using a golden-ratio hue walk.
  const float golden_ratio = 0.61803398875f;
  float hue = 0.0f;
  for( nom::size_type i = 1; i < count; ++i ) {
    uint8 h = static_cast<uint8>(hue * 255.0f);
    uint8 s = 180 + ((i * 37) % 75); // 180-254
    uint8 v = 200 + ((i * 19) % 55); // 200-254

    // Simple HSV to RGB conversion.
    uint8 max_val = v;
    uint8 min_val = static_cast<uint8>(max_val * (255 - s) / 255.0f);
    uint8 c = max_val - min_val;

    uint8 hh = h / 43;
    uint8 x = static_cast<uint8>((c * (255 - std::abs((h % 86) - 43))) / 255);

    uint8 r, g, b;
    switch( hh ) {
      case 0: r = c; g = x; b = 0; break;
      case 1: r = x; g = c; b = 0; break;
      case 2: r = 0; g = c; b = x; break;
      case 3: r = 0; g = x; b = c; break;
      case 4: r = x; g = 0; b = c; break;
      default: r = c; g = 0; b = x; break;
    }

    palette.push_back(Color4i(
      r + min_val,
      g + min_val,
      b + min_val,
      255));

    hue += golden_ratio;
    hue -= static_cast<int>(hue);
  }

  return palette;
}

void draw_circle_icon(RenderWindow& target, nom::int32 cx, nom::int32 cy,
                      nom::int32 r, const Color4i& color, bool filled)
{
  const int segs = 16;
  Point2i prev;
  bool first = true;
  for( int i = 0; i <= segs; ++i ) {
    float t = static_cast<float>(i) / segs * 2.0f * static_cast<float>(NOM_PI);
    int x = cx + static_cast<int>(std::cos(t) * r);
    int y = cy + static_cast<int>(std::sin(t) * r);
    if( !first ) {
      if( filled ) {
        // Approximate filled by drawing horizontal lines across center.
        // Simpler: draw spokes to center.
        Line spoke(IntRect(cx, cy, x - cx, y - cy), color);
        spoke.draw(target);
      } else {
        Line line(IntRect(prev.x, prev.y, x - prev.x, y - prev.y), color);
        line.draw(target);
      }
    }
    first = false;
    prev = Point2i(x, y);
  }
}

} // anonymous namespace

const char* tool_name( Tool t )
{
  switch( t ) {
    case Tool::Pencil: return "Pencil";
    case Tool::Eraser: return "Eraser";
    case Tool::Rectangle: return "Rectangle";
    case Tool::FilledRectangle: return "Filled Rectangle";
    case Tool::Circle: return "Circle";
    case Tool::FilledCircle: return "Filled Circle";
    case Tool::FillBucket: return "Flood Fill";
    case Tool::Text: return "Text";
  }
  return "";
}

// ---------------------------------------------------------------------------
// Application
// ---------------------------------------------------------------------------
class App: public SDLApp
{
  public:
    App( void );
    ~App( void );

    bool on_init( void ) override;
    void on_update( float delta ) override;
    void on_draw( RenderWindow& target ) override;

  private:
    void process_events( void );
    void handle_key_event( const Event& ev );
    void handle_mouse_event( const Event& ev );
    void commit_shape( void );
    void take_snapshot( void );
    void undo( void );
    void redo( void );
    void refresh_texture( void );
    void draw_toolbar( RenderWindow& target );
    void draw_palette( RenderWindow& target );
    void draw_overlay( RenderWindow& target );
    void draw_tooltip( RenderWindow& target );
    void update_texture_from_canvas( void );

  public:
    nom::RenderWindow& window() { return this->window_; }
    bool running() const { return this->running_; }

  private:
    bool running_ = true;
    RenderWindow window_;
    EventHandler evt_handler_;

    Image canvas_;
    Texture canvas_texture_;

    std::vector<Color4i> palette_;
    nom::size_type active_color_index_ = 1; // skip white background
    Color4i secondary_color_ = Color4i::Black;
    bool use_gradient_fill_ = false;

    Tool active_tool_ = Tool::Pencil;
    nom::int32 tool_size_ = 1; // 1..40; interpreted per tool

    bool dragging_ = false;
    nom::int32 hovered_tool_ = -1;
    Point2i drag_start_;
    Point2i drag_end_;
    Point2i mouse_pos_;
    Point2i last_pencil_;

    std::unique_ptr<HistoryNode> history_head_;
    HistoryNode* history_current_ = nullptr;
    std::size_t history_size_ = 0;
    static constexpr std::size_t MAX_HISTORY = 50;

    Font font_;
    Text text_overlay_;
    bool typing_text_ = false;
    std::string typed_text_;
    Point2i text_origin_;
    std::vector<PlacedText> placed_texts_;
};

App::App( void )
{
  NOM_LOG_TRACE(NOM);
}

App::~App( void )
{
  NOM_LOG_TRACE(NOM);
}

bool App::on_init( void )
{
  if( !this->window_.create(APP_NAME,
                            Size2i(WINDOW_WIDTH, WINDOW_HEIGHT),
                            SDL_WINDOW_RESIZABLE) )
  {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Failed to create window");
    return false;
  }

  if( !this->canvas_.initialize(Point2i(CANVAS_WIDTH, CANVAS_HEIGHT)) ) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Failed to create canvas");
    return false;
  }

  // Build the 256-entry palette from the 16 basic HTML colors, tiled.
  this->palette_ = generate_html_palette();

  // Force white as the first swatch and black as the last swatch.
  this->palette_[0] = Color4i(255, 255, 255, 255);
  this->palette_[255] = Color4i(0, 0, 0, 255);

  // Clear to the first palette color (white by convention).
  this->canvas_.lock();
  draw_rect_filled(this->canvas_, 0, 0,
                   CANVAS_WIDTH - 1, CANVAS_HEIGHT - 1,
                   this->palette_[0]);
  this->canvas_.unlock();

  if( !this->canvas_texture_.create(this->canvas_,
                                    SDL_PIXELFORMAT_ARGB8888,
                                    Texture::Streaming) )
  {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Failed to create canvas texture");
    return false;
  }

  this->canvas_texture_.set_position(Point2i(TOOLBAR_WIDTH, 0));
  this->canvas_texture_.set_size(Size2i(CANVAS_WIDTH, CANVAS_HEIGHT));

  std::string font_path = "Resources/OpenSans-Regular.ttf";
  if( !this->font_.load(font_path) ) {
    // Try the source tree path when running without post-build copy.
    font_path = "../Resources/tests/gui/OpenSans-Regular.ttf";
    if( !this->font_.load(font_path) ) {
      NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                  "Failed to load font; text tool unavailable");
      // Non-fatal: the rest of the program still works.
    }
  }

  this->text_overlay_.set_font(this->font_);
  this->text_overlay_.set_text_size(20);
  this->text_overlay_.set_color(Color4i::Black);

  this->take_snapshot();
  this->refresh_texture();

  return true;
}

void App::on_update( float /*delta*/ )
{
  this->process_events();
}

void App::on_draw( RenderWindow& target )
{
  target.fill(Color4i(64, 64, 64, 255));

  this->canvas_texture_.draw(target);

  // Preview shape while dragging.
  if( this->dragging_ ) {
    if( this->active_tool_ == Tool::Rectangle ||
        this->active_tool_ == Tool::FilledRectangle )
    {
      Rectangle preview(
        IntRect(this->drag_start_.x, this->drag_start_.y,
                this->drag_end_.x - this->drag_start_.x,
                this->drag_end_.y - this->drag_start_.y),
        this->palette_[this->active_color_index_]);
      preview.draw(target);
    }
    else if( this->active_tool_ == Tool::Circle ||
             this->active_tool_ == Tool::FilledCircle )
    {
      // A simple circle preview by drawing short line segments.
      float cx = (this->drag_start_.x + this->drag_end_.x) / 2.0f;
      float cy = (this->drag_start_.y + this->drag_end_.y) / 2.0f;
      float rx = std::abs(this->drag_end_.x - this->drag_start_.x) / 2.0f;
      float ry = std::abs(this->drag_end_.y - this->drag_start_.y) / 2.0f;
      const int segments = 32;
      Point2i prev;
      bool first = true;
      for( int i = 0; i <= segments; ++i ) {
        float t = static_cast<float>(i) / segments * 2.0f * static_cast<float>(NOM_PI);
        int x = static_cast<int>(cx + std::cos(t) * rx);
        int y = static_cast<int>(cy + std::sin(t) * ry);
        if( !first ) {
          Line line(IntRect(prev.x, prev.y, x - prev.x, y - prev.y),
                    this->palette_[this->active_color_index_]);
          line.draw(target);
        }
        first = false;
        prev = Point2i(x, y);
      }
      // Close the loop precisely.
      if( segments > 0 ) {
        int x = static_cast<int>(cx + rx);
        int y = cy;
        Line close_loop(IntRect(prev.x, prev.y, x - prev.x, y - prev.y),
                        this->palette_[this->active_color_index_]);
        close_loop.draw(target);
      }
    }
  }

  // Stamp placed text strings.
  for( const auto& pt : this->placed_texts_ ) {
    this->text_overlay_.set_text(pt.text);
    this->text_overlay_.set_text_size(pt.size);
    this->text_overlay_.set_position(canvas_to_screen(pt.pos));
    this->text_overlay_.set_color(pt.color);
    this->text_overlay_.draw(target);
  }

  // Preview text overlay following the cursor.
  if( this->typing_text_ && !this->typed_text_.empty() ) {
    nom::int32 text_size = std::max(8, std::min(40, this->tool_size_));
    this->text_overlay_.set_text(this->typed_text_);
    this->text_overlay_.set_text_size(text_size);
    this->text_overlay_.set_position(this->mouse_pos_);
    this->text_overlay_.set_color(this->palette_[this->active_color_index_]);
    this->text_overlay_.draw(target);
  }

  this->draw_toolbar(target);
  this->draw_palette(target);
  this->draw_overlay(target);
  this->draw_tooltip(target);

  target.update();
}

void App::process_events( void )
{
  Event ev;
  while( this->evt_handler_.poll_event(ev) ) {
    if( ev.type == Event::QUIT_EVENT ) {
      this->running_ = false;
    }
    else if( ev.type == Event::KEY_PRESS ) {
      this->handle_key_event(ev);
    }
    else if( ev.type == Event::MOUSE_MOTION ) {
      this->mouse_pos_.x = ev.motion.x;
      this->mouse_pos_.y = ev.motion.y;
      this->drag_end_ = this->mouse_pos_;

      // Update tooltip target when hovering the left toolbar.
      const nom::int32 toolbar_bottom = 8 * 40;
      if( ev.motion.x < TOOLBAR_WIDTH && ev.motion.y < toolbar_bottom ) {
        this->hovered_tool_ = ev.motion.y / 40;
      } else {
        this->hovered_tool_ = -1;
      }

      if( this->dragging_ &&
          (this->active_tool_ == Tool::Pencil ||
           this->active_tool_ == Tool::Eraser) )
      {
        Color4i color = (this->active_tool_ == Tool::Eraser)
                        ? this->palette_[0]
                        : this->palette_[this->active_color_index_];
        Point2i cur = screen_to_canvas(this->mouse_pos_);
        Point2i prev = screen_to_canvas(this->last_pencil_);
        this->canvas_.lock();
        draw_brush_line(this->canvas_, prev.x, prev.y, cur.x, cur.y,
                        this->tool_size_, color);
        this->canvas_.unlock();
        this->last_pencil_ = this->mouse_pos_;
      }
    }
    else if( ev.type == Event::MOUSE_BUTTON_CLICK ) {
      this->handle_mouse_event(ev);
    }
    else if( ev.type == Event::MOUSE_BUTTON_RELEASE ) {
      if( this->dragging_ ) {
        this->commit_shape();
        this->dragging_ = false;
      }
    }
    else if( ev.type == Event::TEXT_INPUT ) {
      if( this->typing_text_ ) {
        this->typed_text_.append(ev.text.text);
      }
    }
  }
}

void App::handle_key_event( const Event& ev )
{
  SDL_Keycode key = static_cast<SDL_Keycode>(ev.key.sym);
  uint16 mod = ev.key.mod;
  bool ctrl = (mod & KMOD_CTRL) != 0;

  // Let the user's keyboard locale generate these; included only for clarity.
  if( key == SDLK_LEFTBRACKET || key == SDLK_RIGHTBRACKET ) {
    // handled below
  }

  // Tool selection
  if( key == SDLK_1 ) this->active_tool_ = Tool::Pencil;
  else if( key == SDLK_2 ) this->active_tool_ = Tool::Eraser;
  else if( key == SDLK_3 ) this->active_tool_ = Tool::Rectangle;
  else if( key == SDLK_4 ) this->active_tool_ = Tool::FilledRectangle;
  else if( key == SDLK_5 ) this->active_tool_ = Tool::Circle;
  else if( key == SDLK_6 ) this->active_tool_ = Tool::FilledCircle;
  else if( key == SDLK_7 ) this->active_tool_ = Tool::FillBucket;
  else if( key == SDLK_8 ) this->active_tool_ = Tool::Text;

  // Tool size (1..40)
  else if( key == SDLK_LEFTBRACKET || key == SDLK_MINUS ) {
    this->tool_size_ = std::max(MIN_TOOL_SIZE, this->tool_size_ - 1);
  }
  else if( key == SDLK_RIGHTBRACKET || key == SDLK_EQUALS ) {
    this->tool_size_ = std::min(MAX_TOOL_SIZE, this->tool_size_ + 1);
  }

  // Undo / redo
  else if( ctrl && key == SDLK_z ) this->undo();
  else if( ctrl && key == SDLK_y ) this->redo();
  else if( key == SDLK_z ) this->undo();
  else if( key == SDLK_y ) this->redo();

  // Save / load
  else if( key == SDLK_s ) {
    this->canvas_.save_png(SAVE_FILENAME);
    NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Saved", SAVE_FILENAME);
  }
  else if( key == SDLK_l ) {
    Image tmp;
    if( tmp.load(SAVE_FILENAME, SDL_PIXELFORMAT_ARGB8888) ) {
      this->canvas_ = tmp;
      this->placed_texts_.clear();
      this->take_snapshot();
      this->refresh_texture();
      NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Loaded", SAVE_FILENAME);
    } else {
      NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Failed to load", SAVE_FILENAME);
    }
  }

  // Clear canvas
  else if( key == SDLK_c ) {
    this->canvas_.lock();
    draw_rect_filled(this->canvas_, 0, 0,
                     CANVAS_WIDTH - 1, CANVAS_HEIGHT - 1,
                     this->palette_[0]);
    this->canvas_.unlock();
    this->placed_texts_.clear();
    this->take_snapshot();
    this->refresh_texture();
  }

  // Gradient toggle
  else if( key == SDLK_g ) {
    this->use_gradient_fill_ = !this->use_gradient_fill_;
  }

  // Text tool
  else if( this->typing_text_ ) {
    if( key == SDLK_RETURN || key == SDLK_KP_ENTER ) {
      if( !this->typed_text_.empty() ) {
        PlacedText pt;
        pt.text = this->typed_text_;
        pt.pos = this->text_origin_;
        pt.color = this->palette_[this->active_color_index_];
        pt.size = std::max(8, std::min(40, this->tool_size_));
        this->placed_texts_.push_back(pt);
        this->take_snapshot();
      }
      this->typing_text_ = false;
      this->typed_text_.clear();
      SDL_StopTextInput();
    }
    else if( key == SDLK_BACKSPACE && !this->typed_text_.empty() ) {
      this->typed_text_.pop_back();
    }
    else if( key == SDLK_ESCAPE ) {
      this->typing_text_ = false;
      this->typed_text_.clear();
      SDL_StopTextInput();
    }
  }
  else if( key == SDLK_ESCAPE ) {
    this->running_ = false;
  }
}

void App::handle_mouse_event( const Event& ev )
{
  Point2i screen_pos(ev.mouse.x, ev.mouse.y);
  bool on_toolbar = (ev.mouse.x < TOOLBAR_WIDTH && ev.mouse.y < CANVAS_HEIGHT);

  // Tool selection on the left toolbar.
  if( on_toolbar ) {
    const nom::int32 button_height = 40;
    nom::size_type idx = ev.mouse.y / button_height;
    if( idx < 8 ) { // 8 tools
      this->active_tool_ = static_cast<Tool>(idx);
    }
    this->dragging_ = false;
    return;
  }

  // Color palette selection in the bottom-left grid.
  if( ev.mouse.y >= CANVAS_HEIGHT && ev.mouse.x < PALETTE_AREA_WIDTH ) {
    nom::int32 col = ev.mouse.x / PALETTE_SWATCH_SIZE;
    nom::int32 row = (ev.mouse.y - CANVAS_HEIGHT) / PALETTE_SWATCH_SIZE;
    if( col < PALETTE_COLS && row < PALETTE_ROWS ) {
      nom::size_type idx = row * PALETTE_COLS + col;
      if( idx < this->palette_.size() ) {
        if( ev.mouse.button == nom::RIGHT_MOUSE_BUTTON ) {
          this->secondary_color_ = this->palette_[idx];
        } else {
          this->active_color_index_ = idx;
        }
      }
    }
    return;
  }

  if( ev.mouse.button != nom::LEFT_MOUSE_BUTTON ) return;

  // Canvas interaction.
  this->dragging_ = true;
  this->drag_start_ = screen_pos;
  this->drag_end_ = screen_pos;
  this->last_pencil_ = screen_pos;

  Point2i canvas_pos = screen_to_canvas(screen_pos);

  if( this->active_tool_ == Tool::Pencil ||
      this->active_tool_ == Tool::Eraser )
  {
    Color4i color = (this->active_tool_ == Tool::Eraser)
                    ? this->palette_[0]
                    : this->palette_[this->active_color_index_];
    this->canvas_.lock();
    draw_brush(this->canvas_, canvas_pos.x, canvas_pos.y,
               this->tool_size_, color);
    this->canvas_.unlock();
  }
  else if( this->active_tool_ == Tool::Text ) {
    this->typing_text_ = true;
    this->typed_text_.clear();
    this->text_origin_ = canvas_pos;
    this->mouse_pos_ = screen_pos;
    SDL_StartTextInput();
    this->dragging_ = false;
  }
}

void App::commit_shape( void )
{
  Point2i s = screen_to_canvas(this->drag_start_);
  Point2i e = screen_to_canvas(this->drag_end_);

  this->canvas_.lock();

  if( this->active_tool_ == Tool::Rectangle ) {
    draw_rect_outline(this->canvas_, s.x, s.y, e.x, e.y,
                      this->palette_[this->active_color_index_]);
  }
  else if( this->active_tool_ == Tool::FilledRectangle ) {
    draw_rect_filled(this->canvas_, s.x, s.y, e.x, e.y,
                     this->palette_[this->active_color_index_]);
  }
  else if( this->active_tool_ == Tool::Circle ||
           this->active_tool_ == Tool::FilledCircle )
  {
    float rx = std::abs(e.x - s.x) / 2.0f;
    float ry = std::abs(e.y - s.y) / 2.0f;
    float r = std::max(rx, ry);
    float cx = (s.x + e.x) / 2.0f;
    float cy = (s.y + e.y) / 2.0f;

    draw_circle(this->canvas_,
                static_cast<nom::int32>(cx),
                static_cast<nom::int32>(cy),
                static_cast<nom::int32>(r),
                this->palette_[this->active_color_index_],
                this->active_tool_ == Tool::FilledCircle);
  }
  else if( this->active_tool_ == Tool::FillBucket ) {
    flood_fill(this->canvas_, e.x, e.y,
               this->palette_[this->active_color_index_],
               this->secondary_color_,
               this->use_gradient_fill_);
  }

  this->canvas_.unlock();
  this->take_snapshot();
  this->refresh_texture();
}

void App::take_snapshot( void )
{
  // Truncate redo branch the moment a new edit is made.
  if( this->history_current_ && this->history_current_->next ) {
    this->history_current_->next.reset();
  }

  std::unique_ptr<HistoryNode> node(new HistoryNode);
  node->image = this->canvas_;
  node->texts = this->placed_texts_;

  if( this->history_current_ ) {
    node->prev = this->history_current_;
    this->history_current_->next = std::move(node);
    this->history_current_ = this->history_current_->next.get();
  } else {
    this->history_head_ = std::move(node);
    this->history_current_ = this->history_head_.get();
  }

  ++this->history_size_;

  // Enforce maximum depth by dropping oldest nodes.
  while( this->history_size_ > MAX_HISTORY && this->history_head_->next ) {
    std::unique_ptr<HistoryNode> new_head = std::move(this->history_head_->next);
    new_head->prev = nullptr;
    this->history_head_ = std::move(new_head);
    --this->history_size_;
  }
}

void App::undo( void )
{
  if( this->history_current_ && this->history_current_->prev ) {
    this->history_current_ = this->history_current_->prev;
    this->canvas_ = this->history_current_->image;
    this->placed_texts_ = this->history_current_->texts;
    this->refresh_texture();
  }
}

void App::redo( void )
{
  if( this->history_current_ && this->history_current_->next ) {
    this->history_current_ = this->history_current_->next.get();
    this->canvas_ = this->history_current_->image;
    this->placed_texts_ = this->history_current_->texts;
    this->refresh_texture();
  }
}

void App::refresh_texture( void )
{
  this->canvas_texture_.create(this->canvas_,
                               SDL_PIXELFORMAT_ARGB8888,
                               Texture::Streaming);
  this->canvas_texture_.set_position(Point2i(0, 0));
  this->canvas_texture_.set_size(Size2i(CANVAS_WIDTH, CANVAS_HEIGHT));
  this->update_texture_from_canvas();
}

void App::update_texture_from_canvas( void )
{
  this->canvas_texture_.update_pixels(
    this->canvas_.pixels(),
    static_cast<uint16>(this->canvas_.pitch()),
    IntRect::null);
}

void App::draw_toolbar( RenderWindow& target )
{
  const nom::int32 button_h = 40;
  const Color4i bg(48, 48, 48, 255);
  const Color4i active_bg(72, 72, 72, 255);
  const Color4i icon_color(220, 220, 220, 255);

  for( nom::int32 i = 0; i < 8; ++i ) {
    Tool t = static_cast<Tool>(i);
    IntRect btn(0, i * button_h, TOOLBAR_WIDTH, button_h);
    Rectangle bg_rect(btn, (this->active_tool_ == t) ? active_bg : bg);
    bg_rect.draw(target);

    // Highlight border for the active tool.
    if( this->active_tool_ == t ) {
      Rectangle active_frame(IntRect(2, i * button_h + 2,
                                     TOOLBAR_WIDTH - 4, button_h - 4),
                             Color4i::Yellow);
      active_frame.draw(target);
    }

    nom::int32 cx = TOOLBAR_WIDTH / 2;
    nom::int32 cy = i * button_h + button_h / 2;

    switch( t ) {
      case Tool::Pencil:
      {
        Line pencil(IntRect(cx - 8, cy + 10, 16, -16), icon_color);
        pencil.draw(target);
        Rectangle tip(IntRect(cx + 6, cy - 8, 4, 4), icon_color);
        tip.draw(target);
        break;
      }
      case Tool::Eraser:
      {
        Rectangle rubber(IntRect(cx - 8, cy - 6, 18, 14), Color4i::White);
        rubber.draw(target);
        break;
      }
      case Tool::Rectangle:
      {
        Rectangle outline(IntRect(cx - 8, cy - 8, 18, 18), icon_color);
        outline.draw(target);
        break;
      }
      case Tool::FilledRectangle:
      {
        Rectangle filled(IntRect(cx - 8, cy - 8, 18, 18), icon_color);
        filled.draw(target);
        break;
      }
      case Tool::Circle:
      {
        draw_circle_icon(target, cx, cy, 10, icon_color, false);
        break;
      }
      case Tool::FilledCircle:
      {
        draw_circle_icon(target, cx, cy, 10, icon_color, true);
        break;
      }
      case Tool::FillBucket:
      {
        // A small bucket-ish icon using a rectangle and a "pour" line.
        Line pour(IntRect(cx - 6, cy - 10, 4, 10), icon_color);
        pour.draw(target);
        Rectangle bucket(IntRect(cx - 8, cy + 0, 18, 12), icon_color);
        bucket.draw(target);
        break;
      }
      case Tool::Text:
      {
        Text label("A", this->font_, 22, icon_color);
        label.set_position(Point2i(cx - 8, cy - 14));
        label.draw(target);
        break;
      }
    }
  }
}

void App::draw_palette( RenderWindow& target )
{
  // Dark background behind the grid.
  Rectangle bg(IntRect(0, CANVAS_HEIGHT,
                       PALETTE_AREA_WIDTH, PALETTE_AREA_HEIGHT),
               Color4i(32, 32, 32, 255));
  bg.draw(target);

  for( nom::size_type i = 0; i < this->palette_.size(); ++i ) {
    nom::int32 col = static_cast<nom::int32>(i % PALETTE_COLS);
    nom::int32 row = static_cast<nom::int32>(i / PALETTE_COLS);

    nom::int32 x = col * PALETTE_SWATCH_SIZE;
    nom::int32 y = CANVAS_HEIGHT + row * PALETTE_SWATCH_SIZE;

    Rectangle swatch(IntRect(x, y, PALETTE_SWATCH_SIZE, PALETTE_SWATCH_SIZE),
                     this->palette_[i]);
    swatch.draw(target);

    // Active color indicator.
    if( static_cast<nom::size_type>(i) == this->active_color_index_ ) {
      Rectangle frame(IntRect(x + 2, y + 2,
                              PALETTE_SWATCH_SIZE - 4,
                              PALETTE_SWATCH_SIZE - 4),
                      Color4i::Yellow);
      frame.draw(target);
    }
  }
}

void App::draw_overlay( RenderWindow& target )
{
  Text status(tool_name(this->active_tool_), this->font_, 18, Color4i::White);
  status.set_position(Point2i(TOOLBAR_WIDTH + 8, CANVAS_HEIGHT - 20));
  status.draw(target);

  std::string size_label = "SIZE: " + std::to_string(this->tool_size_);
  Text size_text(size_label, this->font_, 18, Color4i(255, 255, 160, 255));
  size_text.set_position(Point2i(TOOLBAR_WIDTH + 120, CANVAS_HEIGHT - 20));
  size_text.draw(target);

  // Draw a dark backing strip for the help text just above the palette.
  Rectangle help_bg(IntRect(TOOLBAR_WIDTH, CANVAS_HEIGHT - 24,
                            WINDOW_WIDTH - TOOLBAR_WIDTH, 24),
                    Color4i(24, 24, 24, 255));
  help_bg.draw(target);

  Text prompt("1-8:tools   S:save L:load   Z:undo Y:redo   C:clear G:gradient   [-]/[=]: size",
              this->font_, 16, Color4i::White);
  prompt.set_position(Point2i(TOOLBAR_WIDTH + 10, CANVAS_HEIGHT - 20));
  prompt.draw(target);
}

void App::draw_tooltip( RenderWindow& target )
{
  if( this->hovered_tool_ < 0 || this->hovered_tool_ > 7 ) return;

  Tool t = static_cast<Tool>(this->hovered_tool_);
  const char* label = tool_name(t);
  std::string text(label);

  nom::int32 tip_w = static_cast<nom::int32>(text.length() * 9 + 12);
  nom::int32 tip_h = 22;
  nom::int32 tx = this->mouse_pos_.x + 12;
  nom::int32 ty = this->mouse_pos_.y + 12;

  // Keep tooltip inside window bounds.
  if( tx + tip_w > WINDOW_WIDTH ) tx = WINDOW_WIDTH - tip_w - 4;
  if( ty + tip_h > WINDOW_HEIGHT ) ty = WINDOW_HEIGHT - tip_h - 4;

  Rectangle bg(IntRect(tx, ty, tip_w, tip_h), Color4i(0, 0, 0, 220));
  bg.draw(target);

  Text hint(text, this->font_, 14, Color4i::White);
  hint.set_position(Point2i(tx + 6, ty + 3));
  hint.draw(target);
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
nom::sint main( nom::int32 argc, char* argv[] )
{
  if( nom::init(argc, argv) == false ) {
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  App app;

  if( app.on_init() == false ) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Application failed to initialize");
    return NOM_EXIT_FAILURE;
  }

  Timer delta;
  delta.start();
  while( app.running() ) {
    app.on_update(delta.ticks() / 1000.0f);
    delta.restart();
    app.on_draw(app.window());
  }

  return NOM_EXIT_SUCCESS;
}
