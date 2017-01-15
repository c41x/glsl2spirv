#include <glslang/Include/ShHandle.h>
#include <glslang/Include/revision.h>
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/GLSL.std.450.h>
#include <SPIRV/doc.h>
#include <SPIRV/disassemble.h>
#include <functional>
#include <fstream>
#include <glslang/OSDependent/osinclude.h>
#include "ResourceLimits.h"

void glsl2spirvInit() {
    glslang::InitializeProcess();
}

enum glsl2spirvShaderType {
    glsl2spirvShaderTypeVertex,
    glsl2spirvShaderTypeTessControl,
    glsl2spirvShaderTypeTessEvaluation,
    glsl2spirvShaderTypeGeometry,
    glsl2spirvShaderTypeFragment,
    glsl2spirvShaderTypeCompute,
};

bool glsl2spirv(glsl2spirvShaderType type, const char *name, const char * const *glsl, std::vector<unsigned int> &out, std::function<void(const char*)> errorMessage) {
    bool compileFailed = false;
    bool linkFailed = false;
    TBuiltInResource resources = glslang::DefaultTBuiltInResource;

    auto stage = EShLangVertex;
    if (type == glsl2spirvShaderTypeTessControl) stage = EShLangTessControl;
    else if (type == glsl2spirvShaderTypeTessEvaluation) stage = EShLangTessEvaluation;
    else if (type == glsl2spirvShaderTypeGeometry) stage = EShLangGeometry;
    else if (type == glsl2spirvShaderTypeFragment) stage = EShLangFragment;
    else if (type == glsl2spirvShaderTypeCompute) stage = EShLangCompute;

    const char * const *fileNameList = &name;

    EShMessages messages = (EShMessages)(EShMsgDefault | EShMsgSpvRules | EShMsgVulkanRules);

    glslang::TProgram& program = *new glslang::TProgram;
    glslang::TShader* shader = new glslang::TShader(stage);
    shader->setStringsWithLengthsAndNames(glsl, NULL, fileNameList, 1);
    shader->setShiftSamplerBinding(0);
    shader->setShiftTextureBinding(0);
    shader->setShiftImageBinding(0);
    shader->setShiftUboBinding(0);
    shader->setFlattenUniformArrays(false);
    shader->setNoStorageFormat(false);

    const int defaultVersion = 100;
    if (!shader->parse(&resources, defaultVersion, false, messages))
        compileFailed = true;

    program.addShader(shader);

    if (errorMessage) {
        errorMessage(shader->getInfoLog());
        errorMessage(shader->getInfoDebugLog());
    }

    // Link
    if (!program.link(messages))
        linkFailed = true;

    // Map IO
    if (!program.mapIO())
        linkFailed = true;

    // Dump SPIR-V
    if (compileFailed || linkFailed) {
        return false;
    }

    for (int stage = 0; stage < EShLangCount; ++stage) {
        if (program.getIntermediate((EShLanguage)stage)) {
            std::string warningsErrors;
            spv::SpvBuildLogger logger;
            glslang::GlslangToSpv(*program.getIntermediate((EShLanguage)stage), out, &logger);

            if (errorMessage) {
                errorMessage(logger.getAllMessages().c_str());
            }
        }
    }

    // Free everything up, program has to go before the shaders
    // because it might have merged stuff from the shaders, and
    // the stuff from the shaders has to have its destructors called
    // before the pools holding the memory in the shaders is freed.
    delete &program;
    delete shader;

    return !compileFailed && !linkFailed;
}

bool glsl2spirv(glsl2spirvShaderType type, const char *name, const char * const glsl, std::vector<unsigned int> &out, std::function<void(const char*)> errorMessage) {
    const char * const * const g = &glsl;
    return glsl2spirv(type, name, g, out, errorMessage);
}

void glsl2spirvShutdown() {
    glslang::FinalizeProcess();
}

int C_DECL main(int argc, char* argv[])
{
    glsl2spirvInit();

    std::ifstream file("/home/calx/dev/granite/tests/rosemary/shader.vert");
    std::string str;
    std::string file_contents;
    while (std::getline(file, str)) {
        file_contents += str;
        file_contents.push_back('\n');
    }

    std::cout << ":::" << file_contents << ":::" <<std::endl;

    std::vector<unsigned int> spv;
    glsl2spirv(glsl2spirvShaderTypeVertex, "shaderfile", file_contents.c_str(), spv, [](const char *m) { std::cout<< m << std::endl; });

    for (auto &h : spv) {
        std::cout << h << " ";
    }

    glsl2spirvShutdown();
    return 0;
}
