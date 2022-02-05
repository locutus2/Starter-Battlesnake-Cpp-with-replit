#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>

#include "defs.h"

using  namespace std;

void print(const string& msg, ostream& out = cout);

void print_debug(const string& msg, ostream& out = cout);

void print_verbose(const string& msg, ostream& out = cout);

ostream& operator<<(ostream& out, Move move);

ostream& operator<<(ostream& out, SQUARE sq);

#endif
