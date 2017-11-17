
#ifndef ONLINE_Future_H
#define ONLINE_Future_H

#include <assert.h>
#include <functional>
#include <list>
#include <unordered_map>

namespace online
{
	struct Future
	{
	public:
        static const int REPEAT_FOREVER;
		typedef std::function<void(void)> Callback;

	public:
		Future(float time, Callback callback, int repeat = 0);

        void cancel();
		bool update(float dt);
	private:
        float m_time;
        float m_original;
		Callback m_callback;
        bool m_cancelled;
        int m_repeat;

	};

	class Futures
	{
	public:

        Futures() :
            m_nextId(1)
        {}
        
		~Futures()
		{}

        void cancel(int futureId);
		int add(float time, Future::Callback callback, int repeat = 0);
		void postNextUpdate(Future::Callback callback);

		void update(float dt);
        void clear();
        
	private:
		std::unordered_map<int, Future> m_futures;
		std::list<Future::Callback> m_nextUpdate;
        int m_nextId;
	};
};

#endif
