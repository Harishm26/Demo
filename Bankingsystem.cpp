#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolderName;
    double balance;
    double minBalance;
    char accType;

public:
    Account(const std::string& accNum,const std::string& name, double initialBalance, char type, double minBal)
        : accountNumber(accNum),accountHolderName(name), balance(initialBalance), accType(type),minBalance(minBal) {}

    virtual ~Account() {}
    virtual void interestRate(){};
    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
      if (amount > balance) {
        std::cout << "Insufficient funds!" << std::endl;
        return false;
      }
        balance -= amount;
      if (balance < minBalance){
        std::cout << "Your current balance is less than the minimum balance!.." << std::endl;
      }
      return true;
    }
    
    Account operator+(double amount) {
      this->deposit(amount);
      std::cout << "Rs " << std::to_string(amount) << "deposited into the Account: "<< this->accountNumber<< std::endl;
      return *this;
    }

    Account operator-(double amount) {
      this->withdraw(amount);
      std::cout << "Rs " << std::to_string(amount) << "withdrew from the Account: "<< this->accountNumber<< std::endl;
      return *this;
    }

    double getBalance() const {
        return balance;
    }

    std::string getAccountNumber() const {
        return accountNumber;
    }
    
    void displayAccountDetails(){
      std::cout << "-------"<<this->accountNumber<<"-------" << std::endl;
      std::cout << "NAME       : "<< this->accountHolderName << std::endl;
      std::cout << "ACC No     : " <<this->accountNumber<< std::endl;
      std::cout << "ACC TYPE   : " <<this->accType<< std::endl;
      std::cout << "ACC BALANCE: " <<this->balance<< std::endl;
      std::cout << "---------------------" << std::endl;
      if(this->balance <this->minBalance);
        std::cout << "Your Acount Balance is less than the minimum Balance!" << std::endl;
    }
    
    void saveAccountDetails(){
      std::ofstream file(this->accountNumber+".txt");
      file << "-------"<<this->accountNumber<<"-------" << std::endl;
      file << "NAME       : "<< this->accountHolderName << std::endl;
      file << "ACC No     : " <<this->accountNumber<< std::endl;
      file << "ACC TYPE   : " <<this->accType<< std::endl;
      file << "ACC BALANCE: " <<this->balance<< std::endl;
      file << "---------------------" << std::endl;
      file.close();
    }
    
    static void showOptions() {
      std::cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Check Balance\n5. Display Account Details\n6. Save Account Details\n7. Delete Account\n8. Exit\n";
    }

    static std::string generateAccountNumber(){
      static int acc_count = 0;
      return "SBIACC" + std::to_string(++(acc_count));
    }
};

class CheckingAccount : public Account {
public:
    CheckingAccount(const std::string& accNum,const std::string& name, double initialBalance)
        : Account(accNum, name, initialBalance, 'C',500) {}
    
    void interestRate(){
      std::cout << "Interest Rate per Annum : 0.05" << std::endl;
    }
    
};

class SavingsAccount : public Account {
public:
    SavingsAccount(const std::string& accNum,const std::string& name, double initialBalance)
        : Account(accNum, name, initialBalance,'S',1000){}
    
    void interestRate(){
      std::cout << "Interest Rate per Annum : 0.1" << std::endl;
    }
    
};

std::unordered_map<std::string, std::unique_ptr<Account>> accounts;
bool isValidAccountNum(std::string accNum){
  return accounts.contains(accNum);
}
void createAccount(double initialBalance,const std::string& name, bool isChecking) {
    std::string accNum = Account::generateAccountNumber();
    if (isChecking) {
        accounts[accNum] = std::make_unique<CheckingAccount>(accNum,name, initialBalance);
        std::cout << "Dear "<<name <<", your Checking Account is created." << std::endl;
    } else {
        accounts[accNum] = std::make_unique<SavingsAccount>(accNum,name, initialBalance);
        std::cout << "Dear "<<name <<", your Savings Account is created." << std::endl;
    }
    accounts[accNum]->displayAccountDetails();
}

void deleteAccount(const std::string& accNum) {
    std::cout << "Your Account: " << accNum <<" is deleted."<< std::endl;
    double balance = accounts[accNum]->getBalance();
    if(balance>0)
      std::cout << "Please collect your Account balance amount of Rs "<< balance << std::endl;
    accounts.erase(accNum);// unique_ptr will automatically clean up
}

int main(){
  int choice,isChecking;
  double amount;
  std::string name,accNum;
  while (true) {
        Account::showOptions();
        std::cout << "Please select an option: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << "Enter your Full Name in Block Letters: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter initial Deposit: ";
                std::cin >> amount;
                std::cout << "Is it a checking account? (1 for Yes, 0 for No): ";
                std::cin >> isChecking;
                createAccount(amount,name,isChecking);
                break;
            case 2:
                std::cout << "Enter account number: ";
                std::cin >> accNum;
                if(!isValidAccountNum(accNum)){
                  std::cout << "Invalid Account number!\n" << std::endl;
                }
                else{
                  std::cout << "Enter amount to deposit: ";
                  std::cin >> amount;
                  *accounts[accNum] = *accounts[accNum] + amount; 
                }
                break;
            case 3:
                std::cout << "Enter account number: ";
                std::cin >> accNum;
                if(!isValidAccountNum(accNum)){
                  std::cout << "Invalid Account number!\n" << std::endl;
                }
                else{
                  std::cout << "Enter amount to withdraw: ";
                  std::cin >> amount;
                  *accounts[accNum] = *accounts[accNum] - amount;
                }
                break;
            case 4:
                std::cout << "Enter account number: ";
                std::cin >> accNum;
                if(!isValidAccountNum(accNum)){
                  std::cout << "Invalid Account number!\n" << std::endl;
                }
                else{
                  std::cout << "Balance: " << accounts[accNum]->getBalance() << std::endl;
                }
                break;
            case 5:
                std::cout << "Enter account number: ";
                std::cin >> accNum;
                if(!isValidAccountNum(accNum)){
                  std::cout << "Invalid Account number!\n" << std::endl;
                }
                else{
                  accounts[accNum]->displayAccountDetails();
                }
                break;
            case 6:
                std::cout << "Enter account number: ";
                std::cin >> accNum;
                if(!isValidAccountNum(accNum)){
                  std::cout << "Invalid Account number!\n" << std::endl;
                }
                else{
                  accounts[accNum]->saveAccountDetails();
                }
                break;
            case 7:
                std::cout << "Enter account number: ";
                std::cin >> accNum;
                if(!isValidAccountNum(accNum)){
                  std::cout << "Invalid Account number!\n" << std::endl;
                }
                else{
                  deleteAccount(accNum);
                }
                break;
            case 8:
                accounts.clear();
                return 0;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
  return 0;
}

