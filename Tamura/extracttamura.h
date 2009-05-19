#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

// imagenamefile should be utf-8 encoded if containing Chinese characters.
extern "C" DLL_EXPORT int extractTamura(const char* imagenamefile, int n, double* feature);
