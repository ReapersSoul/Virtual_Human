#pragma once
#include <string>
#include <iostream>
#include <windows.h>

void printFloating(double v, int Precicion) {
	std::cout << v << std::setprecision(Precicion);
}
void printInt(int i) {
	std::cout << i;
}

bool CharIsNum(char c) {
	switch (c)
	{
	case '0':
		return true;
		break;
	case '1':
		return true;
		break;
	case '2':
		return true;
		break;
	case '3':
		return true;
		break;
	case '4':
		return true;
		break;
	case '5':
		return true;
		break;
	case '6':
		return true;
		break;
	case '7':
		return true;
		break;
	case '8':
		return true;
		break;
	case '9':
		return true;
		break;
	default:
		return false;
		break;
	}
}

void printfc(const char* c,bool ResetOnSpace, ...)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
	va_list lst;
	va_start(lst, ResetOnSpace);
	std::string tmp = "";

	while (*c != '\0') {
		if (*c == ' ' && ResetOnSpace) {
			SetConsoleTextAttribute(hConsole, 15);
		}

		if (*c != '%') {
			putchar(*c);
			c++;
			continue;
		}
		c++;
		while (CharIsNum(*c))
		{
			tmp += *c;
			c++;
		}
		if (*c == '.') {
			if (CharIsNum(*(c + 1))) {
				tmp += *c;
				c++;
				while (CharIsNum(*c))
				{
					tmp += *c;
					c++;
				}
			}
		}
		int d;
		double f;

		switch (*c)
		{
		case 'f':
			f = va_arg(lst, double);
			tmp = '%' + tmp+'f';
			printf(tmp.c_str(), f);
			break;
		case 'd':
			d = va_arg(lst, int);
			std::cout << d;
			break;
		case 's':
			std::cout << va_arg(lst, char*);
			break;
		case 'C':
			if (!tmp.empty()) {
				SetConsoleTextAttribute(hConsole, atoi(tmp.c_str()));
				tmp.clear();
			}
			else {
				SetConsoleTextAttribute(hConsole, va_arg(lst, int));
			}
			break;

		default:
			break;
		}
		c++;
	}
	SetConsoleTextAttribute(hConsole, 15);
}

class ColoredText : public std::string{
public:
	void Print() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 15);
		for (int i = 0; i < this->size(); i++)
		{
			
			if ((*this)[i] == '\\') {
				std::string tmp = "";
				tmp += (*this)[i + 1];
				tmp += (*this)[i + 2];
				tmp += (*this)[i + 3];
				SetConsoleTextAttribute(hConsole, atoi(tmp.c_str()));
				i += 4;
			}
			std::cout<<(*this)[i];
		}
		SetConsoleTextAttribute(hConsole, 15);
	}

	ColoredText() {
		*this = std::string();
	}
	ColoredText(std::string str) {
		*this = str;
	}

	ColoredText operator=(std::string str) {
		std::string::operator=(str);
		return *this;
	}

 };