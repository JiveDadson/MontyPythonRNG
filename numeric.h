#pragma once

#include <limits>

namespace dj {

	template<class T>
	inline int int_cast(T x) {
		constexpr std::numeric_limits<int> lims;
		constexpr auto big = lims.max();
		assert(x<=big); // XXX to do - throw instead?
		if constexpr(std::numeric_limits<T>().is_signed) {
			constexpr auto small = lims.min();
			assert( x >= small); // XXX to do - throw instead?
		}
		return static_cast<int>(x);
	}
}