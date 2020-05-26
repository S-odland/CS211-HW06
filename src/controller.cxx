#include "controller.hxx"

using Position = ge211::Position;
using Dimensions = ge211::Dimensions;
using Color = ge211::Color;

// Position of the mouse on the board, always in board coordinates (0-7)
static Position mouse_pos{0, 0};

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height)
        , view_(model_)
{ }

void Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_pos);
    if (model_.turn() == Player::dark)
        background_color = Color::black();
    else if (model_.turn() == Player::light)
        background_color = Color::white();
    else {
        background_color = Color{128, 128, 128};
        get_window().set_dimensions(initial_window_dimensions() +
        Dimensions{0, initial_window_dimensions().height / model_.board().height});
    }

}

void Controller::on_mouse_move(ge211::Position pos)
{
    mouse_pos = view_.screen_to_board(pos);
}

void Controller::on_mouse_up(ge211::Mouse_button, ge211::Position pos)
{
    ge211::Position board_pos = view_.screen_to_board(pos);

    if (model_.find_move(board_pos))
        model_.play_move(view_.screen_to_board(pos));
}

ge211::Dimensions Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

