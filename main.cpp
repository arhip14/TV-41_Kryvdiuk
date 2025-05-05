#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const int ROWS = 6;
const int COLS = 13;

int grid[ROWS][COLS] = {
    {3, 6, -2, -2, -2, 0, 1, 1, 3, 0, 1, 5, 5},
    {6, 2, -2, -1, -2, 0, 4, 5, -1, 6, 1, 6, 2},
    {0, 4, -2, 0, -2, 6, 5, 6, 1, -1, 3, -2, -2},
    {0, 2, 3, 5, 2, 2, 3, 3, -2, -2, -1, -2, -2},
    {6, 4, 2, 4, 4, 3, 5, 2, -2, -2, 5, 0, 1},
    {-2, -2, 1, -1, 4, -2, -2, -1, -2, -2, 1, 4, 2}
};

bool usedCell[ROWS][COLS] = {false};
bool usedDominoes[7][7] = {false};

int solutions = 0;

bool isValid(int row, int col) {
    bool inBounds = (row >= 0 && row < ROWS && col >= 0 && col < COLS);
    bool isNotBlocked = (grid[row][col] != -2);
    bool isNotUsed = (!usedCell[row][col]);
    return inBounds && isNotBlocked && isNotUsed;
}

void printGrid() {
    cout << "    ";
    for (int c = 0; c < COLS; ++c)
        cout << setw(3) << c;
    cout << "\n";
    cout << "    ┌";
    for (int c = 0; c < COLS; ++c) cout << "───";
    cout << "┐\n";

    for (int r = 0; r < ROWS; ++r) {
        cout << setw(3) << r << " │";
        for (int c = 0; c < COLS; ++c) {
            if (grid[r][c] == -2)
                cout << "   "; 
            else if (grid[r][c] == -1)
                cout << " . "; 
            else
                cout << " " << grid[r][c] << " ";
        }
        cout << "│\n";
    }

    cout << "    └";
    for (int c = 0; c < COLS; ++c) cout << "───";
    cout << "┘\n\n";
}

void solve() {
    bool placed = false;

    for (int row = 0; row < ROWS && !placed; ++row) {
        for (int col = 0; col < COLS && !placed; ++col) {

            if (!isValid(row, col)) continue;

            vector<pair<int, int> > directions;
            directions.push_back(make_pair(0, 1));
            directions.push_back(make_pair(1, 0));

            for (int d = 0; d < directions.size(); ++d) {
                int deltaRow = directions[d].first;
                int deltaCol = directions[d].second;

                int nextRow = row + deltaRow;
                int nextCol = col + deltaCol;

                if (!isValid(nextRow, nextCol)) continue;

                int value1 = grid[row][col];
                int value2 = grid[nextRow][nextCol];

                vector<pair<int, int> > possibleDominoes;

                if (value1 != -1 && value2 != -1) {
                    int minVal = min(value1, value2);
                    int maxVal = max(value1, value2);
                    possibleDominoes.push_back(make_pair(minVal, maxVal));
                }
                else if (value1 == -1 && value2 == -1) {
                    for (int i = 0; i <= 6; ++i)
                        for (int j = i; j <= 6; ++j)
                            possibleDominoes.push_back(make_pair(i, j));
                }
                else {
                    int known = (value1 != -1) ? value1 : value2;
                    for (int i = 0; i <= 6; ++i) {
                        int minVal = min(known, i);
                        int maxVal = max(known, i);
                        possibleDominoes.push_back(make_pair(minVal, maxVal));
                    }
                }

                for (int i = 0; i < possibleDominoes.size(); ++i) {
                    int a = possibleDominoes[i].first;
                    int b = possibleDominoes[i].second;

                    if (usedDominoes[a][b]) continue;

                    int prev1 = value1;
                    int prev2 = value2;

                    if (value1 == -1) {
                        grid[row][col] = (value2 == -1 || value2 == b) ? a : b;
                    }

                    if (value2 == -1) {
                        grid[nextRow][nextCol] = (grid[row][col] == a) ? b : a;
                    }

                    usedCell[row][col] = true;
                    usedCell[nextRow][nextCol] = true;

                    int valA = grid[row][col];
                    int valB = grid[nextRow][nextCol];
                    int minVal = min(valA, valB);
                    int maxVal = max(valA, valB);

                    usedDominoes[minVal][maxVal] = true;

                    solve();

                    grid[row][col] = prev1;
                    grid[nextRow][nextCol] = prev2;
                    usedCell[row][col] = false;
                    usedCell[nextRow][nextCol] = false;
                    usedDominoes[minVal][maxVal] = false;
                }

                placed = true;
            }
        }
    }

    if (!placed) {
        cout << "═════════════════════════════════════════════════════════════\n";
        cout << "                    🟢 ЗНАЙДЕНО РІШЕННЯ #" << ++solutions << ":\n";
        printGrid();
        int usedCount = 0;
        for (int i = 0; i <= 6; ++i)
            for (int j = i; j <= 6; ++j)
                if (usedDominoes[i][j]) ++usedCount;
        cout << "   🔢 Кількість використаних доміно: " << usedCount << "\n";
        cout << "═════════════════════════════════════════════════════════════\n\n";
    }
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
    solve();
    if (solutions == 0) {
        cout << "❌ Рішень не знайдено.\n";
    } else {
        cout << "✅ Загальна кількість рішень: " << solutions << "\n";
    }
    return 0;
}