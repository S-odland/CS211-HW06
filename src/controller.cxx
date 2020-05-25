#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height)
        , view_(model_)
{ }

void Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites);
}

void Controller::on_mouse_move(ge211::Position pos)
{
    mouse_pos = pos;
}

void Controller::on_mouse_up(ge211::Mouse_button, ge211::Position pos)
{
    mouse_click_pos = view_.board_to_screen(view_.screen_to_board(pos));
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

