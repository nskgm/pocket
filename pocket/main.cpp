/*---------------------------------------------------------------------
* _UNUSING_MATH_INT_FLOAT	: int, float型を使用しない
* _USING_MATH_DOUBLE		: double型を使用する
* _USING_MATH_LONG_DOUBLE	: long double型を使用する
* _UNUSING_SIMD				: SIMDが使用できる環境でも使用しないようにする
*---------------------------------------------------------------------*/

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
	static_cast<void>(argc);
	static_cast<void>(argv);

	cout << std::fixed << std::boolalpha << std::showpos;
	//const long N = LONG_MAX / 10;

	pocket::container::array<pocket::vector2f, 3> ary = {
		pocket::vector2f::zero,
		pocket::vector2f::one,
		pocket::vector2f::unit_x
	};
	cout << ary << endl;

	namespace behavior = pocket::behavior;

	pocket::vector2f v2(behavior::half_of_half);
	pocket::vector3f v3(behavior::half);
	pocket::vector4f v4(behavior::one, behavior::direction);
	pocket::matrix3x3f m3(behavior::identity);
	pocket::matrix4x4f m4(behavior::identity);
	pocket::quaternionf q(behavior::identity);
	pocket::rectanglef r(640.0f, 480.0f);
	pocket::colorf c(0x7F5080FF);
	pocket::planef p(behavior::down, 40.0f);
	pocket::math_traitsf::sin_cos_t sc = 30.0f;
	pocket::ray3f r2(pocket::vector3f::up);
	pocket::line3f l(r2, 50.0f);

	pocket::frustumf::look_at_t la(pocket::vector3f(0.0f, 5.0f, 10.0f), pocket::vector3f::zero);
	pocket::frustumf::projection_field_of_view_t fov(45.0f, r.aspect(), 0.1f, 100.0f);
	pocket::frustumf f(la, fov);

	cout << endl << "--------------------" << endl <<
		"vector2(half_of_half): " << v2 << endl <<
		"vector3(half): " << v3 << endl <<
		"vector4(one, position): " << v4 << endl <<
		"matrix3x3(identity): " << m3 << endl <<
		"matrix4x4(identity): " << m4 << endl <<
		"quaternion(identity): " << q << endl <<
		"rectangle(640, 480): " << r << endl <<
		"color(0xFF0080FF): " << c << endl <<
		"plane(down, 40): " << p << endl <<
		"sin_cos_t(30): " << sc << endl <<
		"ray3(up): " << r2 << endl <<
		"line3(ray3, 50): " << l << endl <<
		"frustum: " << f << endl <<
		"--------------------" << endl << endl;

	typedef pocket::simd_traits<float> simd;
	simd::type zero = simd::zero();
	simd::type one = simd::one();
	simd::type set = simd::set(4.0f, 2.0f, 5.0f, 3.0f);
	simd::type set2 = simd::set(14.0f, 12.0f, 15.0f, 13.0f);
	cout << "-----simd-----" << endl <<
		"is_vectorize: " << simd::is_vectorize() << endl <<
		"set(4, 2, 5, 3): " << set << endl <<
		"set2(14, 12, 15, 13): " << set2 << endl <<
		"shuffle<1, 0, 2, 1>(set, set2): " << simd::shuffle<1, 0, 3, 2>(set, set2) << endl <<
		"equal(zero, zero): " << simd::equal(zero, zero) << endl <<
		"less_equal(zero, one): " << simd::less_equal(zero, one) << endl <<
		"---------------" << endl;

	pocket::bool4 b4(true, false, false, true);
	pocket::int4 i4(0, 1, 2, 3);
	pocket::float4 f4(1.0f, 2.0f, 5.0f, 3.0f);
	pocket::byte4 by4('A', 'b', 'c', 'D');
	cout << "-----fixed_array-----" << endl <<
		b4 << endl <<
		i4 << endl <<
		f4 << endl <<
		by4 << endl <<
		"---------------" << endl;

	return 0;
}
