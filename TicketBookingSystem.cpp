#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <windows.h>

class Ticket {
private:
	static int total_tickets;
	int ticket_number;
	std::string date;
	double base_price;
public:
	Ticket(const std::string& date, double price)
		: ticket_number(++total_tickets), date(date), base_price(price) {
		std::cout << "\033[31mСоздан Ticket #" << ticket_number << "\033[0m\n";
	}
	virtual ~Ticket() {
		std::cout << "\033[31mУдален Ticket #" << ticket_number << "\033[0m\n";
	}
	virtual double CalculateFinalPrice() const = 0;
	virtual bool CanRefund() const = 0;
	virtual std::string GetDescription() const = 0;
	int GetTicketNumber() const { return ticket_number; }
	std::string GetDate() const { return date; }
	double GetBasePrice() const { return base_price; }
	virtual bool Refund() {
		if (CanRefund()) {
			std::cout << "Билет #" << ticket_number << " возвращен\n";
			return true;
		}
		std::cout << "Билет #" << ticket_number << " нельзя вернуть\n";
		return false;
	}
	Ticket& operator=(const Ticket& other) {
		if (this != &other) {
			date = other.date;
			base_price = other.base_price;
		}
		return *this;
	}
};

int Ticket::total_tickets = 0;

class MovieTicket : public Ticket {
private:
	std::string movie_name;
	int hall_number;
	int row_number;
	int seat_number;
public:
	MovieTicket(const std::string& date, double price, const std::string& movie, int hall, int row, int seat)
		: Ticket(date, price), movie_name(movie), hall_number(hall), row_number(row), seat_number(seat) {
		std::cout << "\033[31mСоздан MovieTicket\033[0m\n";
	}
	~MovieTicket() override {
		std::cout << "\033[31mУдален MovieTicket\033[0m\n";
	}
	double CalculateFinalPrice() const override {
		double final_price = GetBasePrice();
		final_price += GetBasePrice() * 0.1;
		return final_price;
	}
	bool CanRefund() const override {
		return true;
	}
	std::string GetDescription() const override {
		std::stringstream ss;
		ss << "Билет в кино #" << GetTicketNumber()
			<< " | Фильм: " << movie_name
			<< " | Дата: " << GetDate()
			<< " | Зал: " << hall_number
			<< " | Ряд: " << row_number
			<< " | Место: " << seat_number
			<< " | Цена: " << GetBasePrice() << " руб."
			<< " | Итоговая цена: " << CalculateFinalPrice() << " руб.";
		return ss.str();
	}
};

class FlightTicket : public Ticket {
private:
	std::string flight_number;
	std::string service_class;
	double baggage_weight;
public:
	FlightTicket(const std::string& date, double price, const std::string& flight, const std::string& s_class, double baggage)
		: Ticket(date, price), flight_number(flight), service_class(s_class), baggage_weight(baggage) {
		std::cout << "\033[31mСоздан FlightTicket\033[0m\n";
	}
	~FlightTicket() override {
		std::cout << "\033[31mУдален FlightTicket\033[0m\n";
	}
	double CalculateFinalPrice() const override {
		double final_price = GetBasePrice();
		if (service_class == "business") {
			final_price += GetBasePrice() * 0.5;
		}
		else if (service_class == "first") {
			final_price += GetBasePrice() * 1.0;
		}
		if (baggage_weight > 20) {
			final_price += (baggage_weight - 20) * 50;
		}
		final_price += GetBasePrice() * 0.15;
		return final_price;
	}
	bool CanRefund() const override {
		return service_class != "first";
	}
	std::string GetDescription() const override {
		std::stringstream ss;
		ss << "Авиабилет #" << GetTicketNumber()
			<< " | Рейс: " << flight_number
			<< " | Дата: " << GetDate()
			<< " | Класс: " << service_class
			<< " | Багаж: " << baggage_weight << "кг"
			<< " | Цена: " << GetBasePrice() << " руб."
			<< " | Итоговая цена: " << CalculateFinalPrice() << " руб.";
		return ss.str();
	}
};

class TrainTicket : public Ticket {
private:
	std::string train_number;
	int carriage_number;
	std::string seat_type;
public:
	TrainTicket(const std::string& date, double price, const std::string& train, int carriage, const std::string& seat_t)
		: Ticket(date, price), train_number(train), carriage_number(carriage), seat_type(seat_t) {
		std::cout << "\033[31mСоздан TrainTicket\033[0m\n";
	}
	~TrainTicket() override {
		std::cout << "\033[31mУдален TrainTicket\033[0m\n";
	}
	double CalculateFinalPrice() const override {
		double final_price = GetBasePrice();
		if (seat_type == "compartment") {
			final_price += GetBasePrice() * 0.3;
		}
		else if (seat_type == "lux") {
			final_price += GetBasePrice() * 0.8;
		}
		final_price += GetBasePrice() * 0.05;
		return final_price;
	}
	bool CanRefund() const override {
		return seat_type != "lux";
	}
	std::string GetDescription() const override {
		std::stringstream ss;
		ss << "ЖД билет #" << GetTicketNumber()
			<< " | Поезд: " << train_number
			<< " | Дата: " << GetDate()
			<< " | Вагон: " << carriage_number
			<< " | Тип места: " << seat_type
			<< " | Цена: " << GetBasePrice() << " руб."
			<< " | Итоговая цена: " << CalculateFinalPrice() << " руб.";
		return ss.str();
	}
};

