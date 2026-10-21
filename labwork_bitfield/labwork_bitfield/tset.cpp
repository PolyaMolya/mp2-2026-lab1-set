// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    if (mp <= 0) {
        throw std::invalid_argument("MaxPower must be positive");
    }
}

// Конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}


// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem >= 0 && Elem < MaxPower) {
        return BitField.GetBit(Elem);
    }
    return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element index out of range");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element index out of range");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this == &s) {
        return *this;
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower) return 0;
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    // Результирующая мощность - максимальная из двух
    int maxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;

    // Создаем пустое результирующее множество
    TSet result(maxPower);

    // Побитовое ИЛИ
    TBitField tempBF = BitField | s.BitField;
    result = TSet(tempBF);

    // Если мощности различались, нужно убедиться, что MaxPower у result правильный
    // (Хотя конструктор TSet(TBitField) берет длину из bf, что может быть меньше maxPower)
    // Для безопасности:
    if (result.MaxPower < maxPower) {
        TSet resized(maxPower);
        for (int i = 0; i < result.MaxPower; i++) {
            if (result.IsMember(i)) resized.InsElem(i);
        }
        result = resized;
    }

    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element index out of range");
    }
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element index out of range");
    }
    TSet result(*this); // Копируем текущее множество
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int maxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;

    TSet result(maxPower);

    for (int i = 0; i < maxPower; i++) {
        if (IsMember(i) && s.IsMember(i)) {
            result.InsElem(i);
        }
    }
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    // Побитовое НЕ
    TBitField tempBF = ~BitField;
    TSet result(tempBF);
    // Восстанавливаем MaxPower, так как конструктор TSet(TBitField) взял длину bf
    result.MaxPower = MaxPower;
    return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    // Читаем строку из 0 и 1
    std::string str;
    istr >> str;
    if (str.length() != (size_t)s.GetMaxPower()) {
        throw std::invalid_argument("Input string length does not match MaxPower");
    }
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (str[i] == '1') {
            s.InsElem(i);
        }
        else {
            s.DelElem(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    for (int i = 0; i < s.GetMaxPower(); i++) {
        ostr << s.IsMember(i);
    }
    return ostr;
}