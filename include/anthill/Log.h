
#ifndef ONLINE_Log_H
#define ONLINE_Log_H

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <functional>

namespace online
{
	class Log {
	private:
		std::ostringstream oss;
		std::function<void(const std::string&)> callback;
	public:

		static Log& get()
		{
			static Log instance;
			return instance;
		}

		void overrideLog(std::function<void(const std::string&)> _callback)
		{
			this->callback = _callback;
		}

		template <typename T>
		Log& operator<<(T a)
		{
			oss << a;
			return *this;
		}

		Log& operator<<(std::ostream&(*f)(std::ostream&))
		{
            std::ostream& (*pEndl)(std::ostream &) = &(std::endl);
            
			if (f == pEndl)
			{
				if (callback)
				{
					callback(oss.str());
				}
				else
				{
					time_t t = std::time(nullptr);
                    char foo[32];
                    
					if(0 < strftime(foo, sizeof(foo), "[%T%z %F] ", std::localtime(&t)))
                    {
                        std::cout << "[ *********** ONLINE *********** ] " << foo << " " << oss.str() << std::endl;
                    }
				}

				oss.str("");
			}
			return *this;
		}
	};
};

#endif
