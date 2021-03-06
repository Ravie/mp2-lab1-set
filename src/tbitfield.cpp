// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define TYPE_SIZE sizeof(TELEM)*8

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw "UNEXPECTED_SIZE";
	BitLen = len;
	MemLen = (len + TYPE_SIZE-1) >> int(log(TYPE_SIZE)/log(2)); //в эл-те рћем 16 бит (TELEM==int) 
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n >> int(log(TYPE_SIZE) / log(2)); // в эл-те рћем 16 бит 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (TYPE_SIZE-1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n <= -1) || (n >= BitLen))
		throw "UNEXPECTED_INDEX";
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n <= -1) || (n >= BitLen))
		throw "UNEXPECTED_INDEX";
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n <= -1) || (n >= BitLen))
		throw "UNEXPECTED_INDEX";
	return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		if (pMem != NULL) delete pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
	return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
	bool res = true;
	if (BitLen != bf.BitLen) 
		res = false;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) 
			{ 
				res = false; 
				break;
			}
	return res;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
	bool res = false;
	if (BitLen != bf.BitLen) 
		res = true;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) 
			{ 
				res = true; 
				break;
			}
	return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i, len = BitLen;
	if (bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++)	temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++)	temp.pMem[i] |= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i, len = BitLen;
	if (bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++)	temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++)	temp.pMem[i] &= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	int len = BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen - 1; i++)
	{
		temp.pMem[i] = ~pMem[i];
		temp.pMem[i] &= ((1 << TYPE_SIZE) - 1);
	}
	temp.pMem[MemLen - 1] = ~pMem[MemLen - 1];
	temp.pMem[MemLen - 1] &= ((1 << len) - 1);
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0; char ch;
	do { istr >> ch; } while (ch != ' ');
	while (1)
	{
		istr >> ch;
		if (ch == '0') bf.ClrBit(i++);
		else if (ch == '1') bf.SetBit(i++); else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for (int i = 0; i < len; i++)
		if (bf.GetBit(i)) ostr << '1'; else ostr << '0';
	return ostr;
}
