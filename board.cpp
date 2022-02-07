#include <queue>
#include <set>

using namespace std;

#include "helper.h"
#include "board.h"

void Board::setDimensions(int height, int width)
{
    if(height != this->height || width != this->width)
    {
        this->height = height;
        this->width = width;

        if (height > 0 && width > 0)
        {
            moveField = vector<vector<Move>>(height, vector<Move>(width, UP));
            initMoveField(moveField);
        }
    }

    if (height > 0 && width > 0)
        board = vector<vector<Square>>(height, vector<Square>(width));
}

void Board::initMoveField(vector<vector<Move>> &moveField)
{
   maxSnakeLength = width * height;

   for(int y = 0; y < height-1; ++y)
       moveField[y][0] = UP;

   moveField[height-1][0] = RIGHT;

   for(int y = height-1; y > 0; y -= 2)
   {
       for(int x = 1; x < width-1; ++x)
           moveField[y][x] = RIGHT;
       moveField[y][width-1] = DOWN;

       for(int x = width-1; x > 1; --x)
           moveField[y-1][x] = LEFT;
       moveField[y-1][1] = DOWN;
   }

   if (height % 2 == 0)
   {
       moveField[0][1] = LEFT;
   }
   else
   {
       for(int x = width-1; x > 0; x -= 2)
       {
           moveField[1][x] = DOWN;
           moveField[0][x] = LEFT;
	   if(x > 1)
	   {
               moveField[0][x-1] = UP;
               moveField[1][x-1] = LEFT;
	   }
       }

       if (width % 2 == 1) // one square (1/1) will not be visited by the snake
       {
           maxSnakeLength--;
           moveField[0][1] = LEFT;
       }
   }


   if(DEBUG && VERBOSE)
   {
        cout << "MoveField:" << endl;
        for(int y = height-1; y >= 0; --y)
        {
            for(int x = 0; x < width; ++x)
            {
                cout << moveField[y][x];
            }
            cout << endl;
        }
   }
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

Coord Board::moveFrom(const Coord& from, Move move) const
{
    switch(move)
    {
        case UP:
            return Coord(from.y+1, from.x);
        case DOWN:
            return Coord(from.y-1, from.x);
        case LEFT:
            return Coord(from.y, from.x-1);
        case RIGHT:
            return Coord(from.y, from.x+1);
        default:
	    break;
    }
    return from;
}

// Use bread first search to search for shortest path from head to nearest square of target type
tuple<int, Move, int> Board::getShortestPathToSquareType(const Coord& head, SQUARE targetType) const
{
    vector<vector<Coord>> visit(height, vector<Coord>(width, Coord(-1,-1)));
    Move move = NO_MOVE;
    queue<Coord> q;
    Coord pos(-1, -1);

    q.push(head);
    visit[head.y][head.x] = pos;
    bool cont = true;
    bool found = false;
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
            if(visit[next.y][next.x].y >= 0)
                continue;

            if (board[next.y][next.x].square == targetType)
            {
                visit[next.y][next.x] = pos;
                pos = next;
                cont = false;
		found = true;
                break;
            }
            else if (board[next.y][next.x].square != SNAKE)
            {
                visit[next.y][next.x] = pos;
                q.push(next);
            }
        }

        q.pop();
    }

    Coord lastPos = pos;
    int dist = 0;
    int cost = 0;
    if(found)
    {
        dist++;
        Coord next = visit[pos.y][pos.x];
	while(next.y != head.y || next.x != head.x)
	{
                ++dist;
                ++cost;
                pos = next;
                next = visit[pos.y][pos.x];
	}
	move = moveFromTo(head, pos);

    }

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
                        cout << board[y][x].square;
		    else if (board[y][x].square == SNAKE)
                        cout << board[y][x].square;
		    else
		        cout << moveFromTo(visit[y][x], {y, x});
	        }
	        cout << endl;
	    }
	    cout << "dist: " << dist << endl;
	    cout << "move: " << move << endl;
        }

    return make_tuple(dist, move, cost);
}

int Board::distanceTo(const Coord& pos, SQUARE squareType, bool allow_hazard) const
{
    int h = 0;
    Coord next = pos;
    while(   board[next.y][next.x].square == FREE
          || (squareType == SNAKE && board[next.y][next.x].square == FOOD)
          || (allow_hazard && board[next.y][next.x].square == HAZARD))
    {
        if (moveField[next.y][next.x] == NO_MOVE)
            break;
	next = moveFrom(next, moveField[next.y][next.x]);
	++h;
        if (h > width * height)
            break;
    }
    if(board[next.y][next.x].square != squareType)
    {
        h = INF;
    }
    return h;
}

