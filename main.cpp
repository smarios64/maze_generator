#include <vector>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>

#define MAZE_WIDTH 16
#define MAZE_HEIGHT 9
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) >= 0 ? (a) : -(a))
#define WALL_CHAR "#"
#define SPACE_CHAR " "

struct MazeCell
{
    int x, y;
    bool visited;
    std::vector<MazeCell*> neighbors;
};

// use the even y indexes for the walls that separate the cells horizontally
// and the odd y indexes for the walls that separate the cells vertically
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
        // reset maze cells
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            for (int x = 0; x < MAZE_WIDTH; ++x) {
                cells[y][x].visited = false;
            }
        }

        // reset maze walls
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
            // For the y index of the wall, get the minimum y index between the examined cells,
            // times 2 and add the difference of their indexes. That way, if they are on
            // the same y index, their difference will be 0, so wall that will be removed
            // will be on the even indexes, otherwise it would be on the odd ones.
            
            // For the x index of the wall, just get the maximum x index between the examined cells.
            // Keep in mind when processing the walls, the 0 x index should be ignored as the
            // implementation is now. That is because the walls that separate the cells horizontally
            // are one less than the walls that separate the cells vertically.
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
                // print the outer walls
                printf(WALL_CHAR);
            }
            else if (x % 2 == 1 || y % 2 == 1) {
                if (x % 2 == 1 && y % 2 == 1) {
                    // print the cells
                    printf(SPACE_CHAR);
                }
                else {
                    // process the walls array and print a wall wherever there is one,
                    // otherwize print a whitespace
                    if (walls[y - 1][x / 2]) {
                        printf(WALL_CHAR);
                    }
                    else {
                        printf(SPACE_CHAR);
                    }
                }
            }
            else {
                // print the inner dummy walls
                printf(WALL_CHAR);
            }
        }
        printf("\n");
    }
}