
#ifndef ONLINE_Report_Service_H
#define ONLINE_Report_Service_H

#include "anthill/services/Service.h"
#include "anthill/requests/Request.h"
#include "anthill/ApplicationInfo.h"
#include <json/value.h>

#include <istream>
#include <array>

namespace online
{
    typedef std::shared_ptr< class ReportService > ReportServicePtr;
    
    class AnthillRuntime;
    

    class ReportService : public Service
    {
        friend class AnthillRuntime;
	public:
        enum class ReportFormat
        {
            json = 0,
            binary,
            text,
            
            count
        };
        
    private:
        static const std::array<std::string, (size_t)ReportFormat::count> ReportFormatValues;
        
    public:
		typedef std::function< void(const ReportService& service, Request::Result result,
            const Request& request, const std::string& reportId) > UploadReportCallback;
        
    public:
        static const std::string ID;
        static const std::string API_VERSION;
        
    public:
        static ReportServicePtr Create(const std::string& location);
        virtual ~ReportService();
        
        void uploadReportJSON(const std::string& category, const std::string& message,
            const Json::Value& info, const Json::Value& contents,
            const std::string& accessToken, UploadReportCallback callback);
        
        void uploadReport(const std::string& category, const std::string& message,
            ReportFormat format, const Json::Value& info, const std::string& contents,
            const std::string& accessToken, UploadReportCallback callback);
        
		void uploadReport(const std::string& category, const std::string& message,
            ReportFormat format, const Json::Value& info, std::istream& contents,
			const std::string& accessToken, UploadReportCallback callback);
        
    protected:
        ReportService(const std::string& location);
        bool init();
        
    private:
    };
};

#endif
