#ifndef VERTEX_GRAPH_H
#define VERTEX_GRAPH_H


#include "interfaces/ivertex_in.h"
#include "interfaces/ivertex_out.h"

#include <QString>
#include <QVector>

namespace reversehash {
	class VertexGraph
    {
        public:
			VertexGraph(int nInputs);
			void genBase();
			void randomChanges(int n);
			bool out();
			QString conv2dot();
            bool save(QString filename);
            bool load(QString filename);
		private:
            void writeHeader(QDataStream &stream, int nVersion);
            bool readHeader(QDataStream &stream, int &nVersion);
            void writeDataAsVersion1(QDataStream &stream);
            void readDataAsVersion1(QDataStream &stream);
            IVertexOut *findVertexByName(QString name);

			QVector<IVertexIn *> m_vVertexIn;
			QVector<IVertexOut *> m_vVertexes;
			IVertexOut *m_pOut;
			int m_nInputs;
            int m_nVersion;
    };
}

#endif // VERTEX_GRAPH_H
