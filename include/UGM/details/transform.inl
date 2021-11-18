#pragma once

namespace Ubpa {
	template<typename F>
	transform<F>::transform(const mat<F, 4>& m) noexcept
		: transform(m[0], m[1], m[2], m[3]) {}

	template<typename F>
	transform<F>::transform(const mat<F, 3>& m) noexcept :
		transform{
			m[0][0], m[1][0], m[2][0],    0,
			m[0][1], m[1][1], m[2][1],    0,
			m[0][2], m[1][2], m[2][2],    0,
			      0,       0,       0,    1 } {}

	template<typename F>
	transform<F>::transform(const vec<F, 3>& t) noexcept :
		transform{
			1, 0, 0, t[0],
			0, 1, 0, t[1],
			0, 0, 1, t[2],
			0, 0, 0,    1 } { }

	template<typename F>
	transform<F>::transform(const scale<F, 3>& s) noexcept :
		transform{
			s[0],    0,    0,    0,
			   0, s[1],    0,    0,
			   0,    0, s[2],    0,
			   0,    0,    0,    1 } { }

	template<typename F>
	transform<F>::transform(F s) noexcept
		: transform{ scale<F,3>{s} } {}
	
	template<typename F>
	transform<F>::transform(const quat<F>& q) noexcept {
		F x = q.imag()[0];
		F y = q.imag()[1];
		F z = q.imag()[2];
		F w = q.real();

		F xx = x * x;
		F xy = x * y;
		F xz = x * z;
		F xw = x * w;
		F yy = y * y;
		F yz = y * z;
		F yw = y * w;
		F zz = z * z;
		F zw = z * w;

		this->init(
			1 - 2 * (yy + zz),     2 * (xy - zw),     2 * (xz + yw), 0,
			    2 * (xy + zw), 1 - 2 * (zz + xx),     2 * (yz - xw), 0,
				2 * (xz - yw),     2 * (yz + xw), 1 - 2 * (xx + yy), 0,
				            0,                 0,                 0, 1
		);
	}

	template<typename F>
	transform<F>::transform(const euler<F>& e) noexcept {
		F x = to_radian(e[0]);
		F y = to_radian(e[1]);
		F z = to_radian(e[2]);
		F cX = std::cos(x);
		F sX = std::sin(x);
		F cY = std::cos(y);
		F sY = std::sin(y);
		F cZ = std::cos(z);
		F sZ = std::sin(z);

		this->init(
			  cY * cZ + sX * sY * sZ, - cY * sZ + sX * sY * cZ, cX * sY, 0,
			                 cX * sZ,                  cX * cZ,    - sX, 0,
			- sY * cZ + sX * cY * sZ,   sY * sZ + sX * cY * cZ, cX * cY, 0,
			                       0,                        0,       0, 1
		);
	}

