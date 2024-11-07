#include "VoleMachine.h"
using namespace std ;
Memory::Memory(int size) : cells(size, "00") {}

string Memory::getCell(size_t addr) const {
    if (addr >= cells.size()) {
        cerr << "Error: Memory address out of bounds!\n";
        return "00";
    }
    return cells[addr];
}

void Memory::setCell(size_t addr, const string& value) {
    if (addr >= cells.size()) {
        cerr << "Error: Memory address out of bounds!\n";
        return;
    } else {
        cells[addr] = value.size() < 2 ? ("0" + value) : value;
    }
}

int Memory::getSize() const {
    return cells.size();
}

void Memory::display() const {
    cout << "Memory Contents:\n";
    for (size_t i = 0; i < cells.size(); i += 16) {
        cout << hex << uppercase << setw(1) << setfill('0') << (i / 16) << ": ";
        for (size_t j = i; j < i + 16 && j < cells.size(); ++j) {
            cout << cells[j] << " ";
        }
        cout <<endl;
   }
}

RegisterBank::RegisterBank(int size) : registers(size, "00") {}

string RegisterBank::getRegister(int r) const {
    if (r < 0 || r >= registers.size()) {
        cerr << "Error: Register index out of bounds!\n";
        return "00";
    }
    return registers[r];
}

void RegisterBank::setRegister(int r, const string& value) {
    if (r < 0 || r >= registers.size()) {
        cerr << "Error: Register index out of bounds!\n";
        return;
    }
    registers[r] = value;
}

void RegisterBank::display() const {
    cout << "Registers:\n";
    for (size_t i = 0; i < registers.size(); ++i) {
        cout << "R" << i << ": " << setw(2) << setfill('0') << hex << uppercase << registers[i] << "  ";
        cout << endl;
    }
}

// Screen Class Implementation
void Screen::display(const string& text) const {
    cout << "Screen Output: " << text << endl;
}

// Helper function implementation
int hexStringToInt(const string& hexStr) {
    try {
        return stoi(hexStr, nullptr, 16);
    } catch (...) {
        cerr << "Error: Invalid hex format: " << hexStr << endl;
        return -1;
    }
}

// CPU Class Implementation
void CPU::incrementPC() {
    pc += 2;
}

void CPU::fetch(const Memory& memory) {
    ir = memory.getCell(pc) + memory.getCell(pc + 1);
}

bool CPU::execute(Memory& memory, Screen& screen) {
    int instructionInt = hexStringToInt(ir);
    if (instructionInt == -1) return false;

    uint8_t opcode = (instructionInt >> 12) & 0xF;
    uint8_t r = (instructionInt >> 8) & 0xF;
    uint8_t x = (instructionInt >> 4) & 0xF;
    uint8_t y = instructionInt & 0xF;

    switch (opcode) {
        case 1:  // LOAD
            registerBank.setRegister(r, memory.getCell((x << 4) | y));
            break;
        case 2:  // LOAD immediate
        {
            int immediateValue = (x << 4) | y;
            std::stringstream hexStream;
            hexStream << std::uppercase << std::hex << immediateValue;
            registerBank.setRegister(r, hexStream.str());
        }
            break;
        case 3:  // STORE
            memory.setCell((x << 4) | y, registerBank.getRegister(r));
            break;
        case 4:  // MOVE
            registerBank.setRegister(y, registerBank.getRegister(x));
            break;
        case 5:  // ADD
        {
            int sum = stoi(registerBank.getRegister(x), nullptr, 16) + stoi(registerBank.getRegister(y), nullptr, 16);
            sum = sum > 0x0F ? (sum & 0x0F) : sum;
            std::stringstream hexStream;
            hexStream << std::uppercase << std::hex << sum;
            registerBank.setRegister(r, hexStream.str());
        }
            break;
        case 6:  // ADD_FLOAT
        {
            float sum = stof(registerBank.getRegister(x)) + stof(registerBank.getRegister(y));
            std::stringstream floatStream;
            floatStream << sum;
            registerBank.setRegister(r, floatStream.str());
        }
            break;
        case 0xB:  // JUMP if equal
            if (registerBank.getRegister(r) == registerBank.getRegister(0)) {
                pc = (x << 4) | y;
            }
            break;
        case 0xC:  // HALT
            cout << "Program Halted\n";
            incrementPC();
            return false;
        default:
            cerr << "Error: Unknown instruction!\n";
    }
    return true;
}

void CPU::displayStatus() const {
    cout << "PC: " << setw(2) << setfill('0') << hex << uppercase << pc - 2
         << " | IR: " << ir << endl;
    registerBank.display();
}

int CPU::getPC() const {
    return pc;
}

// VoleMachine Class Implementation
void VoleMachine::loadProgram(const vector<string>& program) {
    programSize = program.size() * 2;
    for (size_t i = 0; i < program.size(); ++i) {
        string instruction = program[i];
        if (instruction.size() != 4) {
            cerr << "Error: Invalid instruction size!\n";
            return;
        }
        memory.setCell(i * 2, instruction.substr(0, 2));
        memory.setCell(i * 2 + 1, instruction.substr(2, 2));
    }
    cout << "Program Loaded Successfully\n";
}

void VoleMachine::executeProgram() {
    while (cpu.getPC() < programSize) {
        cpu.fetch(memory);
        if (!cpu.execute(memory, screen)) break;
        cpu.incrementPC();
    }
    displayStatus();
}

void VoleMachine::stepByStepExecution() {
    while (cpu.getPC() < programSize) {
        cpu.fetch(memory);
        if (!cpu.execute(memory, screen)) break;
        cpu.incrementPC();
        displayStatus();
        cout << "Press Enter to continue...";
        cin.ignore();
    }
}

void VoleMachine::displayStatus() {
    cout << "--- Machine Status ---\n";
    cpu.displayStatus();
    memory.display();
}

// UserInterface Class Implementation
void UserInterface::showMenu() {
    while (true) {
        cout << "--- Vole Machine Simulator ---\n";
        cout << "1. Load Program from File\n";
        cout << "2. Execute Program\n";
        cout << "3. Step-by-Step Execution\n";
        cout << "4. Display Machine Status\n";
        cout << "5. Exit\n";

        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: loadProgramFromFile(); break;
            case 2: machine.executeProgram(); break;
            case 3: machine.stepByStepExecution(); break;
            case 4: machine.displayStatus(); break;
            case 5: return;
            default: cout << "Invalid choice.\n";
        }
    }
}

void UserInterface::loadProgramFromFile() {
    cout << "Enter filename: ";
    string filename;
    cin >> filename;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file!\n";
        return;
    }
    vector<string> program;
    string instruction;
    while (file >> instruction) {
        program.push_back(instruction);
    }
    file.close();
    machine.loadProgram(program);
}
