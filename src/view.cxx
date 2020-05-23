#include "view.hxx"
#include <cmath>

// Convenient type aliases:
using Dimensions = ge211::Dimensions;
using Position   = ge211::Position;
using Color      = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;
static int const sprite_radius = 6;

static ge211::Color const
        dark_sprite_color = ge211::Color::black(),
        light_sprite_color = ge211::Color::white(),
        dark_hover = dark_sprite_color.lighten(0.5),
        light_hover = dark_sprite_color.lighten(0.5),
        loser_color = ge211::Color {128,128,128},
        grid_color = ge211::Color {0, 255, 0};


View::View(Model const& model)
        : model_(model)
        , dark_sprite_(sprite_radius, dark_sprite_color)
        , light_sprite_(sprite_radius, light_sprite_color)
        , dark_hover_sprite_(sprite_radius, dark_hover)
        , light_hover_sprite_(sprite_radius, light_hover)
        , loser_sprite_(sprite_radius, loser_color)
        , grid_sprite_({grid_size - 2,grid_size - 2},grid_color)
{ }

void View::draw(Sprite_set& set, Position pos, bool b)
{
    for ( ge211::Position pos : model_.board() )
    {
        set.add_sprite(grid_sprite_,screen_to_board(pos),0);
    }

    if (b){
        add_player_(set,model_.turn(),pos,1);
    } else {
        hover_player_(set,model_.turn(),pos);
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
                        Player p,
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
        if (p == Player::dark) {
            set.add_sprite(dark_sprite_, pos, z);
        } else if (p == Player::light) {
            set.add_sprite(light_sprite_, pos, z);
        }
    }
}

void View::hover_player_(ge211::Sprite_set &set,
                       Player p,
                       ge211::Position pos) const
{
    if (p == Player::dark){
        set.add_sprite(dark_hover_sprite_, pos, 1);
        Position_set pos_set = model_.find_move(screen_to_board(pos))->second;

        for (Position poss : pos_set)
        {
            set.add_sprite(dark_sprite_, board_to_screen(poss), 2);
        }

    } else if (p == Player::light){
        set.add_sprite(light_hover_sprite_, pos, 1);
        Position_set pos_set = model_.find_move(screen_to_board(pos))->second;

        for (Position poss : pos_set)
        {
            set.add_sprite(dark_sprite_, board_to_screen(poss), 2);
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
