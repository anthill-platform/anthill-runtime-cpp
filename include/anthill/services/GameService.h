
#ifndef ONLINE_Game_Service_H
#define ONLINE_Game_Service_H

#include "anthill/services/Service.h"
#include "anthill/requests/Request.h"
#include "anthill/ApplicationInfo.h"

#include <json/value.h>
#include "DiscoveryService.h"

namespace online
{
	typedef std::shared_ptr< class GameService > GameServicePtr;

	class AnthillRuntime;

	class GameService : public Service
	{
		friend class AnthillRuntime;

	public:
		static const std::string ID;
        static const std::string API_VERSION;
        
        typedef struct JoinResult_
        {
            std::string roomId;
            std::string key;
            std::string host;
            std::vector<int> ports;
            Json::Value settings;
        } JoinResult;

		typedef std::function< void(const GameService&, Request::Result result, const Request& request,
			const JoinResult& joinResult) > JoinRoomCallback;

	public:
		static GameServicePtr Create(const std::string& location);
		virtual ~GameService();

        void joinRoom(const std::string& roomId, const std::string& accessToken, JoinRoomCallback callback);
        
        void joinRoom(const std::string& accessToken, JoinRoomCallback callback, const std::string& gameServer, const Json::Value& settings, const Json::Value& createSettings, bool autoCreate, bool myRegionOnly = true);
        void joinRoom(const std::string& accessToken, JoinRoomCallback callback, const std::string& gameServer, bool autoCreate, bool myRegionOnly = true);

	protected:
		GameService(const std::string& location);
		bool init();

	private:
	};
};

#endif
