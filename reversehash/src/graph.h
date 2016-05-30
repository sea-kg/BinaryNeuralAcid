#ifndef CRACK_HASH_H
#define CRACK_HASH_H

#include <QVector>
#include <QString>
#include <QMap>
#include "memory.h"
#include "line.h"

namespace crack_hash {

class Graph;

// ----------------------------------------------------------------

class Graph {
	public:
		Graph();
		void addPoint(QString name);
		void addLine(Line line);
		void removeLine(Line line);
		bool containsLine(Line line);
		bool containsPoint(QString name);
		void searchParents(QString name, QVector<QString> &parents);
		void searchLinesTo(QString name, QVector<Line> &lines);
		void searchChildren(QString name, QVector<QString> &children);
		void removeCycles(QString rootName);
		void removeMoreThan2();
		void o00hasALotOfLines();
		void removeRandomLines(int nCount);
		void addRandomLines(int nCount);
		void mHasNotParents();
		void iHasNotChildren();
		void mHasNotChildren();
		
		void generateRandomGraph();
		void modifyRandomGraph();
		QString generateOperation();

		void exportToFileDot(QString fileName);
		void save(QString fileName);
		void load(QString fileName);

	private:
		void removeCyclesRecoursive(QVector<QString> vPath);
	
		QVector<QString> m_vPoints;
		QVector<Line> m_vLines;
};

// ----------------------------------------------------------------

class Generator {
	
};

// ----------------------------------------------------------------

class Calculate {
	public:
		Calculate(Graph *);
		bool calc(QString node, QMap<QString, bool> input);
		bool getValue(QString node);
		void setValue(QString node, bool bValue);
	private:
		Graph *m_pGraph;
		QMap<QString, bool> m_mapCache;
};

// ----------------------------------------------------------------

}; // namespace crack_hash
#endif // CRACK_HASH_H

