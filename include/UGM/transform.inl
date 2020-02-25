#pragma once

namespace Ubpa {
	template<typename T>
	transform<T>::transform(const vec<T, 3>& t) noexcept :
		transform{ std::array<T, 4 * 4>{
		    1, 0, 0, t[0],
			0, 1, 0, t[1],
			0, 0, 1, t[2],
			0, 0, 0, 1} } { }

	template<typename T>
	transform<T>::transform(const scale<T, 3>& s) noexcept :
		transform{ std::array<T, 4 * 4>{
			s[0],    0,    0,    0,
			   0, s[1],    0,    0,
			   0,    0, s[2],    0,
			   0,    0,    0,    1 } } { }

	template<typename T>
	void transform<T>::init(const vec<T, 3>& a, float theta) noexcept {
		assert((a.normalize() - a).norm() < static_cast<T>(0.000001));

		float sinTheta = std::sin(theta);
		float cosTheta = std::cos(theta);
		float OneMinusCosTheta = static_cast<T>(1) - cosTheta;
		
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
		if (q.is_identity())
			init(std::array<T,16>{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 });
		else
			init(q.axis(), q.theta());
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
		if (right.rmv_epsilon().is_zero()) {
			vec<T,3> newUp = up.normalize();
			newUp.min_component() = 1;
			right = front.cross(newUp).normalize();
		}
		const vec<T,3> camUp = right.cross(front);

		transform m;

		m(0, 0) = right[0];
		m(0, 1) = right[1];
		m(0, 2) = right[2];
		m(0, 3) = 0;

		m(1, 0) = camUp[0];
		m(1, 1) = camUp[1];
		m(1, 2) = camUp[2];
		m(1, 3) = 0;

		m(2, 0) = -front[0];
		m(2, 1) = -front[1];
		m(2, 2) = -front[2];
		m(2, 3) = 0;

		vec<T, 3> posV(pos);
		m(3, 0) = -right.dot(posV);
		m(3, 1) = -camUp.dot(posV);
		m(3, 2) = front.dot(posV);
		m(3, 3) = 1;

		return m;
	}

	template<typename T>
	const transform<T> transform<T>::orthographic(float width, float height, float zNear, float zFar) noexcept {
		assert(width > 0 && height > 0 && zNear >= 0 && zFar > zNear);
		return std::array<T, 4 * 4>{
			2 / width, 0, 0, 0,
			0, 2 / height, 0, 0,
			0, 0, -2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
			0, 0, 0, 1
		};
	}

	template<typename T>
	const transform<T> transform<T>::perspcetive(float fovY, float aspect, float zNear, float zFar) noexcept {
		assert(fovY > 0 && aspect > 0 && zNear >= 0 && zFar > zNear);
		T tanHalfFovY = std::tan(fovY / static_cast<T>(2));
		return std::array<T, 4 * 4>{
			1 / (aspect * tanHalfFovY), 0, 0, 0,
			0, 1 / (tanHalfFovY), 0, 0,
			0, 0, -(zFar + zNear) / (zFar - zNear), -(2 * zFar * zNear) / (zFar - zNear),
			0, 0, -1, 0
		};
	}
}