	template<typename F>
	transform<F>::transform(const vec<F, 3>& axis, F theta) noexcept {
		auto a = axis.normalize();

		F sinTheta = std::sin(theta);
		F cosTheta = std::cos(theta);
		F OneMinusCosTheta = One<F> -cosTheta;

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

	template<typename F>
	transform<F>::transform(const vec<F, 3>& t, const scale<F, 3>& s) noexcept :
		transform{ std::array<F, 4 * 4>{
			s[0],    0,    0, t[0],
			   0, s[1],    0, t[1],
			   0,    0, s[2], t[2],
			   0,    0,    0,    1 } } { }

	template<typename F>
	transform<F>::transform(const vec<F, 3>& t, const quat<F>& q) noexcept {
		F x = q.imag()[0];
		F y = q.imag()[1];
		F z = q.imag()[2];
		F w = q.real();

		F xx = x * x;
		F xy = x * y;
		F xz = x * z;
		F xw = x * w;
		F yy = y * y;
		F yz = y * z;
		F yw = y * w;
		F zz = z * z;
		F zw = z * w;

		this->init(
			1 - 2 * (yy + zz),     2 * (xy - zw),     2 * (xz + yw), t[0],
			    2 * (xy + zw), 1 - 2 * (zz + xx),     2 * (yz - xw), t[1],
				2 * (xz - yw),     2 * (yz + xw), 1 - 2 * (xx + yy), t[2],
				            0,                 0,                 0,    1
		);
	}

	template<typename F>
	transform<F>::transform(const quat<F>& rot, const scale<F, 3>& scale) noexcept
		: transform{ vec<F, 3>{0,0,0}, rot, scale } {}

	template<typename F>
	transform<F>::transform(const vec<F, 3>& t, const quat<F>& q, const scale<F, 3>& s) noexcept {
		F x = q.imag()[0];
		F y = q.imag()[1];
		F z = q.imag()[2];
		F w = q.real();

		F xx = x * x;
		F xy = x * y;
		F xz = x * z;
		F xw = x * w;
		F yy = y * y;
		F yz = y * z;
		F yw = y * w;
		F zz = z * z;
		F zw = z * w;

#ifdef UBPA_UGM_USE_SIMD
		if constexpr (SI_ImplTraits_SupportSIMD<SI_ImplTraits_T<transform<F>>>) {
			this->init(
				1 - 2 * (yy + zz),     2 * (xy - zw),     2 * (xz + yw), t[0],
				    2 * (xy + zw), 1 - 2 * (zz + xx),     2 * (yz - xw), t[1],
				    2 * (xz - yw),     2 * (yz + xw), 1 - 2 * (xx + yy), t[2],
				                0,                 0,                 0,    1
			);
			(*this)[0] *= s[0];
			(*this)[1] *= s[1];
			(*this)[2] *= s[2];
		}
		else
#endif // UBPA_UGM_USE_SIMD
		this->init(
			s[0] * (1 - 2 * (yy + zz)), s[1] * (    2 * (xy - zw)), s[2] * (    2 * (xz + yw)), t[0],
			s[0] * (    2 * (xy + zw)), s[1] * (1 - 2 * (zz + xx)), s[2] * (    2 * (yz - xw)), t[1],
			s[0] * (    2 * (xz - yw)), s[1] * (    2 * (yz + xw)), s[2] * (1 - 2 * (xx + yy)), t[2],
			                         0,                          0,                          0,    1
		);
	}

	template<typename F>
	transform<F> transform<F>::look_at(const point<F, 3>& pos, const point<F, 3>& target, const vec<F, 3>& up) noexcept {
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
		// [ R -RF ]
		// [ 0   1 ]

		vec<F,3> front = (target - pos).normalize();
		vec<F, 3> right = front.cross(up).normalize();
		vec<F,3> camUp = right.cross(front);
		auto posV = pos.template cast_to<vec<F, 3>>();

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

	template<typename F>
	transform<F> transform<F>::orthographic(F width, F height, F zNear, F zFar) noexcept {
		assert(width > 0 && height > 0 && zNear >= 0 && zFar > zNear);

		F m00 = 2 / width;
		F m11 = 2 / height;
		F m22 = 2 / (zNear - zFar);
		F m23 = (zFar + zNear) / (zNear - zFar);

		return {
			m00,   0,   0,   0,
			  0, m11,   0,   0,
			  0,   0, m22, m23,
			  0,   0,   0,   1,
		};
	}

	template<typename F>
	transform<F> transform<F>::perspective(F fovY, F aspect, F zNear, F zFar, F near_clip_vlaue) noexcept {
		assert(fovY > 0 && aspect > 0 && zNear >= 0 && zFar > zNear);

		F tanHalfFovY = std::tan(fovY / static_cast<F>(2));
		F cotHalfFovY = 1 / tanHalfFovY;

		F m00 = cotHalfFovY / aspect;
		F m11 = cotHalfFovY;
		F m22 = (zFar - near_clip_vlaue * zNear) / (zNear - zFar);
		F m23 = ((1 - near_clip_vlaue) * zFar * zNear) / (zNear - zFar);

		return {
			m00,   0,   0,   0,
			  0, m11,   0,   0,
			  0,   0, m22, m23,
			  0,   0,  -1,   0
		};
	}

	template<typename F>
	transform<F> transform<F>::inverse_sim() const noexcept {
		// ref: https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html
#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<F, float>) {
			transform<F> r;
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
				_mm_cmplt_ps(sizeSqr, _mm_set1_ps(Epsilon<float>))
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

	template<typename F>
	vec<F, 3> transform<F>::decompose_translation() const noexcept {
		return { (*this)(0,3), (*this)(1,3), (*this)(2,3) };
	}

	template<typename F>
	mat<F, 3> transform<F>::decompose_rotation_matrix() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<F, float>) {
			return { m[0].normalize().template cast_to<vecf3>(),
				m[1].normalize().template cast_to<vecf3>(),
				m[2].normalize().template cast_to<vecf3>(),
			};
		}
#endif
		{
			mat<F, 3> m3{
				m(0, 0), m(0, 1), m(0, 2),
				m(1, 0), m(1, 1), m(1, 2),
				m(2, 0), m(2, 1), m(2, 2)
			};

			m3[0].normalize_self();
			m3[1].normalize_self();
			m3[2].normalize_self();

			return m3;
		}
	}

	template<typename F>
	quat<F> transform<F>::decompose_quatenion() const noexcept {
		// ref: https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

		auto rM = decompose_rotation_matrix();

		F real;
		vec<F, 3> imag;

		F tr = rM.trace();

		if (tr > 0) {
			F S = std::sqrt(tr + 1) * 2;// S = 4 * real 
			real = static_cast<F>(0.25) * S;
			imag[0] = (rM(2, 1) - rM(1, 2)) / S;
			imag[1] = (rM(0, 2) - rM(2, 0)) / S;
			imag[2] = (rM(1, 0) - rM(0, 1)) / S;
		}
		else if ((rM(0, 0) > rM(1, 1))& (rM(0, 0) > rM(2, 2))) {
			F S = std::sqrt(1 + rM(0, 0) - rM(1, 1) - rM(2, 2)) * 2;// S=4*imag[0]
			real = (rM(2, 1) - rM(1, 2)) / S;
			imag[0] = static_cast<F>(0.25)* S;
			imag[1] = (rM(0, 1) + rM(1, 0)) / S;
			imag[2] = (rM(0, 2) + rM(2, 0)) / S;
		}
		else if (rM(1, 1) > rM(2, 2)) {
			F S = sqrt(1 + rM(1, 1) - rM(0, 0) - rM(2, 2)) * 2;// S=4*imag[1]
			real = (rM(0, 2) - rM(2, 0)) / S;
			imag[0] = (rM(0, 1) + rM(1, 0)) / S;
			imag[1] = static_cast<F>(0.25)* S;
			imag[2] = (rM(1, 2) + rM(2, 1)) / S;
		}
		else {
			F S = sqrt(1 + rM(2, 2) - rM(0, 0) - rM(1, 1)) * 2;// S=4*imag[2]
			real = (rM(1, 0) - rM(0, 1)) / S;
			imag[0] = (rM(0, 2) + rM(2, 0)) / S;
			imag[1] = (rM(1, 2) + rM(2, 1)) / S;
			imag[2] = static_cast<F>(0.25)* S;
		}

		return quat<F>::imag_real(imag, real);
	}

	template<typename F>
	euler<F> transform<F>::decompose_euler() const noexcept {
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
			return {
				sgn(rM(1, 2)) * static_cast<F>(90),
				to_degree(std::atan2(rM(0, 1), rM(2, 1))),
				0
			};
		}
		else {
			F x = std::asin(-rM(1, 2)); // rM(1,2) == -sinX
			F y = std::atan2(rM(0, 2), rM(2, 2)); // rM(0,2) == cXsY, rM(2,2) == cXcY
			F z = std::atan2(rM(1, 0), rM(1, 1)); // rM(1,0) == cXsZ, rM(1,1) == cXcZ
			return {
				to_degree(x),
				to_degree(y),
				to_degree(z)
			};
		}
	}

