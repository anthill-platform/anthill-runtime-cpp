
#include "anthill/services/MessageService.h"
#include "anthill/requests/JsonRequest.h"
#include "anthill/AnthillRuntime.h"
#include "anthill/Utils.h"
#include "anthill/Log.h"

#include <json/writer.h>

#include <set>

namespace online
{
    const std::string MessageService::ID = "message";
    const std::string MessageService::API_VERSION = "0.2";
    
    
    MessageSession::MessageSession(const std::string location) :
        m_sockets(WebsocketRPC::Create()),
        m_location(location)
    {
    }
        
    bool MessageSession::isConnected() const
    {
        return m_sockets && m_sockets->isConnected();
    }
    
    void MessageSession::send(const std::string& recipientClass, const std::string& recipient, const std::string& messageType, const Json::Value& message, const std::set<std::string>& flags, MessageSession::MessageSendCallback callback)
    {
        if (!isConnected())
        {
            callback(false, "Connection is not opened");
            return;
        }
        
        Json::Value args(Json::ValueType::objectValue);
        
        args["recipient_class"] = recipientClass;
        args["recipient_key"] = recipient;
        args["message_type"] = messageType;
        args["message"] = message;
        
        Json::Value flags_(Json::ValueType::arrayValue);
        
        for (const std::string& flag: flags)
        {
            flags_.append(flag);
        }
        
        args["flags"] = flags_;
        
        m_sockets->request("send_message", [callback](const Json::Value& response)
        {
            if (callback)
                callback(true, response.asString());
        }, [callback](int code, const std::string& message, const std::string& data)
        {
            std::stringstream reason;
            reason << code << " " << message << " " << data;
            
            if (callback)
                callback(false, reason.str());
        }, args);
    }
    
    void MessageSession::listen(const std::string& accessToken, MessageSession::ListenCallback callback,
            MessageSession::ConnectionClosedCallback closed, MessageSession::MessageCallback message)
    {
        if (!m_sockets)
        {
            callback(false, "Sockets is not initialized");
            return;
        }
        
        WebsocketRPC::Options options;
        
        options["access_token"] = accessToken;
        
        std::map<std::string, std::string> extraHeaders = {
            {"X-Api-Version", MessageService::API_VERSION}
        };
        
        m_sockets->connect(m_location + "/listen", options, [=](bool success, int response)
        {
            std::stringstream cnt;
            cnt << "Connection: " << response;
            callback(success, cnt.str());
        }, [=](int code, const std::string& reason)
        {
            closed(code, reason);
        }, extraHeaders);
        
        m_sockets->handle("message", [=](const Json::Value& params, JsonRPC::Success success, JsonRPC::Failture failture)
        {
            if (!params.isMember("message_id"))
            {
                Log::get() << "No message_id field!" << std::endl;
                return;
            }
            
            std::string uuid = params["message_id"].asString();
            
            if (!params.isMember("sender"))
            {
                Log::get() << "Message " << uuid << ": No sender field!" << std::endl;
                return;
            }
            
            if (!params.isMember("recipient_class"))
            {
                Log::get() << "Message " << uuid << ": No recipient_class field!" << std::endl;
                return;
            }
            
            if (!params.isMember("recipient_key"))
            {
                Log::get() << "Message " << uuid << ": No recipient_key field!" << std::endl;
                return;
            }
            
            if (!params.isMember("message_type"))
            {
                Log::get() << "Message " << uuid << ": No message_type field!" << std::endl;
                return;
            }
            
            if (!params.isMember("payload"))
            {
                Log::get() << "Message " << uuid << ": No payload field!" << std::endl;
                return;
            }
            
            if (!params.isMember("time"))
            {
                Log::get() << "Message " << uuid << ": No time field!" << std::endl;
                return;
            }
        
            std::string sender = params["sender"].asString();
            std::string recipientClass = params["recipient_class"].asString();
            std::string recipient = params["recipient_key"].asString();
            std::string messageType = params["message_type"].asString();
            std::string time = params["time"].asString();
            const Json::Value& payload = params["payload"];

			MessageFlags flags;

			if (params.isMember("flags"))
			{
				const Json::Value& flags_ = params["flags"];

				for (Json::ValueConstIterator it = flags_.begin(); it != flags_.end(); it++)
				{
					flags.insert(it->asString());
				}
			}
            
            message(uuid, sender, recipientClass, recipient, messageType, payload, time, flags);
        });

    }
    
    void MessageSession::update()
    {
        m_sockets->update();
    }
    
    void MessageSession::disconnect(int code, const std::string& reason)
    {
        m_sockets->disconnect(code, reason);
    }
    
	void MessageSession::waitForShutdown()
	{
		m_sockets->waitForShutdown();
	}

    MessageSessionPtr MessageSession::Create(const std::string location)
    {
        return std::make_shared<MessageSession>(location);
    }

