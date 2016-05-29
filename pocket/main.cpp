/*---------------------------------------------------------------------
* _UNUSING_MATH_INT_FLOAT	: int, float型を使用しない
* _USING_MATH_DOUBLE		: double型を使用する
* _USING_MATH_LONG_DOUBLE	: long double型を使用する
* _USING_MATH_IO			: 数学クラスの入出力オペレーターを使用する
* _USING_MATH_EXTERN_TYPE	: 数学クラスの暗黙定義を抑制する
* _UNUSING_SIMD				: SIMDが使用できる環境でも使用しないようにする
*---------------------------------------------------------------------*/

#define _USING_MATH_IO
//#define _UNUSING_SIMD

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
#endif /* _USE_CXX11 */

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
	const long N = LONG_MAX / 10;

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
		<< pocket::make_simd<float>(0, 1, 2, 3) << endl
		<< pocket::Mathf::SinCos(320.0f) << endl << endl
		<< pocket::Planef::Up << endl
		;
	pocket::Matrix4x4f s, r, t;
	s.load_scale(2.0f);
	r.load_rotate_z(30.0f);
	t.load_translate(0.0f, 1.0f, 2.0f);

	{
		Timer calc("matrix");
		for (int i = 0; i < N; ++i)
		{
			pocket::Matrix4x4f m = s * r * t;
			m.load_identity();
		}
	}
	
	return 0;
}
