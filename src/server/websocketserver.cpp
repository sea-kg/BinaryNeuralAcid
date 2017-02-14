#include "websocketserver.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include <QHostAddress>
#include "cmd_handlers/create_cmd_handlers.h"

#include "../vertex_graph.h"
#include "../helpers.h"
#include "../memory.h"
#include "../memoryitem.h"

// QT_USE_NAMESPACE

// ---------------------------------------------------------------------

WebSocketServer::WebSocketServer(quint16 port, bool debug, QObject *parent) : QObject(parent) {
	m_pWebSocketServer = new QWebSocketServer(QStringLiteral("reversehashd"), QWebSocketServer::NonSecureMode, this);
	m_debug = debug;
	m_sFilename = "/etc/reversehashd/conf.ini";
	
	if(!QFile::exists("/usr/share/reversehashd")){
		qDebug() << "Error /usr/share/reversehashd - file does not exists";
		return;
	}
	
	if(!QFile::exists("/usr/share/reversehashd/md5")){
		qDebug() << "Error /usr/share/reversehashd/md5 - file does not exists";
		return;
	}
	
	// init memory if not exists
	if(!QFile::exists("/usr/share/reversehashd/md5/memory_md5_10000.rhmem")){
		reverse_hash::Memory *pMemory = new reverse_hash::Memory();
        pMemory->generateData(10000);
        pMemory->save("/usr/share/reversehashd/md5/memory_md5_10000.rhmem");
		return;
	}
	
	// init vertexes
	int nCount = 55*8;
	for (int i = 0; i < nCount; i++) {
		bool bResult = false;
		QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
		QFile file(filename);
		if(!file.exists()){
			// create new vertex file
			reversehash::VertexGraph *pVertexGraph = new reversehash::VertexGraph(128);
			pVertexGraph->genBase();
			pVertexGraph->saveToFile(filename);
			qDebug() << "Created new file: " << filename;
		}
	}
	
	if(QFile::exists(m_sFilename)){
		QSettings sett(m_sFilename, QSettings::IniFormat);
		m_sPassword = readStringFromSettings(sett, "MAIN/password", "password");
		if (m_debug){
			qDebug() << "Password: " << m_sPassword;
		}
	}

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "reversehashd listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
        create_cmd_handlers(m_mapCmdHandlers);
    }
}

// ---------------------------------------------------------------------

WebSocketServer::~WebSocketServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

// ---------------------------------------------------------------------

QString WebSocketServer::readStringFromSettings(QSettings &sett, QString settName, QString defaultValue){
	QString sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return sResult;
}

// ---------------------------------------------------------------------

int WebSocketServer::readIntFromSettings(QSettings &sett, QString settName, int defaultValue){
	int nResult = defaultValue;
	if(sett.contains(settName)){
		nResult = sett.value(settName, nResult).toInt();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return nResult;
}

// ---------------------------------------------------------------------

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
	
	if (m_debug)
        qDebug() << "NewConnection " << pSocket->peerAddress().toString() << " " << pSocket->peerPort();
        
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
}

// ---------------------------------------------------------------------

void WebSocketServer::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug){
		qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] <<< " << message;
	}

	QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonData = doc.object();
	
	int id = 0;
	if(jsonData.contains("id")){
		id = jsonData["id"].toInt();
	}
	
	if(!jsonData.contains("cmd")){
		this->sendMessageError(pClient, "cmd", id, Error(400, "Invalid command format (Not found cmd)"));
		return;
	}

	QString cmd = jsonData["cmd"].toString();
		
	if(m_mapCmdHandlers.contains(cmd)){
		ICmdHandler *pCmdHandler = m_mapCmdHandlers[cmd];
		pCmdHandler->handle(pClient, this, jsonData);	
	}else{
		qDebug() << "Unknown command: " << cmd;
		this->sendMessageError(pClient, cmd, id, Error(404, "Unknown command"));
	}
	
}

// ---------------------------------------------------------------------

void WebSocketServer::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

// ---------------------------------------------------------------------

int WebSocketServer::getConnectedUsers(){
	return m_clients.length();
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessage(QWebSocket *pClient, QJsonObject obj){
	 if (pClient) {
		QJsonDocument doc(obj);
		QString message = doc.toJson(QJsonDocument::Compact);
		qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] >>> " << message;
        pClient->sendTextMessage(message);
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessageError(QWebSocket *pClient, QString cmd, int id, Error error){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd);
	jsonData["rid"] = QJsonValue(id);
	jsonData["result"] = QJsonValue("FAIL");
	jsonData["error"] = QJsonValue(error.message());
	jsonData["code"] = QJsonValue(error.codeError());
	this->sendMessage(pClient, jsonData);
	return;
}
