#pragma once

#include<assert.h>
#include <stdint.h>
#include <memory>



class LinearMemory
{
public:
	

	inline size_t GetMemorySize() const 
	{
		return m_current_memory_size;
	}

	inline void ReserveMemory(size_t _newMemorySize) 
	{
		m_current_memory_size = _newMemorySize;
		m_buffer = std::make_unique<uint8_t[]>(m_current_memory_size);
	}

	inline uint8_t* SetToMemory(size_t _size) 
	{
		if (m_current_offset + _size <= m_current_memory_size)
		{
			uint8_t* ret = &m_buffer[m_current_offset];
			m_current_offset += _size;
			return ret;
		}
		return nullptr;
	}

	inline void FreeMemory(size_t _size) 
	{
		assert(m_current_offset >= _size,"NO ENOUGH MEMORY TO FREE");
		m_current_offset -= _size;
	}

	inline void ResetAllMemory() 
	{
		m_current_offset = 0;
	}


private:
	std::unique_ptr<uint8_t[]> m_buffer;
	size_t m_current_memory_size = 0;
	size_t m_current_offset = 0;
};

