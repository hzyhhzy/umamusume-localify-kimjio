#pragma once

#define NOMINMAX

#include <Windows.h>

#include <functional>
#include <vector>
#include <string>
#include <filesystem>

#include "il2cpp-object-internals.h"

template<typename T>
struct MethodInfo_t
{
	T methodPointer;
	void* (*invoker_method)(T, const MethodInfo_t<T>*, void*, void**);
	const char* name;
	Il2CppClass* klass;
	const Il2CppType* return_type;
	const ParameterInfo* parameters;

	union
	{
		const Il2CppRGCTXData* rgctx_data; /* is_inflated is true and is_generic is false, i.e. a generic instance method */
		Il2CppMetadataMethodDefinitionHandle methodMetadataHandle;
	};

	/* note, when is_generic == true and is_inflated == true the method represents an uninflated generic method on an inflated type. */
	union
	{
		const Il2CppGenericMethod* genericMethod; /* is_inflated is true */
		Il2CppMetadataGenericContainerHandle genericContainerHandle; /* is_inflated is false and is_generic is true */
		Il2CppMethodPointer nativeFunction; /* if is_marshaled_from_native is true */
	};

	uint32_t token;
	uint16_t flags;
	uint16_t iflags;
	uint16_t slot;
	uint8_t parameters_count;
	uint8_t is_generic : 1; /* true if method is a generic method definition */
	uint8_t is_inflated : 1; /* true if declaring_type is a generic instance or if method is a generic instance*/
	uint8_t wrapper_type : 1; /* always zero (MONO_WRAPPER_NONE) needed for the debugger */
	uint8_t is_marshaled_from_native : 1; /* a fake MethodInfo wrapping a native function pointer */
};

template<typename T>
struct Il2CppArraySize_t : public Il2CppArray
{
	ALIGN_TYPE(8) T vector[IL2CPP_ZERO_LEN_ARRAY];
};

template<typename T>
struct Il2CppDelegate_t
{
	Il2CppObject object;
	/* The compiled code of the target method */
	T method_ptr;
	/* The invoke code */
	InvokerMethod invoke_impl;
	Il2CppObject* target;
	const MethodInfo* method;

	void* delegate_trampoline;

	intptr_t extraArg;

	/*
	 * If non-NULL, this points to a memory location which stores the address of
	 * the compiled code of the method, or NULL if it is not yet compiled.
	 */
	uint8_t** method_code;
	Il2CppReflectionMethod* method_info;
	Il2CppReflectionMethod* original_method_info;
	Il2CppObject* data;

	bool method_is_virtual;
};

template <typename T>
struct TypedField
{
	FieldInfo* Field;

	constexpr FieldInfo* operator->() const noexcept
	{
		return Field;
	}
};

#define DO_API(r, n, p) extern r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

// char* il2cpp_array_addr_with_size(void* arr, int32_t size, uintptr_t idx);

// array macro
#define il2cpp_array_addr(array, type, index) reinterpret_cast<type*>(il2cpp_array_addr_with_size(array, sizeof(type), index))

#define il2cpp_array_setref(array, index, value)  \
    do \
	{ \
        auto __p = il2cpp_array_addr (array, void*, index); \
         *__p = (value); \
    } \
	while (0)

#define il2cpp_array_setref_type(array, type, index, value)  \
    do \
	{ \
        auto __p = il2cpp_array_addr (array, type, index); \
         *__p = (value); \
    } \
	while (0)

#define il2cpp_array_setref_type_memmove(array, type, index, value)  \
    do \
	{ \
        auto __p = il2cpp_array_addr (array, type, index); \
        memmove(__p, (value), sizeof(type)); \
    } \
	while (0)

extern Il2CppDefaults il2cpp_defaults;

constexpr int IgnoreNumberOfArguments = -1;

Il2CppString* il2cpp_string_new16(const wchar_t* value);

FieldInfo* il2cpp_class_get_field_from_name_wrap(Il2CppClass* klass, const char* name);

std::string il2cpp_fn_name(const char* name);

namespace il2cpp_symbols
{
	extern Il2CppDomain* il2cpp_domain;

	extern std::vector<std::function<void()>> init_callbacks;

	void load_symbols(std::filesystem::path& path);

	void init(HMODULE game_module);

	void call_init_callbacks();

	void init_defaults();

