#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Student.h"
#include "MemoStudentVector.h"  

MemoStudentVector::MemoStudentVector(const char* fileName) : fileName(fileName)
{
	refill();
}

void MemoStudentVector::refill()
{
	std::ifstream in(fileName);
	students.clear();
	std::string line;
	while(std::getline(in, line))
		students.emplace_back(line);
	in.close();
}

void MemoStudentVector::save() const
{
	const std::string tempFileName = std::string("temp.") + fileName;
	std::ofstream out(tempFileName);

	for (auto& student : students) out << student;
	out.close();

	remove(fileName);
	rename(tempFileName.c_str(), fileName);
	remove(tempFileName.c_str());
}

std::vector<Student> MemoStudentVector::getAll()
{
	return students;
}

void MemoStudentVector::push_back(const Student& student)
{
	this->students.push_back(student);
	save();
}

void MemoStudentVector::update(const int pos, const Student& student)
{
	this->students[pos] = student;
	save();
}



