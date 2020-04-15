#pragma once

namespace Ubpa {
	template<typename T>
	inline transform<T>::transform(const mat<T, 4>& m) noexcept
		: transform(m[0], m[1], m[2], m[3]) {}

	template<typename T>
	inline transform<T>::transform(const mat<T, 3>& m) noexcept :
		transform{ std::array<T, 4 * 4>{
			m[0][0], m[1][0], m[2][0],    0,
			m[0][1], m[1][1], m[2][1],    0,
			m[0][2], m[1][2], m[2][2],    0,
			      0,       0,       0,    1 } } { }

	template<typename T>
	transform<T>::transform(const point<T, 3>& p) noexcept :
		transform{ std::array<T, 4 * 4>{
			1, 0, 0, p[0],
			0, 1, 0, p[1],
			0, 0, 1, p[2],
			0, 0, 0,    1 } } { }

	template<typename T>
	transform<T>::transform(const scale<T, 3>& s) noexcept :
		transform{ std::array<T, 4 * 4>{
			s[0],    0,    0,    0,
			   0, s[1],    0,    0,
			   0,    0, s[2],    0,
			   0,    0,    0,    1 } } { }
	
	template<typename T>
	transform<T>::transform(const quat<T>& q) noexcept {
		T x = q.imag()[0];
		T y = q.imag()[1];
		T z = q.imag()[2];
		T w = q.real();

		T xx = x * x;
		T xy = x * y;
		T xz = x * z;
		T xw = x * w;
		T yy = y * y;
		T yz = y * z;
		T yw = y * w;
		T zz = z * z;
		T zw = z * w;

		this->init(std::array<T, 4 * 4> {
			1 - 2 * (yy + zz),     2 * (xy - zw),     2 * (xz + yw), 0,
			    2 * (xy + zw), 1 - 2 * (zz + xx),     2 * (yz - xw), 0,
				2 * (xz - yw),     2 * (yz + xw), 1 - 2 * (xx + yy), 0,
				            0,                 0,                 0, 1,
		});
	}

	template<typename T>
	transform<T>::transform(const euler<T>& e) noexcept {
		T cX = std::cos(e[0]);
		T sX = std::sin(e[0]);
		T cY = std::cos(e[1]);
		T sY = std::sin(e[1]);
		T cZ = std::cos(e[2]);
		T sZ = std::sin(e[2]);

		this->init(std::array<T, 4 * 4>{
			  cY * cZ + sX * sY * sZ, - cY * sZ + sX * sY * cZ, cX * sY, 0,
			                 cX * sZ,                  cX * cZ,    - sX, 0,
			- sY * cZ + sX * cY * sZ,   sY * sZ + sX * cY * cZ, cX * cY, 0,
			                       0,                        0,       0, 1,
		});
	}

	template<typename T>
	transform<T>::transform(const point<T, 3>& t, const scale<T, 3>& s) noexcept :
		transform{ std::array<T, 4 * 4>{
			s[0],    0,    0, t[0],
			   0, s[1],    0, t[1],
			   0,    0, s[2], t[2],
			   0,    0,    0,    1 } } { }

	template<typename T>
	transform<T>::transform(const point<T, 3>& p, const quat<T>& q) noexcept {
		T x = q.imag()[0];
		T y = q.imag()[1];
		T z = q.imag()[2];
		T w = q.real();

		T xx = x * x;
		T xy = x * y;
		T xz = x * z;
		T xw = x * w;
		T yy = y * y;
		T yz = y * z;
		T yw = y * w;
		T zz = z * z;
		T zw = z * w;

		this->init(std::array<T, 4 * 4> {
			1 - 2 * (yy + zz),     2 * (xy - zw),     2 * (xz + yw), p[0],
			    2 * (xy + zw), 1 - 2 * (zz + xx),     2 * (yz - xw), p[1],
				2 * (xz - yw),     2 * (yz + xw), 1 - 2 * (xx + yy), p[2],
				            0,                 0,                 0,    1,
		});
	}

