#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

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

ofstream outFile("solutions.txt");

bool isValid(int row, int col) {
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS && grid[row][col] != -2 && !usedCell[row][col]);
}

void printGrid(ostream &out) {
    out << "    ";
    for (int c = 0; c < COLS; ++c)
        out << setw(3) << c;
    out << "\n    ┌";
    for (int c = 0; c < COLS; ++c)
        out << "───";
    out << "┐\n";

    for (int r = 0; r < ROWS; ++r) {
        out << setw(3) << r << " │";
        for (int c = 0; c < COLS; ++c) {
            if (grid[r][c] == -2)
                out << "   ";
            else if (grid[r][c] == -1)
                out << " . ";
            else
                out << " " << grid[r][c] << " ";
        }
        out << "│\n";
    }

    out << "    └";
    for (int c = 0; c < COLS; ++c)
        out << "───";
    out << "┘\n\n";
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
                int dirRow = directions[d].first;
                int dirCol = directions[d].second;

                int nextRow = row + dirRow;
                int nextCol = col + dirCol;

                if (!isValid(nextRow, nextCol)) continue;

                int value1 = grid[row][col];
                int value2 = grid[nextRow][nextCol];

                vector<pair<int, int> > possibleDominoes;

                if (value1 != -1 && value2 != -1) {
                    int a = min(value1, value2);
                    int b = max(value1, value2);
                    possibleDominoes.push_back(make_pair(a, b));
                } else if (value1 == -1 && value2 == -1) {
                    for (int i = 0; i <= 6; ++i)
                        for (int j = i; j <= 6; ++j)
                            possibleDominoes.push_back(make_pair(i, j));
                } else {
                    int known = (value1 != -1) ? value1 : value2;
                    for (int i = 0; i <= 6; ++i) {
                        int a = min(known, i);
                        int b = max(known, i);
                        possibleDominoes.push_back(make_pair(a, b));
                    }
                }

                for (int k = 0; k < possibleDominoes.size(); ++k) {
                    int a = possibleDominoes[k].first;
                    int b = possibleDominoes[k].second;

                    if (usedDominoes[a][b]) continue;

                    int prev1 = value1;
                    int prev2 = value2;

                    if (value1 == -1)
                        grid[row][col] = (value2 == -1 || value2 == b) ? a : b;

                    if (value2 == -1)
                        grid[nextRow][nextCol] = (grid[row][col] == a) ? b : a;

                    usedCell[row][col] = true;
                    usedCell[nextRow][nextCol] = true;
                    usedDominoes[a][b] = true;

                    solve();

                    grid[row][col] = prev1;
                    grid[nextRow][nextCol] = prev2;
                    usedCell[row][col] = false;
                    usedCell[nextRow][nextCol] = false;
                    usedDominoes[a][b] = false;
                }

                placed = true;
            }
        }
    }

    if (!placed) {
        ++solutions;
        cout << "═════════════════════════════════════════════════════════════\n";
        cout << "                    🟢 ЗНАЙДЕНО РІШЕННЯ #" << solutions << ":\n";
        printGrid(cout);
        int usedCount = 0;
        for (int i = 0; i <= 6; ++i)
            for (int j = i; j <= 6; ++j)
                if (usedDominoes[i][j]) ++usedCount;
        cout << "   🔢 Кількість використаних доміно: " << usedCount << "\n";
        cout << "═════════════════════════════════════════════════════════════\n\n";

        outFile << "═════════════════════════════════════════════════════════════\n";
        outFile << "                    🟢 ЗНАЙДЕНО РІШЕННЯ #" << solutions << ":\n";
        printGrid(outFile);
        outFile << "   🔢 Кількість використаних доміно: " << usedCount << "\n";
        outFile << "═════════════════════════════════════════════════════════════\n\n";
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
    if (solutions == 0)
        cout << "❌ Рішень не знайдено.\n";
    else
        cout << "✅ Загальна кількість рішень: " << solutions << "\n";

    outFile << "✅ Загальна кількість рішень: " << solutions << "\n";
    outFile.close();
    return 0;
}