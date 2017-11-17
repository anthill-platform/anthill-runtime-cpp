
#ifndef _JsonRPC_h_
#define _JsonRPC_h_

#include "json/value.h"
#include <unordered_map>
#include <functional>

namespace online
{
    class JsonRPC
	{
    public:
        typedef std::function < void (const Json::Value& response) > Success;
        typedef std::function < void (int code, const std::string& message, const std::string& data) > Failture;
        
        typedef std::function < void (const Json::Value& params, Success success, Failture failture) > RequestHandler;
        
        struct ResponseHandler
        {
            ResponseHandler(Success& success, Failture& failture, int future) :
                m_success(success),
                m_failture(failture),
                m_future(future)
            {}
        
            Success m_success;
            Failture m_failture;
            int m_future;
        };
        
        typedef std::unordered_map< std::string, RequestHandler > RequestHandlers;
        typedef std::unordered_map< int, ResponseHandler > ResponseHandlers;
        typedef std::function < void (bool success) > WriteCallback;
        
	public:
		virtual ~JsonRPC();
        
        void handle(const std::string& method, RequestHandler handler);
        void request(const std::string& method, Success success, Failture failture, const Json::Value& params, float timeout = 0);
        void rpc(const std::string& method, const Json::Value& params);
        virtual void update();
        
    private:
        Json::Value serializeError(int code, const std::string& message, const std::string& data = "");
        void writeError(int code, const std::string& message, const std::string& data = "", int id = -1);
        void writeResponse(const Json::Value result, int id);

	protected:
        JsonRPC();
        
    protected:
        virtual bool read(std::string& data) = 0;
        virtual void write(const std::string& data) = 0;
        virtual void error(int code, const std::string& message, const std::string& data) = 0;
        
        void received(const std::string& message);
        void rejectAllResponseHandlers(int code, const std::string& message, const std::string& data);
        
    private:
        RequestHandlers m_handlers;
        ResponseHandlers m_responseHandlers;
        int m_nextId;
	};

}

#endif 
