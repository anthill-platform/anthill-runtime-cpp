
#ifndef ONLINE_DiscoverServices_State_H
#define ONLINE_DiscoverServices_State_H

#include "State.h"
#include "../services/DiscoveryService.h"

#include <set>

namespace online
{
	class StateMachine;

	class DiscoverServicesState: public State
	{
		friend class StateMachine;

	public:
		DiscoverServicesState(std::shared_ptr<StateMachine> stateMachine, const std::set<std::string>& services);
		virtual ~DiscoverServicesState();

		virtual std::string ID() override { return "DiscoverServicesState"; }

	protected:
		virtual void init() override;
		virtual void release() override;

	private:
		void proceed();

	private:
		void discoverServices(const DiscoveryService&, Request::Result result, const Request& request, const DiscoveredServices& services);

	private:
		std::set<std::string> m_services;
	};
};

#endif