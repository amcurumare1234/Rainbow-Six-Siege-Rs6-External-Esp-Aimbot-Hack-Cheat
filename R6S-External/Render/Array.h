//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#pragma once
#include <cstdint>
#include "Memory.h"

namespace Engine
{
    template <typename T>
	class Array
    {
		private:
			T* m_pBuffer;
			uint64_t m_size;

		public:
			uint32_t GetSize()
			{
				return m_size;
			}

			const T& operator [](uint64_t i)
			{
				if (Memory::IsValidPtr<T>(m_pBuffer))
					return m_pBuffer[i];

				return nullptr;
			}
    };
}
