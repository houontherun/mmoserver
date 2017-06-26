﻿#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include "common_define.h"
#include "ILogModule.h"
#include <map>
#include <set>
#include <string.h>
#ifdef _LINUX
#include <signal.h>
#include <unistd.h>
#else
#include "getopt.h"
#endif // !_LINUX

#include "exception.h"


//#define _LINUX
//获取当前毫秒时间
uint64_t GetNowTimeMille();

uint64_t GetNowTimeSecond();

void SetTimeOffset(int o);

namespace game {
	// game提供的服务类型
	enum  GAMESERVICE {
		GAMESERVICE_NONE = 100,
		GAMESERVICE_REGISTER_GATE,
		GAMESERVICE_NOTIFY_CLIENT_DISCONNECT,
		GAMESERVICE_ENTITY_MESSAGE,
		GAMESERVICE_FIGHT_MESSAGE, //来自战斗服的lua的消息
		GAMESERVICE_FORWARD_MESSAGE,  // 处理服务端的消息   
		GAMESERVICE_SERVICE_MESSAGE,  // 客户端直接调用的service消息
		GAMESERVICE_HEARTBEAT, //心跳
		GAMESERVICE_GM_MESSAGE, //GM控制协议
		GAMESERVICE_TEST_CONNECTION,
		GAMESERVICE_PING_GAME,
		GAMESERVICE_PING_GATE_REPLY,
	};

	// game作为客户端发出的消息，只关心自己，到时会做一个转换
	enum  GAMECLIENT {
		GAMECLIENT_NONE = 200,
		GAMECLIENT_ENTITY_MESSAGE, //发消息给客户端
		GAMECLIENT_NOTIFY_KICK_CLIENT,  //通知断开客户端
		GAMECLIENT_FORWARD_MESSAGE, // 转发服务端消息
		GAMECLIENT_BROADCAST_TO_CLIENT, //广播给所有客户端
		GAMECLIENT_BROADCAST_TO_GAME, //广播给所有的game进程
		GAMECLIENT_REGISTER_SERVICE, //注册服务路由地址
		GAMECLIENT_FIGHT_MESSAGE, //发消息给战斗服
		GAMECLIENT_HEARTBEAT,
		GAMECLIENT_PING_GATE,
		GAMECLIENT_PING_GAME_REPLY,
	};
};

namespace gate {
	enum  GATESERVICE 
	{
		GATESERVICE_NONE = 300,
		GATESERVICE_ENTITY_MESSAGE, //客户端的正常信息
		GATESERVICE_SERVICE_MESSAGE, //客户端直接调用service信息
		GATESERVICE_AVATAR_INFO,
	};

	enum GATECLIENT 
	{
		GATECLIENT_NONE = 400,
		GATECLIENT_ENTITY_MESSAGE, //通知客户端的消息
	};
}

namespace dbproxy {
	enum DBPROXYSERVICE
	{
		DBSERVICE_NONE = 500,
		DBSERVICE_REGISTER_SERVER,
		DBSERVICE_INSERT_DOC,
		DBSERVICE_FIND_ONE_DOC,
		DBSERVICE_UPDATE_DOC,
		DBSERVICE_FIND_N_DOC,
		DBSERVICE_FIND_AND_MODIFY_DOC,
		DBSERVICE_TEST_ECHO,
		DBSERVICE_CLOSE_SERVER,
	};

	enum DBPROXYCLIENT 
	{
		DBCLIENT_NONE = 600,
		DBCLIENT_INSERT_DOC_REPLY,
		DBCLIENT_FIND_ONE_DOC_REPLY,
		DBCLIENT_UPDATE_DOC_REPLY,
		DBCLIENT_FIND_N_DOC_REPLY,
		DBCLIENT_FIND_AND_MODIFY_DOC_REPLY,
		DBCLIENT_TEST_ECHO_BACK,
	};
}

namespace fight {
	enum FIGHTSERVICE
	{

	};

	enum FIGHTCLIENT
	{

	};
}

namespace gamemanager {
	enum GAMEMANAGER_SERVICE
	{
		GAMEMANAGER_SERVICE_NONE = 2000,
		GAMEMANAGER_SERVICE_REGISTER_SERVER,
		//GAMEMANAGER_SERVICE_REGISTER_GAME,
		//GAMEMANAGER_SERVICE_REGISTER_GATE,
		//GAMEMANAGER_SERVICE_REGISTER_DB,
		GAMEMANAGER_SERVICE_HEARTBEAT,
		GAMEMANAGER_SERVICE_RUN_SCRIPT,
		GAMEMANAGER_SERVICE_RUN_SCRIPT_REPLY,
		GAMEMANAGER_SERVICE_RUN_LUA_MESSAGE,
		GAMEMANAGER_SERVICE_CTRL,
		GAMEMANAGER_SERVICE_FORWARD_GAME_MESSAGE,

