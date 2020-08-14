# API

本数学库采用了组件编程的思想（一个类由各组件组合而成）。我们首先介绍各个组件的功能，然后给出各个具体类包含哪些组件。

> 近期可能会对 API 进行一个革新，目前作者在忙于其他事情

## 1. 组件

- `Impl` 指最终类
- 接口
  - 部分接口（主要是对称型，`operator`）含有 static 和 non-static 两种形式
  - 含`<...>` 说明是模板函数
  - 省略 `const`, `noexcept`, `Type::`，参数（部分）
  - 包含一些简化书写的记号（`{`, `}`, `/`）

### 1.1 代数型组件

#### IInOut

输入输出

| 接口                                | 说明                               |
| ----------------------------------- | ---------------------------------- |
| `std::{i/o}stream& operator{<</>>}` | 输入输出，格式为元素列，用空格间隔 |

#### IAdd

加法

| 接口                                   | 说明                                 |
| -------------------------------------- | ------------------------------------ |
| `operator{+/-/+=/-=}(const Impl& rhs)` | 元素间 +/- 运算                      |
| `operator{*/*=}(int k)`                | 多重加法变乘法，注意参数类型为 `int` |
| `operator-`                            | 逆                                   |

#### IMul

乘法

| 接口                              | 说明          |
| --------------------------------- | ------------- |
| `Impl operator*(const Impl& rhs)` | 元素间 * 运算 |
| `Impl inverse()`                  | 逆            |

#### IRing

环

该组件包含依赖组件：`IAdd`, `IMul`  

#### IScalarMul

> TODO: 该接口需要整理

数乘

| 接口                    | 说明       |
| ----------------------- | ---------- |
| `Impl operator*(U k)`   | 数乘       |
| `Impl& operator*=(U k)` | 数乘       |
| `Impl operator/(U k)`   | 数乘逆运算 |
| `Impl& operator/=(U k)` | 数乘逆运算 |

#### ILinear

线性

该组件包含依赖组件：`IAdd`, `IScalarMul`  

#### IMetric

度量

| 接口         | 说明 |
| ------------ | ---- |
| `F distance` | 距离 |

#### INorm

范数，自然诱导度量

该组件包含依赖组件：`IMetric`, `ILinear`  

| 接口                     | 说明                           |
| ------------------------ | ------------------------------ |
| `F norm()`               | 范数                           |
| `Impl normalize()`       | 范化                           |
| `bool is_normalized()`   | 检测是否已范化，容忍度为 0.001 |
| `Impl& normalize_self()` | 范化自身                       |

#### IInnerProduct

内积，自然诱导范数，夹角

该组件包含依赖组件：`INorm` 

| 接口                         | 说明                                                       |
| ---------------------------- | ---------------------------------------------------------- |
| `F dot`                      | 内积                                                       |
| `F norm2()`                  | 范数平方                                                   |
| `F distance2`                | 距离平方                                                   |
| `F cos_theta`                | 两元素夹角的余弦值                                         |
| `F cot_theta`                | 量元素夹角的余切值                                         |
| `Impl project(Impl n)`       | 投影到方向 `n` 上                                          |
| `Impl perpendicular(Impl n)` | 投影到方向 `n` 垂直的平面上（或者说求出垂直于 `n` 的分量） |

#### IAffineSubspace

仿射子空间（点，线，面）

| 接口                                 | 说明 |
| ------------------------------------ | ---- |
| `Impl operator{+|-|+=|-=}(Vector v)` | 平移 |

#### IAffineRealSubspace

仿射真子空间（线，面）

该组件包含依赖组件：`IAffineSubspace` 

| 接口                       | 说明           |
| -------------------------- | -------------- |
| `point`                    | 成员变量，锚点 |
| `Impl move`                | 改变锚点       |
| `Impl& move_self(Point p)` | 改变锚点       |

#### IAffine

仿射（点）

该组件包含依赖组件：`IAffineSubspace` 

| 接口                        | 说明       |
| --------------------------- | ---------- |
| `Vector operator-(Point y)` | 点差成向量 |

#### IOLine

过原点直线（即仅含方向）

`O` 指原点 origin

