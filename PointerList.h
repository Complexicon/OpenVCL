#ifndef PTRLIST_H_
#define PTRLIST_H_

// 
// cmplx' simple PointerList
//

template <class T> struct Element {
	T* ptr = nullptr;
	Element<T>* next = nullptr;
};

template <class T> class PointerList {
private:
	size_t len = 0;
	Element<T>* firstElement = new Element<T>();

public:

	// get length of list
	size_t length() { return len; }
	
	// clear list and delete contained pointers
	void clear() { for (size_t i = 0; i <= len; i++) delete pop(); }

	// check if list contains pointer
	bool contains(T* val) { return index_of(val) == -1 ? false : true; }

	// appends pointer to the list
	void append(T* val) {
		Element<T>* cur = firstElement;
		while (cur->next != nullptr) cur = cur->next;
		cur->next = new Element<T>();
		cur->ptr = val;
		len++;
	}

	// pushes pointer on top of the list
	void push(T* val) {
		firstElement->next = new Element<T>(*firstElement);
		firstElement->ptr = val;
		len++;
	}

	// pops first element off the list
	// returns popped element pointer.
	T* pop() {
		if (firstElement->next == nullptr) return nullptr;

		Element<T>* top = firstElement;
		T* ptr = top->ptr;
		firstElement = top->next;

		delete top;

		len--;

		return ptr;
	}

	// removes first element from the list
	// returns removed element pointer.
	T* remove_last() {
		if (firstElement->next == nullptr) return nullptr;

		Element<T>* cur = firstElement;
		while (cur->next->next != nullptr) cur = cur->next;

		T* ptr = cur->ptr;
		cur->ptr = nullptr;

		delete cur->next;
		cur->next = nullptr;

		len--;

		return ptr;

	}

	// removes nth element from the list
	// returns removed element pointer.
	T* remove(size_t n) {
		if (n + 1 > len) return nullptr;

		Element<T>* cur = firstElement;
		for (size_t i = 0; i < n; i++) cur = cur->next;

		T* ptr = cur->ptr;
		Element<T>* trgt = cur->next;

		cur->ptr = trgt->ptr;
		cur->next = trgt->next;
		delete trgt;

		len--;

		return ptr;

	}

	// gets the nth element from the list
	// returns element pointer.
	T* get(size_t n) {
		if (n + 1 > len) return nullptr;
		Element<T>* cur = firstElement;
		for (size_t i = 0; i < n; i++) cur = cur->next;
		return cur->ptr;
	}

	// sets nth element
	// returns true on success.
	bool set(int n, T* val) {
		if (n + 1 > len) return false;
		Element<T>* cur = firstElement;
		for (size_t i = 0; i < n; i++) cur = cur->next;
		delete cur->ptr;
		cur->ptr = val;
		return true;
	}

	// returns index of element if found
	int index_of(T* val) {
		if (len == 0) return -1;

		Element<T>* cur = firstElement;
		size_t index = 0;
		while (cur->ptr != val) {
			if (cur->next->next == nullptr) return -1;
			cur = cur->next;
			index++;
		}
		return index;
	}

	// swaps n1 with n2
	bool swap(int n1, int n2) {
		if (n1 + 1 > len || n2 + 1 > len) return false;

		Element<T>* e1 = firstElement;
		for (size_t i = 0; i < n1; i++) e1 = e1->next;

		Element<T>* e2 = firstElement;
		for (size_t i = 0; i < n2; i++) e2 = e2->next;

		T* temp = e1->ptr;
		e1->ptr = e2->ptr;
		e2->ptr = temp;

		return true;
	}

};

#endif