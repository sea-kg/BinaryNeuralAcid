#include "reversehashd_server.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include <QHostAddress>
#include "cmd_handlers/create_cmd_handlers.h"

#include "../vertex_graph.h"
#include <bna.h>
#include "../helpers.h"
#include "../memory.h"
#include "../memoryitem.h"
#include <QDir>

// QT_USE_NAMESPACE

// ---------------------------------------------------------------------

ReverseHashDServer::ReverseHashDServer(quint16 port, bool debug, QObject *parent) : QObject(parent) {
	m_pReverseHashDServer = new QWebSocketServer(QStringLiteral("reversehashd"), QWebSocketServer::NonSecureMode, this);
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
	
	// deprecated
	// init vertexes
	int nCount = 55*8;
	for (int i = 0; i < nCount; i++) {
		bool bResult = false;
		QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
		QFile file(filename);
		if(!file.exists()){
			// create new vertex file
			VertexGraph vg(128);
			vg.genBase();
			vg.saveToFile(filename);
			qDebug() << "Created new file: " << filename;
		}
	}
	
	// init bna
	QString path = "/usr/share/reversehashd/md5";
	QDir dir(".");
	dir.mkpath(path);
	
	QString pro_file_content_sources = "SOURCES += \\\r\n";
	pro_file_content_sources += "\tmd5revert_helpers.h \\\r\n";
	
	QString pro_file_content_headers = "HEADERS += \\\r\n";
	pro_file_content_sources += "\tmd5revert_main.cpp \\\r\n";
	pro_file_content_sources += "\tmd5revert_helpers.cpp \\\r\n";
	
	for(int i = 0; i < 440; i++){
		QString name = QString::number(i).rightJustified(3, '0');
		QString subdir = name[0] + "/" + name[1] + "/" + name[2];
		QString d = path + "/" + subdir;
		dir.mkpath(d);
		QString filename_bna = d + "/" + name + ".bna";
		QString filename_cpp = d + "/" + name + ".cpp";
		QString filename_h = d + "/" + name + ".h";
		QString filename_dot = d + "/" + name + ".dot";
		
		QFile file(filename_bna);
		if(!file.exists()){
			BNA bna;
			bna.randomGenerate(128,1,256);
			bna.save(filename_bna);
			qDebug() << filename_bna << " - created";
		}
		
		if(QFile(filename_bna).exists()){
			BNA bna;
			bna.load(filename_bna);
			bna.exportToCpp(filename_cpp, "func" + name);
		}
		
		if(QFile(filename_bna).exists()){
			BNA bna;
			bna.load(filename_bna);
			bna.exportToDot(filename_dot, "func" + name);
		}
		
		pro_file_content_sources += "\t" + subdir + "/" + name + ".cpp \\\r\n";
		pro_file_content_headers += "\t" + subdir + "/" + name + ".h \\\r\n";
	}
	
	// write pro file
	{
		pro_file_content_sources += "\r\n";
		pro_file_content_headers += "\r\n";

		QString sDilenameProFile = path + "/md5revert.pro";
		QFile file(sDilenameProFile);
		if (file.exists()) {
			file.remove();
		}
		if ( !file.open(QIODevice::WriteOnly) ) {
			qDebug().noquote().nospace() << "Could not write file: " << sDilenameProFile;
		}else{
			QTextStream stream( &file );
			stream << "TEMPLATE = app\r\n";
			stream << "TARGET = md5revert\r\n";
			stream << "QT += core\r\n";
			stream << "QT -= gui\r\n";
			stream << "\r\n";
			stream << "CONFIG += console\r\n";
			stream << "CONFIG -= app_bundle\r\n";
			stream << "CONFIG += c++11 c++14\r\n";
			stream << "\r\n";
			stream << "OBJECTS_DIR = tmp/\r\n";
			stream << "MOC_DIR = tmp/\r\n";
			stream << "RCC_DIR = tmp/\r\n";
			stream << "\r\n";
			stream << pro_file_content_sources;
			stream << pro_file_content_headers;
		}
	}

	// init main.cpp
	this->extractFile(":/res/md5revert_main.cpp", path + "/md5revert_main.cpp");
	this->extractFile(":/res/md5revert_helpers.h", path + "/md5revert_helpers.h");
	this->extractFile(":/res/md5revert_helpers.cpp", path + "/md5revert_helpers.cpp");
	
	if(QFile::exists(m_sFilename)){
		QSettings sett(m_sFilename, QSettings::IniFormat);
		m_sPassword = readStringFromSettings(sett, "MAIN/password", "password");
		if (m_debug){
			qDebug() << "Password: " << m_sPassword;
		}
	}

    if (m_pReverseHashDServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "reversehashd listening on port" << port;
        connect(m_pReverseHashDServer, &QWebSocketServer::newConnection, this, &ReverseHashDServer::onNewConnection);
        connect(m_pReverseHashDServer, &QWebSocketServer::closed, this, &ReverseHashDServer::closed);
        connect(this, &ReverseHashDServer::sendToAllSignal, this, &ReverseHashDServer::sendToAllSlot);
        
        create_cmd_handlers(m_mapCmdHandlers);
    }

    m_pTrainingThread = new TrainingThread(this);
    m_pTrainingThread->start(QThread::LowestPriority);
}

