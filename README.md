# glsl2spirv
Simple embeedable header/static lib GLSL to SPIRV generator

#usage
```C++
#include <glsl2spirv.hpp>
// ...
glsl2spirvInit();

std::ifstream file("shader.vert");
std::string str;
std::string file_contents;
while (std::getline(file, str)) {
    file_contents += str;
    file_contents.push_back('\n');
}

std::vector<unsigned int> spv;
glsl2spirv(glsl2spirvShaderTypeVertex, "shaderfile", file_contents.c_str(), spv, [](const char *m) { std::cout<< m << std::endl; });

glsl2spirvShutdown();
```

#compilation
```
cmake <DIR>
make
make install
```
