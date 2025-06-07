#include "../hdr/grid.hpp"

void Grid::setWidth(int arg) {grid_width = arg;}
void Grid::setHeight(int arg) {grid_height = arg;}
void Grid::setSpeed(int arg) {game_speed = arg;}

int Grid::getWidth() {return grid_width;}
int Grid::getHeight() {return grid_height;}
int Grid::getSpeed() {return game_speed;}

std::vector<uint32_t> Grid::current_grid;
std::vector<uint32_t> Grid::nextgen_grid;
int Grid::step_interval_ms;


void Grid::init() {
	current_grid = std::vector<uint32_t>((grid_width + 2) * (grid_height + 2), dead);
	nextgen_grid = std::vector<uint32_t>((grid_width + 2) * (grid_height + 2), dead);

	step_interval_ms = 500 / game_speed;
}

const std::vector<uint32_t>& Grid::getCurrentGrid() { return current_grid; }

void Grid::switchCell(int index) {
	if (current_grid[index] == dead) current_grid[index] = alive;
	else current_grid[index] = dead;
}

int Grid::isCellAlive(int index) {
	if (current_grid[index] == alive) return 1;
	return 0;
}

void Grid::step() {
	for (int y = 1; y <= grid_height; ++y) {
		for (int x = 1; x <= grid_width; ++x) {
		
			int w = grid_width + 2;
			int neighbors = 0;

			neighbors += isCellAlive((y - 1) * w + (x - 1));
			neighbors += isCellAlive((y - 1) * w + (x));
			neighbors += isCellAlive((y - 1) * w + (x + 1));
			neighbors += isCellAlive((y) * w + (x - 1));
			neighbors += isCellAlive((y) * w + (x + 1));
			neighbors += isCellAlive((y + 1) * w + (x - 1));
			neighbors += isCellAlive((y + 1) * w + (x));
			neighbors += isCellAlive((y + 1) * w + (x+1));

			if (isCellAlive(y * w + x) == 0) {
				if (neighbors == 3) nextgen_grid[y * w + x] = alive;
			}
			else {
				if (neighbors == 2 or neighbors == 3) nextgen_grid[y * w + x] = alive;
				else nextgen_grid[y * w + x] = dead;
			}
		}
	}	
	std::swap(current_grid, nextgen_grid);
}