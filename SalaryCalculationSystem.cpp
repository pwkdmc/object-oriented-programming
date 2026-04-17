#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include "TicketBookingSystem.hpp"

class Employee {
protected:
    std::string name_;
    int id_;
    double base_rate_;
    static int total_employees_count_;
public:
    Employee() : name_("Unknown"), id_(0), base_rate_(0.0) {
        total_employees_count_++;
        std::cout << "\033[31mСоздан Employee\033[0m\n";
    }
    Employee(const std::string& name, int id, double base_rate)
        : name_(name), id_(id), base_rate_(base_rate) {
        total_employees_count_++;
        std::cout << "\033[31mСоздан Employee\033[0m\n";
    }
    Employee(const Employee& other)
        : name_(other.name_), id_(other.id_), base_rate_(other.base_rate_) {
        total_employees_count_++;
        std::cout << "\033[31mСоздан Employee\033[0m\n";
    }
    virtual ~Employee() {
        total_employees_count_--;
        std::cout << "\033[31mУдален Employee\033[0m\n";
    }
    virtual double CalculateSalary() const = 0;
    std::string GetName() const { return name_; }
    int GetId() const { return id_; }
    double GetBaseRate() const { return base_rate_; }
    void SetName(const std::string& name) { name_ = name; }
    void SetId(int id) { id_ = id; }
    void SetBaseRate(double rate) { base_rate_ = rate; }
    static int GetTotalEmployeesCount() { return total_employees_count_; }
    virtual void SaveToFile(std::ofstream& file) const {
        file << "Employee," << id_ << "," << name_ << "," << base_rate_;
    }
    virtual void LoadFromFile(const std::string& data) {
        std::stringstream ss(data);
        std::string type;
        std::getline(ss, type, ',');
        ss >> id_;
        ss.ignore();
        std::getline(ss, name_, ',');
        ss >> base_rate_;
    }
    virtual std::string GetDescription() const = 0;
};

int Employee::total_employees_count_ = 0;

class SalariedEmployee : public Employee {
private:
    double bonus_;
public:
    SalariedEmployee() : Employee(), bonus_(0.0) {
        std::cout << "\033[31mСоздан SalariedEmployee\033[0m\n";
    }
    SalariedEmployee(const std::string& name, int id, double salary, double bonus)
        : Employee(name, id, salary), bonus_(bonus) {
        std::cout << "\033[31mСоздан SalariedEmployee\033[0m\n";
    }
    SalariedEmployee(const SalariedEmployee& other)
        : Employee(other), bonus_(other.bonus_) {
        std::cout << "\033[31mСоздан SalariedEmployee\033[0m\n";
    }
    ~SalariedEmployee() override {
        std::cout << "\033[31mУдален SalariedEmployee\033[0m\n";
    }
    double CalculateSalary() const override {
        return base_rate_ + bonus_;
    }
    void SaveToFile(std::ofstream& file) const override {
        file << "SalariedEmployee," << id_ << "," << name_ << "," << base_rate_ << "," << bonus_;
    }
    void LoadFromFile(const std::string& data) override {
        std::stringstream ss(data);
        std::string type;
        std::getline(ss, type, ',');
        ss >> id_;
        ss.ignore();
        std::getline(ss, name_, ',');
        ss >> base_rate_;
        ss.ignore();
        ss >> bonus_;
    }
    std::string GetDescription() const override {
        std::stringstream ss;
        ss << "Штатный сотрудник | ID: " << this->GetId()
            << " | Имя: " << this->GetName()
            << " | Зарплата: " << this->CalculateSalary() << " руб.\n";
        return ss.str();
    }
};

