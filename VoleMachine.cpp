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
