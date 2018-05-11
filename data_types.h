#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>

using namespace std;

namespace SAT
{
	vector<vector<int> > vector_literals;

	typedef struct clause {
		int satisfied_count;
		vector<int> unassigned_literals;
	} clause;

	vector<clause> vector_clauses;
}

#endif
