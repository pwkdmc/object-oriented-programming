# Software Systems Simulator Suite
A C++ console application integrating three independent business system simulations with polymorphic class hierarchies, custom exception handling, and persistent file storage.
## 🌟 Overview
This project demonstrates practical implementation of real-world business logic through three distinct subsystems. Each features deep inheritance trees, virtual function dispatch, manual memory management with RAII principles, and domain-specific exception handling. The systems operate independently with their own data models and business rules.
## 🧩 Subsystems
### 🎫 Ticket Booking System
A multi-category ticket reservation platform supporting three booking types with dynamic pricing models:
- **MovieTicket** — 10% service fee, fully refundable
- **FlightTicket** — Class multipliers (economy 0%, business +50%, first +100%), baggage overage fees (50₽/kg over 20kg), 15% fuel surcharge. First class non-refundable
- **TrainTicket** — Seat type multipliers (reserved_seat 0%, compartment +30%, lux +80%), 5% service fee. Lux class non-refundable
Features ticket creation, listing, cancellation with refund validation, and total revenue calculation.
### 💰 Salary Calculation System
A payroll management system with four employee types and distinct compensation formulas:
- **SalariedEmployee** — Fixed salary + bonus
- **HourlyEmployee** — Hourly rate × hours worked
- **Manager** — Base salary + commission percentage on sales
- **Freelancer** — Flat project payment
Supports adding, removing, searching by ID, generating payroll reports with total labor cost, and persistent storage via CSV-like serialization to `employees.txt`.
### 🏦 ATM Simulator
A banking terminal simulation with three card types and robust security features:
- **DebitCard** — Balance cannot go below zero
- **CreditCard** — Withdrawals allowed up to credit limit
- **BonusCard** — Earns 1% on withdrawals, 2% on deposits as cashback points
Implements card database management, PIN verification with attempt tracking (3 tries before blocking), admin card unblocking with PIN reset, and masked card number display for security.
## 🎯 Key Features Across All Subsystems
- **Robust input validation** with retry loops for all user inputs
- **Custom exception classes** for domain-specific error handling
- **Polymorphic class hierarchies** with virtual methods
- **Automatic resource cleanup** through destructors (no memory leaks)
- **Persistent storage** (`employees.txt` for payroll system)
- **Task-repeat workflow** — each operation can be re-executed without returning to main menu
- **Console UI** with colored output markers (`\033[31m`) and structured menus
- **Operator overloading** for assignment operations
- **Static members** for ID generation and instance counting
## 📄 License
The project is intended for educational purposes. It can be used and modified freely.