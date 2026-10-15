#include "test_main.hpp"
#include "input/drag.hpp"
using namespace triples;
TEST(button_slide_preserves_board_until_commit) {
    game::Board board{};
    board.cells[0] = 1;
    board.cells[1] = 2;
    input::DragController drag;
    drag.start_external_commit(game::Direction::Left, board);
    CHECK_EQ(drag.state, input::DragState::Committing);
    drag.step(0.07f);
    CHECK(drag.f > 0.0f && drag.f < 1.0f);
    CHECK(!drag.commit_ready);
    CHECK_EQ(board.cells[0], 1);
    CHECK_EQ(board.cells[1], 2);
    drag.start_external_commit(game::Direction::Right, board);
    CHECK_EQ(drag.dir, game::Direction::Left);
    drag.step(0.08f);
    CHECK(drag.commit_ready);
    CHECK_EQ(drag.dry.new_board.cells[0], 3);
}
TEST(rejected_button_move_does_not_commit) {
    game::Board board{};
    board.cells[0] = 1;
    input::DragController drag;
    drag.start_external_commit(game::Direction::Left, board);
    drag.step(1.0f);
    CHECK_EQ(drag.state, input::DragState::Idle);
    CHECK(!drag.commit_ready);
}
