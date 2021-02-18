#pragma once
#ifndef Buffer_H
#define Buffer_H
#include <d3d11_4.h>
#include "GPGPU.h"
#include "Shader.h"
#pragma message ("Padre")


/// <summary>
/// This class contains all useful interfaces to communicate with GPU buffers. 
/// </summary>
class Buffer
{
public:
	Buffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t length, size_t byteDimension, unsigned short GPUslot);


	~Buffer();

protected:
	/// <summary>
	/// GPU slot of buffer. if 255 it means it is not set. 
	/// https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
	/// </summary>
	unsigned short GPUslot;

	ID3D11Buffer* bufferPointer = NULL;

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
#endif