class BookingSystem {
private:
	std::vector<Ticket*> tickets;
public:
	BookingSystem() {
		std::cout << "\033[31mСоздана BookingSystem\033[0m\n";
	}
	~BookingSystem() {
		std::cout << "\033[31mУдален BookingSystem\033[0m\n";
	}
	void AddTicket(Ticket* ticket) {
		tickets.push_back(ticket);
		std::cout << "Билет добавлен! Итоговая цена: "
			<< tickets.back()->CalculateFinalPrice() << " руб.\n";
	}
	void ShowAllTickets() const {
		if (tickets.empty()) {
			std::cout << "Нет добавленных билетов\n";
			return;
		}
		std::cout << "= СПИСОК ВСЕХ БИЛЕТОВ =\n";
		for (Ticket* ticket : tickets) {
			std::cout << ticket->GetDescription() << "\n";
		}
	}
	bool CancelBooking(int ticket_number) {
		auto it = std::find_if(tickets.begin(), tickets.end(),
			[ticket_number](const Ticket* ticket) {
				return ticket->GetTicketNumber() == ticket_number;
			});
		if (it != tickets.end()) {
			if ((*it)->Refund()) {
				delete* it;
				tickets.erase(it);
				std::cout << "Бронирование отменено\n";
				return true;
			}
			else {
				std::cout << "Невозможно отменить бронирование\n";
				return false;
			}
		}
		std::cout << "Билет #" << ticket_number << " не найден\n";
		return false;
	}
	double CalculateTotalRevenue() const {
		double total = 0;
		for (const auto& ticket : tickets) {
			total += ticket->CalculateFinalPrice();
		}
		return total;
	}
	void ShowTotalRevenue() const {
		std::cout << "Общая выручка: " << std::fixed << std::setprecision(2)
			<< CalculateTotalRevenue() << " руб.\n";
	}
	bool HasTickets() const {
		return !tickets.empty();
	}
};

bool IsValidDate(const std::string& date) {
	if (date.length() != 10 || date[2] != '.' || date[5] != '.') return false;
	for (int i = 0; i < 10; i++) {
		if (i == 2 || i == 5) continue;
		if (!isdigit(date[i])) return false;
	}
	int day = (date[0] - '0') * 10 + (date[1] - '0');
	int month = (date[3] - '0') * 10 + (date[4] - '0');
	int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');
	if (year < 1900 || year > 2100) return false;
	if (month < 1 || month > 12) return false;
	int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (month == 2) {
		bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (is_leap) days_in_month[1] = 29;
	}
	return (day >= 1 && day <= days_in_month[month - 1]);
}

bool RepeatTask() {
	int issue_number;
	do
	{
		std::cout << "Задача завершена\n";
		std::cout << "1. Выполнить задачу еще раз\n";
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
		if (issue_number != 1 && issue_number != 0)
		{
			std::cout << "ОШИБКА: Введено некорректное число\n";
		}
	} while (issue_number != 1 && issue_number != 0);
	return issue_number == 1;
}

