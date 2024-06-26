#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

class Maze {
public:
    Maze(int width, int height);
    void generate();
    void print();
    bool solve();
    void printSolution();

private:
    struct Cell {
        bool visited = false;
        bool top = true;
        bool bottom = true;
        bool left = true;
        bool right = true;
        bool inSolution = false;
    };

    int width;
    int height;
    std::vector<std::vector<Cell>> grid;

    void removeWall(Cell& current, Cell& next, char direction);
    bool solveUtil(int x, int y);
    void resetVisited();
};

Maze::Maze(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<Cell>(width));
    srand(time(nullptr));
}

void Maze::generate() {
    std::stack<std::pair<int, int>> stack;
    int startX = 0;
    int startY = 0;
    grid[startY][startX].visited = true;
    stack.push({startX, startY});

    while (!stack.empty()) {
        int x = stack.top().first;
        int y = stack.top().second;

        std::vector<std::pair<int, int>> neighbors;

        if (x > 0 && !grid[y][x - 1].visited) neighbors.push_back({x - 1, y}); // izquierda
        if (x < width - 1 && !grid[y][x + 1].visited) neighbors.push_back({x + 1, y}); // derecha
        if (y > 0 && !grid[y - 1][x].visited) neighbors.push_back({x, y - 1}); // arriba
        if (y < height - 1 && !grid[y + 1][x].visited) neighbors.push_back({x, y + 1}); // abajo

        if (!neighbors.empty()) {
            auto [nextX, nextY] = neighbors[rand() % neighbors.size()];

            if (nextX == x - 1) removeWall(grid[y][x], grid[nextY][nextX], 'L');
            else if (nextX == x + 1) removeWall(grid[y][x], grid[nextY][nextX], 'R');
            else if (nextY == y - 1) removeWall(grid[y][x], grid[nextY][nextX], 'U');
            else if (nextY == y + 1) removeWall(grid[y][x], grid[nextY][nextX], 'D');

            grid[nextY][nextX].visited = true;
            stack.push({nextX, nextY});
        } else {
            stack.pop();
        }
    }

    // Asegurar entrada y salida
    grid[0][0].left = false;  // Entrada
    grid[height - 1][width - 1].right = false;  // Salida

    resetVisited();  // Resetear el estado de visitado antes de resolver el laberinto
}

void Maze::removeWall(Cell& current, Cell& next, char direction) {
    switch (direction) {
        case 'L':
            current.left = false;
            next.right = false;
            break;
        case 'R':
            current.right = false;
            next.left = false;
            break;
        case 'U':
            current.top = false;
            next.bottom = false;
            break;
        case 'D':
            current.bottom = false;
            next.top = false;
            break;
    }
}

bool Maze::solve() {
    return solveUtil(0, 0);
}

bool Maze::solveUtil(int x, int y) {
    if (x == width - 1 && y == height - 1) {
        grid[y][x].inSolution = true;
        return true;
    }

    if (x >= 0 && x < width && y >= 0 && y < height && !grid[y][x].visited) {
        grid[y][x].visited = true;

        if (!grid[y][x].top && solveUtil(x, y - 1)) {
            grid[y][x].inSolution = true;
            return true;
        }
        if (!grid[y][x].right && solveUtil(x + 1, y)) {
            grid[y][x].inSolution = true;
            return true;
        }
        if (!grid[y][x].bottom && solveUtil(x, y + 1)) {
            grid[y][x].inSolution = true;
            return true;
        }
        if (!grid[y][x].left && solveUtil(x - 1, y)) {
            grid[y][x].inSolution = true;
            return true;
        }
    }
    return false;
}

void Maze::resetVisited() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x].visited = false;
        }
    }
}

void Maze::printSolution() {
    for (int y = 0; y < height; ++y) {
        // Print top walls
        for (int x = 0; x < width; ++x) {
            std::cout << (grid[y][x].top ? "+---" : "+   ");
        }
        std::cout << "+\n";

        // Print left walls and cells
        for (int x = 0; x < width; ++x) {
            if (grid[y][x].left)
                std::cout << "|";
            else
                std::cout << " ";
            if (grid[y][x].inSolution)
                std::cout << " * ";
            else
                std::cout << "   ";
        }
        if(y == height - 1){ 
        std::cout << "\n";
        }else{
        std::cout << "|\n";
        }
    }
    

    // Print bottom walls
    for (int x = 0; x < width; ++x) {
        std::cout << "+---";
    }
    std::cout << "+\n";
}

void Maze::print() {
    for (int y = 0; y < height; ++y) {
        // Print top walls
        for (int x = 0; x < width; ++x) {
            std::cout << (grid[y][x].top ? "+---" : "+   ");
        }
        std::cout << "+\n";

        // Print left walls and cells
        for (int x = 0; x < width; ++x) {
            std::cout << (grid[y][x].left ? "|   " : "    ");
        }
        if(y == height - 1){ 
        std::cout << "\n";
        }else{
        std::cout << "|\n";
        }
    }

    // Print bottom walls
    for (int x = 0; x < width; ++x) {
        std::cout << "+---";
    }
    std::cout << "+\n";
}

int main() {
    int width;
    int height;
    std::cout << "Digite el ancho del laberinto: ";
    std::cin >> width;
    std::cout << "Digite el alto del laberinto: ";
    std::cin >> height;

    Maze maze(width, height);
    maze.generate();
    maze.print();

    if (maze.solve()) {
        std::cout << "\nLa solucion es:\n";
        maze.printSolution();
    } else {
        std::cout << "\nNo se encontro la solucion\n";
    }

    return 0;
}
