#include "net_handler.h"
#include "structures.h"
#include "download.h"

//====================================================================
NET_HANDLER::NET_HANDLER(uint32_t buffer_size, std::string id, std::string host) noexcept(false)
	: m_buffer_index(0), m_body_size(0), m_body(nullptr), m_header_parsed(false),
	m_content_size(0), m_query(id), m_msg_info(), m_host(host),
	m_is_body_pointer_is_null(true), m_buffer_size(buffer_size),
	m_current_msg_type(MESSAGE_GET_VERSION), m_addr(NULL)
{
	m_buffer = new unsigned char[buffer_size];
	init_session();
}
//====================================================================
NET_HANDLER::NET_HANDLER(std::string id, std::string host) noexcept(false)
	: m_buffer_index(0), m_body_size(0), m_body(nullptr), m_header_parsed(false),
	m_content_size(0), m_buffer_size(10'000), m_query(id), m_msg_info(),
	m_host(host), m_is_body_pointer_is_null(true),
	m_current_msg_type(MESSAGE_GET_VERSION), m_addr(NULL)
{
	m_buffer = new unsigned char[10'000];
	init_session();
}
//====================================================================
void NET_HANDLER::init_session() noexcept(false)
{
	// init OpenSSL
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

	m_ssl_ctx = SSL_CTX_new(TLS_client_method());
	if (!m_ssl_ctx)
	{
		throw NET_STAT_SSL_CTX_ERR;
	}

	int32_t iResult = 0;
	WSAData wsaData = { 0 };

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		throw NET_STAT_WSA_INIT_ERR;
	}

	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		throw NET_STAT_CREATE_SOCKET_ERR;
	}

	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	DWORD dwRetval = getaddrinfo(m_host.c_str(), "https", &hints, &m_addr);
	if (dwRetval != 0)
	{
		WSACleanup();
		throw NET_STAT_GET_ADDR_INFO_ERR;
	}

	sockaddr_in* service = (sockaddr_in*)m_addr->ai_addr;

	iResult = connect(m_socket, (sockaddr*)service, sizeof(*service));
	if (iResult == SOCKET_ERROR)
	{
		freeaddrinfo(m_addr);
		WSACleanup();
		throw NET_STAT_CONN_ERR;
	}

	m_ssl = SSL_new(m_ssl_ctx);
	if (!m_ssl)
	{
		freeaddrinfo(m_addr);
		closesocket(m_socket);
		WSACleanup();
		throw NET_STAT_SSL_INIT_ERR;
	}

	// Привязка SSL к сокету
	SSL_set_fd(m_ssl, m_socket);

	// Установка SSL соединения
	if (SSL_connect(m_ssl) != 1)
	{
		freeaddrinfo(m_addr);
		closesocket(m_socket);
		WSACleanup();
		throw NET_STAT_TLS_CONN_ERR;
	}
}
//====================================================================
NET_HANDLER::~NET_HANDLER()
{
	delete[] m_buffer;

	if (m_body)
	{
		delete[] m_body;
	}

	closesocket(m_socket);
	SSL_shutdown(m_ssl);
	SSL_free(m_ssl);
	freeaddrinfo(m_addr);
	WSACleanup();
}
//====================================================================
NET_HANDLER& NET_HANDLER::send_message(MESSAGE_TYPE msg_type) noexcept(false)
{
	m_current_msg_type = msg_type;
	std::string header = get_header_msg();

	size_t iResult = SSL_write(m_ssl, header.data(), header.length());

	if (iResult < header.length())
	{
		throw NET_STAT_SEND_MSG_ERR;
	}

	return *this;
}
//====================================================================
NET_HANDLER& NET_HANDLER::recieve_message() noexcept(false)
{
	m_header_parsed = false;

	std::ofstream of_clean_file, of;
	uint64_t downloaded = 0;
	if (m_current_msg_type == MESSAGE_GET_FILE)
	{
		const std::string zip_path = has_custom_zip_path() ? get_zip_path() : DEFAULT_ZIP_PATH;

		namespace fs = std::filesystem;
		fs::path archive_path(zip_path);

		of_clean_file.open(zip_path, std::ios::out | std::ios::binary);
		of_clean_file.close();
		of.open(zip_path, std::ios::out | std::ios::binary | std::ios::app);
	}

	int iResult = 0;
	char* recv_buff = new char[5000];
	DWORD timeout = NRM_TIMER_EVAL;
	uint64_t content_length = 0;
	// Creating socket event
	WSAEVENT sock_event = WSACreateEvent();
	WSAEventSelect(m_socket, sock_event, FD_READ);

	while (true)
	{
		if (m_current_msg_type == MESSAGE_GET_FILE)
		{
			m_game_ver->progress = ((long double)downloaded / get_content_length()) * 100;
		}
		DWORD result = WaitForSingleObject(sock_event, timeout);
		if (result == WAIT_TIMEOUT)
		{
			WSACloseEvent(sock_event);
			throw NET_STAT_RECV_TIMEOUT;
		}
		if (result != WAIT_OBJECT_0)
		{
			WSACloseEvent(sock_event);
			throw NET_STAT_RECV_UNKNOWN_ERR;
		}

		iResult = SSL_read(m_ssl, recv_buff, 5000);
		if (iResult < 1)
		{
			int err = SSL_get_error(m_ssl, iResult);
			switch (err)
			{
			case SSL_ERROR_WANT_READ:
				// If received data have no payload
				continue;
			case SSL_ERROR_WANT_WRITE:
				throw NET_STAT_SSL_RIGHTS_ERR;
				break;
			case SSL_ERROR_SYSCALL:
				throw NET_STAT_SSL_SOCK_LVL_ERR;
				break;
			case SSL_ERROR_SSL:
				throw NET_STAT_SSL_INTERNAL_ERR;
				break;
			}
		}

		if (!is_header_parsed())
		{
			fill_buffer(recv_buff, iResult).is_header_ended(m_msg_info);
		}
		if (m_msg_info.is_header_ended() && !is_header_parsed())
		{
			if (m_msg_info.get_status() != 200)
			{
				throw NET_STAT_RES_CODE_NOT_OK;
			}
			parse_header();
			if (m_current_msg_type != MESSAGE_GET_FILE)
			{
				malloc_body(get_content_length());

				if (m_msg_info.get_body_length() > 0)
				{
					write_body(m_msg_info.get_body(), m_msg_info.get_body_length());
				}
			}
			else if (m_current_msg_type == MESSAGE_GET_FILE)
			{
				if (m_msg_info.get_body_length() > 0)
				{
					of.write(m_msg_info.get_body(), m_msg_info.get_body_length());
				}
			}

			continue;
		}
		if (is_header_parsed() && m_current_msg_type != MESSAGE_GET_FILE)
		{
			write_body(recv_buff, iResult);
		}
		else if (is_header_parsed() && m_current_msg_type == MESSAGE_GET_FILE)
		{
			of.write(recv_buff, iResult);
			downloaded += iResult;
			set_body_size(downloaded);
		}
		if (
			is_header_parsed()
			&& m_current_msg_type != MESSAGE_GET_FILE
			&& get_body_size() >= get_content_length()
		)
		{
			break;
		}
		if (
			is_header_parsed()
			&& m_current_msg_type == MESSAGE_GET_FILE
			&& downloaded >= get_content_length()
		)
		{
			break;
		}
	}
	delete[] recv_buff;
	of.close();
	WSACloseEvent(sock_event);

	if (m_current_msg_type == MESSAGE_TRY_GET_FILE)
	{
		m_query.parse_uuid((char*)m_body, m_body_size);
	}

	return *this;
}
//====================================================================
NET_HANDLER& NET_HANDLER::set_custom_zip_path(const char* path) noexcept(true)
{
	m_zip_temp_path.clear();
	m_zip_temp_path = path;

	return *this;
}
//====================================================================
bool NET_HANDLER::has_custom_zip_path() noexcept(true)
{
	return m_zip_temp_path.length();
}
//====================================================================
std::string NET_HANDLER::get_zip_path() noexcept(true)
{
	return m_zip_temp_path;
}
//====================================================================
unsigned char* NET_HANDLER::get_body() noexcept(true)
{
	return m_body;
}
//====================================================================
void NET_HANDLER::set_body_size(uint64_t size) noexcept(true)
{
	m_body_size = size;
}
//====================================================================
uint64_t NET_HANDLER::get_body_size() const noexcept(true)
{
	return m_body_size;
}
//====================================================================
NET_HANDLER::MESSAGE_QUERIES& NET_HANDLER::get_message_queries() noexcept(true)
{
	return m_query;
}
//====================================================================
std::string NET_HANDLER::get_header_msg() noexcept(true)
{
	std::stringstream ss;
	ss << "GET /download"
		<< get_queries()
		<< " HTTP/1.1\r\n"
		<< "Host: " << m_host << "\r\n"
		<< "User-Agent: bergemon client\r\n"
		<< "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
		<< "Accept-Language: en-US;q=0.8,en;q=0.5\r\n"
		<< "Accept-Encoding: gzip, deflate\r\n"
		<< "Connection: keep-alive\r\n\r\n";

	return ss.str();
}
//====================================================================
std::string NET_HANDLER::get_queries() noexcept(false)
{
	std::stringstream ss;
	ss << "?" << m_query.get_id().first << "=" << m_query.get_id().second
		<< "&" << m_query.get_export().first.c_str() << "=" << m_query.get_export().second.c_str()
		<< "&" << m_query.get_auth_user().first.c_str() << "=" << m_query.get_auth_user().second.c_str();

	if (m_current_msg_type == MESSAGE_GET_FILE && m_query.is_uuid_parsed())
	{
		ss << "&" << m_query.get_confirm().first << "=" << m_query.get_confirm().second
			<< "&" << m_query.get_uuid().first << "=" << m_query.get_uuid().second;
	}

	return ss.str();
}
//====================================================================
NET_HANDLER& NET_HANDLER::fill_buffer(char* src, uint32_t length) noexcept(false)
{
	const char* some_str = "Hello!";

	if (length == -1)
	{
		throw NET_STAT_FILL_BUFFER_LENGTH_ERR;
	}

	if (m_buffer_index + length >= m_buffer_size)
	{
		throw NET_STAT_FILL_BUFFER_OVERRUN;
	}

	std::memcpy(m_buffer + m_buffer_index, src, length);
	m_buffer_index += length - 1;

	return *this;
}
//====================================================================
void NET_HANDLER::is_header_ended(MESSAGE_INFO& info) noexcept(true)
{
	std::string str((char*)m_buffer, m_buffer_index + 1);
	size_t end_pos = str.find("\r\n\r\n");
	info.m_header_ended = false;
	info.m_body = nullptr;
	info.m_body_length = 0;

	if (end_pos != std::string::npos)
	{
		info.m_header_ended = true;
		info.m_body = (char*)m_buffer + end_pos + 4;
		info.m_body_length = (m_buffer_index + 1) - (end_pos + 4);
		uint64_t pos = 0;

		if ((pos = str.find("HTTP/1.1")) != std::string::npos)
		{
			info.m_status = std::atoi(str.substr(pos + 9, 3).c_str());
		}
		else if ((pos = str.find("HTTP/1.0")) != std::string::npos)
		{
			info.m_status = std::atoi(str.substr(pos + 9, 3).c_str());
		}
		else
		{
			info.m_status = 400;
		}
	}
}
//====================================================================
void NET_HANDLER::parse_header() noexcept(true)
{
	std::string str_buff((char*)m_buffer, m_buffer_index + 1);
	std::string line;

	while (str_buff.length() > 2)
	{
		line = str_buff.substr(0, str_buff.find("\r\n"));
		str_buff = str_buff.substr(
			str_buff.find("\r\n") + 2,
			str_buff.length() - (str_buff.find("\r\n") + 2)
		);

		std::string key = line.substr(0, line.find(":"));
		std::string value = line.substr(
			line.find(":") + 1,
			line.length() - (line.find(":") + 1)
		);
		m_header[key] = value;
		line.clear();
	}

	m_header_parsed = true;
}
//====================================================================
unsigned char* NET_HANDLER::malloc_body(uint64_t size) noexcept(false)
{
	free_body();

	m_body = new unsigned char[size];
	m_is_body_pointer_is_null = false;

	return m_body;
}
//====================================================================
bool NET_HANDLER::free_body() const noexcept(false)
{
	if (!m_is_body_pointer_is_null)
	{
		delete[] m_body;
		return true;
	}

	return false;
}
//====================================================================
void NET_HANDLER::write_body(char* src, uint64_t length) noexcept(true)
{
	std::memcpy(m_body + m_body_size, src, length);
	m_body_size += length;
}
//====================================================================
uint64_t NET_HANDLER::get_content_length() noexcept(false)
{
	if (m_header_parsed)
	{
		return std::atoi(m_header["Content-Length"].c_str());
	}

	return 0;
}
//====================================================================
bool NET_HANDLER::is_header_parsed() const noexcept(true)
{
	return m_header_parsed;
}
//====================================================================
NET_HANDLER& NET_HANDLER::set_info_to_handler(GAME_VERSION* ver) noexcept(true)
{
	m_game_ver = ver;

	return *this;
}
//====================================================================
uint8_t NET_HANDLER::ver_parse_n_compare()
{
	int8_t is_version_last = 1;
	std::string v_txt((char*)m_body, m_body_size);
	size_t pos;
	std::string line;
	uint8_t iter = 0;
	std::string value;

	while ((pos = v_txt.find("\r\n")) != std::string::npos)
	{
		line = v_txt.substr(0, pos);

		switch (iter)
		{
		case 0:
			// version:
			value = line.substr(line.find(":") + 1, 4);
			if (std::atoi(value.c_str()) != m_game_ver->version)
			{
				is_version_last = 0;
			}
			m_game_ver->version = std::atoi(value.c_str());
			break;
		case 1:
			// checksum:
			value = line.substr(line.find(":") + 1, 4);
			if (value != std::string(m_game_ver->checksum))
			{
				is_version_last = 0;
			}
			for (int i = 0; i < value.length(); ++i)
			{
				m_game_ver->checksum[i] = value[i];
			}
			m_game_ver->checksum[value.length()] = '\0';
			break;
		case 2:
			// id:
			value = line.substr(line.find(":") + 1, line.find(";") - 3);
			for (int i = 0; i < value.length(); ++i)
			{
				m_game_ver->new_file_id[i] = value[i];
			}
			m_game_ver->new_file_id[value.length()] = '\0';
			break;
		}
		++iter;

		v_txt = v_txt.substr(
			pos + 2,
			v_txt.length() - (pos + 2)
		);
	}

	return is_version_last;
}
//====================================================================
using MESSAGE_QUERIES = NET_HANDLER::MESSAGE_QUERIES;
//====================================================================
MESSAGE_QUERIES::MESSAGE_QUERIES(std::string id) noexcept(true)
	: f_id(std::pair<std::string, std::string>("id", id)),
	f_export(std::pair<std::string, std::string>("export", "download")),
	f_auth_user(std::pair<std::string, std::string>("authuser", "0")),
	f_confirm(std::pair<std::string, std::string>("confirm", "t")),
	f_uuid(std::pair<std::string, std::string>("uuid", "")),
	m_uuid_parsed(false)
{ }
//====================================================================
std::pair<std::string, std::string>& MESSAGE_QUERIES::get_id()
{
	return f_id;
}
//====================================================================
std::pair<std::string, std::string>& MESSAGE_QUERIES::get_export()
{
	return f_export;
}
//====================================================================
std::pair<std::string, std::string>& MESSAGE_QUERIES::get_auth_user()
{
	return f_auth_user;
}
//====================================================================
std::pair<std::string, std::string>& MESSAGE_QUERIES::get_confirm()
{
	return f_confirm;
}
//====================================================================
std::pair<std::string, std::string>& MESSAGE_QUERIES::get_uuid()
{
	if (!m_uuid_parsed)
	{
		throw NET_STAT_UUID_NOT_PARSED;
	}

	return f_uuid;
}
//====================================================================
bool MESSAGE_QUERIES::parse_uuid(char* src, uint64_t length)
{
	std::string body(src, length);
	uint64_t pos = body.find("<input type=\"hidden\" name=\"uuid\"");

	if (pos == std::string::npos)
	{
		return false;
	}
	src = src + pos;

	body.clear();
	body.append(src, length - pos);
	std::string value = body.substr(
		body.find("value=\"") + 7,
		body.find("\">") - body.find("value=\"") - 7
	);
	f_uuid.second = value;

	m_uuid_parsed = true;
	return true;
}
//====================================================================
bool MESSAGE_QUERIES::is_uuid_parsed() const noexcept(true)
{
	return m_uuid_parsed;
}
//====================================================================
using MESSAGE_INFO = NET_HANDLER::MESSAGE_INFO;
//====================================================================
MESSAGE_INFO::MESSAGE_INFO() noexcept(true)
	: m_header_ended(false), m_body(nullptr), m_body_length(0), m_status(0)
{ }
//====================================================================
bool MESSAGE_INFO::is_header_ended() const noexcept(true)
{
	return m_header_ended;
}
//====================================================================
char* MESSAGE_INFO::get_body() noexcept(true)
{
	return m_body;
}
//====================================================================
size_t MESSAGE_INFO::get_body_length() const noexcept(true)
{
	return m_body_length;
}
//====================================================================
uint16_t MESSAGE_INFO::get_status() const noexcept(true)
{
	return m_status;
}
//====================================================================