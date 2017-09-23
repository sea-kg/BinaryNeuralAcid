#include <iostream>
#include "vertex_graph.h"
#include "helpers.h"
#include "memory.h"
#include "memoryitem.h"
#include "tests/tests.h"
#include <reversehashd_server.h>
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

void print_help(QVector<QString> &vParams) {
	std::cout << "\n"
		<< "  Please usage: " << vParams[0].toStdString() << " [command] [parameters]\n"
		<< "\t --run-tests                           - run tests\n"
		<< "\t --run-reverse-test                    - run reverse test\n"
		<< "\t --run-random-remove-vertex-test       - run random remove vertex test\n"
		<< "\t --reset-persents                      - reset last persents from every bit and create missing files\n" 
        << "\t --server                              - start server\n"
        << "\t --generate-md5-bna                    - test bna\n"
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
	
	if(vParams.contains("--gencode")){
		
		/*for(int i = 0; i < 128; i++){
			qDebug().noquote().nospace() << "\tbool in" + QString::number(i) + " = hash_bool[" + QString::number(i) + "];";
		}*/
		
		for(int i = 0; i < 440; i++){
			QString name = QString::number(i).rightJustified(3, '0');
			qDebug().noquote().nospace() << "\tbool out" << name << ";";
			
			QString args_ = "";
			for(int i_ = 0; i_ < 128; i_++){
				args_ += "in" + QString::number(i_) + ", ";
			}
			args_ += "out" + name;
			qDebug().noquote().nospace() << "\tfunc" << name << "(" << args_ << ");";
			qDebug().noquote().nospace() << "\tresult_bool[" << QString::number(i) << "] = out" << name << ";";
		}
		
		/*for(int i = 0; i < 440; i++){
			QString name = QString::number(i).rightJustified(3, '0');
			QString subdir = name[0] + "/" + name[1] + "/" + name[2];
			qDebug().noquote().nospace() << "#include \"" << subdir << "/" << name << ".h\"";
		}*/
		
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
		qDebug() << "Server starting on 43735 port";
		ReverseHashDServer *server = new ReverseHashDServer(43735, true);
		QObject::connect(server, &ReverseHashDServer::closed, &app, &QCoreApplication::quit);
		
		return app.exec();
	}

	print_help(vParams);
    return 0;
}