	template<typename T>
	transform<T>::transform(const point<T, 3>& p, const scale<T, 3>& s, const quat<T>& q) noexcept {
		T x = q.imag()[0];
		T y = q.imag()[1];
		T z = q.imag()[2];
		T w = q.real();

		T xx = x * x;
		T xy = x * y;
		T xz = x * z;
		T xw = x * w;
		T yy = y * y;
		T yz = y * z;
		T yw = y * w;
		T zz = z * z;
		T zw = z * w;

		this->init(std::array<T, 4 * 4> {
			s[0] * (1 - 2 * (yy + zz)), s[1] * (    2 * (xy - zw)), s[2] * (    2 * (xz + yw)), p[0],
			s[0] * (    2 * (xy + zw)), s[1] * (1 - 2 * (zz + xx)), s[2] * (    2 * (yz - xw)), p[1],
			s[0] * (    2 * (xz - yw)), s[1] * (    2 * (yz + xw)), s[2] * (1 - 2 * (xx + yy)), p[2],
			                         0,                          0,                          0,    1,
		});
	}

	template<typename T>
	transform<T>::transform(const vec<T, 3>& axis, T theta) noexcept {
		auto a = axis.normalize();

		T sinTheta = std::sin(theta);
		T cosTheta = std::cos(theta);
		T OneMinusCosTheta = ONE<T> - cosTheta;

		auto& m = static_cast<transform&>(*this);
		// Compute rotation of first basis vector
		m(0, 0) = a[0] * a[0] * OneMinusCosTheta + cosTheta;
		m(0, 1) = a[0] * a[1] * OneMinusCosTheta - a[2] * sinTheta;
		m(0, 2) = a[0] * a[2] * OneMinusCosTheta + a[1] * sinTheta;
		m(0, 3) = 0;

		// Compute rotations of second and third basis vectors
		m(1, 0) = a[0] * a[1] * OneMinusCosTheta + a[2] * sinTheta;
		m(1, 1) = a[1] * a[1] * OneMinusCosTheta + cosTheta;
		m(1, 2) = a[1] * a[2] * OneMinusCosTheta - a[0] * sinTheta;
		m(1, 3) = 0;

		m(2, 0) = a[0] * a[2] * OneMinusCosTheta - a[1] * sinTheta;
		m(2, 1) = a[1] * a[2] * OneMinusCosTheta + a[0] * sinTheta;
		m(2, 2) = a[2] * a[2] * OneMinusCosTheta + cosTheta;
		m(2, 3) = 0;

		m(3, 0) = 0;
		m(3, 1) = 0;
		m(3, 2) = 0;
		m(3, 3) = 1;
	}

	template<typename T>
	const transform<T> transform<T>::look_at(const point<T, 3>& pos, const point<T, 3>& target, const vec<T, 3>& up) noexcept {
		// R
		// [ right  ]
		// [ camUp  ]
		// [ -front ]
		//
		// T
		// [ pos[0] ]
		// [ pos[1] ]
		// [ pos[2] ]
		//
		// m
		// [ R -RT ]
		// [ 0   1 ]

		assert(up.is_normalized());

		const vec<T,3> front = (target - pos).normalize();
		assert(front != up);
		vec<T, 3> right = front.cross(up).normalize();
		const vec<T,3> camUp = right.cross(front);
		auto posV = pos.cast_to<vec<T, 3>>();

		transform m;

		m(0, 0) = right[0];
		m(0, 1) = right[1];
		m(0, 2) = right[2];
		m(0, 3) = -right.dot(posV);

		m(1, 0) = camUp[0];
		m(1, 1) = camUp[1];
		m(1, 2) = camUp[2];
		m(1, 3) = -camUp.dot(posV);

		m(2, 0) = -front[0];
		m(2, 1) = -front[1];
		m(2, 2) = -front[2];
		m(2, 3) = front.dot(posV);

		m(3, 0) = 0;
		m(3, 1) = 0;
		m(3, 2) = 0;
		m(3, 3) = 1;

		return m;
	}

