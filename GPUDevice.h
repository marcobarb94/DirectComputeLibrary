#pragma once
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <map>

//#include <d3dx11.h> // non c'è più in W10 - how to survive: https://walbourn.github.io/living-without-d3dx/
namespace DCL {
	class GPUDevice
	{
	public:
		D3D11_CREATE_DEVICE_FLAG flags = D3D11_CREATE_DEVICE_DEBUG;
		/// <summary>
		/// Constructor - set feature(s) level
		/// </summary>
		/// <param name="featureLevel"></param>
		GPUDevice(D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1);
		/// <summary>
		/// Create a device to run something on
		/// </summary>
		/// <returns>
		/// 	 0->OK
		/// 	 -1->error on create
		/// 	 -2->already created on this object
		/// 	 </returns>
		int create();
		/// <summary>
		/// a safe way to destroy the device - destructor
		/// </summary>
		~GPUDevice();
		/// <summary>
		/// Add a compiled shader to this device 
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns></returns>
		unsigned short addCompiledShader(LPCWSTR fileName);
		/// <summary>
		/// Compile and add a shader to this device 
		/// </summary>
		/// <param name="fileName"></param>
		/// <param name="startupFunctionName"></param>
		/// <returns></returns>
		unsigned short compileAndAddShader(LPCWSTR fileName, LPCSTR startupFunctionName);
		/// <summary>
		/// Return the device 
		/// </summary>
		/// <returns></returns>
		ID3D11Device* getDevice(); 
	private:
		ID3D11Device* device = NULL;
		ID3D11DeviceContext* context = NULL;
		D3D_FEATURE_LEVEL featureLevel;
		bool isEnabled = false;

		//std::map <unsigned short, Shader*> loadedShaders;

		/// <summary>
		/// release resource and return number of links(the value does not have to be accurate)
		/// </summary>
		/// <param name="resource"></param>
		/// <returns></returns>
		static ULONG releaseResource(IUnknown* resource);
	};
}

