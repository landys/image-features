#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

// 7-dimension vector, only jpg file support
extern "C" DLL_EXPORT int extractMICanny(const char* imagenamefile, double* feature);
