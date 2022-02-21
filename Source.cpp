#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::getline;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::advance;
using std::find_if;

typedef unsigned short ushort_t;

bool IsDigit(const char& c)
{
	return (c >= '0' && c <= '9');
}

bool IsLower(const char& c)
{
	return ((c >= '�' && c <= '�') || (c >= 'a' && c <= 'z'));
}

bool IsUpper(const char& c)
{
	return ((c >= '�' && c <= '�') || (c >= 'A' && c <= 'Z'));
}

bool IsCyrillic(const char& c)
{
	return ((c >= '�' && c <= '�') || (c >= '�' && c <= '�'));
}

class Exception
{
	string msg;
	int errorCode;

public:

	Exception(const char* msg, const int& errorCode)
	{
		this->msg = "������: ";
		this->msg.append(msg);
		this->errorCode = errorCode;
	}

	const char* what() const
	{
		return msg.c_str();
	}

	const int getErrorCode() const
	{
		return errorCode;
	}

};

class FailedOpeningException : public Exception
{
public:

	FailedOpeningException(const char* msg = "�� ������� ������� ����", const int& errorCode = -5)
		: Exception(msg, errorCode) {};

};

string Encrypt(const string& str, string& key)
{
	string result = str;
	key = "";
	char buf[255]{};

	for (size_t i = 0; i < result.size(); i++)
	{
		key.append(_itoa(rand() % 10, buf, 10));
		result[i] += key[i];
	}

	return result;
}

string Decrypt(const string& str, const string& key)
{
	string result = str;

	for (size_t i = 0; i < result.size(); i++)
		result[i] -= key[i];

	return result;
}

class Question
{
	string quest;
	vector<string> answerOptions;
	ushort_t correctOption;

public:

	Question(const string& quest, const vector<string>& answerOptions, const ushort_t& correctOption)
	{
		this->quest = quest;
		this->answerOptions = answerOptions;
		this->correctOption = correctOption;
	}
	Question(const Question& question)
	{
		quest = question.quest;
		answerOptions = question.answerOptions;
		correctOption = question.correctOption;
	}

	void setQuest(const string& quest)
	{
		this->quest = quest;
	}
	void setAnswerOptions(const vector<string>& answerOptions)
	{
		this->answerOptions = answerOptions;
	}
	void setCorrectOption(const ushort_t& correctOption)
	{
		this->correctOption = correctOption;
	}

	string getQuest() const
	{
		return quest;
	}
	vector<string> getAnswerOptions() const
	{
		return answerOptions;
	}
	ushort_t getCorrectOption() const
	{
		return correctOption;
	}

};

class Test
{
	string name;
	vector<ushort_t> gradesHistory;
	ushort_t numOfQuestions;
	vector<Question> questions;

public:

