#include "pch.h"
#include "Catenary.h"
#include "safe_io.h"


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	std::wcout.imbue(std::locale(".866"));

	enum 
	{
		exit = 1, 
		get_ordinate, 
		get_arc_length, 
		get_curvature_radius, 
		get_curvature_center_coordinates, 
		get_trapeze_area
	};

	const wchar_t* msg = L"\n"
		"1. Выход\n"
		"2. Вернуть ординату цепной линии\n"
		"3. Вернуть длину дуги\n"
		"4. Вернуть радиус кривизны\n"
		"5. Вернуть координаты центра кривизны\n"
		"6. Вернуть площадь криволинейной трапеции\n";

	double a;
	sfio::safe_cin(L"Введите значение коэффициента 'a'", a, 
		std::initializer_list<double>{0}, L' ');
	curve::Catenary c(a);

	while (true)
	{
		std::wcout << msg << std::endl;

		int choice;
		sfio::safe_cin(L"Выберите функцию:", choice, 1, 6, L' ');

		double x;
		if (choice != get_trapeze_area && choice != exit)
			sfio::safe_cin(L"Введите значение 'x':", x, L' ');

		switch (choice)
		{
		case exit:
			return 0;

		case get_ordinate:
			std::wcout << L"Результат: ";
			std::cout << abs(c.y(x)) << std::endl;
			break;

		case get_arc_length:
			std::wcout << L"Результат: ";
			std::cout << c.l(x) << std::endl;
			break;

		case get_curvature_radius:
			std::wcout << L"Результат: ";
			std::cout << abs(c.R(x)) << std::endl;
			break;
		
		case get_trapeze_area:
			double x1, x2;
			sfio::safe_cin(L"Введите значение 'x1':", x1, L' ');
			sfio::safe_cin(L"Введите значение 'x2':", x2, L' ');
			std::wcout << L"Результат: ";
			std::cout << abs(c.S(x1, x2)) << std::endl;
			break;

		case get_curvature_center_coordinates:
			const curve::coord first_coord(c.CurvatureCenterCoords(x).first);
			const curve::coord second_coord(c.CurvatureCenterCoords(x).second);
			std::wcout << L"Результат:\n";
			std::cout << '(' << first_coord.first << "; " << first_coord.second << "),"
				<< "\n" << '(' << second_coord.first << "; " << second_coord.second << ')'
				<< std::endl;
			break;
		}
	}
}