tuple<int, int, int, int> Board::distanceToFoodAndSnake(const Coord& pos) const
{
    int d = 0, d_food = INF;
    int food = 0;
    Coord next = pos;
    while(board[next.y][next.x].square != SNAKE)
    {
	if(board[next.y][next.x].square == FOOD)
	{
            if(food == 0) d_food = d;
            ++food;
	}

	next = moveFrom(next, moveField[next.y][next.x]);
	++d;
        if (d > maxSnakeLength)
            break;
    }
    /*
    if(board[next.y][next.x].square != SNAKE)
    {
        h = INF;
    }
    */
    return make_tuple(d, food, board[next.y][next.x].duration, d_food);
}

bool Board::adjacentTo(const Coord& a, const Coord& b) const
{
    return    (a.y == b.y && abs(a.x - b.x) <= 1)
           || (a.x == b.y && abs(a.y - b.y) <= 1);
}

vector<Coord> Board::getNeighbors(const Coord &pos) const
{
    vector<Coord> neighbors;

    if (pos.y > 0)
    {
        neighbors.push_back({ pos.y-1, pos.x });
    }

    if (pos.y < height-1)
    {
        neighbors.push_back({ pos.y+1, pos.x });
    }

    if (pos.x > 0)
    {
        neighbors.push_back({ pos.y, pos.x-1 });
    }

    if (pos.x < width-1)
    {
        neighbors.push_back({ pos.y, pos.x+1 });
    }

    return neighbors;
}

struct Node
{
	static int AGE;
        Coord pos;
        int dist, cost;
	int health;
	int age;

	Node(const Coord& pos = Coord(-1,-1), int dist = 0, int cost = 0, int health = 0) : pos(pos), dist(dist), cost(cost), health(health), age(AGE++) {}

        bool operator<(const Node& node) const
        {
            return cost > node.cost;// && (cost == node.cost && age > node.age);
            //return cost > node.cost && (cost == node.cost && age > node.age);
        }

	static void init()
	{
            AGE = 0;
	}
};

int Node::AGE = 0;

// Use bread first search to search for longest path from source to target square under with considering health not to die
tuple<int, Move, int> Board::getLongestPathTo(const Coord& source, const Coord& target, int health, bool checkAdjacent) const
{
    Node::init();

    vector<vector<Coord>> visit(height, vector<Coord>(width));
    vector<vector<int>> value(height, vector<int>(width, -INF));
    Move move = NO_MOVE;
    priority_queue<Node> q;
    //queue<Node> q;
    Coord pos;

    q.push(Node(source, 0, 0, health));
    visit[source.y][source.x] = pos;

    int dist = 0;
    int cost = 0;
    int h1 = 0;
    bool cont = true;
    bool found = false;
    while(cont && !q.empty())
    {
        Node node = q.top();
        //Node node = q.front();
        pos = node.pos;

        vector<Coord> childs = getNeighbors(pos);

        for(const Coord& next : childs)
        {
            if(visit[next.y][next.x].y >= 0)
                continue;

            if (board[next.y][next.x].square == SNAKE)
                continue;

	    int d = node.dist + 1;
	    int c = node.cost + 1;
	    int h = node.health - 1;
	    //int d = node.dist - 1;
	    //int c = node.cost - 1;

            if (board[next.y][next.x].square == FOOD)
	    {
                h = MAX_HEALTH;
		c += 1000;
	    }

	    if(h <= 0)
                continue;

            if (   (checkAdjacent && adjacentTo(next, target))
                || (!checkAdjacent && next == target))
            {
                visit[next.y][next.x] = pos;
                pos = next;
		dist = d;
		cost = c;
		h1 = h;
                cont = false;
		found = true;
                break;
            }
            else
            {
                visit[next.y][next.x] = pos;
                q.push(Node(next, d, c, h));
            }
        }

        q.pop();
    }

    Coord lastPos = pos;
    if(found)
    {
        Coord next = visit[pos.y][pos.x];
	while(next.y != source.y || next.x != source.x)
	{
            pos = next;
            next = visit[pos.y][pos.x];
	}
	move = moveFromTo(source, pos);
    }

    if (DEBUG && VERBOSE)
    {
        cout << "Shortest PATH: source " << source.y << "/" << source.x << " target=" << target.y << "/" << target.x << endl;
        for(int y = height-1; y >= 0; --y)
        {
            for(int x = 0; x < width; ++x)
            {
                if (y == source.y && x == source.x)
                    cout << "*";
                else if (y == lastPos.y && x == lastPos.x)
                    cout << "!";
                //else if (visit[y][x].y >= 0)
                else if (board[y][x].square == SNAKE)
                    cout << board[y][x].square;
                else if (board[y][x].square == FOOD)
                    cout << board[y][x].square;
                else
                    cout << moveFromTo(visit[y][x], {y, x});
            }
            cout << endl;
        }
        cout << "=>dist: " << dist << endl;
        cout << "=>cost: " << cost << endl;
        cout << "=>move: " << move << endl;
        cout << "=>health: " << h1 << endl;
    }

    return make_tuple(dist, move, cost);
}

