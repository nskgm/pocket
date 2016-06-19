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
	std::setw(2);
	//const long N = LONG_MAX / 10;

	pocket::container::array<int, 3> ary = {0, 1, 20};
	cout << ary << endl;

	namespace behavior = pocket::behavior;

	pocket::vector2f v2(behavior::zero);
	pocket::vector3f v3(behavior::zero);
	pocket::vector4f v4(behavior::zero);
	pocket::matrix3x3f m3(behavior::identity);
	pocket::matrix4x4f m4(behavior::identity);
	pocket::quaternionf q(behavior::identity);
	pocket::rectanglef r(640.0f, 480.0f);
	pocket::colorf c(0.5f);
	pocket::planef p(behavior::down, 40.0f);
	pocket::math_traitsf::sin_cos_t sc = 30.0f;
	pocket::ray3f r2(pocket::vector3f::up);
	pocket::line3f l(r2, 50.0f);

	pocket::frustumf::look_at_t la(pocket::vector3f(0.0f, 0.0f, 10.0f), pocket::vector3f::zero);
	pocket::frustumf::projection_field_of_view_t fov(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	pocket::frustumf f(la, fov);

	cout <<
		v2 << endl <<
		v3 << endl <<
		v4 << endl <<
		m3 << endl <<
		m4 << endl <<
		q << endl <<
		r << endl <<
		c << endl <<
		p << endl <<
		sc << endl <<
		f << endl <<
		r2 << endl <<
		l << endl;

	typedef pocket::simd_traits<float> simd;
	cout << simd::is_vectorize() << endl;
	simd::type zero = simd::zero();
	simd::type one = simd::one();
	cout << simd::equal(zero, zero) << endl;
	cout << simd::greater_equal(zero, one) << endl;
	cout << simd::less_equal(zero, one) << endl;

	return 0;
}
