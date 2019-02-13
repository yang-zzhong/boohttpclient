#include "client.hpp"

namespace boohttp
{
	static size_t booHttpCurlWriteBody(void * ptr, size_t size, size_t nmums, void * stream)
	{
		Response * res = (Response*)(stream);
		res->appendBody(string((const char *)(ptr)));
		return size * nmums;
	}

	static Version parseVersion(string v)
	{
		if (v == "1.0") {
			return V_1_0;
		} else if (v == "1.1") {
			return V_1_1;
		} else if (v == "2.0") {
			return V_2_0;
		} else {
			return V_NON;
		}
	}

	static size_t booHttpCurlWriteHeader(void *ptr, size_t size, size_t nmums, void * stream)
	{
		string input((const char *)(ptr));
		input = input.substr(0, input.length() - 2);
		Response * res = (Response*)(stream);
		regex ex("HTTP/((1\.0)|(1\.1)|2\.0)(.*)(.*)");
		// first row
		if (res->version == V_UNSET && regex_match(input, ex)) {
			res->version = parseVersion(input.substr(4, 3));
			res->statusCode = stoi(input.substr(9, 3));
			res->statusMessage = input.substr(12, input.length() - 12);
			return size * nmums;
		}
		size_t pos = input.find(':');
		if (pos == string::npos) {
			return size * nmums;
		}
		string value = input.substr(pos + 1, input.length() - pos);
		res->withHeaderPair(input.substr(0, pos), regex_replace(value, regex("^ "), ""));

		return size * nmums;
	}

	void prepare()
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	void cleanup()
	{
		curl_global_cleanup();
	}

    void Client::setTimeout(long timeout)
    {
        this->timeout = timeout;
    }

	string Client::errstr()
	{
		string e(curl_easy_strerror(ecode));
		return e;
	}

    void Client::send(const Request & req, Response * res)
    {
        send(&req, res);
    }

    void Client::send(const Request * req, Response * res)
    { 
        CURL * curl = wrapCurl(req, res);
        ecode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
    }

    void Client::async(const Request & req, ONRESPONSE onr)
    {
        return async(&req, onr);
    }

    void Client::async(const Request * req, ONRESPONSE onr)
    {
		auto res = new Response();
		auto rq = new Request();
		*rq = *req;
		thread th(&Client::handleAsync, this, rq, res, onr);
		th.detach();
    }

	void Client::asyncGet(const string & url, ONRESPONSE onr)
	{
		auto req = new Request(url, GET);
		auto res = new Response();
		thread th(&Client::handleAsync, this, req, res, onr);
		th.detach();
	}

	void Client::asyncPost(const string & url, PostData * postData, ONRESPONSE onr)
	{
		_asyncPost(url, postData, onr, POST);
	}

	void Client::asyncPut(const string & url, PostData * postData, ONRESPONSE onr)
	{
		_asyncPost(url, postData, onr, PUT);
	}

	void Client::asyncDel(const string & url, PostData * postData, ONRESPONSE onr)
	{
		_asyncPost(url, postData, onr, DEL);
	}

	void Client::asyncOptions(const string & url, PostData * postData, ONRESPONSE onr)
	{
		_asyncPost(url, postData, onr, OPTIONS);
	}

	void Client::_asyncPost(const string & url, PostData * postData, ONRESPONSE onr, Method method)
	{
		auto req = new Request(url, method);
		req->postData.merge(postData);
		auto res = new Response();

		thread th(&Client::handleAsync, this, req, res, onr);
		th.detach();
	}

	void Client::get(const string & url, Response * res)
	{
		send(Request(url, GET), res);
	}

	void Client::post(const string & url, PostData * postData, Response *res)
	{
		_post(url, postData, res, POST);
	}

	void Client::put(const string & url, PostData * postData, Response *res)
	{
		_post(url, postData, res, PUT);
	}

	void Client::del(const string & url, PostData *postData, Response * res)
	{
		_post(url, postData, res, DEL);
	}

	void Client::options(const string & url, PostData *postData, Response *res)
	{
		_post(url, postData, res, OPTIONS);
	}

	void Client::_post(const string &url, PostData *postData, Response *res, Method method)
	{
		auto req = Request(url, method);
		req.postData.merge(postData);
		send(req, res);
	}

	void Client::handleAsync(Request * req, Response * res, ONRESPONSE onr)
	{
		send(req, res);
		onr(req, res);
	}