class HourlyEmployee : public Employee {
private:
    int hours_worked_;
public:
    HourlyEmployee() : Employee(), hours_worked_(0) {
        std::cout << "\033[31mСоздан HourlyEmployee\033[0m\n";
    }
    HourlyEmployee(const std::string& name, int id, double hourly_rate, int hours)
        : Employee(name, id, hourly_rate), hours_worked_(hours) {
        std::cout << "\033[31mСоздан HourlyEmployee\033[0m\n";
    }
    HourlyEmployee(const HourlyEmployee& other)
        : Employee(other), hours_worked_(other.hours_worked_) {
        std::cout << "\033[31mСоздан HourlyEmployee\033[0m\n";
    }
    ~HourlyEmployee() override {
        std::cout << "\033[31mУдален HourlyEmployee\033[0m\n";
    }
    double CalculateSalary() const override {
        return base_rate_ * hours_worked_;
    }
    void SaveToFile(std::ofstream& file) const override {
        file << "HourlyEmployee," << id_ << "," << name_ << "," << base_rate_ << "," << hours_worked_;
    }
    void LoadFromFile(const std::string& data) override {
        std::stringstream ss(data);
        std::string type;
        std::getline(ss, type, ',');
        ss >> id_;
        ss.ignore();
        std::getline(ss, name_, ',');
        ss >> base_rate_;
        ss.ignore();
        ss >> hours_worked_;
    }
    std::string GetDescription() const override {
        std::stringstream ss;
        ss << "Почасовик | ID: " << this->GetId()
            << " | Имя: " << this->GetName()
            << " | Зарплата: " << this->CalculateSalary() << " руб.\n";
        return ss.str();
    }
};

class Manager : public Employee {
private:
    double sales_amount_;
    double commission_percent_;
public:
    Manager() : Employee(), sales_amount_(0.0), commission_percent_(0.0) {
        std::cout << "\033[31mСоздан Manager\033[0m\n";
    }
    Manager(const std::string& name, int id, double salary, double sales, double commission)
        : Employee(name, id, salary), sales_amount_(sales), commission_percent_(commission) {
        std::cout << "\033[31mСоздан Manager\033[0m\n";
    }
    Manager(const Manager& other)
        : Employee(other), sales_amount_(other.sales_amount_), commission_percent_(other.commission_percent_) {
        std::cout << "\033[31mСоздан Manager\033[0m\n";
    }
    ~Manager() override {
        std::cout << "\033[31mУдален Manager\033[0m\n";
    }
    double CalculateSalary() const override {
        return base_rate_ + (sales_amount_ * commission_percent_ / 100.0);
    }
    void SaveToFile(std::ofstream& file) const override {
        file << "Manager," << id_ << "," << name_ << "," << base_rate_ << "," << sales_amount_ << "," << commission_percent_;
    }
    void LoadFromFile(const std::string& data) override {
        std::stringstream ss(data);
        std::string type;
        std::getline(ss, type, ',');
        ss >> id_;
        ss.ignore();
        std::getline(ss, name_, ',');
        ss >> base_rate_;
        ss.ignore();
        ss >> sales_amount_;
        ss.ignore();
        ss >> commission_percent_;
    }
    std::string GetDescription() const override {
        std::stringstream ss;
        ss << "Менеджер | ID: " << this->GetId()
            << " | Имя: " << this->GetName()
            << " | Зарплата: " << this->CalculateSalary() << " руб.\n";
        return ss.str();
    }
};

class Freelancer : public Employee {
private:
    double project_payment_;
public:
    Freelancer() : Employee(), project_payment_(0.0) {
        std::cout << "\033[31mСоздан Freelancer\033[0m\n";
    }
    Freelancer(const std::string& name, int id, double project_payment)
        : Employee(name, id, 0.0), project_payment_(project_payment) {
        std::cout << "\033[31mСоздан Freelancer\033[0m\n";
    }
    Freelancer(const Freelancer& other)
        : Employee(other), project_payment_(other.project_payment_) {
        std::cout << "\033[31mСоздан Freelancer\033[0m\n";
    }
    ~Freelancer() override {
        std::cout << "\033[31mУдален Freelancer\033[0m\n";
    }
    double CalculateSalary() const override {
        return project_payment_;
    }
    void SaveToFile(std::ofstream& file) const override {
        file << "Freelancer," << id_ << "," << name_ << "," << project_payment_;
    }
    void LoadFromFile(const std::string& data) override {
        std::stringstream ss(data);
        std::string type;
        std::getline(ss, type, ',');
        ss >> id_;
        ss.ignore();
        std::getline(ss, name_, ',');
        ss >> project_payment_;
    }
    std::string GetDescription() const override {
        std::stringstream ss;
        ss << "Фрилансер | ID: " << this->GetId()
            << " | Имя: " << this->GetName()
            << " | Зарплата: " << this->CalculateSalary() << " руб.\n";
        return ss.str();
    }
};

