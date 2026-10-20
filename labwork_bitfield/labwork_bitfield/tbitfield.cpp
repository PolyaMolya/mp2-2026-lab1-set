// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//   Update без сборки для Microsoft Visual Studio 2026 Бусько П.В.
// 
// Битовое поле

#include "tbitfield.h"
#include <cstring>
#include <stdexcept>
#include <iostream>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) // конструктор инициализации
{
  if ( len < 0) // Проверка длины битового поля
  {
    throw std::invalid_argument("Недопустимая длина битового поля! (len < 0)");
  }

  BitLen = len;
  MemLen = len / (8 * sizeof(TELEM)) + (len % (8 * sizeof(TELEM)) != 0);
  pMem = new TELEM[MemLen];
  memset(pMem, 0, sizeof(TELEM) * MemLen);
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  memcpy(pMem, bf.pMem, bf.MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if (n < 0 || n >= BitLen) // если индекс выходит за границы поля
  {
    throw std::invalid_argument("Недопустимое значение индекса n (<0 или >=BitLen)!");
  }
  
  return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n < 0 || n >= BitLen) // если индекс выходит за границы поля
  {
    throw std::invalid_argument("Недопустимое значение индекса n (<0 либо >=BtiLen)!");
  }

  TELEM bitmask = (TELEM)1 << (n % (8 * sizeof(TELEM)));
  return bitmask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  TELEM bitmask = GetMemMask(n);
  int index = GetMemIndex(n); 
  pMem[index] = pMem[index] | bitmask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
  TELEM invert_bitmask = ~(GetMemMask(n));
  int index = GetMemIndex(n);
  pMem[index] = pMem[index] & invert_bitmask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
 TELEM checkmask = GetMemMask(n);
 int index = GetMemIndex(n);
 int res = (int)((pMem[index] & checkmask) != 0);
 return res;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
  TELEM* tmp = new TELEM[bf.MemLen]; // новый временный массив
  for (int i = 0; i < bf.MemLen; i++)
  {
    tmp[i] = bf.pMem[i];
  }
  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = tmp;
  return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
  bool differences = false;
  differences = ((MemLen!=bf.MemLen) || (BitLen!=bf.BitLen));
  if (!differences)
  {
    for (int i = 0; i < MemLen; i++)
    {
      if (pMem[i] == bf.pMem[i]) continue;
      differences = true;
      break;
    }
  }
  return (int)(!differences);
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
  int maxlen = (BitLen >= bf.BitLen) ? BitLen : bf.BitLen;
  TBitField newfield(maxlen);
  const TBitField& shorter = (MemLen <= bf.MemLen) ? *this : bf;
  const TBitField& longer = (MemLen >= bf.MemLen) ? *this : bf;
  for (int i = 0; i < shorter.MemLen; i++)
  {
    newfield.pMem[i] = pMem[i] | bf.pMem[i];
  }
  for (int i = shorter.MemLen; i < longer.MemLen; i++)
  {
    newfield.pMem[i] = longer.pMem[i];
  }
  return newfield;
}   

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
  int maxlen = (BitLen >= bf.BitLen) ? BitLen : bf.BitLen;
  TBitField newfield(maxlen);
  const TBitField& shorter = (MemLen <= bf.MemLen) ? MemLen : bf.MemLen;
  const TBitField& longer = (MemLen >= bf.MemLen) ? MemLen : bf.MemLen;
  for (int i = 0; i < shorter.MemLen; i++)
  {
    newfield.pMem[i] = pMem[i] & bf.pMem[i];
  }
  // хвост уже нулевой
  return newfield;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField newfield(BitLen);
  for (int i = 0; i < MemLen; i++)
  {
    newfield.pMem[i] = ~pMem[i];
  }
  unsigned int num_sigbit = BitLen % (sizeof(TELEM)*8); // значимые биты в последнем элементе
  if (num_sigbit != 0)
  {
    TELEM bitmask = ((TELEM)1 << num_sigbit) - (TELEM)1;
    newfield.pMem[MemLen - 1] = newfield.pMem[MemLen - 1] & bitmask; // зануление незначимых битов
  }
  return newfield;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
  char c;
  int counter = 0;
  TBitField tmp = bf;
  for (int i = 0; i < bf.BitLen; i++)
  {
    istr.get(c);
    if (!istr) return istr; 
    if (c == '1') { tmp.SetBit(i); counter++; }
    else if (c == '0') { tmp.ClrBit(i); counter++; }
    else break;
  }
  if (counter == tmp.BitLen) { bf = tmp; return istr; }
  else { istr.setstate(std::ios::failbit); return istr; }
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{ 
  for (int i = 0; i < bf.BitLen; i++)
  {
    ostr << ((bf.GetBit(i) == 1) ? '1' : '0');
  }
  return ostr;
}
