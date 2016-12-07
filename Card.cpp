#include "Card.h"

Card::Card(std::string cardName, std::string playerColor, std::vector<std::pair<int, int>> coordinatesList)
{
	name = cardName;
	firstPlayer = playerColor;
	coordinatesVector = coordinatesList;
}

Card::Card()
{
	name = "";
	firstPlayer = "";
	coordinatesVector = { std::pair<int,int>(0,0) };
}
