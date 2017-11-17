
#ifndef ONLINE_DLC_Service_H
#define ONLINE_DLC_Service_H

#include "anthill/services/Service.h"
#include "anthill/requests/Request.h"
#include "anthill/ApplicationInfo.h"

#include <json/value.h>

#include <set>
#include <unordered_map>
#include <list>

namespace online
{
    typedef std::shared_ptr< class DLCService > DLCServicePtr;
    
    class AnthillRuntime;
    
    class DLCService : public Service
    {
        friend class AnthillRuntime;
        
    public:
        class Bundle
        {
            friend class DLCService;
        
        public:
            Bundle(const std::string& name, long size, const std::string& url, const std::string& hash, const Json::Value& payload) :
                m_size(size),
                m_name(name),
                m_url(url),
                m_hash(hash),
                m_payload(payload)
            {
            }
        
            long getSize() const { return m_size; }
            const std::string& getName() const { return m_name; }
            const std::string& getUrl() const { return m_url; }
            const std::string& getHash() const { return m_hash; }
            const Json::Value& getPayload() const { return m_payload; }
            
        private:
            long m_size;
            std::string m_name;
            std::string m_url;
            std::string m_hash;
            Json::Value m_payload;
        };
        
        typedef std::list<Bundle> Bundles;
    public:
        typedef std::function< void(const DLCService& service, Request::Result result, const Request& request) > GetUpdatesCallback;
        
    public:
        static const std::string ID;
        static const std::string API_VERSION;
        
    public:
        static DLCServicePtr Create(const std::string& location);
        virtual ~DLCService();
        
        void getUpdates(Bundles& bundlesOutput, GetUpdatesCallback callback);
        void getUpdates(Bundles& bundlesOutput, GetUpdatesCallback callback, 
						const std::string& applicationName, const std::string& applicationVersion,
						const Json::Value& env);

    protected:
        DLCService(const std::string& location);
        bool init();
        
    private:
    };
};

#endif
