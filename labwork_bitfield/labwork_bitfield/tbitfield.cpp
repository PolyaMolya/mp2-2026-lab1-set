// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.
// 
// Битовое поле

#include "tbitfield.h"
TBitField::TBitField(int len)
{
    if (len < 0)
        throw "-lenght";
    BitLen = len;
    MemLen = (len + 31) / 32;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen;i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0;i < MemLen;i++)
        pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n/32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
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
        throw "out of range";
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n); //1|x=1, 0|x=x
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw "out of range";
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n); // x&1=x, x&0=0
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw "out of range";
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
        return 1;
    else
       return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen;i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (*this == bf)
        return 0;
    return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField res(len);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        res.pMem[i] = res.pMem[i] | bf.pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int len = BitLen;
    if (bf.BitLen > len)
        len = bf.BitLen;
    TBitField res(len);              
    int m = MemLen;
    if (bf.MemLen < m)
        m = bf.MemLen;
    for (int i = 0; i < m; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 0)
            res.SetBit(i);
    }
    return res;;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char c;
    for (int i = 0;i < bf.BitLen;i++) {
        istr >> c;
        if (c = '1')
            bf.SetBit(i);
        else if (c == '0')
            bf.ClrBit(i);
        else
            throw "incorrect symbol";
    }
    return istr;
 }

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0;i < bf.BitLen;i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}