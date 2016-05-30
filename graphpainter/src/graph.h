#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QString>
#include <QMap>

namespace graph {

class Graph;
class Line;

// ----------------------------------------------------------------

class Line {
	public:
		Line();
		Line(QString, QString, QString);
		Line(QString, QString);
		QString getDotString();
		QString from() const;
		QString to() const;
		QString label() const;
		bool equal(const Line &line);
	private:
		QString m_sFrom;
		QString m_sTo;
		QString m_sLabel;
};

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
		
		void generateRandomGraph();
		void modifyRandomGraph();
		QString generateOperation();

		void exportToFileDot(QString fileName);
		void save(QString fileName);
		void load(QString fileName);
		const QVector<QString> &points();
		const QVector<Line> &lines();

	private:
		void removeCyclesRecoursive(QVector<QString> vPath);
	
		QVector<QString> m_vPoints;
		QVector<Line> m_vLines;
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

}; // namespace graph
#endif // GRAPH_H

