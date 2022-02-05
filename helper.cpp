#include "helper.h"

void print(const string& msg, ostream& out)
{
    out << msg << endl;
}

void print_debug(const string& msg, ostream& out)
{
   if (DEBUG)
       out << msg << endl;
}

void print_verbose(const string& msg, ostream& out)
{
   if (VERBOSE)
       out << msg << endl;
}

ostream& operator<<(ostream& out, Move move)
{
    switch(move)
    {
        case NO_MOVE:
            out << "-";
	    break;
        case UP:
            //out << "up";
            out << "A";
	    break;
        case DOWN:
            //out << "down";
            out << "V";
	    break;
        case LEFT:
            //out << "left";
            out << "<";
	    break;
        case RIGHT:
            //out << "right";
            out << ">";
	    break;
        default:
	    break;
    }
    return out;
}

ostream& operator<<(ostream& out, SQUARE sq)
{
    switch(sq)
    {
        case FREE:
            out << ".";
	    break;
        case FOOD:
            out << "F";
	    break;
        case HAZARD:
            out << "H";
	    break;
        case SNAKE:
            out << "S";
	    break;
        default:
	    break;
    }
    return out;
}
