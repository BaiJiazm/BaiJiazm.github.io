class SmartPointer;

template<typename T>
class Counter {
		friend class SmartPointer<T>;
	private:
		T* ptr;
		int cnt;
	public:
		Counter() {
			ptr = NULL;
			cnt = 0;
		}
		Counter(T* p) {
			ptr = p;
			cnt = 1;
		}
		~Counter() {
			delete ptr;
		}
};

template <typename T>
class SmartPointer {
	private:
		Counter* ptr_counter;
	public:
		SmartPointer(T* p) {
			ptr_counter = new Counter(p);
		}
		SmartPointer(const SmartPointer &sp) {
			ptr_counter = sp.ptr_counter;
			++ptr_count->cnt;
		}
		SmartPointer& operator=(const SmartPointer &sp) {
			++sp.ptr_counter->cnt;
			--ptr_counter->cnt;
			if (ptr_counter->cnt == 0) {
				delete ptr_counter;
			}
			ptr_counter = sp.ptr_counter;
		}
		~SmartPointer() {
			--ptr_counter->cnt;
			if (ptr_counter->cnt == 0) {
				delete ptr_counter;
			}
		}
};

