#pragma once
#include <sstream>
#include <string>

#define GRADES_AMOUNT 8
struct Student
{
	std::string fullName;
	char gender = '*';
	int group = 0;
	int positionInGroup = 0;
	int grades[GRADES_AMOUNT] = {};

	Student() = default;
	explicit Student(const std::string& serialized)
	{
		std::stringstream ss;
		std::string str;

		ss << serialized;
		std::getline(ss, fullName, ';');

		std::getline(ss, str, ';');
		gender = str[0];

		std::getline(ss, str, ';');
		group = stoi(str);

		std::getline(ss, str, ';');
		positionInGroup = stoi(str);

		for (int& grade : grades) ss >> grade;  
	}
};

inline std::ostream& operator<<(std::ostream& out, Student const& st) {

	out << st.fullName << ';'
	<< st.gender << ';'
	<< st.group << ';'
	<< st.positionInGroup << ';';

	for (const int grade : st.grades)
		out << grade << ' ';
	out <<'\n';

	return out;
}
