#include "astNodeCalc.hpp"

AstNodeCalc::AstNodeCalc( ) { }
/*данные внутри дерева
std::string					_data;
Левые и правые ветки узла
AstNodeCalc					*_lNode;
AstNodeCalc					*_rNode;
Потомки (ветви) данного узла дерева
std::list<AstNodeCalc *>	_child;
Конструктор по умолчанию, кидаем в private, потому что бесполезен
в рамках программы
Подсчет вершин дерева*/
size_t		AstNodeCalc::countNodes(AstNodeCalc *head = NULL, int count = 0) {
	if (!head && !count) {
		head = this;
		++count;
	}
	if (!head)
		return (0);
	else
		return (1 + countNodes(head->_lNode, count)
			+ countNodes(head->_rNode, count));
}
//Очистка дерева
void		AstNodeCalc::astClear(AstNodeCalc *head = NULL, int count = 0) {
	if (!head && !count) {
		head = this;
		++count;
	}
	if (head) {
		astClear(head->_lNode, count);
		astClear(head->_rNode, count);
		delete (head);
		head = NULL;
	}
}
//Возврат соответствующей строки с соответствующим типом
std::string	AstNodeCalc::astNodeTypeToString(int const &type) {
			switch (type) {
				case UNKNOWN: 	return ("?");
				case NUMBER:	return ("NUM");
				case IDENT:		return ("ID");
				case ADD:		return ("+");
				case SUB:		return ("-");
				case MUL:		return ("*");
				case DIV:		return ("/");
				case ASSIGN:	return ("=");
				case INPUT:		return ("input");
				case PRINT:		return ("print");
				case BLOCK:		return ("..");
				case PROGRAM:	return ("program");
				default:		return ("");	
			}
		}
//Параметрический конструктор
AstNodeCalc::AstNodeCalc(int type, std::string text = std::string(),
			AstNodeCalc *lNode = NULL, AstNodeCalc *rNode = NULL) :
			_type(type), _text(text), _lNode(lNode), _rNode(rNode) { } 
//Конструктор глубого копирования
AstNodeCalc::AstNodeCalc(const AstNodeCalc &rhs) {
	if (this != &rhs)
		*this = rhs;
}
AstNodeCalc	&AstNodeCalc::operator=(const AstNodeCalc &rhs) {
	astClear();
	this->
	return (*this);
}
AstNodeCalc	*AstNodeCalc::getRoot() {
	return (this);
}
		int			type;
		//Текст связянный с узлом
		std::string	text;
		//метод добавления дочернего узла
		void		addChild(AstNodeCalc *child);
		//метод удаления дочернего узла
		void		delChild(AstNodeCalc *child = NULL);
		//индекс данного узла в дочерних узлах
		AstNodeCalc	&operator[](int idx);