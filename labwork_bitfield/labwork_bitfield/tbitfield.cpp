// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.
// 
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
const int TELEM_BITS = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
    if (len < 0) {
        cout << len;throw "whong lenght";
    }
    BitLen = len;
    MemLen = (BitLen + TELEM_BITS - 1) / TELEM_BITS;

    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
    else {
        pMem = nullptr;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    else {
        pMem = nullptr;
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / TELEM_BITS;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1u << (n % TELEM_BITS);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= 0 && n < this->BitLen) {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
    else {
        throw "Incorrect index";
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= 0 && n < BitLen) {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
    else {
        throw "Incorrect index";
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= 0 && n < BitLen) {
        return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
    }
    else {
        throw "Incorrect index";
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) return *this; // Проверка на самоприсваивание

    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        if (MemLen > 0) {
            pMem = new TELEM[MemLen];
        }
        else {
            pMem = nullptr;
        }
    }

    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField result(std::max(BitLen, bf.BitLen));

    for (int i = 0; i < result.MemLen; i++) {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a | b;
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField result(max(BitLen, bf.BitLen));

    for (int i = 0; i < result.MemLen; i++) {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a & b;
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < result.MemLen; i++) {
        
        result.pMem[i] = ~pMem[i];
    }
    int extraBits = result.MemLen * TELEM_BITS - result.BitLen;//зануляем неиспользуемые биты

    TELEM mask = (1u << (TELEM_BITS - extraBits)) - 1;
    result.pMem[result.MemLen - 1] &= mask;
    return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    std::string s;
    istr >> s;
    int n = min<int>(bf.BitLen, s.length());
    for (int i = 0; i < n; i++) {
        if (s[n - i - 1] == '1'){
            bf.SetBit(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = bf.BitLen - 1; i >= 0; i--) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
