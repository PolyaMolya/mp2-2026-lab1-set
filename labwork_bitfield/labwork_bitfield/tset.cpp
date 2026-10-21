// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.

/*МНОЖЕСТВО*/

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp){}

/*конструктор копирования*/
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower){}

/*конструктор преобразования типа*/
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength()){}

TSet::operator TBitField()
{
    return BitField;
}

/*получить макс. к-во эл-тов*/
int TSet::GetMaxPower(void) const 
{
    return MaxPower;
}

/*элемент множества?*/
int TSet::IsMember(const int Elem) const 
{
    return BitField.GetBit(Elem);
}

/*включение элемента множества*/
void TSet::InsElem(const int Elem) 
{
    BitField.SetBit(Elem);
}

/*исключение элемента множества*/
void TSet::DelElem(const int Elem) 
{
    BitField.ClrBit(Elem);
}

/*ТЕОРЕТИКО-МНОЖЕСТВЕННЫЕ ОПЕРАЦИИ*/

/*присваивание*/
TSet& TSet::operator=(const TSet& s) 
{
    if (this == &s)
        return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

/*сравнение*/
int TSet::operator==(const TSet& s) const 
{
    return BitField == s.BitField;
}

/*сравнение*/
int TSet::operator!=(const TSet& s) const
{
    return BitField != s.BitField;
}

/*объединение*/
TSet TSet::operator+(const TSet& s)
{
    TSet res(BitField | s.BitField);
    return res;
}

/*объединение с элементом*/
TSet TSet::operator+(const int Elem)
{
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

/*разность с элементом*/
TSet TSet::operator-(const int Elem)
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

/*пересечение*/
TSet TSet::operator*(const TSet& s)
{
    TSet res(BitField & s.BitField);
    return res;
}

/*дополнение*/
TSet TSet::operator~(void)
{
    TSet res(~BitField);
    return res;
}

/*ПЕРЕГРУЗКА ВВОДА/ВЫВОДА*/

/*ввод*/
istream& operator>>(istream& istr, TSet& s)
{
    istr >> s.BitField;
    return istr;
}

/*вывод*/
ostream& operator<<(ostream& ostr, const TSet& s)
{
    ostr << s.BitField;
    return ostr;
}