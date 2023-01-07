#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

template <class T>
class Vector
{
protected:
	T* arr;
	uint16_t _size;
	uint16_t _capacity;

public:
	Vector() : _size(0), _capacity(5)
	{
#ifdef DEBUG
		Serial.println(F("--- Vector() ---"));
#endif // DEBUG
		arr = new T[5];
#ifdef DEBUG
		Serial.println(F("--- Vector() end ---"));
#endif // DEBUG
	}

	Vector(uint16_t size) : _size(size), _capacity(size)
	{
#ifdef DEBUG
		Serial.println("--- Vector(size) ---");
#endif // DEBUG
		arr = new T[size];
#ifdef DEBUG
		Serial.println("--- Vector(size) end ---");
#endif // DEBUG
	}

	Vector(const Vector<T>& _V)
	{
#ifdef DEBUG
		Serial.println(F("--- Vector(& _V) ---"));
#endif // DEBUG
		this->_size = _V._size;
		this->_capacity = _V._capacity;
		arr = new T[_size];
		for (uint16_t i = 0; i < _size; ++i)
			this->arr[i] = _V.arr[i];
#ifdef DEBUG
		Serial.println(F("--- Vector(& _V) end ---"));
#endif // DEBUG
	}

	~Vector()
	{
#ifdef DEBUG
		Serial.println(F("--- ~ Vector ---"));
#endif // DEBUG
		delete[] arr;
#ifdef DEBUG
		Serial.println(F("--- ~ Vector end ---"));
#endif // DEBUG
	}

	/*Vector<T>& operator=(const Vector& _Right)
	{
		if (this == &_Right)
			return *this;

		delete[] arr;
		this->_size = _Right._size;
		this->_capacity = _Right._capacity;
		arr = new T[this->_size];
		for (uint16_t i = 0; i < this->_size; ++i)
			this->arr[i] = _Right.arr[i];
		return *this;
	}*/

	T& operator[](uint16_t const& i)
	{
		return arr[i];
	}


	//bool operator==(const Vector& _Right)
	//{
	//	if (this->_size != _Right._size) return false;
	//	for (uint16_t i = 0; i < this->_size; ++i)
	//		if (this->arr[i] != _Right.arr[i]) return false;
	//	return true;
	//}

	const uint16_t size()
	{
		return this->_size;
	}

	// TODO: uint16_t vector<T>::max_size() const;

	const uint16_t capacity()
	{
		return _capacity;
	}
	const bool empty()
	{
		return _size == 0;
	}
	
	void resize(const uint16_t _NewSize)
	{
		if (_NewSize = this->_size) return;
		if (_NewSize > this->_size && _NewSize < this->_capacity - 1)
			this->_size = _NewSize;
		else if (_NewSize > this->_capacity - 1)
		{
			T* newArray = new T[this->_NewSize + 10];
			for (uint16_t i = 0; i < this->_size; i++)
				newArray[i] = this->arr[i];
			delete[] this->arr;
			this->arr = newArray;
			this->_capacity = _NewSize + 10;
			this->_size = _NewSize;
		}
		else if (_NewSize < this->_size)
		{
			T* newArray = new T[this->_NewSize + 10];
			for (uint16_t i = 0; i < _NewSize; i++)
				newArray[i] = this->arr[i];
			delete[] this->arr;
			this->arr = newArray;
			this->_capacity = _NewSize + 10;
			this->_size = _NewSize;
		}
	}
	void resize(const uint16_t _NewSize, const T& Val)
	{
		if (_NewSize = this->_size) return;
		if (_NewSize > this->_size && _NewSize < this->_capacity - 1)
			this->_size = _NewSize;
		else if (_NewSize > this->_capacity - 1)
		{
			T* newArray = new T[this->_NewSize + 10];
			for (uint16_t i = 0; i < this->_size; i++)
				newArray[i] = this->arr[i];
			for (uint16_t i = this->_size; i < _NewSize; i++)
				newArray[i] = Val;
			delete[] this->arr;
			this->arr = newArray;
			this->_capacity = _NewSize + 10;
			this->_size = _NewSize;
		}
		else if (_NewSize < this->_size)
		{
			T* newArray = new T[this->_NewSize + 10];
			for (uint16_t i = 0; i < _NewSize; i++)
				newArray[i] = this->heroesKeeper[i];
			delete[] this->arr;
			this->arr = newArray;
			this->_capacity = _NewSize + 10;
			this->_size = _NewSize;
		}
	}
	void reserve(uint16_t Newcapacity)
	{
		if (Newcapacity < 0) return; //throw - 1;
		if (Newcapacity > this->_capacity)
		{
			T* newArray = new T[Newcapacity];
			if (this->_size >= Newcapacity)
				this->_size = Newcapacity;
			for (uint16_t i = 0; i < this->_size; i++)
				newArray[i] = this->arr[i];
			delete[] this->arr;
			this->arr = newArray;
			this->_capacity = Newcapacity;
		}
	}