	template<typename F>
	mat<F, 3> transform<F>::decompose_mat3() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
		return {
			m(0,0), m(0,1), m(0,2),
			m(1,0), m(1,1), m(1,2),
			m(2,0), m(2,1), m(2,2)
		};
	}

	template<typename F>
	scale<F, 3> transform<F>::decompose_scale() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<F, float>)
			return { m[0].norm(), m[1].norm(), m[2].norm() };
		else
#endif // UBPA_UGM_USE_SIMD
		{
			vec<F, 3> col0(m(0, 0), m(1, 0), m(2, 0));
			vec<F, 3> col1(m(0, 1), m(1, 1), m(2, 1));
			vec<F, 3> col2(m(0, 2), m(1, 2), m(2, 2));
			return { col0.norm(), col1.norm(), col2.norm() };
		}
	}

	namespace detail {
		template<Axis axis>
		struct rotate_with;

		template<>
		struct rotate_with<Axis::X> {
			template<typename F>
			static const transform<F> run(F angle) noexcept {
				F c = std::cos(angle);
				F s = std::sin(angle);
				
				return {
					1, 0,  0, 0,
					0, c, -s, 0,
					0, s,  c, 0,
					0, 0,  0, 1
				};
			}
		};

		template<>
		struct rotate_with<Axis::Y> {
			template<typename F>
			static const transform<F> run(F angle) noexcept {
				F c = std::cos(angle);
				F s = std::sin(angle);

				return {
					 c, 0, s, 0,
					 0, 1, 0, 0,
					-s, 0, c, 0,
					 0, 0, 0, 1
				};
			}
		};

		template<>
		struct rotate_with<Axis::Z> {
			template<typename F>
			static const transform<F> run(F angle) noexcept {
				F c = std::cos(angle);
				F s = std::sin(angle);

				return {
					c, -s, 0, 0,
					s,  c, 0, 0,
					0,  0, 1, 0,
					0,  0, 0, 1
				};
			}
		};
	}

	template<typename F>
	template<Axis axis>
	transform<F> transform<F>::rotate_with(F angle) noexcept {
		return detail::rotate_with<axis>::run(angle);
	}

	template<typename F>
	hvec<F, 4> transform<F>::operator*(const hvec<F, 4>& hv) const noexcept {
		return (*this) * hv.template as<vec<F, 4>>();
	}

	template<typename F>
	point<F, 3> transform<F>::operator*(const point<F, 3>& p) const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		F x = p[0];
		F y = p[1];
		F z = p[2];

