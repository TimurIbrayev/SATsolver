#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

#include <iostream>
#include <algorithm>
#include <cmath>
#include "data_types.h"

using namespace std;
using namespace SAT;

bool enable_restarts;
int number_conflicts = 0, k, number_restarts = 0;
float a;
vector<int> pure_literals;
vector<int> forced_literals;
vector<int> free_literals;
vector<int> decision_literals;
vector<int> satisfied_clause_numbers;
vector<int> free_literal_positions;

int sign(int temporary_integer)
{
	if(temporary_integer < 0) {
		return -1;
	} else {
		return 1;
	}
}

int find_pure_literals()
{
	int negative, i, j;
	bool flag;

	for(i = 0; i < vector_literals.size(); i++) {
		negative = sign(vector_literals.at(i).front());
		flag = true;
		for(j = 1; j < vector_literals.at(i).size(); j++) {
			if(sign(vector_literals.at(i).at(j)) != negative) {
				flag = false;
				break;
			}
		}
		if(flag) {
			pure_literals.push_back(negative * (i + 1));
		}
	}

	return 0;
}

int find_forced_literals()
{
	int current_literal, i;

	for(i = 0; i < vector_clauses.size(); i++) {
		if(vector_clauses.at(i).satisfied_count == 0 && vector_clauses.at(i).unassigned_literals.size() == 1) {
			current_literal = vector_clauses.at(i).unassigned_literals.front();
			if(find(forced_literals.begin(), forced_literals.end(), current_literal) == forced_literals.end()) {
				forced_literals.push_back(current_literal);
			}
		}
	}

	return 0;
}

int find_free_literals()
{
	int i;

	for(i = 1; i <= vector_literals.size(); i++) {
		if(find(decision_literals.begin(), decision_literals.end(), i) == decision_literals.end() && find(decision_literals.begin(), decision_literals.end(), -i) == decision_literals.end()) {
			free_literals.push_back(i);
		}
	}

	return 0;
}

int assign_literal(int current_literal)
{
	int negative, current_clause_number, i;

	negative = sign(current_literal);

	for(i = 0; i < vector_literals.at(negative * current_literal - 1).size(); i++) {
		current_clause_number = vector_literals.at(negative * current_literal - 1).at(i);
		if(sign(current_clause_number) == negative) {
			vector_clauses.at(negative * current_clause_number - 1).satisfied_count += 1;
			vector_clauses.at(negative * current_clause_number - 1).unassigned_literals.erase(
			remove(vector_clauses.at(negative * current_clause_number - 1).unassigned_literals.begin(),
			vector_clauses.at(negative * current_clause_number - 1).unassigned_literals.end(), current_literal),
			vector_clauses.at(negative * current_clause_number - 1).unassigned_literals.end());

			if(find(satisfied_clause_numbers.begin(), satisfied_clause_numbers.end(), negative * current_clause_number) == satisfied_clause_numbers.end()) {
				satisfied_clause_numbers.push_back(negative * current_clause_number);
			}
		} else {
			vector_clauses.at(-1 * negative * current_clause_number - 1).unassigned_literals.erase(
			remove(vector_clauses.at(-1 * negative * current_clause_number - 1).unassigned_literals.begin(),
			vector_clauses.at(-1 * negative * current_clause_number - 1).unassigned_literals.end(), -current_literal),
			vector_clauses.at(-1 * negative * current_clause_number - 1).unassigned_literals.end());
		}
	}

	return 0;
}

bool detect_conflict()
{
	int i;

	for(i = 0; i < forced_literals.size(); i++) {
		if(find(forced_literals.begin(), forced_literals.end(), -forced_literals.at(i)) != forced_literals.end()) {
			return true;
		}
	}

	return false;
}

int result_satisfiable()
{
	int i;

	sort(decision_literals.begin(), decision_literals.end());

	cout << "Result: SATISFIABLE" << endl;
	cout << "Variable assignment:";
	for(i = 0; i < decision_literals.size(); i++) {
		cout << " " << decision_literals.at(i);
	}
	cout << endl;

	return 0;
}

