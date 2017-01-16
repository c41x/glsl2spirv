#ifndef GLSL_2_SPIRV_HPP
#define GLSL_2_SPIRV_HPP

enum glsl2spirvShaderType {
    glsl2spirvShaderTypeVertex,
    glsl2spirvShaderTypeTessControl,
    glsl2spirvShaderTypeTessEvaluation,
    glsl2spirvShaderTypeGeometry,
    glsl2spirvShaderTypeFragment,
    glsl2spirvShaderTypeCompute,
};

void glsl2spirvInit();
bool glsl2spirv(glsl2spirvShaderType type, const char *name, const char * const *glsl, std::vector<unsigned int> &out, std::function<void(const char*)> errorMessage);
bool glsl2spirv(glsl2spirvShaderType type, const char *name, const char * const glsl, std::vector<unsigned int> &out, std::function<void(const char*)> errorMessage);
void glsl2spirvShutdown();

#endif
