
#include "anthill/requests/FileRequest.h"
#include <json/reader.h>

using namespace std::placeholders;

namespace online
{
	FileRequestPtr FileRequest::Create(const std::string& location, Request::Method method, std::fstream& file)
	{
		FileRequestPtr _object(new FileRequest(location, method, file));
		if( !_object->init() )				
			return FileRequestPtr(nullptr);

		return _object;
	}
	
	FileRequest::FileRequest(const std::string& location, Request::Method method, std::fstream& file) :
		FileStreamRequest(location, method, file),
        m_downloaded(0),
        m_total(0)
	{
		getTransport().add<CURLOPT_NOPROGRESS>(0L);
        
        getTransport().add<CURLOPT_XFERINFODATA>(this);
        getTransport().add<CURLOPT_XFERINFOFUNCTION>(&FileRequest::processProgress);
	}
    
    int FileRequest::processProgress(void *clientp, curl_off_t dltotal, curl_off_t dlnow,  curl_off_t ultotal, curl_off_t ulnow)
    {
        FileRequest* file = static_cast<FileRequest*>(clientp);
        
        file->m_downloaded = dlnow;
        file->m_total = dltotal;
        
        if (file->isCancelled())
        {
            return 1;
        }
        
        if (file->m_onProgress)
        {
            if (!file->m_onProgress(*file, file->m_downloaded, file->m_total))
            {
                file->cancel();
                return 1;
            }
        }
        
        return 0;
    }

	void FileRequest::setOnResponse(FileRequest::ResponseCallback onResponse)
	{
		m_onResponse = onResponse;
	}

	void FileRequest::setOnProgress(FileRequest::ProgressCallback onProgress)
	{
		m_onProgress = onProgress;
	}

	void FileRequest::done()
	{
		Request::done();

		if (m_onResponse)
		{
			m_onResponse(*this);
		}
	}

	FileRequest::~FileRequest()
	{
		//
	}

	bool FileRequest::init()
	{
		return Request::init();
	}
}
