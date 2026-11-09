// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.
// 
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
// Fake variables used as placeholders in tests
static const size_t BIE = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
    if (len <= 0)
       throw invalid_argument("Error, length is positive");

    BitLen = len;
    MemLen = (len + BIE - 1) / BIE;
    pMem = new TELEM[MemLen];
    if (pMem == nullptr)
        throw std::runtime_error("Out of memory");
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;


}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    pMem = new TELEM[MemLen];
    if (pMem == nullptr)
        throw std::runtime_error("Out of memory");
    for (int i = 0; i < MemLen; i++)
    {
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
        throw invalid_argument("Wrong argument: n");
    return n/BIE;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw invalid_argument("Wrong argument: n");
    int pos = n % BIE;
    return 1 << pos;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw invalid_argument("Wrong argument: n");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw invalid_argument("Wrong argument: n");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw invalid_argument("Wrong argument: n");

    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
        return 1;
    else
        return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf)
        return *this;
    else
    {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];

        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }

        return *this;
    }
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int maxLen = BitLen;
    if (bf.BitLen > maxLen) 
    {
        maxLen = bf.BitLen;
    }

    TBitField result(maxLen);

    for (int i = 0; i < maxLen; i++)
    {
        int leftBit = 0;
        if (i < BitLen) 
        {
            leftBit = GetBit(i);
        }

        int rightBit = 0;
        if (i < bf.BitLen) 
        {
            rightBit = bf.GetBit(i);
        }

        if (leftBit == 1 || rightBit == 1) 
        {
            result.SetBit(i);
        }
    }

    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxLen = BitLen;
    if (bf.BitLen > maxLen)
    {
        maxLen = bf.BitLen;
    }

    TBitField result(maxLen);

    for (int i = 0; i < maxLen; i++)
    {
        int leftBit = 0;
        if (i < BitLen)
        {
            leftBit = GetBit(i);
        }

        int rightBit = 0;
        if (i < bf.BitLen)
        {
            rightBit = bf.GetBit(i);
        }

        if (leftBit == 1 && rightBit == 1)
        {
            result.SetBit(i);
        }
    }

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 0)
        {
            result.SetBit(i);
        }
    }
    return result;
}


// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    string s;
    istr >> s;
    for (int i = 0; i < s.length() && i < bf.GetLength(); i++)
    {
        if (s[i] == '1')
            bf.SetBit(i);
        else if (s[i] == '0')
            bf.ClrBit(i);
        else
            throw std::invalid_argument("Must be only 0 or 1");
    }

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
