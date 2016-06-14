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

	cout << pocket::Vector2f::UnitX << endl
		<< pocket::Vector3f::UnitY << endl
		<< pocket::Vector4f::UnitZ << endl
		<< pocket::Matrix3x3f::Identity << endl
		<< pocket::Matrix4x4f::Identity << endl
		<< pocket::Quaternionf::Identity << endl
		<< pocket::Rectanglef(0.0f, 320.0f, 0.0f, 240.0f) << endl
		<< pocket::Colorf::White << endl
		<< pocket::Mathf::SinCos(320.0f) << endl << endl
		<< pocket::Planef::Up << endl
		;
	pocket::Matrix4x4f s, r, t;
	s(pocket::behavior::scale, 2.0f);
	r(pocket::behavior::rotate, pocket::behavior::x, 30.0f);
	t(pocket::behavior::translate, 0.0f, 1.0f, 2.0f);

	cout << s * r * t << endl;

	typedef pocket::SIMD<float> simd;
	cout << simd::is_vectorize() << endl;
	simd::type zero = simd::zero();
	simd::type one = simd::one();
	cout << simd::equal(zero, zero) << endl;
	cout << simd::greater_equal(zero, one) << endl;
	cout << simd::less_equal(zero, one) << endl;

	pocket::Vector4f v, d(2.0f, 5.0f, 4.0f, 0.0f);
	v.from_pitch_yaw(45.0f, 15.0f);
	cout << v << endl;
	v += d;
	cout << v << endl;
#ifdef _USE_SIMD_ANONYMOUS
	v.w1();
	cout << v << endl;
	pocket::Vector4f v2(simd::select1110(v.mm));
	cout << v2 << endl;
#endif // _USE_SIMD_ANONYMOUS

	cout << v.cross(d) << endl;

	pocket::bool2 b = {true, false};
	cout << b << endl;
	pocket::float3 f = {4.0f, 1.0f, 5.0f};
	cout << f << endl;

	return 0;
}
