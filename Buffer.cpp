#include "Buffer.h"

Buffer::Buffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t length, size_t byteDimension, unsigned short GPUslot)
{
	this->shader = shader;
	this->CPURequirement = CPURequirement;
	this->GPURequirement = GPURequirement;
	this->shader = shader;
	this->length = length;
	this->elementDimension = byteDimension;
	this->GPUslot = GPUslot;
}


Buffer::~Buffer()
{
	// TODO
	GPGPU::releaseResource(bufferPointer);
}

HRESULT Buffer::createShaderResourceView()
{
	return E_NOTIMPL;
}

HRESULT Buffer::createUnorderedAccessView()
{
	return E_NOTIMPL;
}

HRESULT Buffer::createStructuredBuffer(void* dataPtr)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = (this->CPURequirement==CPU_IO::READ_N_WRITE)?D3D11_BIND_UNORDERED_ACCESS : D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = this->length * this->elementDimension; // size in byte
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = this->elementDimension; // number of elements

	if (dataPtr)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = dataPtr;
		return this->shader->getGPUDevice()->getDevice()->CreateBuffer(&desc, &InitData, &bufferPointer);
	}
	else
		return this->shader->getGPUDevice()->getDevice()->CreateBuffer(&desc, NULL, &bufferPointer);

}

HRESULT Buffer::createRWStructuredBuffer(void* dataPtr)
{
	return E_NOTIMPL;
}
