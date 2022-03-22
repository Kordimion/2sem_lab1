#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

#include "Student.h"
#include "MemoStudentVector.h"

constexpr auto CREATE_STUDENT = "new";
constexpr auto CHANGE_STUDENT = "edit";
constexpr auto STUDENT_INFO = "info";
constexpr auto TOP = "top";
constexpr auto COUNT_BY_GENDER = "gender";
constexpr auto STUDENT_INFO_GRADES = "info_grades";
constexpr auto STUDENT_INFO_LIST_NUMBER = "info_list_number";
constexpr auto STUDENT_INFO_GROUP = "info_group";
constexpr auto EXIT = "exit";
constexpr auto HELP = "help";
constexpr auto ABOUT = "about";

MemoStudentVector database("students.txt");

inline void askFor(const char* item)
{
    std::cout << "\n" << item << " : ";
}

inline void printLine(const char* item)
{
	std::cout << "\n" << item;
}
inline void printStudentInfo(const Student& student)
{
	std::cout << "\nFullName: " << student.fullName;
	std::cout << "\nGender " << student.gender;
	std::cout << "\nGroup: " << student.group;
	std::cout << "\nPosition in group: " << student.positionInGroup;
	std::cout << "\nGrades: ";
	for (const int& grade : student.grades) std::cout << grade << ' ';
	std::cout << "\n";
}

void createStudentEndpoint()
{
	Student student;
	printLine("Student creation started");

	std::cout << "\nFull name: ";
	getline(std::cin, student.fullName);

	std::cout << "Gender (F/M): ";
	scanf_s("%c", &student.gender);

	std::cout << "Group number: ";
	scanf_s("%d", &student.group);

	std::cout << "Position in Group : ";
	scanf_s("%d", &student.positionInGroup);

	std::cout << "Grades : ";
	for (int& grade : student.grades) scanf_s("%d", &grade);

	database.push_back(student);

	printLine("Student added");
}
void changeStudentEndpoint()
{
	int studentNumber;
	askFor("Student number");
	std::cin >> studentNumber;

	const auto students = database.getAll();
	if(students.size() < studentNumber)
	{
		std::cout << "\nStudent #" << studentNumber << " does not exist in database";
		return;
	}
	
	Student student = students[studentNumber];
	std::cout << "\nStudent #" << studentNumber << " data:";
	std::cout << "\nEmpty line keeps previous value";

	std::string inputStr;
	std::stringstream ss;
	getline(std::cin, inputStr); // this extra getline is required to wait before getting next line

	std::cout << "\n" << "FullName (before: " << student.fullName << "): ";
	getline(std::cin, inputStr);
	if (!inputStr.empty()) student.fullName = inputStr;

	std::cout << "Gender (F/M) (before: " << student.gender << "): ";
	getline(std::cin, inputStr);
	if (!inputStr.empty())
	{
		ss << inputStr << ' ';
		ss >> student.gender;
	}

	std::cout << "Group Number (before: " << student.group << "): ";
	getline(std::cin, inputStr);
	if (!inputStr.empty())
	{
		ss << inputStr << ' ' ;
		ss >> student.group;
	}

	std::cout << "Position in Group (before: " << student.positionInGroup << "): ";
	getline(std::cin, inputStr);
	if (!inputStr.empty())
	{
		ss << inputStr << ' ';
		int pos;
		ss >> pos;
		student.positionInGroup = pos;
	}

	std::cout << "Grades (before: ";
	for (const int& grade : student.grades) std::cout << grade << ' ';
	std::cout << "): ";

	getline(std::cin, inputStr);
	if (!inputStr.empty())
	{
		ss << inputStr << ' ';
		for (int& grade : student.grades) {
			int tempGrade;
			ss >> tempGrade;
			grade = tempGrade;
		}
	}

	database.update(studentNumber, student);

	std::cout << "\nStudent #" << studentNumber << " was changed";
}
void studentInfoEndpoint()
{
	const auto students = database.getAll();
	std::cout << "\nStudents info: \n";

	for (const auto& student : students)
		printStudentInfo(student);

}
void topEndpoint()
{
	const auto students = database.getAll();
	std::vector<std::pair<Student, int>> vct(students.size());

	for(int i = 0; i < students.size(); ++i)
	{
		vct[i].first = students[i];
		vct[i].second = std::accumulate(students[i].grades, students[i].grades + GRADES_AMOUNT, 0);
	}
	std::sort(vct.begin(), vct.end(), [](const std::pair<Student, int>& a, const std::pair<Student, int>& b)
		{
			return a.second > b.second;
		});

	for(int i = 0; i < vct.size(); ++i)
	{
		std::cout << "\n" << i + 1 << ": ";
		std::cout << "Avg grade (" << static_cast<double>(vct[i].second) / GRADES_AMOUNT << ") ";
		std::cout << "Full name (" << vct[i].first.fullName << ")";
	}
}
void countByGenderEndpoint()
{
	const auto students = database.getAll();

	int femaleCount = 0, maleCount = 0;
	for (const auto& student : students)
	{
		if (student.gender == 'F') ++femaleCount;
		else if (student.gender == 'M') ++maleCount;
	}

	std::cout << "\nGender count: \n";
	std::cout << "\nFemale students total: " << femaleCount;
	std::cout << "\nMale students total: " << maleCount << "\n";
}
void studentInfoGradesEndpoint()
{
	const auto students = database.getAll();
	std::vector<Student> studentsWithoutScholarship, students4And5, students5Only;

	for (const auto& student : students)
	{
		if (std::any_of(student.grades, student.grades + GRADES_AMOUNT, [](const int a) {return a <= 3; }))
		{
			studentsWithoutScholarship.push_back(student);
		}
		else 
		{
			students4And5.push_back(student);
			if (std::all_of(student.grades, student.grades + GRADES_AMOUNT, [](const int a) {return a == 5; }))
				students5Only.push_back(student);
		}
	} 
		
	std::cout << "\nInfo about students without scholarship: \n";
	for (const auto& student : studentsWithoutScholarship)
		printStudentInfo(student);

	std::cout << "\nInfo about students with 4 and 5 grades only: \n";
	for (const auto& student : students4And5)
		printStudentInfo(student);

	std::cout << "\nInfo about students with 5 grades only: \n";
	for (const auto& student : students5Only)
		printStudentInfo(student);
}
void studentInfoListNumberEndpoint()
{
	int groupListNumber;
	askFor("Group List Number");
	std::cin >> groupListNumber;

	const auto students = database.getAll();
	std::vector<Student> groupListNumberStudents;
	for (const auto& student : students)
		if (student.positionInGroup == groupListNumber)
			groupListNumberStudents.push_back(student);

	std::cout << "\nInfo about students with group list number (" << groupListNumber << "): \n";
	for (const auto& student : groupListNumberStudents)
		printStudentInfo(student);
}
void studentInfoGroupEndpoint()
{
	int groupNumber;
	askFor("Group Number");
	std::cin >> groupNumber;

	const auto students = database.getAll();
	std::vector<Student> groupNumberStudents;
	for (const auto& student : students)
		if (student.group == groupNumber)
			groupNumberStudents.push_back(student);

	std::cout << "\nInfo about students with group number (" << groupNumber << "): \n";
	for (const auto& student : groupNumberStudents)
		printStudentInfo(student);
}
void infoEndpoint()
{
	std::cout << "\nThis lab is all about structures:";
	std::cout << "\nUser should be able to create and store object of the same structure";
	std::cout << "\nStructure of this lab is a student";
	std::cout << "\nStudent has (FullName, Gender(M/F), Group Number, Position in Group, Grades)";
	std::cout << "\nTo view all available operations with students, print " << HELP;
}

