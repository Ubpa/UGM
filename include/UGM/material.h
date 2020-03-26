#pragma once

#include "svec.h"
#include "rgb.h"

namespace Ubpa {
	// [summary]
	// schlick's approximation fresnel for dielectric
	// [reference]
	// Schlick C. An inexpensive BRDF model for physically©\based rendering.
	// [argument]
	// - cos_theta: theta is on one side of the surface with low refractive index
	// - eta: refractive index of material (refractive index of air is 1 as default)
	// [return]
	// specular reflection coefficient
	inline float fresnel_schlick(float cos_theta, float eta) noexcept;

	// [summary]
	// compile-time R0, faster than `fresnel_schlick(float, float)`
	// R0 = pow((eta - 1)/(eta + 1))^2, use 0.04 as default
	// [argument]
	// - R0_fraction: [0, 100]
	// - cos_theta
	//   - theta is on one side of the surface with low refractive index
	template<size_t R0_fraction = 4>
	inline float fresnel_schlick_R0(float cos_theta) noexcept;

	// [summary]
	// schlick's approximation fresnel for metal flowwork
	// LearnOpenGL-PBR: https://learnopengl-cn.github.io/07%20PBR/01%20Theory/
	// [argument]
	// - cos_theta: theta is on one side of the surface with low refractive index
	// - reflectance: R0 of dielectric
	// - metalness: 0 - 1
	// - metal_color: maybe call 'albedo' in model files, the word 'albedo' is for lambertian reflection
	// [return]
	// specular reflection coefficient
	inline float fresnel_schlick(float cos_theta, float metalness, const rgbf& metal_color, float reflectance = 0.04) noexcept;

	// [summary]
	// brdf of specular reflection
	// [argument]
	// - Fr: fresnel
	// - cos_theta: theta between wo / wi with normal
	// [return]
	// brdf value without delta(wi == R)
	inline float specular_reflection(float Fr, float cos_theta) noexcept;

	// [summary]
	// brdf of specular reflection
	// [argument]
	// - Fr: fresnel
	// - cos_theta: theta between wo / wi with normal
	// [return]
	// brdf value without delta(wi == R)
	inline const rgbf specular_reflection(const rgbf& Fr, float cos_theta) noexcept;

	// [summary]
	// brdf of specular refraction
	// [argument]
	// - Fr: fresnel, float/rgbf
	// - etai : refractive index of the media on the incident side
	// - etao : refractive index of the media on the transmitted side
	// - cos_thetai: theta between wi with normal
	// [return]
	// brdf value without delta(wi == T)
	inline float specular_refraction(float Fr, float etai, float etao, float cos_thetai) noexcept;

	// [summary]
	// brdf of specular refraction
	// call `specular_refraction(float, float, float, float)` internally
	// use `wi.is_above()` to distinguish `etai` and `etao`
	// [argument]
	// - Fr: fresnel, float/rgbf
	// - eta : refractive index of material (refractive index of is 1 as default)
	// - wi : refracted direction (calculated by wo.refract(eta))
	// [return]
	// brdf value without delta(wi == T)
	inline float specular_refraction(float Fr, float eta, const svecf& wi) noexcept;

	// [summary]
	// ggx lambda
	// ref
	// - PBRT-08-ReflectionModels
	// - Eric Heitz. Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs.
	// [argument]
	// - alpha: function arguments, related with roughness
	// - w: wi/wo
	// - wm: microfacet normal
	inline float GGX_Lambda(float alpha, const svecf& w, const svecf& wm);

	// [summary]
	// ggx geometry function
	// ref
	// - PBRT-08-ReflectionModels
	// - Eric Heitz. Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs.
	// [argument]
	// - alpha: function arguments, related with roughness
	// - wi: incident direction
	// - wo: outgoing direction
	// - wm: microfacet normal
	inline float GGX_G(float alpha, const svecf& wi, const svecf& wo, const svecf& wm) noexcept;

	// [summary]
	// ggx normal distribution function
	// ref
	// - PBRT-08-ReflectionModels
	// - Walter B, et al. Microfacet Models for Refraction through Rough Surfaces.
	// [argument]
	// - alpha: function arguments, related with roughness
	// - wi: incident direction
	// - wo: outgoing direction
	// - wm: microfacet normal
	inline float GGX_D(float alpha, const svecf& wm) noexcept;

	//// [summary]
	//// ||dwh / dwi|| in case of microfacet specular reflection
	//// ref: Walter B, et al. Microfacet Models for Refraction through Rough Surfaces.
	//// [parameter]
	//// - wh : microfacet normal, outward from microfacet
	//// - wi : **reflected** direction
	//// [return]
	//// ||dwh / dwi||
	//inline float microfacet_specualr_reflection_dwh_dwi(const svecf& wh, const svecf& wi) noexcept;

	//// [summary]
	//// ||dwh / dwi|| in case of microfacet specular refraction
	//// ref: Walter B, et al. Microfacet Models for Refraction through Rough Surfaces.
	//// [parameter]
	//// - wh : microfacet normal, outward from microfacet
	//// - wi : **refracted** direction
	//// - etai : refractive index of the media on the incident side
	//// - etao : refractive index of the media on the transmitted side
	//// [return]
	//// ||dwh / dwi||
	//inline float microfacet_specular_refraction_dwh_dwi(const svecf& wh, const svecf& wi, const svecf& wo, float etai, float etao) noexcept;
}

#include "detail/material.inl"