	template<typename T>
	const transform<T> transform<T>::orthographic(T width, T height, T zNear, T zFar) noexcept {
		assert(width > 0 && height > 0 && zNear >= 0 && zFar > zNear);

		T m00 = 2 / width;
		T m11 = 2 / height;
		T m22 = 2 / (zNear - zFar);
		T m23 = (zFar + zNear) / (zNear - zFar);

		return std::array<T, 4 * 4>{
			m00,   0,   0,   0,
			  0, m11,   0,   0,
			  0,   0, m22, m23,
			  0,   0,   0,   1,
		};
	}

	template<typename T>
	const transform<T> transform<T>::perspective(T fovY, T aspect, T zNear, T zFar) noexcept {
		assert(fovY > 0 && aspect > 0 && zNear >= 0 && zFar > zNear);

		T tanHalfFovY = std::tan(fovY / static_cast<T>(2));
		T cotHalfFovY = 1 / tanHalfFovY;

		T m00 = cotHalfFovY / aspect;
		T m11 = cotHalfFovY;
		T m22 = (zFar + zNear) / (zNear - zFar);
		T m23 = (2 * zFar * zNear) / (zNear - zFar);

		return std::array<T, 4 * 4>{
			m00,   0,   0,   0,
			  0, m11,   0,   0,
			  0,   0, m22, m23,
			  0,   0,  -1,   0,
		};
	}

	template<typename T>
	const transform<T> transform<T>::inverse_sim() const noexcept {
		// ref: https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html
#ifdef UBPA_USE_XSIMD
		if constexpr (std::is_same_v<T, float>) {
			transform<T> r;
			const auto& inM = *this;

			// transpose 3x3, we know m03 = m13 = m23 = 0
			__m128 t0 = VecShuffle_0101(inM[0], inM[1]); // 00, 01, 10, 11
			__m128 t1 = VecShuffle_2323(inM[0], inM[1]); // 02, 03, 12, 13
			r[0] = VecShuffle(t0, inM[2], 0, 2, 0, 3); // 00, 10, 20, 23(=0)
			r[1] = VecShuffle(t0, inM[2], 1, 3, 1, 3); // 01, 11, 21, 23(=0)
			r[2] = VecShuffle(t1, inM[2], 0, 2, 2, 3); // 02, 12, 22, 23(=0)

			// (SizeSqr(mVec[0]), SizeSqr(mVec[1]), SizeSqr(mVec[2]), 0)
			__m128 sizeSqr;
			sizeSqr = _mm_mul_ps(r[0], r[0]);
			sizeSqr = _mm_add_ps(sizeSqr, _mm_mul_ps(r[1], r[1]));
			sizeSqr = _mm_add_ps(sizeSqr, _mm_mul_ps(r[2], r[2]));

			// optional test to avoid divide by 0
			__m128 one = _mm_set1_ps(1.f);
#if 0
			// for each component, if(sizeSqr < SMALL_NUMBER) sizeSqr = 1;
			__m128 rSizeSqr = _mm_blendv_ps(
				_mm_div_ps(one, sizeSqr),
				one,
				_mm_cmplt_ps(sizeSqr, _mm_set1_ps(EPSILON<float>))
			);
#else
			__m128 rSizeSqr = _mm_div_ps(one, sizeSqr);
#endif

			r[0] = _mm_mul_ps(r[0], rSizeSqr);
			r[1] = _mm_mul_ps(r[1], rSizeSqr);
			r[2] = _mm_mul_ps(r[2], rSizeSqr);

			// last line
			r[3] = _mm_mul_ps(r[0], VecSwizzle1(inM[3], 0));
			r[3] = _mm_add_ps(r[3], _mm_mul_ps(r[1], VecSwizzle1(inM[3], 1)));
			r[3] = _mm_add_ps(r[3], _mm_mul_ps(r[2], VecSwizzle1(inM[3], 2)));
			r[3] = _mm_sub_ps(_mm_setr_ps(0.f, 0.f, 0.f, 1.f), r[3]);

			return r;
		}
		else
#endif
		{
			// TODO
			return Base::inverse();
		}
	}

