#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <iomanip>
#include <map>

using namespace std;

pair<int, int> my_minmax(int a, int b) {
    if (a < b) return {a, b};
    return {b, a};
}

void insert_pair(set<pair<int, int>>& s, pair<int, int> p) {
    if (s.find(p) == s.end()) {
        s.insert(p);
    }
}
bool is_valid(const vector<vector<int>>& grid, int row, int col, int value) {
    if (value < 0 || value > 6) {
        return false;
    }
    int rows = grid.size();
    int cols = (rows > 0) ? grid[0].size() : 0;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] >= 0) {
        }
    }
    return true;
}

pair<int, int> find_next_missing(const vector<vector<int>>& grid) {
    int best_row = -1, best_col = -1;
    int min_possible_values = 8; 

    for (int r = 0; r < grid.size(); ++r) {
        for (int c = 0; c < grid[r].size(); ++c) {
            if (grid[r][c] == -1) {
                set<int> possible_values;
                for (int v = 0; v <= 6; ++v) {
                    if (is_valid(grid, r, c, v)) {
                        possible_values.insert(v);
                    }
                }
                if (possible_values.size() < min_possible_values) {
                    min_possible_values = possible_values.size();
                    best_row = r;
                    best_col = c;
                }
            }
        }
    }
    return {best_row, best_col};
}

set<pair<int, int>> get_formed_dominoes(const vector<vector<int>>& grid) {
    set<pair<int, int>> formed;
    int rows = grid.size();
    int cols = (rows > 0) ? grid[0].size() : 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != -2) {
                if (c + 1 < cols && grid[r][c + 1] != -2) {
                    insert_pair(formed, my_minmax(grid[r][c], grid[r][c + 1]));
                }
                if (r + 1 < rows && grid[r + 1][c] != -2) {
                    insert_pair(formed, my_minmax(grid[r][c], grid[r + 1][c]));
                }
            }
        }
    }
    return formed;
}

bool is_solution(const vector<vector<int>>& grid, const set<pair<int, int>>& all_dominoes) {
    set<pair<int, int>> formed_dominoes = get_formed_dominoes(grid);
    return formed_dominoes == all_dominoes;
}

bool can_lead_to_solution(const vector<vector<int>>& grid, const set<pair<int, int>>& all_dominoes) {
    int filled_count = 0;
    for (const auto& row : grid) {
        for (int val : row) {
            if (val != -1 && val != -2) {
                filled_count++;
            }
        }
    }
    return filled_count <= all_dominoes.size() * 2;
}

