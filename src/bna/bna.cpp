#include "bna.h"
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

BNA::BNA(){
	m_nInput = 1; // Default
	m_nOutput = 1; // 16 bytes of md5 hash
}

// ----------------------------------------------------------------

bool BNA::load(QString filename){
	QFile file(filename);
	if (!file.exists()) {
		qDebug().noquote().nospace() << "BNA:  File did not exists: " << filename;
		return false;
	}
	if (!file.open(QIODevice::ReadOnly) ) {
		qDebug().noquote().nospace() << "BNA: Could not open file " << filename;
		return false;
	}
	QDataStream stream(&file);

	stream >> m_nInput;
	stream >> m_nOutput;

	while(!stream.atEnd()){
		BNAItem item;
		stream >> item.X;
		stream >> item.Y;
		stream >> item.T;
		m_vItems.push_back(item);
	}
	normalize();
	return true;
};

// ----------------------------------------------------------------

bool BNA::save(QString filename){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename;
		return false;
	}
	QDataStream stream( &file );
	stream << m_nInput << m_nOutput;
	for (int i = 0; i < m_vItems.size(); i++) {
		stream << m_vItems[i].X << m_vItems[i].Y << m_vItems[i].T;
	}
	file.close();
	return true;
};

// ----------------------------------------------------------------

void BNA::normalize(){
	int nodes = m_nInput;
	for(int i = 0; i < m_vItems.size(); i++){
		m_vItems[i].X = m_vItems[i].X % nodes;
		m_vItems[i].Y = m_vItems[i].Y % nodes;
		m_vItems[i].T = m_vItems[i].T % 3;
		nodes++;
	}
}

// ----------------------------------------------------------------

void BNA::randomGenerate(int nInput, int nOutput, int nSize){
	m_nInput = nInput;
	m_nOutput = nOutput;
	nSize = nSize + m_nInput + m_nOutput;
	m_vItems.clear();
	for(int i = 0; i < nSize; i++){
		BNAItem item;
		item.X = qrand();
		item.Y = qrand();
		item.T = qrand();
		m_vItems.push_back(item);
	}
	normalize();
}

// ----------------------------------------------------------------

bool BNA::exportToDot(QString filename, QString graphname){
	QFile file(filename);
	if (file.exists()) {
		file.remove();
	}
	if ( !file.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename;
		return false;
	}
	QTextStream stream( &file );
	stream << "digraph " << graphname << " {\n";
	int nodes = m_nInput;

	for(int i = 0; i < m_vItems.size(); i++){
		QString sX = (m_vItems[i].X < m_nInput ? "in" : "node") + QString::number(m_vItems[i].X);
		QString sY = (m_vItems[i].Y < m_nInput ? "in" : "node") + QString::number(m_vItems[i].Y);
		QString sNode = "node" + QString::number(nodes); 
		stream << "\t" << sX << " -> " << sNode << ";\n";
		stream << "\t" << sY << " -> " << sNode << ";\n";
		nodes++;
	}
	int out_nodes = nodes-m_nOutput-1;
	for(int i = out_nodes; i < nodes; i++){
		QString sOut = "out" + QString::number(i-out_nodes);
		QString sNode = "node" + QString::number(i);
		stream << sNode << " -> " << sOut << ";\n";
	}
	stream << "}\n";
	file.close();
	return true;
}

// ----------------------------------------------------------------

bool BNA::exportToCpp(QString filename, QString funcname){
	QFile file(filename);
	QFileInfo fi(filename);
	if(fi.suffix() != "cpp"){
		qDebug().noquote().nospace() << "[ERROR]" << filename << " file must be have suffix 'cpp'";
		return false;
	}
	
	QString filename_h = filename.left(filename.length() - 3);
	filename_h += "h";
	
	if (file.exists()) {
		file.remove();
	}
	
	QFile file_h(filename_h);
	if (file_h.exists()) {
		file_h.remove();
	}
	
	if ( !file.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename;
		return false;
	}
	
	if ( !file_h.open(QIODevice::WriteOnly) ) {
		qDebug().noquote().nospace() << "Could not write file: " << filename_h;
		return false;
	}
	
	QTextStream stream_h( &file_h );
	stream_h << "#ifndef BNA_MD5_" << funcname.toUpper() << "_H\r\n";
	stream_h << "#define BNA_MD5_" << funcname.toUpper() << "_H\r\n\r\n";
	stream_h << "void " << funcname << "(";
	
	QTextStream stream( &file );
	stream << "#include \"" << fi.baseName() << ".h\"\r\n";
	stream << "void " << funcname << "(";
	for(unsigned int i = 0; i < m_nInput; i++){
		stream << "\r\n\tbool in" << i << ", ";
		stream_h << "\r\n\tbool in" << i << ", ";
	}
	
	for(unsigned  int i = 0; i < m_nOutput; i++){
		stream << "\r\n\tbool &out" << i;
		stream_h << "\r\n\tbool &out" << i;
		if(i < m_nOutput-1){
			stream << ", ";
			stream_h << ", ";
		}
	}
	stream << "\r\n) {\r\n";
	stream_h << "\r\n);\r\n\r\n";
	stream_h << "#endif //BNA_MD5_" << funcname.toUpper() << "_H\r\n";
	
	
	int nodes = m_nInput;
	for(int i = 0; i < m_vItems.size(); i++){
		QString sX = (m_vItems[i].X < m_nInput ? "in" : "node") + QString::number(m_vItems[i].X);
		QString sY = (m_vItems[i].Y < m_nInput ? "in" : "node") + QString::number(m_vItems[i].Y);
		QString sNode = "node" + QString::number(nodes); 
		stream << "\tbool " << sNode << " = " << sX << "|" << sY << ";\n";
		nodes++;
	}
	int out_nodes = nodes-m_nOutput;
	
	for(int i = out_nodes; i < nodes; i++){
		QString sOut = "out" + QString::number(i-out_nodes);
		QString sNode = "node" + QString::number(i);
		stream << "\t" << sOut << " = " << sNode << ";\n";
	}

	stream << "}\n";
	file.close();
	return true;
}

// ----------------------------------------------------------------

