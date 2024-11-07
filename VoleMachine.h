#ifndef VOLEMACHINE_H
#define VOLEMACHINE_H

#include <bits/stdc++.h>
using namespace std;

// Memory Class
class Memory {
private:
    vector<string> cells;
public:
    Memory(int size = 256);
    string getCell(size_t addr) const;
    void setCell(size_t addr, const string& value);
    int getSize() const;
    void display() const;
};

// RegisterBank Class
class RegisterBank {
private:
    vector<string> registers;
public:
    RegisterBank(int size = 16);
    string getRegister(int r) const;
    void setRegister(int r, const string& value);
    void display() const;
};

// Screen Class
class Screen {
public:
    void display(const string& text) const;
};

// CPU Class
class CPU {
private:
    int pc = 0;
    string ir;
    RegisterBank registerBank;

public:
    void incrementPC();
    void fetch(const Memory& memory);
    bool execute(Memory& memory, Screen& screen);
    void displayStatus() const;
    int getPC() const;
};

// VoleMachine Class
class VoleMachine {
private:
    CPU cpu;
    Memory memory;
    Screen screen;
    size_t programSize = 0;

public:
    void loadProgram(const vector<string>& program);
    void executeProgram();
    void stepByStepExecution();
    void displayStatus();
};

// UserInterface Class
class UserInterface {
private:
    VoleMachine machine;

public:
    void showMenu();
    void loadProgramFromFile();
};

// Helper function to convert hex string to int
int hexStringToInt(const string& hexStr);

#endif // VOLEMACHINE_H
