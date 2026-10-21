// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.
// 
// Битовое поле

#include "tbitfield.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

const int TELEM_SIZE = 8 * sizeof(TELEM);

TBitField::TBitField(int len)
{
    if (len <= 0) {
        throw std::invalid_argument("Length must be positive");
    }
    BitLen = len;
    MemLen = (len + TELEM_SIZE - 1) / TELEM_SIZE;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}


TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

// Деструктор
TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / TELEM_SIZE;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % TELEM_SIZE);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);

    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

    for (int i = 0; i < minMem; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }
    for (int i = minMem; i < MemLen; i++) {
        result.pMem[i] = pMem[i];
    }
    for (int i = minMem; i < bf.MemLen; i++) {
        result.pMem[i] = bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);
    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

    for (int i = 0; i < minMem; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }

    int lastBit = BitLen % TELEM_SIZE;
    if (lastBit != 0) {
        TELEM mask = (1 << lastBit) - 1;
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    std::string s;
    istr >> s;
    if (s.length() != (size_t)bf.GetLength()) {
        throw std::invalid_argument("Input string length does not match bitfield length");
    }
    for (int i = 0; i < bf.GetLength(); i++) {
        if (s[i] == '1') {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
