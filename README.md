# UGM

> Ubpa Graphics Math

## 类

- bbox
- euler：roll -> pitch -> yaw，Tait–Bryan angles, extrinsic rotations
- mat：欧式向量空间，矩阵乘法
- normal：欧式向量空间
- point：欧式仿射空间
- quat
- ray
- rgb：线性 + 逐元素乘法（Hadamard Product）
- rgba
- scale：线性 + 逐元素乘法（Hadamard Product）
- transform：矩阵乘法
- val：cast 特化，加法 + 数乘 + 逐元素乘法（Hadamard Product）
- vec：欧式向量空间

## 接口

### 工具

**输入输出** 

```c++ 
struct IInOut {
    friend std::ostream& operator<<(std::ostream& os, const Impl& x);
    friend std::istream& operator>>(std::istream& is, Impl& x);
};
```

**其他** 

### 代数

**加法** 

```c++
template<typename Impl>
struct IAdd {
    const Impl operator+(const Impl& y) const;
    Impl& operator+=(const Impl& y);
    const Impl operator-() const;
    const Impl operator-(const Impl& y) const;
    Impl& operator-=(const Impl& y);
};
```

**乘法** 

```c++
template<typename Impl>
struct IMul {
    const Impl operator*(const Impl& y) const;
    const Impl inverse() const;
};
```

**数乘** 

```c++
template<typename Impl>
struct IScalarMul {
    const Impl operator*(U k) const;
    Impl& operator*=(U k);
}
```

**度量** 

```c++
template<typename Impl>
struct IMetric {
    static F distance(const Impl& x, const Impl& y);
    F distance(const Impl& y) const;
}
```

**范数** 

```c++
template<typename Impl>
struct INorm {
    F norm() const;
    const Impl normalize() const;
    bool is_normalized() const;
    Impl& normalize_self();
};
```

**内积** 

```c++
template<typename Impl>
struct IInnerProduct {
    static F dot(const Impl& x, const Impl& y);
    F dot(const Impl& y) const;
    F norm2() const;
    static F distance2(const Impl& x, const Impl& y);
    F distance2(const Impl& y) const;
    static F cos_theta(const Impl& x, const Impl& y);
    F cos_theta(const Impl& y) const;
};
```

**仿射** 

```c++
template<typename ImplP, typename ImplV>
struct IAffine {
    const ImplP operator+(const ImplV& v) const;
    ImplP& operator+=(const ImplV& v);
    ImplP operator-(const ImplV& v) const;
    ImplP& operator-=(const ImplV& v);
    const ImplV operator-(const ImplP& v) const;
};
```

**环** 

加法 + 乘法

**线性** 

加法 + 数乘

**欧式向量空间** 

内积 + 范数 + 度量 + 线性

**欧式仿射空间** 

仿射 + 度量

