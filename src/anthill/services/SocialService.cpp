
#include "anthill/services/SocialService.h"
#include "anthill/requests/JsonRequest.h"
#include "anthill/AnthillRuntime.h"
#include "anthill/Utils.h"

#include "json/writer.h"
#include <numeric>

namespace online
{
	const std::string SocialService::ID = "social";
    const std::string SocialService::API_VERSION = "0.2";
    
    
    SocialConnection::CredentialInfo::CredentialInfo(const Json::Value& data)
    {
        if (data.isMember("social"))
        {
            social = data["social"];
        }
    }
    
    SocialConnection::SocialConnection(const Json::Value& data)
    {
        if (data.isMember("credentials"))
        {
            const Json::Value& credentials = data["credentials"];
            
            for (Json::ValueConstIterator it = credentials.begin(); it != credentials.end(); it++)
            {
                std::string credential = it.name();
                
                m_credentials.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(credential),
                    std::forward_as_tuple(*it));
            }
        }
        
        if (data.isMember("profile"))
        {
            m_profile = data["profile"];
        }
    }
	
    SocialRequest::SocialRequest(const Json::Value& data)
    {
        if(data.isMember( "type" ))
		{
			std::string tempType = data["type"].asString();

			if(tempType == "group")
			{ 
				m_type = Type::group;
			}
			else if (tempType == "account")
			{
				m_type = Type::account;
			}
			else
			{
				m_type = Type::unknown;
			}
		}
		else
		{
			m_type = Type::unknown;
		}
				
		if(data.isMember( "object" ))
			m_object = data["object"].asString();
				
		if(data.isMember( "payload" ))
			m_payload = data["payload"];
				
		if(data.isMember( "key" ))
			m_key = data["key"].asString();
	}

	SocialServicePtr SocialService::Create(const std::string& location)
	{
		SocialServicePtr _object(new SocialService(location));
		if (!_object->init())
			return SocialServicePtr(nullptr);

		return _object;
	}

	SocialService::SocialService(const std::string& location) :
		Service(location)
	{
		
    }
    
	void SocialService::getConnections(
        const std::string& accessToken,
        const std::set<std::string>& profileFields,
        GetConnectionsCallback callback)
	{
		JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/connections", Request::METHOD_GET);

		if (request)
		{
            request->setAPIVersion(API_VERSION);
        
            Json::Value profileFields_(Json::ValueType::arrayValue);
            
            for (const std::string& field: profileFields)
            {
                profileFields_.append(field);
            }
            
			request->setRequestArguments({
                {"access_token", accessToken },
                {"profile_fields", Json::FastWriter().write(profileFields_)}
            });
        
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful() && request.isResponseValueValid())
				{
					const Json::Value& value = request.getResponseValue();
                    
                    SocialConnections out_;
                    
                    if (value.isMember("connections"))
                    {
                        const Json::Value& connections = value["connections"];
                        
                        for (Json::ValueConstIterator it = connections.begin(); it != connections.end(); it++)
                        {
                            std::string account = it.name();
                            const Json::Value& connection = *it;
                            
                            out_.emplace(
                                std::piecewise_construct,
                                std::forward_as_tuple(account),
                                std::forward_as_tuple(connection));
                        }
                    }

					callback(*this, request.getResult(), request, out_);
				}
				else
				{
					callback(*this, request.getResult(), request, SocialConnections());
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}
	
	void SocialService::addConnection(
		const std::string& account,
		const std::string& accessToken,
        AddConnectionsCallback callback,
		bool approval,
		const Json::Value& notify,
		const Json::Value& payload)
	{
		JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/connection/" + account, Request::METHOD_POST);

		if (request)
		{
            request->setAPIVersion(API_VERSION);

			Request::Fields fields = {
                {"access_token", accessToken },
				{ "approval", approval ? "true" : "false" }
			};

			if( notify.isObject() )
				fields["notify"] = Json::FastWriter().write(notify);
			
			if( payload.isObject() )
				fields["payload"] = Json::FastWriter().write(payload);

			request->setPostFields(fields);
                
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful() && request.isResponseValueValid())
				{
					const Json::Value& value = request.getResponseValue();
					
					std::string key;
                    if (value.isMember("key"))
					{
						key = value["key"].asString();
					}
                    
					callback(*this, request.getResult(), request, key);
				}
				else
				{
					callback(*this, request.getResult(), request, "");
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}
	
	void SocialService::getIncomingRequests(
		const std::string& accessToken,
        GetRequestsCallback callback) 
	{
		JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/requests/incoming", Request::METHOD_GET);

		if (request)
		{
            request->setAPIVersion(API_VERSION);

			Request::Fields fields = {
                {"access_token", accessToken },
			};

			request->setRequestArguments(fields);
                
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful() && request.isResponseValueValid())
				{
					const Json::Value& value = request.getResponseValue();
					SocialRequests requests;
					
					if (value.isMember("requests"))
                    {
                        const Json::Value& requestsJson = value["requests"];
                        
                        for (Json::ValueConstIterator it = requestsJson.begin(); it != requestsJson.end(); it++)
                        {
							requests.emplace_back(*it);
                        }
                    }
					                    
					callback(*this, request.getResult(), request, requests);
				}
				else
				{
					callback(*this, request.getResult(), request, {});
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}
	
	void SocialService::getOutgoingRequests(
		const std::string& accessToken,
        GetRequestsCallback callback) 
	{
		JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/requests/outgoing", Request::METHOD_GET);

		if (request)
		{
            request->setAPIVersion(API_VERSION);

			Request::Fields fields = {
                {"access_token", accessToken },
			};

			request->setRequestArguments(fields);
                
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful() && request.isResponseValueValid())
				{
					const Json::Value& value = request.getResponseValue();
					SocialRequests requests;
					
					if (value.isMember("requests"))
                    {
                        const Json::Value& requestsJson = value["requests"];
                        
                        for (Json::ValueConstIterator it = requestsJson.begin(); it != requestsJson.end(); it++)
                        {
							requests.emplace_back(*it);
                        }
                    }
					                    
					callback(*this, request.getResult(), request, requests);
				}
				else
				{
					callback(*this, request.getResult(), request, {});
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}

	void SocialService::deleteConnection(
		const std::string& account,
		const std::string& accessToken,
        DeleteConnectionsCallback callback,
		const Json::Value& notify)
	{
		JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/connection/" + account, Request::METHOD_DELETE);

		if (request)
		{
            request->setAPIVersion(API_VERSION);

			Request::Fields fields = {
                {"access_token", accessToken }
			};

			if( notify.isObject() )
				fields["notify"] = Json::FastWriter().write(notify);

			request->setRequestArguments(fields);
                
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				callback(*this, request.getResult(), request);
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}
	
	void SocialService::approveConnection(
		const std::string& account,
		const std::string& key,
		const std::string& accessToken,
        ApproveConnectionsCallback callback,
		const Json::Value& notify)
	{
		JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/connection/" + account + "/approve", Request::METHOD_POST);

		if (request)
		{
            request->setAPIVersion(API_VERSION);

			Request::Fields fields = {
                {"access_token", accessToken },
				{ "key", key }
			};

			if( notify.isObject() )
				fields["notify"] = Json::FastWriter().write(notify);

			request->setPostFields(fields);
                
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				callback(*this, request.getResult(), request);
			
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}

	void SocialService::rejectConnection(
		const std::string& account,
		const std::string& key,
		const std::string& accessToken,
        RejectConnectionsCallback callback,
		const Json::Value& notify)
	{
		JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/connection/" + account + "/reject", Request::METHOD_POST);

		if (request)
		{
            request->setAPIVersion(API_VERSION);

			Request::Fields fields = {
                {"access_token", accessToken },
				{ "key", key }
			};

			if( notify.isObject() )
				fields["notify"] = Json::FastWriter().write(notify);

			request->setPostFields(fields);
                
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				callback(*this, request.getResult(), request);
			
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
	}

	SocialService::~SocialService()
	{
		//
	}

	bool SocialService::init()
	{
		return true;
	}
}
