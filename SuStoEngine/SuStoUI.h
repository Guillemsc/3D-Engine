#ifndef SUSTO_UI
#define SUSTO_UI

namespace SuStoUI
{
	// Data types
	struct Vec2 
	{
		Vec2();
		Vec2(float x, float y);
		Vec2(const Vec2& copy);
		bool operator == (const Vec2& comp);

		float x, y = 0;
	};

	struct Vec3 
	{
		Vec3();
		Vec3(float x, float y, float z);
		Vec3(const Vec3& copy);
		bool operator == (const Vec3& comp);

		float x, y, z = 0;
	};

	struct Rect 
	{
		Rect();
		Rect(float x, float y, float w, float z);
		Rect(const Rect& copy);
		bool operator == (const Rect& comp);

		float x, y, w, z = 0;
	};

	struct Color 
	{
		Color();
		Color(float r, float g, float b, float a);
		Color(const Color& copy);
		bool operator == (const Color& comp);

		float r, g, b, a = 0;
	};

	template<typename T>
	class Vector
	{
	public:
		typedef T                   value_type;
		typedef value_type*         iterator;
		typedef const value_type*   const_iterator;

		Vector();
		~Vector();
		bool empty();
		int size();

		value_type& operator[](int i);
		const value_type& operator[](int i) const;

		void clear();
		iterator begin();
		const_iterator begin();
		iterator end();
		const_iterator end();
		value_type& front();
		const value_type& front();
		value_type& back();
		const value_type& back();
		void swap(Vector<T>& rhs);

		void push_back(const value_type& v);
		void pop_back();
		iterator erase();
		iterator insert(const_iterator it, const value_type& v);

	private:
		int capacity();
		int _grow_capacity(int size) const;
		void resize(int new_size);
		void resize(int new_size, const T& v);
		void reserve(int new_capacity);

		//ImVector() { Size = Capacity = 0; Data = NULL; }
		//~ImVector() { if (Data) ImGui::MemFree(Data); }

		//inline bool                 empty() const { return Size == 0; }
		//inline int                  size() const { return Size; }
		//inline int                  capacity() const { return Capacity; }

		//inline value_type&          operator[](int i) { IM_ASSERT(i < Size); return Data[i]; }
		//inline const value_type&    operator[](int i) const { IM_ASSERT(i < Size); return Data[i]; }

		//inline void                 clear() { if (Data) { Size = Capacity = 0; ImGui::MemFree(Data); Data = NULL; } }
		//inline iterator             begin() { return Data; }
		//inline const_iterator       begin() const { return Data; }
		//inline iterator             end() { return Data + Size; }
		//inline const_iterator       end() const { return Data + Size; }
		//inline value_type&          front() { IM_ASSERT(Size > 0); return Data[0]; }
		//inline const value_type&    front() const { IM_ASSERT(Size > 0); return Data[0]; }
		//inline value_type&          back() { IM_ASSERT(Size > 0); return Data[Size - 1]; }
		//inline const value_type&    back() const { IM_ASSERT(Size > 0); return Data[Size - 1]; }
		//inline void                 swap(ImVector<T>& rhs) { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; value_type* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

		//inline int                  _grow_capacity(int size) const { int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; return new_capacity > size ? new_capacity : size; }

		//inline void                 resize(int new_size) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
		//inline void                 resize(int new_size, const T& v) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) Data[n] = v; Size = new_size; }
		//inline void                 reserve(int new_capacity)
		//{
		//	if (new_capacity <= Capacity) return;
		//	T* new_data = (value_type*)ImGui::MemAlloc((size_t)new_capacity * sizeof(T));
		//	if (Data)
		//		memcpy(new_data, Data, (size_t)Size * sizeof(T));
		//	ImGui::MemFree(Data);
		//	Data = new_data;
		//	Capacity = new_capacity;
		//}

		//inline void                 push_back(const value_type& v) { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); Data[Size++] = v; }
		//inline void                 pop_back() { IM_ASSERT(Size > 0); Size--; }

		//inline iterator             erase(const_iterator it) { IM_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(value_type)); Size--; return Data + off; }
		//inline iterator             insert(const_iterator it, const value_type& v) { IM_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(Capacity ? Capacity * 2 : 4); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(value_type)); Data[off] = v; Size++; return Data + off; }


	private:
		int Size = 0;
		int Capacity = 0;
		T*  Data = nullptr;
	};

	class SuStoUIMain
	{
	public:
		SuStoUIMain();
		~SuStoUIMain();

		void Awake();
		void Start();
		void PreUpdate();
		void Update();
		void PostUpdate();
		void CleanUp();

	private:
	
	};
}






















#endif // !SUSTO_UI
