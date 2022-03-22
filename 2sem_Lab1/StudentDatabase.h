#pragma once
#include "Student.h"
#include <vector>

class StudentDatabase
{
private:
	const char * filePath ;
public:
	StudentDatabase(const char * filePath);

	Student append(const Student& student) const;
	Student edit(int pos, const Student& student) const;
	std::vector<Student> getStudents() const;
};
