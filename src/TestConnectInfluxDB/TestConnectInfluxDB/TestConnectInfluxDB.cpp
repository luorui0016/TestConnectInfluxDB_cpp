#include "influxdb.hpp"
#include <iostream>
#include "time.h"
#include "stdio.h"
using namespace std;

// Visit https://github.com/orca-zhang/influxdb-c for more test cases

int main(int argc, char const *argv[])
{
	//当前时间
	time_t t;
	t = time(NULL) + 8 * 60 * 60;
	//初始化
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	influxdb_cpp::server_info si("127.0.0.1", 8086, "history", "admin", "123456");
	// post_http demo with resp[optional]
	string resp;
	int ret = influxdb_cpp::builder()
		.meas("tab1")
		.tag("k", "v")
		.tag("x", "y")
		.field("x", 10)
		.field("y", 10.3, 2)
		.field("b", !!10)
		.timestamp(t *1000*1000*1000)
		.post_http(si, &resp);

	cout << ret << endl << resp << endl;

	// send_udp demo
	ret = influxdb_cpp::builder()
		.meas("tab1")
		.tag("k", "v")
		.tag("x", "y")
		.field("x", 10)
		.field("y", 3.14e18, 3)
		.field("b", !!10)
		.timestamp(t * 1000 * 1000 * 1000)
		.send_udp("127.0.0.1", 8089);

	cout << ret << endl;

	// query from table
	influxdb_cpp::server_info si_new("127.0.0.1", 8086, "history", "admin", "123456");
	influxdb_cpp::query(resp, "show databases", si);
	cout << resp << endl;

	// create_db
	influxdb_cpp::create_db(resp, "x", si_new);
	cout << resp << endl;

	//释放
	WSACleanup();
	return 0;
}
