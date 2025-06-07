#pragma once
#include <vector>
#include <cstdint>
#include "config.hpp"

class Grid {
private:
	inline static int grid_width = config::GRID_WIDTH_DEFAULT;
	inline static int grid_height = config::GRID_HEIGHT_DEFAULT;
	inline static int game_speed = config::SPEED_DEFAULT;

	static std::vector<uint32_t> current_grid;
	static std::vector<uint32_t> nextgen_grid;

public:
	inline static constexpr uint32_t dead = 0xFF262222;
	inline static constexpr uint32_t alive = 0xFFFFFFFF;
	static int step_interval_ms;

	static int getWidth();
	static int getHeight();
	static int getSpeed();

	static void setWidth(int arg);
	static void setHeight(int arg);
	static void setSpeed(int arg);

	static void init();
	static const std::vector<uint32_t>& getCurrentGrid();
	static void switchCell(int index);
	static int isCellAlive(int index);
	static void step();
};