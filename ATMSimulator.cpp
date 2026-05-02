#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "TicketBookingSystem.h"

class InvalidPinException : public std::exception {
public:
	const char* what() const noexcept override {
		return "ОШИБКА: Неверный PIN-код";
	}
};

class InsufficientFundsException : public std::exception {
public:
	const char* what() const noexcept override {
		return "ОШИБКА: Недостаточно средств на счете";
	}
};

class CardBlockedException : public std::exception {
public:
	const char* what() const noexcept override {
		return "ОШИБКА: Карта заблокирована из-за превышения числа попыток ввода PIN";
	}
};

class BankCard {
protected:
	std::string card_number;
	std::string pin_code;
	double balance;
	std::string owner_name;
	int remaining_attempts;
	bool is_blocked;
public:
	BankCard() : card_number(""), pin_code(""), balance(0.0),
		owner_name(""), remaining_attempts(3), is_blocked(false) {
		std::cout << "\033[31mСоздан BankCard\033[0m\n";
	}
	BankCard(const std::string& number, const std::string& pin, double bal, const std::string& owner)
		: card_number(number), pin_code(pin), balance(bal), owner_name(owner),
		remaining_attempts(3), is_blocked(false) {
		std::cout << "\033[31mСоздан BankCard\033[0m\n";
	}
	virtual ~BankCard() {
		std::cout << "\033[31mУдален BankCard\033[0m\n";
	}
	virtual bool VerifyPin(const std::string& entered_pin) {
		if (is_blocked) {
			throw CardBlockedException();
		}
		if (entered_pin != pin_code) {
			remaining_attempts--;
			std::cout << "Неверный PIN. Осталось попыток: " << remaining_attempts << "\n";
			if (remaining_attempts <= 0) {
				is_blocked = true;
				throw CardBlockedException();
			}
			return false;
		}
		remaining_attempts = 3;
		return true;
	}
	virtual void Withdraw(double amount) = 0;
	virtual void Deposit(double amount) = 0;
	virtual std::string GetCardType() const = 0;
	virtual std::string GetBonusInfo() const { return "Бонусная программа не поддерживается."; }
	std::string GetNumber() const { return card_number; }
	std::string GetOwner() const { return owner_name; }
	double GetBalance() const { return balance; }
	bool IsBlocked() const { return is_blocked; }
	int GetRemainingAttempts() const { return remaining_attempts; }
	void UnblockCard(const std::string& new_pin) {
		is_blocked = false;
		remaining_attempts = 3;
		pin_code = new_pin;
		std::cout << "Карта разблокирована\n";
	}
};

class DebitCard : public BankCard {
public:
	DebitCard() : BankCard() {
		std::cout << "\033[31mСоздан DebitCard\033[0m\n";
	}
	DebitCard(const std::string& number, const std::string& pin, double bal, const std::string& owner)
		: BankCard(number, pin, bal, owner) {
		std::cout << "\033[31mСоздан DebitCard\033[0m\n";
	}
	~DebitCard() {
		std::cout << "\033[31mУдален DebitCard\033[0m\n";
	}
	void Withdraw(double amount) override {
		if (amount <= 0) throw std::invalid_argument("Сумма должна быть положительной");
		if (balance < amount) throw InsufficientFundsException();
		balance -= amount;
	}
	void Deposit(double amount) override {
		if (amount <= 0) throw std::invalid_argument("Сумма должна быть положительной");
		balance += amount;
	}
	std::string GetCardType() const override { return "Дебетовая"; }
};

class CreditCard : public BankCard {
private:
	double credit_limit;
public:
	CreditCard() : BankCard(), credit_limit(0.0) {
		std::cout << "\033[31mСоздан CreditCard\033[0m\n";
	}
	CreditCard(const std::string& number, const std::string& pin, double bal, const std::string& owner, double limit)
		: BankCard(number, pin, bal, owner), credit_limit(limit) {
		std::cout << "\033[31mСоздан CreditCard\033[0m\n";
	}
	~CreditCard() {
		std::cout << "\033[31mУдален CreditCard\033[0m\n";
	}
	void Withdraw(double amount) override {
		if (amount <= 0) throw std::invalid_argument("Сумма должна быть положительной");
		if (balance - amount < -credit_limit) throw InsufficientFundsException();
		balance -= amount;
	}
	void Deposit(double amount) override {
		if (amount <= 0) throw std::invalid_argument("Сумма должна быть положительной");
		balance += amount;
	}
	std::string GetCardType() const override { return "Кредитная"; }
	double GetCreditLimit() const { return credit_limit; }
};