	template<typename T>
	const scale<T, 3> transform<T>::decompose_scale() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
#ifdef UBPA_USE_XSIMD
		if constexpr (std::is_same_v<T, float>)
			return { m[0].norm(), m[1].norm(), m[2].norm() };
		else
#endif // UBPA_USE_XSIMD
		{
			vec<T, 3> col0(m(0, 0), m(1, 0), m(2, 0));
			vec<T, 3> col1(m(0, 1), m(1, 1), m(2, 1));
			vec<T, 3> col2(m(0, 2), m(1, 2), m(2, 2));
			return { col0.norm(), col1.norm(), col2.norm() };
		}
	}

	template<typename T>
	const mat<T, 3> transform<T>::decompose_rotation_matrix() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
#ifdef UBPA_USE_XSIMD
		if constexpr (std::is_same_v<T, float>) {
			return { m[0].normalize().cast_to<vecf3>(),
				m[1].normalize().cast_to<vecf3>(),
				m[2].normalize().cast_to<vecf3>(),
			};
		}
#endif
		{
			mat<T, 3> m3{ std::array<T, 3 * 3>{
				m(0, 0), m(0, 1), m(0, 2),
				m(1, 0), m(1, 1), m(1, 2),
				m(2, 0), m(2, 1), m(2, 2)
			} };

			m3[0].normalize_self();
			m3[1].normalize_self();
			m3[2].normalize_self();

			return m3;
		}
	}

	template<typename T>
	const quat<T> transform<T>::decompose_quatenion() const noexcept {
		// ref: https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

		auto rM = decompose_rotation_matrix();

		T real;
		vec<T, 3> imag;

		T tr = rM.trace();

		if (tr > 0) {
			T S = std::sqrt(tr + 1) * 2;// S = 4 * real 
			real = static_cast<T>(0.25)* S;
			imag[0] = (rM(2, 1) - rM(1, 2)) / S;
			imag[1] = (rM(0, 2) - rM(2, 0)) / S;
			imag[2] = (rM(1, 0) - rM(0, 1)) / S;
		}
		else if ((rM(0, 0) > rM(1, 1))& (rM(0, 0) > rM(2, 2))) {
			T S = std::sqrt(1 + rM(0, 0) - rM(1, 1) - rM(2, 2)) * 2;// S=4*imag[0]
			real = (rM(2, 1) - rM(1, 2)) / S;
			imag[0] = static_cast<T>(0.25)* S;
			imag[1] = (rM(0, 1) + rM(1, 0)) / S;
			imag[2] = (rM(0, 2) + rM(2, 0)) / S;
		}
		else if (rM(1, 1) > rM(2, 2)) {
			T S = sqrt(1 + rM(1, 1) - rM(0, 0) - rM(2, 2)) * 2;// S=4*imag[1]
			real = (rM(0, 2) - rM(2, 0)) / S;
			imag[0] = (rM(0, 1) + rM(1, 0)) / S;
			imag[1] = static_cast<T>(0.25)* S;
			imag[2] = (rM(1, 2) + rM(2, 1)) / S;
		}
		else {
			T S = sqrt(1 + rM(2, 2) - rM(0, 0) - rM(1, 1)) * 2;// S=4*imag[2]
			real = (rM(1, 0) - rM(0, 1)) / S;
			imag[0] = (rM(0, 2) + rM(2, 0)) / S;
			imag[1] = (rM(1, 2) + rM(2, 1)) / S;
			imag[2] = static_cast<T>(0.25)* S;
		}

		return quat<T>::imag_real(imag, real);
	}

	template<typename T>
	const euler<T> transform<T>::decompose_euler() const noexcept {
		auto rM = decompose_rotation_matrix();
		/*
		* rM is
		* 
		*  cYcZ + sXsYsZ  -cYsZ + sXsYsZ  cXsY
		*           cXsZ            cXcZ   -sX
		* -sYcZ + sXcYsZ   sYsZ + sXcYcZ  cXcY
		*/

		if (std::abs(rM(1, 2)) == 1) { // |sin X| = 1
			/*
			* if sinX ==  1, rM is
			*
			*  cos(Y-Z)  sin(Y-Z)        0
			*         0         0       -1
			* -sin(Y-Z)  cos(Y-Z)        0
			*
			* if sinX == -1, rM is
			*
			*  cos(Y+Z)  sin(Y+Z)        0
			*         0         0       -1
			* -sin(Y+Z)  cos(Y+Z)        0
			*
			* degenerated, so set Z = 0, then in both cases, rM is
			*
			*  cos Y  sin Y      0
			*      0      0     -1
			* -sin Y  cos Y      0
			*/
			return { sgn(rM(1, 2)) * to_radian<T>(90), std::atan2(rM(0, 1), rM(2, 1)), 0 };
		}
		else {
			T x = std::asin(-rM(1, 2)); // rM(1,2) == -sinX
			T y = std::atan2(rM(0, 2), rM(2, 2)); // rM(0,2) == cXsY, rM(2,2) == cXcY
			T z = std::atan2(rM(1, 0), rM(1, 1)); // rM(1,0) == cXsZ, rM(1,1) == cXcZ
			return { x, y, z };
		}
	}

	template<typename T>
	const mat<T, 3> transform<T>::decompose_mat3() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
		return std::array<T, 3 * 3>{
			m(0,0), m(0,1), m(0,2),
			m(1,0), m(1,1), m(1,2),
			m(2,0), m(2,1), m(2,2)
		};
	}

	namespace detail {
		template<Axis axis>
		struct rotate_with;

		template<>
		struct rotate_with<Axis::X> {
			template<typename T>
			static const transform<T> run(T angle) noexcept {
				T c = std::cos(angle);
				T s = std::sin(angle);
				
				return std::array<T, 4 * 4>{
					1, 0,  0, 0,
					0, c, -s, 0,
					0, s,  c, 0,
					0, 0,  0, 1
				};
			}
		};

		template<>
		struct rotate_with<Axis::Y> {
			template<typename T>
			static const transform<T> run(T angle) noexcept {
				T c = std::cos(angle);
				T s = std::sin(angle);

				return std::array<T, 4 * 4>{
					 c, 0, s, 0,
					 0, 1, 0, 0,
					-s, 0, c, 0,
					 0, 0, 0, 1
				};
			}
		};

		template<>
		struct rotate_with<Axis::Z> {
			template<typename T>
			static const transform<T> run(T angle) noexcept {
				T c = std::cos(angle);
				T s = std::sin(angle);

				return std::array<T, 4 * 4>{
					c, -s, 0, 0,
					s,  c, 0, 0,
					0,  0, 1, 0,
					0,  0, 0, 1
				};
			}
		};
	}

	template<typename T>
	template<Axis axis>
	static const transform<T> transform<T>::rotate_with(T angle) noexcept {
		return detail::rotate_with<axis>::run(angle);
	}

	template<typename T>
	const hvec<T, 4> transform<T>::operator*(const hvec<T, 4>& hv) const noexcept {
		return (*this) * hv.as<vec<T, 4>>();
	}

	template<typename T>
	const point<T, 3> transform<T>::operator*(const point<T, 3>& p) const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		T x = p[0];
		T y = p[1];
		T z = p[2];

