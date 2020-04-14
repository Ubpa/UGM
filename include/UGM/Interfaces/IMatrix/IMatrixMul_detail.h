#pragma once

namespace Ubpa::detail::IMatrixMul {
    template<size_t N>
    struct inverse;

    template<>
    struct inverse<3> {
        template<typename M>
        static const M run(const M& m) noexcept {
            static_assert(M::N == 3);
            using F = typename M::F;

            F determinant = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
                + m[1][0] * (m[2][1] * m[0][2] - m[0][1] * m[2][2])
                + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);

            assert(determinant != 0);

            F inverseDeterminant = 1 / determinant;

            M rst{};

            rst[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * inverseDeterminant;
            rst[1][0] = (m[2][0] * m[1][2] - m[1][0] * m[2][2]) * inverseDeterminant;
            rst[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * inverseDeterminant;
            rst[0][1] = (m[2][1] * m[0][2] - m[0][1] * m[2][2]) * inverseDeterminant;
            rst[1][1] = (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * inverseDeterminant;
            rst[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * inverseDeterminant;
            rst[0][2] = (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * inverseDeterminant;
            rst[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * inverseDeterminant;
            rst[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * inverseDeterminant;

            return rst;
        }
    };

    template<>
    struct inverse<4> {
        // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
        template<typename M>
        static const M run(const M& m) noexcept {
            static_assert(M::N == 4);
            using F = typename M::F;

#ifdef USE_XSIMD
            if constexpr (std::is_same_v<F, float>) {
                // ref
                // 1. https://software.intel.com/en-us/articles/optimized-matrix-library-for-use-with-the-intel-pentiumr-4-processors-sse2-instructions/
                // 2. https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html
                using V = xsimd::batch<float, 4>;
                // The inverse is calculated using "Divide and Conquer" technique. The 
                // original matrix is divide into four 2x2 sub-matrices. Since each 
                // register holds four matrix element, the smaller matrices are 
                // represented as a registers. Hence we get a better locality of the 
                // calculations.

                V A = _mm_movelh_ps(m[0], m[1]),    // the four sub-matrices 
                    B = _mm_movehl_ps(m[1], m[0]),
                    C = _mm_movelh_ps(m[2], m[3]),
                    D = _mm_movehl_ps(m[3], m[2]);
                V iA, iB, iC, iD,					// partial inverse of the sub-matrices
                    DC, AB;
                V dA, dB, dC, dD;                 // determinant of the sub-matrices
                V det, d, d1, d2;
                V rd;

                //  AB = A# * B
                AB = _mm_mul_ps(_mm_shuffle_ps(A, A, 0x0F), B);
                AB -= (V)_mm_mul_ps(_mm_shuffle_ps(A, A, 0xA5), _mm_shuffle_ps(B, B, 0x4E));
                //  DC = D# * C
                DC = _mm_mul_ps(_mm_shuffle_ps(D, D, 0x0F), C);
                DC -= (V)_mm_mul_ps(_mm_shuffle_ps(D, D, 0xA5), _mm_shuffle_ps(C, C, 0x4E));

                //  dA = |A|
                dA = _mm_mul_ps(_mm_shuffle_ps(A, A, 0x5F), A);
                dA = _mm_sub_ss(dA, _mm_movehl_ps(dA, dA));
                //  dB = |B|
                dB = _mm_mul_ps(_mm_shuffle_ps(B, B, 0x5F), B);
                dB = _mm_sub_ss(dB, _mm_movehl_ps(dB, dB));

                //  dC = |C|
                dC = _mm_mul_ps(_mm_shuffle_ps(C, C, 0x5F), C);
                dC = _mm_sub_ss(dC, _mm_movehl_ps(dC, dC));
                //  dD = |D|
                dD = _mm_mul_ps(_mm_shuffle_ps(D, D, 0x5F), D);
                dD = _mm_sub_ss(dD, _mm_movehl_ps(dD, dD));

                //  d = trace(AB*DC) = trace(A#*B*D#*C)
                d = _mm_mul_ps(_mm_shuffle_ps(DC, DC, 0xD8), AB);

                //  iD = C*A#*B
                iD = _mm_mul_ps(_mm_shuffle_ps(C, C, 0xA0), _mm_movelh_ps(AB, AB));
                iD += (V)_mm_mul_ps(_mm_shuffle_ps(C, C, 0xF5), _mm_movehl_ps(AB, AB));
                //  iA = B*D#*C
                iA = _mm_mul_ps(_mm_shuffle_ps(B, B, 0xA0), _mm_movelh_ps(DC, DC));
                iA += (V)_mm_mul_ps(_mm_shuffle_ps(B, B, 0xF5), _mm_movehl_ps(DC, DC));

                //  d = trace(AB*DC) = trace(A#*B*D#*C) [continue]
                d = _mm_add_ps(d, _mm_movehl_ps(d, d));
                d = _mm_add_ss(d, _mm_shuffle_ps(d, d, 1));
                d1 = dA * dD;
                d2 = dB * dC;

                //  iD = D*|A| - C*A#*B
                iD = D * (V)_mm_shuffle_ps(dA, dA, 0) - iD;

                //  iA = A*|D| - B*D#*C;
                iA = A * (V)_mm_shuffle_ps(dD, dD, 0) - iA;

                //  det = |A|*|D| + |B|*|C| - trace(A#*B*D#*C)
                det = d1 + d2 - d;
                rd = (V)(float(1.0f) / det);
#ifdef ZERO_SINGULAR
                rd = _mm_and_ps(_mm_cmpneq_ss(det, _mm_setzero_ps()), rd);
#endif

                //  iB = D * (A#B)# = D*B#*A
                iB = _mm_mul_ps(D, _mm_shuffle_ps(AB, AB, 0x33));
                iB -= (V)_mm_mul_ps(_mm_shuffle_ps(D, D, 0xB1), _mm_shuffle_ps(AB, AB, 0x66));
                //  iC = A * (D#C)# = A*C#*D
                iC = _mm_mul_ps(A, _mm_shuffle_ps(DC, DC, 0x33));
                iC -= (V)_mm_mul_ps(_mm_shuffle_ps(A, A, 0xB1), _mm_shuffle_ps(DC, DC, 0x66));

                rd = _mm_shuffle_ps(rd, rd, 0);
                alignas(16) const uint32_t _Sign_PNNP[4] = { 0x00000000, 0x80000000, 0x80000000, 0x00000000 };
                rd ^= (*(V*)&_Sign_PNNP);

                //  iB = C*|B| - D*B#*A
                iB = C * (V)_mm_shuffle_ps(dB, dB, 0) - iB;

                //  iC = B*|C| - A*C#*D;
                iC = B * (V)_mm_shuffle_ps(dC, dC, 0) - iC;

                //  iX = iX / det
                iA *= rd;
                iB *= rd;
                iC *= rd;
                iD *= rd;

                V rst0 = _mm_shuffle_ps(iA, iB, 0x77);
                V rst1 = _mm_shuffle_ps(iA, iB, 0x22);
                V rst2 = _mm_shuffle_ps(iC, iD, 0x77);
                V rst3 = _mm_shuffle_ps(iC, iD, 0x22);
                return { rst0,rst1,rst2,rst3 };
            }
#endif // USE_XSIMD
            {
                M rst{};

                rst[0][0] = m[1][1] * m[2][2] * m[3][3] -
                    m[1][1] * m[2][3] * m[3][2] -
                    m[2][1] * m[1][2] * m[3][3] +
                    m[2][1] * m[1][3] * m[3][2] +
                    m[3][1] * m[1][2] * m[2][3] -
                    m[3][1] * m[1][3] * m[2][2];

                rst[1][0] = -m[1][0] * m[2][2] * m[3][3] +
                    m[1][0] * m[2][3] * m[3][2] +
                    m[2][0] * m[1][2] * m[3][3] -
                    m[2][0] * m[1][3] * m[3][2] -
                    m[3][0] * m[1][2] * m[2][3] +
                    m[3][0] * m[1][3] * m[2][2];

                rst[2][0] = m[1][0] * m[2][1] * m[3][3] -
                    m[1][0] * m[2][3] * m[3][1] -
                    m[2][0] * m[1][1] * m[3][3] +
                    m[2][0] * m[1][3] * m[3][1] +
                    m[3][0] * m[1][1] * m[2][3] -
                    m[3][0] * m[1][3] * m[2][1];

                rst[3][0] = -m[1][0] * m[2][1] * m[3][2] +
                    m[1][0] * m[2][2] * m[3][1] +
                    m[2][0] * m[1][1] * m[3][2] -
                    m[2][0] * m[1][2] * m[3][1] -
                    m[3][0] * m[1][1] * m[2][2] +
                    m[3][0] * m[1][2] * m[2][1];

                rst[0][1] = -m[0][1] * m[2][2] * m[3][3] +
                    m[0][1] * m[2][3] * m[3][2] +
                    m[2][1] * m[0][2] * m[3][3] -
                    m[2][1] * m[0][3] * m[3][2] -
                    m[3][1] * m[0][2] * m[2][3] +
                    m[3][1] * m[0][3] * m[2][2];

                rst[1][1] = m[0][0] * m[2][2] * m[3][3] -
                    m[0][0] * m[2][3] * m[3][2] -
                    m[2][0] * m[0][2] * m[3][3] +
                    m[2][0] * m[0][3] * m[3][2] +
                    m[3][0] * m[0][2] * m[2][3] -
                    m[3][0] * m[0][3] * m[2][2];

                rst[2][1] = -m[0][0] * m[2][1] * m[3][3] +
                    m[0][0] * m[2][3] * m[3][1] +
                    m[2][0] * m[0][1] * m[3][3] -
                    m[2][0] * m[0][3] * m[3][1] -
                    m[3][0] * m[0][1] * m[2][3] +
                    m[3][0] * m[0][3] * m[2][1];

                rst[3][1] = m[0][0] * m[2][1] * m[3][2] -
                    m[0][0] * m[2][2] * m[3][1] -
                    m[2][0] * m[0][1] * m[3][2] +
                    m[2][0] * m[0][2] * m[3][1] +
                    m[3][0] * m[0][1] * m[2][2] -
                    m[3][0] * m[0][2] * m[2][1];

                rst[0][2] = m[0][1] * m[1][2] * m[3][3] -
                    m[0][1] * m[1][3] * m[3][2] -
                    m[1][1] * m[0][2] * m[3][3] +
                    m[1][1] * m[0][3] * m[3][2] +
                    m[3][1] * m[0][2] * m[1][3] -
                    m[3][1] * m[0][3] * m[1][2];

                rst[1][2] = -m[0][0] * m[1][2] * m[3][3] +
                    m[0][0] * m[1][3] * m[3][2] +
                    m[1][0] * m[0][2] * m[3][3] -
                    m[1][0] * m[0][3] * m[3][2] -
                    m[3][0] * m[0][2] * m[1][3] +
                    m[3][0] * m[0][3] * m[1][2];

                rst[2][2] = m[0][0] * m[1][1] * m[3][3] -
                    m[0][0] * m[1][3] * m[3][1] -
                    m[1][0] * m[0][1] * m[3][3] +
                    m[1][0] * m[0][3] * m[3][1] +
                    m[3][0] * m[0][1] * m[1][3] -
                    m[3][0] * m[0][3] * m[1][1];

                rst[3][2] = -m[0][0] * m[1][1] * m[3][2] +
                    m[0][0] * m[1][2] * m[3][1] +
                    m[1][0] * m[0][1] * m[3][2] -
                    m[1][0] * m[0][2] * m[3][1] -
                    m[3][0] * m[0][1] * m[1][2] +
                    m[3][0] * m[0][2] * m[1][1];

                rst[0][3] = -m[0][1] * m[1][2] * m[2][3] +
                    m[0][1] * m[1][3] * m[2][2] +
                    m[1][1] * m[0][2] * m[2][3] -
                    m[1][1] * m[0][3] * m[2][2] -
                    m[2][1] * m[0][2] * m[1][3] +
                    m[2][1] * m[0][3] * m[1][2];

                rst[1][3] = m[0][0] * m[1][2] * m[2][3] -
                    m[0][0] * m[1][3] * m[2][2] -
                    m[1][0] * m[0][2] * m[2][3] +
                    m[1][0] * m[0][3] * m[2][2] +
                    m[2][0] * m[0][2] * m[1][3] -
                    m[2][0] * m[0][3] * m[1][2];

                rst[2][3] = -m[0][0] * m[1][1] * m[2][3] +
                    m[0][0] * m[1][3] * m[2][1] +
                    m[1][0] * m[0][1] * m[2][3] -
                    m[1][0] * m[0][3] * m[2][1] -
                    m[2][0] * m[0][1] * m[1][3] +
                    m[2][0] * m[0][3] * m[1][1];

                rst[3][3] = m[0][0] * m[1][1] * m[2][2] -
                    m[0][0] * m[1][2] * m[2][1] -
                    m[1][0] * m[0][1] * m[2][2] +
                    m[1][0] * m[0][2] * m[2][1] +
                    m[2][0] * m[0][1] * m[1][2] -
                    m[2][0] * m[0][2] * m[1][1];

                F det = m[0][0] * rst[0][0] + m[0][1] * rst[1][0] + m[0][2] * rst[2][0] + m[0][3] * rst[3][0];

                assert(det != 0);

                F invDet = 1 / det;

                // rst *= invDet is not correct
                // because transform can't be multiplied by F
                rst[0] *= invDet;
                rst[1] *= invDet;
                rst[2] *= invDet;
                rst[3] *= invDet;

                return rst;
            }
        }

    };

    // ----------------------

    template<size_t N>
    struct mul;

    template<>
    struct mul<3> {
        template<typename M>
        static const M run(const M& x, const M& y) noexcept {
            static_assert(M::N == 3);
            using F = typename M::F;

            // must unloop by hand, complier may not auto unloop
            F f00 = x(0, 0) * y(0, 0) + x(0, 1) * y(1, 0) + x(0, 2) * y(2, 0);
            F f01 = x(0, 0) * y(0, 1) + x(0, 1) * y(1, 1) + x(0, 2) * y(2, 1);
            F f02 = x(0, 0) * y(0, 2) + x(0, 1) * y(1, 2) + x(0, 2) * y(2, 2);
            F f10 = x(1, 0) * y(0, 0) + x(1, 1) * y(1, 0) + x(1, 2) * y(2, 0);
            F f11 = x(1, 0) * y(0, 1) + x(1, 1) * y(1, 1) + x(1, 2) * y(2, 1);
            F f12 = x(1, 0) * y(0, 2) + x(1, 1) * y(1, 2) + x(1, 2) * y(2, 2);
            F f20 = x(2, 0) * y(0, 0) + x(2, 1) * y(1, 0) + x(2, 2) * y(2, 0);
            F f21 = x(2, 0) * y(0, 1) + x(2, 1) * y(1, 1) + x(2, 2) * y(2, 1);
            F f22 = x(2, 0) * y(0, 2) + x(2, 1) * y(1, 2) + x(2, 2) * y(2, 2);

            return std::array<F, 3 * 3>{
                f00, f01, f02,
                f10, f11, f12,
                f20, f21, f22
            };
        }

        template<typename M>
        static const typename M::Vector run(const M& m, const typename M::Vector& v) noexcept {
            static_assert(M::N == 3);
            using F = typename M::F;

            F x = v[0];
            F y = v[1];
            F z = v[2];

            F xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z;
            F yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z;
            F zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z;

            return { xp,yp,zp };
        }
    };

    template<>
    struct mul<4> {
        template<typename M>
        static const M run(const M& x, const M& y) noexcept {
            static_assert(M::N == 4);
            using F = typename M::F;
#ifdef USE_XSIMD
            if constexpr (std::is_same_v<F, float>)
                return { x * y[0],x * y[1],x * y[2],x * y[3] };
#endif // USE_XSIMD
            {
                // must unloop by hand, complier may not auto unloop
                F f00 = x(0, 0) * y(0, 0) + x(0, 1) * y(1, 0) + x(0, 2) * y(2, 0) + x(0, 3) * y(3, 0);
                F f01 = x(0, 0) * y(0, 1) + x(0, 1) * y(1, 1) + x(0, 2) * y(2, 1) + x(0, 3) * y(3, 1);
                F f02 = x(0, 0) * y(0, 2) + x(0, 1) * y(1, 2) + x(0, 2) * y(2, 2) + x(0, 3) * y(3, 2);
                F f03 = x(0, 0) * y(0, 3) + x(0, 1) * y(1, 3) + x(0, 2) * y(2, 3) + x(0, 3) * y(3, 3);
                F f10 = x(1, 0) * y(0, 0) + x(1, 1) * y(1, 0) + x(1, 2) * y(2, 0) + x(1, 3) * y(3, 0);
                F f11 = x(1, 0) * y(0, 1) + x(1, 1) * y(1, 1) + x(1, 2) * y(2, 1) + x(1, 3) * y(3, 1);
                F f12 = x(1, 0) * y(0, 2) + x(1, 1) * y(1, 2) + x(1, 2) * y(2, 2) + x(1, 3) * y(3, 2);
                F f13 = x(1, 0) * y(0, 3) + x(1, 1) * y(1, 3) + x(1, 2) * y(2, 3) + x(1, 3) * y(3, 3);
                F f20 = x(2, 0) * y(0, 0) + x(2, 1) * y(1, 0) + x(2, 2) * y(2, 0) + x(2, 3) * y(3, 0);
                F f21 = x(2, 0) * y(0, 1) + x(2, 1) * y(1, 1) + x(2, 2) * y(2, 1) + x(2, 3) * y(3, 1);
                F f22 = x(2, 0) * y(0, 2) + x(2, 1) * y(1, 2) + x(2, 2) * y(2, 2) + x(2, 3) * y(3, 2);
                F f23 = x(2, 0) * y(0, 3) + x(2, 1) * y(1, 3) + x(2, 2) * y(2, 3) + x(2, 3) * y(3, 3);
                F f30 = x(3, 0) * y(0, 0) + x(3, 1) * y(1, 0) + x(3, 2) * y(2, 0) + x(3, 3) * y(3, 0);
                F f31 = x(3, 0) * y(0, 1) + x(3, 1) * y(1, 1) + x(3, 2) * y(2, 1) + x(3, 3) * y(3, 1);
                F f32 = x(3, 0) * y(0, 2) + x(3, 1) * y(1, 2) + x(3, 2) * y(2, 2) + x(3, 3) * y(3, 2);
                F f33 = x(3, 0) * y(0, 3) + x(3, 1) * y(1, 3) + x(3, 2) * y(2, 3) + x(3, 3) * y(3, 3);

                return std::array<F, 4 * 4>{
                    f00, f01, f02, f03,
                        f10, f11, f12, f13,
                        f20, f21, f22, f23,
                        f30, f31, f32, f33
                };
            }
        }

        template<typename M>
        static const typename M::Vector run(const M& m, const typename M::Vector& v) noexcept {
            static_assert(M::N == 4);
            using F = typename M::F;

#ifdef USE_XSIMD
            if constexpr (std::is_same_v<F, float>)
                return m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3] * v[3];
            else
#endif // USE_XSIMD
            {
                F x = v[0];
                F y = v[1];
                F z = v[2];
                F w = v[3];

                F xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z + m(0, 3) * w;
                F yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z + m(1, 3) * w;
                F zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z + m(2, 3) * w;
                F wp = m(3, 0) * x + m(3, 1) * y + m(3, 2) * z + m(3, 3) * w;

                return { wp,yp,zp,wp };
            }
        }
    };
}