void TicketBookingSystem() {
	BookingSystem booking_system;
	int issue_number;
	do {
		std::cout << "== СИСТЕМА БРОНИРОВАНИЯ БИЛЕТОВ ==\n";
		std::cout << "1. Добавить билет в кино\n";
		std::cout << "2. Добавить авиабилет\n";
		std::cout << "3. Добавить ЖД билет\n";
		std::cout << "4. Показать все билеты\n";
		std::cout << "5. Отменить бронирование\n";
		std::cout << "6. Общая выручка\n";
		std::cout << "0. Выход\n";
		std::cout << "Выберите действие:\n";
		std::cin >> issue_number;
		std::cin.ignore(10000, '\n');
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "ОШИБКА: Введите число\n";
			continue;
		}
		do {
			if (issue_number == 0) {
				break;
			}
			if (issue_number == 1 || issue_number == 2 || issue_number == 3) {
				std::string date;
				double base_price;
				do {
					std::cout << "Введите дату (ДД.ММ.ГГГГ):\n";
					std::getline(std::cin, date);

					if (!IsValidDate(date)) {
						std::cout << "ОШИБКА: Неверный формат даты! Используйте ДД.ММ.ГГГГ\n";
						continue;
					}
					break;
				} while (true);
				do {
					std::cout << "Введите базовую цену:\n";
					std::cin >> base_price;
					std::cin.ignore(10000, '\n');

					if (std::cin.fail()) {
						std::cin.clear();
						std::cin.ignore(10000, '\n');
						std::cout << "ОШИБКА: Введите число\n";
						continue;
					}

					if (base_price <= 0) {
						std::cout << "ОШИБКА: Цена должна быть положительной\n";
						continue;
					}
					break;
				} while (true);
				if (issue_number == 1) {
					std::string movie_name;
					int hall, row, seat;
					std::cout << "Введите название фильма:\n";
					std::getline(std::cin, movie_name);
					do {
						std::cout << "Введите номер зала:\n";
						std::cin >> hall;
						std::cin.ignore(10000, '\n');
						if (std::cin.fail() || hall <= 0) {
							std::cin.clear();
							std::cin.ignore(10000, '\n');
							std::cout << "ОШИБКА: Введите положительное число\n";
							continue;
						}
						break;
					} while (true);
					do {
						std::cout << "Введите номер ряда:\n";
						std::cin >> row;
						std::cin.ignore(10000, '\n');
						if (std::cin.fail() || row <= 0) {
							std::cin.clear();
							std::cin.ignore(10000, '\n');
							std::cout << "ОШИБКА: Введите положительное число\n";
							continue;
						}
						break;
					} while (true);
					do {
						std::cout << "Введите номер места:\n";
						std::cin >> seat;
						std::cin.ignore(10000, '\n');
						if (std::cin.fail() || seat <= 0) {
							std::cin.clear();
							std::cin.ignore(10000, '\n');
							std::cout << "ОШИБКА: Введите положительное число\n";
							continue;
						}
						break;
					} while (true);
					booking_system.AddTicket(new MovieTicket(date, base_price, movie_name, hall, row, seat));
					if (RepeatTask()) {
						continue;
					}
					else {
						break;
					}
				}
				else if (issue_number == 2) {
					std::string flight_number, service_class;
					double baggage_weight;
					std::cout << "Введите номер рейса:\n";
					std::getline(std::cin, flight_number);
					do {
						std::cout << "Введите класс обслуживания (economy/business/first):\n";
						std::getline(std::cin, service_class);
						if (service_class != "economy" && service_class != "business" && service_class != "first") {
							std::cout << "ОШИБКА: Доступные классы: economy, business, first\n";
							continue;
						}
						break;
					} while (true);
					do {
						std::cout << "Введите вес багажа (кг):\n";
						std::cin >> baggage_weight;
						std::cin.ignore(10000, '\n');
						if (std::cin.fail() || baggage_weight < 0) {
							std::cin.clear();
							std::cin.ignore(10000, '\n');
							std::cout << "ОШИБКА: Введите неотрицательное число\n";
							continue;
						}
						break;
					} while (true);
					booking_system.AddTicket(new FlightTicket(date, base_price, flight_number, service_class, baggage_weight));
					if (RepeatTask()) {
						continue;
					}
					else {
						break;
					}
				}
				else if (issue_number == 3) {
					std::string train_number, seat_type;
					int carriage_number;
					std::cout << "Введите номер поезда:\n";
					std::getline(std::cin, train_number);
					do {
						std::cout << "Введите номер вагона:\n";
						std::cin >> carriage_number;
						std::cin.ignore(10000, '\n');

						if (std::cin.fail() || carriage_number <= 0) {
							std::cin.clear();
							std::cin.ignore(10000, '\n');
							std::cout << "ОШИБКА: Введите положительное число\n";
							continue;
						}
						break;
					} while (true);
					do {
						std::cout << "Введите тип места (reserved_seat/compartment/lux):\n";
						std::getline(std::cin, seat_type);
						if (seat_type != "reserved_seat" && seat_type != "compartment" && seat_type != "lux") {
							std::cout << "ОШИБКА: Доступные типы: reserved_seat, compartment, lux\n";
							continue;
						}
						break;
					} while (true);
					booking_system.AddTicket(new TrainTicket(date, base_price, train_number, carriage_number, seat_type));
					if (RepeatTask()) {
						continue;
					}
					else {
						break;
					}
				}
			}
			else if (issue_number == 4) {
				booking_system.ShowAllTickets();
				booking_system.ShowTotalRevenue();
				if (RepeatTask()) {
					continue;
				}
				else {
					break;
				}
			}
			else if (issue_number == 5) {
				booking_system.ShowAllTickets();
				int ticket_num;
				std::cout << "Введите номер билета для отмены:\n";
				std::cin >> ticket_num;
				std::cin.ignore(10000, '\n');
				booking_system.CancelBooking(ticket_num);
				if (RepeatTask()) {
					continue;
				}
				else {
					break;
				}
			}
			else if (issue_number == 6) {
				booking_system.ShowTotalRevenue();
				if (RepeatTask()) {
					continue;
				}
				else {
					break;
				}
			}
			else {
				std::cout << "ОШИБКА: Выберите пункт от 0 до 6\n";
				continue;
			}
		} while (issue_number != 0);
	} while (issue_number != 0);
}