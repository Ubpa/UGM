#pragma once

namespace Ubpa {
	template<typename T>
	inline transform<T>::transform(const mat<T, 4>& m) noexcept
		: transform(m[0], m[1], m[2], m[3]) {}

	template<typename T>
	inline transform<T>::transform(const mat<T, 3>& m) noexcept
		: transform(
			vec<T, 4>{m[0][0], m[0][1], m[0][2], 0},
			vec<T, 4>{m[1][0], m[1][1], m[1][2], 0},
			vec<T, 4>{m[2][0], m[2][1], m[2][2], 0},
			vec<T, 4>{      0,       0,       0, 1}
		) { }

	template<typename T>
	transform<T>::transform(const vec<T, 3>& t) noexcept :
		transform{ std::array<T, 4 * 4>{
		    1, 0, 0, t[0],
			0, 1, 0, t[1],
			0, 0, 1, t[2],
			0, 0, 0, 1} } { }

	template<typename T>
	transform<T>::transform(const point<T, 3>& t) noexcept
		: transform(t.cast_to<vec<T, 3>>()) {}

	template<typename T>
	transform<T>::transform(const scale<T, 3>& s) noexcept :
		transform{ std::array<T, 4 * 4>{
			s[0],    0,    0,    0,
			   0, s[1],    0,    0,
			   0,    0, s[2],    0,
			   0,    0,    0,    1 } } { }
			
	template<typename T>
	transform<T>::transform(const vec<T, 3>& axis, T theta) noexcept {
		auto a = axis.normalize();

		T sinTheta = std::sin(theta);
		T cosTheta = std::cos(theta);
		T OneMinusCosTheta = static_cast<T>(1) - cosTheta;

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
				            0,                 0,                 0, 1
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
			                       0,                        0,       0, 1
		});
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
		//
		// cameraToWorld
		// [ R^T  T ]
		// [ 0    1 ]

		const vec<T,3> front = (target - pos).normalize();
		vec<T,3> right = front.cross(up);
		if (right.rmv_epsilon().is_all_zero()) {
			vec<T,3> newUp = up.normalize();
			newUp.min_component() = 1;
			right = front.cross(newUp).normalize();
		}
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
		return std::array<T, 4 * 4>{
			2 / width, 0, 0, 0,
			0, 2 / height, 0, 0,
			0, 0, -2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
			0, 0, 0, 1
		};
	}

	template<typename T>
	const transform<T> transform<T>::perspective(T fovY, T aspect, T zNear, T zFar) noexcept {
		assert(fovY > 0 && aspect > 0 && zNear >= 0 && zFar > zNear);
		T tanHalfFovY = std::tan(fovY / static_cast<T>(2));
		return std::array<T, 4 * 4>{
			1 / (aspect * tanHalfFovY), 0, 0, 0,
			0, 1 / (tanHalfFovY), 0, 0,
			0, 0, -(zFar + zNear) / (zFar - zNear), -(2 * zFar * zNear) / (zFar - zNear),
			0, 0, -1, 0
		};
	}

	template<typename T>
	const scale<T, 3> transform<T>::decompose_scale() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
		vec<T, 3> col0(m(0, 0), m(1, 0), m(2, 0));
		vec<T, 3> col1(m(0, 1), m(1, 1), m(2, 1));
		vec<T, 3> col2(m(0, 2), m(1, 2), m(2, 2));
		return { col0.norm(), col1.norm(), col2.norm() };
	}

	template<typename T>
	const mat<T, 3> transform<T>::decompose_rotation_matrix() const noexcept {
		const auto& m = static_cast<const transform&>(*this);
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
	const point<T, 3> transform<T>::operator*(const point<T, 3>& p) const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		T x = p[0];
		T y = p[1];
		T z = p[2];

		T xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z + m(0, 3);
		T yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z + m(1, 3);
		T zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z + m(2, 3);
		T wp = m(3, 0) * x + m(3, 1) * y + m(3, 2) * z + m(3, 3);

		if (wp != 1) {
			T invWP = static_cast<T>(1) / wp;
			return { xp * invWP,yp * invWP,zp * invWP };
		}
		else
			return { xp, yp, zp };
	}

	template<typename T>
	const vec<T, 3> transform<T>::operator*(const vec<T, 3>& v) const noexcept {
		const auto& m = static_cast<const transform&>(*this);

		T x = v[0];
		T y = v[1];
		T z = v[2];

		T xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z;
		T yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z;
		T zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z;

		return { xp,yp,zp };
	}

	template<typename T>
	const normal<T> transform<T>::operator*(const normal<T>& n) const noexcept {
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
	const bbox<T, 3> transform<T>::operator*(const bbox<T, 3>& b)const noexcept {
		return { (*this) * b.minP(), (*this) * b.maxP() };
	}

	template<typename T>
	const ray<T, 3> transform<T>::operator*(const ray<T, 3>& r) const noexcept {
		return{ (*this) * r.o,(*this) * r.d };
	}
}