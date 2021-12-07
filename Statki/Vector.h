#pragma once

template <typename T> class Vector {
public:
	T* arr;
	int max_length;
	int cur_length;
	Vector();
	void push(T data);
	void push(T data, int index);
	T get( int index);
	T get();
	T pop();
	int getLength();
	int getSize();
	void print();
	T operator [](int i);
};

template <typename T>
Vector<T>::Vector() {
	arr = new T[1];
	max_length = 1;
	cur_length = 0;
}

template <typename T>
void Vector<T>::push(T data) {
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
		cur_length++;
	}

template <typename T>
void Vector<T>::push(T data, int index) {
	if (index == max_length) {
		push(data);
	}
	else {
		arr[index] = data;
	}
}

template <typename T>
T Vector<T>::pop() {
	cur_length--;
	return arr[cur_length + 1];
}

template <typename T>
T Vector<T>::get() {
	return arr[cur_length - 1];
}

template <typename T>
T Vector<T>::get(int index) {
	if (index < cur_length) {
		return arr[index];
	}
}

template <typename T>
int Vector<T>::getLength() {
	return cur_length;
}

template <typename T>
int Vector<T>::getSize() {
	return max_length;
}
template <typename T>
void Vector<T>::print() {
	for (int i = 0; i < cur_length; i++) {
		std::cout << arr[i] << " ";
	}
}

//Dont use this doesdnt work
template <typename T>
T Vector<T>::operator [](int i) {
	return arr[i];
}


