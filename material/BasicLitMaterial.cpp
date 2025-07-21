#include "BasicLitMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicLitMaterial::BasicLitMaterial(const std::string& vertexShader, const std::string& fragmentShader):
        ShaderMaterial<BaseLitMaterial>(vertexShader, fragmentShader)  {
    }

    BasicLitMaterial::BasicLitMaterial(const std::string& builtInShaderName):
        ShaderMaterial<BaseLitMaterial>(builtInShaderName) {
    }

    BasicLitMaterial::BasicLitMaterial(): BasicLitMaterial("BasicLit") {
    }

    Bool BasicLitMaterial::build() {
        ShaderMaterial<BaseLitMaterial>::build();
        this->setLit(true);
        return true;
    }

    WeakPointer<Material> BasicLitMaterial::clone() {
        WeakPointer<BasicLitMaterial> newMaterial = Engine::instance()->createMaterial<BasicLitMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }
}