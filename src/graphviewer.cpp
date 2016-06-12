#include "ui/widget.h"
#include <QApplication>
// #include "graph.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	GLWidget w;
	w.loadGraph("test.vertexgraph");
	w.show();
	return app.exec();
}
