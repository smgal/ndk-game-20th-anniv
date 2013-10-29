
#ifndef __YUNJR_BASE_H__
#define __YUNJR_BASE_H__

#include "yunjr_z_config.h"

#include "../flat_board/flat_board.h"
#include "../flat_board/target_dep.h"

#include <cassert>
#include <vector>
#include <utility>

namespace yunjr
{
	struct BufferDesc
	{
		void* p_start_address;
		int   width;
		int   height;
		int   bytes_per_line;
		int   bits_per_pixel;
	};

	void init(const char* sz_id);
	void done(void);
	bool loop(const BufferDesc& buffer_desc);
}

namespace yunjr
{
	typedef flat_board::FlatBoard<flat_board::PIXELFORMAT_ARGB8888> FlatBoard32;
	using flat_board::Color32;

	typedef unsigned long TargetPixel;

	template<typename BaseType>
	class auto_ptr
	{
	public:
		explicit auto_ptr(BaseType* ptr = 0)
			: m_owns(ptr != 0)
			, m_ptr((BaseType*)ptr)
		{
		}
		~auto_ptr()
		{
			if (m_owns)
				delete (BaseType*)m_ptr;
		}
		void bind(BaseType* ptr)
		{
			assert(m_ptr == 0);

			m_owns = (ptr != 0);
			m_ptr  = (BaseType*)ptr;
		}
		BaseType* operator ->() const
		{
			return m_ptr;
		}
		BaseType* get() const
		{
			return m_ptr;
		}
		BaseType* release() const
		{
			((auto_ptr<BaseType>*)this)->m_owns = false;
			return m_ptr;
		}

	private:
		bool m_owns;
		BaseType* m_ptr;
	};

	template<typename BaseType>
	class auto_ptr<BaseType[]>
	{
	public:
		explicit auto_ptr(BaseType* ptr = 0)
			: m_owns(ptr != 0)
			, m_ptr((BaseType*)ptr)
		{
		}
		~auto_ptr()
		{
			if (m_owns)
				delete[] (BaseType*)m_ptr;
		}
		BaseType& operator[](unsigned int index) const
		{
			return m_ptr[index];
		}
		void bind(BaseType* ptr)
		{
			assert(m_ptr == 0);

			m_owns = (ptr != 0);
			m_ptr  = (BaseType*)ptr;
		}
		void reset(BaseType* ptr = 0)
		{
			if (m_owns)
				delete[] (BaseType*)m_ptr;

			m_owns = (ptr != 0);
			m_ptr  = (BaseType*)ptr;
		}
		BaseType* get() const
		{
			return m_ptr;
		}
		BaseType* release() const
		{
			((auto_ptr<BaseType[]>*)this)->m_owns = false;
			return m_ptr;
		}

	private:
		bool m_owns;
		BaseType* m_ptr;
	};

	template<typename BaseType>
	class shared_ptr
	{
	protected:
		BaseType* m_p_instance;
		unsigned int* m_use_count;

	public:
		shared_ptr()
			: m_p_instance(0)
			, m_use_count(0)
		{
		}
		explicit shared_ptr(BaseType* p_instance)
			: m_p_instance(p_instance)
			, m_use_count(new unsigned int(1))
		{
		}
		shared_ptr(const shared_ptr& ref_instance)
			: m_p_instance(0)
			, m_use_count(0)
		{
			m_p_instance = ref_instance.m_p_instance;
			m_use_count  = ref_instance.m_use_count; 

			if (m_use_count)
				++(*m_use_count); 
		}
		virtual ~shared_ptr()
		{
			release();
		}

		shared_ptr& operator=(const shared_ptr& ref_instance)
		{
			if (m_p_instance == ref_instance.m_p_instance)
				return *this;

			release();

			{
				m_p_instance = ref_instance.m_p_instance;
				m_use_count  = ref_instance.m_use_count;
				if (m_use_count)
					++(*m_use_count);
			}

			return *this;
		}

		inline BaseType& operator*() const
		{
			assert(m_p_instance);

			return *m_p_instance;
		}
		inline BaseType* operator->() const
		{
			assert(m_p_instance);

			return m_p_instance;
		}
		inline BaseType* get() const
		{
			return m_p_instance;
		}

		void bind(BaseType* rep)
		{
			assert(!m_p_instance && !m_use_count);

			m_use_count  = new unsigned int(1);
			m_p_instance = rep;
		}

		inline bool unique() const
		{
			assert(m_use_count);

			return (*m_use_count == 1);
		}
		inline unsigned int useCount() const
		{
			assert(m_use_count);

			return *m_use_count;
		}
		inline unsigned int* useCountPointer() const
		{
			return m_use_count;
		}

		inline BaseType* getPointer() const
		{
			return m_p_instance;
		}

		inline bool isNull(void) const
		{
			return (m_p_instance == 0);
		}

		inline void setNull(void)
		{ 
			if (m_p_instance)
			{
				release();
				m_p_instance = 0;
				m_use_count  = 0;
			}
		}

	protected:
		inline void release(void)
		{
			if (m_use_count)
				if (--(*m_use_count) == 0) 
					destroy();
		}

		virtual void destroy(void)
		{
			delete m_p_instance;
			delete m_use_count;
		}
	};

	template<typename TBaseType1, typename TBaseType2> inline bool operator==(shared_ptr<TBaseType1> const& a, shared_ptr<TBaseType2> const& b)
	{
		return a.get() == b.get();
	}

	template<typename TBaseType1, typename TBaseType2> inline bool operator!=(shared_ptr<TBaseType1> const& a, shared_ptr<TBaseType2> const& b)
	{
		return a.get() != b.get();
	}

	template <typename T>
	void deleteVector(std::vector<T*>& container)
	{
		typename std::vector<T*>::iterator item = container.begin();

		for ( ; item != container.end(); ++item)
			delete *item;

		container.clear();
	}

	template <typename T1, typename T2>
	void deleteVectorForPair(std::vector<std::pair<T1, T2*> >& container)
	{
		typename std::vector<std::pair<T1, T2*> >::iterator item = container.begin();

		for ( ; item != container.end(); ++item)
			delete item->second;

		container.clear();
	}

	template <typename T, typename Param1>
	struct Operator
	{
		Param1 param;

		Operator(Param1 in_param)
			: param(in_param) {	}

		void operator()(T p_obj);
	};
}

#endif // #ifndef __YUNJR_BASE_H__
