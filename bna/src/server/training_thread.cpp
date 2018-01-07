#include "training_thread.h"

#include <bna.h>
#include <helpers.h>
#include <QFile>
#include <iostream>

TrainingThread::TrainingThread(IReverseHashDServer *pReverseHashDServer){
	m_pReverseHashDServer = pReverseHashDServer;
	m_nSleep = 5;
}

void TrainingThread::run(){
    std::cout << "Start Training Thread\n";
    BNAMemory *pMemory = new BNAMemory();
    pMemory->load("tests_bna_md5/memory_md5_10000.bnamemory");
	
	while(true){
		QVector<TrainingThreadItem *> list = this->sortedList();
		for(int i = 0; i < list.size(); i++){
			TrainingThreadItem *pItem = list[i];
			
			QString bitid = pItem->bitid();
			QString filename = pItem->filename();
			QFile file(filename);
			if (!file.exists()) {
				TrainingThreadMessage msg(pItem);
				msg.setMessage(filename + " file did not found");
				this->sendMessage(msg);
				QThread::sleep(m_nSleep);
				continue;
			}
			BNA bna;
			if(!bna.load(filename)){
                std::cout << "Could not load file " << filename.toStdString() << "\n";
				continue;
			}

			{
				TrainingThreadMessage msg(pItem);
				msg.setMessage("Training... ");
				this->sendMessage(msg);
			}

			int nMemorySize = pMemory->size();
			
			int nPersent = pItem->percent();
			int nExperiments = 0;
			while(nPersent < 90 && nExperiments < 100){
				QThread::sleep(m_nSleep);
				nExperiments++;
				int nSuccessCount = 0;

				for (int t = 0; t < nMemorySize; t++){
                    BNAMemoryItem memoryItem = pMemory->at(t);
                    QVector<BNABit> vInputs = memoryItem.outputToVectorBool();

                    bool b = bna.calc(vInputs, 0);
					if(b == memoryItem.inputToVectorBool()[pItem->id()]){
						nSuccessCount++;
					}
					
					if(t > 0 && t % 1000 == 0){
						nPersent = (nSuccessCount * 100) / (t);

						TrainingThreadMessage msg(pItem);
						msg.setMessage("Training...");
						msg.setMaxExperiments(100);
						msg.setCompletedExperiments(nExperiments);
						msg.setPercent(nPersent);
						this->sendMessage(msg);
						
						QThread::sleep(1);
					}
				}

				nPersent = (nSuccessCount * 100) / (nMemorySize);
				if(nPersent > pItem->percent()){
					TrainingThreadMessage msg(pItem);
					msg.setMessage("New percent result set!");
					msg.setLastSuccessPersents(nPersent);
					msg.setMaxExperiments(100);
					msg.setCompletedExperiments(nExperiments);
					this->sendMessage(msg);
					
					pItem->savePercent(nPersent);
				}else{
					TrainingThreadMessage msg(pItem);
					msg.setMessage("Reloading graph.");
					msg.setLastSuccessPersents(pItem->percent());
					this->sendMessage(msg);
					
					bna.load(filename);
					nPersent = pItem->percent();
                    bna.generateRandomMutations(10);
				}
			}
			
			{
				TrainingThreadMessage msg(pItem);
				msg.setMessage("Result: " + QString::number(pItem->percent()) + "%");
				msg.setMaxExperiments(100);
				msg.setCompletedExperiments(100);
				this->sendMessage(msg);
			}
			QThread::sleep(m_nSleep);
		}
		QThread::sleep(m_nSleep);
	}
	qDebug().noquote().nospace() << "Stop Training Thread";
}

void TrainingThread::sendMessage(TrainingThreadMessage &msg){
	if(m_lastMessage.equals(msg)){
		return;
	}
	
	m_lastMessage = msg;
	qDebug().noquote().nospace() << "Training Thread: [" << msg.bitid() << "] " << msg.message();

	m_pReverseHashDServer->sendToAll(msg.toJson());
}

bool sortFunctionTTI( const TrainingThreadItem * e1, const TrainingThreadItem * e2 ) {
	if(e1->percent() < e2->percent()){
		return true;
	}else if(e1->percent() > e2->percent()){
		return false;
	}
    return e1->id() < e2->id();
}

QVector<TrainingThreadItem *> TrainingThread::sortedList(){
	QVector<TrainingThreadItem *> result;
	int nCount = 55*8;
	for (int i = 0; i < nCount; i++) {
		result.push_back(new TrainingThreadItem(i));
	}
	qSort ( result.begin(), result.end(), sortFunctionTTI );
	return result;
}
