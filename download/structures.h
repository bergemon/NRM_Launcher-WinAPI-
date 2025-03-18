#ifndef STRUCTURES_MODULE_DEFINED

//====================================================================
enum NET_STATUS_CODES
{
	NET_STAT_OK,
	NET_STAT_SSL_CTX_ERR,
	NET_STAT_WSA_INIT_ERR,
	NET_STAT_CREATE_SOCKET_ERR,
	NET_STAT_GET_ADDR_INFO_ERR,
	NET_STAT_CONN_ERR,
	NET_STAT_SSL_INIT_ERR,
	NET_STAT_TLS_CONN_ERR,
	NET_STAT_SEND_MSG_ERR,
	NET_STAT_RECV_TIMEOUT,
	NET_STAT_RECV_UNKNOWN_ERR,
	NET_STAT_SSL_RIGHTS_ERR,
	NET_STAT_SSL_SOCK_LVL_ERR,
	NET_STAT_SSL_INTERNAL_ERR,
	NET_STAT_RES_CODE_NOT_OK,
	NET_STAT_FILL_BUFFER_LENGTH_ERR,
	NET_STAT_FILL_BUFFER_OVERRUN,
	NET_STAT_UUID_NOT_PARSED,

};
//====================================================================
struct GAME_VERSION
{
	int32_t version = 0;
	/* Must be null terminated C string */
	char* checksum;
	/* Must be null terminated C string */
	char* host;
	/* Must be null terminated C string */
	char* file_id;
	int8_t compare_result = 0;
	int8_t is_ended = 0;
	int8_t status = 1;
	int32_t status_code = NET_STAT_OK;
};
//====================================================================
enum UNZIP_STATUS_CODES
{
	STATUS_OK,
	STATUS_CANT_OPEN_ARCHIVE,
	STATUS_ERR_GETTING_FILE_INFO,
	STATUS_CANT_OPEN_ARCHIVE_FILE,
	STATUS_CANT_OPEN_OUTFILE,
	STATUS_PATH_TOO_LONG
};
//====================================================================
struct ZIP_INFO
{
	/* Must be null terminated C string */
	char* path_to_zip;
	int64_t num_of_entries = 0;
	int8_t status = 1;
	int32_t status_code = STATUS_OK;
};
//====================================================================
struct UNZIP_INFO
{
	/* Must null terminated C string */
	char* src_path;
	/* Must null terminated C string */
	char* out_path;
	int64_t start_index;
	int64_t end_index;
	/* 0% - 100% */
	int8_t progress = 0;
	int8_t status = 1;
	int8_t is_ended = 0;
	int32_t status_code = STATUS_OK;
};
//====================================================================

#else
enum NET_STATUS_CODES;
struct GAME_VERSION;
#endif
#define STRUCTURES_MODULE_DEFINED