| 接口               | 说明           |
| ------------------ | -------------- |
| `dir`              | 成员变量，方向 |
| `Vector inv_dir()` | 反方向         |

#### ILine

直线（方向+起点）

该组件包含依赖组件：`IOLine`, `IAffineSubspace` 

| 接口            | 说明              |
| --------------- | ----------------- |
| `Point at(F t)` | `point + t * dir` |

#### IOPlane

过原点直线（即仅含方向）

`O` 指原点 origin

| 接口     | 说明           |
| -------- | -------------- |
| `normal` | 成员变量，法向 |

#### IPlane

直线（方向+起点）

该组件包含依赖组件：`IOPlane`, `IAffineSubspace` 

#### IEuclideanV

欧式向量空间

该组件包含依赖组件：`IInnerProduct`, `IArrayLinear`（`IArrayLinear` 是基于 `IArray` 对 `ILinear` 的具体化）

对于 4 维向量，因为有 `__m128` 加速，所以提供了一些 3 维向量的接口（`v3_*`），用于加速 3 维向量的运算（用 4 维向量存储 3 维向量）

#### IEuclideanA

欧式仿射空间

该组件包含依赖组件：`IMetric`, `IAffine`, `IArray` 

| 接口          | 说明     |
| ------------- | -------- |
| `F distance2` | 距离平方 |

#### ICross

叉乘（仅支持 3 维向量）

| 接口          | 说明                                                         |
| ------------- | ------------------------------------------------------------ |
| `Impl cross`  | 叉乘                                                         |
| `F sin_theta` | 利用叉乘计算 `sin_theta`（不一定比用 `cos_theta()` 再求 `sin theta` 快） |

### 1.2 存储型组件

#### 1.2.1 数组

##### IArray

> TODO：进一步分割功能

底层用 `std::array<T, N>` 实现，当使用 `<float, 4>` 时，用 `__m128` 实现

支持 `std::array` 的所有接口，如 `begin`, `end`, `operator[]` 等

此外，还有接口

| 接口                                        | 说明                                                         |
| ------------------------------------------- | ------------------------------------------------------------ |
| `IArray(const T* data)`                     | 用数组指针构造                                               |
| `IArray(const IArray& arr)`                 | 拷贝构造函数                                                 |
| `IArray(T t)`                               | 用单元素构造                                                 |
| `IArray(Ts... ts)`                          | 用多元素构造，元素个数 `sizeof...(Ts)` 等于维度 `N`，要求 `Ts...` 能 `static_cast<T>` |
| `static Impl zero()`                        | 等价于 `IArray(0)`                                           |
| `T get<size_t i>()`                         | 类似于 `operator[]`，但对于 `__m128` 有优化                  |
| `void set<size_t i>(T t)`                   | 对于 `__m128` 有优化                                         |
| `Impl replicate<size_t i>`                  | 用第 `i` 个元素填充所有元素得到新 `Impl`，对于 `__m128` 有优化 |
| `operator{</<=/>/>=/==/!=}`                 | 字典序比较，对于 `__m128` 有优化                             |
| `bool all_{lt/le/gt/ge}(const IArray& rhs)` | 全序比较，对于 `__m128` 有优化                               |

对上述各种代数型组件有基于数组的具体实现，如 `IArrayAdd`，`IArrayMScalarMul` 等，没有新增接口

##### IArrayCast

该组件包含依赖组件：`IArray` 

| 接口                        | 说明                                                         |
| --------------------------- | ------------------------------------------------------------ |
| `To cast_to<typename To>()` | 对每一个成员进行`static_cast<To::T>`，要求 `To::N` 不超过 `Impl::N` |
| `To& as<typename To>()`     | 直接 `reinterpret_cast`，要求 `sizeof(To)==sizeof(Impl)`     |

##### IArray1D

静态要求数组是 1D 的

该组件包含依赖组件：`IArray` 

##### IArray1D_Util

该组件包含依赖组件：`IArray` 

| 接口                            | 说明                                     |
| ------------------------------- | ---------------------------------------- |
| `Impl abs()`                    | 绝对值                                   |
| `T& {min/max}_component()`      | 最小/大元素                              |
| `T {min/max}_component() const` | 最小/大元素，该 const 版本支持 SIMD 加速 |
| `size_t {min/max}_dim()`        | 数值最小的维度                           |
| `Impl {min/max}`                | 逐元素取较小/大值                        |

