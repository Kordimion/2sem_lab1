#pragma once
#include <vector>
#include "Student.h"

class MemoStudentVector
{
private:
	const char* fileName;
	std::vector<Student> students;
	void refill();
	void save() const;
public:
	explicit MemoStudentVector(const char* fileName);
	std::vector<Student> getAll();
	void push_back(const Student& student);
	void update(const int pos, const Student& student);
};