class PayrollSystem {
private:
    std::vector<Employee*> employees_;
    std::string filename_;
    Employee* CreateEmployeeFromString(const std::string& data) {
        std::stringstream ss(data);
        std::string type;
        std::getline(ss, type, ',');
        Employee* employee = nullptr;
        if (type == "SalariedEmployee") {
            employee = new SalariedEmployee();
        }
        else if (type == "HourlyEmployee") {
            employee = new HourlyEmployee();
        }
        else if (type == "Manager") {
            employee = new Manager();
        }
        else if (type == "Freelancer") {
            employee = new Freelancer();
        }
        else {
            return nullptr;
        }
        employee->LoadFromFile(data);
        return employee;
    }
public:
    PayrollSystem(const std::string& filename = "employees.txt") : filename_(filename) {
        std::cout << "\033[31mСоздан PayrollSystem\033[0m\n";
    }
    ~PayrollSystem() {
        SaveToFile();
        for (Employee* emp : employees_) {
            delete emp;
        }
        employees_.clear();
        std::cout << "\033[31mУдален PayrollSystem\033[0m\n";
    }
    PayrollSystem(const PayrollSystem&) = delete;
    PayrollSystem& operator=(const PayrollSystem&) = delete;
    std::vector<Employee*> GetEmployees() {
        return employees_;
    }
    bool LoadFromFile() {
        std::ifstream file(filename_);
        if (!file.is_open()) {
            std::cout << "Файл не найден\n";
            return false;
        }
        file.seekg(0, std::ios::end);
        if (file.tellg() == 0) {
            std::cout << "Файл пуст\n";
            file.close();
            return false;
        }
        file.seekg(0, std::ios::beg);
        for (Employee* emp : employees_) {
            delete emp;
        }
        employees_.clear();
        std::string line;
        bool has_error = false;
        int line_number = 0;
        while (std::getline(file, line)) {
            line_number++;
            if (!line.empty()) {
                Employee* employee = CreateEmployeeFromString(line);
                if (employee) {
                    employees_.push_back(employee);
                }
                else {
                    std::cout << "ОШИБКА: Некорректные данные в строке " << line_number << "\n";
                    has_error = true;
                }
            }
        }
        file.close();
        if (has_error) {
            std::cout << "Файл содержит ошибки, данные загружены частично\n";
        }
        std::cout << "Загружено сотрудников из файла: " << employees_.size() << "\n";
        return !employees_.empty();
    }
    void SaveToFile() const {
        std::ofstream file(filename_);
        if (!file.is_open()) {
            std::cout << "ОШИБКА: Не удалось открыть файл для сохранения\n";
            return;
        }
        for (const auto& emp : employees_) {
            emp->SaveToFile(file);
            file << "\n";
        }
        file.close();
    }
    void AddEmployee(Employee* employee) {
        if (employee) {
            employees_.push_back(employee);
            SaveToFile();
        }
    }
    bool DeleteEmployee(int id) {
        auto it = std::find_if(employees_.begin(), employees_.end(),
            [id](const Employee* emp) {
                return emp->GetId() == id;
            });
        if (it != employees_.end()) {
            delete* it;
            employees_.erase(it);
            SaveToFile();
            return true;
        }
        std::cout << "ОШИБКА: Сотрудник с ID " << id << " не найден\n";
        return false;
    }
    double CalculateTotalPayroll() const {
        double total = 0.0;
        for (const auto& emp : employees_) {
            total += emp->CalculateSalary();
        }
        return total;
    }
    Employee* FindEmployeeById(int id) const {
        auto it = std::find_if(employees_.begin(), employees_.end(),
            [id](const Employee* emp) {
                return emp->GetId() == id;
            });
        if (it != employees_.end()) {
            return *it;
        }
        return nullptr;
    }
    void GeneratePayrollReport() const {
        std::cout << "Всего сотрудников: " << employees_.size() << "\n";
        for (const auto& emp : employees_) {
            std::cout << emp->GetDescription();
        }
        std::cout << "Общий фонд оплаты труда: " << CalculateTotalPayroll() << " руб.\n";
    }
    size_t GetEmployeeCount() const {
        return employees_.size();
    }
    void ShowAllEmployees() const {
        if (employees_.empty()) {
            std::cout << "Нет сотрудников в системе\n";
        }
        else {
            for (const auto& emp : employees_) {
                std::cout << emp->GetDescription();
            }
        }
    }
};

