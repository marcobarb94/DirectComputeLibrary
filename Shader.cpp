#include "Shader.h"

using namespace DCL;

HRESULT Shader::compileFromFileAndLoad(LPCWSTR fileName, LPCSTR startupFunctionName)
{
	/// compiled shader errors
	ID3DBlob* errorsPointer = NULL;

	/// compiled shader code pointer
	ID3DBlob* shaderCodePointer = NULL;


	HRESULT res = D3DCompileFromFile(fileName, NULL, NULL, startupFunctionName, "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &shaderCodePointer, &errorsPointer);
	if ((res != S_OK)) return res;
	res = device.getDevice()->CreateComputeShader(shaderCodePointer->GetBufferPointer(), shaderCodePointer->GetBufferSize(), NULL, &computeShaderPointer);
	isLoaded = (res == S_OK);
	return res;
}

HRESULT Shader::loadCompiledShader(LPCWSTR fileName)
{
	return E_NOTIMPL;
}

Shader::Shader(GPUDevice device)
{
	this->device = device; 
}

bool Shader::removeBuffer(unsigned short GPUslot)
{
	return false;
}

Shader::~Shader()
{
}

int Shader::runShader(UINT X, UINT Y, UINT Z)
{
	return 0;
}

GPUDevice Shader::getDevice()
{
	return device;
}

template<typename T>
unsigned short Shader::addBuffer(CPUGPU_IO CPURequirement, CPUGPU_IO GPURequirement, size_t size, unsigned short GPUslot, T* data)
{
	return 0;
}
