#include <queue>

using namespace std;

#include "helper.h"
#include "board.h"

void Board::setDimensions(int height, int width)
{
    if(height != this->height || width != this->width)
    {
        this->height = height;
        this->width = width;
    }
    board = vector<vector<Square>>(height, vector<Square>(width));
}

void Board::setSquare(int y, int x, const Square& square)
{
    board[y][x] = square;
}

Square Board::getSquare(int y, int x) const
{
    return board[y][x];
}

Move Board::moveFromTo(const Coord& from, const Coord& to) const
{
    if (from.y < 0 || to.y < 0)
        return NO_MOVE;
    else if (to.y > from.y)
        return UP;
    else if (to.y < from.y)
        return DOWN;
    else if (to.x > from.x)
        return RIGHT;
    else if (to.x < from.x)
        return LEFT;
    else
        return NO_MOVE;
}

// Use bread first search to search for shortest path from head to nearest square of target type
Move Board::getShortestPathToSquareType(const Coord& head, SQUARE targetType) const
{
    vector<vector<Coord>> visit(height, vector<Coord>(width, Coord(-1,-1)));
    Move move = NO_MOVE;
    queue<Coord> q;
    Coord pos(-1, -1);

    q.push(head);
    bool cont = true;
    while(cont && !q.empty())
    {
        pos = q.front();

	vector<Coord> childs;

	if (pos.y > 0)
	{
            childs.push_back({ pos.y-1, pos.x });
	}


	if (pos.y < height-1)
	{
            childs.push_back({ pos.y+1, pos.x });
	}

	if (pos.x > 0)
	{
            childs.push_back({ pos.y, pos.x-1 });
	}

	if (pos.x < width-1)
	{
            childs.push_back({ pos.y, pos.x+1 });
	}

	for(const Coord& next : childs)
	{
            if (board[next.y][next.x].square == targetType)
	    {
                visit[next.y][next.x] = pos;
		pos = next;
		cont = false;
                break;
	    }
	    else if (board[next.y][next.x].square != SNAKE)
	    {
	        if (visit[next.y][next.x].y < 0)
		{
                    visit[next.y][next.x] = pos;
		    q.push(next);
		}
	    }
	}

        q.pop();
    }

    Coord lastPos = pos;

    if(!q.empty())
    {
        Coord next = visit[pos.y][pos.x];
	while(next.y != head.y || next.x != head.x)
	{
		pos = next;
                next = visit[pos.y][pos.x];
	}
	move = moveFromTo(head, pos);

        if (DEBUG && VERBOSE)
        {
            cout << "PATH: start " << head.y << "/" << head.x << " target=" << targetType << endl;
	    for(int y = height-1; y >= 0; --y)
            {
                for(int x = 0; x < width; ++x)
    	        {
		    if (y == head.y && x == head.x)
                        cout << "*";
		    else if (y == lastPos.y && x == lastPos.x)
                        cout << "!";
		    else if (board[y][x].square == targetType)
                        cout << "#";
		    else
		        cout << moveFromTo(visit[y][x], {y, x});
	        }
	        cout << endl;
	    }
	    cout << "result: " << move << endl;
        }
    }

    return move;
}

vector<Move> Board::generateLongLivingMoves(const Coord& head, int health) const
{
    vector<Move> moves = generateSafeMoves(head, true);

    if (health <= height + width + FOOD_GREEDINESS)
    {
        Move move = getShortestPathToSquareType(head, FOOD);
	if(move != NO_MOVE)
	    moves = { move };
    }

    return moves;
}

vector<Move> Board::generateGreedySafeMoves(const Coord& head) const
{
    vector<Move> moves = generateSafeMoves(head, true);

    if(moves.empty())
    {
        moves = generateSafeMoves(head);
    }

    return moves;
}

vector<Move> Board::generateSafeMoves(const Coord& head, bool onlyFood) const
{
    vector<Move> moves;

    if (head.y > 0 && (   (!onlyFood && getSquare(head.y-1, head.x).square == FREE)
                       || getSquare(head.y-1, head.x).square == FOOD))
        moves.push_back(DOWN);

    else if (head.y < height-1 && (   (!onlyFood && getSquare(head.y+1, head.x).square == FREE)
                                   || getSquare(head.y+1, head.x).square == FOOD))
        moves.push_back(UP);

    else if (head.x <= width / 2) 
    {
    	if (head.x < width-1 && (   (!onlyFood && getSquare(head.y, head.x+1).square == FREE)
                                 || getSquare(head.y, head.x+1).square == FOOD))
             moves.push_back(RIGHT);

        else if (head.x > 0 && (   (!onlyFood && getSquare(head.y, head.x-1).square == FREE)
                                || getSquare(head.y, head.x-1).square == FOOD))
             moves.push_back(LEFT);
    }
    else
    {
    	if (head.x > 0 && (   (!onlyFood && getSquare(head.y, head.x-1).square == FREE)
                           || getSquare(head.y, head.x-1).square == FOOD))
             moves.push_back(LEFT);

        else if (head.x < width-1 && (   (!onlyFood && getSquare(head.y, head.x+1).square == FREE)
                                      || getSquare(head.y, head.x+1).square == FOOD))
             moves.push_back(RIGHT);
    }

    return moves;
}

