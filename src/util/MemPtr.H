#ifndef MEMPTR_H
#define MEMPTR_H

#include <string>
#include <cstring>
#include <inttypes.h>

template<int N>
class MemPtr
{
	public:
		template<typename T> struct type { };
		MemPtr(char *ptr_)
		{
			_ptr = ptr_;
			_totalSize = N;
			_actualSize = 0;
		}

		template<typename T>
		T getValue()
		{
			return get_as(type<T>());
		}

		void setValue(void *ptr_,int size_)
		{
			memcpy(_ptr,ptr_,size_);
			_actualSize = size_;
		}
	private:
		template<typename T> 
		T get_as(type<T>)
		{
		}

		std::string get_as(type<std::string>)
		{
			std::string data(_ptr);
			return data;
		}

		char get_as(type<char>)
		{
			char data = _ptr[0];
			return data;
		}

		uint16_t get_as(type<uint16_t>)
		{
			uint16_t data;

			memcpy(&data,_ptr,2);
			return data;
		}

		uint8_t get_as(type<uint8_t>)
		{
			uint8_t data;

			memcpy(&data,_ptr,1);
			return data;
		}

		uint32_t get_as(type<uint32_t>)
		{
			uint32_t data;

			memcpy(&data,_ptr,4);
			return data;
		}

                double get_as(type<double>)
                {
                        double data;

                        memcpy(&data,_ptr,sizeof(double));
                        return data;
                }


		char *_ptr;
		int  _totalSize;
		int _actualSize;
};

#endif
