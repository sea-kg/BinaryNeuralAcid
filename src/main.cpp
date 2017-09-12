#include <iostream>
#include "vertex_graph.h"
#include "helpers.h"
#include "memory.h"
#include "memoryitem.h"
#include "tests/tests.h"
#include "server/websocketserver.h"
#include <QString>
#include <QList>
#include <QFile>
#include <QTime>
#include <QDir>
#include <QVector>
#include <QTextStream>


#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QCoreApplication>
#include "tests/reverse_test.h"
#include "tests/random_remove_vertex_test.h"
#include "bna.h"

void print_help(QVector<QString> &vParams) {
	std::cout << "\n"
		<< "  Please usage: " << vParams[0].toStdString() << " [command] [parameters]\n"
		<< "\t --run-tests                           - run tests\n"
		<< "\t --run-reverse-test                    - run reverse test\n"
		<< "\t --run-random-remove-vertex-test       - run random remove vertex test\n"
		<< "\t --reset-persents                      - reset last persents from every bit and create missing files\n" 
        << "\t --server                              - start server\n"
        << "\t --test-bna                            - test bna\n"
        << "\t --sin-learning                        - sin learning\n"
        << "\n";
};

int main(int argc, char* argv[])
{
	// Seed the random generator with current time
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());

	QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("reversehash");
	
	QVector<QString> vParams;
	for(int i = 0; i < argc; i++){
		vParams.push_back(QString(argv[i]));
	}

    if(vParams.contains("--help")){
        print_help(vParams);
        return 0;
    }

	if(vParams.contains("--run-tests")){
		runtests();
		return 0;
	}
	
	if(vParams.contains("--run-reverse-test")){
		IReverseHashTest *pTest = new Reverse_Test();
		qDebug().nospace().noquote() << " Run " << pTest->name() << " ... ";
		if(!pTest->run()){
			qDebug().nospace().noquote() << "\t  ->  [FAIL] \n";
		}else{
			qDebug().nospace().noquote() << "\t  ->  [OK] \n";
		}
		return 0;
	}
		
	if(vParams.contains("--bna-prepare-md5")){
		QDir dir(".");
		dir.mkpath("bna/md5");
		return 0;
	}
		
	if(vParams.contains("--test-bna")){
		BNA bna;
		bna.randomGenerate(16,2,100);
		bna.save("test/test.bna");
		bna.exportToDot("test/test.dot", "func16");
		bna.exportToCpp("test/test.cpp", "func16");
		return 0;
	}
	
	if(vParams.contains("--run-random-remove-vertex-test")){
		IReverseHashTest *pTest = new RandomRemoveVertex_Test();
		qDebug().nospace().noquote() << " Run " << pTest->name() << " ... ";
		for(int i = 0; i < 5; i++){
			if(!pTest->run()){
				qDebug().nospace().noquote() << "\t  ->  [FAIL] \n";
			}else{
				qDebug().nospace().noquote() << "\t  ->  [OK] \n";
			}
		}
		return 0;
	}

	// reset persents
	if(vParams.contains("--reset-persents")){
		int nCount = 55*8;
		for (int i = 0; i < nCount; i++) {
			QString filename = "/usr/share/reversehashd/md5/bit" + QString::number(i).rightJustified(3, '0') + ".vertexgraph";
			VertexGraph pVertexGraph(128);
			QFile file(filename);
			if (!file.exists()) {
				pVertexGraph.genBase();
				pVertexGraph.saveToFile(filename);
				std::cout << "Created new file: " << filename.toStdString() << "\n";
			}else{
				pVertexGraph.loadFromFile(filename);
				std::cout << "Loaded file: " << filename.toStdString() << "\n";
			}
			pVertexGraph.setLastSuccessPersents(0);
			pVertexGraph.saveToFile(filename);
		}
		return 0;
	}
	
	if(vParams.contains("--server")){
		qDebug() << "Server starting on 888 port";
		WebSocketServer *server = new WebSocketServer(43735, true);
		QObject::connect(server, &WebSocketServer::closed, &app, &QCoreApplication::quit);
		
		return app.exec();
	}

	print_help(vParams);
    return 0;
}
