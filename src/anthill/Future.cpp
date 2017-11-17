
#include "anthill/Future.h"
#include <algorithm>

namespace online
{
    const int Future::REPEAT_FOREVER = -1;

	Future::Future(float time, Callback callback, int repeat) :
			m_time(time),
            m_original(time),
			m_callback(callback),
            m_repeat(repeat)
	{
		//
	}

	bool Future::update(float dt)
	{
		m_time -= dt;
		if (m_time <= 0)
		{
			m_callback();
            if (m_repeat)
            {
                m_time = m_original;
                
                if (m_repeat < 0)
                    return false;
                
                m_repeat--;
                return m_repeat > 0;
            }
            
			return true;
		}

		return false;
	}
    
    void Futures::cancel(int futureId)
    {
        m_futures.erase(futureId);
    }

	int Futures::add(float time, Future::Callback callback, int repeat)
	{
        int id = m_nextId++;
    
        m_futures.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(time, callback, repeat));
    
        return id;
	}

	void Futures::postNextUpdate(Future::Callback callback)
	{
		m_nextUpdate.push_back(callback);
	}
	
    void Futures::clear()
    {
        m_nextUpdate.clear();
        m_futures.clear();
    }

	void Futures::update(float dt)
	{
        for (std::unordered_map<int, Future>::iterator it = m_futures.begin(); it != m_futures.end(); )
        {
            if (it->second.update(dt))
            {
                it = m_futures.erase(it);
            }
            else
            {
                ++it;
            }
        }

		if (!m_nextUpdate.empty())
		{
			for (Future::Callback callback : m_nextUpdate)
			{
				callback();
			}

			m_nextUpdate.clear();
		}
	}
}
