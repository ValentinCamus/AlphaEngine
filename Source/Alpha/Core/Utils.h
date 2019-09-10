#pragma once

#include <sstream>
#include <memory>

#include <Alpha/Core/Types.h>
#include <Alpha/Core/Logger.h>

namespace Alpha
{
	/// Cast an object (lvalue) to string.
	template<typename T>
	inline std::string ToString(const T& object)
	{
		return std::to_string(object);
	}

	/// Cast an object (rvalue) to string.
	template<typename T>
	inline std::string ToString(const T&& object)
	{
		return std::to_string(object);
	}

	/// Cast a string (lvalue) to C string format.
	inline const char * ToCharArray(const std::string& str)
	{
		return str.c_str();
	}

	/// Exit the application.
	/// @code: The returned code.
	inline void ForceQuit(int32 code = EXIT_FAILURE)
	{
		Logger::Info("ForceQuit with code : [{0}]", code);
		exit(code);
	}

	/// Exit if the condition is false.
	inline void ExitIf(bool cond)
	{
		if (cond) ForceQuit(EXIT_FAILURE);
	}

	inline void AtExitCallback()
	{
#ifdef ALPHA_QUIT_CONFIRMATION
		std::cout << std::endl; // Esthetic only
		std::cout << ">> Press any touch to quit" << std::endl;
		std::cin.get();
#endif
	}

	inline void SetAtExitCallback()
	{
		ALPHA_CHECK(std::atexit(AtExitCallback) == 0, "Failed to register AtExit");
	}

	/// Create an Alpha Pointer (aka shared_ptr).
	template<class T, class... Args>
	inline Pointer<T> NewPointer(Args&&... args) { return std::make_shared<T>(args...); }

	/// Cast an Alpha pointer to an other Alpha pointer.
	template <class T, class U>
	inline Pointer<T> Cast(const Pointer<U>& ptr) { return std::dynamic_pointer_cast<T>(ptr); }
}
