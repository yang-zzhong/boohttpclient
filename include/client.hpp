#ifndef _BOO_HTTP_CLIENT_CLIENT_H_
#define _BOO_HTTP_CLIENT_CLIENT_H_

#include <string>
#include <sstream>
#include <regex>
#include <thread>
#include "body.hpp"
#include "header.hpp"
#include "request.hpp"
#include "response.hpp"
#include "curl/curl.h"
#include "curl/easy.h"

namespace boohttp
{
	using namespace std;

	void prepare();
	void cleanup();

	typedef void(*ONRESPONSE) (Request * req, Response *res);

	const string AGENT = "BOO HTTP CLIENT";

	class Client
	{
	public:
		Client() {};
		Client(string hostname) : hostname(hostname) {};
		Client(string hostname, string path_base): hostname(hostname), path_base(path_base) {};

		CURLcode error() { return ecode;  };
		string errstr();
		void setHostname(string hostname);
		void setPathBase(string path_base);
		void setTimeout(long timeout);
		void send(const Request &req, Response * res);
		void send(const Request *req, Response * res);
		void async(const Request &req, ONRESPONSE onr);
		void async(const Request *req, ONRESPONSE onr);

		void get(const string & url, Response * res);
		void post(const string & url, PostData * postData, Response *res);
		void put(const string & url, PostData * postData, Response *res);
		void del(const string & url, PostData * postData, Response *res);
		void options(const string & url, PostData * postData, Response *res);
		void asyncGet(const string & url, ONRESPONSE onr);
		void asyncPost(const string & url, PostData * postData, ONRESPONSE onr);
		void asyncPut(const string & url, PostData * postData, ONRESPONSE onr);
		void asyncDel(const string & url, PostData * postData, ONRESPONSE onr);
		void asyncOptions(const string & url, PostData * postData, ONRESPONSE onr);

		~Client();
	private:
		void _post(const string & url, PostData * postData, Response * res, Method method);
		void _asyncPost(const string & url, PostData * postData, ONRESPONSE onr, Method method);
		void handleAsync(Request * req, Response * res, ONRESPONSE onr);
		CURL * wrapCurl(const Request * req, Response * res);
		bool handleMethod(const Request * req, CURL * curl);
		bool handlePostData(const Request * req, CURL * curl);
		bool handleHeader(const Header * header, CURL * curl);
		bool handleBody(const Body * body, CURL * curl);
		bool handleUrl(const Request * req, CURL * curl);
		bool handleReferer(const Request * req, CURL * curl);
		bool handleVersion(Version v, CURL * curl);
		bool handleTimeout(CURL * curl);
		bool handleAuth(const Request * req, CURL *curl);

	private:
		string hostname;
		string path_base;
		string agent = AGENT;

		CURLcode ecode;

		bool autoreferer = true;
		long timeout = 0;
	};

}

#endif