void AddEmployeeSubtask(PayrollSystem* payroll) {
    int emp_type;
    do {
        std::cout << "1. Штатный сотрудник (оклад + премия)\n";
        std::cout << "2. Почасовик\n";
        std::cout << "3. Менеджер (оклад + % от продаж)\n";
        std::cout << "4. Фрилансер (оплата за проект)\n";
        std::cout << "Выберите тип сотрудника:\n";
        std::cin >> emp_type;
        std::cin.ignore(10000, '\n');
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "ОШИБКА: Некорректный ввод\n";
            continue;
        }
        if (emp_type < 1 || emp_type > 4) {
            std::cout << "ОШИБКА: Введите число от 1 до 4\n";
            continue;
        }
        break;
    } while (true);
    std::string name;
    int id;
    std::cout << "Введите имя:\n";
    std::getline(std::cin, name);
    do {
        std::cout << "Введите ID:\n";
        std::cin >> id;
        std::cin.ignore(10000, '\n');
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "ОШИБКА: Некорректный ввод ID\n";
            continue;
        }
        if (payroll->FindEmployeeById(id) != nullptr) {
            std::cout << "ОШИБКА: Сотрудник с таким ID уже существует\n";
            continue;
        }
        break;
    } while (true);
    switch (emp_type) {
    case 1: {
        double salary, bonus;
        do {
            std::cout << "Введите оклад:\n";
            std::cin >> salary;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            if (salary < 0) {
                std::cout << "ОШИБКА: Оклад не может быть отрицательным\n";
                continue;
            }
            break;
        } while (true);
        do {
            std::cout << "Введите премию:\n";
            std::cin >> bonus;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            break;
        } while (true);
        payroll->AddEmployee(new SalariedEmployee(name, id, salary, bonus));
        break;
    }
    case 2: {
        double rate;
        int hours;
        do {
            std::cout << "Введите почасовую ставку:\n";
            std::cin >> rate;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            break;
        } while (true);
        do {
            std::cout << "Введите отработанные часы:\n";
            std::cin >> hours;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            if (hours < 0) {
                std::cout << "ОШИБКА: Часы не могут быть отрицательными\n";
                continue;
            }
            break;
        } while (true);
        payroll->AddEmployee(new HourlyEmployee(name, id, rate, hours));
        break;
    }
    case 3: {
        double salary, sales, commission;
        do {
            std::cout << "Введите оклад:\n";
            std::cin >> salary;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            break;
        } while (true);
        do {
            std::cout << "Введите сумму продаж:\n";
            std::cin >> sales;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            if (sales < 0) {
                std::cout << "ОШИБКА: Сумма продаж не может быть отрицательной\n";
                continue;
            }
            break;
        } while (true);
        do {
            std::cout << "Введите процент комиссии:\n";
            std::cin >> commission;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            if (commission < 0) {
                std::cout << "ОШИБКА: Процент комиссии не может быть отрицательным\n";
                continue;
            }
            break;
        } while (true);
        payroll->AddEmployee(new Manager(name, id, salary, sales, commission));
        break;
    }
    case 4: {
        double payment;
        do {
            std::cout << "Введите оплату за проект:\n";
            std::cin >> payment;
            std::cin.ignore(10000, '\n');
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "ОШИБКА: Некорректный ввод\n";
                continue;
            }
            if (payment < 0) {
                std::cout << "ОШИБКА: Оплата не может быть отрицательной\n";
                continue;
            }
            break;
        } while (true);
        payroll->AddEmployee(new Freelancer(name, id, payment));
        break;
    }
    }
}

