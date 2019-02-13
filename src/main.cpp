#include "client.hpp"
#include "postdata.hpp"
#include <iostream>
#include "url.hpp"

int main()
{
	// boohttp::Url *url = new boohttp::Url("https://yang-zhong:15935789@baidu.com:8080/hello/world?a=b&c=d#efgh");
	// boohttp::Url *url = new boohttp::Url("https://yang-zhong:15935789@baidu.com/hello/world?a=b&c=d#efgh");
	// boohttp::Url *url = new boohttp::Url("https://yang-zhong@baidu.com/hello/world?a=b&c=d#efgh");
	// boohttp::Url *url = new boohttp::Url("https://baidu.com/hello/world?a=b&c=d#efgh");
	// boohttp::Url *url = new boohttp::Url("baidu.com/hello/world?a=b&c=d#efgh");
	// boohttp::Url *url = new boohttp::Url("baidu.com?a=b&c=d#efgh");
	// fprintf(stdout, "username: %s\n", url->username.c_str());
	// fprintf(stdout, "password: %s\n", url->password.c_str());
	// fprintf(stdout, "schema: %s\n", url->schema.c_str());
	// fprintf(stdout, "host: %s\n", url->host.c_str());
	// fprintf(stdout, "port: %s\n", url->port.c_str());
	// fprintf(stdout, "path: %s\n", url->path.c_str());
	// fprintf(stdout, "hash: %s\n", url->hash.c_str());
	// for (auto i = url->query.begin(); i != url->query.end(); ++i) {
	// 	fprintf(stdout, "key: %s, value: %s\n", i->first.c_str(), i->second.c_str());
	// }
	// fprintf(stdout, "url: %s\n", url->str().c_str());
	// string escaped = boohttp::Url::escape(url->str());
	// fprintf(stdout, "escape url: %s\n", escaped.c_str());
	// fprintf(stdout, "origin url: %s\n", boohttp::Url::unescape(escaped).c_str());
	// delete url;
	// return 0;

	boohttp::prepare();

    boohttp::Client client;
	boohttp::Response res;
	client.get("https://booblogger.com/api/blogs", &res);
	fprintf(stdout, "status code: %d\n", res.statusCode);
	fprintf(stdout, "body: %s\n", res.body()->str().c_str());
	auto h = res.header();
	fprintf(stdout, "header: \n");
	for (auto i = h->all().begin(); i != h->all().end(); ++i) {
		fprintf(stdout, "\t%s: %s\n", i->first.c_str(), i->second.c_str());
	}

	// boohttp::Response res;
	// boohttp::PostData postData;

	// postData.set("hello[0]", "hello");
	// postData.set("hello[1]", "hello1");
	// client.post("https://baidu.com", &postData, &res);
	// if (client.error() != 0) {
	// 	printf("%s\n", client.errstr().c_str());
	// }
	thread th([]() {
		this_thread::sleep_for(chrono::seconds(10));
	});
	if (th.joinable()) {
		th.join();
	}

	boohttp::cleanup();

    // Request req1("/test-sample");
    // Header header;
    // header.set("content-type", "application/json");
    // req.withHeader(header);
    // client->send(req1);
}