# UGM

> **U**bpa **G**raphics **M**athematics，Ubpa 图形数学库

⭐ Star us on GitHub — it helps!

[![repo-size](https://img.shields.io/github/languages/code-size/Ubpa/UGM?style=flat)](https://github.com/Ubpa/UGM/archive/master.zip) [![tag](https://img.shields.io/github/v/tag/Ubpa/UGM)](https://github.com/Ubpa/UGM/tags) [![license](https://img.shields.io/github/license/Ubpa/UGM)](LICENSE) 

**特点** 

- 着重“正确”的**代数**概念（环、线性、欧式空间、仿射空间等）
- 面向对象（所有方法都是类方法）
- 只有头文件 head-only
- 高性能：SIMD 加速，各算法最优化
- 利用[单继承化](https://zhuanlan.zhihu.com/p/106672814)优化代码结构（不使用恶心的宏）
- 提供 [natvis](https://docs.microsoft.com/en-us/visualstudio/debugger/create-custom-views-of-native-objects?view=vs-2019) 优化 debug 信息
- ...

## 1. 简介

UGM 是着重于**代数**概念的数学库，区分点、向量、法向、颜色等，从而尽可能地避免了错误的计算。

常用的图形数学库（如 Eigen，glm）只提供 `vec` 类，并使其能做各种运算（如 `+-*/` 等），但从代数方面考虑，这并不合理。

> **示例** 
>
> - 点与点之间不能相加
> - 颜色与点之间没有关系
> - 变换矩阵（4x4）与法向的乘积不同于一般向量
> - 在考虑齐次坐标时变换矩阵（4x4）与向量和点的乘积不同
> - ...

我们通过提供 `point`，`vec`，`normal`，`rgb` 等来区分不同的代数概念，并仅让他们支持合理的操作，这样就能在编译期就发现各种代数方面的错误，另外还能减轻心智负担（根据类型执行不同的操作）。

此外，我们还通过单继承的技术实现了极佳的代码编写，特点如下

- 复用函数实现（不同于 C++20 的 `concept` 或者接口，他们只是对类支持的“操作”进行了约束）
- 空基类优化
- ...

## 2. 示例

```c++
#include <UGM/UGM.h>

using namespace Ubpa;
using namespace std;

int main() {
    transformf tsfm{
        vecf3{1,1,1},                        // T
        quatf{vecf3{1,0,0}, to_radian(90.f), // R
        scalef3{2.f}}                        // S
    }; // T * R * S

    pointf3 p{ 1,2,3 };
    vecf3 v{ 1,1,1 };
    normalf n{ 0,1,0 };
    bboxf3 b{ p, p + v }; // min: 1 2 3, max: 2 3 4
    rayf3 r{ p, v }; // point: 1 2 3, dir: 1 1 1, tmin: EPSILON, tmax: FLT_MAX

    cout << tsfm * p << endl; // 3 -5 5
    cout << tsfm * v << endl; // 2 -2 2
    cout << tsfm * n << endl; // 0 0 0.5
    cout << tsfm * b << endl; // 3 -7 5, 5 -5 7
    cout << tsfm * r << endl; // 3 -5 5, 2 -2 2, EPSILON, FLT_MAX

    return 0;
}
```

## 3. 文档

- [安装使用说明](setup.md) 
- [API](API.md) 

## 4. 设计思路

为了更好地使用该数学库，我们很有必要先了解下该库的设计思路。

### 4.1 代数概念

该库着重于正确的代数概念，使用者很可能对这方面并不了解，但只要知道基础的线性代数知识即可。

下边我简单介绍下该库涉及的主要代数概念。

- 加法 [`IAdd`](include/UGM/Interfaces/IAdd.h)：相同元素之间的运算，具有交换性（`a+b==b+a`）和可逆性（`a+(-a)=0`）
- 乘法 [`IMul`](include/UGM/Interfaces/IMul.h)：相同元素之间的运算，具有可逆性（`a*(1/a)=1`），不一定具备交换性（`a*b==b*a`）。
- 数乘 [`IScalarMul`](include/UGM/Interfaces/IScalarMul.h)：类与标量（如 `float`）之间的运算，具有交换性。
- 线性 [`ILinear`](include/UGM/Interfaces/ILinear.h)：加法 + 数乘，该空间中的元素称为向量
- 环 [`IRing`](include/UGM/Interfaces/IRing.h)：加法 + 乘法
- 度量 [`IMetric`](include/UGM/Interfaces/IMetric.h)：也叫距离
- 范数 [`INorm`](include/UGM/Interfaces/INorm.h)：向量 => 标量的函数，一般也叫大小 / 长度，可**自然诱导**出度量（`distance(a,b) == (a-b).norm()`）
- 内积 [`IInnerProduct`](include/UGM/Interfaces/IInnerProduct.h)：可**自然诱导**出范数（`sqrt(dot(x, x)) == norm`）
- 仿射空间 [`IAffine`](include/UGM/Interfaces/IAffine.h)：具有位置概念的空间，该空间中的元素称为点，会对应一个线性空间，两空间之间的元素有关联，如 `point-point => vector`，`point+vector => point` 

### 4.2 底层存储类型

- 数组 [`IArray`](include/UGM/Interfaces/IArray/IArray.h)：有序的元素序列，这将是我们各种类的基类，一般是 `std::array<T, N>`，其中 `T` 可以是 `float`，`int`，也可以是 `point`，`vec` 
- 矩阵 [`IMatrix`](include/UGM/Interfaces/IMatrix/IMatrix.h)：一维数组的数组

由于底层存储类型不同，上述代数概念的具体实现有所不同（抽象 => 具体），并引申出新的代数概念

#### 4.2.1 数组

底层存储类型为数组时，则可引申出如下代数概念

- 欧式（向量）空间 [`IEuclideanV`](include/UGM/Interfaces/IEuclideanV.h)：线性空间 + 内积（`dot(a,b) == a.x*b.x + a.y*b.y + a.z*b.z`）
- 欧式仿射空间 [`IEuclideanA`](include/UGM/Interfaces/IEuclideanA.h)：欧式（向量）空间对应的仿射空间
- 逐元素乘 [`IArrayHadamardProduct`](include/UGM/Interfaces/IArrayHadamardProduct.h)：`a*b=(a.x*b.x, a.y*b.y, a.z*b.z)` 

上述各种概念在具体为数组时会有对应的实现，如

```c++
T operator+(T a, T b) const {
    return {a[0]+b[0], a[1]+b[1], a[2]+b[2]};
}
```

#### 4.2.2 矩阵

由于该库用于离线渲染，实时渲染，游戏等，基本只要用 float4，因此也只需 3x3 和 4x4 的矩阵，因此该库也限制为只支持 3x3 和 4x4 的矩阵(并特化矩阵的乘法与逆的实现，如循环展开，simd 加速，以提高性能)。

大型矩阵的支持一般需要用线性代数库，如 Eigen 等。

底层通过一维数组的数组来实现，右乘，列优先，同于 OpenGL 与 DX（右乘+列优先的方案十分适合于 SIMD，同理左乘+行优先也如此）。

### 4.3 类

通过组合多个代数概念并加上具体类型支持的操作，可以轻松得到各种各样的代数类。他们满足不同的操作，极大地帮助使用者避免错误。

目前各种组合（主要部分）如下

![graph.jpg](https://ubpa.github.io/UData/UGM/graph.jpg)

> 若图片加载失败，请用该链接 [graph.jpg](https://ubpa.github.io/UData/UGM/graph.jpg) 

图中含有的类有

- 向量 [`vec`](include/UGM/vec.h) 
- 法向 [`normal`](include/UGM/normal.h)：本质是二重向量 bivector（[wiki](https://en.wikipedia.org/wiki/Bivector), [stackoverflow](https://stackoverflow.com/questions/30465573/transforming-surface-normal-vectors-and-tangent-vectors)）
- 点 [`point`](include/UGM/point.h) 
- 四元数 [`quat`](include/UGM/quat.h)：限制为单位四元数，用于表示旋转
- 矩阵 [`mat`](include/UGM/mat.h) 
- 变换 [`transform`](include/UGM/transform.h)：可表示仿射变换（平移，旋转，缩放）和射影变换（正交投影，透视投影）
- 颜色 [`rgb`](include/UGM/rgb.h) 
- 缩放 [`scale`](include/UGM/scale.h) 

该库还含有类

- 值 [`val`](include/UGM/val.h)：加法 + 数乘 + 逐元素乘
- 欧拉角 [`euler`](include/UGM/euler.h)：roll -> pitch -> yaw，同于 Unity3D
- 透明颜色 [`rgba`](include/UGM/rgba.h) 
- 表面向量 [`svec`](include/UGM/svec.h)：切空间的单位向量，上方向为 z 轴
- 齐次向量 [`hvec`](include/UGM/svec.h) 
- 包围盒 [`bbox`](include/UGM/bbox.h)：axis-aligned bounding box (AABB)
- 三角形 [`triangle`](include/UGM/triangle.h)：三个 `point` 
- 直线 [`line`](include/UGM/line.h)：`point` + `direction`
- 射线 [`ray`](include/UGM/ray.h)：增加了 `t min` 和 `t max` 的直线

## 5. 接口

类由多个代数概念组合而成，所以关键在于把握代数概念的接口，各代数概念位于 [include/UGM/Interfaces/](include/UGM/Interfaces/)。

所有接口都是类方法，方便使用，大部分情况下都可以利用 IDE 的代码提示功能（如 VS2019 的 intellisense）来查询接口。

此外还提供了渲染领域常见函数 / 算法，如相交（位于 `line`，`ray` 内）、[采样](include/UGM/sample.h)、[材质](include/UGM/material.h) 等。

## 6. SIMD

该库支持 SIMD，只要求支持 SSE2 指令。

主要加速的类为 `float4`，包括 `vecf4`，`pointf4` 等。

注意 `float3` 并没有 SIMD 加速，这是为了保持 `sizeof(float3)==3*sizeof(float)`，部分数学库通过使用含 `__m128` 的 `float3` 来实现 SIMD 加速，但这样 `sizeof(float3)==4*sizeof(float)`。目前可以通过显式将 `float3` 转成 `float4` 来获取加速效果。

加速部分包括

- `+-*/ ...` 
- `min/max/min_component/max_component/abs/sin/cos/...` 
- `transform * float4/bbox/transform` 
- `transform inverse` 
- `ray` 和 `sphere/triangle/bbox` 的相交
- `float3` 的 `dot/cross`（需要扩展成 `float4` 并使用 `float4::dot3` 和 `float4::cross3`）
- ...

## 7. Natvis

泛型编程在 debug 时会引入大量的单继承，该库使用了单继承化技术，单继承深度也很大，导致在 IDE 中查看类成员变量会很麻烦。

> **示例** 
>
> ![TextBox default visualization](https://docs.microsoft.com/en-us/visualstudio/debugger/media/dbg_natvis_textbox_default.png?view=vs-2019)
>
> > 若图片加载失败，请用该链接 [TextBox default visualization](https://docs.microsoft.com/en-us/visualstudio/debugger/media/dbg_natvis_textbox_default.png?view=vs-2019) 

我们可使用 VS2019 的 natvis 功能来实现定制的视图

![natvis_demo.jpg](https://ubpa.github.io/UData/UGM/natvis_demo.jpg)

> 若图片加载失败，请用该链接 [natvis_demo.jpg](https://cdn.jsdelivr.net/gh/Ubpa/UData@master/UGM/natvis_demo.jpg) 

当使用 `find_package(UGM REQUIRED)` 时，会自动给解决方案添加一个项目，包含 `UGM_<VERSION>.natvis`，从而使得其他项目都可以支持 natvis（[VS2019 支持多种方式引入 natvis](https://docs.microsoft.com/en-us/visualstudio/debugger/create-custom-views-of-native-objects?view=vs-2019#BKMK_natvis_location)，但这是目前我能想到的最合适的方式）。

![use_natvis.jpg](https://ubpa.github.io/UData/UGM/use_natvis.jpg)

> 若图片加载失败，请用该链接 [use_natvis.jpg](https://ubpa.github.io/UData/UGM/use_natvis.jpg) 

## Future Features

- [x] 更轻量级的单继承化技术（减少 coding overhead），进一步加快编译速度
- [x] 提供 `config.h`，方便定制功能
- [x] 给 `float4` 一系列 `v3_*` 接口
- [ ] 支持 CUDA
- [x] swizzle
- [x] 去除依赖 XSIMD
- [ ] vector op: log, exp, cos, sin, ...
- [ ] ...

## Copyright and Licensing

You can copy and paste the license summary from below.

```
MIT License

Copyright (c) 2020 Ubpa

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

