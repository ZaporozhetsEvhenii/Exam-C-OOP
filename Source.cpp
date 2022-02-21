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
	return ((c >= 'а' && c <= 'я') || (c >= 'a' && c <= 'z'));
}

bool IsUpper(const char& c)
{
	return ((c >= 'А' && c <= 'Я') || (c >= 'A' && c <= 'Z'));
}

bool IsCyrillic(const char& c)
{
	return ((c >= 'А' && c <= 'Я') || (c >= 'а' && c <= 'я'));
}

class Exception
{
	string msg;
	int errorCode;

public:

	Exception(const char* msg, const int& errorCode)
	{
		this->msg = "Ошибка: ";
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

	FailedOpeningException(const char* msg = "Не удалось открыть файл", const int& errorCode = -5)
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
				throw Exception("Некорректные данные в конструкторе теста (количество вопросов не совпадает с размером вектора вопросов)", -20);
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

	//Конструктор исключительно для метода LoadUsers() из Database
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
			cout << "\nОшибка (невозможно посмотреть результаты тестирований, поскольку Вы ещё не завершили ни одного теста)\n\n";
			system("pause");
			return;
		}

		cout << "\nРезультаты тестирований:\n";
		for (ushort_t i = 0; i < testResults.size(); i++)
		{
			cout << "\nТест: " << testResults[i].getTestName() << "\n\tОценки: ";
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

	//Метод исключительно для метода LoadAdmin() из Database
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
				throw Exception("Админ по какой-то причине уже зарегистрирован (возможно, метод вызван из неправильной части программы)", -20);
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
				throw Exception("Админ по какой-то причине не зарегистрирован (возможно, метод вызван из неправильной части программы)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		if (Decrypt(this->username, usernameKey) != username || Decrypt(this->password, passwordKey) != password)
		{
			cout << "\nОшибка авторизации (неправильное имя пользователя или пароль)\n\n";
			return false;
		}

		return (isAuthorized = true);
	}

	void ChangeUsername(const string& username)
	{
		try
		{
			if (!isAuthorized)
				throw Exception("Админ по какой-то причине не авторизован (возможно, метод вызван из неправильной части программы)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		this->username = Encrypt(username, usernameKey);
		cout << "\nИмя пользователя успешно изменено\n\n";
		system("pause");
	}

	void ChangePassword(const string& password)
	{
		try
		{
			if (!isAuthorized)
				throw Exception("Админ по какой-то причине не авторизован (возможно, метод вызван из неправильной части программы)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		this->password = Encrypt(password, passwordKey);
		cout << "\nПароль успешно изменён\n\n";
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
			cout << "\nОшибка (на данный момент не зарегистрировано ни одного пользователя)\n\n";
			system("pause");
			return;
		}

		cout << "\nИмя пользователя: " << Decrypt(users[userIndex].username, users[userIndex].usernameKey)
			<< "\nПароль: " << Decrypt(users[userIndex].password, users[userIndex].passwordKey)
			<< "\nФИО: " << users[userIndex].fullname << "\nНомер телефона: " << users[userIndex].phoneNumber
			<< "\nДомашний адрес: " << users[userIndex].homeAddress;
	}
	void ShowUser() const
	{
		if (users.empty())
		{
			cout << "\nОшибка (на данный момент не зарегистрировано ни одного пользователя)\n\n";
			return;
		}

		cout << "\nПользователи:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\nВыберите пользователя (1-" << users.size() << "): ";
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
			cout << "\nОшибка (на данный момент не зарегистрировано ни одного пользователя)\n\n";
			system("pause");
			return;
		}

		for (ushort_t i = 0; i < users.size(); i++)
		{
			cout << "Пользователь #" << i + 1 << ':';
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
				buf.erase(0, 17); //обрезать "IncompleteTests: "

				while (buf.find('(') != string::npos)
				{
					string categoryName = "";
					string testName = "";
					ushort_t lastQuestNum{};
					ushort_t numOfCorrectAnswers{};

					buf.erase(0, 1); //обрезать открывающую скобку
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
				buf.erase(0, 13); //обрезать "TestResults: "

				while (buf.find('(') != string::npos)
				{
					string testName = "";
					vector<ushort_t> grades{};

					buf.erase(0, 1); //обрезать открывающую скобку
					testName = buf.substr(0, buf.find(','));
					buf.erase(0, buf.find(',') + 1);
					while (buf.find(',') < buf.find(')'))
					{
						grades.push_back(atoi(buf.substr(0, buf.find(',')).c_str()));
						buf.erase(0, buf.find(',') + 1);
					}
					buf.erase(0, 1); //обрезать закрывающую скобку

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
				buf.erase(0, 8); //обрезать "Grades: "
				while (buf.find(',') != string::npos)
				{
					gradesHistory.push_back(atoi(buf.substr(0, buf.find(',')).c_str()));
					buf.erase(0, buf.find(',') + 1);
				}

				isGradesHistoryParsed = true;
			}

			else if (buf.find("Tests: ") != string::npos)
			{
				buf.erase(0, 7); //обрезать Tests: 
				while (buf.find('(') != string::npos)
				{
					string name = "";
					vector<ushort_t> gradesHistory{};
					short numOfQuestions{};
					vector<Question> questions{};

					buf.erase(0, 1); //обрезать открывающую скобку
					name = buf.substr(0, buf.find(','));
					buf.erase(0, buf.find(',') + 1);

					while (buf.find('[') < buf.find(','))
					{
						buf.erase(0, 1); //обрезать открывающую скобку
						gradesHistory.push_back(atoi(buf.substr(0, buf.find(']')).c_str()));
						buf.erase(0, buf.find(']') + 1);
					}

					buf.erase(0, 1); //обрезать запятую
					numOfQuestions = atoi(buf.substr(0, buf.find(',')).c_str());
					buf.erase(0, buf.find(',') + 1);

					while (buf.find('<') < buf.find(')'))
					{
						string quest = "";
						vector<string> answerOptions{};
						ushort_t correctOption{};

						buf.erase(0, 1); //обрезать открывающую скобку
						quest = buf.substr(0, buf.find('{'));
						buf.erase(0, buf.find('{'));

						while (buf.find('{') < buf.find(','))
						{
							buf.erase(0, 1); //обрезать открывающую скобку
							answerOptions.push_back({ buf.substr(0, buf.find('}')) });
							buf.erase(0, buf.find('}') + 1);
						}

						buf.erase(0, 1); //обрезать запятую
						correctOption = atoi(buf.substr(0, buf.find('>')).c_str());
						buf.erase(0, buf.find('>') + 1);
						questions.push_back({ quest, answerOptions, correctOption });
					}

					buf.erase(0, 1); //обрезать закрывающую скобку
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
			cout << "\nПридумайте имя пользователя: ";
			getline(cin, username);
			if (!CheckUsernameUniqueness(username, users))
			{
				cout << "\nОшибка регистрации. Имя пользователя не соответствует заданным правилам:"
					<< "\n\t1. Имя пользователя должно быть длиной не менее 3-х символов"
					<< "\n\t2. Имя пользователя не должно содержать никаких символов, кроме A-Z, a-z, 0-9, -, _, ~"
					<< "\n\t3. Имя пользователя должно быть не занято\n\n";

				system("pause");
				system("cls");
				continue;
			}

			cout << "\nПридумайте пароль: ";
			getline(cin, password);
			if (!CheckPasswordUniqueness(password))
			{
				cout << "\nОшибка регистрации. Пароль не соответствует заданным правилам:"
					<< "\n\t1. Пароль не должен быть короче 8 символов"
					<< "\n\t2. Пароль должен содержать буквы только латинского алфавита"
					<< "\n\t3. Пароль должен содержать цифры, буквы (строчные и прописные), уникальные символы"
					<< "\n\t4. Пароль должен быть уникальным\n\n";

				system("pause");
				system("cls");
				continue;
			}

			break;
		}

		cout << "\nВведите своё ФИО: ";
		getline(cin, fullname);

		while (true)
		{
			system("cls");
			cout << "\nВведите номер телефона (в формате \"+xyyyzzzzzzz\", минимальное количество цифр: 9, "
				"максимальное количество цифр: 18)\n> ";
			getline(cin, phoneNumber);

			if (phoneNumber[0] != '+')
			{
				cout << "\nОшибка (не соблюдён формат \"+xyyyzzzzzzz\")\n\n";
				system("pause");
				continue;
			}
			else if (phoneNumber.size() > 19)
			{
				cout << "\nОшибка (количество цифр превышает 18)\n\n";
				system("pause");
				continue;
			}
			else if (phoneNumber.size() < 10)
			{
				cout << "\nОшибка (количество цифр меньше 9)\n\n";
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
				cout << "\nОшибка (в введённом номере телефона присутствуют не только цифры)\n\n";
				system("pause");
				continue;
			}

			break;
		}

		cout << "\nВведите свой домашний адрес: ";
		getline(cin, homeAddress);

		RegisterUser(username, password, fullname, phoneNumber, homeAddress);
		cout << "\nАккаунт успешно зарегистрирован\n\n";
		system("pause");
	}
	void AddUser()
	{
		string username = "";
		string password = "";
		string fullname = "";
		string phoneNumber = "";
		string homeAddress = "";

		cout << "\nВведите имя пользователя (оно будет проверено только на неповторяемость): ";
		getline(cin, username);
		for (ushort_t i = 0; i < users.size(); i++)
		{
			if (Decrypt(users[i].username, users[i].usernameKey) == username)
			{
				cout << "\nОшибка (невозможно установить для пользователя такое имя, поскольку оно уже занято)\n\n";
				system("pause");
				return;
			}
		}

		cout << "\nВведите пароль (он не будет проверен на уникальность): ";
		getline(cin, password);

		cout << "\nВведите ФИО пользователя: ";
		getline(cin, fullname);

		cout << "\nВведите номер телефона (в формате \"+xyyyzzzzzzz\", минимальное количество цифр: 9, "
			"максимальное количество цифр: 18)\n> ";
		getline(cin, phoneNumber);

		if (phoneNumber[0] != '+')
		{
			cout << "\nОшибка (не соблюдён формат \"+xyyyzzzzzzz\")\n\n";
			system("pause");
			return;
		}
		else if (phoneNumber.size() > 19)
		{
			cout << "\nОшибка (количество цифр превышает 18)\n\n";
			system("pause");
			return;
		}
		else if (phoneNumber.size() < 10)
		{
			cout << "\nОшибка (количество цифр меньше 9)\n\n";
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
			cout << "\nОшибка (в введённом номере телефона присутствуют не только цифры)\n\n";
			system("pause");
			return;
		}

		cout << "\nВведите домашний адрес пользователя: ";
		getline(cin, homeAddress);

		RegisterUser(username, password, fullname, phoneNumber, homeAddress);
		cout << "\nПользователь успешно добавлен\n\n";
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
			cout << "\nОшибка (невозможно отредактировать пользователя, поскольку нет ни одного пользователя)\n\n";
			system("pause");
			return;
		}

		cout << "\nПользователи:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\nВыберите пользователя, который будет отредактирован (1-" << users.size() << "): ";
			cin >> userIndex;
		}
		userIndex--;

		while (true)
		{
			system("cls");
			cout << "\nТекущие данные пользователя:\n";
			ShowUser(userIndex);
			cout << "\n\n1 - Изменить имя пользователя\n2 - Изменить пароль\n3 - Изменить ФИО\n4 - Изменить номер телефона"
				"\n5 - Изменить домашний адрес\nЛюбое другое число - выход из редактирования\n\nВыберите действие: ";
			ushort_t choice{};
			cin >> choice;
			system("cls");

			switch (choice)
			{
			case 1:
			{
				string username = "";
				cout << "\nВведите новое имя пользователя (оно будет проверено только на неповторяемость): ";
				cin.ignore();
				getline(cin, username);
				for (ushort_t i = 0; i < users.size(); i++)
				{
					if (Decrypt(users[i].username, users[i].usernameKey) == username)
					{
						cout << "\nОшибка (невозможно установить для пользователя такое имя, поскольку оно уже занято)\n\n";
						system("pause");
						return;
					}
				}

				cout << "\nИмя пользователя успешно изменено с \"" << Decrypt(users[userIndex].username, users[userIndex].usernameKey)
					<< "\" на \"" << username << "\"\n\n";
				users[userIndex].username = Encrypt(username, users[userIndex].usernameKey);
				system("pause");
				continue;
			}
			case 2:
			{
				string password = "";
				cout << "\nВведите новый пароль (он не будет проверен на уникальность): ";
				cin.ignore();
				getline(cin, password);

				cout << "\nПароль пользователя успешно изменён с \"" << Decrypt(users[userIndex].password, users[userIndex].passwordKey)
					<< "\" на \"" << password << "\"\n\n";
				users[userIndex].password = Encrypt(password, users[userIndex].passwordKey);
				system("pause");
				continue;
			}
			case 3:
			{
				string fullname = "";
				cout << "\nВведите новое ФИО: ";
				cin.ignore();
				getline(cin, fullname);

				cout << "\nФИО пользователя успешно изменено с \"" << users[userIndex].fullname << "\" на \"" << fullname << "\"\n\n";
				users[userIndex].fullname = fullname;
				system("pause");
				continue;
			}
			case 4:
			{
				string phoneNumber = "";
				cout << "\nВведите номер телефона (в формате \"+xyyyzzzzzzz\", минимальное количество цифр: 9, "
					"максимальное количество цифр: 18)\n> ";
				cin.ignore();
				getline(cin, phoneNumber);

				if (phoneNumber[0] != '+')
				{
					cout << "\nОшибка (не соблюдён формат \"+xyyyzzzzzzz\")\n\n";
					system("pause");
					continue;
				}
				else if (phoneNumber.size() > 19)
				{
					cout << "\nОшибка (количество цифр превышает 18)\n\n";
					system("pause");
					continue;
				}
				else if (phoneNumber.size() < 10)
				{
					cout << "\nОшибка (количество цифр меньше 9)\n\n";
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
					cout << "\nОшибка (в введённом номере телефона присутствуют не только цифры)\n\n";
					system("pause");
					continue;
				}

				cout << "\nНомер телефона успешно изменён с \"" << users[userIndex].phoneNumber << "\" на \"" << phoneNumber << "\"\n\n";
				users[userIndex].phoneNumber = phoneNumber;
				system("pause");
				continue;
			}
			case 5:
			{
				string homeAddress = "";
				cout << "\nВведите новый домашний адрес: ";
				cin.ignore();
				getline(cin, homeAddress);

				cout << "\nДомашний адрес успешно изменён с \"" << users[userIndex].homeAddress << "\" на \"" << homeAddress << "\"\n\n";
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
			cout << "\nОшибка (невозможно удалить пользователя, поскольку нет ни одного пользователя)\n\n";
			system("pause");
			return;
		}

		cout << "\nПользователи:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\nВыберите пользователя, который будет удалён (1-" << users.size() << "): ";
			cin >> userIndex;
		}
		userIndex--;
		system("cls");

		cout << "\nПользователь \"" << Decrypt(users[userIndex].username, users[userIndex].usernameKey) << "\" успешно удалён\n\n";
		users.erase(users.begin() + userIndex);
		system("pause");
	}

	void ShowCategories() const
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно посмотреть категории, поскольку нет ни одной категории)\n\n";
			return;
		}

		cout << "\nКатегории:\n\n";
		for (ushort_t i = 0; i < categories.size(); i++)
			cout << i + 1 << ". " << categories[i].getName() << endl;
	}
	void ShowTests(const ushort_t& categoryIndex) const
	{
		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\nОшибка (невозможно посмотреть тесты внутри категории, поскольку в категории нет ни одного теста)\n\n";
			return;
		}

		cout << "\nТесты:\n\n";
		for (ushort_t i = 0; i < categories[categoryIndex].getTests().size(); i++)
			cout << i + 1 << ". " << categories[categoryIndex].getTests()[i].getName() << endl;
	}
	void ShowTests() const
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно посмотреть тесты внутри категории, поскольку нет ни одной категории)\n\n";
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, тесты внутри которой будут отображены (1-" << categories.size() << "): ";
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
			cout << "\nОшибка (невозможно посмотреть вопросы в тесте, поскольку в тесте нет вопросов)\n\n";
			return;
		}

		cout << "\nВопросы:\n\n";
		for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions().size(); i++)
		{
			cout << i + 1 << ". " << categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getQuest()
				<< "\nВарианты ответа: ";
			for (ushort_t j = 0; j < categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getAnswerOptions().size(); j++)
				cout << "\n\t" << j + 1 << ") " << categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getAnswerOptions()[j];

			cout << "\n\tНомер правильного варианта ответа: "
				<< categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getCorrectOption() + 1 << endl;
		}
	}
	void ShowQuestions() const
	{
		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, вопросы в тесте внутри которой будут отображены (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\nВыберите тест, вопросы в котором будут отображены (1-" << categories[categoryIndex].getTests().size() << "): ";
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
			cout << "\nОшибка (невозможно посмотреть оценки в категории, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;

		auto categoryIt = categories.cbegin() + categoryIndex;
		if (categoryIt->getGradesHistory().empty())
		{
			cout << "\nОшибка (невозможно посмотреть оценки в категории, поскольку в категории нет оценок)\n\n";
			system("pause");
			return;
		}

		cout << "\nОценки в категории \"" << categoryIt->getName() << "\":\n";
		for (ushort_t i = 0; i < categoryIt->getGradesHistory().size(); i++)
			cout << categoryIt->getGradesHistory()[i] << (i + 1 < categoryIt->getGradesHistory().size() ? ", " : "");

		cout << endl << endl;
		system("pause");
	}
	void ShowTestGrades() const
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозожно посмотреть оценки в тесте, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\nОшибка (невозможно посмотреть оценки в тесте, поскольку в категории нет тестов)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\nВыберите тест (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		if (categories[categoryIndex].getTests()[testIndex].getGradesHistory().empty())
		{
			cout << "\nОшибка (невозможно посмотреть оценки в тесте, поскольку в тесте нет оценок)\n\n";
			system("pause");
			return;
		}

		cout << "\nОценки в тесте \"" << categories[categoryIndex].getTests()[testIndex].getName() << "\":\n";
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
			cout << "\nОшибка (невозможно посмотреть результаты тестов пользователя, поскольку нет ни одного пользователя)\n\n";
			system("pause");
			return;
		}

		cout << "\nПользователи:\n";
		for (ushort_t i = 0; i < users.size(); i++)
			cout << i + 1 << ". " << Decrypt(users[i].username, users[i].usernameKey) << endl;

		ushort_t userIndex{};
		while (userIndex < 1 || userIndex > users.size())
		{
			cout << "\nВыберите пользователя, который будет удалён (1-" << users.size() << "): ";
			cin >> userIndex;
		}
		userIndex--;
		system("cls");

		if (users[userIndex].testResults.empty())
		{
			cout << "\nОшибка (невозможно посмотреть результаты тестов пользователя, поскольку он не завершил ни один тест)\n\n";
			system("pause");
			return;
		}

		cout << "\nРезультаты тестов пользователя " << Decrypt(users[userIndex].username, users[userIndex].usernameKey) << ":\n";
		for (ushort_t i = 0; i < users[userIndex].testResults.size(); i++)
		{
			cout << "\nТест: " << users[userIndex].testResults[i].getTestName() << "\n\tОценки: ";
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

		cout << "\nВведите название категории: ";
		getline(cin, categoryName);
		for (ushort_t i = 0; i < categories.size(); i++)
		{
			if (categories[i].getName() == categoryName)
			{
				cout << "\nОшибка (невозможно добавить категорию, поскольку категория с таким названием уже существует)\n\n";
				system("pause");
				return;
			}
		}

		short numOfTests{};
		cout << "\nВведите количество тестов, которые Вы готовы заполнить прямо сейчас (позже их можно будет добавить): ";
		cin >> numOfTests;

		bool isAlreadyExist = false;
		for (ushort_t i = 0; i < numOfTests; i++)
		{
			system("cls");
			string testName = "";
			short numOfQuestions{};
			vector<Question> questions{};

			cout << "\nЗаполнение " << i + 1 << "-го теста\n\nВведите название теста: ";
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
				cout << "\nОшибка (невозможно добавить тест в категорию, поскольку тест с таким названием уже существует)\n\n";
				isAlreadyExist = false;
				i--;
				system("pause");
				continue;
			}

			cout << "\nВведите количество вопросов, которые Вы готовы заполнить прямо сейчас (позже их можно будет добавить): ";
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

				cout << "\nЗаполнение " << j + 1 << "-го вопроса\n\nВведите вопрос: ";
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
					cout << "\nОшибка (невозможно добавить вопрос, поскольку такой вопрос уже существует)\n\n";
					isAlreadyExist = false;
					j--;
					system("pause");
					continue;
				}

				while (numOfAnswerOptions < 2)
				{
					cout << "\nВведите количество вариантов ответа (минимум: 2): ";
					cin >> numOfAnswerOptions;
				}

				cin.ignore();
				for (ushort_t k = 0; k < numOfAnswerOptions; k++)
				{
					system("cls");
					string answerOption = "";
					cout << "\nВведите " << k + 1 << " вариант ответа: ";
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
						cout << "\nОшибка (невозможно добавить вариант ответа, поскольку такой вариант ответа уже существует)\n\n";
						isAlreadyExist = false;
						k--;
						system("pause");
						continue;
					}

					answerOptions.push_back(answerOption);
				}
				system("cls");

				cout << "\nДобавленные варианты ответа:\n\n";
				for (ushort_t i = 0; i < numOfAnswerOptions; i++)
					cout << i + 1 << ". " << answerOptions[i] << endl;

				while (correctOption < 1 || correctOption > numOfAnswerOptions)
				{
					cout << "\nВведите номер правильного варианта ответа (1-" << numOfAnswerOptions << "): ";
					cin >> correctOption;
				}
				correctOption--;

				questions.push_back({ quest, answerOptions, correctOption });
			}

			tests.push_back({ testName, {}, static_cast<ushort_t>(numOfQuestions), questions });
		}
		system("cls");

		categories.push_back({ categoryName, {}, tests });
		cout << "\nКатегория \"" << categoryName << "\" успешно добавлена\n\n";
		system("pause");
	}

	void AddTest()
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно добавить тест, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, в которую будет добавлен тест (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		string testName = "";
		short numOfQuestions{};
		vector<Question> questions{};

		cout << "\nВведите название теста: ";
		cin.ignore();
		getline(cin, testName);

		for (ushort_t i = 0; i < categories[categoryIndex].getTests().size(); i++)
		{
			if (categories[categoryIndex].getTests()[i].getName() == testName)
			{
				cout << "\nОшибка (невозможно добавить тест, поскольку тест с таким названием уже существует)\n\n";
				system("pause");
				return;
			}
		}

		cout << "\nВведите количество вопросов, которые Вы готовы заполнить прямо сейчас (позже их можно будет добавить): ";
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

			cout << "\nЗаполнение " << i + 1 << "-го вопроса\n\nВведите вопрос: ";
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
				cout << "\nОшибка (невозможно добавить вопрос, поскольку такой вопрос уже существует)\n\n";
				isAlreadyExist = false;
				i--;
				system("pause");
				continue;
			}

			while (numOfAnswerOptions < 2)
			{
				cout << "\nВведите количество вариантов ответа (минимум: 2): ";
				cin >> numOfAnswerOptions;
			}

			cin.ignore();
			for (ushort_t j = 0; j < numOfAnswerOptions; j++)
			{
				system("cls");
				string answerOption = "";

				cout << "\nВведите " << j + 1 << " вариант ответа: ";
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
					cout << "\nОшибка (невозможно добавить вариант ответа, поскольку такой вариант ответа уже существует)\n\n";
					isAlreadyExist = false;
					j--;
					system("pause");
					continue;
				}

				answerOptions.push_back(answerOption);
			}
			system("cls");

			cout << "\nДобавленные варианты ответа:\n\n";
			for (ushort_t i = 0; i < numOfAnswerOptions; i++)
				cout << i + 1 << ". " << answerOptions[i] << endl;

			while (correctOption < 1 || correctOption > numOfAnswerOptions)
			{
				cout << "\nВведите номер правильного варианта ответа (1-" << numOfAnswerOptions << "): ";
				cin >> correctOption;
			}
			correctOption--;

			questions.push_back({ quest, answerOptions, correctOption });
		}
		system("cls");

		categories[categoryIndex].AddTest({ testName, {}, static_cast<ushort_t>(numOfQuestions), questions });
		cout << "\nТест \"" << testName << "\" успешно добавлен в категорию \"" << categories[categoryIndex].getName() << "\"\n\n";
		system("pause");
	}

	void AddQuestion()
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно добавить вопрос к тесту, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, в тест которой будет добавлен вопрос (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\nОшибка (невозможно добавить вопрос к тесту, поскольку в категории нет тестов)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\nВыберите тест, в который будет добавлен вопрос (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		system("cls");
		string quest = "";
		short numOfAnswerOptions{};
		vector<string> answerOptions{};
		ushort_t correctOption{};

		cout << "\nВведите вопрос: ";
		cin.ignore();
		getline(cin, quest);

		for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions().size(); i++)
		{
			if (categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getQuest() == quest)
			{
				cout << "\nОшибка (невозможно добавить вопрос, поскольку такой вопрос уже существует)\n\n";
				system("pause");
				return;
			}
		}

		while (numOfAnswerOptions < 2)
		{
			cout << "\nВведите количество вариантов ответа (минимум: 2): ";
			cin >> numOfAnswerOptions;
		}

		cin.ignore();
		bool isAlreadyExist = false;
		for (ushort_t i = 0; i < numOfAnswerOptions; i++)
		{
			system("cls");
			string answerOption = "";
			cout << "\nВведите " << i + 1 << " вариант ответа: ";
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
				cout << "\nОшибка (невозможно добавить вариант ответа, поскольку такой вариант ответа уже существует)\n\n";
				isAlreadyExist = false;
				i--;
				system("pause");
				continue;
			}

			answerOptions.push_back(answerOption);
		}
		system("cls");

		cout << "\nДобавленные варианты ответа:\n\n";
		for (ushort_t i = 0; i < numOfAnswerOptions; i++)
			cout << i + 1 << ". " << answerOptions[i] << endl;

		while (correctOption < 1 || correctOption > numOfAnswerOptions)
		{
			cout << "\nВведите номер правильного варианта ответа (1-" << numOfAnswerOptions << "): ";
			cin >> correctOption;
		}
		correctOption--;

		categories[categoryIndex].AddQuestion(testIndex, { quest, answerOptions, correctOption });
		cout << "\nВопрос \"" << quest << "\" успешно добавлен в тест \"" << categories[categoryIndex].getTests()[testIndex].getName() << "\"\n\n";
		system("pause");
	}

	void ChangeCategoryName()
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно изменить название категории, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, название которой будет изменено (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		string categoryName = "";
		cout << "\nВведите новое название категории: ";
		cin.ignore();
		getline(cin, categoryName);

		for (ushort_t i = 0; i < categories.size(); i++)
		{
			if (categories[i].getName() == categoryName)
			{
				cout << "\nОшибка (невозможно изменить название категории, поскольку категория с таким названием уже существует)\n\n";
				system("pause");
				return;
			}
		}

		cout << "\nНазвание категории \"" << categories[categoryIndex].getName() << "\" успешно изменено на \"" << categoryName << "\"\n\n";
		categories[categoryIndex].setName(categoryName);
		system("pause");
	}

	void ChangeTestName()
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно изменить название теста, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, название теста внутри которой будет изменено (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\nОшибка (невозможно изменить название теста, поскольку в категории нет тестов)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\nВыберите тест, название которого будет изменено (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;
		system("cls");

		string testName = "";
		cout << "\nВведите новое название теста: ";
		cin.ignore();
		getline(cin, testName);

		for (ushort_t i = 0; i < categories[categoryIndex].getTests().size(); i++)
		{
			if (categories[categoryIndex].getTests()[i].getName() == testName)
			{
				cout << "\nОшибка (невозможно изменить название теста, поскольку тест с таким названием уже существует)\n\n";
				system("pause");
				return;
			}
		}

		categories[categoryIndex].ChangeTestName(testIndex, testName);
		cout << "\nНазвание теста успешно изменено на \"" << testName << "\"\n\n";
		system("pause");
	}

	void ChangeQuestion()
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно изменить вопрос у теста, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, вопрос в тесте внутри которой будет изменён (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\nОшибка (невозможно изменить вопрос в тесте, поскольку в категории нет тестов)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\nВыберите тест, вопрос в котором будет изменён (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;
		system("cls");

		if (categories[categoryIndex].getTests()[testIndex].getQuestions().empty())
		{
			cout << "\nОшибка (невозможно изменить вопрос в тесте, поскольку в тесте нет вопросов)\n\n";
			system("pause");
			return;
		}

		ShowQuestions(categoryIndex, testIndex);
		ushort_t questIndex{};
		while (questIndex < 1 || questIndex > categories[categoryIndex].getTests()[testIndex].getQuestions().size())
		{
			cout << "\nВведите номер вопроса, который будет изменён (1-"
				<< categories[categoryIndex].getTests()[testIndex].getQuestions().size() << "): ";
			cin >> questIndex;
		}
		questIndex--;

		while (true)
		{
			system("cls");
			cout << "\nВопрос: " << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getQuest()
				<< "\nВарианты ответа:\n";
			for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions().size(); i++)
				cout << i + 1 << ". " << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions()[i] << endl;
			cout << "Номер правильного варианта ответа: " << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getCorrectOption() + 1;

			cout << "\n\n1 - Изменить вопрос\n2 - Изменить варианты ответа\n3 - Изменить номер правильного варианта ответа"
				"\nЛюбое другое число - прекратить редактирование\n\nВыберите действие: ";
			ushort_t choice{};
			cin >> choice;
			system("cls");

			switch (choice)
			{
			case 1:
			{
				string quest = "";
				cout << "\nВведите новый вопрос: ";
				cin.ignore();
				getline(cin, quest);

				for (ushort_t i = 0; i < categories[categoryIndex].getTests()[testIndex].getQuestions().size(); i++)
				{
					if (categories[categoryIndex].getTests()[testIndex].getQuestions()[i].getQuest() == quest)
					{
						cout << "\nОшибка (невозможно изменить вопрос, поскольку такой вопрос уже существует)\n\n";
						system("pause");
						continue;
					}
				}

				cout << "\nВопрос успешно изменён с \""
					<< categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getQuest() << "\" на \""
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
					cout << "\nВведите количество вариантов ответа (минимум: 2): ";
					cin >> numOfAnswerOptions;
				}

				cin.ignore();
				bool isAlreadyExist = false;
				for (ushort_t i = 0; i < numOfAnswerOptions; i++)
				{
					system("cls");
					string answerOption = "";
					cout << "\nВведите " << i + 1 << " вариант ответа: ";
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
						cout << "\nОшибка (невозможно добавить вариант ответа, поскольку такой вариант ответа уже существует)\n\n";
						isAlreadyExist = false;
						i--;
						system("pause");
						continue;
					}

					answerOptions.push_back(answerOption);
				}
				system("cls");

				categories[categoryIndex].ChangeQuestionAnswerOptions(testIndex, questIndex, answerOptions);
				cout << "\nВарианты ответа успешно изменены\n\n";
				system("pause");
				continue;
			}
			case 3:
			{
				ushort_t correctOption{};

				while (correctOption < 1 || correctOption > categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions().size())
				{
					cout << "\nВведите номер правильного ответа (1-"
						<< categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getAnswerOptions().size() << "): ";
					cin >> correctOption;
				}

				cout << "\nНомер правильного варианта ответа успешно изменён с \""
					<< categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getCorrectOption() + 1 << "\" на \""
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
			cout << "\nОшибка (невозможно удалить категорию, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, которая будет удалена (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		cout << "\nКатегория \"" << categories[categoryIndex].getName() << "\" успешно удалена\n\n";
		categories.erase(categories.begin() + categoryIndex);
		system("pause");
	}

	void RemoveTest()
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно удалить тест, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, тест внутри которой будет удалён (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\nОшибка (невозможно удалить тест, поскольку в категории нет тестов)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\nВыберите тест, который будет удалён (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		cout << "\nТест \"" << categories[categoryIndex].getTests()[testIndex].getName() << "\" успешно удалён\n\n";
		categories[categoryIndex].RemoveTest(testIndex);
		system("pause");
	}

	void RemoveQuestion()
	{
		if (categories.empty())
		{
			cout << "\nОшибка (невозможно удалить вопрос у теста, поскольку нет ни одной категории)\n\n";
			system("pause");
			return;
		}

		ShowCategories();
		ushort_t categoryIndex{};
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "\nВыберите категорию, вопрос в тесте внутри которой будет удалён (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		system("cls");

		if (categories[categoryIndex].getTests().empty())
		{
			cout << "\nОшибка (невозможно удалить вопрос в тесте, поскольку в категории нет тестов)\n\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		ushort_t testIndex{};
		while (testIndex < 1 || testIndex > categories[categoryIndex].getTests().size())
		{
			cout << "\nВыберите тест, вопрос в котором будет удалён (1-" << categories[categoryIndex].getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		if (categories[categoryIndex].getTests()[testIndex].getQuestions().empty())
		{
			cout << "\nОшибка (невозможно удалить вопрос в тесте, поскольку в тесте нет вопросов)\n\n";
			system("pause");
			return;
		}

		ShowQuestions(categoryIndex, testIndex);
		ushort_t questIndex{};
		while (questIndex < 1 || questIndex > categories[categoryIndex].getTests()[testIndex].getQuestions().size())
		{
			cout << "\nВыберите вопрос, который будет удалён (1-"
				<< categories[categoryIndex].getTests()[testIndex].getQuestions().size() << "): ";
			cin >> questIndex;
		}
		questIndex--;

		cout << "\nВопрос \"" << categories[categoryIndex].getTests()[testIndex].getQuestions()[questIndex].getQuest()
			<< "\" успешно удалён\n\n";
		categories[categoryIndex].RemoveQuestion(testIndex, questIndex);
		system("pause");
	}

	void StartTest()
	{
		try
		{
			if (currentUser == nullptr)
				throw Exception("Пользователь по какой-то причине не авторизован (возможно, метод вызван из неправильной части программы)", -20);
		}
		catch (const Exception& e)
		{
			cerr << e.what() << endl;
			exit(e.getErrorCode());
		}

		if (categories.empty())
		{
			cout << "\nОшибка (невозможно сдать тест, поскольку нет ни одной категории)\n";
			system("pause");
			return;
		}

		ushort_t categoryIndex{};
		ushort_t testIndex{};

		ShowCategories();
		while (categoryIndex < 1 || categoryIndex > categories.size())
		{
			cout << "Выберите категорию (1-" << categories.size() << "): ";
			cin >> categoryIndex;
		}
		categoryIndex--;
		auto categoryIt = categories.begin() + categoryIndex;
		system("cls");

		if (categoryIt->getTests().empty())
		{
			cout << "\nОшибка (невозможно сдать тест, поскольку в категории нет тестов)\n";
			system("pause");
			return;
		}

		ShowTests(categoryIndex);
		while (testIndex < 1 || testIndex > categoryIt->getTests().size())
		{
			cout << "Выберите тест (1-" << categoryIt->getTests().size() << "): ";
			cin >> testIndex;
		}
		testIndex--;

		if (categories[categoryIndex].getTests()[testIndex].getQuestions().empty())
		{
			cout << "\nОшибка (невозможно сдать тест, поскольку в тесте нет вопросов)\n\n";
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
			cout << "\nНачало теста \"" << currentTest->getName()
				<< "\"\nДля преждевременного выхода, введите в поле ответа команду /Выход\n";
		}
		else
		{
			cout << "\nПродолжение ранее преждевременно завершённого теста \"" << currentTest->getName()
				<< "\"\nДля преждевременного выхода, введите в поле ответа команду /Выход\n";
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
			cout << i + 1 << ") " << currentTest->getQuestions()[i].getQuest() << "\nВарианты ответа:\n\n";
			for (ushort_t j = 0; j < currentTest->getQuestions()[i].getAnswerOptions().size(); j++)
				cout << j + 1 << ". " << currentTest->getQuestions()[i].getAnswerOptions()[j] << endl;

			cout << "\nВыберите номер ответа (1-" << currentTest->getQuestions()[i].getAnswerOptions().size() << "): ";
			string selectedOption = "";
			getline(cin, selectedOption);
			if (selectedOption == "/Выход")
			{
				currentUser->incompleteTests.push_back({ categoryIt->getName(), currentTest->getName(), i, correctAnswersCounter });
				return;
			}
			else if ((atoi(selectedOption.c_str()) - 1) == currentTest->getQuestions()[i].getCorrectOption())
			{
				correctAnswersCounter++;
				cout << "\nПравильный ответ!\n\n";
			}
			else
				cout << "\nНеправильный ответ!\n\n";
			system("pause");
		}

		system("cls");
		ushort_t grade = static_cast<ushort_t>(correctAnswersCounter * (12.0 / currentTest->getNumOfQuestions()));
		cout << "\nТест завершён.\nКоличество правильных ответов: " << correctAnswersCounter
			 << "\nПроцент правильных ответов: " << (int)((double)correctAnswersCounter / currentTest->getNumOfQuestions() * 100.0) << '%';
		cout << "\nОценка: " << grade << ' ' << (grade == 1 ? "балл" : grade >= 2 && grade <= 4 ? "балла" : "баллов")
			 << " из 12 возможных\n\n";

		//Обновление статистики у пользователя
		auto statIt = find_if(currentUser->testResults.begin(), currentUser->testResults.end(),
			[&](const Statistics& statistics) { return statistics.getTestName() == currentTest->getName(); });
		if (statIt != currentUser->testResults.end())
			statIt->AddGrade(grade);
		else
			currentUser->testResults.push_back({ currentTest->getName(), { grade } });

		//Обновление статистики у категории
		categoryIt->AddGrade(grade);
		//Обновление статистики у теста
		categoryIt->AddTestGrade(testIndex, grade);

		system("pause");
	}

	void ImportCategoriesFromFile()
	{
		string path = "";
		cout << "\nВведите полный путь к .txt файлу, из которого будут импортированы категории"
			"\nНапример: D:\\Documents\\backup.txt\n> ";
		getline(cin, path);

		LoadCategories(path);
		cout << "\nКатегории успешно импортированы из файла " << path << endl << endl;
		system("pause");
	}

	void ExportCategoriesInFile()
	{
		string path = "";
		cout << "\nВведите полный путь к .txt файлу, в который будут экспортированы категории"
			"\nНапример: D:\\Documents\\backup.txt\n> ";
		getline(cin, path);

		SaveCategories(path);
		cout << "\nКатегории успешно экспортированы в файл " << path << endl << endl;
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
		cout << "\n1 - Пользователь\n2 - Админ\nЛюбое другое число - выход из программы\n\nВыберите режим: ";
		ushort_t choice{};
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			while (true)
			{
				system("cls");
				cout << "\n1 - Зарегистрироваться\n2 - Авторизоваться\nЛюбое другое число - выход из программы\n\nВыберите действие: ";
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

					cout << "\nВведите имя пользователя: ";
					cin.ignore();
					getline(cin, username);
					cout << "Введите пароль: ";
					getline(cin, password);

					if (database.AuthorizeUser(username, password))
					{
						cout << "\nВы успешно авторизованы\n\n";
						system("pause");

						while (true)
						{
							system("cls");
							cout << "\n1 - Посмотреть свои результаты тестирований\n2 - Сдать новое тестирование"
								"\nЛюбое другое число - выход из программы\n\nВыберите действие: ";
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
						cout << "\nОшибка авторизации (неправильное имя пользователя или пароль)\n\n";
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

					cout << "\nАвторизация:\n\nВведите имя пользователя: ";
					getline(cin, username);

					cout << "Введите пароль: ";
					getline(cin, password);

					if (database.getAdmin()->Authorize(username, password))
					{
						cout << "\nВы успешно авторизованы\n\n";
						system("pause");

						while (true)
						{
							system("cls");
							cout << "\n---Учётная запись---\n1.  Изменить своё имя пользователя\n2.  Изменить свой пароль"
								 << "\n\n---Пользователи---\n3.  Посмотреть профили всех пользователей"
								 << "\n4.  Посмотреть профиль пользователя\n5.  Добавить пользователя"
								 << "\n6.  Редактировать пользователя\n7.  Удалить пользователя"
								 << "\n\n---Результаты тестирований---\n8.  Посмотреть результаты тестирований у категории"
								 << "\n9.  Посмотреть результаты тестирований у теста"
								 << "\n10. Посмотреть результаты тестирований у пользователя"
								 << "\n\n---Категории---\n11. Посмотреть все категории"
								 << "\n12. Добавить категорию\n13. Изменить название категории\n14. Удалить категорию"
								 << "\n\n---Тесты---\n15. Посмотреть все тесты в категории\n16. Добавить тест"
							  	 << "\n17. Изменить название теста\n18. Удалить тест\n\n---Вопросы---"
								 << "\n19. Посмотреть все вопросы в тесте\n20. Добавить вопрос к тесту"
								 << "\n21. Изменить вопрос в тесте\n22. Удалить вопрос в тесте"
								 << "\n\n---Импорт и экспорт категорий и тестов---"
								 << "\n23. Импортировать категории и тесты из файла"
								 << "\n24. Экспортировать категории и тесты в файл"
								 << "\n\n---Выход---\nЛюбое другое число\n\nВыберите действие: ";
							cin >> choice;
							system("cls");

							switch (choice)
							{
							case 1:
							{
								string username = "";
								cout << "\nВведите новое имя пользователя: ";
								cin.ignore();
								getline(cin, username);

								database.getAdmin()->ChangeUsername(username);
								continue;
							}
							case 2:
							{
								string password = "";
								cout << "\nВведите новый пароль (он не будет проверен на уникальность): ";
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

					cout << "\nРегистрация:\n\nПридумайте имя пользователя: ";
					getline(cin, username);
					cout << "Придумайте пароль (он не будет проверен на уникальность): ";
					getline(cin, password);

					database.getAdmin()->Register(username, password);
					cout << "\nВы успешно зарегистрированы\n\n";
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