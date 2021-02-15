#pragma once
#include "GPUDevice.h"
#include <d3d11_4.h>
#include <map>
#include "GPGPU.h"
//#include "Buffer.h"

namespace DCL {
	class Shader
	{

	public:

		HRESULT compileFromFileAndLoad(LPCWSTR fileName, LPCSTR startupFunctionName);

		HRESULT loadCompiledShader(LPCWSTR fileName);

		Shader(GPUDevice device);


		template <typename T> unsigned short addBuffer(CPUGPU_IO CPURequirement, CPUGPU_IO GPURequirement, size_t size, unsigned short GPUslot = 255, T* data = NULL);


		bool removeBuffer(unsigned short GPUslot);


		~Shader();

		int runShader(UINT X, UINT Y, UINT Z);

		/// <summary>
		/// Return the Device 
		/// </summary>
		/// <returns></returns>
		GPUDevice getDevice();

	private:
		bool isLoaded = false;
		GPUDevice device;
		/// <summary>
		/// GPU slots that are not free to use.  
		/// </summary>
		std::map<unsigned short, void*> GPUSlots;

		ID3D11ComputeShader* computeShaderPointer = NULL;

	};


}
