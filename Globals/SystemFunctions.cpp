//TODO: verify that compiling this file results in calls to these functions being overriden by functions at 
//listed memory addresses (instead of linking in libc.a implementations)

#include "Memory.h"

#define system_memmove ((void* const (*)(void* dest, const void* source, size_t size)) 0x803f602c)
#define system_memset ((void* (*)(void* data, int value, size_t size)) 0x8000443c)
#define system_memcmp ((int (*)(const void* a, const void* b, size_t size)) 0x803f6150)

#define system_strcat ((char* (*)(char* destination, const char* source)) 0x803fa384)
#define system_strcpy ((char* (*)(char* destination, const char* source)) 0x803fa280)

//used anywhere?
#define system_strcpylen ((char* (*)(char* destination, const char* source, int size)) 0x803fa340)
//used anywhere?
#define system_sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define system_strstr ((char* (*)(const char* buffer, const char* target)) 0x803fa798)
#define system_strcmp ((int (*)(const char* str1, const char* str2)) 0x803fa3fc)

#define system_atoi ((int (*)(const char* buffer)) 0x803fc704)
#define system_atof ((float (*)(const char* buffer)) 0x803fbbf8)


extern "C" void* memmove(void* dest, const void* source, size_t size) {
	return system_memmove(dest, source, size);
}

extern "C" void* memcpy(void* dest, const void* source, size_t size) {
	return system_memmove(dest, source, size);
}

extern "C" void* memset(void* data, int value, size_t size) {
	return system_memset(data, value, size);
}

//returns <0 if a < b, 0 if a == b
extern "C" int memcmp(const void* a, const void* b, size_t size) {
    return system_memcmp(a, b, size);
}

extern "C" void* malloc(size_t size) {
	return allocFromExpHeap(mainHeap, size, 4);
}

extern "C" void free(void* ptr) {
	freeToExpHeap(mainHeap, ptr);
}

extern "C" char* strcat(char* destination, const char* source){
	return system_strcat(destination, source);
}

extern "C" char* strcpy(char* destination, const char* source){
	return system_strcpy(destination, source);
}

extern "C" char* strstr(const char* buffer, const char* target){
	return system_strstr(buffer, target);
}

extern "C" int atoi( const char* buffer){
	return system_atoi( buffer );
}

extern "C" float atof( const char* buffer){
	return system_atof( buffer );
}

//not sure how to pass var number of arguments through to system_sprintf
// extern "C" int sprintf( char* buffer, const char* format, ... ){
// 	return system_sprintf( buffer, format, ... );
// }