// ---------------------------------------------------------------------

ReverseHashDServer::~ReverseHashDServer() {
    m_pReverseHashDServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

// ---------------------------------------------------------------------

QString ReverseHashDServer::readStringFromSettings(QSettings &sett, QString settName, QString defaultValue){
	QString sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
		qDebug().noquote().nospace() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return sResult;
}

// ---------------------------------------------------------------------

int ReverseHashDServer::readIntFromSettings(QSettings &sett, QString settName, int defaultValue){
	int nResult = defaultValue;
	if(sett.contains(settName)){
		nResult = sett.value(settName, nResult).toInt();
	}else{
		qDebug().noquote().nospace() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return nResult;
}

// ---------------------------------------------------------------------

void ReverseHashDServer::extractFile(QString res_name, QString to_name){
	QFile file_main_cpp_res(res_name);
	if (!file_main_cpp_res.open(QIODevice::ReadOnly)){
		qDebug().noquote().nospace() << "Could not open file: '" << res_name << "'";
		return;
	}
	QTextStream res_main_cpp_file( &file_main_cpp_res );
	QString main_cpp_contents;
	main_cpp_contents.append(res_main_cpp_file.readAll());
	file_main_cpp_res.close();
	QString sFilenameMainCpp = to_name;
	QFile file_main_cpp(sFilenameMainCpp);
	if (file_main_cpp.exists()) {
		file_main_cpp.remove();
	}
	if ( !file_main_cpp.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << to_name;
	}else{
		QTextStream stream_main_cpp( &file_main_cpp );
		stream_main_cpp << main_cpp_contents;
	}
}

// ---------------------------------------------------------------------

void ReverseHashDServer::onNewConnection()
{
    QWebSocket *pSocket = m_pReverseHashDServer->nextPendingConnection();
	
	if (m_debug)
        qDebug() << "NewConnection " << pSocket->peerAddress().toString() << " " << pSocket->peerPort();
        
    connect(pSocket, &QWebSocket::textMessageReceived, this, &ReverseHashDServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &ReverseHashDServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &ReverseHashDServer::socketDisconnected);

    m_clients << pSocket;
}

// ---------------------------------------------------------------------

void ReverseHashDServer::processTextMessage(QString message) {
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

void ReverseHashDServer::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

// ---------------------------------------------------------------------

void ReverseHashDServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

// ---------------------------------------------------------------------

int ReverseHashDServer::getConnectedUsers(){
	return m_clients.length();
}

// ---------------------------------------------------------------------

void ReverseHashDServer::sendMessage(QWebSocket *pClient, QJsonObject obj){
	 if (pClient && m_clients.contains(pClient)) {
		QJsonDocument doc(obj);
		QString message = doc.toJson(QJsonDocument::Compact);
		QString cmd = obj["cmd"].toString();
		int rid = obj["rid"].toInt();
		qDebug().nospace().noquote() << QDateTime::currentDateTimeUtc().toString() << " [WS] <<< (cmd = " << cmd << ", " << rid << ") ";
        pClient->sendTextMessage(message);
    }
}

// ---------------------------------------------------------------------

void ReverseHashDServer::sendMessageError(QWebSocket *pClient, QString cmd, int id, Error error){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd);
	jsonData["rid"] = QJsonValue(id);
	jsonData["result"] = QJsonValue("FAIL");
	jsonData["error"] = QJsonValue(error.message());
	jsonData["code"] = QJsonValue(error.codeError());
	this->sendMessage(pClient, jsonData);
	return;
}

// ---------------------------------------------------------------------

void ReverseHashDServer::sendToAll(QJsonObject obj){
	emit sendToAllSignal(obj);
}

// ---------------------------------------------------------------------

void ReverseHashDServer::sendToAllSlot(QJsonObject obj){
	for(int i = 0; i < m_clients.size(); i++){
		this->sendMessage(m_clients.at(i), obj);
	}
}

// ---------------------------------------------------------------------
