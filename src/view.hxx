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
    // pos will be determined by the mouse position in controller and b will
    // signify whether or not the mouse has been clicked or not

    void draw(ge211::Sprite_set& set);

    ge211::Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    ge211::Position screen_to_board(ge211::Position pos) const;
    ge211::Position board_to_screen(ge211::Position pos) const;

private:
    Model const& model_;

    ge211::Circle_sprite dark_sprite_;
    ge211::Circle_sprite light_sprite_;
    ge211::Circle_sprite dark_hover_sprite_;
    ge211::Circle_sprite light_hover_sprite_;
    ge211::Circle_sprite loser_sprite_;
    ge211::Rectangle_sprite grid_sprite_;

    void add_player_(ge211::Sprite_set& set,
                    Player p,
                    ge211::Position pos,
                    int z) const;
};
