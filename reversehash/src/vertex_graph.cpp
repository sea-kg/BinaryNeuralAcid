#include "vertex_graph.h"
#include "vertexes/vertex_in.h"
#include "vertexes/vertex.h"
#include <iostream>
#include <cstring>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QTextStream>

namespace reversehash {
	VertexGraph::VertexGraph(int nInputs){
		m_pOut = NULL;
		m_nInputs = nInputs;
        m_nVersion = 1;
	};
	
	// -----------------------------------------------------------------
	
	void VertexGraph::genBase(){
        m_vVertexIn.clear();
        m_pOut = NULL;
        m_vVertexes.clear();

		QVector<IVertexOut *> vTmp;
		for(int i = 0; i < m_nInputs; i++){
			IVertexIn *pVertexIn = new VertexIn();
			pVertexIn->setValue(false);
            pVertexIn->setNumber(i);
			IVertexOut *pVertexOut = dynamic_cast<IVertexOut *>(pVertexIn);
			pVertexOut->setName("i" + QString::number(i));
			m_vVertexIn.push_back(pVertexIn);
			m_vVertexes.push_back(pVertexOut);
			vTmp.push_back(pVertexOut);
		}
		int mI = 0;
		while(vTmp.size() > 2){
			QVector<IVertexOut *> vTmp2;
			for(int i = 0; i < vTmp.size()/2; i++){
				IVertexOut *pVertexOut = new Vertex();
				pVertexOut->setName("m" + QString::number(mI));
				mI++;
				IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
				pVertexOperation->setOperation("|");
				pVertexOperation->setIn1(vTmp[i*2]);
				pVertexOperation->setIn2(vTmp[i*2+1]);
				vTmp2.push_back(pVertexOut);
				m_vVertexes.push_back(pVertexOut);
			}
			vTmp.clear();
			for(int i = 0; i < vTmp2.size(); i++){
				vTmp.push_back(vTmp2[i]);
			}
			vTmp2.clear();
		}
		m_pOut = new Vertex();
		m_pOut->setName("out");
		m_vVertexes.push_back(m_pOut);
		IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(m_pOut);
		pVertexOperation->setIn1(vTmp[0]);
		pVertexOperation->setIn2(vTmp[1]);
        pVertexOperation->setOperation("|");
	};

	// -----------------------------------------------------------------
	
	bool VertexGraph::out(){
		return m_pOut->out();
	}
	
	// -----------------------------------------------------------------
	
