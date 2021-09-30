#include <ctime>
#include <queue>
#include <random>
#include "minesweeper.h"

std::vector<Minesweeper::Cell> Minesweeper::MinePositionGenerator(size_t width, size_t height, size_t mines_count) {
    std::mt19937_64 generator(std::time(nullptr));
    size_t cnt_all_cells = width * height;
    std::vector<size_t> all_cells(cnt_all_cells);
    for (size_t i = 0; i < cnt_all_cells; ++i) {
        all_cells[i] = i;
    }
    std::vector<Minesweeper::Cell> cells_with_mines(mines_count);
    for (size_t i = 0; i < mines_count; ++i) {
        std::uniform_int_distribution<size_t> distribution(i, cnt_all_cells - 1);
        size_t choice = distribution(generator);
        std::swap(all_cells[i], all_cells[choice]);
        cells_with_mines[i] = {all_cells[i] % width, all_cells[i] / width};
    }
    return cells_with_mines;
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell> &cells_with_mines) {
    NewGame(width, height, cells_with_mines);
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    NewGame(width, height, mines_count);
}

void Minesweeper::ClearField() {
    for (size_t x = 0; x < width_; ++x) {
        delete[] field_[x];
    }
    delete[] field_;
    width_ = 0;
    height_ = 0;
    cnt_closed_cells_without_mines_ = 0;
}

Minesweeper::~Minesweeper() {
    ClearField();
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell> &cells_with_mines) {
    game_status_ = GameStatus::NOT_STARTED;
    start_time_ = -1;
    finish_time_ = -1;
    ClearField();

    width_ = width;
    height_ = height;
    cnt_closed_cells_without_mines_ = width * height - cells_with_mines.size();
    field_ = new CellData *[width_];
    for (size_t x = 0; x < width_; ++x) {
        field_[x] = new CellData[height_];
    }
    for (auto [x, y] : cells_with_mines) {
        field_[x][y].is_mine = true;
        for (int32_t delta_x = -1; delta_x <= 1; ++delta_x) {
            if ((delta_x == -1 && x > 0) || delta_x == 0 || (delta_x == 1 && x + 1 < width_)) {
                size_t around_x = x + delta_x;
                for (int32_t delta_y = -1; delta_y <= 1; ++delta_y) {
                    if ((delta_y == -1 && y > 0) || delta_y == 0 || (delta_y == 1 && y + 1 < height_)) {
                        size_t around_y = y + delta_y;
                        if (0 <= around_x && around_x < width_ && 0 <= around_y && around_y < height_ &&
                            (x != around_x || y != around_y)) {
                            ++field_[around_x][around_y].count_mine_neighboring_cells;
                        }
                    }
                }
            }
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    std::vector<Minesweeper::Cell> cells_with_mines = Minesweeper::MinePositionGenerator(width, height, mines_count);
    NewGame(width, height, cells_with_mines);
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

time_t Minesweeper::GetGameTime() const {
    if (game_status_ == GameStatus::NOT_STARTED) {
        return 0;
    } else if (game_status_ == GameStatus::IN_PROGRESS) {
        return std::time(nullptr) - start_time_;
    } else {
        return finish_time_;
    }
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    RenderedField rendered_field(height_, std::string(width_, '-'));
    for (size_t x = 0; x < width_; ++x) {
        for (size_t y = 0; y < height_; ++y) {
            if (field_[x][y].is_marked) {
                rendered_field[y][x] = '?';
            } else if (field_[x][y].is_opened) {
                if (field_[x][y].is_mine) {
                    rendered_field[y][x] = '*';
                } else if (field_[x][y].count_mine_neighboring_cells == 0) {
                    rendered_field[y][x] = '.';
                } else {
                    rendered_field[y][x] = '0' + static_cast<char>(field_[x][y].count_mine_neighboring_cells);
                }
            }
        }
    }
    return rendered_field;
}

void Minesweeper::MarkCell(const Cell &cell) {
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_time_ = std::time(nullptr);
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (game_status_ == GameStatus::IN_PROGRESS) {
        field_[cell.x][cell.y].is_marked = !field_[cell.x][cell.y].is_marked;
    }
}

void Minesweeper::OpenCell(const Cell &cell) {
    if (game_status_ == GameStatus::DEFEAT || game_status_ == GameStatus::VICTORY || field_[cell.x][cell.y].is_marked) {
        return;
    }
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_time_ = std::time(nullptr);
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (field_[cell.x][cell.y].is_mine) {
        finish_time_ = std::time(nullptr);
        game_status_ = GameStatus::DEFEAT;
        for (size_t x = 0; x < width_; ++x) {
            for (size_t y = 0; y < height_; ++y) {
                field_[x][y].is_opened = true;
            }
        }
    } else if (!field_[cell.x][cell.y].is_opened) {
        field_[cell.x][cell.y].is_opened = true;
        --cnt_closed_cells_without_mines_;
        std::queue<Cell> bfs_queue;
        bfs_queue.push(cell);
        while (!bfs_queue.empty()) {
            Cell cur_cell = bfs_queue.front();
            bfs_queue.pop();
            if (field_[cur_cell.x][cur_cell.y].count_mine_neighboring_cells == 0) {
                for (int32_t delta_x = -1; delta_x <= 1; ++delta_x) {
                    if ((delta_x == -1 && cur_cell.x > 0) || delta_x == 0 ||
                        (delta_x == 1 && cur_cell.x + 1 < width_)) {
                        size_t around_x = cur_cell.x + delta_x;
                        for (int32_t delta_y = -1; delta_y <= 1; ++delta_y) {
                            if ((delta_y == -1 && cur_cell.y > 0) || delta_y == 0 ||
                                (delta_y == 1 && cur_cell.y + 1 < height_)) {
                                size_t around_y = cur_cell.y + delta_y;
                                if ((cur_cell.x != around_x || cur_cell.y != around_y) &&
                                    !field_[around_x][around_y].is_opened && !field_[around_x][around_y].is_marked) {
                                    field_[around_x][around_y].is_opened = true;
                                    --cnt_closed_cells_without_mines_;
                                    bfs_queue.push({around_x, around_y});
                                }
                            }
                        }
                    }
                }
            }
        }
        if (cnt_closed_cells_without_mines_ == 0) {
            finish_time_ = std::time(nullptr);
            game_status_ = GameStatus::VICTORY;
        }
    }
}