// Use bread first search to search for shortest path from source to target square under with considering health not to die
tuple<int, Move, int> Board::getShortestPathTo(const Coord& source, const Coord& target, int health, bool checkAdjacent) const
{
    Node::init();

    vector<vector<Coord>> visit(height, vector<Coord>(width, Coord(-1,-1)));
    Move move = NO_MOVE;
    priority_queue<Node> q;
    //queue<Node> q;
    Coord pos(-1, -1);

    q.push(Node(source, 0, 0, health));
    visit[source.y][source.x] = pos;

    int dist = 0;
    int cost = 0;
    int h1 = 0;
    bool cont = true;
    bool found = false;
    while(cont && !q.empty())
    {
        Node node = q.top();
        //Node node = q.front();
        pos = node.pos;

        vector<Coord> childs = getNeighbors(pos);

        for(const Coord& next : childs)
        {
            if(visit[next.y][next.x].y >= 0)
                continue;

            if (board[next.y][next.x].square == SNAKE)
                continue;

	    int d = node.dist + 1;
	    int c = node.cost + 1;
	    int h = node.health - 1;
	    //int d = node.dist - 1;
	    //int c = node.cost - 1;

            if (board[next.y][next.x].square == FOOD)
	    {
                h = MAX_HEALTH;
		c += 1000;
	    }

	    if(h <= 0)
                continue;

            if (   (checkAdjacent && adjacentTo(next, target))
                || (!checkAdjacent && next == target))
            {
                visit[next.y][next.x] = pos;
                pos = next;
		dist = d;
		cost = c;
		h1 = h;
                cont = false;
		found = true;
                break;
            }
            else
            {
                visit[next.y][next.x] = pos;
                q.push(Node(next, d, c, h));
            }
        }

        q.pop();
    }

    Coord lastPos = pos;
    if(found)
    {
        Coord next = visit[pos.y][pos.x];
	while(next.y != source.y || next.x != source.x)
	{
            pos = next;
            next = visit[pos.y][pos.x];
	}
	move = moveFromTo(source, pos);
    }

    if (DEBUG && VERBOSE)
    {
        cout << "Shortest PATH: source " << source.y << "/" << source.x << " target=" << target.y << "/" << target.x << endl;
        for(int y = height-1; y >= 0; --y)
        {
            for(int x = 0; x < width; ++x)
            {
                if (y == source.y && x == source.x)
                    cout << "*";
                else if (y == lastPos.y && x == lastPos.x)
                    cout << "!";
                //else if (visit[y][x].y >= 0)
                else if (board[y][x].square == SNAKE)
                    cout << board[y][x].square;
                else if (board[y][x].square == FOOD)
                    cout << board[y][x].square;
                else
                    cout << moveFromTo(visit[y][x], {y, x});
            }
            cout << endl;
        }
        cout << "=>dist: " << dist << endl;
        cout << "=>cost: " << cost << endl;
        cout << "=>move: " << move << endl;
        cout << "=>health: " << h1 << endl;
    }

    return make_tuple(dist, move, cost);
}

