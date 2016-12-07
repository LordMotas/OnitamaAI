#include "BoardSpace.h"

BoardSpace::BoardSpace(Piece spacePiece, int onmyo)
{
	piece = spacePiece;
	isOnmyoSpace = onmyo;
}

BoardSpace::BoardSpace(Piece spacePiece)
{
	piece = spacePiece;
	isOnmyoSpace = 0;
}