		GAMEMANAGER_SERVICE_FORBID_NEW_CONNECTION,
		GAMEMANAGER_SERVICE_IGNORE_CLIENT_ENTITY_MSG,
		GAMEMANAGER_SERVICE_DISCONNECT_ALL_CONNECTION,
		GAMEMANAGER_SERVICE_CLOSE_GATE,

		GAMEMANAGER_SERVICE_NOTIFY_GAME_CLOSING,
		GAMEMANAGER_SERVICE_NOTIFY_GAME_CLOSED,
		GAMEMANAGER_SERVICE_CLOSE_GAME,

		GAMEMANAGER_SERVICE_CLOSE_DB,
	};

	enum GAMEMANAGER_CLIENT
	{
		GAMEMANAGER_CLIENT_NONE = 2100,
		GAMEMANAGER_CLIENT_HEARTBEAT,
		GAMEMANAGER_CLIENT_NEW_GAME,
		GAMEMANAGER_CLIENT_ALL_GAME_INFO,
		GAMEMANAGER_CLIENT_RUN_SCRIPT,
		GAMEMANAGER_CLIENT_RUN_SCRIPT_REPLY,
		GAMEMANAGER_CLIENT_LUA_MESSAGE,
		GAMEMANAGER_CLIENT_FORWARD_GAME_MESSAGE,
	};
}

typedef uint32_t CLIENTID;
typedef uint32_t SERVERID;
typedef uint64_t SESSIONID;
typedef uint16_t MESSAGEID;


enum SERVER_TYPE
{
	SERVER_TYPE_NONE = 0,						 // 未定义
	SERVER_TYPE_GATE,
	SERVER_TYPE_FIGHT,
	SERVER_TYPE_GAME,					     // 游戏服务器
	SERVER_TYPE_DB,
	SERVER_TYPE_GAMEMANAGER,
};

enum ENTITY_TYPE
{
	ENTITY_TYPE_INVALID = 0,
	ENTITY_TYPE_HERO = 2,
	ENTITY_TYPE_NPC = 8,
	ENTITY_TYPE_MONSTER = 4,
	ENTITY_TYPE_PET = 16,
	ENTITY_TYPE_WILDPET = 32,
	ENTITY_TYPE_SUMMON = 64,
	ENTITY_TYPE_DROP = 128,
	ENTITY_TYPE_SCENEOBJ = 128,
	ENTITY_TYPE_BULLET = 512,
	ENTITY_TYPE_EMPTYGO = 1024,
	MAX_ENTITY_TYPE = 100000,
};

enum DST_SERVER_TYPE
{

};

// 数据接收缓存 1M
#define MAX_RECVBUF_LEN  1048576*16

// 数据发送缓存 1M
#define MAX_SENDBUF_LEN  1048576*16

//  
#define MAX_CLIENT_SENDBUF_LEN 65536
#define MAX_CLIENT_RECVBUF_LEN 65536

//

enum SERVER_MESSAGE
{
	SERVER_MESSAGE_DB_INSERT = 101,
};

enum SERVER_STATE
{
	SERVER_STATE_CREATE = 0,       //
	SERVER_STATE_INIT,             // connect other server
	SERVER_STATE_RUN,              // all server connected, do work
	SERVER_STATE_STOP,             // will server_stop
	SERVER_STATE_STOPING,
	SERVER_STATE_FINISH,           // all done
};

#define INVALID_PROXY_ID  0
#define INVALID_SOCKET_ID -1
#define INVALID_SERVER_ID 0
#define ANY_SERVER_ID 0xffffffff

//typedef std::string   ENTITYID;
typedef uint32_t   ProxyID;


enum GM_COMMAND
{
	GM_COMMAND_CLOSE_SERVER = 8888,
};


struct ClientSessionInfo
{
	SESSIONID   SessionID;
	int         DeltaTime;
	int         Latency;
	int         MinLatency;

	ClientSessionInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};

// must little than 65536
#define MAX_MESSAGE_LENGTH = 60000




#endif // !__COMMON_H__

