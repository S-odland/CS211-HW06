#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    turn_ = Player::dark;
    compute_next_moves_();
}

Rectangle Model::board() const
{
    return board_.all_positions();
}

Player Model::operator[](Position pos) const
{
    return board_[pos];
}

Move const* Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end())
        return nullptr;
    else
        return &*i;
}

void Model::play_move(Position pos)
{
    if (is_game_over())
        throw Client_logic_error("Model::play_move: game over");

    Move const* movep = find_move(pos);
    if (!movep)
        throw Client_logic_error("Model::play_move: no such move");

    really_play_move_(*movep);
}

//
// BELOW ARE *OPTIONAL* HELPER FUNCTIONS
//

Position_set Model::find_flips_(Position current, Dimensions dir) const
{
    Position_set pset{};

    for (int n = 0; board_.good_position(current) ; ++n, current += n * dir) {
        if (board_[current] == other_player(turn_)) {
            pset[current] = true;
        }
        else if (board_[current] == turn_) {
            return pset;
        }
    }
    return Position_set();
}

Position_set Model::evaluate_position_(Position pos) const
{
    if (board_[pos] == Player::neither) return Position_set();

    Position_set pset{};
    for (Dimensions dir : Board::all_directions()) {
        pset |= find_flips_(pos, dir);
    }
    return pset;
}

void Model::compute_next_moves_()
{
    next_moves_.clear();

    for (Position pos : board_.center_positions()) {
        if ((board_[pos] == Player::neither))
            next_moves_[pos] = {pos};
    }
    if (!next_moves_.empty()) return;

    for (Position pos : board_.all_positions()) {
        Position_set pset = evaluate_position_(pos);
        if (!pset.empty()) next_moves_[pos] = pset;
    }
}

bool Model::advance_turn_()
{
    turn_ = other_player(turn_);
    compute_next_moves_();
    return !next_moves_.empty();
}

void Model::set_game_over_()
{
    turn_ = Player::neither;

    size_t dark_count = board_.count_player(Player::dark);
    size_t light_count = board_.count_player(Player::light);

    if (dark_count > light_count) {
        winner_ = Player::dark;
    } else if (dark_count < light_count) {
        winner_ = Player::light;
    } else {
        winner_ = Player::neither;
    }
}

void Model::really_play_move_(Move move)
{
    board_[move.first] = turn_;
    for (Position pos : move.second) {
        board_[pos] = turn_;
    }
    if (advance_turn_()) {
        return;
    } else if (advance_turn_()) {
        return;
    } else {
        set_game_over_();
    }
}
