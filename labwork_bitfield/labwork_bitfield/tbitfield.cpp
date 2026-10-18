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

TBitField::TBitField(int len)
{
    if (len == -1) {
        len = 1;
    }
    if (len < 0)
        throw out_of_range("Otritsatelnoe chislo");
    BitLen = len;
    MemLen = (len + 31) >> 5;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0;i < MemLen; i++)
            pMem[i] = 0;
    }
    else { pMem = nullptr; }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    if (bf.pMem == nullptr) {
        MemLen = 0;
        BitLen = 0;
        pMem = nullptr;
    }
    else {
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        pMem = new TELEM[MemLen];
        for (int i = 0;i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("incorrect n");
    return n>>5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("incorrect n");
    return 1<<(n%32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("incorrect n");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("incorrect n");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("incorrect n");
    if ((pMem[GetMemIndex(n)] & GetMemMask(n))!=0)
        return 1;
    else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        if (bf.pMem == nullptr) {
            MemLen = 0;
            BitLen = 0;
            pMem = nullptr;
        }
        else {
            delete[] pMem;
            MemLen = bf.MemLen;
            BitLen = bf.BitLen;
            pMem = new TELEM[MemLen];
            for (int i = 0;i < MemLen; i++)
                pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if(BitLen!=bf.BitLen)
        return 0;
    else{
        for (int i = 0;i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return 0;
        return 1;
    }
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxBitLen = max(BitLen, bf.BitLen);
    TBitField temp(maxBitLen);
    int minMemLen = min(MemLen, bf.MemLen);
    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] | bf.pMem[i];
    }
    if (MemLen > bf.MemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            temp.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            temp.pMem[i] = bf.pMem[i];
        }
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxBitLen = max(BitLen, bf.BitLen);
    TBitField temp(maxBitLen);
    int minMemLen = min(MemLen, bf.MemLen);
    for (int i = 0;i < minMemLen;i++)
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < MemLen; i++)
        temp.pMem[i] = ~pMem[i];
    int lastBits = BitLen % 32;
    if (lastBits > 0) {
        TELEM mask = (TELEM(1) << lastBits) - 1;
        temp.pMem[MemLen - 1] &= mask; 
    }
    return temp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char ch;
    for (int i = 0; i < bf.GetLength(); i++) {
        istr >> ch;
        if (ch == '1') {
            bf.SetBit(i);
        }
        else if (ch == '0') {
            bf.ClrBit(i);
        }
        else {
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        if (bf.GetBit(i)!=0) {
            ostr << '1';
        }
        else {
            ostr << '0';
        }
    }
    return ostr;
}
