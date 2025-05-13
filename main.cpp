/* ----------------------------------------------------------------<Header>-
 Назва:        domino_solver.cpp
 Заголовок:    Розв'язувач головоломки з доміно
 Група:        ТВ-41
 Студент:      Кривдюк А.В.
 Дата створення: 2025-05-12
 Останнє редагування: 2025-05-12
 Опис:
    Програма розв'язує головоломку з доміно на 2D полі методом рекурсивного
    перебору (backtracking). Поле частково заповнене числами від 0 до 6.
    Мета - заповнити всі клітинки так, щоб кожна пара сусідніх чисел
    утворювала доміно. Кожна можлива пара чисел (незалежно від порядку)
    використовується рівно один раз.
 ------------------------------------------------------------------</Header>-*/

 #include <iostream>
 #include <vector>
 #include <set>
 #include <fstream>
 #include <iomanip>
 #include <map>
 #include <limits>
 
 using namespace std;
 
 #define EMPTY -2
 #define MISSING -1
 #define MIN_VALUE 0
 #define MAX_VALUE 6
 #define TOTAL_DOMINOES 28
 #define MAX_GRID_CELLS 100
 
 /* ---------------------------------------------------------------------[<]-
     Клас DominoSolver
     Реалізує логіку вирішення головоломки з доміно методом backtracking.
  ---------------------------------------------------------------------[>]-*/
 class DominoSolver {
 public:
     set<pair<int, int>> autoInserted;
 private:
     vector<vector<int>> grid;
     set<pair<int, int>> allDominoes;
     set<pair<int, int>> usedDominoes;
 
 /* ---------------------------------------------------------------------[<]-
       Повертає пару чисел у порядку зростання.
  ---------------------------------------------------------------------[>]-*/
     pair<int, int> myMinMax(int a, int b) {
         if (a < b) {
             return {a, b};
         } else {
             return {b, a};
         }
     }
 /* ---------------------------------------------------------------------[<]-
      Додає пару до множини, якщо її ще немає.
 ---------------------------------------------------------------------[>]-*/
     void insertPair(set<pair<int, int>>& s, pair<int, int> p) {
         if (!s.count(p)) {
             s.insert(p);
         }
     }
 /* ---------------------------------------------------------------------[<]-
       Перевіряє, чи можна вставити значення в обрану клітинку.
 ---------------------------------------------------------------------[>]-*/
     bool isValid(int row, int col, int value) {
         if (value < MIN_VALUE || value > MAX_VALUE) {
             return false;
         }
         int rows = grid.size();
         int cols = (rows > 0) ? grid[0].size() : 0;
         int dr[4] = {-1, 1, 0, 0};
         int dc[4] = {0, 0, -1, 1};
         for (int i = 0; i < 4; i++) {
             int newRow = row + dr[i];
             int newCol = col + dc[i];
             if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                 if (grid[newRow][newCol] >= MIN_VALUE && grid[newRow][newCol] <= MAX_VALUE) {
                     pair<int, int> domino = myMinMax(value, grid[newRow][newCol]);
                     if (allDominoes.count(domino) && usedDominoes.count(domino)) {
                         return false;
                     }
                 }
             }
         }
         return true;
     }
 /* ---------------------------------------------------------------------[<]-
      Знаходить наступну незаповнену клітинку з найменшою кількістю можливих значень.
 ---------------------------------------------------------------------[>]-*/
     pair<int, int> findNextMissing() {
         int best_row = -1, best_col = -1;
         int min_possible_values = MAX_VALUE + 2;
         for (int r = 0; r < grid.size(); ++r) {
             for (int c = 0; c < grid[r].size(); ++c) {
                 if (grid[r][c] == MISSING) {
                     set<int> possible_values;
                     for (int v = MIN_VALUE; v <= MAX_VALUE; ++v) {
                         if (isValid(r, c, v)) {
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
 /* ---------------------------------------------------------------------[<]-
      Повертає множину всіх утворених доміно на полі.
  ---------------------------------------------------------------------[>]-*/
     set<pair<int, int>> getFormedDominoes() {
         set<pair<int, int>> formed;
         int rows = grid.size();
         int cols = (rows > 0) ? grid[0].size() : 0;
         for (int r = 0; r < rows; ++r) {
             for (int c = 0; c < cols; ++c) {
                 if (grid[r][c] != EMPTY) {
                     if (c + 1 < cols && grid[r][c + 1] != EMPTY) {
                         pair<int, int> domino = myMinMax(grid[r][c], grid[r][c + 1]);
                         insertPair(formed, domino);
                         drawDominoLines(r, c, r, c + 1);
                     }
                     if (r + 1 < rows && grid[r + 1][c] != EMPTY) {
                         pair<int, int> domino = myMinMax(grid[r][c], grid[r + 1][c]);
                         insertPair(formed, domino);
                         drawDominoLines(r, c, r + 1, c);
                     }
                 }
             }
         }
         return formed;
     }
 /* ---------------------------------------------------------------------[<]-
      Перевіряє, чи всі необхідні доміно присутні на полі.
  ---------------------------------------------------------------------[>]-*/
     bool isSolution() {
         set<pair<int, int>> formed_dominoes = getFormedDominoes();
         return formed_dominoes == allDominoes;
     }
 /* ---------------------------------------------------------------------[<]-
       Перевіряє, чи поточне поле ще може привести до правильного розв'язку.
  ---------------------------------------------------------------------[>]-*/
      bool canLeadToSolution() {
         cout << "🔍 Перевірка можливості рішення на поточному полі\n";
         int filled_count = 0;
         for (const auto& row : grid) {
             for (int val : row) {
                 if (val != MISSING && val != EMPTY) {
                     filled_count++;
                 }
             }
         }
         return filled_count <= int(allDominoes.size()) * 2;
     }
 /* ---------------------------------------------------------------------[<]-
       Малює лінії доміно між клітинками (можна змінити для графічного інтерфейсу).
  ---------------------------------------------------------------------[>]-*/
     void drawDominoLines(int r1, int c1, int r2, int c2) {
         if (r1 == r2) {
             cout << "Намальовано лінію між клітинками (" << r1 << "," << c1 << ") та (" << r2 << "," << c2 << ")\n";
         } else if (c1 == c2) {
             cout << "Намальовано лінію між клітинками (" << r1 << "," << c1 << ") та (" << r2 << "," << c2 << ")\n";
         }
     }
 public:
 /* ---------------------------------------------------------------------[<]-
      Виводить поточний стан поля в консоль або у файл.
  ---------------------------------------------------------------------[>]-*/
    // Виводить поточний стан поля в консоль
    void printGrid(ostream& os) {
        os << "\n==================== Рішення ====================\n";
        for (int r = 0; r < grid.size(); ++r) {
            for (int c = 0; c < grid[r].size(); ++c) {
                int val = grid[r][c];
                if (val != EMPTY) {
                    if (autoInserted.count({r, c})) {
                        os << "\033[32m" << val << "\033[0m ";
                    } else {
                        os << val << " ";
                    }
                } else {
                    os << "  ";
                }
            }
            os << endl;
        }
        os << "==================================================\n";
        os << "\nВикористані доміно:\n";
        for (const auto& d : usedDominoes) {
            os << "Доміно: {"
               << "\033[32m" << d.first << "\033[0m"
               << ", "
               << "\033[32m" << d.second << "\033[0m"
               << "}\n";
        }
    }

    // Виводить поточний стан поля у файл
    void printGridToFile(ostream& os) {
        os << "\n==================== Рішення ====================\n";
        for (int r = 0; r < grid.size(); ++r) {
            for (int c = 0; c < grid[r].size(); ++c) {
                int val = grid[r][c];
                if (val != EMPTY) {
                    if (autoInserted.count({r, c})) {
                        os << val << " ";
                    } else {
                        os << val << " ";
                    }
                } else {
                    os << "  ";
                }
            }
            os << endl;
        }
        os << "==================================================\n";
    }
 /* ---------------------------------------------------------------------[<]-
      Рекурсивно вирішує головоломку методом перебору.
  ---------------------------------------------------------------------[>]-*/
     bool solvePuzzle() {
         bool changed = true;
         while (changed) {
             changed = false;
             for (int r = 0; r < grid.size(); r++) {
                 for (int c = 0; c < grid[r].size(); c++) {
                     if (grid[r][c] >= MIN_VALUE && grid[r][c] <= MAX_VALUE) {
                         int dr[4] = {-1, 1, 0, 0};
                         int dc[4] = {0, 0, -1, 1};
                         int missing_neighbor_r = -1, missing_neighbor_c = -1;
                         int neighbor_count = 0;
                         for (int i = 0; i < 4; i++) {
                             int nr = r + dr[i];
                             int nc = c + dc[i];
                             if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[nr].size()) {
                                 if (grid[nr][nc] == MISSING) {
                                     missing_neighbor_r = nr;
                                     missing_neighbor_c = nc;
                                     neighbor_count++;
                                 } else if (grid[nr][nc] >= MIN_VALUE && grid[nr][nc] <= MAX_VALUE) {
                                     neighbor_count++;
                                 }
                             }
                         }
                         if (neighbor_count < 2 && missing_neighbor_r != -1) {
                             set<int> possible_values;
                             for (int v = MIN_VALUE; v <= MAX_VALUE; v++) {
                                 vector<vector<int>> temp_grid = grid;
                                 temp_grid[missing_neighbor_r][missing_neighbor_c] = v;
                                 pair<int, int> domino = myMinMax(grid[r][c], v);
                                 if (allDominoes.count(domino) && !usedDominoes.count(domino)) {
                                     possible_values.insert(v);
                                 }
                             }
                             if (possible_values.size() == 1) {
                                 grid[missing_neighbor_r][missing_neighbor_c] = *possible_values.begin();
                                 autoInserted.insert({missing_neighbor_r, missing_neighbor_c});
                                 cout << "➕ Додається доміно: {" << grid[r][c] << ", " << *possible_values.begin() << "} // Додаємо нове доміно\n";
                                 insertPair(usedDominoes, myMinMax(grid[r][c], *possible_values.begin()));
                                 pair<int, int> domino = myMinMax(grid[r][c], *possible_values.begin());
                                 if (usedDominoes.count(domino)) {
                                     cout << "✅ Доміно добавлено: {" << domino.first << ", " << domino.second << "} // Доміно вже у списку використаних\n";
                                 }
                                 changed = true;
                             }
                         }
                     }
                 }
             }
         }
         pair<int, int> next_missing = findNextMissing();
         int row = next_missing.first;
         int col = next_missing.second;
         if (row == -1) {
             if (isSolution()) {
                 cout << "✅ Рішення знайдено!\n";
                 cout << "✅ Кількість використаних доміно: " << usedDominoes.size() << " (очікується: " << TOTAL_DOMINOES << ")\n";
                 for (const auto& d : usedDominoes) {
                     cout << "Доміно: {" << d.first << "," << d.second << "}\n";
                 }
                 return true;
             }
             return isSolution();
         }
         if (!canLeadToSolution()) {
             return false;
         }
         for (int value = MIN_VALUE; value <= MAX_VALUE; value++) {
             if (isValid(row, col, value)) {
                 vector<vector<int>> temp_grid = grid;
                 temp_grid[row][col] = value;
                 set<pair<int, int>> new_used_dominoes = usedDominoes;
                 bool possible = true;
                 int dr[4] = {-1, 1, 0, 0};
                 int dc[4] = {0, 0, -1, 1};
                 for (int i = 0; i < 4; i++) {
                     int nr = row + dr[i];
                     int nc = col + dc[i];
                     if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[nr].size() &&
                         temp_grid[nr][nc] >= MIN_VALUE && temp_grid[nr][nc] <= MAX_VALUE) {
                         pair<int, int> domino = myMinMax(value, temp_grid[nr][nc]);
                         if (allDominoes.count(domino) && usedDominoes.count(domino)) {
                             possible = false;
                             break;
                         } else if (allDominoes.count(domino)) {
                             cout << "➕ Додається доміно: {" << value << ", " << temp_grid[nr][nc] << "} // Додаємо доміно для цієї пари\n";
                             insertPair(new_used_dominoes, domino);
                             if (new_used_dominoes.count(domino)) {
                                 cout << "✅ Доміно добавлено: {" << domino.first << ", " << domino.second << "} // Доміно тепер у списку використаних\n";
                             }
                         }
                     }
                 }
                 if (possible) {
                     grid[row][col] = value;
                     autoInserted.insert({row, col});
                     cout << "Спроба вставити " << value << " у клітинку (" << row << ", " << col << ") // Показує, що ми намагаємось поставити це число\n";
                     set<pair<int, int>> backup_used = usedDominoes;
                     set<pair<int, int>> backup_autoInserted = autoInserted;
                     usedDominoes = new_used_dominoes;
                     if (solvePuzzle()) {
                         cout << "✅ Вдалось вставити " << value << " у (" << row << ", " << col << ") // Число успішно вставлено\n";
                         return true;
                     }
                     cout << "↩️ Відкат вставки " << value << " з клітинки (" << row << ", " << col << ") // Відміняємо цю спробу\n";
                     grid[row][col] = MISSING;
                     usedDominoes = backup_used;
                     autoInserted = backup_autoInserted;
                 }
             }
         }
         return false;
     }
 public:
 /* ---------------------------------------------------------------------[<]-
       Конструктор. Приймає початкове поле.
  ---------------------------------------------------------------------[>]-*/
     DominoSolver(const vector<vector<int>>& initial) : grid(initial) {
         for (int i = MIN_VALUE; i <= MAX_VALUE; ++i) {
             for (int j = i; j <= MAX_VALUE; ++j) {
                 allDominoes.insert({i, j});
             }
         }
     }
 /* ---------------------------------------------------------------------[<]-
       Запускає вирішення та виводить результат у вказаний потік.
  ---------------------------------------------------------------------[>]-*/
    bool solve(ostream& os, ostream& of) { 
        bool found = solvePuzzle();
        if (found) {
            os << "\n🎉 Рішення знайдено!\n";
            printGrid(os); 
            of << "\n🎉 Рішення знайдено!\n"; 
            printGridToFile(of);
            os << "\n🔍 Перевірка використаних доміно...\n";
            os << "✅ Кількість використаних доміно: " << usedDominoes.size() << " (очікується: " << TOTAL_DOMINOES << ")\n";
            for (const auto& d : usedDominoes) {
                os << "Доміно: {" << d.first << "," << d.second << "}\n";
                of << "Доміно: {" << d.first << "," << d.second << "}\n";
            }
        } else {
            os << "❌ Не знайдено рішення.\n";
            of << "❌ Не знайдено рішення.\n";
            printGrid(os);
            printGridToFile(of);
        }
        return found;
    }
 };
 
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
 }
 
 /* ---------------------------------------------------------------------[<]-
  Функція: main
  Опис: Основна функція: меню користувача, введення та запуск розв’язання.
  ---------------------------------------------------------------------[>]-*/
 int main() {
     printHeader();
     cin.ignore();
     ofstream output("output.txt"); 
     if (!output.is_open()) {
         cerr << "Помилка відкриття файлу output.txt для запису!\n";
         return 1;
     }
     vector<vector<int>> initial_grid = {
         {3, 6, EMPTY, EMPTY, EMPTY, 0, 1, 1, 3, 0, 1, 5, 5},
         {6, 2, EMPTY, MISSING, EMPTY, 0, 4, 5, MISSING, 6, 1, 6, 2},
         {0, 4, EMPTY, 0, EMPTY, 6, 5, 6, 1, MISSING, 3, EMPTY, EMPTY},
         {0, 2, 3, 5, 2, 2, 3, 3, EMPTY, EMPTY, MISSING, EMPTY, EMPTY},
         {6, 4, 2, 4, 4, 3, 5, 2, EMPTY, EMPTY, 5, 0, 1},
         {EMPTY, EMPTY, 1, MISSING, 4, EMPTY, EMPTY, MISSING, EMPTY, EMPTY, 1, 4, 2}
     };
     vector<vector<int>> grid;
     int choice = 0;
     while (true) {
         cout << "\n==================== Меню ====================\n";
         cout << "1 - Ввести самостійно\n";
         cout << "2 - Введені програмою значення\n";
         cout << "3 - Вихід\n";
         cout << "=============================================\n";
         cout << "Виберіть (1, 2, або 3): ";
         cin >> choice;
         if (cin.fail()) {
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
             cout << "❌ Невірний вибір. Спробуйте ще раз.\n";
             continue;
         }
         if (choice == 1) {
             grid.clear();
             int rows, cols;
             cout << "Введіть кількість рядків: ";
             cin >> rows;
             cout << "Введіть кількість стовпців: ";
             cin >> cols;
             if (cin.fail() || rows <= 0 || cols <= 0 || rows * cols > MAX_GRID_CELLS) {
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 cout << "❌ Недопустимий розмір поля. Спробуйте знову.\n";
                 continue;
             }
             grid.resize(rows, vector<int>(cols, MISSING));
             cout << "Введіть значення поля (від " << MIN_VALUE << " до " << MAX_VALUE << ", " << MISSING << " для втрачених, " << EMPTY << " для пустих):\n";
             for (int r = 0; r < rows; ++r)
                 for (int c = 0; c < cols; ++c) {
                     int val;
                     while (true) {
                         cout << "Клітинка [" << r << "][" << c << "]: ";
                         cin >> val;
                         if (cin.fail() || val < EMPTY || val > MAX_VALUE) {
                             cin.clear();
                             cin.ignore(numeric_limits<streamsize>::max(), '\n');
                             cout << "❌ Невірне значення. Введіть число від " << EMPTY << " до " << MAX_VALUE << ".\n";
                         } else {
                             grid[r][c] = val;
                             break;
                         }
                     }
                 }
             cout << "\nПочаток вирішення...\n";
             DominoSolver solver(grid);
             solver.solve(cout, output); 
         } else if (choice == 2) {
             cout << "Початок вирішення...\n";
             DominoSolver solver(initial_grid);
             solver.solve(cout, output);
         } else if (choice == 3) {
             cout << "Вихід з програми...\n";
             break;
         } else {
             cout << "❌ Невірний вибір. Спробуйте ще раз.\n";
         }
     }
     output.close(); 
     return 0;
 }
 