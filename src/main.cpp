#include <iostream>
#include "helpers.h"
#include "memory.h"
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
#include "tests/bnaoper_test.h"
#include "tests/reverse_test.h"
#include "tests/learning_test.h"

void print_help(QVector<QString> &vParams) {
	std::cout << "\n"
		<< "  Please usage: " << vParams[0].toStdString() << " [command] [parameters]\n"
		<< "\t --run-tests                           - run tests\n"
        << "\t --run-learning-test                   - run learning test\n"
		<< "\t --run-random-remove-vertex-test       - run random remove vertex test\n"
		<< "\t --reset-persents                      - reset last persents from every bit and create missing files\n" 
        << "\t --server                              - start server\n"
        << "\t --generate-md5-bna                    - test bna\n"
        << "\n";
};

int main(int argc, char* argv[])
{
	// Seed the random generator with current time
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

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

    if(vParams.contains("--run-learning-test")){
        IReverseHashTest *pTest = new Learning_Test();
        std::cout << " Run " << pTest->name().toStdString() << " ... \n";
        if(!pTest->run()){
            std::cout << "\t  ->  [FAIL] \n";
        }else{
            std::cout << "\t  ->  [OK] \n";
        }
        IReverseHashTest *pTest2 = new Reverse_Test();
        std::cout << " Run " << pTest2->name().toStdString() << " ... \n";
        if(!pTest2->run()){
            std::cout << "\t  ->  [FAIL] \n";
        }else{
            std::cout << "\t  ->  [OK] \n";
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

