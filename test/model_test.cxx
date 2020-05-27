#include "model.hxx"
#include <catch.hxx>

using namespace ge211;
using P = Player;

struct Test_access
{
    Model& m_;

    Board& board()
    {
        return m_.board_;
    }

    Player& turn()
    {
        return m_.turn_;
    }

    Move_map& next_moves()
    {
        return m_.next_moves_;
    }

    Position_set find_flips(Position start, Dimensions dir)
    {
        return m_.find_flips_(start, dir);
    }

    Position_set evaluate_position(Position pos)
    {
        return m_.evaluate_position_(pos);
    }
};

// These pass with the starter code and should continue
// to pass.
TEST_CASE("Passes with starter code")
{
    Model m(8, 8);
    CHECK(m.board() == Rectangle{0, 0, 8, 8});
    CHECK_FALSE(m.is_game_over());
    CHECK(m.turn() == Player::dark);
    CHECK(m.winner() == Player::neither);
    CHECK(m[{0, 0}] == Player::neither);
    CHECK(m.find_move({0, 0}) == nullptr);
    CHECK_THROWS_AS(m.play_move({0, 0}), Client_logic_error);
}

// This fails with the starter code, but should pass.
TEST_CASE("Fails with starter code")
{
    Model m(6);
    CHECK(m.find_move({2, 2}));
}

// This fails with the starter code, but should pass.
TEST_CASE("Throws with starter code")
{
    Model m(4);
    m.play_move({1, 1});
}

TEST_CASE("Proper Opening Phase")
{
    Model m(8);
    Test_access t{m};

    CHECK( m.turn() == P::dark );

    CHECK( m.find_move({3, 3}) );
    CHECK( m.find_move({3, 4}) );
    CHECK( m.find_move({4, 3}) );
    CHECK( m.find_move({4, 4}) );

    CHECK_THROWS_AS( m.play_move({0,0}), Client_logic_error );
    CHECK_NOTHROW( m.play_move({3,3}) );

}

TEST_CASE("Transition to Main Phase")
{
    Model       m(8);
    Test_access t{m};

    m.play_move({3, 3}); //Dark
    m.play_move({4, 3}); //Light
    m.play_move({4, 4}); //Dark
    m.play_move({3, 4}); //Light

    Position_set pset = t.find_flips(Position{3, 5}, Dimensions{0, -1});
    CHECK( pset[{3, 4}] );

    CHECK( m.find_move({3, 5}) );
    CHECK( m.find_move({2, 4}) );
    CHECK( m.find_move({4, 2}) );
    CHECK( m.find_move({5, 3}) );

    CHECK_THROWS_AS( m.play_move({3, 6}), Client_logic_error);

    CHECK_NOTHROW( m.play_move({3,5}) );

    CHECK( m[{3,5}] == P::dark );
    CHECK( m[{3, 4}] == P::dark );
}

TEST_CASE("Dark Places Twice")
{
    Model       m(4);
    Test_access t{m};

    t.board()[{0, 0}] = P::dark;
    t.board()[{1, 0}] = P::dark;
    t.board()[{2, 0}] = P::light;
    t.board()[{0, 1}] = P::light;

    t.board()[{1, 1}] = P::light;
    t.board()[{1, 2}] = P::light;
    t.board()[{2, 1}] = P::light;

    t.turn() = P::light;
    m.play_move({2, 2});

    CHECK( m.turn() == P::dark );

    CHECK( m.find_move({3, 0}) );

    CHECK( m.turn() == Player::dark);

    m.play_move({3, 0});

    CHECK( m.turn() == Player::dark);
}

TEST_CASE("Dark wins")
{
    Model       m(4);
    Test_access t{m};

    Player p = P::dark;

    for (Position pos : t.board().all_positions()) {
        t.board()[pos] = p = other_player(p);
    }
    t.board()[{1, 2}] = P::neither;

    m.play_move({1, 2});

    CHECK( m.is_game_over() );
    CHECK( t.board().count_player(P::light) == 7 );
    CHECK( t.board().count_player(P::dark) == 9 );

    CHECK( m.winner() == P::dark );
}

TEST_CASE("Light wins")
{
    Model       m(4);
    Test_access t{m};

    Player p = P::light;

    for (Position pos : t.board().all_positions()) {
        t.board()[pos] = p = other_player(p);
    }
    t.board()[{1, 2}] = P::neither;
    t.turn() = P::light;

    m.play_move({1, 2});

    CHECK( m.is_game_over() );
    CHECK( t.board().count_player(P::light) == 9 );
    CHECK( t.board().count_player(P::dark) == 7 );

    CHECK( m.winner() == P::light );
}

TEST_CASE("Tie Game")
{
    Model       m(2);
    Test_access t{m};

    for (Position pos : t.board().all_positions() ) {
        m.play_move(pos);
    }

    CHECK( t.next_moves().empty() );

    CHECK( m.is_game_over() );

    CHECK( t.board().count_player(P::light) == 2 );
    CHECK( t.board().count_player(P::neither) == 0 );

    CHECK( m.turn() == P::neither );
    CHECK( m.winner() == P::neither );
}

TEST_CASE("Play full game")
{
    Model       m(8);
    Test_access t{m};

    while (!m.is_game_over()) {

        Move_map mm = t.next_moves();
        Position p = mm.begin()->first;
        Position_set pset = mm.begin()->second;
        Board before = t.board();

        CHECK_NOTHROW( m.play_move(p) );
        for (auto pos : pset) {
            if (before[pos] != P::neither)
                CHECK( m[pos] == other_player(before[pos]) );
        }
    }
    CHECK( t.next_moves().empty() );
    CHECK( m.turn() == P::neither );
}

TEST_CASE("Play full game with odd dimensions")
{
    Model       m(7);
    Test_access t{m};

    while (!m.is_game_over()) {

        Move_map mm = t.next_moves();
        Position p = mm.begin()->first;
        Position_set pset = mm.begin()->second;
        Board before = t.board();

        CHECK_NOTHROW( m.play_move(p) );
        for (auto pos : pset) {
            if (before[pos] != P::neither)
                CHECK( m[pos] == other_player(before[pos]) );
        }
    }
    CHECK( t.next_moves().empty() );
    CHECK( m.turn() == P::neither );
}