	void push_back(T& val)
	{
		if (this->_size >= this->_capacity - 1)
		{
			this->arr[this->_size] = val;
			T* newArray = new T[this->_capacity + 10];
			for (uint16_t i = 0; i < this->_capacity; i++)
				newArray[i] = this->arr[i];
			delete[] this->arr;
			this->arr = newArray;
			this->_capacity += 10;
			this->_size++;
		}
		else
			this->arr[this->_size++] = val;
	}

	void push_back(const T& val)
	{
		if (this->_size >= this->_capacity - 1)
		{
			this->arr[this->_size] = val;
			T* newArray = new T[this->_capacity + 10];
			for (uint16_t i = 0; i < this->_capacity; i++)
				newArray[i] = this->arr[i];
			delete[] this->arr;
			this->arr = newArray;
			this->_capacity += 10;
			this->_size++;
		}
		else
			this->arr[this->_size++] = val;
	}

	void pop_back()
	{
		this->_size--;
	}
	void clear()
	{
		delete[] arr;
		this->_size = 0;
		this->_capacity = 0;
	}
	//void swap(Vector<T>& _Right)
	//{
	//	if (this != &_Right)
	//	{
	//		T* newArrayLeft = new T[_Right._capacity];
	//		for (uint16_t i = 0; i < this->_size; i++)
	//			newArrayLeft[i] = _Right.arr[i];

	//		T* newArrayRight = new T[this->_capacity];
	//		for (uint16_t i = 0; i < this->_size; i++)
	//			newArrayRight[i] = this->arr[i];

	//		delete[] _Right.arr;
	//		_Right.arr = newArrayRight;
	//		delete[] this->arr;
	//		this->arr = newArrayLeft;

	//		uint16_t buffer = this->_capacity;
	//		this->_capacity = _Right._capacity;
	//		_Right._capacity = buffer;

	//		buffer = this->_size;
	//		this->_size = _Right._size;
	//		_Right._size = buffer;
	//	}
	//}
	//void erase(uint16_t Where)
	//{
	//	for (uint16_t i = Where; i < this->_size - 1; i++)
	//		this->arr[i] = this->arr[i + 1];
	//	this->_size--;
	//}
	////vector<T>::iterator erase(vector<T>::const_iterator Where);
	////vector<T>::iterator insert(
	//   //vector<T>::const_iterator _Where,
	//   // initializer_list _Ilist
 //  // );
	////vector<T>::iterator insert(
	//   // vector<T>::const_iterator _Where,
	//   // const uint16_t _Count,
	//   // const T& Val
	////);
	////vector<T>::iterator insert(vector<T>::const_iterator _Where, T&& _Val);
	////vector<T>::iterator insert(vector<T>::const_iterator _Where, const T& _Val);
	//void insert(uint16_t _Where, T& _Val)
	//{
	//	if (_Where < 0) throw - 1;
	//	if (this->_size >= this->_capacity - 1)
	//	{
	//		T* newArray = new T[this->_capacity + 10];
	//		uint16_t j = 0;
	//		for (uint16_t i = 0; i < this->_capacity; i++)
	//		{
	//			if (i == _Where)
	//			{
	//				this->arr[i] = _Val;
	//				continue;
	//			}
	//			newArray[i] = this->arr[j++];
	//		}
	//		delete[] this->arr;
	//		this->arr = newArray;
	//		this->_capacity += 10;
	//		this->_size++;
	//	}
	//	else
	//	{
	//		for (uint16_t i = this->_size; i > _Where; i--)
	//			this->arr[i] = this->arr[i - 1];
	//		this->arr[_Where] = _Val;
	//	}
	//}
	//void insert(uint16_t _Where, const T& _Val)
	//{
	//	if (_Where < 0) throw - 1;
	//	if (this->_size >= this->_capacity - 1)
	//	{
	//		T* newArray = new T[this->_capacity + 10];
	//		uint16_t j = 0;
	//		for (uint16_t i = 0; i < this->_capacity; i++)
	//		{
	//			if (i == _Where)
	//			{
	//				this->arr[i] = _Val;
	//				continue;
	//			}
	//			newArray[i] = this->arr[j++];
	//		}
	//		delete[] this->arr;
	//		this->arr = newArray;
	//		this->_capacity += 10;
	//		this->_size++;
	//	}
	//	else
	//	{
	//		for (uint16_t i = this->_size; i > _Where; i--)
	//			this->arr[i] = this->arr[i - 1];
	//		this->arr[_Where] = _Val;
	//	}
	//}
	//vector<T>::iterator insert<_Iter <unnamed>>(
	   // vector<T>::const_iterator _Where,
	   // _Iter _First,
	   // _Iter _Last
   // );
};
