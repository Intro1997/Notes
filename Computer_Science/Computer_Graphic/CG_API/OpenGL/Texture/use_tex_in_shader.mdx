# 如何借助 OpenGL 保存图像数据到 GPU？

OpenGL 的 Texture 对象，就是解决标题提出问题的答案。我们通过以下操作，将保存了图像的 data 数据，拷贝到 GPU 中去：

```cpp
int img_width, img_height;
GLenum img_format;
const char * data = LoadImage("path/to/image.png", img_width, img_height, img_format);
GLuint textureId;
glGenTextures(1, &textureId);
glBindTexture(GL_TEXTUER_2D, textureId);
glTexImage2d(GL_TEXTUER_2D, img_format, img_width, img_height,
             0, img_format, GL_UNSIGNED_BYTE, data);
```

# 如何在 shader 中使用？

如果我有一个 fragment shader：

```glsl
#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main() { FragColor = texture(texture1, TexCoords); }
```

那么，可以通过 glUniform 相关的函数来指定 shader 访问的 texture：

```cpp
glUseProgram(shader_program);
glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
```

# texture1 为什么是 0？

参阅 [glBindTexture 文档](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindTexture.xhtml)了解到：

> The value zero is reserved to represent the default texture for each texture target.

对于 Texture 来说，0 表示默认 texture 对象的名称；并且如果我们输出之前创建的 texture 对象，正常情况下得到的应该是一个非 0 值。
那么，texture1 为什么是 0 呢？

不借助世界网友的回答下，我们能参考信息只有官方文档了。首先 shader 中的 texture1 是一个 sampler2D 对象，我们可以找到这篇[文档](<https://www.khronos.org/opengl/wiki/Sampler_(GLSL)>)，在 [4 Binding textures to samplers](https://www.khronos.org/opengl/wiki/Sampler_
(GLSL)#Binding_textures_to_samplers) 这节中，出现了一个 "texture image units" 这个概念：

> The OpenGL rendering context has multiple independent bind locations for textures called texture image units.

那么这个 "texture image unit" 和 "GLSL sampler" 之间的的关系是什么呢：

> The value you provide to a sampler uniform is the texture image unit to which you will bind the texture that the sampler will access.

破案了，我们的 sampler 需要访问的就是这个 "texture image unit"。同时，通过官方给出的示例我们也可以得知：

1. "texture image unit" 通过 glActiveTexture(GL_TEXTURE0 + i) 来激活
2. 通过 glBindTexture() 的方式可以将创建的 texture 对象绑定到当前激活的 "texture image unit" 上

那么，我们传给 sampler 的值为什么是 0 呢？
参考 [glActiveTexture 文档](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glActiveTexture.xhtml)，可知：

> The initial value is GL_TEXTURE0.

# 为什么不直接使用 texture id？

其实就连官方自己都说这种关联 texture 到 sampler 的方式[并不直观](<https://www.khronos.org/opengl/wiki/Sampler_(GLSL)#Binding_textures_to_samplers:~:text=textures%20is%20somewhat-,unintuitive,-.%20The%20mapping%20is>)，我在 KHRONOS 论坛上的一处提问中找到了方便理解这一流程的[答案](https://community.khronos.org/t/why-use-or-define-texture-units-and-what-is-the-mapping-with-uniforms-and-texture-units/106265/8)

简单来说，就是 OpenGL 1.3 引入多纹理之前，每个纹理类型只能绑定一张纹理，为了兼容这个情况，才有了现在这种 glActiveTexture 的机制。我个人理解引入 glActiveTexture 之后，将之前“每个纹理类型只能绑定一张纹理”的整体看作一个 "texture image unit"，用多个 "texture image unit" 的方式来扩展支持的纹理数量。

不过 OpenGL 4.5 之后，就无需在关心 "texture image unit" 这个东西了。

但。。。为什么不学 vulkan 呢？`:)`

# texture image unit 的上限与 GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS

texture image unit 的上限很好获取，参考[这里](https://stackoverflow.com/questions/46426331/number-of-texture-units-gl-texturei-in-opengl-4-implementation-in-visual-studi)

而 GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 不好理解，根据[官方的解释](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml#:~:text=GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)，我理解为，当定义 max 个 texture 对象时，让所有类型的 shader 同时访问这些 texture 对象，所产生的 texture 对象的数量。

这里的所有类型的 shader，指的是 OpenGL 支持的 [shader stage](https://www.khronos.org/opengl/wiki/Shader#Stages)

因此，理论上 GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 查询到的数量应该等于 `shader_stage_count * max_texture_unit`。
