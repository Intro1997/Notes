# Array

## 引用 vs 浅拷贝 vs 深拷贝

- 引用

```js
const a = {};
b = a; // b refers to a
```

- 浅拷贝

只能复制第一层，对于第一层之后的数据表现为引用。<a href="#r1"><sup>[1]</sup></a>

```js
const obj_arr = [
  { name: "Tom", age: 12 },
  { name: "Sam", age: 13 },
];

const obj_arr_copy = Array.from(obj_arr);

obj_arr_copy.push({ name: "Jane", age: 14 });

console.log(obj_arr); // [ { name: 'Tom', age: 12 }, { name: 'Sam', age: 13 } ]
console.log(obj_arr_copy); /* [
  { name: 'Tom', age: 12 },
  { name: 'Sam', age: 13 },
  { name: 'Jane', age: 14 }
] */

obj_arr_copy[0].age = 99;

console.log(obj_arr); // [ { name: 'Tom', age: 99 }, { name: 'Sam', age: 13 } ]
console.log(obj_arr_copy); /* [
  { name: 'Tom', age: 99 },
  { name: 'Sam', age: 13 },
  { name: 'Jane', age: 14 }
] */
```

- 深拷贝

可以通过 JSON stringify 和 parse 方法进行深拷贝

```js
const obj_arr = [
  { name: "Tom", age: 12 },
  { name: "Sam", age: 13 },
];

const obj_arr_copy = JSON.parse(JSON.stringify(obj_arr));

obj_arr_copy[0].age = 99;

console.log(obj_arr); // [ { name: 'Tom', age: 12 }, { name: 'Sam', age: 13 } ]
console.log(obj_arr_copy); // [ { name: 'Tom', age: 99 }, { name: 'Sam', age: 13 } ]
```

# Reference

1. <a id="r1" href="https://kanboo.github.io/2018/01/27/JS-ShallowCopy-DeepCopy/#:~:text=%E6%B7%BA%E6%8B%B7%E8%B2%9D%EF%BC%9A-,%E5%8F%AA%E8%83%BD%E9%81%94%E5%88%B0%E6%B7%BA%E5%B1%A4%E7%9A%84%E8%A4%87%E8%A3%BD(%E7%AC%AC%E4%B8%80%E5%B1%A4),-%EF%BC%8C%E8%8B%A5%E6%9C%89%E7%AC%AC%E4%BA%8C%E5%B1%A4">Kanboo Notes —— JS-淺拷貝(Shallow Copy) VS 深拷貝(Deep Copy)</a>
