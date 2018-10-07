#pragma once

#include <iostream>
#include <string>

namespace sfio 
{

	template <typename T>
	void safe_cin(const std::wstring& msg, T& foo, const wchar_t aftermsg = '\n')
	{
		std::wcout << msg << aftermsg;

		for (;;)
		{
			std::cin >> foo;

			if (std::cin && std::cin.peek() != '\n')
			{
				std::cin.ignore(std::cin.rdbuf()->in_avail());
				std::wcout << L"Получено слишком много символов."
					L" Пожалуйста, повторите ввод:"
					<< std::endl;
			}
			else if (std::cin.eof())
			{
				foo = T();
				std::cin.clear();
				break;
			}
			else if (std::cin.bad() || std::cin.fail())
			{
				std::wcout << L"Получено неверное значение."
					L" Пожалуйста, повторите ввод:"
					<< std::endl;
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			}
			else break;
		}
	}

	template <typename T>
	void safe_cin(const std::wstring& msg, T& foo, T lowest, T max, const wchar_t aftermsg = '\n')
	{
		std::wcout << msg <<
			L"[" << lowest << 
			L" ; " << max << L"]:" << aftermsg;

		for (;;)
		{
			std::cin >> foo;

			if (std::cin && std::cin.peek() != '\n')
			{
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			}
			else if (std::cin.eof())
			{
				foo = T();
				std::cin.clear();
				break;
			}
			else if (std::cin.bad() || std::cin.fail())
			{
				std::wcout << L"Получено неверное значение."
					L" Пожалуйста, повторите ввод:"
					<< std::endl;
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			}
			else if (foo < lowest || foo > max)
			{
				std::wcout << L"Введенное число выходит за указанные границы.\n"
					L"Пожалуйста, повторите ввод:" << std::endl;
			}
			else break;
		}
	}

	template <class T>
	void safe_cin(const std::wstring& msg, T& foo, const std::initializer_list<T>& exceptvals, const wchar_t aftermsg = '\n')
	{
		std::wcout << msg << L" (все значения кроме ";

		for (auto it = exceptvals.begin(); it != exceptvals.end(); ++it)
		{
			std::wcout << *it;
			if (it + 1 == exceptvals.end()) break;
			std::wcout << L", ";
		}

		std::wcout << L"):" << aftermsg;

		for (;;)
		{
			std::cin >> foo;

			if (std::cin.eof())
			{
				foo = T();
				std::cin.clear();
				break;
			}

			if (std::cin && std::cin.peek() != '\n')
			{
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			}
			else if (std::cin.bad() || std::cin.fail())
			{
				std::wcout << L"Получено неверное значение."
					L" Пожалуйста, повторите ввод:"
					<< std::endl;
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			}
			else if (std::find(exceptvals.begin(), exceptvals.end(), foo) != exceptvals.end())
			{
				std::wcout << L"Введенное значение недопустимо.\n"
					L"Пожалуйста, повторите ввод:" << std::endl;
			}
			else break;
		}
	}

}