	Test(const string& name, const vector<ushort_t>& gradesHistory, const ushort_t& numOfQuestions, const vector<Question>& questions)
	{
		try
		{
			if (numOfQuestions != questions.size())
				throw Exception("������������ ������ � ������������ ����� (���������� �������� �� ��������� � �������� ������� ��������)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		this->name = name;
		this->gradesHistory = gradesHistory;
		this->numOfQuestions = numOfQuestions;
		this->questions = questions;
	}
	Test(const Test& test)
	{
		name = test.name;
		gradesHistory = test.gradesHistory;
		numOfQuestions = test.numOfQuestions;
		questions = test.questions;
	}

	void AddQuestion(const Question& question)
	{
		numOfQuestions++;
		questions.push_back(question);
	}

	void AddGrade(const ushort_t& grade)
	{
		gradesHistory.push_back(grade);
	}

	void ChangeQuestionQuest(const ushort_t& questIndex, const string& quest)
	{
		questions[questIndex].setQuest(quest);
	}
	void ChangeQuestionAnswerOptions(const ushort_t& questIndex, const vector<string>& answerOptions)
	{
		questions[questIndex].setAnswerOptions(answerOptions);
	}
	void ChangeQuestionCorrectOption(const ushort_t& questIndex, const ushort_t& correctOption)
	{
		questions[questIndex].setCorrectOption(correctOption);
	}

	void RemoveQuestion(const ushort_t& questIndex)
	{
		questions.erase(questions.begin() + questIndex);
		numOfQuestions--;
	}

	void setName(const string& name)
	{
		this->name = name;
	}

	string getName() const
	{
		return name;
	}
	vector<ushort_t> getGradesHistory() const
	{
		return gradesHistory;
	}
	ushort_t getNumOfQuestions() const
	{
		return numOfQuestions;
	}
	vector<Question> getQuestions() const
	{
		return questions;
	}

};

class Category
{
	string name;
	vector<ushort_t> gradesHistory;
	vector<Test> tests;

public:

	Category(const string& name, const vector<ushort_t>& gradesHistory, const vector<Test>& tests)
	{
		this->name = name;
		this->gradesHistory = gradesHistory;
		this->tests = tests;
	}
	Category(const Category& category)
	{
		name = category.name;
		gradesHistory = category.gradesHistory;
		tests = category.tests;
	}

	void AddTest(const Test& test)
	{
		tests.push_back(test);
	}
	void AddQuestion(const ushort_t& testIndex, const Question& question)
	{
		tests[testIndex].AddQuestion(question);
	}
	void AddGrade(const ushort_t& grade)
	{
		gradesHistory.push_back(grade);
	}
	void AddTestGrade(const ushort_t& testIndex, const ushort_t& grade)
	{
		tests[testIndex].AddGrade(grade);
	}

	void ChangeTestName(const ushort_t& testIndex, const string& testName)
	{
		tests[testIndex].setName(testName);
	}
	void ChangeQuestionQuest(const ushort_t& testIndex, const ushort_t& questIndex,
		const string& quest)
	{
		tests[testIndex].ChangeQuestionQuest(questIndex, quest);
	}
	void ChangeQuestionAnswerOptions(const ushort_t& testIndex, const ushort_t& questIndex,
		const vector<string>& answerOptions)
	{
		tests[testIndex].ChangeQuestionAnswerOptions(questIndex, answerOptions);
	}
	void ChangeQuestionCorrectOption(const ushort_t& testIndex, const ushort_t& questIndex, const ushort_t& correctOption)
	{
		tests[testIndex].ChangeQuestionCorrectOption(questIndex, correctOption);
	}

	void RemoveTest(const ushort_t& index)
	{
		tests.erase(tests.begin() + index);
	}
	void RemoveQuestion(const ushort_t& testIndex, const ushort_t& questIndex)
	{
		tests[testIndex].RemoveQuestion(questIndex);
	}

	void setName(const string& name)
	{
		this->name = name;
	}

	string getName() const
	{
		return name;
	}
	vector<ushort_t> getGradesHistory() const
	{
		return gradesHistory;
	}
	vector<Test> getTests() const
	{
		return tests;
	}

};

class Statistics
{
	string testName;
	vector<ushort_t> grades;

public:

	Statistics(const string& testName, const vector<ushort_t>& grades)
	{
		this->testName = testName;
		this->grades = grades;
	}
	Statistics(const Statistics& statistics)
	{
		testName = statistics.testName;
		grades = statistics.grades;
	}

	void AddGrade(const ushort_t& grade)
	{
		grades.push_back(grade);
	}

	string getTestName() const
	{
		return testName;
	}
	vector<ushort_t> getGrades() const
	{
		return grades;
	}

};

class IncompleteTest
{
	string categoryName;
	string testName;
	ushort_t lastQuestionNum;
	ushort_t numOfCorrectAnswers;

public:

	IncompleteTest(const string& categoryName, const string& testName, const ushort_t& lastQuestionNum, const ushort_t& numOfCorrectAnswers)
	{
		this->categoryName = categoryName;
		this->testName = testName;
		this->lastQuestionNum = lastQuestionNum;
		this->numOfCorrectAnswers = numOfCorrectAnswers;
	}
	IncompleteTest(const IncompleteTest& activeTest)
	{
		categoryName = activeTest.categoryName;
		testName = activeTest.testName;
		lastQuestionNum = activeTest.lastQuestionNum;
		numOfCorrectAnswers = activeTest.numOfCorrectAnswers;
	}

	string getCategoryName() const
	{
		return categoryName;
	}
	string getTestName() const
	{
		return testName;
	}
	ushort_t getLastQuestionNum() const
	{
		return lastQuestionNum;
	}
	ushort_t getNumOfCorrectAnswers() const
	{
		return numOfCorrectAnswers;
	}

};

class User
{
	friend class Database;
	string username;
	string usernameKey;
	string password;
	string passwordKey;
	string fullname;
	string phoneNumber;
	string homeAddress;
	vector<IncompleteTest> incompleteTests;
	vector<Statistics> testResults;
	bool isAuthorized;

	//����������� ������������� ��� ������ LoadUsers() �� Database
	User(const string& username, const string& usernameKey, const string& password, const string& passwordKey,
		const string& fullname, const string& phoneNumber, const string& homeAddress,
		const vector<IncompleteTest>& incompleteTests, const vector<Statistics>& testResults)
	{
		this->username = username;
		this->usernameKey = usernameKey;
		this->password = password;
		this->passwordKey = passwordKey;
		this->fullname = fullname;
		this->phoneNumber = phoneNumber;
		this->homeAddress = homeAddress;
		this->incompleteTests = incompleteTests;
		this->testResults = testResults;
		isAuthorized = false;
	}

public:

	User(const string& username, const string& password, const string& fullname, const string& phoneNumber, const string& homeAddress)
	{
		this->fullname = fullname;
		this->phoneNumber = phoneNumber;
		this->homeAddress = homeAddress;
		this->username = Encrypt(username, usernameKey);
		this->password = Encrypt(password, passwordKey);
		isAuthorized = false;
	}
	User(const string& username, const string& password, const string& fullname, const string& phoneNumber, const string& homeAddress,
		const vector<Statistics>& testResults, const vector<IncompleteTest>& incompleteTests)
		: User(username, password, fullname, phoneNumber, homeAddress)
	{
		this->testResults = testResults;
		this->incompleteTests = incompleteTests;
	}
	User(const User& user)
	{
		fullname = user.fullname;
		phoneNumber = user.phoneNumber;
		homeAddress = user.homeAddress;
		username = user.username;
		usernameKey = user.usernameKey;
		password = user.password;
		passwordKey = user.passwordKey;
		testResults = user.testResults;
		incompleteTests = user.incompleteTests;
		isAuthorized = false;
	}

	bool Authorize(const string& username, const string& password, User*& currentUser)
	{
		if (Decrypt(this->username, usernameKey) != username || Decrypt(this->password, passwordKey) != password)
			return false;

		currentUser = this;
		return (isAuthorized = true);
	}

	void ShowTestResults() const
	{
		if (testResults.empty())
		{
			cout << "\n������ (���������� ���������� ���������� ������������, ��������� �� ��� �� ��������� �� ������ �����)\n\n";
			system("pause");
			return;
		}

		cout << "\n���������� ������������:\n";
		for (ushort_t i = 0; i < testResults.size(); i++)
		{
			cout << "\n����: " << testResults[i].getTestName() << "\n\t������: ";
			for (ushort_t j = 0; j < testResults[i].getGrades().size(); j++)
				cout << testResults[i].getGrades()[j] << (j + 1 < testResults[i].getGrades().size() ? ", " : "");
		}

		cout << endl << endl;
		system("pause");
	}

	bool IsAuthorized() const
	{
		return isAuthorized;
	}

	string getFullname() const
	{
		return fullname;
	}
	string getPhoneNumber() const
	{
		return phoneNumber;
	}
	string getHomeAddress() const
	{
		return homeAddress;
	}

};

class Admin
{
	friend class Database;
	string username;
	string usernameKey;
	string password;
	string passwordKey;
	bool isRegistered;
	bool isAuthorized;

	//����� ������������� ��� ������ LoadAdmin() �� Database
	void RegisterWithoutEncrypt(const string& username, const string& usernameKey, const string& password, const string& passwordKey)
	{
		this->username = username;
		this->usernameKey = usernameKey;
		this->password = password;
		this->passwordKey = passwordKey;
		isRegistered = true;
		isAuthorized = false;
	}

public:

	Admin() : username(""), usernameKey(""), password(""), passwordKey(""), isRegistered(false), isAuthorized(false) {};
	Admin(const Admin& admin)
	{
		username = admin.username;
		usernameKey = admin.usernameKey;
		password = admin.password;
		passwordKey = admin.passwordKey;
		isRegistered = admin.isRegistered;
		isAuthorized = admin.isAuthorized;
	}

	void Register(const string& username, const string& password)
	{
		try
		{
			if (isRegistered)
				throw Exception("����� �� �����-�� ������� ��� ��������������� (��������, ����� ������ �� ������������ ����� ���������)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		this->username = Encrypt(username, usernameKey);
		this->password = Encrypt(password, passwordKey);
		isRegistered = true;
	}
	bool Authorize(const string& username, const string& password)
	{
		try
		{
			if (!isRegistered)
				throw Exception("����� �� �����-�� ������� �� ��������������� (��������, ����� ������ �� ������������ ����� ���������)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		if (Decrypt(this->username, usernameKey) != username || Decrypt(this->password, passwordKey) != password)
		{
			cout << "\n������ ����������� (������������ ��� ������������ ��� ������)\n\n";
			return false;
		}

		return (isAuthorized = true);
	}

	void ChangeUsername(const string& username)
	{
		try
		{
			if (!isAuthorized)
				throw Exception("����� �� �����-�� ������� �� ����������� (��������, ����� ������ �� ������������ ����� ���������)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		this->username = Encrypt(username, usernameKey);
		cout << "\n��� ������������ ������� ��������\n\n";
		system("pause");
	}

	void ChangePassword(const string& password)
	{
		try
		{
			if (!isAuthorized)
				throw Exception("����� �� �����-�� ������� �� ����������� (��������, ����� ������ �� ������������ ����� ���������)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		this->password = Encrypt(password, passwordKey);
		cout << "\n������ ������� ������\n\n";
		system("pause");
	}

	bool IsRegistered() const
	{
		return isRegistered;
	}
	bool IsAuthorized() const
	{
		return isAuthorized;
	}

};

class Database
{
	string pathToUsers;
	string pathToAdmin;
	string pathToCategories;
	vector<User> users;
	Admin admin;
	vector<Category> categories;
	User* currentUser;

	bool CheckPasswordUniqueness(const string& password)
	{
		if (password.length() < 8)
			return false;

		bool hasDigits = false;
		bool hasLowercase = false;
		bool hasUppercase = false;
		bool hasUniqueSymbols = false;

		for (size_t i = 0; i < password.size(); i++)
		{
			if (IsCyrillic(password[i]))
				return false;

			if (IsDigit(password[i]))
				hasDigits = true;
			else if (IsLower(password[i]))
				hasLowercase = true;
			else if (IsUpper(password[i]))
				hasUppercase = true;
			else
				hasUniqueSymbols = true;

		}
		if (!hasDigits || !hasLowercase || !hasUppercase || !hasUniqueSymbols)
			return false;

		vector<string> nonUniqueTokens
		{
			"pass",
			"qwe",
			"abc",
			"asd",
			"999",
			"888",
			"777",
			"666",
			"555",
			"444",
			"333",
			"222",
			"111",
			"000",
			"123",
			"456",
			"987"
		};

		for (size_t i = 0; i < nonUniqueTokens.size(); i++)
		{
			if (password.find(nonUniqueTokens[i].c_str()) != string::npos)
				return false;
		}

		return true;
	}

	bool CheckUsernameUniqueness(const string& username, const vector<User>& users)
	{
		if (username.size() < 3)
			return false;

		for (size_t i = 0; i < username.size(); i++)
		{
			if (!((IsDigit(username[i]) || (IsUpper(username[i])) || (IsLower(username[i])) 
				|| username[i] == '-' || username[i] == '_' || username[i] == '~')))
				return false;
		}

		for (size_t i = 0; i < users.size(); i++)
		{
			if (Decrypt(users[i].username, users[i].usernameKey) == username)
				return false;
		}

		return true;
	}

public:

	Database(const string& pathToUsers = "users.txt", const string& pathToAdmin = "admin.txt",
		const string& pathToCategories = "categories.txt")
	{
		this->pathToUsers = pathToUsers;
		this->pathToAdmin = pathToAdmin;
		this->pathToCategories = pathToCategories;
		currentUser = nullptr;
	}

	void ShowUser(const ushort_t& userIndex) const
	{
		if (users.empty())
		{
			cout << "\n������ (�� ������ ������ �� ���������������� �� ������ ������������)\n\n";
			system("pause");
			return;
		}

		cout << "\n��� ������������: " << Decrypt(users[userIndex].username, users[userIndex].usernameKey)
			<< "\n������: " << Decrypt(users[userIndex].password, users[userIndex].passwordKey)
			<< "\n���: " << users[userIndex].fullname << "\n����� ��������: " << users[userIndex].phoneNumber
			<< "\n�������� �����: " << users[userIndex].homeAddress;
	}
	void ShowUser() const
	{
		if (users.empty())
		{
			cout << "\n������ (�� ������ ������ �� ���������������� �� ������ ������������)\n\n";
			return;
		}

		cout << "\n������������:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\n�������� ������������ (1-" << users.size() << "): ";
			cin >> userIndex;
		}
		userIndex--;
		system("cls");

		ShowUser(userIndex);
	}

	void ShowUsers() const
	{
		if (users.empty())
		{
			cout << "\n������ (�� ������ ������ �� ���������������� �� ������ ������������)\n\n";
			system("pause");
			return;
		}

		for (ushort_t i = 0; i < users.size(); i++)
		{
			cout << "������������ #" << i + 1 << ':';
			ShowUser(i);
			cout << endl << endl;
		}

		system("pause");
	}

	void SaveUsers() const
	{
		ofstream ofs(pathToUsers);
		try
		{
			if (!ofs.is_open())
				throw FailedOpeningException();
		}
		catch (const FailedOpeningException& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		for (ushort_t i = 0; i < users.size(); i++)
		{
			ofs << "\nUsername: " << users[i].username << "\nUsernameKey: " << users[i].usernameKey
				<< "\nPassword: " << users[i].password << "\nPasswordKey: " << users[i].passwordKey
				<< "\nFullname: " << users[i].fullname << "\nHomeAddress: " << users[i].homeAddress
				<< "\nPhoneNumber: " << users[i].phoneNumber << "\nIncompleteTests: ";
			for (ushort_t j = 0; j < users[i].incompleteTests.size(); j++)
			{
				ofs << '(' << users[i].incompleteTests[j].getCategoryName() << ',' << users[i].incompleteTests[j].getTestName()
					<< ',' << users[i].incompleteTests[j].getLastQuestionNum() << ','
					<< users[i].incompleteTests[j].getNumOfCorrectAnswers() << ')';
			}

			ofs << "\nTestResults: ";
			for (ushort_t j = 0; j < users[i].testResults.size(); j++)
			{
				ofs << '(' << users[i].testResults[j].getTestName() << ',';
				for (ushort_t k = 0; k < users[i].testResults[j].getGrades().size(); k++)
					ofs << users[i].testResults[j].getGrades()[k] << ',';

				ofs << ')';
			}
		}

		ofs.close();
	}

	void SaveAdmin() const
	{
		if (!admin.IsRegistered())
			return;

		ofstream ofs(pathToAdmin);
		try
		{
			if (!ofs.is_open())
				throw FailedOpeningException();
		}
		catch (const FailedOpeningException& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		ofs << "Username: " << admin.username << "\nUsernameKey: " << admin.usernameKey
			<< "\nPassword: " << admin.password << "\nPasswordKey: " << admin.passwordKey;

		ofs.close();
	}

	void SaveCategories(const string& path) const
	{
		ofstream ofs(path);
		try
		{
			if (!ofs.is_open())
				throw FailedOpeningException();
		}
		catch (const FailedOpeningException& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		for (ushort_t i = 0; i < categories.size(); i++)
		{
			ofs << "\nCategory: " << categories[i].getName() << "\nGrades: ";
			for (ushort_t j = 0; j < categories[i].getGradesHistory().size(); j++)
				ofs << categories[i].getGradesHistory()[j] << ',';

			ofs << "\nTests: ";
			for (ushort_t j = 0; j < categories[i].getTests().size(); j++)
			{
				ofs << '(' << categories[i].getTests()[j].getName() << ',';
				for (ushort_t k = 0; k < categories[i].getTests()[j].getGradesHistory().size(); k++)
					ofs << '[' << categories[i].getTests()[j].getGradesHistory()[k] << ']';
				ofs << ',' << categories[i].getTests()[j].getNumOfQuestions() << ',';
				for (ushort_t k = 0; k < categories[i].getTests()[j].getQuestions().size(); k++)
				{
					ofs << '<' << categories[i].getTests()[j].getQuestions()[k].getQuest();
					for (ushort_t l = 0; l < categories[i].getTests()[j].getQuestions()[k].getAnswerOptions().size(); l++)
						ofs << '{' << categories[i].getTests()[j].getQuestions()[k].getAnswerOptions()[l] << '}';
					ofs << ',' << categories[i].getTests()[j].getQuestions()[k].getCorrectOption() << '>';
				}

				ofs << ')';
			}
		}

		ofs.close();
	}

	void Save() const
	{
		SaveUsers();
		SaveAdmin();
		SaveCategories(pathToCategories);
	}

	void LoadUsers()
	{
		ifstream ifs(pathToUsers, ios::app);
		try
		{
			if (!ifs.is_open())
				throw FailedOpeningException();
		}
		catch (const FailedOpeningException& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}
		users.clear();

		string username = "";
		string usernameKey = "";
		string password = "";
		string passwordKey = "";
		string fullname = "";
		string homeAddress = "";
		string phoneNumber = "";
		vector<Statistics> testResults{};
		vector<IncompleteTest> incompleteTests{};

		string buf = "";
		bool isIncompleteTestsParsed = false;
		bool isTestResultsParsed = false;

		while (getline(ifs, buf))
		{
			if (buf.find("Username: ") != string::npos)
				username = buf.substr(10);

			else if (buf.find("UsernameKey: ") != string::npos)
				usernameKey = buf.substr(13);

			else if (buf.find("Password: ") != string::npos)
				password = buf.substr(10);

			else if (buf.find("PasswordKey: ") != string::npos)
				passwordKey = buf.substr(13);

			else if (buf.find("Fullname: ") != string::npos)
				fullname = buf.substr(10);

			else if (buf.find("HomeAddress: ") != string::npos)
				homeAddress = buf.substr(13);

			else if (buf.find("PhoneNumber: ") != string::npos)
				phoneNumber = buf.substr(13);

			else if (buf.find("IncompleteTests: ") != string::npos)
			{
				buf.erase(0, 17); //�������� "IncompleteTests: "

				while (buf.find('(') != string::npos)
				{
					string categoryName = "";
					string testName = "";
					ushort_t lastQuestNum{};
					ushort_t numOfCorrectAnswers{};

					buf.erase(0, 1); //�������� ����������� ������
					categoryName = buf.substr(0, buf.find(','));
					buf.erase(0, buf.find(',') + 1);
					testName = buf.substr(0, buf.find(','));
					buf.erase(0, buf.find(',') + 1);
					lastQuestNum = atoi(buf.substr(0, buf.find(',')).c_str());
					buf.erase(0, buf.find(',') + 1);
					numOfCorrectAnswers = atoi(buf.substr(0, buf.find(')')).c_str());
					buf.erase(0, buf.find(')') + 1);

					incompleteTests.push_back({ categoryName, testName, lastQuestNum, numOfCorrectAnswers });
				}

				isIncompleteTestsParsed = true;
			}

			else if (buf.find("TestResults: ") != string::npos)
			{
				buf.erase(0, 13); //�������� "TestResults: "

				while (buf.find('(') != string::npos)
				{
					string testName = "";
					vector<ushort_t> grades{};

					buf.erase(0, 1); //�������� ����������� ������
					testName = buf.substr(0, buf.find(','));
					buf.erase(0, buf.find(',') + 1);
					while (buf.find(',') < buf.find(')'))
					{
						grades.push_back(atoi(buf.substr(0, buf.find(',')).c_str()));
						buf.erase(0, buf.find(',') + 1);
					}
					buf.erase(0, 1); //�������� ����������� ������

					testResults.push_back({ testName, grades });
				}

				isTestResultsParsed = true;
			}

			if (!username.empty() && !usernameKey.empty() && !password.empty() && !passwordKey.empty() && !fullname.empty()
				&& !homeAddress.empty() && !phoneNumber.empty() && isIncompleteTestsParsed && isTestResultsParsed)
			{
				users.push_back({ username, usernameKey, password, passwordKey, fullname, phoneNumber,
					homeAddress, incompleteTests, testResults });

				username.clear();
				usernameKey.clear();
				password.clear();
				passwordKey.clear();
				fullname.clear();
				homeAddress.clear();
				phoneNumber.clear();
				incompleteTests.clear();
				testResults.clear();
				isIncompleteTestsParsed = false;
				isTestResultsParsed = false;
			}

		}

		ifs.close();
	}

	void LoadAdmin()
	{
		ifstream ifs(pathToAdmin, ios::app);
		try
		{
			if (!ifs.is_open())
				throw FailedOpeningException();
		}
		catch (const FailedOpeningException& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		string username = "";
		string usernameKey = "";
		string password = "";
		string passwordKey = "";

		string buf = "";
		while (getline(ifs, buf))
		{
			if (buf.find("Username: ") != string::npos)
				username = buf.substr(10);

			else if (buf.find("UsernameKey: ") != string::npos)
				usernameKey = buf.substr(13);

			else if (buf.find("Password: ") != string::npos)
				password = buf.substr(10);

			else if (buf.find("PasswordKey: ") != string::npos)
				passwordKey = buf.substr(13);

			if (!username.empty() && !usernameKey.empty() && !password.empty() && !passwordKey.empty())
				admin.RegisterWithoutEncrypt(username, usernameKey, password, passwordKey);

		}

		ifs.close();
	}

	void LoadCategories(const string& path)
	{
		ifstream ifs(path, ios::app);
		try
		{
			if (!ifs.is_open())
				throw FailedOpeningException();
		}
		catch (const FailedOpeningException& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}
		categories.clear();

		string name = "";
		vector<ushort_t> gradesHistory{};
		vector<Test> tests{};
		bool isGradesHistoryParsed = false;
		bool isTestsParsed = false;

		string buf = "";
		while (getline(ifs, buf))
		{
			if (buf.find("Category: ") != string::npos)
				name = buf.substr(10);

			else if (buf.find("Grades: ") != string::npos)
			{
				buf.erase(0, 8); //�������� "Grades: "
				while (buf.find(',') != string::npos)
				{
					gradesHistory.push_back(atoi(buf.substr(0, buf.find(',')).c_str()));
					buf.erase(0, buf.find(',') + 1);
				}

				isGradesHistoryParsed = true;
			}

			else if (buf.find("Tests: ") != string::npos)
			{
				buf.erase(0, 7); //�������� Tests: 
				while (buf.find('(') != string::npos)
				{
					string name = "";
					vector<ushort_t> gradesHistory{};
					short numOfQuestions{};
					vector<Question> questions{};

					buf.erase(0, 1); //�������� ����������� ������
					name = buf.substr(0, buf.find(','));
					buf.erase(0, buf.find(',') + 1);

					while (buf.find('[') < buf.find(','))
					{
						buf.erase(0, 1); //�������� ����������� ������
						gradesHistory.push_back(atoi(buf.substr(0, buf.find(']')).c_str()));
						buf.erase(0, buf.find(']') + 1);
					}

					buf.erase(0, 1); //�������� �������
					numOfQuestions = atoi(buf.substr(0, buf.find(',')).c_str());
					buf.erase(0, buf.find(',') + 1);

					while (buf.find('<') < buf.find(')'))
					{
						string quest = "";
						vector<string> answerOptions{};
						ushort_t correctOption{};

						buf.erase(0, 1); //�������� ����������� ������
						quest = buf.substr(0, buf.find('{'));
						buf.erase(0, buf.find('{'));

						while (buf.find('{') < buf.find(','))
						{
							buf.erase(0, 1); //�������� ����������� ������
							answerOptions.push_back({ buf.substr(0, buf.find('}')) });
							buf.erase(0, buf.find('}') + 1);
						}

						buf.erase(0, 1); //�������� �������
						correctOption = atoi(buf.substr(0, buf.find('>')).c_str());
						buf.erase(0, buf.find('>') + 1);
						questions.push_back({ quest, answerOptions, correctOption });
					}

					buf.erase(0, 1); //�������� ����������� ������
					tests.push_back({ name, gradesHistory, static_cast<ushort_t>(numOfQuestions), questions });
				}

				isTestsParsed = true;
			}

			if (!name.empty() && isGradesHistoryParsed && isTestsParsed)
			{
				categories.push_back({ name, gradesHistory, tests });

				name.clear();
				gradesHistory.clear();
				tests.clear();
				isGradesHistoryParsed = false;
				isTestsParsed = false;
			}

		}

		ifs.close();
	}

	void Load()
	{
		LoadUsers();
		LoadAdmin();
		LoadCategories(pathToCategories);
	}

	void RegisterUser(const string& username, const string& password, const string& fullname, const string& phoneNumber,
		const string& homeAddress)
	{
		users.push_back({ username, password, fullname, phoneNumber, homeAddress });
	}
	void RegisterUser()
	{
		string username = "";
		string password = "";
		string fullname = "";
		string phoneNumber = "";
		string homeAddress = "";

		while (true)
		{
			cout << "\n���������� ��� ������������: ";
			getline(cin, username);
			if (!CheckUsernameUniqueness(username, users))
			{
				cout << "\n������ �����������. ��� ������������ �� ������������� �������� ��������:"
					<< "\n\t1. ��� ������������ ������ ���� ������ �� ����� 3-� ��������"
					<< "\n\t2. ��� ������������ �� ������ ��������� ������� ��������, ����� A-Z, a-z, 0-9, -, _, ~"
					<< "\n\t3. ��� ������������ ������ ���� �� ������\n\n";

				system("pause");
				system("cls");
				continue;
			}

			cout << "\n���������� ������: ";
			getline(cin, password);
			if (!CheckPasswordUniqueness(password))
			{
				cout << "\n������ �����������. ������ �� ������������� �������� ��������:"
					<< "\n\t1. ������ �� ������ ���� ������ 8 ��������"
					<< "\n\t2. ������ ������ ��������� ����� ������ ���������� ��������"
					<< "\n\t3. ������ ������ ��������� �����, ����� (�������� � ���������), ���������� �������"
					<< "\n\t4. ������ ������ ���� ����������\n\n";

				system("pause");
				system("cls");
				continue;
			}

			break;
		}

		cout << "\n������� ��� ���: ";
		getline(cin, fullname);

		while (true)
		{
			system("cls");
			cout << "\n������� ����� �������� (� ������� \"+xyyyzzzzzzz\", ����������� ���������� ����: 9, "
				"������������ ���������� ����: 18)\n> ";
			getline(cin, phoneNumber);

			if (phoneNumber[0] != '+')
			{
				cout << "\n������ (�� ������� ������ \"+xyyyzzzzzzz\")\n\n";
				system("pause");
				continue;
			}
			else if (phoneNumber.size() > 19)
			{
				cout << "\n������ (���������� ���� ��������� 18)\n\n";
				system("pause");
				continue;
			}
			else if (phoneNumber.size() < 10)
			{
				cout << "\n������ (���������� ���� ������ 9)\n\n";
				system("pause");
				continue;
			}
			bool containsOnlyDigits = true;
			for (ushort_t i = 1; i < phoneNumber.size(); i++)
			{
				if (!IsDigit(phoneNumber[i]))
				{
					containsOnlyDigits = false;
					break;
				}
			}
			if (!containsOnlyDigits)
			{
				cout << "\n������ (� �������� ������ �������� ������������ �� ������ �����)\n\n";
				system("pause");
				continue;
			}

			break;
		}

		cout << "\n������� ���� �������� �����: ";
		getline(cin, homeAddress);

		RegisterUser(username, password, fullname, phoneNumber, homeAddress);
		cout << "\n������� ������� ���������������\n\n";
		system("pause");
	}
	void AddUser()
	{
		string username = "";
		string password = "";
		string fullname = "";
		string phoneNumber = "";
		string homeAddress = "";

		cout << "\n������� ��� ������������ (��� ����� ��������� ������ �� ���������������): ";
		getline(cin, username);
		for (ushort_t i = 0; i < users.size(); i++)
		{
			if (Decrypt(users[i].username, users[i].usernameKey) == username)
			{
				cout << "\n������ (���������� ���������� ��� ������������ ����� ���, ��������� ��� ��� ������)\n\n";
				system("pause");
				return;
			}
		}

		cout << "\n������� ������ (�� �� ����� �������� �� ������������): ";
		getline(cin, password);

		cout << "\n������� ��� ������������: ";
		getline(cin, fullname);

		cout << "\n������� ����� �������� (� ������� \"+xyyyzzzzzzz\", ����������� ���������� ����: 9, "
			"������������ ���������� ����: 18)\n> ";
		getline(cin, phoneNumber);

		if (phoneNumber[0] != '+')
		{
			cout << "\n������ (�� ������� ������ \"+xyyyzzzzzzz\")\n\n";
			system("pause");
			return;
		}
		else if (phoneNumber.size() > 19)
		{
			cout << "\n������ (���������� ���� ��������� 18)\n\n";
			system("pause");
			return;
		}
		else if (phoneNumber.size() < 10)
		{
			cout << "\n������ (���������� ���� ������ 9)\n\n";
			system("pause");
			return;
		}
		bool containsOnlyDigits = true;
		for (ushort_t i = 1; i < phoneNumber.size(); i++)
		{
			if (!IsDigit(phoneNumber[i]))
			{
				containsOnlyDigits = false;
				break;
			}
		}
		if (!containsOnlyDigits)
		{
			cout << "\n������ (� �������� ������ �������� ������������ �� ������ �����)\n\n";
			system("pause");
			return;
		}

		cout << "\n������� �������� ����� ������������: ";
		getline(cin, homeAddress);

		RegisterUser(username, password, fullname, phoneNumber, homeAddress);
		cout << "\n������������ ������� ��������\n\n";
		system("pause");
	}

	bool AuthorizeUser(const string& username, const string& password)
	{
		for (auto i = users.begin(); i != users.end(); advance(i, 1))
		{
			if (Decrypt(i->username, i->usernameKey) == username)
				return (i->Authorize(username, password, currentUser));
		}

		return false;
	}

	void EditUser()
	{
		if (users.empty())
		{
			cout << "\n������ (���������� ��������������� ������������, ��������� ��� �� ������ ������������)\n\n";
			system("pause");
			return;
		}

		cout << "\n������������:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\n�������� ������������, ������� ����� �������������� (1-" << users.size() << "): ";
			cin >> userIndex;
		}
		userIndex--;

		while (true)
		{
			system("cls");
			cout << "\n������� ������ ������������:\n";
			ShowUser(userIndex);
			cout << "\n\n1 - �������� ��� ������������\n2 - �������� ������\n3 - �������� ���\n4 - �������� ����� ��������"
				"\n5 - �������� �������� �����\n����� ������ ����� - ����� �� ��������������\n\n�������� ��������: ";
			ushort_t choice{};
			cin >> choice;
			system("cls");

			switch (choice)
			{
			case 1:
			{
				string username = "";
				cout << "\n������� ����� ��� ������������ (��� ����� ��������� ������ �� ���������������): ";
				cin.ignore();
				getline(cin, username);
				for (ushort_t i = 0; i < users.size(); i++)
				{
					if (Decrypt(users[i].username, users[i].usernameKey) == username)
					{
						cout << "\n������ (���������� ���������� ��� ������������ ����� ���, ��������� ��� ��� ������)\n\n";
						system("pause");
						return;
					}
				}

				cout << "\n��� ������������ ������� �������� � \"" << Decrypt(users[userIndex].username, users[userIndex].usernameKey)
					<< "\" �� \"" << username << "\"\n\n";
				users[userIndex].username = Encrypt(username, users[userIndex].usernameKey);
				system("pause");
				continue;
			}
			case 2:
			{
				string password = "";
				cout << "\n������� ����� ������ (�� �� ����� �������� �� ������������): ";
				cin.ignore();
				getline(cin, password);

				cout << "\n������ ������������ ������� ������ � \"" << Decrypt(users[userIndex].password, users[userIndex].passwordKey)
					<< "\" �� \"" << password << "\"\n\n";
				users[userIndex].password = Encrypt(password, users[userIndex].passwordKey);
				system("pause");
				continue;
			}
			case 3:
			{
				string fullname = "";
				cout << "\n������� ����� ���: ";
				cin.ignore();
				getline(cin, fullname);

				cout << "\n��� ������������ ������� �������� � \"" << users[userIndex].fullname << "\" �� \"" << fullname << "\"\n\n";
				users[userIndex].fullname = fullname;
				system("pause");
				continue;
			}
			case 4:
			{
				string phoneNumber = "";
				cout << "\n������� ����� �������� (� ������� \"+xyyyzzzzzzz\", ����������� ���������� ����: 9, "
					"������������ ���������� ����: 18)\n> ";
				cin.ignore();
				getline(cin, phoneNumber);

				if (phoneNumber[0] != '+')
				{
					cout << "\n������ (�� ������� ������ \"+xyyyzzzzzzz\")\n\n";
					system("pause");
					continue;
				}
				else if (phoneNumber.size() > 19)
				{
					cout << "\n������ (���������� ���� ��������� 18)\n\n";
					system("pause");
					continue;
				}
				else if (phoneNumber.size() < 10)
				{
					cout << "\n������ (���������� ���� ������ 9)\n\n";
					system("pause");
					continue;
				}
				bool containsOnlyDigits = true;
				for (ushort_t i = 1; i < phoneNumber.size(); i++)
				{
					if (!IsDigit(phoneNumber[i]))
					{
						containsOnlyDigits = false;
						break;
					}
				}
				if (!containsOnlyDigits)
				{
					cout << "\n������ (� �������� ������ �������� ������������ �� ������ �����)\n\n";
					system("pause");
					continue;
				}

				cout << "\n����� �������� ������� ������ � \"" << users[userIndex].phoneNumber << "\" �� \"" << phoneNumber << "\"\n\n";
				users[userIndex].phoneNumber = phoneNumber;
				system("pause");
				continue;
			}
			case 5:
			{
				string homeAddress = "";
				cout << "\n������� ����� �������� �����: ";
				cin.ignore();
				getline(cin, homeAddress);

				cout << "\n�������� ����� ������� ������ � \"" << users[userIndex].homeAddress << "\" �� \"" << homeAddress << "\"\n\n";
				users[userIndex].homeAddress = homeAddress;
				system("pause");
				continue;
			}
			default:
				return;
			}

		}

	}

	void RemoveUser()
	{
		if (users.empty())
		{
			cout << "\n������ (���������� ������� ������������, ��������� ��� �� ������ ������������)\n\n";
			system("pause");
			return;
		}

		cout << "\n������������:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\n�������� ������������, ������� ����� ����� (1-" << users.size() << "): ";
			cin >> userIndex;
		}
		userIndex--;
		system("cls");

		cout << "\n������������ \"" << Decrypt(users[userIndex].username, users[userIndex].usernameKey) << "\" ������� �����\n\n";
		users.erase(users.begin() + userIndex);
		system("pause");
	}

	void ShowCategories() const
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� ���������� ���������, ��������� ��� �� ����� ���������)\n\n";
			return;
		}

		cout << "\n���������:\n\n";
		for (ushort_t i = 0; i < categories.size(); i++)
			cout << i + 1 << ". " << categories[i].getName() << endl;
	}
	void ShowTests(const ushort_t& categoryIndex) const
	{
		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\n������ (���������� ���������� ����� ������ ���������, ��������� � ��������� ��� �� ������ �����)\n\n";
			return;
		}

		cout << "\n�����:\n\n";
		for (ushort_t i = 0; i < categories[categoryIndex].getTests().size(); i++)
			cout << i + 1 << ". " << categories[categoryIndex].getTests()[i].getName() << endl;
	}
	void ShowTests() const
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� ���������� ����� ������ ���������, ��������� ��� �� ����� ���������)\n\n";
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, ����� ������ ������� ����� ���������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		ShowTests(categoryIndex);
	}
	void ShowQuestions(const ushort_t& categoryIndex, const ushort_t& testIndex) const
	{
		if (categories[categoryIndex].getTests()[testIndex].getQuestions().empty())
		{
			cout << "\n������ (���������� ���������� ������� � �����, ��������� � ����� ��� ��������)\n\n";
			return;
		}

		cout << "\n�������:\n\n";
		for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions().size(); i++)
		{
			cout << i + 1 << ". " << categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getQuest()
				<< "\n�������� ������: ";
			for (ushort_t j = 0; j < categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getAnswerOptions().size(); j++)
				cout << "\n\t" << j + 1 << ") " << categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getAnswerOptions()[j];

			cout << "\n\t����� ����������� �������� ������: "
				<< categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getCorrectOption() + 1 << endl;
		}
	}
	void ShowQuestions() const
	{
		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, ������� � ����� ������ ������� ����� ���������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\n�������� ����, ������� � ������� ����� ���������� (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;
		system("cls");

		ShowQuestions(categoryIndex, testIndex);
	}

	void ShowCategoryGrades() const
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� ���������� ������ � ���������, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ��������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;

		auto categoryIt = categories.cbegin() + categoryIndex;
		if (categoryIt->getGradesHistory().empty())
		{
			cout << "\n������ (���������� ���������� ������ � ���������, ��������� � ��������� ��� ������)\n\n";
			system("pause");
			return;
		}

		cout << "\n������ � ��������� \"" << categoryIt->getName() << "\":\n";
		for (ushort_t i = 0; i < categoryIt->getGradesHistory().size(); i++)
			cout << categoryIt->getGradesHistory()[i] << (i + 1 < categoryIt->getGradesHistory().size() ? ", " : "");

		cout << endl << endl;
		system("pause");
	}
	void ShowTestGrades() const
	{
		if (categories.empty())
		{
			cout << "\n������ (��������� ���������� ������ � �����, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ��������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\n������ (���������� ���������� ������ � �����, ��������� � ��������� ��� ������)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\n�������� ���� (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		if (categories[categoryIndex].getTests()[testIndex].getGradesHistory().empty())
		{
			cout << "\n������ (���������� ���������� ������ � �����, ��������� � ����� ��� ������)\n\n";
			system("pause");
			return;
		}

		cout << "\n������ � ����� \"" << categories[categoryIndex].getTests()[testIndex].getName() << "\":\n";
		for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getGradesHistory().size(); i++)
			cout << categories[categoryIndex].getTests()[testIndex].getGradesHistory()[i]
			<< (i + 1 < categories[categoryIndex].getTests()[testIndex].getGradesHistory().size() ? ", " : "");

		cout << endl << endl;
		system("pause");
	}
	void ShowUserTestResults() const
	{
		if (users.empty())
		{
			cout << "\n������ (���������� ���������� ���������� ������ ������������, ��������� ��� �� ������ ������������)\n\n";
			system("pause");
			return;
		}

		cout << "\n������������:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\n�������� ������������, ������� ����� ����� (1-" << users.size() << "): ";
			cin >> userIndex;
		}
		userIndex--;
		system("cls");

		if (users[userIndex].testResults.empty())
		{
			cout << "\n������ (���������� ���������� ���������� ������ ������������, ��������� �� �� �������� �� ���� ����)\n\n";
			system("pause");
			return;
		}

		cout << "\n���������� ������ ������������ " << Decrypt(users[userIndex].username, users[userIndex].usernameKey) << ":\n";
		for (ushort_t i = 0; i < users[userIndex].testResults.size(); i++)
		{
			cout << "\n����: " << users[userIndex].testResults[i].getTestName() << "\n\t������: ";
			for (ushort_t j = 0; j < users[userIndex].testResults[i].getGrades().size(); j++)
				cout << users[userIndex].testResults[i].getGrades()[j]
				<< (j + 1 < users[userIndex].testResults[i].getGrades().size() ? ", " : "");
		}

		cout << endl << endl;
		system("pause");
	}

	void AddCategory()
	{
		string categoryName = "";
		vector<Test> tests{};

		cout << "\n������� �������� ���������: ";
		getline(cin, categoryName);
		for (ushort_t i = 0; i < categories.size(); i++)
		{
			if (categories[i].getName() == categoryName)
			{
				cout << "\n������ (���������� �������� ���������, ��������� ��������� � ����� ��������� ��� ����������)\n\n";
				system("pause");
				return;
			}
		}

		short numOfTests{};
		cout << "\n������� ���������� ������, ������� �� ������ ��������� ����� ������ (����� �� ����� ����� ��������): ";
		cin >> numOfTests;

		bool isAlreadyExist = false;
		for (ushort_t i = 0; i < numOfTests; i++)
		{
			system("cls");
			string testName = "";
			short numOfQuestions{};
			vector<Question> questions{};

			cout << "\n���������� " << i + 1 << "-�� �����\n\n������� �������� �����: ";
			cin.ignore();
			getline(cin, testName);

			for (ushort_t j = 0; j < tests.size(); j++)
			{
				if (tests[j].getName() == testName)
				{
					isAlreadyExist = true;
					break;
				}
			}
			if (isAlreadyExist)
			{
				cout << "\n������ (���������� �������� ���� � ���������, ��������� ���� � ����� ��������� ��� ����������)\n\n";
				isAlreadyExist = false;
				i--;
				system("pause");
				continue;
			}

			cout << "\n������� ���������� ��������, ������� �� ������ ��������� ����� ������ (����� �� ����� ����� ��������): ";
			cin >> numOfQuestions;
			if (numOfQuestions < 0)
				numOfQuestions = 0;

			for (ushort_t j = 0; j < numOfQuestions; j++)
			{
				system("cls");
				string quest = "";
				short numOfAnswerOptions{};
				vector<string> answerOptions{};
				ushort_t correctOption{};

				cout << "\n���������� " << j + 1 << "-�� �������\n\n������� ������: ";
				cin.ignore();
				getline(cin, quest);

				for (ushort_t k = 0; k < questions.size(); k++)
				{
					if (questions[k].getQuest() == quest)
					{
						isAlreadyExist = true;
						break;
					}
				}
				if (isAlreadyExist)
				{
					cout << "\n������ (���������� �������� ������, ��������� ����� ������ ��� ����������)\n\n";
					isAlreadyExist = false;
					j--;
					system("pause");
					continue;
				}

				while (numOfAnswerOptions < 2)
				{
					cout << "\n������� ���������� ��������� ������ (�������: 2): ";
					cin >> numOfAnswerOptions;
				}

				cin.ignore();
				for (ushort_t k = 0; k < numOfAnswerOptions; k++)
				{
					system("cls");
					string answerOption = "";
					cout << "\n������� " << k + 1 << " ������� ������: ";
					getline(cin, answerOption);

					for (ushort_t l = 0; l < answerOptions.size(); l++)
					{
						if (answerOptions[l] == answerOption)
						{
							isAlreadyExist = true;
							break;
						}
					}
					if (isAlreadyExist)
					{
						cout << "\n������ (���������� �������� ������� ������, ��������� ����� ������� ������ ��� ����������)\n\n";
						isAlreadyExist = false;
						k--;
						system("pause");
						continue;
					}

					answerOptions.push_back(answerOption);
				}
				system("cls");

				cout << "\n����������� �������� ������:\n\n";
				for (ushort_t i = 0; i < numOfAnswerOptions; i++)
					cout << i + 1 << ". " << answerOptions[i] << endl;

				while (correctOption < 1 || correctOption > numOfAnswerOptions)
				{
					cout << "\n������� ����� ����������� �������� ������ (1-" << numOfAnswerOptions << "): ";
					cin >> correctOption;
				}
				correctOption--;

				questions.push_back({ quest, answerOptions, correctOption });
			}

			tests.push_back({ testName, {}, static_cast<ushort_t>(numOfQuestions), questions });
		}
		system("cls");

		categories.push_back({ categoryName, {}, tests });
		cout << "\n��������� \"" << categoryName << "\" ������� ���������\n\n";
		system("pause");
	}

	void AddTest()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� �������� ����, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, � ������� ����� �������� ���� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		string testName = "";
		short numOfQuestions{};
		vector<Question> questions{};

		cout << "\n������� �������� �����: ";
		cin.ignore();
		getline(cin, testName);

		for (ushort_t i = 0; i < categories[categoryIndex].getTests().size(); i++)
		{
			if (categories[categoryIndex].getTests()[i].getName() == testName)
			{
				cout << "\n������ (���������� �������� ����, ��������� ���� � ����� ��������� ��� ����������)\n\n";
				system("pause");
				return;
			}
		}

		cout << "\n������� ���������� ��������, ������� �� ������ ��������� ����� ������ (����� �� ����� ����� ��������): ";
		cin >> numOfQuestions;
		if (numOfQuestions < 0)
			numOfQuestions = 0;

		bool isAlreadyExist = false;
		for (ushort_t i = 0; i < numOfQuestions; i++)
		{
			system("cls");
			string quest = "";
			short numOfAnswerOptions{};
			vector<string> answerOptions{};
			ushort_t correctOption{};

			cout << "\n���������� " << i + 1 << "-�� �������\n\n������� ������: ";
			cin.ignore();
			getline(cin, quest);

			for (ushort_t j = 0; j < questions.size(); j++)
			{
				if (questions[j].getQuest() == quest)
				{
					isAlreadyExist = true;
					break;
				}
			}
			if (isAlreadyExist)
			{
				cout << "\n������ (���������� �������� ������, ��������� ����� ������ ��� ����������)\n\n";
				isAlreadyExist = false;
				i--;
				system("pause");
				continue;
			}

			while (numOfAnswerOptions < 2)
			{
				cout << "\n������� ���������� ��������� ������ (�������: 2): ";
				cin >> numOfAnswerOptions;
			}

			cin.ignore();
			for (ushort_t j = 0; j < numOfAnswerOptions; j++)
			{
				system("cls");
				string answerOption = "";

				cout << "\n������� " << j + 1 << " ������� ������: ";
				getline(cin, answerOption);

				for (ushort_t k = 0; k < answerOptions.size(); k++)
				{
					if (answerOptions[k] == answerOption)
					{
						isAlreadyExist = true;
						break;
					}
				}
				if (isAlreadyExist)
				{
					cout << "\n������ (���������� �������� ������� ������, ��������� ����� ������� ������ ��� ����������)\n\n";
					isAlreadyExist = false;
					j--;
					system("pause");
					continue;
				}

				answerOptions.push_back(answerOption);
			}
			system("cls");

			cout << "\n����������� �������� ������:\n\n";
			for (ushort_t i = 0; i < numOfAnswerOptions; i++)
				cout << i + 1 << ". " << answerOptions[i] << endl;

			while (correctOption < 1 || correctOption > numOfAnswerOptions)
			{
				cout << "\n������� ����� ����������� �������� ������ (1-" << numOfAnswerOptions << "): ";
				cin >> correctOption;
			}
			correctOption--;

			questions.push_back({ quest, answerOptions, correctOption });
		}
		system("cls");

		categories[categoryIndex].AddTest({ testName, {}, static_cast<ushort_t>(numOfQuestions), questions });
		cout << "\n���� \"" << testName << "\" ������� �������� � ��������� \"" << categories[categoryIndex].getName() << "\"\n\n";
		system("pause");
	}

	void AddQuestion()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� �������� ������ � �����, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, � ���� ������� ����� �������� ������ (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\n������ (���������� �������� ������ � �����, ��������� � ��������� ��� ������)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\n�������� ����, � ������� ����� �������� ������ (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		system("cls");
		string quest = "";
		short numOfAnswerOptions{};
		vector<string> answerOptions{};
		ushort_t correctOption{};

		cout << "\n������� ������: ";
		cin.ignore();
		getline(cin, quest);

		for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions().size(); i++)
		{
			if (categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getQuest() == quest)
			{
				cout << "\n������ (���������� �������� ������, ��������� ����� ������ ��� ����������)\n\n";
				system("pause");
				return;
			}
		}

		while (numOfAnswerOptions < 2)
		{
			cout << "\n������� ���������� ��������� ������ (�������: 2): ";
			cin >> numOfAnswerOptions;
		}

		cin.ignore();
		bool isAlreadyExist = false;
		for (ushort_t i = 0; i < numOfAnswerOptions; i++)
		{
			system("cls");
			string answerOption = "";
			cout << "\n������� " << i + 1 << " ������� ������: ";
			getline(cin, answerOption);

			for (ushort_t j = 0; j < answerOptions.size(); j++)
			{
				if (answerOptions[j] == answerOption)
				{
					isAlreadyExist = true;
					break;
				}
			}
			if (isAlreadyExist)
			{
				cout << "\n������ (���������� �������� ������� ������, ��������� ����� ������� ������ ��� ����������)\n\n";
				isAlreadyExist = false;
				i--;
				system("pause");
				continue;
			}

			answerOptions.push_back(answerOption);
		}
		system("cls");

		cout << "\n����������� �������� ������:\n\n";
		for (ushort_t i = 0; i < numOfAnswerOptions; i++)
			cout << i + 1 << ". " << answerOptions[i] << endl;

		while (correctOption < 1 || correctOption > numOfAnswerOptions)
		{
			cout << "\n������� ����� ����������� �������� ������ (1-" << numOfAnswerOptions << "): ";
			cin >> correctOption;
		}
		correctOption--;

		categories[categoryIndex].AddQuestion(testIndex, { quest, answerOptions, correctOption });
		cout << "\n������ \"" << quest << "\" ������� �������� � ���� \"" << categories[categoryIndex].getTests()[testIndex].getName() << "\"\n\n";
		system("pause");
	}

	void ChangeCategoryName()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� �������� �������� ���������, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, �������� ������� ����� �������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		string categoryName = "";
		cout << "\n������� ����� �������� ���������: ";
		cin.ignore();
		getline(cin, categoryName);

		for (ushort_t i = 0; i < categories.size(); i++)
		{
			if (categories[i].getName() == categoryName)
			{
				cout << "\n������ (���������� �������� �������� ���������, ��������� ��������� � ����� ��������� ��� ����������)\n\n";
				system("pause");
				return;
			}
		}

		cout << "\n�������� ��������� \"" << categories[categoryIndex].getName() << "\" ������� �������� �� \"" << categoryName << "\"\n\n";
		categories[categoryIndex].setName(categoryName);
		system("pause");
	}

	void ChangeTestName()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� �������� �������� �����, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, �������� ����� ������ ������� ����� �������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\n������ (���������� �������� �������� �����, ��������� � ��������� ��� ������)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\n�������� ����, �������� �������� ����� �������� (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;
		system("cls");

		string testName = "";
		cout << "\n������� ����� �������� �����: ";
		cin.ignore();
		getline(cin, testName);

		for (ushort_t i = 0; i < categories[categoryIndex].getTests().size(); i++)
		{
			if (categories[categoryIndex].getTests()[i].getName() == testName)
			{
				cout << "\n������ (���������� �������� �������� �����, ��������� ���� � ����� ��������� ��� ����������)\n\n";
				system("pause");
				return;
			}
		}

		categories[categoryIndex].ChangeTestName(testIndex, testName);
		cout << "\n�������� ����� ������� �������� �� \"" << testName << "\"\n\n";
		system("pause");
	}

	void ChangeQuestion()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� �������� ������ � �����, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, ������ � ����� ������ ������� ����� ������ (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\n������ (���������� �������� ������ � �����, ��������� � ��������� ��� ������)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\n�������� ����, ������ � ������� ����� ������ (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;
		system("cls");

		if (categories[categoryIndex].getTests()[testIndex].getQuestions().empty())
		{
			cout << "\n������ (���������� �������� ������ � �����, ��������� � ����� ��� ��������)\n\n";
			system("pause");
			return;
		}

		ShowQuestions(categoryIndex, testIndex);
		ushort_t questIndex{};
		while (questIndex < 1 || questIndex > categories[categoryIndex].getTests()[testIndex].getQuestions().size())
		{
			cout << "\n������� ����� �������, ������� ����� ������ (1-"
				<< categories[categoryIndex].getTests()[testIndex].getQuestions().size() << "): ";
			cin >> questIndex;
		}
		questIndex--;

		while (true)
		{
			system("cls");
			cout << "\n������: " << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getQuest()
				<< "\n�������� ������:\n";
			for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions().size(); i++)
				cout << i + 1 << ". " << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions()[i] << endl;
			cout << "����� ����������� �������� ������: " << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getCorrectOption() + 1;

			cout << "\n\n1 - �������� ������\n2 - �������� �������� ������\n3 - �������� ����� ����������� �������� ������"
				"\n����� ������ ����� - ���������� ��������������\n\n�������� ��������: ";
			ushort_t choice{};
			cin >> choice;
			system("cls");

			switch (choice)
			{
			case 1:
			{
				string quest = "";
				cout << "\n������� ����� ������: ";
				cin.ignore();
				getline(cin, quest);

				for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions().size(); i++)
				{
					if (categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getQuest() == quest)
					{
						cout << "\n������ (���������� �������� ������, ��������� ����� ������ ��� ����������)\n\n";
						system("pause");
						continue;
					}
				}

				cout << "\n������ ������� ������ � \""
					<< categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getQuest() << "\" �� \""
					<< quest << "\"\n\n";
				categories[categoryIndex].ChangeQuestionQuest(testIndex, questIndex, quest);
				system("pause");
				continue;
			}
			case 2:
			{
				short numOfAnswerOptions{};
				vector<string> answerOptions{};

				while (numOfAnswerOptions < 2)
				{
					cout << "\n������� ���������� ��������� ������ (�������: 2): ";
					cin >> numOfAnswerOptions;
				}

				cin.ignore();
				bool isAlreadyExist = false;
				for (ushort_t i = 0; i < numOfAnswerOptions; i++)
				{
					system("cls");
					string answerOption = "";
					cout << "\n������� " << i + 1 << " ������� ������: ";
					getline(cin, answerOption);

					for (ushort_t j = 0; j < answerOptions.size(); j++)
					{
						if (answerOptions[j] == answerOption)
						{
							isAlreadyExist = true;
							break;
						}
					}
					if (isAlreadyExist)
					{
						cout << "\n������ (���������� �������� ������� ������, ��������� ����� ������� ������ ��� ����������)\n\n";
						isAlreadyExist = false;
						i--;
						system("pause");
						continue;
					}

					answerOptions.push_back(answerOption);
				}
				system("cls");

				categories[categoryIndex].ChangeQuestionAnswerOptions(testIndex, questIndex, answerOptions);
				cout << "\n�������� ������ ������� ��������\n\n";
				system("pause");
				continue;
			}
			case 3:
			{
				ushort_t correctOption{};

				while (correctOption < 1 || correctOption > categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions().size())
				{
					cout << "\n������� ����� ����������� ������ (1-"
						<< categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions().size() << "): ";
					cin >> correctOption;
				}

				cout << "\n����� ����������� �������� ������ ������� ������ � \""
					<< categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getCorrectOption() + 1 << "\" �� \""
					<< correctOption-- << "\"\n\n";
				categories[categoryIndex].ChangeQuestionCorrectOption(testIndex, questIndex, correctOption);
				system("pause");
				continue;
			}
			default:
				return;
			}

		}
	}

