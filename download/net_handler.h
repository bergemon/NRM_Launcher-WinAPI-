#ifndef NET_HANDLER_MODULE_DEFINED

#include <map>
#include <cstdint>
#include <utility>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <ws2tcpip.h>
#include "windows.h"
#include "openssl/ssl.h"
#include "openssl/conf.h"
#include "structures.h"
#include "const.h"

//====================================================================
enum MESSAGE_TYPE
{
	MESSAGE_GET_VERSION,
	MESSAGE_TRY_GET_FILE,
	MESSAGE_GET_FILE
};
//====================================================================
class NET_HANDLER
{
public:
	class MESSAGE_INFO
	{
	public:
		MESSAGE_INFO() noexcept(true);

		bool is_header_ended() const noexcept(true);
		char* get_body() noexcept(true);
		size_t get_body_length() const noexcept(true);
		uint16_t get_status() const noexcept(true);

		friend class NET_HANDLER;

	private:
		bool m_header_ended;
		char* m_body;
		size_t m_body_length;
		uint16_t m_status;
	};
	//====================================================================
	class MESSAGE_QUERIES
	{
	public:
		MESSAGE_QUERIES(std::string id) noexcept(true);

		std::pair<std::string, std::string>& get_id();
		std::pair<std::string, std::string>& get_export();
		std::pair<std::string, std::string>& get_auth_user();
		std::pair<std::string, std::string>& get_confirm();
		std::pair<std::string, std::string>& get_uuid();

		bool parse_uuid(char* src, uint64_t length);
		bool is_uuid_parsed();

	private:
		bool m_uuid_parsed;
		std::pair<std::string, std::string> f_id;
		std::pair<std::string, std::string> f_export;
		std::pair<std::string, std::string> f_auth_user;
		std::pair<std::string, std::string> f_confirm;
		std::pair<std::string, std::string> f_uuid;
	};
	//====================================================================
	NET_HANDLER(uint32_t buffer_size, std::string id, std::string host) noexcept(false);
	NET_HANDLER(std::string id, std::string host) noexcept(false);
	~NET_HANDLER();

	/* This method can throw an exception! */
	void init_session() noexcept(false);

	/* This method can throw an exception! */
	NET_HANDLER& send_message(MESSAGE_TYPE msg_type) noexcept(false);

	/* This method can throw an exception! */
	NET_HANDLER& recieve_message() noexcept(false);

	unsigned char* get_body() noexcept(true);
	uint64_t get_body_size() noexcept(true);
	MESSAGE_QUERIES& get_message_queries() noexcept(true);

	/* This method can throw an exception! */
	bool ver_parse_n_compare(GAME_VERSION& ver) noexcept(false);

private:
	std::string get_header_msg() noexcept(true);

	/* This method can throw an exception! */
	std::string get_queries() noexcept(false);

	/* This method can throw an exception! */
	NET_HANDLER& fill_buffer(char* src, uint32_t length) noexcept(false);

	void is_header_ended(MESSAGE_INFO& info) noexcept(true);

	void parse_header() noexcept(true);

	/* This method can throw an exception! */
	unsigned char* malloc_body(uint64_t size) noexcept(false);

	/* This method can throw an exception! */
	bool free_body() noexcept(false);

	void write_body(char* src, uint64_t length) noexcept(true);

	/* This method can throw an exception! */
	uint64_t get_content_length() noexcept(false);

	bool is_header_parsed() const noexcept(true);

	SOCKET m_socket;
	SSL* m_ssl;
	SSL_CTX* m_ssl_ctx;
	addrinfo* m_addr;
	unsigned char* m_buffer;
	uint32_t m_buffer_index;
	uint32_t m_buffer_size;
	std::map<std::string, std::string> m_header;
	uint64_t m_content_size;
	bool m_is_body_pointer_is_null;
	unsigned char* m_body;
	uint64_t m_body_size;
	bool m_header_parsed;
	MESSAGE_INFO m_msg_info;
	MESSAGE_QUERIES m_query;
	std::string m_host;
	MESSAGE_TYPE m_current_msg_type;
};

#else
class NET_HANDLER;
class NET_HANDLER::MESSAGE_INFO;
class NET_HANDLER::MESSAGE_QUERIES;
#endif
#define NET_HANDLER_MODULE_DEFINED