inline void printCommandDescription(const char * commandName, const char * desc)
{
	std::cout << "\n" << commandName << " - " << desc;
}
void helpEndpoint()
{
	std::cout << "\nCommands:";
	printCommandDescription(CREATE_STUDENT, "Create new student");
	printCommandDescription(STUDENT_INFO, "View info about all students");
	printCommandDescription(STUDENT_INFO_GRADES, "View grades statistics");
	printCommandDescription(STUDENT_INFO_GROUP, "View info about all students by group number");
	printCommandDescription(STUDENT_INFO_LIST_NUMBER, "View info about all students by group list number");
	printCommandDescription(COUNT_BY_GENDER, "Count amount of students by gender");
	printCommandDescription(TOP, "Create top of the most performing students");
	printCommandDescription(ABOUT, "View info about this program");
	printCommandDescription(EXIT, "Stop program execution");
}
void exitEndpoint()
{
	std::cout << "\nProgram execution stopped";
}

void processEndpoint(const char * endpoint)
{
	if (strcmp(endpoint,CREATE_STUDENT) == 0) return createStudentEndpoint();
	if (strcmp(endpoint,CHANGE_STUDENT) == 0) return changeStudentEndpoint();
	if (strcmp(endpoint,STUDENT_INFO) == 0) return studentInfoEndpoint();
	if (strcmp(endpoint,TOP) == 0) return topEndpoint();
	if (strcmp(endpoint,COUNT_BY_GENDER) == 0) return countByGenderEndpoint();
	if (strcmp(endpoint,STUDENT_INFO_GRADES) == 0) return studentInfoGradesEndpoint();
	if (strcmp(endpoint,STUDENT_INFO_GROUP) == 0) return studentInfoGroupEndpoint();
	if (strcmp(endpoint,STUDENT_INFO_LIST_NUMBER) == 0) return studentInfoListNumberEndpoint();
	if (strcmp(endpoint,HELP) == 0) return helpEndpoint();
	if (strcmp(endpoint,ABOUT) == 0) return infoEndpoint();
	if (strcmp(endpoint,EXIT) == 0) return exitEndpoint();

	std::cout << "\nCommand not found";
	std::cout << "\nPrint help to view list of all commands\n";
}

int main()
{
    std::string endpoint;
	infoEndpoint();
	bool shouldAskForYourCommand = true;
    while(endpoint != EXIT)
    {
		if(shouldAskForYourCommand) askFor("\nYour Command");
		std::getline(std::cin, endpoint);
		shouldAskForYourCommand = true;

		if (!endpoint.empty())
			processEndpoint(endpoint.c_str());
		else shouldAskForYourCommand = false;
    }
}
