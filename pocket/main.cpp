/*---------------------------------------------------------------------
* _UNUSING_MATH_INT_FLOAT	: int, float型を使用しない
* _USING_MATH_DOUBLE		: double型を使用する
* _USING_MATH_LONG_DOUBLE	: long double型を使用する
* _USING_MATH_IO			: 数学クラスの入出力オペレーターを使用する
* _UNUSING_SIMD				: SIMDが使用できる環境でも使用しないようにする
*---------------------------------------------------------------------*/

#define _USING_MATH_IO
//#define _UNUSING_SIMD
//#define _USING_MATH_DOUBLE

#include "math/all.h"
#include "fixed_array.h"
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>

//_NS_USING_POCKET;

using std::cout;
using std::endl;

#ifdef _USE_CXX11
#include <chrono>
#else
#include <ctime>
#endif // _USE_CXX11

class Timer
{
public:
	Timer(const std::string& s) :
		m(s),
#ifdef _USE_CXX11
		p(std::chrono::system_clock::now())
#else
		c(std::clock())
#endif
	{

	}
	~Timer()
	{
		cout << m << endl;
#ifdef _USE_CXX11
		cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - p).count() << endl;
#else
		cout << (std::clock() - c) << endl;
#endif
	}

private:
	std::string m;
#ifdef _USE_CXX11
	std::chrono::system_clock::time_point p;
#else
	std::clock_t c;
#endif
};

int main(int argc, char** argv)
{
	cout << std::fixed << std::boolalpha << std::showpos;
	//const long N = LONG_MAX / 10;

	pocket::container::array<int, 3> ary = {0, 1, 20};
	cout << ary << endl;

	cout << pocket::vector2f::unit_x << endl
		<< pocket::vector3f::unit_y << endl
		<< pocket::vector4f::unit_z << endl
		<< pocket::matrix3x3f::identity << endl
		<< pocket::matrix4x4f::identity << endl
		<< pocket::quaternionf::identity << endl
		<< pocket::rectanglef(0.0f, 320.0f, 0.0f, 240.0f) << endl
		<< pocket::colorf::white << endl
		<< pocket::math_traitsf::sin_cos_t(320.0f) << endl << endl
		<< pocket::planef::up << endl;

	namespace behavior = pocket::behavior;
	pocket::matrix4x4f s, r, t;
	s(behavior::scale, 2.0f);
	r(behavior::rotate, behavior::x, 30.0f);
	t(behavior::translate, 0.0f, 1.0f, 2.0f);

	cout << s * r * t << endl;

	typedef pocket::simd_traits<float> simd;
	cout << simd::is_vectorize() << endl;
	simd::type zero = simd::zero();
	simd::type one = simd::one();
	cout << simd::equal(zero, zero) << endl;
	cout << simd::greater_equal(zero, one) << endl;
	cout << simd::less_equal(zero, one) << endl;

	pocket::float2 f2(5.0f, 1.0f);
	cout << f2 << endl;
	pocket::float3 f3a(f2, 2.0f);
	cout << f3a << endl;
	pocket::float3 f3b(10.0f, f2);
	cout << f3b << endl;
	pocket::float4 f4a(f3a, 100.0f);
	cout << f4a << endl;
	pocket::float4 f4b(-10.0f, f2, 100.0f);
	cout << f4b << endl;

	pocket::bool4 b4(true, true, false, true);
	cout << b4 << endl;
	pocket::bool3 b3(true, false, true);
	cout << b3 << endl;
	pocket::bool2 b2(b3);
	cout << b2 << endl;
	pocket::bool2 b2a(b4);
	cout << b2a << endl;
	pocket::bool4 b4a(true, b2, false);
	cout << b4a << endl;

	return 0;
}
