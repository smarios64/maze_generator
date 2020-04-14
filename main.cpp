#include <vector>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>

#define MAZE_WIDTH 16
#define MAZE_HEIGHT 9
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) >= 0 ? (a) : -(a))

struct MazeCell
{
    int x, y;
    bool visited;
    std::vector<MazeCell*> neighbors;
};

static bool walls[MAZE_HEIGHT * 2 - 1][MAZE_WIDTH];
static void generateMaze(MazeCell *cell);
static void printMaze();

int main(int argc, char **argv)
{
    MazeCell cells[MAZE_HEIGHT][MAZE_WIDTH];

    srand(time(0));

    // initialize maze cells
    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            cells[y][x].x = x;
            cells[y][x].y = y;
            if (y > 0) {
                cells[y - 1][x].neighbors.push_back(&cells[y][x]);
                cells[y][x].neighbors.push_back(&cells[y - 1][x]);
            }
            if (y < MAZE_HEIGHT - 1) {
                cells[y + 1][x].neighbors.push_back(&cells[y][x]);
                cells[y][x].neighbors.push_back(&cells[y + 1][x]);
            }
            if (x > 0) {
                cells[y][x - 1].neighbors.push_back(&cells[y][x]);
                cells[y][x].neighbors.push_back(&cells[y][x - 1]);
            }
            if (x < MAZE_WIDTH - 1) {
                cells[y][x + 1].neighbors.push_back(&cells[y][x]);
                cells[y][x].neighbors.push_back(&cells[y][x + 1]);
            }
        }
    }

    while (true) {
        // reset maze cells and walls
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            for (int x = 0; x < MAZE_WIDTH; ++x) {
                cells[y][x].visited = false;
            }
        }
        for (int y = 0; y < MAZE_HEIGHT * 2 - 1; ++y) {
            for (int x = 0; x < MAZE_WIDTH; ++x) {
                walls[y][x] = true;
            }
        }

        generateMaze(&cells[rand() % MAZE_HEIGHT][rand() % MAZE_WIDTH]);
        printMaze();
        
        if (getchar() == 'q') {
            break;
        }
    }

    return 0;
}

static void generateMaze(MazeCell *cell)
{
    std::vector<MazeCell*> neighbors = cell->neighbors;
    cell->visited = true;
    while (neighbors.size() > 0) {
        int i = rand() % neighbors.size();
        if (!neighbors[i]->visited) {
            walls[MIN(cell->y, neighbors[i]->y) * 2 + ABS(cell->y - neighbors[i]->y)][MAX(cell->x, neighbors[i]->x)] = false;
            generateMaze(neighbors[i]);
        }
        neighbors.erase(neighbors.begin() + i);
    }
}

static void printMaze()
{
    for (int y = 0; y <= MAZE_HEIGHT * 2; ++y) {
        for (int x = 0; x <= MAZE_WIDTH * 2; ++x) {
            if (x == 0 || y == 0 || x == MAZE_WIDTH * 2 || y == MAZE_HEIGHT * 2) {
                printf("#");
            }
            else if (x % 2 == 1 || y % 2 == 1) {
                if (x % 2 == 1 && y % 2 == 1) {
                    printf(" ");
                }
                else {
                    if (walls[y - 1][x / 2]) {
                        printf("#");
                    }
                    else {
                        printf(" ");
                    }
                }
            }
            else {
                printf("#");
            }
        }
        printf("\n");
    }
}