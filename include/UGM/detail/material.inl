#include "..\material.h"
#pragma once

namespace Ubpa {
	float fresnel_schlick(float cos_theta, float eta) noexcept {
		float x = (eta - 1) / (eta + 1);
		float R0 = x * x; // reflectance at normal incidence, 'R' means specular [r]eflection coefficient
		float one_cos_theta = 1 - cos_theta;
		return R0 + (1 - R0) * pow5(one_cos_theta);
	}

	template<size_t R0_fraction>
	float fresnel_schlick_R0(float cos_theta) noexcept {
		constexpr float R0 = R0_fraction / 100.f;
		constexpr float one_R0 = 1 - R0;
		float one_cos_theta = 1 - cos_theta;
		return R0 + one_R0 * pow5(one_cos_theta);
	}

	float fresnel_schlick(float cos_theta, float metalness, const rgbf& metal_color, float reflectance) noexcept {
		auto R0 = rgbf::lerp(rgbf{ reflectance }, metal_color, metalness);
		float one_cos_theta = 1 - cos_theta;
		return R0 + (rgbf{ 1.f } -R0) * pow5(one_cos_theta);
	}

	float specular_reflection(float Fr, float cos_theta) noexcept {
		return Fr / cos_theta;
	}

	const rgbf specular_reflection(const rgbf& Fr, float cos_theta) noexcept {
		return Fr / cos_theta;
	}

	float specular_refraction(float Fr, float etai, float etao, float cos_thetai) noexcept {
		float etao_etai = etao / etai;
		return etao_etai * etao_etai * (1 - Fr) / cos_thetai;
	}

	float specular_refraction(float Fr, float eta, const svecf& wi) noexcept {
		if (wi.is_above()) // from surface to air
			return specular_refraction(Fr, 1, eta, wi.cos_stheta());
		else // from air to surface
			return specular_refraction(Fr, eta, 1, -wi.cos_stheta());
	}

	inline float GGX_Lambda(float alpha, const svecf& w, const svecf& wm) noexcept {
		float cos_stheta = w.cos_stheta();
		if (cos_stheta * w.dot(wm) <= 0)
			return 0.f;
		float tan2_stheta = 1 / pow2(cos_stheta) - 1;
		return 0.5f * (-1 + std::sqrt(1 + pow2(alpha) * tan2_stheta));
	}

	inline float GGX_G(float alpha, const svecf& wi, const svecf& wo, const svecf& wm) {
		// return 1 / (1 + GGX_Lambda(alpha, wi, wm) + GGX_Lambda(alpha, wo, wm));
		float alpha2 = pow2(alpha);

		float cos_sthetai = wi.cos_stheta();
		float cos_sthetao = wo.cos_stheta();

		float tan2_sthetai = 1 / pow2(cos_sthetai) - 1;
		float tan2_sthetao = 1 / pow2(cos_sthetao) - 1;

		return 1 / (std::sqrt(1 + alpha2 * tan2_sthetai) + std::sqrt(1 + alpha2 * tan2_sthetao));
	}

	float GGX_D(float alpha, const svecf& wm) noexcept {
		float cos_stheta = wm.cos_stheta();
		if (cos_stheta <= 0)
			return 0.f;
		float alpha2 = alpha * alpha;
		float denominator = PI<float> * pow2(1 + (alpha2 - 1) * pow2(cos_stheta));
		return alpha2 / denominator;
	}

	/*float microfacet_specualr_reflection_dwh_dwi(const svecf& wh, const svecf& wi) noexcept {
		return 1 / (4 * std::abs(wh.dot(wi)));
	}

	float microfacet_specular_refraction_dwh_dwi(const svecf& wh, const svecf& wi, const svecf& wo, float etai, float etao) noexcept {
		float wo_dot_wh = wo.dot(wh);
		float wi_dot_wh = wi.dot(wh);
		float numerator = etai * etai * std::abs(wo_dot_wh);
		float sqrt_denominator = etao * wo_dot_wh + etai * wi_dot_wh;
		float denominator = sqrt_denominator * sqrt_denominator;
		return numerator / denominator;
	}*/
}