class BonusCard : public BankCard {
private:
	double bonus_points;
public:
	BonusCard() : BankCard(), bonus_points(0.0) {
		std::cout << "\033[31mСоздан BonusCard\033[0m\n";
	}
	BonusCard(const std::string& number, const std::string& pin, double bal, const std::string& owner, double points)
		: BankCard(number, pin, bal, owner), bonus_points(points) {
		std::cout << "\033[31mСоздан BonusCard\033[0m\n";
	}
	~BonusCard() {
		std::cout << "\033[31mУдален BonusCard\033[0m\n";
	}
	std::string GetBonusInfo() const override {
		std::ostringstream oss;
		oss << "Бонусов доступно: " << std::fixed << std::setprecision(2) << bonus_points;
		return oss.str();
	}
	void Withdraw(double amount) override {
		if (amount <= 0) throw std::invalid_argument("Сумма должна быть положительной");
		if (balance < amount) throw InsufficientFundsException();
		balance -= amount;
		bonus_points += amount * 0.01;
	}
	void Deposit(double amount) override {
		if (amount <= 0) throw std::invalid_argument("Сумма должна быть положительной");
		balance += amount;
		bonus_points += amount * 0.02;
	}
	std::string GetCardType() const override { return "Бонусная"; }
};

class ATM {
private:
	BankCard* inserted_card;
	std::vector<BankCard*> card_database;
	std::string MaskCardNumber(const std::string& number) {
		if (number.length() < 4) return "****";
		return "**** **** **** " + number.substr(number.length() - 4);
	}
	bool IsValidCardNumber(const std::string& number) {
		if (number.length() != 16) return false;
		for (char c : number) {
			if (!isdigit(c)) return false;
		}
		return true;
	}
	std::string FormatCardNumber(const std::string& number) {
		std::string formatted;
		for (size_t i = 0; i < number.length(); ++i) {
			if (i > 0 && i % 4 == 0) formatted += " ";
			formatted += number[i];
		}
		return formatted;
	}
	BankCard* FindCardByNumber(const std::string& number) {
		std::string clean_number = number;
		clean_number.erase(std::remove(clean_number.begin(), clean_number.end(), ' '), clean_number.end());
		for (auto card : card_database) {
			std::string stored_number = card->GetNumber();
			stored_number.erase(std::remove(stored_number.begin(), stored_number.end(), ' '), stored_number.end());
			if (stored_number == clean_number) {
				return card;
			}
		}
		return nullptr;
	}
public:
	ATM() : inserted_card(nullptr) {
		std::cout << "\033[31mСоздан ATM\033[0m\n";
	}
	~ATM() {
		std::cout << "\033[31mУдален ATM\033[0m\n";
		for (auto card : card_database) {
			delete card;
		}
		card_database.clear();
	}
	void AddCard() {
		std::cout << "= ДОБАВЛЕНИЕ НОВОЙ КАРТЫ =\n";
		int card_type;
		do {
			std::cout << "1. Дебетовая\n";
			std::cout << "2. Кредитная\n";
			std::cout << "3. Бонусная\n";
			std::cout << "Выберите тип карты:\n";
			std::cin >> card_type;
			std::cin.ignore(10000, '\n');
			if (std::cin.fail() || card_type < 1 || card_type > 3) {
				std::cin.clear();
				std::cout << "ОШИБКА: Введите число от 1 до 3\n";
				continue;
			}
			break;
		} while (true);
		std::string card_number;
		do {
			std::cout << "Введите номер карты:\n";
			std::getline(std::cin, card_number);
			card_number.erase(std::remove(card_number.begin(), card_number.end(), ' '), card_number.end());
			if (!IsValidCardNumber(card_number)) {
				std::cout << "ОШИБКА: Номер карты должен содержать ровно 16 цифр\n";
				continue;
			}
			if (FindCardByNumber(card_number) != nullptr) {
				std::cout << "ОШИБКА: Карта с таким номером уже существует\n";
				continue;
			}
			break;
		} while (true);
		std::string pin_code;
		do {
			std::cout << "Введите PIN-код:\n";
			std::getline(std::cin, pin_code);
			if (pin_code.length() != 4 || !std::all_of(pin_code.begin(), pin_code.end(), ::isdigit)) {
				std::cout << "ОШИБКА: PIN-код должен содержать ровно 4 цифры\n";
				continue;
			}
			break;
		} while (true);
		std::string owner_name;
		do {
			std::cout << "Введите имя владельца:\n";
			std::getline(std::cin, owner_name);
			if (owner_name.empty()) {
				std::cout << "ОШИБКА: Имя не может быть пустым\n";
				continue;
			}
			break;
		} while (true);
		double initial_balance;
		do {
			std::cout << "Введите начальный баланс:\n";
			std::cin >> initial_balance;
			std::cin.ignore(10000, '\n');
			if (std::cin.fail() || initial_balance < 0) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				std::cout << "ОШИБКА: Баланс должен быть неотрицательным числом\n";
				continue;
			}
			break;
		} while (true);
		BankCard* new_card = nullptr;
		std::string formatted_number = FormatCardNumber(card_number);
		if (card_type == 1) {
			new_card = new DebitCard(formatted_number, pin_code, initial_balance, owner_name);
		}
		else if (card_type == 2) {
			double credit_limit;
			do {
				std::cout << "Введите кредитный лимит:\n";
				std::cin >> credit_limit;
				std::cin.ignore(10000, '\n');
				if (std::cin.fail() || credit_limit <= 0) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
					std::cout << "ОШИБКА: Кредитный лимит должен быть положительным числом\n";
					continue;
				}
				break;
			} while (true);
			new_card = new CreditCard(formatted_number, pin_code, initial_balance, owner_name, credit_limit);
		}
		else if (card_type == 3) {
			double bonus_points;
			do {
				std::cout << "Введите начальное количество бонусов:\n";
				std::cin >> bonus_points;
				std::cin.ignore(10000, '\n');
				if (std::cin.fail() || bonus_points < 0) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
					std::cout << "ОШИБКА: Количество бонусов не может быть отрицательным\n";
					continue;
				}
				break;
			} while (true);
			new_card = new BonusCard(formatted_number, pin_code, initial_balance, owner_name, bonus_points);
		}
		card_database.push_back(new_card);
	}
	void ShowAllCards() {
		if (card_database.empty()) {
			std::cout << "База данных карт пуста.\n";
			return;
		}
		for (size_t i = 0; i < card_database.size(); ++i) {
			std::cout << card_database[i]->GetOwner()
				<< " | " << MaskCardNumber(card_database[i]->GetNumber())
				<< " | " << card_database[i]->GetCardType();
			if (card_database[i]->IsBlocked()) {
				std::cout << " [ЗАБЛОКИРОВАНА]";
			}
			std::cout << "\n";
		}
	}
	void DeleteCard() {
		if (card_database.empty()) {
			std::cout << "База данных карт пуста.\n";
			return;
		}
		ShowAllCards();
		std::string card_number;
		std::cout << "Введите номер карты для удаления:\n";
		std::getline(std::cin, card_number);
		BankCard* found_card = FindCardByNumber(card_number);
		if (!found_card) {
			std::cout << "ОШИБКА: Карта с таким номером не найдена\n";
			return;
		}
		auto it = std::find(card_database.begin(), card_database.end(), found_card);
		if (it != card_database.end()) {
			card_database.erase(it);
			delete found_card;
		}
	}
	void UnblockCard() {
		if (card_database.empty()) {
			std::cout << "База данных карт пуста.\n";
			return;
		}
		ShowAllCards();
		std::string card_number;
		std::cout << "Введите номер карты для разблокировки:\n";
		std::getline(std::cin, card_number);
		BankCard* found_card = FindCardByNumber(card_number);
		if (!found_card) {
			std::cout << "ОШИБКА: Карта с таким номером не найдена\n";
			return;
		}
		if (!found_card->IsBlocked()) {
			std::cout << "Карта не заблокирована\n";
			return;
		}
		std::string new_pin;
		do {
			std::cout << "Введите новый PIN-код:\n";
			std::getline(std::cin, new_pin);
			if (new_pin.length() != 4 || !std::all_of(new_pin.begin(), new_pin.end(), ::isdigit)) {
				std::cout << "ОШИБКА: PIN-код должен содержать ровно 4 цифры\n";
				continue;
			}
			break;
		} while (true);
		found_card->UnblockCard(new_pin);
	}
	bool InsertCard() {
		if (card_database.empty()) {
			std::cout << "База данных карт пуста. Сначала добавьте карту\n";
			return false;
		}
		ShowAllCards();
		std::string card_number;
		std::cout << "Введите номер карты:\n";
		std::getline(std::cin, card_number);
		BankCard* found_card = FindCardByNumber(card_number);
		if (!found_card) {
			std::cout << "ОШИБКА: Карта с таким номером не найдена\n";
			return false;
		}
		if (found_card->IsBlocked()) {
			std::cout << "ОШИБКА: Карта заблокирована\n";
			return false;
		}
		bool pin_verified = false;
		while (!pin_verified && !found_card->IsBlocked()) {
			std::cout << "Введите PIN-код:\n";
			std::string pin_input;
			std::getline(std::cin, pin_input);
			try {
				pin_verified = found_card->VerifyPin(pin_input);
			}
			catch (const CardBlockedException& e) {
				std::cout << e.what() << "\n";
				return false;
			}
		}
		if (pin_verified) {
			inserted_card = found_card;
			std::cout << "Добро пожаловать, " << inserted_card->GetOwner() << "!\n";
			ShowBalanceInfo();
			return true;
		}
		return false;
	}
	void ShowBalanceInfo() {
		std::cout << "Владелец: " << inserted_card->GetOwner() << "\n";
		std::cout << "Номер карты: " << MaskCardNumber(inserted_card->GetNumber()) << "\n";
		std::cout << "Баланс: " << std::fixed << std::setprecision(2) << inserted_card->GetBalance() << " руб.\n";
		std::cout << "Тип карты: " << inserted_card->GetCardType() << "\n";
		if (auto credit = dynamic_cast<CreditCard*>(inserted_card)) {
			std::cout << "Кредитный лимит: " << credit->GetCreditLimit() << " руб.\n";
		}
		std::string bonus_info = inserted_card->GetBonusInfo();
		if (bonus_info != "Бонусная программа не поддерживается.") {
			std::cout << bonus_info << "\n";
		}
	}
	void WithdrawCash() {
		double amount;
		std::cout << "Введите сумму для снятия:\n";
		std::cin >> amount;
		std::cin.ignore(10000, '\n');
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "ОШИБКА: Некорректная сумма.\n";
			return;
		}
		try {
			inserted_card->Withdraw(amount);
			std::cout << "Новый баланс: " << std::fixed << std::setprecision(2) << inserted_card->GetBalance() << " руб.\n";
		}
		catch (const InsufficientFundsException& e) {
			std::cout << e.what() << "\n";
		}
		catch (const std::exception& e) {
			std::cout << "ОШИБКА: " << e.what() << "\n";
		}
	}
	void DepositCash() {
		double amount;
		std::cout << "Введите сумму для внесения:\n";
		std::cin >> amount;
		std::cin.ignore(10000, '\n');
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "ОШИБКА: Некорректная сумма.\n";
			return;
		}
		try {
			inserted_card->Deposit(amount);
			std::cout << "Новый баланс: " << std::fixed << std::setprecision(2) << inserted_card->GetBalance() << " руб.\n";
		}
		catch (const std::exception& e) {
			std::cout << "ОШИБКА: " << e.what() << "\n";
		}
	}
	bool IsCardInserted() const { return inserted_card != nullptr; }
};