// Use bread first search to search for farest node from  geven source squares 
tuple<int, Coord, int> Board::getFarestFromSquares(const std::vector<Coord>& sources) const
{
    Node::init();

    vector<vector<Coord>> visit(height, vector<Coord>(width, Coord(-1,-1)));
    //priority_queue<Node> q;
    queue<Node> q;
    Coord pos(-1, -1);
    Coord lastPos = pos;
    set<Coord> used;

    for(const Coord& c : sources)
    {
        if(used.find(c) == used.end())
        {
            q.push(Node(c, 0, 0));
            visit[c.y][c.x] = pos;
	    used.insert(c);
        }
    }

    int dist = 0;
    int cost = 0;
    //int h1 = 0;
    while(!q.empty())
    {
        //Node node = q.top();
        Node node = q.front();
        pos = node.pos;

        vector<Coord> childs = getNeighbors(pos);

        for(const Coord& next : childs)
        {
            if(visit[next.y][next.x].y >= 0)
                continue;

            if (board[next.y][next.x].square == SNAKE)
                continue;

	    int d = node.dist + 1;
	    int c = node.cost + 1;
	    int h = node.health - 1;
	    //int d = node.dist - 1;
	    //int c = node.cost - 1;

	    /*
            if (board[next.y][next.x].square == FOOD)
	    {
                h = MAX_HEALTH;
		c += 1000;
	    }

	    if(h <= 0)
                continue;
		*/

            if (c > cost)
            {
                lastPos = next;
		dist = d;
		cost = c;
		//h1 = h;
            }

            visit[next.y][next.x] = pos;
            q.push(Node(next, d, c, h));
        }

        q.pop();
    }

    if (DEBUG && VERBOSE)
    {
        cout << "Farest PATH: sources:";
	for(auto s : sources)
       	    cout << " " << s.y << "/" << s.x; 
	cout << " target=" << lastPos.y << "/" << lastPos.x << endl;
        for(int y = height-1; y >= 0; --y)
        {
            for(int x = 0; x < width; ++x)
            {
                if (used.find(Coord(y, x)) != used.end())
                    cout << "*";
                else if (y == lastPos.y && x == lastPos.x)
                    cout << "!";
                //else if (visit[y][x].y >= 0)
                else if (board[y][x].square == SNAKE)
                    cout << board[y][x].square;
                else if (board[y][x].square == FOOD)
                    cout << board[y][x].square;
                else
                    cout << moveFromTo(visit[y][x], {y, x});
            }
            cout << endl;
        }
        cout << "=>dist: " << dist << endl;
        cout << "=>cost: " << cost << endl;
        //cout << "=>health: " << h1 << endl;
    }

    return make_tuple(dist, lastPos, cost);
}

vector<Move> Board::generateRealLongLivingMoves4(const Snake& snake) const
{
    vector<Move> moves;
    auto [dist, move, cost] = getLongestPathTo(snake.head, snake.tail, snake.health, true);

    if (move != NO_MOVE)
        moves.push_back(move);

    return moves;
}

vector<Move> Board::generateRealLongLivingMoves3(const Snake& snake) const
{
    vector<Move> moves;
    auto [dist, target, cost] = getFarestFromSquares(snake.body);

    if (target.valid())
    {
        auto [dist, move, cost] = getShortestPathTo(snake.head, target, snake.health, false);

        if (move != NO_MOVE)
            moves.push_back(move);
    }

    return moves;
}

vector<Move> Board::generateRealLongLivingMoves2(const Snake& snake) const
{
    vector<Move> moves;
    auto [dist, move, cost] = getShortestPathTo(snake.head, snake.body.back(), snake.health, true);

    if (move != NO_MOVE)
        moves.push_back(move);

    return moves;
}

vector<Move> Board::generateRealLongLivingMoves(const Coord& head, int health, int length) const
{
    vector<Move> moves;
    vector<Move> moves2 = generateAllSafeMoves(head, false, false);

    if (!moves2.empty())
    {
        if(DEBUG && VERBOSE)
        {
             cout << "MoveField+Food:" << endl;
             for(int y = height-1; y >= 0; --y)
             {
                 for(int x = 0; x < width; ++x)
                 {
                     if(head.y == y && head.x == x)
                         cout << "*";
		     else if(board[y][x].square == FOOD)
                         cout << "F";
		     else if(board[y][x].square != FREE)
                         cout << "#";
		     else
                         cout << moveField[y][x];
                 }
                 cout << endl;
             }
        }

        int best = -INF;
	for(Move move : moves2)
	{
           Coord pos = moveFrom(head, move);
	   //int distToFood = (board[pos.y][pos.x].square == FOOD ? 0 : distanceTo(pos, FOOD));
	   //int distToSnake = distanceTo(pos, SNAKE);
	   tuple<int, int, int, int> score = distanceToFoodAndSnake(pos);
	   int distToSnake = get<0>(score);
	   int countFood = get<1>(score);
	   int snakeDuration = get<2>(score);
	   int distToFood = get<3>(score);
           bool skip = distToSnake <= snakeDuration + countFood; 
	   //int val = -1000 * countFood + distToSnake;
	   //int val = 1000 * distToSnake - countFood;
	   int val = 1000 * distToFood + distToSnake;
	   if(val > best && !skip)
	   {
               moves = { move };
	       best = val;
	   }

	   /*
           if (   distToFood < INF && distToFood < health
               && distToFood > best
               //&& distToSnake > best
               && distToSnake < INF && distToSnake + length >= maxSnakeLength)
           {
               moves = { move };
	       best = distToFood;
	       //best = distToSnake;
           }
	   */

           if(DEBUG && VERBOSE)
           {
             //cout << "=> test move=" << move << " distToFood=" << distToFood << " distToSnake=" << distToSnake << endl;
             cout << "=> test val=" << val << " move=" << move << " countFood=" << countFood << " distToSnake=" << distToSnake << " snakeDuration=" << snakeDuration << (skip ? " => SKIP" : "") << endl;
	   }
	}
    }


    if(moves.empty())
    {
        moves.push_back(moveField[head.y][head.x]);
    }

    return moves;
}