    //////////////////////
    
    MessageServicePtr MessageService::Create(const std::string& location)
    {
        MessageServicePtr _object(new MessageService(location));
        if (!_object->init())
            return MessageServicePtr(nullptr);
        
        return _object;
    }
    
    MessageService::MessageService(const std::string& location) :
        Service(location)
    {
    }
    
    void MessageService::joinGroup(const std::string& groupClass, const std::string& groupKey, const std::string& role, const std::string& accessToken, JoinCallback callback)
    {
        JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/group/" + groupClass + "/" + groupKey + "/join", Request::METHOD_POST);
        
		if (request)
        {
            request->setAPIVersion(API_VERSION);
        
            Json::FastWriter fastWriter;
            
            request->setPostFields({
                {"access_token", accessToken },
                {"role", role}
            });
            
			request->setOnResponse([=](const online::JsonRequest& request)
			{
                if (request.isSuccessful())
                {
                    if (request.isResponseValueValid())
                    {
                        const Json::Value& response = request.getResponseValue();
                        /*
                        if (response.isMember("reply-to") && response.isMember("reply-to-class"))
                        {
                            callback(*this, request.getResult(), request, response["reply-to-class"].asString(), response["reply-to"].asString());
                        }*/
						if (response.isMember("reply_to"))
                        {
                            callback(*this, request.getResult(), request, "", "");
                        }
                        else
                        {
                            callback(*this, Request::BAD_ARGUMENTS, request, "", "");
                        }
                    }
                    else
                    {
                        callback(*this, Request::BAD_ARGUMENTS, request, "", "");
                    }
                }
                else
                {
                    callback(*this, request.getResult(), request, "", "");
                }
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
    }
    
    void MessageService::readRecipientMessages(const std::string& recipient, const std::string& accessToken, ReadMessagesCallback callback, int limit)
    {
        JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/messages/with/" + recipient, Request::METHOD_GET);
        
		if (request)
        {
            request->setAPIVersion(API_VERSION);
        
            Json::FastWriter fastWriter;
            
            std::stringstream _limit; _limit << limit;
            
            request->setRequestArguments({
                {"access_token", accessToken },
                {"limit", _limit.str()}
            });
            
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful())
				{
					const Json::Value& value = request.getResponseValue();

					if (value.isMember("reply_to") && value.isMember("messages"))
					{
						const Json::Value& replyTo = value["reply_to"];

                        std::string recipientClass = replyTo["recipient_class"].asString();
                        std::string recipient = replyTo["recipient"].asString();
                        const Json::Value& messages_ = value["messages"];
                        
                        std::vector<Json::Value> messages;
                        
                        for (const Json::Value& message : messages_)
                        {
                            messages.push_back(message);
                        }
                        
						callback(*this, request.getResult(), request, recipientClass, recipient, messages);
					}
					else
					{
						callback(*this, Request::MISSING_RESPONSE_FIELDS, request, "", "", std::vector<Json::Value>());
					}
				}
				else
				{
					callback(*this, request.getResult(), request, "", "", std::vector<Json::Value>());
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
    }
     
    void MessageService::readGroupInbox(const std::string& groupClass, const std::string& groupKey, const std::string& accessToken, ReadMessagesCallback callback, int limit)
    {
        JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/group/" + groupClass + "/" + groupKey, Request::METHOD_GET);
        
		if (request)
        {
            request->setAPIVersion(API_VERSION);
        
            Json::FastWriter fastWriter;
            
            std::stringstream _limit; _limit << limit;
            
            request->setRequestArguments({
                {"access_token", accessToken },
                {"limit", _limit.str()}
            });
            
			request->setOnResponse([=](const online::JsonRequest& request)
			{
				if (request.isSuccessful())
				{
					const Json::Value& value = request.getResponseValue();

					if (value.isMember("reply_to") && value.isMember("messages"))
					{
						const Json::Value& replyTo = value["reply_to"];

                        std::string recipientClass = replyTo["recipient_class"].asString();
                        std::string recipient = replyTo["recipient"].asString();
                        const Json::Value& messages_ = value["messages"];
                        
                        std::vector<Json::Value> messages;
                        
                        for (const Json::Value& message : messages_)
                        {
                            messages.push_back(message);
                        }
                        
						callback(*this, request.getResult(), request, recipientClass, recipient, messages);
					}
					else
					{
						callback(*this, Request::MISSING_RESPONSE_FIELDS, request, "", "", std::vector<Json::Value>());
					}
				}
				else
				{
					callback(*this, request.getResult(), request, "", "", std::vector<Json::Value>());
				}
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
    }
    
    MessageSessionPtr MessageService::session()
    {
        return MessageSession::Create(getLocation());
    }
    
    MessageService::~MessageService()
    {
        //
    }
    
    bool MessageService::init()
    {
        return true;
    }
}