void ATMSimulator() {
	int issue_number;
	ATM atm;
	do {
		std::cout << "== СИМУЛЯТОР РАБОТЫ БАНКОМАТА ==\n";
		std::cout << "1. Добавить карту\n";
		std::cout << "2. Вставить карту\n";
		std::cout << "3. Разблокировать карту\n";
		std::cout << "4. Удалить карту\n";
		std::cout << "0. Выход\n";
		std::cout << "Выберите действие:\n";
		std::cin >> issue_number;
		std::cin.ignore(10000, '\n');
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			issue_number = -1;
		}
		do {
			if (issue_number == 1) {
				atm.AddCard();
			}
			else if (issue_number == 2) {
				if (atm.InsertCard()) {
					int atm_action;
					do {
						std::cout << "1. Снять наличные\n";
						std::cout << "2. Внести наличные\n";
						std::cout << "3. Показать баланс\n";
						std::cout << "0. Выход\n";
						std::cout << "Выберите действие:\n";
						std::cin >> atm_action;
						std::cin.ignore(10000, '\n');
						if (std::cin.fail()) {
							std::cin.clear();
							std::cin.ignore(10000, '\n');
							atm_action = -1;
						}
						if (atm_action == 1) {
							atm.WithdrawCash();
						}
						else if (atm_action == 2) {
							atm.DepositCash();
						}
						else if (atm_action == 3) {
							atm.ShowBalanceInfo();
						}
						else if (atm_action != 0) {
							std::cout << "ОШИБКА: Введено некорректное число\n";
						}
					} while (atm_action != 0 && atm.IsCardInserted());
				}
			}
			else if (issue_number == 3) {
				atm.UnblockCard();
			}
			else if (issue_number == 4) {
				atm.DeleteCard();
			}
			else if (issue_number != 0) {
				std::cout << "ОШИБКА: Введено некорректное число\n";
				break;
			}
			if (issue_number == 0) {
				break;
			}
			if (issue_number != 2 && RepeatTask()) {
				continue;
			}
			else {
				break;
			}
		} while (issue_number != 0);
	} while (issue_number != 0);
}