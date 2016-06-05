# pocket library
ちょこちょこ追加していく小さなライブラリ.

## test compiler
***
* VisualC++ : 12, 14

* GCC : 5.2

## simd
***
#### ※ float128: true, float256: falseの環境で, Vector4d を使用すると遅くなると思う.

* 使用できるか確かめる方法.
```cpp
#include "math/SIMD.h"

std::cout << std::boolalpha <<
	"float128: " << pocket::SIMD<float>::is_vectorize() << std::endl <<
	"float256: " << pocket::SIMD<double>::is_vectorize() << std::endl;
```
```cpp
#include "math/SIMD.h"

// SSE
// SSE2
// SSE3
// SSE4
// AVX
// AVX2
#ifdef _USE_SIMD
	std::cout << _USE_SIMD_TYPE_STRING << std::endl;
#endif // _USE_SIMD
```

* 使用できる環境でも使用しない.
```cpp
#define _UNUSING_SIMD
#include "math/SIMD.h"
```

## user defines
***
```cpp
// int, float型を使用しない
#define _UNUSING_MATH_INT_FLOAT
// double型を使用する
#define _USING_MATH_DOUBLE
// long double型を使用する
#define _USING_MATH_LONG_DOUBLE
// 数学クラスの入出力オペレーターを使用する
#define _USING_MATH_IO
// SIMDが使用できる環境でも使用しないようにする
#define _UNUSING_SIMD
```

## behavior
***
```cpp
#include "math/all.h"

namespace behavior = pocket::behavior;
pocket::Vector3<float> v, v2(4.0f, 1.0f, 5.0f);
std::cout << v(behavior::from_pitch_yaw, 45.0f, 20.0f)(behavior::add, v2)(behavior::normalize)(behavior::dot, v2) << std::endl;

#ifdef _USE_CXX11
std::cout << 30.0f * behavior::sin * behavior::vec2 << endl;
#endif // _USE_CXX11
```