#ifdef UBPA_USE_XSIMD
		if constexpr (std::is_same_v<T, float>) {
			auto mp = m[0] * x + m[1] * y + m[2] * z + m[4];
			return (mp / mp[3]).cast_to<pointf3>();
		}
		else
#endif // UBPA_USE_XSIMD
		{
			T xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z + m(0, 3);
			T yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z + m(1, 3);
			T zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z + m(2, 3);
			T wp = m(3, 0) * x + m(3, 1) * y + m(3, 2) * z + m(3, 3);

			if (wp != 1) {
				T invWP = ONE<T> / wp;
				return { xp * invWP,yp * invWP,zp * invWP };
			}
			else
				return { xp, yp, zp };
		}
	}

	template<typename T>
	const vec<T, 3> transform<T>::operator*(const vec<T, 3>& v) const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		T x = v[0];
		T y = v[1];
		T z = v[2];

#ifdef UBPA_USE_XSIMD
		if constexpr (std::is_same_v<T, float>)
			return (m[0] * x + m[1] * y + m[2] * z).cast_to<vecf3>();
		else
#endif // UBPA_USE_XSIMD
		{
			T xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z;
			T yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z;
			T zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z;

			return { xp,yp,zp };
		}
	}

	template<typename T>
	const normal<T> transform<T>::operator*(const normal<T>& n) const noexcept {
		// TODO: transform::operator*(normal) SIMD

		// N = (M^{-1})^T * n

		mat<T, 3> m3 = decompose_mat3().inverse();

		T x = n[0];
		T y = n[1];
		T z = n[2];
		
		T xp = m3(0, 0) * x + m3(1, 0) * y + m3(2, 0) * z;
		T yp = m3(0, 1) * x + m3(1, 1) * y + m3(2, 1) * z;
		T zp = m3(0, 2) * x + m3(1, 2) * y + m3(2, 2) * z;

		return { xp,yp,zp };
	}

	template<typename T>
	const bbox<T, 3> transform<T>::operator*(const bbox<T, 3>& A)const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		// See Christer Ericson's Real-time Collision Detection, p. 87, or
		// James Arvo's "Transforming Axis-aligned Bounding Boxes" in Graphics Gems 1, pp. 548-550.
		// http://www.graphicsgems.org/
