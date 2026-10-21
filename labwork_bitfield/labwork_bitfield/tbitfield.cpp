// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.
// 

/*БИТОВОЕ ПОЛЕ*/

#include "tbitfield.h"

static const int BIT = sizeof(TELEM) * 8; /* # бит в одном элементе pMem*/

TBitField::TBitField(int len){
    if (len < 0)
        throw out_of_range("negative length");
    BitLen = len;
    MemLen = (len + BIT - 1) / BIT; /*округление вверх*/
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

/*конструктор копирования*/
TBitField::TBitField(const TBitField& bf) { 
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField() {
    delete[] pMem;
}

/*индекс Мем для бита n */
int TBitField::GetMemIndex(const int n) const { 
    if (n < 0 || n >= BitLen)
        throw invalid_argument("incorrect number");
    return  n / BIT;
}

/*битовая маска для бита n*/
TELEM TBitField::GetMemMask(const int n) const { 
    if (n < 0 || n >= BitLen)
        throw invalid_argument("incorrect number");
    return 1 << (n % BIT);
}

/*ДОСТУП К БИТАМ БИТОВОГО ПОЛЯ*/

/*получить длину (к-во битов)*/
int TBitField::GetLength(void) const { 
    return BitLen;
}

/*установить бит*/
void TBitField::SetBit(const int n) { 
    if (n < 0 || n >= BitLen)
        throw invalid_argument("incorrect number");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

/*очистить бит*/
void TBitField::ClrBit(const int n) { 
    if (n < 0 || n >= BitLen)
        throw invalid_argument("incorrect number");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

/*получить значение бита*/
int TBitField::GetBit(const int n) const { 
    if (n < 0 || n >= BitLen)
        throw invalid_argument("incorrect number");
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0)
        return 1;
    else
        return 0;
}

/*БИТОВЫЕ ОПЕРАЦИИ*/

/*присваивание*/
TBitField& TBitField::operator=(const TBitField& bf) {
    if (this == &bf)
        return *this;
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

/*сравнение*/
int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

/*сравнение*/
int TBitField::operator!=(const TBitField& bf) const {
    return !(*this == bf);
}

/*операция "или"*/
TBitField TBitField::operator|(const TBitField& bf) {
    int mlen = BitLen;
    if (bf.BitLen > mlen)
        mlen = bf.BitLen;
    TBitField res(mlen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        res.pMem[i] |= bf.pMem[i];
    return res;
}

/*операция "и"*/
TBitField TBitField::operator&(const TBitField& bf) {
    int mlen = BitLen;
    if (bf.BitLen > mlen)
        mlen = bf.BitLen;
    TBitField res(mlen); 
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int i = 0; i < res.MemLen; i++)
    {
        if (i < bf.MemLen)
            res.pMem[i] = res.pMem[i] & bf.pMem[i];
        else
            res.pMem[i] = 0; 
    }
    return res;
}

/*отрицание*/
TBitField TBitField::operator~(void) {
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 0)
            res.SetBit(i);
    }
    return res;
}

/*ВВОД/ВЫВОД*/

/*ввод*/
istream& operator>>(istream& istr, TBitField& bf) {
    char с;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> с;
        if (с == '0')
            bf.ClrBit(i);
        else if (с == '1')
            bf.SetBit(i);
        else
            break;
    }
    return istr;
}

/*вывод*/
ostream& operator<<(ostream& ostr, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
