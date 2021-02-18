#include "Shader.h"
#include <iostream>

HRESULT Shader::compileFromFileAndLoad(LPCWSTR fileName, LPCSTR startupFunctionName)
{
	HRESULT res = D3DCompileFromFile(fileName, NULL, NULL, startupFunctionName, "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &shaderCodePointer, &errorsPointer);
	if ((res != S_OK)) 
		return res;
	res = device->getDevice()->CreateComputeShader(shaderCodePointer->GetBufferPointer(), shaderCodePointer->GetBufferSize(), NULL, &computeShaderPointer);
	isLoaded = (res == S_OK);
	return res;
}

HRESULT Shader::loadCompiledShader(LPCWSTR fileName)
{
	// TODO
	return E_NOTIMPL;
}

Shader::Shader(GPUDevice* device)
{
	this->device = device; 
	GPUSlots = std::map<unsigned short, Buffer*>();
}

bool Shader::removeBuffer(unsigned short GPUslot)
{
	//TODO
	delete GPUSlots[GPUslot];
	GPUSlots.erase(GPUslot);
	return true;
}

Shader::~Shader()
{
	// remove all shaders 
	for (auto itv = GPUSlots.begin(); itv != GPUSlots.end(); itv++) {
		delete itv->second;
	}
	//free Shader resources 
	GPGPU::releaseResource(computeShaderPointer);
	GPGPU::releaseResource(shaderCodePointer);
	GPGPU::releaseResource(errorsPointer);
}

int Shader::runShader(UINT X, UINT Y, UINT Z)
{
	//TODO
	return 0;
}

GPUDevice* Shader::getGPUDevice()
{
	return device;
}

void Shader::getLastErrors(const char* &messagePointer)
{
	messagePointer = reinterpret_cast<const char*>(this->errorsPointer->GetBufferPointer());
	//std::memcpy((void*)messagePointer, reinterpret_cast<const char*>(this->errorsPointer->GetBufferPointer()), sizeof this->errorsPointer->GetBufferPointer());
}


