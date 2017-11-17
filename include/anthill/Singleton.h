
#ifndef ONLINE_Singleton_H
#define ONLINE_Singleton_H

#include <assert.h>

namespace online
{
	template <typename T>
	class Singleton
	{
	public:
		Singleton();
		virtual ~Singleton();

		static T& Instance();
		static T* InstancePtr();
		static bool IsInstanceValid();

	protected:
		static T* m_instance;
	};

	template<typename T> T* Singleton<T>::m_instance = nullptr;

	template <typename T>
	Singleton<T>::Singleton()
	{
		m_instance = static_cast< T* >(this);
	}

	template <typename T>
	Singleton<T>::~Singleton()
	{
		m_instance = nullptr;
	}

	template <typename T>
	inline T& Singleton<T>::Instance()
	{
		assert(m_instance && "Null singleton instance");

		return (*InstancePtr());
	}

	template <typename T>
	inline T* Singleton<T>::InstancePtr()
	{
		return m_instance;
	}

	template <typename T>
	bool Singleton<T>::IsInstanceValid()
	{
		return (m_instance != nullptr);
	}
};

#endif