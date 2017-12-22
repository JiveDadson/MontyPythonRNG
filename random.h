#pragma once

#include <random>
#include "numeric.h"

namespace dj {

	// Random signed 32 bit integer
	template<class URNG> static inline double sranl(URNG& rng) {
		return static_cast<int32_t>(rng());
	}

	// Random double in [0:1] with 32 bit resolution
	template<class URNG> static inline double ranf(URNG& rng) {
		auto constexpr diff = URNG::max() - URNG::min();
		constexpr uint32_t max32 = 0xffffffffU;
		if constexpr (diff == max32) {
			auto raw = static_cast<uint32_t>(rng());
			return double(raw) / max32;
		} else {
			auto ret = std::generate_canonical<double, 32, URNG>(rng);
			return ret;
		}
	}

	// Gorge Marsaglia's "Monty Python" standard normal distribution
	template<class URNG>
	static inline double standard_normal(URNG& rng) {
		const double a = 1.177410;
		const double b = 2.506628;
		const double s = .8857913;

		double x = sranl(rng)*1.167239e-9;
		double fx = std::abs(x);
		if (fx<a) return x;
		double y = ranf(rng);
		// pre-tests
		double v = 2.8658 - fx * (2.0213 - .3605*fx);
		if (y<v)return x;
		if (y>v + .0506) return fsign(s*(b - fx), x);
		// end of pre-tests
		if (log(y)<.6931472 - .5*fsqr(x)) return x;
		x = fsign(s*(b - fx), x);
		if (log(1.8857913 - y)<.5718733 - .5*fsqr(x)) return x;
		do {
			v = sranl(rng)*4.656613e-10; // 1.0/0x7fffffff
			x = -log(std::abs(v)) / b;
			y = -log(1 - ranf(rng));
		} while (y + y<fsqr(x));
		return fsign(b + x, v);
	}

	template <class Flt=double> 
	class normal_distribution {
	public:
		explicit normal_distribution(Flt mean = 0, Flt stddev= 1)
			: params(mean, stddev)
		{}

		typedef normal_distribution<Flt> My_t;
		typedef Flt result_type;

		struct param_type
		{	// parameter package
			typedef My_t distribution_type;
			result_type mean;
			result_type stddev;
			explicit param_type(Flt mean, Flt stddev)
				: mean(mean)
				, stddev(stddev)
			{}
		};

		template<class URNG>
		Flt operator() (URNG &rng) const {
			auto ret = params.stddev*standard_normal(rng) + params.mean;
			return static_cast<Flt>(ret);
		}

	private:
		param_type params;
	};

	// George Marsaglia's long period (2^60) random number generator
	// Satisfies UniformRandomBitsGenerator concept
	class Mars_URNG {
	public:

		Mars_URNG(uint32_t s1 = 0, uint32_t s2 = 0) {
			seed(s1, s2);
		}

		void seed(uint32_t s1 = 0, uint32_t s2 = 0) {
			z = good(s1); w = good(s2);
		}

		// The following four definitions statisfy the URNG concept
		inline uint32_t operator() () {
			return znew() | wnew();
		}
		constexpr uint32_t min() { return 0; }
		constexpr uint32_t max() { return 0xffffffff; }
		using result_type = uint32_t;

	private:
		static const uint32_t arby = 0xA5A5A5A5; // Arbitrary default seed
		// Zero is a baaad seed.
		static uint32_t good(uint32_t s) { return s ? s : arby; }
		uint32_t z;
		uint32_t w;
		inline uint32_t znew() { z = 36969 * (z & 0xffff) + (z >> 16); return z << 16; }
		inline uint32_t wnew() { w = 18000 * (w & 0xffff) + (w >> 16); return w & 0xffff; }
	};
}
