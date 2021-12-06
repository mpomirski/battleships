#include <iostream>
#include "vector.h"

template <typename T> class vector {
public:
	vector() {
		arr = new T[1];
		max_length = 1;
		cur_length = 0;
	}

	void push(data) {
		if (cur_length == max_length) {
			T* temp = new T[2 * max_length];
			for (int i = 0; i < max_length; i++) {
				temp[i] = arr[i];
			}

			delete[] arr;
			max_length *= 2;
			arr = temp;
		}
		arr[cur_length] = data;
		cur_lenth++;
	}

	void push(T data, int index) {
		if (index == max_length) {
			push(data);
		}
		else {
			arr[index] = data;
		}
	}
	T pop() {
		cur_length--;
		return arr[cur_length + 1];
	}

	T get(int index) {

		if (index < cur_length) {
			return arr[index];
		}
	}

	int getLength() {
		return cur_length;
	}

	int getSize() {
		return max_length;
	}

	void print() {
		for (int i = 0; i < cur_length; i++) {
			std::cout << arr[i] << " ";
		}
	}

	T operator [](int i) {
		return arr[i];
	}
};