	Il2CppClass* get_class(const char* assemblyName, const char* namespaze, const char* klassName);

	Il2CppMethodPointer get_method_pointer(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount);

	template<typename T>
	T get_method_pointer(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount) {
		return reinterpret_cast<T>(get_method_pointer(assemblyName, namespaze, klassName, name,
			argsCount));
	}

	const MethodInfo* get_method(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount);

	template<typename T>
	const MethodInfo_t<T>* get_method(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name, int argsCount)
	{
		return reinterpret_cast<const MethodInfo_t<T>*>(get_method(assemblyName, namespaze, klassName, name, argsCount));
	}

	FieldInfo* get_field(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name);

	template <typename T>
	TypedField<T> get_field(const char* assemblyName, const char* namespaze,
		const char* klassName, const char* name)
	{
		return { get_field(assemblyName, namespaze, klassName, name) };
	}

	Il2CppClass* get_class_from_instance(const void* instance);

	template <typename T = void*> requires std::is_trivial_v<T>
	T read_field(const void* ptr, const FieldInfo* field)
	{
		T result;
		const auto fieldPtr = static_cast<const std::byte*>(ptr) + field->offset;
		std::memcpy(std::addressof(result), fieldPtr, sizeof(T));
		return result;
	}

	template <typename T>
	T read_field(const void* ptr, TypedField<T> field)
	{
		return read_field<T>(ptr, field.Field);
	}

	template <typename T> requires std::is_trivial_v<T>
	void write_field(void* ptr, const FieldInfo* field, const T& value)
	{
		const auto fieldPtr = static_cast<std::byte*>(ptr) + field->offset;
		std::memcpy(fieldPtr, std::addressof(value), sizeof(T));
	}

	template <typename T, typename U>
	void write_field(void* ptr, TypedField<T> field, U&& value)
	{
		write_field<T>(ptr, field.Field, static_cast<T>(std::forward<U>(value)));
	}

	template <typename T = void*>
	void iterate_list(const void* list, std::invocable<int32_t, T> auto&& receiver)
	{
		const auto listClass = get_class_from_instance(list);
		const auto getItemMethod = reinterpret_cast<T(*)(const void*, int32_t)>(il2cpp_class_get_method_from_name(listClass, "get_Item", 1)->methodPointer);
		const auto getCountMethod = reinterpret_cast<int32_t(*)(const void*)>(il2cpp_class_get_method_from_name(listClass, "get_Count", 0)->methodPointer);

		const auto count = getCountMethod(list);
		for (int32_t i = 0; i < count; ++i)
		{
			static_cast<decltype(receiver)>(receiver)(i, getItemMethod(list, i));
		}
	}

	template <typename T = void*>
	void iterate_IEnumerable(const void* obj, std::invocable<T> auto&& receiver)
	{
		const auto klass = get_class_from_instance(obj);
		const auto getEnumeratorMethod = reinterpret_cast<void* (*)(const void*)>(il2cpp_class_get_method_from_name(klass, "GetEnumerator", 0)->methodPointer);
		const auto enumerator = getEnumeratorMethod(obj);
		const auto enumeratorClass = get_class_from_instance(enumerator);
		const auto getCurrentMethod = reinterpret_cast<T(*)(void*)>(il2cpp_class_get_method_from_name(enumeratorClass, "get_Current", 0)->methodPointer);
		const auto moveNextMethod = reinterpret_cast<bool(*)(void*)>(il2cpp_class_get_method_from_name(enumeratorClass, "MoveNext", 0)->methodPointer);

		while (moveNextMethod(enumerator))
		{
			static_cast<decltype(receiver)>(receiver)(getCurrentMethod(enumerator));
		}
	}

	Il2CppString* NewWStr(std::wstring_view str);


	const Il2CppClass* find_class(const char* assemblyName, const char* namespaze,
		const std::function<bool(const Il2CppClass*)>& predict);

	Il2CppMethodPointer find_method(const char* assemblyName, const char* namespaze,
		const char* klassName,
		const std::function<bool(const MethodInfo*)>& predict);

	template<typename T>
	T find_method(const char* assemblyName, const char* namespaze,
		const char* klassName,
		const std::function<bool(const MethodInfo*)>& predict)
	{
		return reinterpret_cast<T>(find_method(assemblyName, namespaze, klassName, predict));
	}
}

#include "il2cpp-api-functions.hpp"
