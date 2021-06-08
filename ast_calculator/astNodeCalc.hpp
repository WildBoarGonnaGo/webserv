#ifndef  ASTNODECALC_HPP
# define ASTNODECALC_HPP
# include <string>
# include <cstdlib>
# include <list>
// набор констант для всевозможных типов токенов
# define UNKNOWN 0
# define NUMBER	1
# define IDENT 5
# define ADD 11
# define SUB 12
# define MUL 13
# define DIV 14
# define ASSIGN 51
# define INPUT 52
# define PRINT 53
# define BLOCK 100
# define PROGRAM 101

/*
Построение AST дерева в процессе разбора:

Старая грамматика:
NUMBER     -> <число>
group      -> "(" add ")" | NUMBER
mult       -> group ( ("*" | "/") group)*
add        -> mult ( ("+" | "-") mult)*
result     -> add

Новая грамматика:
getNUMBER	->	<число>
getIDENT	->	<идентификатор>	
group		->	"(" term ")" | IDENT | NUMBER
mult		->	group ( ("*", "/"), group)*
add 		->	mult ( ("+", "-"), mult)*
expr		->	"print" term
		    | "input" IDENT
			| IDENT "=" term
program		->  ( expr )*
result		->	program

Построение дерева происходит в ходе синтаксического анализа
*/

class AstNodeCalc {
		AstNodeCalc();
	protected:
		//тип токена
		int							_type;
		//данные					
		std::string					_text;
		//Левые и правые ветки узла
		AstNodeCalc					*_lNode;
		AstNodeCalc					*_rNode;
		// Потомки (ветви) данного узла дерева
		std::list<AstNodeCalc *>	_child;
		
		//Конструктор по умолчанию, кидаем в private, потому что бесполезен
		//в рамках программы
		//Подсчет вершин дерева
		size_t		countNodes(AstNodeCalc *head = NULL, int count = 0);
		//Очистка дерева
		void		astClear(AstNodeCalc *head = NULL, int count = 0);
		//Возврат соответствующей строки с соответствующим типом
		std::string	astNodeTypeToString(int const &type);
	public:
		//Параметрический конструкторs
		AstNodeCalc(int type, std::string text = std::string(),
			AstNodeCalc *lNode = NULL, AstNodeCalc *rNode = NULL);
		//Конструктор глубого копирования
		AstNodeCalc(const AstNodeCalc &rhs);
		AstNodeCalc	&operator=(const AstNodeCalc &rhs);
		//Текст связянный с узлом
		std::string	text;
		//метод добавления дочернего узла
		void		addChild(AstNodeCalc *child);
		//метод удаления дочернего узла
		void		delChild(AstNodeCalc *child = NULL);
		//индекс данного узла в дочерних узлах
		AstNodeCalc	&operator[](int idx);
		//Возврат корня дерева
		AstNodeCalc	*getRoot();
};

class MathLangParser : public AstNodeCalc {
	public:
		//Конструктор
		MathLangParser(std::string &src);
		/* Далее идет реализация в виде функций правил грамматики:
		getNUMBER	->	<число>*/
		AstNodeCalc	*getNUMBER();
		//getIdent	->	<идентификатор>
		AstNodeCalc	*getIDENT();
		//group		->	"(" term ")" | IDENT | NUMBER
		AstNodeCalc	*group();
		//mult      -> group ( ("*" | "/") group)*
		AstNodeCalc	*mult();
		//add 		->	mult ( ("+", "-"), mult)*
		AstNodeCalc	*add();
		/*expr		->	"print" term
		    			| "input" IDENT
						| IDENT "=" term*/
		AstNodeCalc	*expr();
		//program		->  ( expr )*
		AstNodeCalc	*program();
		//result		->	program
		AstNodeCalc	&result();
};
#endif