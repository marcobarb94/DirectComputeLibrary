#pragma once
#ifndef IOBuffer_H
#define IOBuffer_H
#include "Buffer.h"

/// <summary>
/// This class contains all useful interfaces to communicate with GPU buffers. 
/// </summary>
class Buffer
{
public:
	Buffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t length, size_t byteDimension, unsigned short GPUslot);
	~Buffer();
	unsigned short getGPUslot() const;
	bool isEnabled() const;

	BUFFER_VIEW_TYPE getViewType() const;

	void setView();

	void removeView();
protected:
	/// <summary>
	/// GPU slot of buffer. if 255 it means it is not set. 
	/// https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
	/// </summary>
	unsigned short GPUslot;

	ID3D11Buffer* bufferPointer = nullptr;
	ID3D11ShaderResourceView* SRVpointer = nullptr;
	ID3D11UnorderedAccessView* UAVpointer = nullptr;


	Shader* shader;
	CPU_IO CPURequirement;
	GPU_IO GPURequirement;
	/// <summary>
	/// number of elements
	/// </summary>
	size_t length;
	/// <summary>
	/// byte per element
	/// </summary>
	size_t elementDimension;
	/// <summary>
	/// buffer working?
	/// </summary>
	bool enabled = false;

	BUFFER_VIEW_TYPE type = BUFFER_VIEW_TYPE::NONE;

	bool viewLoaded = false;

#pragma region Resource Functions
	//https://docs.microsoft.com/en-us/windows/win32/direct3d11/direct3d-11-advanced-stages-cs-resources
#pragma region Views

	HRESULT createShaderResourceView();

	HRESULT createUnorderedAccessView();
#pragma endregion

#pragma region GPU buffers
	/// <summary>
	/// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/sm5-object-structuredbuffer
	/// </summary>
	/// <returns></returns>
	HRESULT createStructuredBuffer(void* dataPtr);
	/// <summary>
	/// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/sm5-object-rwstructuredbuffer
	/// </summary>
	/// <returns></returns>
	HRESULT createRWStructuredBuffer(void* dataPtr);
	/// <summary>
	/// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/sm5-object-appendstructuredbuffer
	/// </summary>
	/// <returns></returns>
	HRESULT createAppendStructuredBuffer();
	/// <summary>
	/// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/sm5-object-consumestructuredbuffer
	/// </summary>
	/// <returns></returns>
	HRESULT createConsumeStructuredBuffer();
#pragma endregion
#pragma endregion
};
template <typename T>
class IOBuffer : public Buffer
{
public:
	IOBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t size, unsigned short GPUslot);

	IOBuffer* create(T* array = NULL);
	/// <summary>
	/// https://docs.microsoft.com/en-us/windows/win32/direct3d11/how-to--use-dynamic-resources
	/// </summary>
	/// <param name="array"></param>
	void updateBuffer(T* array = NULL);

	T* readBuffer();
};

template<typename T>
IOBuffer<T>::IOBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t size, unsigned short GPUslot)
	:Buffer(CPURequirement, GPURequirement, shader, size, sizeof T, GPUslot)
{
}
/// <summary>
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns>Don't worry about delete :wink</returns>
template<typename T>
T* IOBuffer<T>::readBuffer()
{
	//if (this->type==BUFFER_VIEW_TYPE::UAV)
	{
		//TODO
		ID3D11Buffer* newBufferCopy = NULL;
		// build copy buffer
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		this->bufferPointer->GetDesc(&desc);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;
		HRESULT createBuff = this->shader->getGPUDevice()->getDevice()->CreateBuffer(&desc, NULL, &newBufferCopy);
		if (createBuff != S_OK) return nullptr;
		// copy resource in new buffer - i cannot use the GPU active buffer :(
		this->shader->getGPUDevice()->getContext()->CopyResource(newBufferCopy, this->bufferPointer);
		//read
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		T* pointer;
		this->shader->getGPUDevice()->getContext()->Map(newBufferCopy, 0, D3D11_MAP_READ, 0, &MappedResource);
		pointer = (T*)MappedResource.pData;
		// clear 
		GPGPU::releaseResource(newBufferCopy);
		return pointer;
	}
	/*else
		return nullptr;*/
}
template<typename T>
IOBuffer<T>* IOBuffer<T>::create(T* array)
{
	if (this->enabled)
		return nullptr;
	HRESULT res = S_FALSE;

	switch (Buffer::GPURequirement)
	{
	case GPU_IO::READ:
		res = Buffer::createStructuredBuffer(array);
		if (res == S_OK)
			res = Buffer::createShaderResourceView();
		break;
	case GPU_IO::READ_N_WRITE:
		res = Buffer::createRWStructuredBuffer(array);
		if (res == S_OK)
			res = Buffer::createUnorderedAccessView();
		break;
	default:
		break;
	}
	Buffer::enabled = res == S_OK;
	return this;
}
#endif