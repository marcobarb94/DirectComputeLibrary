#include "Shader.h"
#include <iostream>

HRESULT Shader::compileFromFileAndLoad(LPCWSTR fileName, LPCSTR startupFunctionName)
{
	HRESULT res = D3DCompileFromFile(fileName, NULL, NULL, startupFunctionName, "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &shaderCodePointer, &errorsPointer);
	if ((res != S_OK)) {
		return res;
	}
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
	tBufferGPUSlots = std::map<unsigned short, Buffer*>();
	cBufferGPUSlots = std::map<unsigned short, ID3D11Buffer*>();
}

bool Shader::removeStructuredBuffer(unsigned short GPUslot)
{
	//TODO
	if (tBufferGPUSlots.find(GPUslot) == tBufferGPUSlots.end()) return false;
	delete tBufferGPUSlots[GPUslot];
	tBufferGPUSlots.erase(GPUslot);
	return true;
}


Shader::~Shader()
{
	// remove first from shader itself

	// remove all shaders 
	for (auto itv = tBufferGPUSlots.begin(); itv != tBufferGPUSlots.end(); itv++) {
		delete itv->second;
	}
	for (auto itv = uBufferGPUSlots.begin(); itv != uBufferGPUSlots.end(); itv++) {
		delete itv->second;
	}
	for (auto itv = cBufferGPUSlots.begin(); itv != cBufferGPUSlots.end(); itv++) {
		GPGPU::releaseResource(itv->second);
	}

	//free Shader resources 
	GPGPU::releaseResource(computeShaderPointer);
	GPGPU::releaseResource(shaderCodePointer);
	GPGPU::releaseResource(errorsPointer);
}

int Shader::runShader(UINT X, UINT Y, UINT Z)
{
	//TODO - think about a check for buffer to be on 
	if (!readyToRun) {// ready to run shall be set only by external supervisor on other buffers. It means that this shader it's the one enable in GPUDevice. 
		// add shader to context
		if (!this->getGPUDevice()->setActiveShader(this)) return -1;

		// add all buffer to context
		for (auto itv = tBufferGPUSlots.begin(); itv != tBufferGPUSlots.end(); itv++) {
			itv->second->setView();
		}
		// add all buffer to context
		for (auto itv = uBufferGPUSlots.begin(); itv != uBufferGPUSlots.end(); itv++) {
			itv->second->setView();
		}

		// add all constant buffer to context - i'm only one to manage them. 
		for (auto itv = cBufferGPUSlots.begin(); itv != cBufferGPUSlots.end(); itv++) {
			this->getGPUDevice()->getContext()->CSSetConstantBuffers(itv->first, 1, &itv->second);
		}

	}
	// dispatch 

	this->getGPUDevice()->getContext()->Dispatch(X, Y, Z);

	return 0;
}

GPUDevice* const Shader::getGPUDevice() const
{
	return device;
}

void Shader::getLastErrors(const char*& messagePointer) const
{
	if (errorsPointer != NULL) {
		messagePointer = reinterpret_cast<const char*>(this->errorsPointer->GetBufferPointer());
	}
	else
		messagePointer = "NO ERRORS";
	//std::memcpy((void*)messagePointer, reinterpret_cast<const char*>(this->errorsPointer->GetBufferPointer()), sizeof this->errorsPointer->GetBufferPointer());
}
/// <summary>
/// Add a buffer to this shader... If it's not correct, I delete it! 
/// </summary>
/// <param name="buf"></param>
/// <returns></returns>
unsigned short Shader::addBuffer(Buffer* buf)
{
	if (buf == nullptr)
		return -1;
	// add to context! 
	if (buf->isEnabled())
		switch (buf->getViewType())
		{
		case BUFFER_VIEW_TYPE::SRV:
			if(tBufferGPUSlots.insert(std::pair<unsigned short, Buffer*>(buf->getGPUslot(), buf)).second)
				return buf->getGPUslot();
		case BUFFER_VIEW_TYPE::UAV:
			if (uBufferGPUSlots.insert(std::pair<unsigned short, Buffer*>(buf->getGPUslot(), buf)).second)
				return buf->getGPUslot();
		default:
			break;
		}
	delete buf;
	return -1;
}

bool Shader::removeConstantBuffer(unsigned short GPUslot)
{
	if (cBufferGPUSlots.find(GPUslot) == cBufferGPUSlots.end()) return false;
	GPGPU::releaseResource(cBufferGPUSlots[GPUslot]);
	cBufferGPUSlots.erase(GPUslot);
	return true;
}

ID3D11ComputeShader* const Shader::getcomputeShaderPointer() const
{
	return this->computeShaderPointer;
}

bool Shader::isAvailable() const
{
	return isLoaded;
}
