#ifndef SUSTO_UI
#define SUSTO_UI




namespace SuStoUI
{
	struct Vec2 
	{
		Vec2();
		Vec2(float x, float y);
		Vec2(const Vec2& copy);

		float x, y = 0;
	};

	struct Vec3 
	{
		Vec3();
		Vec3(float x, float y, float z);
		Vec3(const Vec3& copy);

		float x, y, z = 0;
	};

	struct Rect 
	{
		Rect();
		Rect(float x, float y, float w, float z);
		Rect(const Rect& copy);

		float x, y, w, z = 0;
	};

	struct Color 
	{

	};


}






















#endif // !SUSTO_UI
