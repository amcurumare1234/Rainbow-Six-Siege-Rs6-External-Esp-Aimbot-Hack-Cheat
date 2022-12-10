/**
 * Rainbow Six: Siege SDK
 * Copyright (C) 2017 RangeMachine
 */

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
