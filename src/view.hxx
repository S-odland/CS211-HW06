#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    explicit View(Model const&);

    // You will probably want to add arguments here so that the
    // controller can communicate UI state (such as a mouse or
    // cursor position):

    void draw(ge211::Sprite_set& set, ge211::Position mouse_pos);

    ge211::Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    // Converts screen coordinates to board indices
    ge211::Position screen_to_board(ge211::Position pos) const;

    // Converts board indices to screen coordinates
    ge211::Position board_to_screen(ge211::Position pos) const;

private:
    Model const& model_;

    ge211::Circle_sprite
        dark_sprite_,
        light_sprite_,
        indicator_sprite_,
        border_sprite_;
    ge211::Rectangle_sprite
        grid_sprite_,
        flip_sprite_;
    ge211::Text_sprite
        text_sprite_;

    ge211::Font win_font_;

    // Adds a player sprite to the sprite set
    void add_player_(ge211::Sprite_set&, ge211::Position);

    void indicate_move_(ge211::Sprite_set&, ge211::Position);
};