bool solve_puzzle(vector<vector<int>>& grid, const set<pair<int, int>>& all_dominoes, set<pair<int, int>>& used_dominoes) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int r = 0; r < grid.size(); ++r) {
            for (int c = 0; c < grid[r].size(); ++c) {
                if (grid[r][c] >= 0) {
                    int dr[] = {-1, 1, 0, 0};
                    int dc[] = {0, 0, -1, 1};
                    int missing_neighbor_r = -1, missing_neighbor_c = -1;
                    int neighbor_count = 0;

                    for (int i = 0; i < 4; ++i) {
                        int nr = r + dr[i];
                        int nc = c + dc[i];
                        if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[nr].size()) {
                            if (grid[nr][nc] == -1) {
                                missing_neighbor_r = nr;
                                missing_neighbor_c = nc;
                                neighbor_count++;
                            } else if (grid[nr][nc] >= 0) {
                                neighbor_count++;
                            }
                        }
                    }

                    if (neighbor_count < 2 && missing_neighbor_r != -1) {
                        set<int> possible_values;
                        for (int v = 0; v <= 6; ++v) {
                            vector<vector<int>> temp_grid = grid;
                            temp_grid[missing_neighbor_r][missing_neighbor_c] = v;
                            set<pair<int, int>> formed = get_formed_dominoes(temp_grid);
                            pair<int, int> domino = my_minmax(grid[r][c], v);
                            if (all_dominoes.count(domino) && used_dominoes.find(domino) == used_dominoes.end()) {
                                possible_values.insert(v);
                            }
                        }
                        if (possible_values.size() == 1) {
                            grid[missing_neighbor_r][missing_neighbor_c] = *possible_values.begin();
                            insert_pair(used_dominoes, my_minmax(grid[r][c], *possible_values.begin()));
                            changed = true;
                        }
                    }
                }
            }
        }
    }

    pair<int, int> next_missing = find_next_missing(grid);
    int row = next_missing.first;
    int col = next_missing.second;

    if (row == -1) {
        return is_solution(grid, all_dominoes);
    }

    if (!can_lead_to_solution(grid, all_dominoes)) {
        return false;
    }
    for (int value = 0; value <= 6; ++value) {
        if (is_valid(grid, row, col, value)) {
            vector<vector<int>> temp_grid = grid;
            temp_grid[row][col] = value;
            set<pair<int, int>> formed = get_formed_dominoes(temp_grid);
            set<pair<int, int>> new_used_dominoes = used_dominoes;
            bool possible = true;
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; ++i) {
                int nr = row + dr[i];
                int nc = col + dc[i];
                if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[nr].size() && temp_grid[nr][nc] >= 0) {
                    pair<int, int> domino = my_minmax(value, temp_grid[nr][nc]);
                    if (all_dominoes.count(domino) && used_dominoes.count(domino)) {
                        possible = false;
                        break;
                    } else if (all_dominoes.count(domino)) {
                        insert_pair(new_used_dominoes, domino);
                    }
                }
            }

            if (possible) {
                grid[row][col] = value;
                if (solve_puzzle(grid, all_dominoes, new_used_dominoes)) {
                    return true;
                }
                grid[row][col] = -1;
            }
        }
    }
    return false;
}

void print_grid(const vector<vector<int>>& grid, ostream& os) {
    os << "\n==================== SOLUTION ====================\n";
    for (const auto& row : grid) {
        for (int val : row) {
            if (val != -2) {
                os << setw(3) << val;
            } else {
                os << "   ";
            }
        }
        os << endl;
    }
    os << "==================================================\n";
}

void printHeader() {
    cout << "╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                 🔷Курсова робота                               ║\n";
    cout << "╠════════════════════════════════════════════════════════════════╣\n";
    cout << "║   Завдання: Розмістити всі доміно на дошці 6x13.               ║\n";
    cout << "║   Доміно містить пари чисел від 0 до 6 (всього 28 штук).       ║\n";
    cout << "║   На дошці можуть бути пусті (.) або заблоковані ( ) клітинки. ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    cout << ">> Натисніть Enter, щоб почати пошук рішень...\n";
    cin.ignore();
    cout << "\n⏳ Виконується обчислення...\n\n";
}

int main() {
    printHeader();
    ofstream output("output.txt");
    vector<vector<int>> initial_grid = {
        {3, 6, -2, -2, -2, 0, 1, 1, 3, 0, 1, 5, 5},
        {6, 2, -2, -1, -2, 0, 4, 5, -1, 6, 1, 6, 2},
        {0, 4, -2, 0, -2, 6, 5, 6, 1, -1, 3, -2, -2},
        {0, 2, 3, 5, 2, 2, 3, 3, -2, -2, -1, -2, -2},
        {6, 4, 2, 4, 4, 3, 5, 2, -2, -2, 5, 0, 1},
        {-2, -2, 1, -1, 4, -2, -2, -1, -2, -2, 1, 4, 2}
    };

    set<pair<int, int>> all_dominoes;
    for (int i = 0; i <= 6; ++i) {
        for (int j = i; j <= 6; ++j) {
            all_dominoes.insert({i, j});
        }
    }

    vector<vector<int>> solved_grid = initial_grid;
    set<pair<int, int>> used_dominoes_main;

    cout << "Starting to solve the puzzle...\n";

    if (solve_puzzle(solved_grid, all_dominoes, used_dominoes_main)) {
        cout << "\n🎉 Solution found!" << endl;
        print_grid(solved_grid, cout);
        print_grid(solved_grid, output);
    } else {
        cout << "❌ No solution found." << endl;
        output << "❌ No solution found." << endl;
    }

    output.close();
    cout << "\nSolution written to 'output.txt'.\n";
    return 0;
}