	void addLine(QString &result, IVertexOut *pTo, IVertexOut *pFrom){
		
		if(pTo->type() == "Vertex"){
			result += "\t" + pFrom->name() + " ->" + pTo->name() + ";\n";
			if(pFrom->type() == "VertexIn"){
				result += "\t" + pFrom->name() + " [ label = \"" + pFrom->name() + "\" ];\n";	
			}else{
				IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pFrom);
				result += "\t" + pFrom->name() + " [ label = \"" + pVertexOperation->operation() + "\" ];\n";
				addLine(result, pFrom, pVertexOperation->in1());
				addLine(result, pFrom, pVertexOperation->in2());
			}
		}
	}
	
	// -----------------------------------------------------------------
	
	QString VertexGraph::conv2dot(){
		QString result = "";
		result += "digraph reversehash {\n";
		result += "\t" + m_pOut->name() + " [ label = \"out\" ];\n";
		
		IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(m_pOut);
		addLine(result, m_pOut, pVertexOperation->in1());
		addLine(result, m_pOut, pVertexOperation->in2());

		result += "}\n";
		return result;
	}
	
    // -----------------------------------------------------------------

    void VertexGraph::writeHeader(QDataStream &stream, int nVersion){
        stream.writeRawData("VERTEXGRAPH", 11);
        stream << nVersion; // version
    }

    // -----------------------------------------------------------------

    bool VertexGraph::readHeader(QDataStream &stream, int &nVersion){
        char *pFileType = new char[12];
        std::memset(pFileType, 0, 12);
        int nReaded = stream.readRawData(pFileType, 11);
        if(nReaded > 0){
            if(QString(pFileType) != "VERTEXGRAPH"){
                std::cerr << "VERTEXGRAPH: File type did not match with VERTEXGRAPH.\n";
                return false;
            }
        }else{
            std::cerr << "VERTEXGRAPH: Could not read file (1)\n";
            return false;
        }
        stream >> nVersion;
        return true;
    }

    // -----------------------------------------------------------------

    void VertexGraph::writeDataAsVersion1(QDataStream &stream){
        stream << m_vVertexes.size();
        for (int i = 0; i < m_vVertexes.size(); i++) {
            reversehash::IVertexOut *pVertexOut = m_vVertexes[i];
            stream << pVertexOut->type().toUtf8();
            stream << pVertexOut->name().toUtf8();
            if(pVertexOut->type() == "Vertex"){
                IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
                stream << pVertexOperation->operation().toUtf8();
                stream << pVertexOperation->in1()->type().toUtf8();
                stream << pVertexOperation->in1()->name().toUtf8();
                stream << pVertexOperation->in2()->type().toUtf8();
                stream << pVertexOperation->in2()->name().toUtf8();
            }else if(pVertexOut->type() == "VertexIn"){
                IVertexIn *pVertexIn = dynamic_cast<IVertexIn *>(pVertexOut);
                stream << pVertexIn->number();
            }
        }
    }

    // -----------------------------------------------------------------

    IVertexOut *VertexGraph::findVertexByName(QString name){
        IVertexOut *pResult = NULL;
        for(int i = 0; i < m_vVertexes.size(); i++){
            if(m_vVertexes[i]->name() == name){
                pResult = m_vVertexes[i];
            }
        }
        return pResult;
    }

    // -----------------------------------------------------------------

    void VertexGraph::readDataAsVersion1(QDataStream &stream){
        int nSize = 0;
        stream >> nSize;
        for(int i = 0; i < nSize; i++){
            QByteArray type;
            stream >> type;
            QByteArray name;
            stream >> name;
            if(type == "Vertex"){
                IVertexOut *pVertexOut = this->findVertexByName(QString(name));
                if(pVertexOut == NULL)
                    pVertexOut = new Vertex();
                pVertexOut->setName(QString(name));
                IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
                QByteArray operation;
                stream >> operation;
                pVertexOperation->setOperation(QString(operation));

                QByteArray in1type;
                QByteArray in1name;
                QByteArray in2type;
                QByteArray in2name;

                stream >> in1type;
                stream >> in1name;
                stream >> in2type;
                stream >> in2name;

                IVertexOut *pVertexIn1 = this->findVertexByName(QString(in1name));
                IVertexOut *pVertexIn2 = this->findVertexByName(QString(in2name));

                if(pVertexIn1 == NULL){
                    if(QString(in1type) == "Vertex"){
                        pVertexIn1 = new Vertex();
                    }else if(QString(in1type) == "VertexIn"){
                        pVertexIn1 = new VertexIn();
                    }
                    pVertexIn1->setName(QString(in1name));
                }

                if(pVertexIn2 == NULL){
                    if(QString(in2type) == "Vertex"){
                        pVertexIn2 = new Vertex();
                    }else if(QString(in2type) == "VertexIn"){
                        pVertexIn2 = new VertexIn();
                    }
                    pVertexIn2->setName(QString(in2name));
                }

                pVertexOperation->setIn1(pVertexIn1);
                pVertexOperation->setIn2(pVertexIn2);

                if(QString(name) == "out"){
                    m_pOut = pVertexOut;
                }
                m_vVertexes.push_back(pVertexOut);
            }else if(type == "VertexIn"){
                IVertexIn *pVertexIn = new VertexIn();
                int nNumber = -1;
                stream >> nNumber;
                m_vVertexIn.push_back(pVertexIn);
                IVertexOut *pVertexOut = dynamic_cast<IVertexOut *>(pVertexIn);
                pVertexOut->setName(QString(name));
                m_vVertexes.push_back(pVertexOut);
            }
        }
    }

    // -----------------------------------------------------------------

    bool VertexGraph::save(QString filename){
        QFile file(filename);
        if (file.exists()) {
            file.remove();
        }
        if (!file.open(QIODevice::WriteOnly)) {
            std::cerr << "Could not write file: " << filename.toStdString() << "\n";
            return false;
        }
        QDataStream stream( &file );
        this->writeHeader(stream, m_nVersion);
        this->writeDataAsVersion1(stream);
        file.close();
        return true;
    }

	// -----------------------------------------------------------------

	bool VertexGraph::saveDot(QString filename){
        QFile file(filename);
        if (file.exists()) {
            file.remove();
        }
        if (!file.open(QIODevice::WriteOnly)) {
            std::cerr << "Could not write file: " << filename.toStdString() << "\n";
            return false;
        }
        QTextStream stream( &file );
        stream << conv2dot();
        file.close();
        return true;
    }
    
	// -----------------------------------------------------------------

    bool VertexGraph::load(QString filename){
        m_vVertexIn.clear();
        m_pOut = NULL;
        m_vVertexes.clear();

        QFile file(filename);
        if (!file.exists()) {
            std::cerr << "File did not exists: " << filename.toStdString() << "\n";
            return false;
        }
        if ( !file.open(QIODevice::ReadOnly) ) {
            std::cerr << "Could not open file " << filename.toStdString() << "\n";
            return false;
        }
        QDataStream stream( &file );

        int nVersion = -1;
        if(!this->readHeader(stream, nVersion)){
            return false;
        }

        if(nVersion == 1){
            this->readDataAsVersion1(stream);
        }else{
            return false;
        }

        return true;
    }

    // -----------------------------------------------------------------
	void VertexGraph::randomChanges(int count){
		
		// replace operations
		QString opers = "|&^";
		for(int i = 0; i < count; i++){
			int n = qrand() % (m_vVertexes.size());
			IVertexOut *pVertexOut = m_vVertexes[n];
			if(pVertexOut->name() != "out" && pVertexOut->type() != "VertexIn"){
				IVertexOperation *pVertexOperation = dynamic_cast<IVertexOperation *>(pVertexOut);
				QString newOper = QString(opers[qrand() % opers.length()]);
				pVertexOperation->setOperation(newOper);
			}
		}
		
		// swaps mXXX
		for(int i = 0; i < count; i++){
			
		}
		
		// TODO: add
		
		// TODO: remove mXXX

		// TODO: check to cicles
	}
}
