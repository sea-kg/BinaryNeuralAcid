#include "create_cmd_handlers.h"
#include "include/cmd_hello_handler.h"
#include "include/cmd_reverse_handler.h"
#include "include/cmd_statistics_handler.h"
#include "include/cmd_training_handler.h"

void create_cmd_handlers(QMap<QString, ICmdHandler *> &pHandlers){
	QVector<ICmdHandler *> v;
	v.push_back(new CmdHelloHandler());
	v.push_back(new CmdReverseHandler());
	v.push_back(new CmdStatisticsHandler());
	v.push_back(new CmdTrainingHandler());

	for(int i = 0; i < v.size(); i++){
		QString cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
			qDebug() << "[WARNING] cmd_handler for command " << cmd << " - already registered and will be skipped";	
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
