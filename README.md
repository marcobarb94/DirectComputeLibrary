# Direct Compute Library

The idea is to develop a CLI library to facilitate the use of Direct Compute on DirectX 11 (in future 12). 

## Linker Input Dependences

Use them on tester too! 

kernel32.lib
user32.lib
gdi32.lib
winspool.lib
comdlg32.lib
advapi32.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
odbc32.lib
odbccp32.lib
dxguid.lib
winmm.lib
comctl32.lib
d3d11.lib
D3DCompiler.lib

- Linker subsystem: Windows

## Library Structure

- Device: define a device to run HLSL code. 
  - Shader: functions to compile and run a shader 
    - Buffer: defines buffers connected to the Shader - very easy to use!  different option since different I/O in CPU and GPU needed!

BUFFERS I/O

| I/O CPU/GPU  | READ       | READ & WRITE | APPEND | 
| ------------ | ---------- | ------------ | ------------ |
| READ         | SB + SRV   | RWSB + SRV   | ASB + SRV | 
| READ & WRITE | RWSB + UAV | RWSB + UAV   | None | 
| NONE         | SB         | RWSB         | ASB | 