#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<F, float>) {
			auto mp = m[0] * x + m[1] * y + m[2] * z + m[3];
			return (mp / mp[3]).template cast_to<pointf3>();
		}
		else
#endif // UBPA_UGM_USE_SIMD
		{
			F xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z + m(0, 3);
			F yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z + m(1, 3);
			F zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z + m(2, 3);
			F wp = m(3, 0) * x + m(3, 1) * y + m(3, 2) * z + m(3, 3);

			if (wp != 1) {
				F invWP = One<F> / wp;
				return { xp * invWP,yp * invWP,zp * invWP };
			}
			else
				return { xp, yp, zp };
		}
	}

	template<typename F>
	vec<F, 3> transform<F>::operator*(const vec<F, 3>& v) const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		F x = v[0];
		F y = v[1];
		F z = v[2];

#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<F, float>)
			return (m[0] * x + m[1] * y + m[2] * z).template cast_to<vecf3>();
		else
#endif // UBPA_UGM_USE_SIMD
		{
			F xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z;
			F yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z;
			F zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z;

			return { xp,yp,zp };
		}
	}

	template<typename F>
	normal<F> transform<F>::operator*(const normal<F>& n) const noexcept {
		// N = (M^{-1})^T * n

		mat<F, 3> m3 = decompose_mat3().inverse();

		F x = n[0];
		F y = n[1];
		F z = n[2];
		
		F xp = m3(0, 0) * x + m3(1, 0) * y + m3(2, 0) * z;
		F yp = m3(0, 1) * x + m3(1, 1) * y + m3(2, 1) * z;
		F zp = m3(0, 2) * x + m3(1, 2) * y + m3(2, 2) * z;

		return { xp,yp,zp };
	}

	template<typename F>
	bbox<F, 3> transform<F>::operator*(const bbox<F, 3>& A)const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		// See Christer Ericson's Real-time Collision Detection, p. 87, or
		// James Arvo's "Transforming Axis-aligned Bounding Boxes" in Graphics Gems 1, pp. 548-550.
		// http://www.graphicsgems.org/
#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<F, float>) {
			vecf<4> Bmin = m[3];
			vecf<4> Bmax = m[3];

			vecf<4> m0Amin = m[0] * A.minP()[0];
			vecf<4> m0Amax = m[0] * A.maxP()[0];
			vecf<4> m1Amin = m[1] * A.minP()[1];
			vecf<4> m1Amax = m[1] * A.maxP()[1];
			vecf<4> m2Amin = m[2] * A.minP()[2];
			vecf<4> m2Amax = m[2] * A.maxP()[2];

			Bmin += vecf<4>::min(m0Amin, m0Amax);
			Bmin += vecf<4>::min(m1Amin, m1Amax);
			Bmin += vecf<4>::min(m2Amin, m2Amax);
			Bmax += vecf<4>::max(m0Amin, m0Amax);
			Bmax += vecf<4>::max(m1Amin, m1Amax);
			Bmax += vecf<4>::max(m2Amin, m2Amax);
			
			return { pointf3{Bmin[0], Bmin[1], Bmin[2]}, pointf3{Bmax[0], Bmax[1], Bmax[2]} };
		}
		else
#endif // UBPA_UGM_USE_SIMD
		{
			point<F, 3> Amin = A.minP();
			point<F, 3> Amax = A.maxP();
			point<F, 3> Bmin = { m(0, 3), m(1, 3), m(2, 3) };
			point<F, 3> Bmax = Bmin;

			/* Now find the extreme points by considering the product of the */
			/* min and max with each component of M.  */

			for (size_t i = 0; i < 3; i++) {
				for (size_t j = 0; j < 3; j++)
				{
					F a = m(i, j) * Amin[j];
					F b = m(i, j) * Amax[j];
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

	template<typename F>
	line<F, 3> transform<F>::operator*(const line<F, 3>& l) const noexcept {
		return{ (*this) * l.point, (*this) * l.dir };
	}

	template<typename F>
	ray<F, 3> transform<F>::operator*(const ray<F, 3>& r) const noexcept {
		return{ (*this) * r.point, (*this) * r.dir, r.tmin, r.tmax };
	}
}