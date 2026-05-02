#include <iostream>
#include "TicketBookingSystem.h"
#include "SalaryCalculationSystem.h"
#include "ATMSimulator.h"

int main()
{
	system("chcp 1251");
	setlocale(LC_ALL, "rus");
	srand(time(0));
	int issue_number;
	do
	{
		std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА 2 ===\n";
		std::cout << "1. Система бронирования билетов (Кино/Авиа/ЖД)\n";
		std::cout << "2. Система расчета зарплаты\n";
		std::cout << "3. Симулятор банкомата\n";
		std::cout << "0. Выход\n";
		std::cout << "Выберите действие:\n";
		std::cin >> issue_number;
		std::cin.ignore(10000, '\n');
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			issue_number = -1;
		}
		if (issue_number == 1)
		{
			TicketBookingSystem();
		}
		else if (issue_number == 2)
		{
			SalaryCalculationSystem();
		}
		else if (issue_number == 3)
		{
			ATMSimulator();
		}
		else if (issue_number != 0)
		{
			std::cout << "ОШИБКА: Введено некорректное число\n";
		}
	} while (issue_number != 0);
	return 0;

}