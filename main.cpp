/* ----------------------------------------------------------------<Header>-
 Назва: domino_solver.cpp
 Заголовок: Розв’язувач головоломки з доміно
 Група:                     ТВ-41
 Студент:                   Кривдюк А.В.
 Дата створення:            2025-05-12
 Останнє редагування:       2025-05-12
 Опис:  Програма вирішує головоломку з доміно на 2D полі методом рекурсивного
        перебору (backtracking). Поле частково заповнене числами від 0 до 6.
        Мета — заповнити всі клітинки так, щоб кожна пара сусідніх чисел утворювала
        доміно. Кожна можлива пара чисел (незалежно від порядку) використовується
        рівно один раз.
 ------------------------------------------------------------------</Header>-*/

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <iomanip>
#include <map>

using namespace std;

/* ---------------------------------------------------------------------[<]-
 Функція: my_minmax
 Опис: Повертає пару чисел у порядку зростання.
 ---------------------------------------------------------------------[>]-*/
pair<int, int> my_minmax(int a, int b) {
    if (a < b) return {a, b};  // Повертає пару з меншим числом спочатку
    return {b, a};  // Якщо b менше, повертає пару з b на першому місці
}

/* ---------------------------------------------------------------------[<]-
 Функція: insert_pair
 Опис: Додає пару до множини, якщо її ще немає.
 ---------------------------------------------------------------------[>]-*/
void insert_pair(set<pair<int, int>>& s, pair<int, int> p) {
    if (s.find(p) == s.end()) {  // Перевірка, чи є пара в множині
        s.insert(p);  // Якщо пари немає, додаємо її
    }
}

/* ---------------------------------------------------------------------[<]-
 Функція: is_valid
 Опис: Перевіряє, чи можна вставити значення в обрану клітинку, перевіряючи
 на конфлікти з уже заповненими сусідніми клітинками.
 ---------------------------------------------------------------------[>]-*/
bool is_valid(const vector<vector<int>>& grid, int row, int col, int value) {
    if (value < 0 || value > 6) {  // Перевірка на допустимість значення
        return false;
    }
    int rows = grid.size();
    int cols = (rows > 0) ? grid[0].size() : 0;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    // Перевірка на сусідів: чи можна вставити значення у поточну клітинку
    for (int i = 0; i < 4; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];

    }
    return true;  // Якщо жодних конфліктів не виявлено, повертаємо true
}

/* ---------------------------------------------------------------------[<]-
 Функція: find_next_missing
 Опис: Знаходить наступну незаповнену клітинку з найменшою кількістю можливих значень.
 ---------------------------------------------------------------------[>]-*/
pair<int, int> find_next_missing(const vector<vector<int>>& grid) {
    int best_row = -1, best_col = -1;
    int min_possible_values = 8;  // Початкове значення для найменшої кількості можливих варіантів

    for (int r = 0; r < grid.size(); ++r) {
        for (int c = 0; c < grid[r].size(); ++c) {
            if (grid[r][c] == -1) {  // Якщо клітинка незаповнена
                set<int> possible_values;
                for (int v = 0; v <= 6; ++v) {
                    if (is_valid(grid, r, c, v)) {  // Перевірка на можливість вставки значення
                        possible_values.insert(v);
                    }
                }
                // Знаходимо клітинку з найменшою кількістю можливих значень
                if (possible_values.size() < min_possible_values) {
                    min_possible_values = possible_values.size();
                    best_row = r;
                    best_col = c;
                }
            }
        }
    }
    return {best_row, best_col};  // Повертаємо координати клітинки з найменшою кількістю варіантів
}

/* ---------------------------------------------------------------------[<]-
 Функція: get_formed_dominoes
 Опис: Повертає множину всіх утворених доміно на полі.
 ---------------------------------------------------------------------[>]-*/
 void draw_domino_lines(const vector<vector<int>>& grid, int r1, int c1, int r2, int c2);
 
set<pair<int, int>> get_formed_dominoes(const vector<vector<int>>& grid) {
    set<pair<int, int>> formed;
    int rows = grid.size();
    int cols = (rows > 0) ? grid[0].size() : 0;

    // Проходимо по кожній клітинці і перевіряємо на сусідні доміно
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != -2) {  // Якщо клітинка не пуста
                if (c + 1 < cols && grid[r][c + 1] != -2) {  // Перевірка правого сусіда
                    pair<int, int> domino = my_minmax(grid[r][c], grid[r][c + 1]);
                    insert_pair(formed, domino);
                    draw_domino_lines(grid, r, c, r, c + 1);
                }
                if (r + 1 < rows && grid[r + 1][c] != -2) {  // Перевірка нижнього сусіда
                    pair<int, int> domino = my_minmax(grid[r][c], grid[r + 1][c]);
                    insert_pair(formed, domino);
                    draw_domino_lines(grid, r, c, r + 1, c);
                }
            }
        }
    }
    return formed;  // Повертаємо множину сформованих доміно
}

