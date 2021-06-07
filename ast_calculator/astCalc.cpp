#include "astCalc.hpp"
#include <iostream>
#include <cctype>

const char	*tokens::tokenException::what() const throw() {
	return ("fatal error: null string");
}

tokens::tokens() : _str(std::string()), _pos(0), _whitespaces(" \n\t\r") { }

tokens::tokens(const tokens &rhs) {
	if (this != &rhs)
		*this = rhs; 
}

tokens::tokens(std::string const &src) : _str(src), _pos(0), _whitespaces(" \n\t\r") { }

tokens	&tokens::operator=(const tokens &rhs) {
	this->_pos = rhs.getPos();
	this->_str = rhs.getStr();
}

std::string const	tokens::getStr() const { return (_str); }

int const			tokens::getPos() const { return (_pos); }
//len - длина строки
int					tokens::len() {
	 return (_str.size());
}
//operator[] также предотвращает возникновения ошибки
//в случае выхода за пределы в строки
char 				&tokens::operator[](int idx) {
	if (idx < 0 || idx >= len()) {
		if (_str.empty())
			_str = std::string();
		return (_str[0]);
	}
	else
		return (_str[idx]);
}
//символ в текущей позиции указателя
char const			&tokens::current() const {
	return (_str[_pos]);
}
//проверка достигнут ли конец строки
bool 				tokens::isEnd() {
	return (_str[_pos] == 0);
}
//Перевод на один символ если это не конец строки
void				tokens::next() {
	if (!isEnd())
		++_pos;
} 
//Пропусе незначащих пробельных символов
void				tokens::skip() {
	if (_whitespaces.find(_str[_pos]) != std::string::npos)
		++_pos;
}
/* Проверяет, можно ли в текущей позиции указателя распознать одну из строк,
указатель не смещается */
bool				tokens::checkMatch(std::list<std::string> terms) {
	int	prevPos = _pos;
	std::string result = matchNoExcept(terms);
	_pos = prevPos;
	return (result.empty());
}
/*
matchNoExcept - распознает одну из строк, при этом указатель смещается
и пропускаются незначащие символы, если ни одну из строк распознать
нельзя, то возвращается NULL
*/
std::string			tokens::matchNoExcept(std::list<std::string> terms) {
	std::list<std::string>::iterator	it;
	bool								match;
	std::string							tmp;
	int									prevPos;

	for (it = terms.begin(); it !=terms.end(); ++it) {
		prevPos = getPos();
		match = true;
		tmp = *it;
		for (int i = 0; i < tmp.size(); i++) {
			if (tmp[i] == _str[i])
				next();
			else {
				_pos = prevPos;
				match = false;
				break ;
			}
		}
		if (match) {
			// после разбора терминала пропускаем незначащие символы
			skip();
			return (*it);
		}
	}
	return (std::string());
}
/* match - распознает одну из строк, при этом указатель смещается
и пропускаются незначащие символы, если ни одну из строк распознать
нельзя, то выбрасывается исключение*/	
std::string			tokens::match(std::list<std::string> terms) {
	int 		pos = getPos();
	std::string	result = matchNoExcept(terms);
	if (result.empty())
		throw tokens::tokenException();
	return (result);
}

/*Перегрузка функции матч для принятия одной строки*/
std::string			tokens::match(std::string sample) {
	std::list<std::string>	tmp(1, sample);
	try
	{
		std::string result = matchNoExcept(tmp);
		if (result.empty())
			throw tokens::tokenException();
		return (result);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

astCalc::astCalc() : tokens(std::string()) { }

astCalc::astCalc(std::string src) : tokens(src) { }

astCalc::astCalc(const astCalc &rhs) {
	if (this != &rhs)
		*this = rhs;
}

astCalc	&astCalc::operator=(const astCalc &rhs) {
	this->_str = rhs.getStr();
	this->_pos = rhs.getPos();
	this->_whitespaces = std::string(" \n\t\r");
	return (*this);
}

char const *astCalc::numErrorCatch::what() const throw() {
	return ("fatal error: invalid NUMBER input");
}
// NUMBER - число (реализация в грамматике не описана)
double	astCalc::getNUMBER() {
	std::string	number;
	int			pointNum = 0;

	while ((isdigit(current()) || current() == '.') && pointNum < 2 && !isEnd()) {
		number.push_back(current());
		if (current() == '.')
			++pointNum;
	}
	if (pointNum == 2 || !number.size()
		|| _whitespaces.find(_str[_pos]) == std::string::npos)
		throw astCalc::numErrorCatch();
	skip();
	return (atof(number.c_str()));
}

//group      -> "(" add ")" | NUMBER
double	astCalc::group() {
	
};