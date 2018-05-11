#include <cstdlib>
#include <ctime>
#include "parse_cnf.h"
#include "sat_solver.h"

using namespace std;
using namespace SAT;

int main(int argc, char **argv)
{
	string file_name;
	clock_t t;

	if(argc == 1) {
		cout << "Usage: " << argv[0] << " " << "<file name>" << " " << "--enable_restarts" << " " << "<k value>(integer)" << " " << "<a value>(float)" << endl;
		return 0;
	} else if(argc == 2) {
		file_name = argv[1];
		enable_restarts = false;
	} else if(argc == 3) {
		file_name = argv[1];

		if(static_cast<string>(argv[2]) != "--enable_restarts") {
			cout << "Usage: " << argv[0] << " " << "<file name>" << " " << "--enable_restarts" << " " << "<k value>(integer)" << " " << "<a value>(float)" << endl;
			return 0;
		}

		enable_restarts = true;
		k = 100;
		a = 1.5;

		cout << "k: " << k << " " << "(default)" << endl;
		cout << "a: " << a << " " << "(default)" << endl;
	} else if(argc == 4) {
		cout << "Usage: " << argv[0] << " " << "<file name>" << " " << "--enable_restarts" << " " << "<k value>(integer)" << " " << "<a value>(float)" << endl;
		return 0;
	} else if(argc == 5) {
		file_name = argv[1];

		if(static_cast<string>(argv[2]) != "--enable_restarts") {
			cout << "Usage: " << argv[0] << " " << "<file name>" << " " << "--enable_restarts" << " " << "<k value>(integer)" << " " << "<a value>(float)" << endl;
			return 0;
		}

		enable_restarts = true;
		k = atoi(argv[3]);
		a = strtof(argv[4], NULL);

		cout << "k: " << k << endl;
		cout << "a: " << a << endl;
	} else {
		cout << "Usage: " << argv[0] << " " << "<file name>" << " " << "--enable_restarts" << " " << "<k value>(integer)" << " " << "<a value>(float)" << endl;
		return 0;
	}

	cout << endl;

	t = clock();
	if(parse_file(file_name) == -1) {
			cout << "ERROR in parsing file. EXIT!" << endl;
			return 0;
	}
	t = clock() - t;

	cout << "Number of variables: " << vector_literals.size() << endl;
	cout << "Number of clauses: " << vector_clauses.size() << endl;
	cout << "Parsing time: " << static_cast<float>(t) / CLOCKS_PER_SEC << "s" << endl;

	cout << endl;

	t = clock();
	dpll_algorithm();
	t = clock() - t;

	cout << "Number of conflicts: " << number_conflicts << endl;
	if(enable_restarts) {
		cout << "Number of restarts: " << number_restarts << endl;
	}
	cout << "Computation time: " << static_cast<float>(t) / CLOCKS_PER_SEC << "s" << endl;

	return 0;
}