#ifdef UBPA_USE_XSIMD
		if constexpr (std::is_same_v<T, float>) {
			using V = xsimd::batch<float, 4>;
			//V Amin{ A.minP()[0], A.minP()[1], A.minP()[2], 0 };
			//V Amax{ A.maxP()[0], A.maxP()[1], A.maxP()[2], 0 };
			V Bmin = m[3].get_batch();
			V Bmax = m[3].get_batch();

			V m0Amin = m[0].get_batch() * A.minP()[0];
			V m0Amax = m[0].get_batch() * A.maxP()[0];
			V m1Amin = m[1].get_batch() * A.minP()[1];
			V m1Amax = m[1].get_batch() * A.maxP()[1];
			V m2Amin = m[2].get_batch() * A.minP()[2];
			V m2Amax = m[2].get_batch() * A.maxP()[2];

			Bmin += xsimd::min(m0Amin, m0Amax);
			Bmin += xsimd::min(m1Amin, m1Amax);
			Bmin += xsimd::min(m2Amin, m2Amax);
			Bmax += xsimd::max(m0Amin, m0Amax);
			Bmax += xsimd::max(m1Amin, m1Amax);
			Bmax += xsimd::max(m2Amin, m2Amax);
			
			return { pointf3{Bmin[0], Bmin[1], Bmin[2]}, pointf3{Bmax[0], Bmax[1], Bmax[2]} };
		}
		else
#endif // UBPA_USE_XSIMD
		{
			point<T, 3> Amin = A.minP();
			point<T, 3> Amax = A.maxP();
			point<T, 3> Bmin = { m(0, 3), m(1, 3), m(2, 3) };
			point<T, 3> Bmax = Bmin;

			/* Now find the extreme points by considering the product of the */
			/* min and max with each component of M.  */

			for (size_t i = 0; i < 3; i++) {
				for (size_t j = 0; j < 3; j++)
				{
					T a = m(i, j) * Amin[j];
					T b = m(i, j) * Amax[j];
					if (a < b) {
						Bmin[i] += a;
						Bmax[i] += b;
					}
					else {
						Bmin[i] += b;
						Bmax[i] += a;
					}
				}
			}

			/* Copy the result into the new box. */

			return { Bmin, Bmax };
		}
	}

	template<typename T>
	const line<T, 3> transform<T>::operator*(const line<T, 3>& l) const noexcept {
		return{ (*this) * l.point, (*this) * l.dir };
	}

	template<typename T>
	const ray<T, 3> transform<T>::operator*(const ray<T, 3>& r) const noexcept {
		return{ (*this) * r.point, (*this) * r.dir, r.tmin, r.tmax };
	}
}