void FindEmployeeSubtask(PayrollSystem* payroll) {
    int search_id;
    do {
        std::cout << "Введите ID сотрудника для поиска:\n";
        std::cin >> search_id;
        std::cin.ignore(10000, '\n');
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "ОШИБКА: Некорректный ввод\n";
            continue;
        }
        break;
    } while (true);

    Employee* emp = payroll->FindEmployeeById(search_id);
    if (emp) {
        std::cout << emp->GetDescription();
    }
    else {
        std::cout << "Сотрудник с ID " << search_id << " не найден.\n";
    }
}

void DeleteEmployeeSubtask(PayrollSystem* payroll) {
    int delete_id;
    do {
        for (const auto& emp : payroll->GetEmployees()) {
            std::cout << emp->GetDescription();
        }
        std::cout << "Введите ID сотрудника для удаления:\n";
        std::cin >> delete_id;
        std::cin.ignore(10000, '\n');
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "ОШИБКА: Некорректный ввод\n";
            continue;
        }
        break;
    } while (true);
    payroll->DeleteEmployee(delete_id);
}

void SalaryCalculationSystem() {
    PayrollSystem* payroll = new PayrollSystem();
    payroll->LoadFromFile();
    int issue_number;
    do {
        std::cout << "== СИСТЕМА РАСЧЕТА ЗАРПЛАТЫ ==\n";
        std::cout << "1. Добавить нового сотрудника\n";
        std::cout << "2. Показать всех сотрудников\n";
        std::cout << "3. Рассчитать общий фонд оплаты труда\n";
        std::cout << "4. Найти сотрудника по ID\n";
        std::cout << "5. Удалить сотрудника\n";
        std::cout << "6. Показать платежную ведомость\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие:\n";
        std::cin >> issue_number;
        std::cin.ignore(10000, '\n');
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "ОШИБКА: Некорректный ввод\n";
            continue;
        }
        do {
            if (issue_number == 1) {
                AddEmployeeSubtask(payroll);
            }
            else if (issue_number == 2) {
                payroll->ShowAllEmployees();
            }
            else if (issue_number == 3) {
                std::cout << "Общий фонд оплаты труда: " << payroll->CalculateTotalPayroll() << " руб.\n";
            }
            else if (issue_number == 4) {
                if (payroll->GetEmployeeCount() == 0) {
                    std::cout << "Нет сотрудников в системе\n";
                }
                else {
                    FindEmployeeSubtask(payroll);
                }
            }
            else if (issue_number == 5) {
                if (payroll->GetEmployeeCount() == 0) {
                    std::cout << "Нет сотрудников в системе\n";
                }
                else {
                    DeleteEmployeeSubtask(payroll);
                }
            }
            else if (issue_number == 6) {
                payroll->GeneratePayrollReport();
            }
            else if (issue_number == 0) {
                break;
            }
            else {
                std::cout << "ОШИБКА: Выберите число от 0 до 6\n";
                break;
            }
            if (RepeatTask()) {
                continue;
            }
            else {
                break;
            }
        } while (issue_number != 0);
    } while (issue_number != 0);
    delete payroll;
}