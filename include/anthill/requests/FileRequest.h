
#ifndef ONLINE_FileRequest_H
#define ONLINE_FileRequest_H

#include "Request.h"

#include <json/value.h>
#include <functional>

namespace online
{
	typedef std::shared_ptr< class FileRequest > FileRequestPtr;

	class AnthillRuntime;

	class FileRequest: public FileStreamRequest
	{
		friend class AnthillRuntime;

	public:
		typedef std::function< void(const FileRequest&) > ResponseCallback;
		typedef std::function< bool (const FileRequest&, long downloaded, long total) > ProgressCallback;

	public:
		static FileRequestPtr Create(const std::string& location, Request::Method method, std::fstream& file);
		virtual ~FileRequest();

		void setOnResponse(ResponseCallback onResponse);
		void setOnProgress(ProgressCallback onResponse);
        
        long getDownloaded() const { return m_downloaded; }
        long getTotal() const { return m_total; }

	protected:
		FileRequest(const std::string& location, Request::Method method, std::fstream& file);
		virtual bool init() override;

		// called once the request is done
		virtual void done() override;
        
    private:
        static int processProgress(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

	private:
		ResponseCallback m_onResponse;
		ProgressCallback m_onProgress;
        
        long m_downloaded;
        long m_total;
	};
};

#endif