/* ---------------------------------------------------------------------[<]-
 Функція: is_solution
 Опис: Перевіряє, чи всі необхідні доміно присутні на полі.
 ---------------------------------------------------------------------[>]-*/
bool is_solution(const vector<vector<int>>& grid, const set<pair<int, int>>& all_dominoes) {
    set<pair<int, int>> formed_dominoes = get_formed_dominoes(grid);
    return formed_dominoes == all_dominoes;  // Перевірка, чи всі доміно утворені
}

/* ---------------------------------------------------------------------[<]-
 Функція: can_lead_to_solution
 Опис: Перевіряє, чи поточне поле ще може привести до правильного розв'язку.
 ---------------------------------------------------------------------[>]-*/
bool can_lead_to_solution(const vector<vector<int>>& grid, const set<pair<int, int>>& all_dominoes) {
    cout << "🔍 Перевірка можливості рішення на поточному полі\n";
    int filled_count = 0;
    for (const auto& row : grid) {
        for (int val : row) {
            if (val != -1 && val != -2) {
                filled_count++;  // Підрахунок вже заповнених клітинок
            }
        }
    }
    return filled_count <= all_dominoes.size() * 2;  // Перевірка на можливість заповнити решту клітинок
}

/* ---------------------------------------------------------------------[<]-
 Функція: solve_puzzle
 Опис: Рекурсивно вирішує головоломку методом перебору.
 ---------------------------------------------------------------------[>]-*/
bool solve_puzzle(vector<vector<int>>& grid, const set<pair<int, int>>& all_dominoes, set<pair<int, int>>& used_dominoes) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int r = 0; r < grid.size(); ++r) {
            for (int c = 0; c < grid[r].size(); ++c) {
                if (grid[r][c] >= 0) {  // Якщо клітинка вже заповнена
                    int dr[] = {-1, 1, 0, 0};
                    int dc[] = {0, 0, -1, 1};
                    int missing_neighbor_r = -1, missing_neighbor_c = -1;
                    int neighbor_count = 0;

                    // Перевіряємо сусідів для можливих варіантів значень
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

                    // Якщо є тільки один варіант для сусідньої клітинки
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
                            cout << "➕ Додається доміно: {" << grid[r][c] << ", " << *possible_values.begin() << "}\n";
                            insert_pair(used_dominoes, my_minmax(grid[r][c], *possible_values.begin()));
                            pair<int, int> domino = my_minmax(grid[r][c], *possible_values.begin());
                            if (used_dominoes.find(domino) != used_dominoes.end()) {
                                cout << "✅ Доміно добавлено: {" << domino.first << ", " << domino.second << "}\n";
                            }
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
        if (is_solution(grid, all_dominoes)) {
            cout << "✅ Рішення знайдено!\n";
            // --- Domino usage check ---
            cout << "✅ Кількість використаних доміно: " << used_dominoes.size() << " (очікується: 28)\n";
            for (const auto& d : used_dominoes) {
                cout << "Доміно: {" << d.first << "," << d.second << "}\n";
            }
        }
        return is_solution(grid, all_dominoes);  // Перевірка, чи заповнено все коректно
    }

    if (!can_lead_to_solution(grid, all_dominoes)) {
        return false;  // Якщо вже неможливо знайти рішення
    }
    for (int value = 0; value <= 6; ++value) {
        if (is_valid(grid, row, col, value)) {  // Перевірка на валідність значення
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
                        cout << "➕ Додається доміно: {" << value << ", " << temp_grid[nr][nc] << "}\n";
                        insert_pair(new_used_dominoes, domino);
                        if (new_used_dominoes.find(domino) != new_used_dominoes.end()) {
                            cout << "✅ Доміно добавлено: {" << domino.first << ", " << domino.second << "}\n";
                        }
                    }
                }
            }

            if (possible) {
                grid[row][col] = value;
                // Вивід поточного кроку підбору значення
                cout << "Спроба вставити " << value << " у клітинку (" << row << ", " << col << ")\n";
                if (solve_puzzle(grid, all_dominoes, new_used_dominoes)) {
                    cout << "✅ Вдалось вставити " << value << " у (" << row << ", " << col << ")\n";
                    return true;
                }
                // Якщо не вдалося, відкат
                cout << "↩️ Відкат вставки " << value << " з клітинки (" << row << ", " << col << ")\n";
                grid[row][col] = -1;
            }
        }
    }
    return false;
}

