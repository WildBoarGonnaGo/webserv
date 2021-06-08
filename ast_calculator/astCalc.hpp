#ifndef ASTCALC_HPP
# define ASTCALC_HPP
# include <string>
# include <cstdlib>
# include <list>
/*
 ГРАМАТИКА ПРОГРАММЫ "CALCULATOR":
 NUMBER     -> <число>
 group      -> "(" add ")" | NUMBER
 mult       -> group ( ("*" | "/") group)*
 add        -> mult ( ("+" | "-") mult)*
 result     -> add

 mult, add, group, result - названия правил (нелексемы)
 "*", "/", "+", "-" - лексемы (токены)
 <выражение>*  - повторять произвольное количество раз (n или более)
 <вырежание 1> | <выражение 2> | ... | <выражение N> - применить любое из перечисленных правил
 <выражение>+ - повтороять выражение любое количество раз, но не менее одного
 <выражение>? - выражение может присутствовать, так и не присутствовать в разбираемой строке
 * */


class   tokens {
protected:
	// разбираемая строка
	std::string 		_str;
	// Позиция строки
	size_t				_pos;
	// незначащие символы - прообельные символы по умолчанию
	std::string   		_whitespaces;
public:
	tokens();
	tokens(const tokens &rhs);
	tokens(std::string const &src);
	virtual ~tokens();

	tokens				&operator=(const tokens &rhs);
	std::string const	&getStr() const;
	size_t const		&getPos() const;
	class				tokenException : public std::exception {
		public:
			virtual const char *what() const throw();
	};
	//len - длина строки
	int					len();
	//operator[] также предотвращает возникновения ошибки
	//в случае выхода за пределы в строки
	char				&operator[](int idx);
	//символ в текущей позиции указателя
	char const			&current() const;
	//isEnd - проверка достигнут ли конец строки
	bool 				isEnd();
	//next - Перевод на один символ если это не конец строки
	void				next();
	//skip - Пропуск незначащих пробельных символов
	virtual void		skip();
	/* Проверяет, можно ли в текущей позиции указателя распознать одну из строк,
	укзатель не смещается */
	bool				checkMatch(std::list<std::string> terms);
	/* match - распознает одну из строк, при этом указатель смещается
	и пропускаются незначащие символы, если ни одну из строк распознать
	нельзя, то выбрасывается исключение*/	
	std::string			match(std::list<std::string> terms);
	/*Перегрузка функции матч для принятия одной строки*/
	std::string			match(std::string sample);
protected:
	/*
	matchNoExcept - распознает одну из строк, при этом указатель смещается
	и пропускаются незначащие символы, если ни одну из строк распознать
	нельзя, то возвращается NULL
	*/
	std::string			matchNoExcept(std::list<std::string> terms);
};

class   astCalc : public tokens {
	public:
		astCalc();
		astCalc(std::string src);
		astCalc(const astCalc &rhs);
		virtual ~astCalc();

		astCalc	&operator=(const astCalc &rhs);
		class	numErrorCatch {
			public:
				char const *what() const throw();
		};
		// NUMBER - число (реализация в грамматике не описана)
		double	getNUMBER();
		//group      -> "(" add ")" | NUMBER
		double	group();
		//mult       -> group ( ("*" | "/") group)*
		double	mult();
		//add        -> mult ( ("+" | "-") mult)*
		double	add();
		//result     -> add
		double	result();
		/* Метод, вызывающий начальное правило грамматики и
		соответствующие вычисления */
		double	execute();
};

#endif