vector<Move> Board::generateLongLivingMoves(const Coord& head, int health) const
{
    vector<Move> moves;

    if (moveField[head.y][head.x] != NO_MOVE)
    {
        moves.push_back(moveField[head.y][head.x]);
    }

    return moves;
}

vector<Move> Board::generateLongLivingMoves_old(const Coord& head, int health) const
{
    vector<Move> moves = generateSafeMoves(head, true);

    if (health <= height + width + FOOD_GREEDINESS)
    {
        auto [dist, move, cost] = getShortestPathToSquareType(head, FOOD);
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

vector<Move> Board::generateSafeMoves(const Coord& head, bool onlyFood, bool noFood) const
{
    vector<Move> moves;

    if (head.y > 0 && (   (!onlyFood && getSquare(head.y-1, head.x).square == FREE)
                       || (!noFood && getSquare(head.y-1, head.x).square == FOOD)))
        moves.push_back(DOWN);

    else if (head.y < height-1 && (   (!onlyFood && getSquare(head.y+1, head.x).square == FREE)
                                   || (!noFood && getSquare(head.y+1, head.x).square == FOOD)))
        moves.push_back(UP);

    else if (head.x <= width / 2) 
    {
    	if (head.x < width-1 && (   (!onlyFood && getSquare(head.y, head.x+1).square == FREE)
                                 || (!noFood && getSquare(head.y, head.x+1).square == FOOD)))
             moves.push_back(RIGHT);

        else if (head.x > 0 && (   (!onlyFood && getSquare(head.y, head.x-1).square == FREE)
                                || (!noFood && getSquare(head.y, head.x-1).square == FOOD)))
             moves.push_back(LEFT);
    }
    else
    {
    	if (head.x > 0 && (   (!onlyFood && getSquare(head.y, head.x-1).square == FREE)
                           || (!noFood && getSquare(head.y, head.x-1).square == FOOD)))
             moves.push_back(LEFT);

        else if (head.x < width-1 && (   (!onlyFood && getSquare(head.y, head.x+1).square == FREE)
                                      || (!noFood && getSquare(head.y, head.x+1).square == FOOD)))
             moves.push_back(RIGHT);
    }

    return moves;
}

vector<Move> Board::generateAllSafeMoves(const Coord& head, bool onlyFood, bool noFood) const
{
    vector<Move> moves;

    if (head.y > 0 && (   (!onlyFood && getSquare(head.y-1, head.x).square == FREE)
                       || (!noFood && getSquare(head.y-1, head.x).square == FOOD)))
        moves.push_back(DOWN);

    if (head.y < height-1 && (   (!onlyFood && getSquare(head.y+1, head.x).square == FREE)
                                   || (!noFood && getSquare(head.y+1, head.x).square == FOOD)))
        moves.push_back(UP);

    if (head.x < width-1 && (   (!onlyFood && getSquare(head.y, head.x+1).square == FREE)
                             || (!noFood && getSquare(head.y, head.x+1).square == FOOD)))
         moves.push_back(RIGHT);

    if (head.x > 0 && (   (!onlyFood && getSquare(head.y, head.x-1).square == FREE)
                            || (!noFood && getSquare(head.y, head.x-1).square == FOOD)))
         moves.push_back(LEFT);

    return moves;
}
