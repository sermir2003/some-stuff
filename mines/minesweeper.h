#pragma once

#include <string>
#include <vector>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    struct CellData {
        bool is_opened = false;
        bool is_mine = false;
        bool is_marked = false;
        int8_t count_mine_neighboring_cells = 0;
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

    ~Minesweeper();

    void ClearField();
    static std::vector<Cell> MinePositionGenerator(size_t width, size_t height, size_t mines_count);

private:
    GameStatus game_status_ = GameStatus::NOT_STARTED;
    time_t start_time_ = -1;
    time_t finish_time_ = -1;
    size_t width_ = 0;
    size_t height_ = 0;
    size_t cnt_closed_cells_without_mines_ = 0;
    CellData** field_ = nullptr;
};
