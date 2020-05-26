#include "model.hxx"
#include <catch.hxx>

using namespace ge211;

struct Test_access {
    Model& m_;

    Board& board() {
        return m_.board_;
    }

    Player& turn() {
        return m_.turn_;
    }

    Move_map& move_map() {
        return m_.next_moves_;
    }

    Position_set find_flips(Position start, Dimensions dir) {
        return m_.find_flips_(start, dir);
    }

    Position_set evaluate_position(Position pos) {
        return m_.evaluate_position_(pos);
    }
};

// These pass with the starter code and should continue
// to pass.
TEST_CASE("Passes with starter code")
{
    Model m(8, 8);
    CHECK( m.board() == Rectangle{0, 0, 8, 8} );
    CHECK_FALSE( m.is_game_over() );
    CHECK( m.turn() == Player::dark );
    CHECK( m.winner() == Player::neither );
    CHECK( m[{0, 0}] == Player::neither );
    CHECK( m.find_move({0, 0}) == nullptr );
    CHECK_THROWS_AS( m.play_move({0, 0}), Client_logic_error );
}

// This fails with the starter code, but should pass.
TEST_CASE("Fails with starter code")
{
    Model m(6);
    CHECK( m.find_move({2, 2}) );
}

// This fails with the starter code, but should pass.
TEST_CASE("Throws with starter code")
{
    Model m(4);
    m.play_move({1, 1});
}

TEST_CASE("Transition to Main Phase")
{
    Model m(8);
    Test_access t{m};

    m.play_move({3, 3}); //Dark
    m.play_move({3, 4}); //Light
    m.play_move({4, 3}); //Dark
    m.play_move({4, 4}); //Light

    Position_set pset = t.find_flips(Position{3,5}, Dimensions{0,-1});

    CHECK( pset[{3,4}] );
    CHECK( m.find_move({3, 5}) );

}
