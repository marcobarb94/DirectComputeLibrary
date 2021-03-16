#pragma once
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <memory>
// this class contains all methods to setup and use Compute Shader on Direct X 11 

// enum for behaviour during computation
enum class CPU_IO { WRITE, NONE}; 
enum class GPU_IO { READ, READ_N_WRITE/*, APPEND, CONSUME*/ };//SRV or UAV
enum class BUFFER_VIEW_TYPE{SRV,UAV,NONE};
class GPGPU
{
public:

	/// <summary>
/// release resource and return number of links(the value does not have to be accurate)
/// </summary>
/// <param name="resource"></param>
/// <returns></returns>
	static ULONG releaseResource (IUnknown* resource);
	/// <summary>
	/// Return a text error instead of HRESULT
	/// </summary>
	/// <param name="result">Print using std::wcout instead of std::cout </param>
	/// <returns></returns>
	static std::unique_ptr<TCHAR[]> resolveHRESULT(HRESULT result);


};


