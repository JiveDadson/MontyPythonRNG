#pragma once

#include "math_consts.h"
#include <cassert>

namespace dj {
	template<class T> static inline T   fsign(T x, T y) { return ((y>0 && x<0) || (y<0 && x>0)) ? -x : x; }
	template<class T> static inline int fsign(T x) { return x >= 0 ? 1 : -1; }
	template<class T> static inline T   fsqr(T x) { return x * x; }
	template<class T> static inline T   fcube(T x) { return x * x * x; }

	template<class T>
	inline T mod(T a, T b) {
		assert(b > 0);
		T ret = a%b;
		return (ret<0)?(ret+b):ret;
	}

}