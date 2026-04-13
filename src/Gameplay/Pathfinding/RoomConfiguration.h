#pragma once
#include <vector>

class Tile;

class RoomConfiguration
{
public:
	static std::vector<std::vector<std::vector<char>>> Type1();
	static std::vector<std::vector<std::vector<char>>> Type2();
	static std::vector<std::vector<std::vector<char>>> Type3();
	static std::vector<std::vector<std::vector<char>>> Type4();
	static std::vector<std::vector<std::vector<char>>> Type5();
	static std::vector<std::vector<std::vector<char>>> Type6();

};

