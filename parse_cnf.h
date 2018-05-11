#ifndef PARSE_CNF_H
#define PARSE_CNF_H

#include <iostream>
#include <fstream>
#include <string>
#include "data_types.h"

using namespace std;
using namespace SAT;

int get_integer(vector<char> vector_characters)
{
	int negative = 1, end_position = 0, temporary_integer = 0, multiplication_factor = 1, i;

	if(vector_characters.front() == '-') {
		negative = -1;
		end_position = 1;
	}

	for(i = vector_characters.size() - 1; i >= end_position; i--) {
		temporary_integer += (vector_characters.at(i) - '0') * multiplication_factor;
		multiplication_factor *= 10;
	}

	return negative * temporary_integer;
}

int add_clause(vector<int> clause_literals)
{
	clause temporary_clause = {0, clause_literals};

	vector_clauses.push_back(temporary_clause);

	return 0;
}

int add_clause_number(int clause_number, int current_literal)
{
	int negative = 1;

	if(current_literal < 0) {
		negative = -1;
	}

	vector_literals.at(negative * current_literal - 1).push_back(negative * clause_number);

	return 0;
}

int parse_file(string file_name)
{
	char character;
	vector<char> vector_characters;
	int number_variables, clause_number = 1, current_literal, i;
	vector<int> clause_literals;

	ifstream in_file(file_name.c_str());

	if(in_file.is_open()) {
		do {
			in_file.get(character);

			if(character == 'c') {
				while(character != '\n') {
					in_file.get(character);
				}
			} else if(character == 'p') {
				while(!isdigit(character)) {
					in_file.get(character);
				}

				do {
					vector_characters.push_back(character);
					in_file.get(character);
				} while(character != ' ');
				number_variables = get_integer(vector_characters);
				vector_characters.clear();
				vector_literals.resize(number_variables);

				while(character != '\n') {
					in_file.get(character);
				}
			} else if(isdigit(character) || character == '-') {
				if(character == '0') {
					add_clause(clause_literals);
					clause_number += 1;
					clause_literals.clear();
					continue;
				}

				do {
					vector_characters.push_back(character);
					in_file.get(character);
				} while(character != ' ');
				current_literal = get_integer(vector_characters);
				vector_characters.clear();
				add_clause_number(clause_number, current_literal);
				clause_literals.push_back(current_literal);
			}
		} while(!in_file.eof());
	} else {
		cout << "File cannot be opened." << endl;
		return -1;
	}

	return 0;
}

#endif
