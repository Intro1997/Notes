# 笔记思路

本笔记从两个部分来描述将傅立叶级数：

1. 傅立叶级数展开：周期为 $[-\pi, \pi]$（或者是 $2\pi$）的周期函数，以下简称 $2\pi$ 情形
2. 傅立叶级数展开：周期为 $[-T, T]$（或者是 $2T$） 的周期函数，以下简称一般情形

# 傅立叶级数公式

## $2\pi$ 情形

$$
f(x)=c_0 + \sum^{\infty}_{n=1}a_ncos(nx) + \sum^{\infty}_{n=1}b_nsin(nx)\\
\begin{cases}
a_n &= \frac{1}{\pi}\int_{-\pi}^{\pi}f(x)\cdot cos(nx)dx\\
b_n&=\frac{1}{\pi}\int_{-\pi}^{\pi}f(x)\cdot sin(nx)dx\\
c_0&=\frac{1}{2}a_0
\end{cases}
$$

推导过程在[这里](./2pi_form.md)

## 一般情形

$$
\begin{align}
&f(x)=\frac{a_0}{2} + \sum^{\infty}_{n=1}a_ncos(n\omega x) + \sum^{\infty}_{n=1}b_nsin(n\omega x)\\
&\begin{cases}
a_n=\frac{2}{T}\int_{0}^{2T}f(x)\cdot cos(n\omega x)dx\\
b_n=\frac{2}{T}\int_{0}^{2T}f(x)\cdot sin(n\omega x)dx
\end{cases}
\end{align}
$$

推导过程在[这里](./general_form.md)

## 复数情形

$$
\begin{align}
f(x)&=\sum_{n=-\infty}^{\infty}C_ne^{in\omega x}\\
C_n&=\frac{1}{T}\int_{0}^{T}f(x)e^{-in\omega x}dx\\
\end{align}
$$

推导过程在[这里](./complex_form.md)

## 非周期函数情形（逆傅立叶变换）

$$
f(x)=\frac{1}{2\pi}\int_{-\infty}^{\infty}\int_{-\infty}^{\infty}f(x)e^{-i\omega x}dx\cdot e^{i\omega x}d\omega\\
$$

其中：

$$
F(\omega)=\int_{-\infty}^{\infty}f(x)e^{-i\omega x}dx
$$

被称作傅立叶变换。
傅立叶变换将时域上的信号变换到频域。傅立叶逆变换则相反。

推导过程在[这里](./non_periodic_function_form.md)

## 离散傅立叶级数

$$
\begin{cases}
x[k]&=\sum^{N-1}_{n=0}X[n]e^{in\frac{2\pi}{N}k}\\
X[n]&=\frac{1}{N}\sum_{k=0}^{N-1}x[k]e^{-in\frac{2\pi}{N}k}
\end{cases}
$$

推导过程在[这里](./DFS.md)