    CURL *Client::wrapCurl(const Request * req, Response * res)
    {
        CURL *curl = curl_easy_init();
        const Header *h = req->header();
        const Body *b = req->body();
        handleUrl(req, curl);
		handleMethod(req, curl);
        handleReferer(req, curl);
        handleHeader(h, curl);
        handleVersion(req->version, curl);
        handleBody(b, curl);
		handleAuth(req, curl);
        handleTimeout(curl);
		(ecode = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, booHttpCurlWriteBody)) == 0 &&
		(ecode = curl_easy_setopt(curl, CURLOPT_WRITEDATA, res)) == 0 &&
		(ecode = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, booHttpCurlWriteHeader)) == 0 &&
		(ecode = curl_easy_setopt(curl, CURLOPT_HEADERDATA, res)) == 0 &&
		(ecode = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L)) == 0 &&
		(ecode = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L)) == 0 &&
		(ecode = curl_easy_setopt(curl, CURLOPT_USERAGENT, agent.c_str())) == 0;
		if (ecode != 0) {
			return nullptr;
		}

		return curl;
    }

	bool Client::handleAuth(const Request *req, CURL *curl)
	{
		if (req->auth.type == AUTH_NON) {
			return true;
		}
		switch (req->auth.type) {
		case AUTH_BASIC:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
			break;
		case AUTH_DIGEST:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
			break;
		case AUTH_DIGEST_IE:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST_IE);
			break;
		// case AUTH_BEARER:
		// 	ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
		// 	break;
		case AUTH_NEGOTIATE:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_NEGOTIATE);
			break;
		case AUTH_NTLM:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_NTLM);
			break;
		case AUTH_NTLM_WB:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_NTLM_WB);
			break;
		case AUTH_ANY:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
			break;
		case AUTH_ANYSAFE:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANYSAFE);
			break;
		case AUTH_ONLY:
			ecode = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ONLY);
			break;
		}

		return ecode == 0 && (ecode = curl_easy_setopt(curl, CURLOPT_USERPWD, (req->auth.user + ":" + req->auth.passwd).c_str()));
	}

	bool Client::handleMethod(const Request * req, CURL * curl)
	{
		switch (req->method) {
		case GET:
			ecode = curl_easy_setopt(curl, CURLOPT_POST, 0);
			ecode = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
			break;
		case POST:
			ecode = curl_easy_setopt(curl, CURLOPT_POST, 1);
			ecode = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
			break;
		case PUT:
			ecode = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
			break;
		case DEL:
			ecode = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
			break;
		case OPTIONS:
			ecode = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
			break;
		}

		return ecode == 0 && handlePostData(req, curl);
	}

	bool Client::handlePostData(const Request * req, CURL * curl)
	{
		map<string, string> data = req->postData.all();
		string raw = Url::queryString(data);
		return (ecode = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, raw.c_str())) == 0 &&
		(ecode = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, raw.length())) == 0;
	}

    bool Client::handleBody(const Body * body, CURL * curl)
    {
        const char * raw = body->str().c_str();
        return (ecode = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, raw)) == 0 &&
        (ecode = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(raw))) == 0;
    }

    bool Client::handleHeader(const Header * header, CURL *curl)
    {
        struct curl_slist *list = NULL;
        for(auto i = header->all().begin(); i != header->all().end(); ++i) {
            if (i->first == "") {
                continue;
            } else if (i->second == "") {
                list = curl_slist_append(list, (i->first + ";").c_str());
                continue;
            }
            list = curl_slist_append(list, (i->first + ":" + i->second).c_str());
        }
        ecode = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_slist_free_all(list);

		return ecode == 0;
    }

    bool Client::handleUrl(const Request * req, CURL * curl)
    {
        Url url = req->url;
        if (url.host.length() == 0 && hostname.length() > 0) {
            url.host = hostname;
        }
        if (path_base.length() > 0) {
            url.path = path_base + url.path;
        }
		string u = url.str();
		ecode = curl_easy_setopt(curl, CURLOPT_URL, u.c_str());

		return ecode == 0;
    }

    bool Client::handleReferer(const Request * req, CURL *curl)
    {
        if (req->referer != "") {
            ecode = curl_easy_setopt(curl, CURLOPT_REFERER, req->referer);
			return ecode == 0;
        } else if (autoreferer) {
			return (ecode = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)) == 0 &&
				(ecode = curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L)) == 0;
        }

		return true;
    }

    bool Client::handleVersion(Version version, CURL * curl)
    {
        switch (version) {
            case V_1_0:
                ecode = curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
                break;
            case V_1_1:
                ecode = curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
                break;
            case V_2_0:
                ecode = curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
                break;
            // case: V_2TSL:
            //     curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TSL);
            //     break;
            // case: V_AUTO:
            //     curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
            //     break;
            default:
                ecode = curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_NONE);
        }

		return ecode == 0;
    }

    bool Client::handleTimeout(CURL * curl)
    {
        if (timeout > 0) {
            ecode = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
			return ecode == 0;
        }

		return true;
    }

	Client::~Client() {}
}