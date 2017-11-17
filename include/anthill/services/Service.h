
#ifndef ONLINE_Service_H
#define ONLINE_Service_H

#include "anthill/Singleton.h"

#include <memory>
#include <string>

namespace online
{
	typedef std::shared_ptr<class Service> ServicePtr;

	class Service
	{
	public:
		virtual ~Service() {}

		const std::string& getLocation() const;
		void setLocation(const std::string& location);

	protected:
		Service(const std::string& location);
		bool init();

	private:
		std::string m_location;
        
    public:
        virtual void update(float dt) {}
	};
};

#endif
