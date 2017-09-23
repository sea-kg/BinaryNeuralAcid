#ifndef VERTEX_GRAPH_H
#define VERTEX_GRAPH_H


#include <ivertex_in.h>
#include <ivertex_out.h>

#include <QString>
#include <QVector>
#include <QJsonArray>

using namespace reversehash;

class VertexGraph
{
	public:
		VertexGraph(int nInputs);
		~VertexGraph();

		void genBase();
		
		bool out();
		reversehash::IVertexOut *vertexOut();
		void setIn(const QVector<bool> &in);
		QString conv2dot();
		QJsonArray conv2json();
		bool saveToFile(QString filename);
		bool saveToStream(QDataStream &stream);
		bool saveDot(QString filename);
		bool loadFromFile(QString filename);
		bool loadFromStream(QDataStream &stream);
	   
		void setLastSuccessPersents(int nVal);
		int lastSuccessPersents();
		
		VertexGraph *clone();
		void copy(VertexGraph *pVertexGraph);

		bool findCicles();

		int countOfVertextes();
	   
		// modifications
		void randomChanges(int n);
		void changeRandomOperation();
		void swapRandomVertextIns();
		void randomRemoveVertex();
		void randomAddVertex();

		// visualization
		void recalculateCoordinates();

	private:
		void writeHeader(QDataStream &stream, int nVersion);
		bool readHeader(QDataStream &stream, int &nVersion);
		void writeDataAsVersion1(QDataStream &stream);
		void readDataAsVersion1(QDataStream &stream);
		IVertexOut *findVertexByName(QString name);
		bool findCiclesRecourse(IVertexOut *pVertexOut, QVector<IVertexOut *> &stack);
		bool findIntersectionRecourse(IVertexOut *pVertexStart, IVertexOut *pVertexSearch);
		IVertexOut *findRandomVertex();
		void printStackVertexes(QVector<IVertexOut *> &stack);
		
		QVector<IVertexIn *> m_vVertexIn;
		QVector<IVertexOut *> m_vVertexes;
		IVertexOut *m_pOut;
		int m_nInputs;
		int m_nVersion;
		int m_nLastSuccessPersents;
		QString m_sFilename;
};


#endif // VERTEX_GRAPH_H
