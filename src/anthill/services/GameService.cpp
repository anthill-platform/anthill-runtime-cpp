
#include "anthill/services/GameService.h"
#include "anthill/requests/JsonRequest.h"
#include "anthill/AnthillRuntime.h"
#include "anthill/Utils.h"

#include <json/writer.h>

namespace online
{
	const std::string GameService::ID = "game";
    const std::string GameService::API_VERSION = "0.2";

	GameServicePtr GameService::Create(const std::string& location)
	{
		GameServicePtr _object(new GameService(location));
		if (!_object->init())
			return GameServicePtr(nullptr);

		return _object;
	}

	GameService::GameService(const std::string& location) :
		Service(location)
	{
		
    }
    
    void GameService::joinRoom(const std::string& roomId, const std::string& accessToken, GameService::JoinRoomCallback callback)
	{
		const ApplicationInfo& applicationInfo = AnthillRuntime::Instance().getApplicationInfo();

        JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/room/" + applicationInfo.applicationName + "/" + roomId + "/join", Request::METHOD_POST);
        
		if (request)
        {
            request->setAPIVersion(API_VERSION);
        
            Json::FastWriter fastWriter;
            
            request->setPostFields({
                {"access_token", accessToken }
            });
            
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful())
				{
					const Json::Value& value = request.getResponseValue();

					if (value.isMember("location") && value.isMember("key") && value.isMember("id"))
					{
                        Json::Value location = value["location"];
                        
                        GameService::JoinResult joinResult;
                        
                        joinResult.roomId = value["id"].asString();
                        joinResult.key = value["key"].asString();
                        joinResult.host = location["host"].asString();
                        
                        for (Json::Value port : location["ports"])
                        {
                            joinResult.ports.push_back(port.asInt());
                        }
                        
                        if (value.isMember("settings"))
                        {
                            joinResult.settings = value["settings"];
                        }
                        
						callback(*this, request.getResult(), request, joinResult);
					}
					else
					{
						callback(*this, Request::MISSING_RESPONSE_FIELDS, request, GameService::JoinResult());
					}
				}
				else
				{
					callback(*this, request.getResult(), request, GameService::JoinResult());
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}
    
    void GameService::joinRoom(const std::string& accessToken, JoinRoomCallback callback, const std::string& gameServer, bool autoCreate, bool myRegionOnly)
    {
        joinRoom(accessToken, callback, gameServer, Json::Value(Json::objectValue), Json::Value(Json::objectValue), autoCreate, myRegionOnly);
    }
    
    void GameService::joinRoom(const std::string& accessToken, GameService::JoinRoomCallback callback, const std::string& gameServer,
                               const Json::Value& settings, const Json::Value& createSettings, bool autoCreate, bool myRegionOnly)
	{
		const ApplicationInfo& applicationInfo = AnthillRuntime::Instance().getApplicationInfo();

        JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/join/" +
                applicationInfo.applicationName + "/" + gameServer + "/" + applicationInfo.applicationVersion, Request::METHOD_POST);
        
		if (request)
        {
            request->setAPIVersion(API_VERSION);
        
            Json::FastWriter fastWriter;
            
            request->setPostFields({
                {"access_token", accessToken },
                {"settings", fastWriter.write(settings) },
                {"create_settings", fastWriter.write(createSettings) },
                {"auto_create", autoCreate ? "true": "false" },
                {"my_region_only", myRegionOnly ? "true": "false" }
            });
            
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful())
				{
					const Json::Value& value = request.getResponseValue();

					if (value.isMember("location") && value.isMember("key") && value.isMember("id"))
					{
                        Json::Value location = value["location"];
                        
                        GameService::JoinResult joinResult;
                        
                        joinResult.roomId = value["id"].asString();
                        joinResult.key = value["key"].asString();
                        joinResult.host = location["host"].asString();
                        
                        for (Json::Value port : location["ports"])
                        {
                            joinResult.ports.push_back(port.asInt());
                        }
                        
                        if (value.isMember("settings"))
                        {
                            joinResult.settings = value["settings"];
                        }
                        
						callback(*this, request.getResult(), request, joinResult);
					}
					else
					{
						callback(*this, Request::MISSING_RESPONSE_FIELDS, request, GameService::JoinResult());
					}
				}
				else
				{
					callback(*this, request.getResult(), request, GameService::JoinResult());
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}

	GameService::~GameService()
	{
		//
	}

	bool GameService::init()
	{
		return true;
	}
}