int restart()
{
	int current_literal, current_clause_number, negative, i;

	while(decision_literals.size() - 1 != free_literal_positions.front() - 1) {
		current_literal = decision_literals.back();
		negative = sign(current_literal);

		for(i = 0; i < vector_literals.at(negative * current_literal - 1).size(); i++) {
			current_clause_number = vector_literals.at(negative * current_literal - 1).at(i);
			if(sign(current_clause_number) == negative) {
				vector_clauses.at(negative * current_clause_number - 1).satisfied_count -= 1;
				vector_clauses.at(negative * current_clause_number - 1).unassigned_literals.push_back(current_literal);

				if(vector_clauses.at(negative * current_clause_number - 1).satisfied_count == 0) {
					satisfied_clause_numbers.erase(remove(satisfied_clause_numbers.begin(), satisfied_clause_numbers.end(), negative * current_clause_number), satisfied_clause_numbers.end());
				}
			} else {
				vector_clauses.at(-1 * negative * current_clause_number - 1).unassigned_literals.push_back(-current_literal);
			}
		}

		free_literals.push_back(negative * current_literal);
		decision_literals.pop_back();
	}

	free_literal_positions.clear();

	return 0;
}

int chronological_backtrack()
{
	int current_literal, current_clause_number, negative, i;

	current_literal = decision_literals.back();
	negative = sign(current_literal);

	for(i = 0; i < vector_literals.at(negative * current_literal - 1).size(); i++) {
		current_clause_number = vector_literals.at(negative * current_literal - 1).at(i);
		if(sign(current_clause_number) == negative) {
			vector_clauses.at(negative * current_clause_number - 1).satisfied_count -= 1;
			vector_clauses.at(negative * current_clause_number - 1).unassigned_literals.push_back(current_literal);

			if(vector_clauses.at(negative * current_clause_number - 1).satisfied_count == 0) {
				satisfied_clause_numbers.erase(remove(satisfied_clause_numbers.begin(), satisfied_clause_numbers.end(), negative * current_clause_number), satisfied_clause_numbers.end());
			}
		} else {
			vector_clauses.at(-1 * negative * current_clause_number - 1).unassigned_literals.push_back(-current_literal);
		}
	}

	if(decision_literals.size() - 1 != free_literal_positions.back()) {
		free_literals.push_back(negative * current_literal);
		decision_literals.pop_back();
		chronological_backtrack();
		return 0;
	} else {
		if(sign(decision_literals.back()) == 1) {
			decision_literals.back() = -decision_literals.back();
			return 0;
		} else {
			free_literals.push_back(negative * current_literal);
			decision_literals.pop_back();
			free_literal_positions.pop_back();

			if(free_literal_positions.empty()) {
				return 0;
			}

			chronological_backtrack();
			return 0;
		}
	}
}

int dpll_algorithm()
{
	int temporary_number_conflicts = 0;

	find_pure_literals();
	while(!pure_literals.empty()) {
		decision_literals.push_back(pure_literals.front());
		pure_literals.erase(pure_literals.begin());
		assign_literal(decision_literals.back());
	}

	find_forced_literals();
	while(!forced_literals.empty()) {
		if(detect_conflict()) {
			number_conflicts += 1;
			cout << "Result: UNSATISFIABLE" << endl;
			return 0;
		}
		decision_literals.push_back(forced_literals.front());
		forced_literals.erase(forced_literals.begin());
		assign_literal(decision_literals.back());
		find_forced_literals();
	}

	find_free_literals();

	while(!free_literals.empty()) {
		decision_literals.push_back(free_literals.front());
		free_literals.erase(free_literals.begin());
		free_literal_positions.push_back(decision_literals.size() - 1);
		assign_literal(decision_literals.back());

		find_forced_literals();
		while(!forced_literals.empty()) {
			if(detect_conflict()) {
				number_conflicts += 1;

				if(enable_restarts) {
					temporary_number_conflicts += 1;

					if(temporary_number_conflicts == static_cast<int>(k * pow(a, number_restarts))) {
						restart();
						forced_literals.clear();
						temporary_number_conflicts = 0;
						number_restarts += 1;
						break;
					}
				}

				chronological_backtrack();

				if(free_literal_positions.empty()) {
					cout << "Result: UNSATISFIABLE" << endl;
					return 0;
				}

				assign_literal(decision_literals.back());
				forced_literals.clear();
				find_forced_literals();
				continue;
			}

			decision_literals.push_back(forced_literals.front());
			free_literals.erase(remove(free_literals.begin(), free_literals.end(), sign(forced_literals.front()) * forced_literals.front()), free_literals.end());
			forced_literals.erase(forced_literals.begin());
			assign_literal(decision_literals.back());
			find_forced_literals();
		}
	}

	if(satisfied_clause_numbers.size() == vector_clauses.size()) {
		result_satisfiable();
	} else {
		cout << "Result: UNSATISFIABLE" << endl;
	}

	return 0;
}

#endif