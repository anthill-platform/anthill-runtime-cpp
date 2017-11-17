
#ifndef ONLINE_Websockets_H
#define ONLINE_Websockets_H

#include "JsonRPC.h"
#include <uWS.h>
#include <thread>

namespace online
{
    typedef std::shared_ptr< class WebsocketRPC > WebsocketRPCPtr;

    class WebsocketRPC: public JsonRPC
    {
    public:
        typedef std::unordered_map<std::string, std::string> Options;
        typedef std::function<void(bool success, int response)> ConnectCallback;
        typedef std::function<void(int code, const std::string& reason)> DisconnectCallback;
        
    public:
        static WebsocketRPCPtr Create();
        virtual void update() override;
        
        void connect(const std::string& location, const Options& options,
                     ConnectCallback onConnect, DisconnectCallback onDisconnect,
                     const std::map<std::string, std::string>& extraHeaders = {});
        void disconnect(int code, const std::string& reason);
        
        void close();
        void terminate();
		void waitForShutdown();
        
        bool isConnected() const { return m_connected; }
    
        ~WebsocketRPC();
    protected:
        WebsocketRPC();
    
        virtual bool read(std::string& data) override;
        virtual void write(const std::string& data) override;
        virtual void error(int code, const std::string& message, const std::string& data) override;
        
    private:
        void onMessage(uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode);
        
        static void OnWrite(void *webSocket, void *data, bool cancelled, void *reserved);
        
    private:
        uWS::Hub m_client;
        uWS::WebSocket<uWS::CLIENT> *m_socket;
        bool m_connected;
    };
}

#endif