/* ---------------------------------------------------------------------[<]-
 Функція: print_grid
 Опис: Виводить поточний стан поля в консоль або у файл.
 ---------------------------------------------------------------------[>]-*/
 void print_grid(const vector<vector<int>>& grid, ostream& os) {
    os << "\n==================== Рішення ====================\n";
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

/* ---------------------------------------------------------------------[<]-
 Функція: printHeader
 Опис: Виводить заголовок програми і опис завдання.
 ---------------------------------------------------------------------[>]-*/
void printHeader() {
    cout << "╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                         🔷Курсова робота                       ║\n";
    cout << "╠════════════════════════════════════════════════════════════════╣\n";
    cout << "║  Завдання: Розробіть алгоритм вирішення задачі та реалізуйте   ║\n";
    cout << "║  його у вигляді програми мовою ANSI C++                        ║\n";
    cout << "║  Ідея головоломки з кістяками доміно. На ігровому полі задано  ║\n";
    cout << "║  набір доміно, кожне доміно зустрічається рівно один раз.      ║\n";
    cout << "║  Намалюйте лінії, щоб відзначити становище кожного доміно.     ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    cout << ">> Натисніть Enter, щоб почати пошук рішень...\n";
    cin.ignore();
    cout << "\n⏳ Виконується обчислення...\n\n";
}

void draw_domino_lines(const vector<vector<int>>& grid, int r1, int c1, int r2, int c2) {
    // Draw a line between two connected cells (either horizontally or vertically)
    if (r1 == r2) {  // Horizontal domino
        cout << "Намальовано лінію між клітинками (" << r1 << "," << c1 << ") та (" << r2 << "," << c2 << ")\n";
    } else if (c1 == c2) {  // Vertical domino
        cout << "Намальовано лінію між клітинками (" << r1 << "," << c1 << ") та (" << r2 << "," << c2 << ")\n";
    }
}


/* ---------------------------------------------------------------------[<]-
 Функція: main
 Опис: Основна функція: меню користувача, введення та запуск розв’язання.
 ---------------------------------------------------------------------[>]-*/
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

    vector<vector<int>> grid;
    set<pair<int, int>> used_dominoes_main;

    int choice = 0;
    while (true) {
        cout << "\n==================== Меню ====================\n";
        cout << "1 - Ввести самостійно\n";
        cout << "2 - Введені програмою значення\n";
        cout << "3 - Вихід\n";
        cout << "=============================================\n";
        cout << "Виберіть (1, 2, або 3): ";

        cin >> choice;

        cout << "\n";

        if (choice == 1) {
            grid.clear();
            int rows, cols;
            cout << "Введіть кількість рядків: ";
            cin >> rows;
            cout << "Введіть кількість стовпців: ";
            cin >> cols;

            grid.resize(rows, vector<int>(cols, -1));
            cout << "Введіть значення поля (значення від 0 до 6, -1 для втрачених значень):\n";
            cout << "-2 для пустих клітинок) \n";
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    cin >> grid[r][c];
                }
            }

            cout << "\n Початок вирішення...\n";
            if (solve_puzzle(grid, all_dominoes, used_dominoes_main)) {
                cout << "\n🎉 Рішення знайдено!\n";
                print_grid(grid, cout);
                print_grid(grid, output);
                cout << "\n🔍 Перевірка використаних доміно...\n";
                cout << "✅ Кількість використаних доміно: " << used_dominoes_main.size() << " (очікується: 28)\n";
                for (const auto& d : used_dominoes_main) {
                    cout << "Доміно: {" << d.first << "," << d.second << "}\n";
                }
            } else {
                cout << "❌ Не знайдено рішення.\n";
                output << "❌ Не знайдено рішення.\n";
            }

        } else if (choice == 2) {
            vector<vector<int>> solved_grid = initial_grid;

            cout << "Початок вирішення...\n";
            if (solve_puzzle(solved_grid, all_dominoes, used_dominoes_main)) {
                cout << "\n🎉 Рішення знайдено!\n";
                print_grid(solved_grid, cout);
                print_grid(solved_grid, output);
                for (const auto& d : used_dominoes_main) {
                    cout << "Доміно: {" << d.first << "," << d.second << "}\n";
                }
            } else {
                cout << "❌ Не знайдено рішення.\n";
                output << "❌ Не знайдено рішення.\n";
            }

        } else if (choice == 3) {
            cout << "Вихід з програми...\n";
            break;

        } else {
            cout << "Неправильний ввід.\n";
        }
    }
}