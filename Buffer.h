#pragma once
#include <d3d11_4.h>
#include "GPGPU.h"
#include "Shader.h"
/// <summary>
/// This class contains all useful interfaces to communicate with GPU buffers. 
/// </summary>
namespace DCL {
	template <typename T>
	class Buffer
	{
	public:
		Buffer<T>(CPUGPU_IO CPURequirement, CPUGPU_IO GPURequirement, Shader* shader, size_t size, unsigned short GPUslot);

		int loadBuffer(T* array);

		T* readBuffer();

		~Buffer();

	private:
		/// <summary>
		/// GPU slot of buffer. if 255 it means it is not set. 
		/// https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits
		/// </summary>
		unsigned short GPUslot;

		ID3D11Buffer* bufferPointer;

		Shader* shader;
		CPUGPU_IO CPURequirement;
		CPUGPU_IO GPURequirement;
		size_t size;


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
		HRESULT createStructuredBuffer();
		/// <summary>
		/// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/sm5-object-rwstructuredbuffer
		/// </summary>
		/// <returns></returns>
		HRESULT createRWStructuredBuffer();
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



}