	void RemoveCategory()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� ������� ���������, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, ������� ����� ������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		cout << "\n��������� \"" << categories[categoryIndex].getName() << "\" ������� �������\n\n";
		categories.erase(categories.begin() + categoryIndex);
		system("pause");
	}

	void RemoveTest()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� ������� ����, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, ���� ������ ������� ����� ����� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\n������ (���������� ������� ����, ��������� � ��������� ��� ������)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\n�������� ����, ������� ����� ����� (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		cout << "\n���� \"" << categories[categoryIndex].getTests()[testIndex].getName() << "\" ������� �����\n\n";
		categories[categoryIndex].RemoveTest(testIndex);
		system("pause");
	}

	void RemoveQuestion()
	{
		if (categories.empty())
		{
			cout << "\n������ (���������� ������� ������ � �����, ��������� ��� �� ����� ���������)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\n�������� ���������, ������ � ����� ������ ������� ����� ����� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\n������ (���������� ������� ������ � �����, ��������� � ��������� ��� ������)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\n�������� ����, ������ � ������� ����� ����� (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		if (categories[categoryIndex].getTests()[testIndex].getQuestions().empty())
		{
			cout << "\n������ (���������� ������� ������ � �����, ��������� � ����� ��� ��������)\n\n";
			system("pause");
			return;
		}

		ShowQuestions(categoryIndex, testIndex);
		ushort_t questIndex{};
		while (questIndex < 1 || questIndex > categories[categoryIndex].getTests()[testIndex].getQuestions().size())
		{
			cout << "\n�������� ������, ������� ����� ����� (1-"
				<< categories[categoryIndex].getTests()[testIndex].getQuestions().size() << "): ";
			cin >> questIndex;
		}
		questIndex--;

		cout << "\n������ \"" << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getQuest()
			<< "\" ������� �����\n\n";
		categories[categoryIndex].RemoveQuestion(testIndex, questIndex);
		system("pause");
	}

	void StartTest()
	{
		try
		{
			if (currentUser == nullptr)
				throw Exception("������������ �� �����-�� ������� �� ����������� (��������, ����� ������ �� ������������ ����� ���������)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		if (categories.empty())
		{
			cout << "\n������ (���������� ����� ����, ��������� ��� �� ����� ���������)\n";
			system("pause");
			return;
		}

		ushort_t categoryIndex{};
		ushort_t testIndex{};

		ShowCategories();
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "�������� ��������� (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		auto categoryIt = categories.begin() + categoryIndex;
		system("cls");

		if (categoryIt->getTests().empty())
		{
			cout << "\n������ (���������� ����� ����, ��������� � ��������� ��� ������)\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		while (testIndex < 1 || testIndex > categoryIt->getTests().size())
		{
			cout << "�������� ���� (1-" << categoryIt->getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		if (categories[categoryIndex].getTests()[testIndex].getQuestions().empty())
		{
			cout << "\n������ (���������� ����� ����, ��������� � ����� ��� ��������)\n\n";
			system("pause");
			return;
		}

		vector<Test> bufTests = categoryIt->getTests();
		auto currentTest = bufTests.begin() + testIndex;
		system("cls");
		auto it = find_if(currentUser->incompleteTests.begin(), currentUser->incompleteTests.end(),
			[&](const IncompleteTest& incompleteTest) { return incompleteTest.getTestName() == currentTest->getName(); });

		ushort_t correctAnswersCounter{};
		ushort_t questionNumToStart{};
		if (it == currentUser->incompleteTests.end())
		{
			cout << "\n������ ����� \"" << currentTest->getName()
				<< "\"\n��� ���������������� ������, ������� � ���� ������ ������� /�����\n";
		}
		else
		{
			cout << "\n����������� ����� �������������� ������������ ����� \"" << currentTest->getName()
				<< "\"\n��� ���������������� ������, ������� � ���� ������ ������� /�����\n";
			correctAnswersCounter = it->getNumOfCorrectAnswers();
			questionNumToStart = it->getLastQuestionNum();
			currentUser->incompleteTests.erase(it);
		}
		system("pause");
		system("cls");
		cin.ignore();

		for (ushort_t i = questionNumToStart; i < currentTest->getNumOfQuestions(); i++)
		{
			system("cls");
			cout << i + 1 << ") " << currentTest->getQuestions()[i].getQuest() << "\n�������� ������:\n\n";
			for (ushort_t j = 0; j < currentTest->getQuestions()[i].getAnswerOptions().size(); j++)
				cout << j + 1 << ". " << currentTest->getQuestions()[i].getAnswerOptions()[j] << endl;

			cout << "\n�������� ����� ������ (1-" << currentTest->getQuestions()[i].getAnswerOptions().size() << "): ";
			string selectedOption = "";
			getline(cin, selectedOption);
			if (selectedOption == "/�����")
			{
				currentUser->incompleteTests.push_back({ categoryIt->getName(), currentTest->getName(), i, correctAnswersCounter });
				return;
			}
			else if ((atoi(selectedOption.c_str()) - 1) == currentTest->getQuestions()[i].getCorrectOption())
			{
				correctAnswersCounter++;
				cout << "\n���������� �����!\n\n";
			}
			else
				cout << "\n������������ �����!\n\n";
			system("pause");
		}

		system("cls");
		ushort_t grade = static_cast<ushort_t>(correctAnswersCounter * (12.0 / currentTest->getNumOfQuestions()));
		cout << "\n���� ��������.\n���������� ���������� �������: " << correctAnswersCounter
			 << "\n������� ���������� �������: " << (int)((double)correctAnswersCounter / currentTest->getNumOfQuestions() * 100.0) << '%';
		cout << "\n������: " << grade << ' ' << (grade == 1 ? "����" : grade >= 2 && grade <= 4 ? "�����" : "������")
			 << " �� 12 ���������\n\n";

		//���������� ���������� � ������������
		auto statIt = find_if(currentUser->testResults.begin(), currentUser->testResults.end(),
			[&](const Statistics& statistics) { return statistics.getTestName() == currentTest->getName(); });
		if (statIt != currentUser->testResults.end())
			statIt->AddGrade(grade);
		else
			currentUser->testResults.push_back({ currentTest->getName(), { grade } });

		//���������� ���������� � ���������
		categoryIt->AddGrade(grade);
		//���������� ���������� � �����
		categoryIt->AddTestGrade(testIndex, grade);

		system("pause");
	}

	void ImportCategoriesFromFile()
	{
		string path = "";
		cout << "\n������� ������ ���� � .txt �����, �� �������� ����� ������������� ���������"
			"\n��������: D:\\Documents\\backup.txt\n> ";
		getline(cin, path);

		LoadCategories(path);
		cout << "\n��������� ������� ������������� �� ����� " << path << endl << endl;
		system("pause");
	}

	void ExportCategoriesInFile()
	{
		string path = "";
		cout << "\n������� ������ ���� � .txt �����, � ������� ����� �������������� ���������"
			"\n��������: D:\\Documents\\backup.txt\n> ";
		getline(cin, path);

		SaveCategories(path);
		cout << "\n��������� ������� �������������� � ���� " << path << endl << endl;
		system("pause");
	}

	User* getCurrentUser()
	{
		return currentUser;
	}

	Admin* getAdmin()
	{
		return &admin;
	}

};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Database database;
	database.Load();

	while (true)
	{
		system("cls");
		cout << "\n1 - ������������\n2 - �����\n����� ������ ����� - ����� �� ���������\n\n�������� �����: ";
		ushort_t choice{};
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			while (true)
			{
				system("cls");
				cout << "\n1 - ������������������\n2 - ��������������\n����� ������ ����� - ����� �� ���������\n\n�������� ��������: ";
				cin >> choice;
				system("cls");

				switch (choice)
				{
				case 1:
				{
					cin.ignore();
					database.RegisterUser();
					continue;
				}
				case 2:
				{
					string username = "";
					string password = "";

					cout << "\n������� ��� ������������: ";
					cin.ignore();
					getline(cin, username);
					cout << "������� ������: ";
					getline(cin, password);

					if (database.AuthorizeUser(username, password))
					{
						cout << "\n�� ������� ������������\n\n";
						system("pause");

						while (true)
						{
							system("cls");
							cout << "\n1 - ���������� ���� ���������� ������������\n2 - ����� ����� ������������"
								"\n����� ������ ����� - ����� �� ���������\n\n�������� ��������: ";
							cin >> choice;
							system("cls");

							switch (choice)
							{
							case 1:
							{
								database.getCurrentUser()->ShowTestResults();
								continue;
							}
							case 2:
							{
								database.StartTest();
								continue;
							}
							default:
							{
								database.Save();
								return 0;
							}
							}

						}

						continue;
					}
					else
					{
						cout << "\n������ ����������� (������������ ��� ������������ ��� ������)\n\n";
						system("pause");
						continue;
					}
				}
				default:
				{
					database.Save();
					return 0;
				}
				}
			}
		}
		case 2:
		{
			cin.ignore();
			while (true)
			{
				system("cls");
				if (database.getAdmin()->IsRegistered())
				{
					string username = "";
					string password = "";

					cout << "\n�����������:\n\n������� ��� ������������: ";
					getline(cin, username);

					cout << "������� ������: ";
					getline(cin, password);

					if (database.getAdmin()->Authorize(username, password))
					{
						cout << "\n�� ������� ������������\n\n";
						system("pause");

						while (true)
						{
							system("cls");
							cout << "\n---������� ������---\n1.  �������� ��� ��� ������������\n2.  �������� ���� ������"
								 << "\n\n---������������---\n3.  ���������� ������� ���� �������������"
								 << "\n4.  ���������� ������� ������������\n5.  �������� ������������"
								 << "\n6.  ������������� ������������\n7.  ������� ������������"
								 << "\n\n---���������� ������������---\n8.  ���������� ���������� ������������ � ���������"
								 << "\n9.  ���������� ���������� ������������ � �����"
								 << "\n10. ���������� ���������� ������������ � ������������"
								 << "\n\n---���������---\n11. ���������� ��� ���������"
								 << "\n12. �������� ���������\n13. �������� �������� ���������\n14. ������� ���������"
								 << "\n\n---�����---\n15. ���������� ��� ����� � ���������\n16. �������� ����"
							  	 << "\n17. �������� �������� �����\n18. ������� ����\n\n---�������---"
								 << "\n19. ���������� ��� ������� � �����\n20. �������� ������ � �����"
								 << "\n21. �������� ������ � �����\n22. ������� ������ � �����"
								 << "\n\n---������ � ������� ��������� � ������---"
								 << "\n23. ������������� ��������� � ����� �� �����"
								 << "\n24. �������������� ��������� � ����� � ����"
								 << "\n\n---�����---\n����� ������ �����\n\n�������� ��������: ";
							cin >> choice;
							system("cls");

							switch (choice)
							{
							case 1:
							{
								string username = "";
								cout << "\n������� ����� ��� ������������: ";
								cin.ignore();
								getline(cin, username);

								database.getAdmin()->ChangeUsername(username);
								continue;
							}
							case 2:
							{
								string password = "";
								cout << "\n������� ����� ������ (�� �� ����� �������� �� ������������): ";
								cin.ignore();
								getline(cin, password);

								database.getAdmin()->ChangePassword(password);
								continue;
							}
							case 3:
							{
								database.ShowUsers();
								continue;
							}
							case 4:
							{
								database.ShowUser();
								cout << endl << endl;
								system("pause");
								continue;
							}
							case 5:
							{
								cin.ignore();
								database.AddUser();
								continue;
							}
							case 6:
							{
								database.EditUser();
								continue;
							}
							case 7:
							{
								database.RemoveUser();
								continue;
							}
							case 8:
							{
								database.ShowCategoryGrades();
								continue;
							}
							case 9:
							{
								database.ShowTestGrades();
								continue;
							}
							case 10:
							{
								database.ShowUserTestResults();
								continue;
							}
							case 11:
							{
								database.ShowCategories();
								cout << endl;
								system("pause");
								continue;
							}
							case 12:
							{
								cin.ignore();
								database.AddCategory();
								continue;
							}
							case 13:
							{
								database.ChangeCategoryName();
								continue;
							}
							case 14:
							{
								database.RemoveCategory();
								continue;
							}
							case 15:
							{
								database.ShowTests();
								cout << endl;
								system("pause");
								continue;
							}
							case 16:
							{
								database.AddTest();
								continue;
							}
							case 17:
							{
								database.ChangeTestName();
								continue;
							}
							case 18:
							{
								database.RemoveTest();
								continue;
							}
							case 19:
							{
								database.ShowQuestions();
								cout << endl;
								system("pause");
								continue;
							}
							case 20:
							{
								database.AddQuestion();
								continue;
							}
							case 21:
							{
								database.ChangeQuestion();
								continue;
							}
							case 22:
							{
								database.RemoveQuestion();
								continue;
							}
							case 23:
							{
								cin.ignore();
								database.ImportCategoriesFromFile();
								continue;
							}
							case 24:
							{
								cin.ignore();
								database.ExportCategoriesInFile();
								continue;
							}
							default:
							{
								database.Save();
								return 0;
							}
							}
						}
					}
					else
					{
						system("pause");
						continue;
					}
				}
				else
				{
					string username = "";
					string password = "";

					cout << "\n�����������:\n\n���������� ��� ������������: ";
					getline(cin, username);
					cout << "���������� ������ (�� �� ����� �������� �� ������������): ";
					getline(cin, password);

					database.getAdmin()->Register(username, password);
					cout << "\n�� ������� ����������������\n\n";
					system("pause");
					continue;
				}
			}
		}
		default:
		{
			database.Save();
			return 0;
		}
		}
	}
}