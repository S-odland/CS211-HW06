#include "view.hxx"
#include <cmath>

// Convenient type aliases:
using Dimensions = ge211::Dimensions;
using Position   = ge211::Position;
using Color      = ge211::Color;
using Sprite_set = ge211::Sprite_set;
using Builder    = ge211::Text_sprite::Builder;

// You can change this or even determine it some other way:
static int const
    grid_size = 48,
    marker_radius = 20,
    border_width = 2,
    indicator_radius = grid_size / 6,
    border_radius = indicator_radius + border_width;


static Dimensions const
    grid_dims       {grid_size - border_width, grid_size - border_width},
    marker_shift    {grid_size / 2 - marker_radius,
                     grid_size / 2 - marker_radius},
    indicator_shift {grid_size / 2 - indicator_radius,
                     grid_size / 2 - indicator_radius},
    border_shift    {grid_size / 2 - border_radius,
                     grid_size / 2 - border_radius};

enum Layer : int
{
    grid,
    flip,
    border,
    indicator,
    marker,
    text,
};

static ge211::Color const
    dark_sprite_color  = Color::black(),
    light_sprite_color = Color::white(),
    loser_color        = ge211::Color {128,128,128},
    grid_color         = Color::medium_green().darken(0.25),
    flip_color         = Color{255, 0, 0};

View::View(Model const& model)
        : model_(model)
        , grid_sprite_(grid_dims, grid_color)
        , dark_sprite_(marker_radius, dark_sprite_color)
        , light_sprite_(marker_radius, light_sprite_color)
        , flip_sprite_(grid_dims, flip_color)
        , indicator_sprite_(indicator_radius, flip_color)
        , border_sprite_(border_radius, Color::black())
        , text_sprite_()
        , win_font_{"sans.ttf", grid_size / 2}
{ }

void View::draw(Sprite_set& set, Position mouse_pos)
{
    if (model_.is_game_over()) {
        if (model_.winner() == Player::dark) {
            light_sprite_.recolor(loser_color);
            text_sprite_.reconfigure(Builder(win_font_).color(dark_sprite_color)
                            << "Winner: Dark");
        }
        else if (model_.winner() == Player::light) {
            dark_sprite_.recolor(loser_color);
            text_sprite_.reconfigure(Builder(win_font_).color(light_sprite_color)
                            << "Winner: Light");
        } else {
            light_sprite_.recolor(loser_color);
            dark_sprite_.recolor(loser_color);
            text_sprite_.reconfigure(Builder(win_font_) << "Draw");
        }

        Position pos{model_.board().dimensions().width * grid_size / 2 -
                     text_sprite_.dimensions().width / 2,
                     model_.board().dimensions().height * grid_size};

        set.add_sprite(text_sprite_, pos);
    }

    // Draw the board, player markers, and possible moves
    for ( ge211::Position pos : model_.board() ) {
        set.add_sprite(grid_sprite_, board_to_screen(pos), grid);
        add_player_(set, pos);

        if (model_.find_move(pos))
            indicate_move_(set, pos);
    }

    if (model_.find_move(mouse_pos)) {
        set.add_sprite(flip_sprite_, board_to_screen(mouse_pos), flip);
        for (Position p : model_.find_move(mouse_pos)->second) {
            set.add_sprite(flip_sprite_, board_to_screen(p), flip);
        }
    }
}

Dimensions View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions() -
                       Dimensions{border_width,border_width};
}

std::string View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

void View::add_player_(ge211::Sprite_set &set, Position pos)
{
    Position screen = board_to_screen(pos);
    if (model_[pos] == Player::dark) {
        set.add_sprite(dark_sprite_, screen + marker_shift, marker);
    } else if (model_[pos] == Player::light) {
        set.add_sprite(light_sprite_, screen + marker_shift, marker);
    }
}

void View::indicate_move_(Sprite_set& set, Position pos)
{
    pos = board_to_screen(pos);
    set.add_sprite(indicator_sprite_, pos + indicator_shift, indicator);
    set.add_sprite(border_sprite_, pos + border_shift, border);
}

Position View::screen_to_board(Position pos) const
{
    pos.x /= grid_size;
    pos.y /= grid_size;
    return pos;
}

Position View::board_to_screen(Position pos) const
{
    pos.x *= grid_size;
    pos.y *= grid_size;
    return pos;
}


