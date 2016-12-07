#pragma once
#include <string>
#include <vector>

class Card {
public:
	std::string name;
	std::string firstPlayer;
	std::vector<std::pair<int, int>> coordinatesVector;
	Card(std::string, std::string, std::vector<std::pair<int,int>>);
	Card();
};