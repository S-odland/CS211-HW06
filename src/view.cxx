#include "view.hxx"
#include <cmath>

// Convenient type aliases:
using Dimensions = ge211::Dimensions;
using Position   = ge211::Position;
using Color      = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;
static int const sprite_radius = grid_size/2 - 1;

static ge211::Color const
        dark_sprite_color = ge211::Color::black(),
        light_sprite_color = ge211::Color::white(),
        loser_color = ge211::Color {128,128,128},
        grid_color = ge211::Color {0, 255, 0};


View::View(Model const& model)
        : model_(model)
        , dark_sprite_(sprite_radius, dark_sprite_color)
        , light_sprite_(sprite_radius, light_sprite_color)
        , loser_sprite_(sprite_radius, loser_color)
        , grid_sprite_({grid_size - 2,grid_size - 2},grid_color)
{ }

void View::draw(Sprite_set& set)
{
    for ( ge211::Position pos : model_.board() )
    {
        set.add_sprite(grid_sprite_,board_to_screen(pos),0);
    }
    if (b == 1){
        if (model_.find_move(screen_to_board(mouse_pos)) != NULL) {
            Position_set pos_set = model_.find_move(screen_to_board(mouse_pos))
                                         ->second;
            add_player_(set, model_.turn(),mouse_click_pos, 1);
            for (Position p : pos_set) {
                add_player_(set, model_.turn(), p, 1);
            }
        }

    }

    if (model_.turn() == Player::dark){
        set.add_sprite(dark_sprite_, board_to_screen
        (screen_to_board(mouse_pos)), 1);
    } else if (model_.turn() == Player::light){
        set.add_sprite(light_sprite_, board_to_screen
        (screen_to_board(mouse_pos)), 1);
    }
    if (model_.find_move(screen_to_board(mouse_pos)) != NULL) {
        Position_set pos_set = model_.find_move(screen_to_board(mouse_pos))
                ->second;
        if (model_.turn() == Player::dark) {
            for (Position p : pos_set) {
                set.add_sprite(dark_sprite_, board_to_screen(p), 2);
            }
        } else if (model_.turn() == Player::light) {
            for (Position p : pos_set) {
                set.add_sprite(light_sprite_, board_to_screen(p), 2);
            }
        }
    }
}

Dimensions View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

void View::add_player_(ge211::Sprite_set &set,
                        Player play,
                        ge211::Position pos,
                        int z) const
{
    if(model_.is_game_over()) {
        if (model_.winner() == Player::dark) {
            for (ge211::Position pos : model_.board()) {
                if (model_.operator[](pos) ==
                    Player::light) {
                    set.add_sprite(loser_sprite_, board_to_screen(pos), z);
                }
            }
        } else if (model_.winner() == Player::light) {
            for (ge211::Position pos : model_.board()) {
                if (model_.operator[](pos) == Player::dark) {
                    set.add_sprite(loser_sprite_, board_to_screen(pos), z);
                }
            }
        }
    } else {
        if (play == Player::dark) {
            set.add_sprite(dark_sprite_, pos, z);
        } else if (play == Player::light) {
            set.add_sprite(light_sprite_, pos, z);
        }
    }
}

Position View::screen_to_board(Position pos) const
{
    pos.x = pos.x / grid_size;
    pos.y = pos.y / grid_size;

    return pos;
}

Position View::board_to_screen(Position pos) const
{
    pos.x = pos.x * grid_size;
    pos.y = pos.y * grid_size;

    return pos;
}
