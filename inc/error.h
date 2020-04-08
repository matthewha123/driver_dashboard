
#if __cplusplus
extern "C" void _Error_Handler(char const *file, int line);
#else
void _Error_Handler(char const *file, int line);
#endif
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