#### 1.2.2 矩阵

##### IMatrix

1D 数组的数组即为矩阵

列优先

该组件包含依赖组件：`IArrayUtil` 

| 接口                                 | 说明                                                  |
| ------------------------------------ | ----------------------------------------------------- |
| `IMatrix(std::array<F, N*N> data)`   | 用数组构造，**列优先**                                |
| `void init(std::array<F, N*N> data)` | 用数组赋值，**列优先**                                |
| `IMatrix(Ts... vals)`                | 用 `N*N` 个元素构造，**行优先**                       |
| `void init(Ts... vals)`              | 用 `N*N` 个元素赋值，**行优先**                       |
| `Impl eye()`                         | 对角为 1 其余为 0 的矩阵                              |
| `operator(size_t r, size_t c)`       | 第 `r` 行，第 `c` 列的元素                            |
| `at(size_t r, size_t c)`             | 第 `r` 行，第 `c` 列的元素                            |
| `operator(size_t n)`                 | 第 `n` 个元素，**行优先**                             |
| `at(size_t n)`                       | 第 `n` 个元素，**行优先**                             |
| `F trace()`                          | 迹                                                    |
| `Impl transpose()`                   | 转置                                                  |
| `(U,S, V) SVD()`                     | SVD 分解，支持 2x2 和 3x3 矩阵（非精确，耗时 1~2 ms） |
| `(U, S, V) signed_SVD()`             | SVD 分解，保证 `det(UV^T)>0`，支持 2x2 和 3x3 矩阵    |
| `F det()`                            | 行列式                                                |
| `F* data()`                          | 数据                                                  |

##### IMatrix_Mul

该组件包含依赖组件：`IMatrix`, `IMul` 

| 接口                         | 说明       |
| ---------------------------- | ---------- |
| `Vector operator*(Vector v)` | 矩阵乘向量 |

##### IMatrix_InOut

该组件包含依赖组件：`IMatrix` 

| 接口                       | 说明           |
| -------------------------- | -------------- |
| `void print(std::ostream)` | 以矩阵形式打印 |

## 2. 类

类就是上述组件的一个组合，另外可能有新增的功能

用户请查看 [include/UGM](include/UGM) 的各个头文件查看具体内容

这里不再对各类的功能进行描述，仅做简述

### bbox

轴对齐包围盒

依赖组件：`IArrayUtil` 

### euler

欧拉角（ZXY）

依赖组件：`IArray1D_Util` 

先绕物体空间 y 轴旋转，再绕物体空间 x 轴旋转，再绕物体空间 z 轴旋转

Tait–Bryan angles, extrinsic rotations (math: x-y-z, game: z-x-y)

### hvec

齐次向量

依赖组件：`IEuclideanV` 

### line

线

依赖组件：`ILine`, `IInOut` 

### mat

矩阵

依赖组件：`IMatrixInOut`, `IMatrixMul`, `IArrayLinear`, `IRing` 

### normal

法向（本质是 bivector）

依赖组件：`ICross` 

### plane

平面

依赖组件：`IInOut`, `IPlane` 

### point

点

依赖组件：`IArray1D_Util`, `IEuclideanA` 

### quat

四元数

依赖组件：`IMul`, `IArrayUtil` 

### ray

射线

依赖组件：`ILine`, `IInOut` 

### rgb

rgb 颜色

依赖组件：`IArray1D_Util`, `IArrayHadamardProduct`, `IArrayLinear` 

### rgba

rgba 颜色

依赖组件：`IArray1D_Util`

### scale

缩放

依赖组件：`IArray1D_Util`

### svec

表面向量

依赖组件：`IArray1D_Util`, `ICross` 

### transform

变换

依赖组件：`IMatrixInOut`, `IMatrixMul`

### triangle

三角形

依赖组件：`IArrayInOut` 

### val

值，不使用 SIMD（方便存储）

依赖组件：`IArrayLinear`, `IArrayHadamardProduct`, `IArray1D_Util` 

### vec

向量

依赖组件：`IArray1D_Util`, `IEuclideanV`（对于 3 维向量，还依赖组件 `ICross`）

