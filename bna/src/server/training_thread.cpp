#include "training_thread.h"

#include <bna.h>
#include <helpers.h>
#include <QFile>
#include <iostream>

TrainingThread::TrainingThread(IReverseHashDServer *pReverseHashDServer, BNAProject *pBNAProject){
	m_pReverseHashDServer = pReverseHashDServer;
	m_nSleep = 5;
    m_pBNAProject = pBNAProject;
    m_pLastMessage = NULL;
}

void TrainingThread::run(){
    std::cout << "Start Training Thread\n";

    // BNAMemory *pMemory = new BNAMemory(128, 440);
    // pMemory->load("tests_bna_md5/memory_md5_10000.bnamemory");
	

    int nMemorySize = m_pBNAProject->getBNAMemory()->size();
    int nOutputBits = m_pBNAProject->getOutputBits();
    int *pStatistics = new int[nOutputBits];
    for(int i = 0; i < nOutputBits; i++){
        pStatistics[i] = 0;
    }
	while(true){
        for(int bitid = 0; bitid < nOutputBits; bitid++){
            if(pStatistics[bitid] == 0){
                pStatistics[bitid] = m_pBNAProject->calculate(bitid, true);
            }
            int p = pStatistics[bitid];
            if(p == nMemorySize){
                continue;
            }

            std::cout << bitid << ": Learning start... : " << p << "/" << nMemorySize << "\n";

            {
                TrainingThreadMessage *pMsg = new TrainingThreadMessage(bitid);
                pMsg->setMessage("Training... ");
                this->sendMessage(pMsg);
            }

            BNA *pBNA = m_pBNAProject->getBNA(bitid);

            QByteArray prevBna = pBNA->exportToByteArray();
			int nExperiments = 0;
            while(nExperiments < 100 && p < nMemorySize){
				QThread::sleep(m_nSleep);
				nExperiments++;
				int nSuccessCount = 0;

                nSuccessCount = m_pBNAProject->calculate(bitid, true);

                if(nSuccessCount > p){
                    prevBna = pBNA->exportToByteArray();
                    m_pBNAProject->saveBNA(bitid);
                    pStatistics[bitid] = nSuccessCount;
                    p = nSuccessCount;
                    std::cout << bitid << " Updated to " << p << "/" << nMemorySize << "\n";
                    TrainingThreadMessage *pMsg = new TrainingThreadMessage(bitid);
                    pMsg->setMessage("New percent result set!");
                    pMsg->setLastSuccessPersents((p*100)/nMemorySize);
                    pMsg->setMaxExperiments(100);
                    pMsg->setCompletedExperiments(nExperiments);
                    this->sendMessage(pMsg);
				}else{
                    TrainingThreadMessage *pMsg = new TrainingThreadMessage(bitid);
                    pMsg->setMessage("Reloading bna.");
                    pMsg->setLastSuccessPersents((p*100)/nMemorySize);
                    this->sendMessage(pMsg);
                    pBNA->importFromByteArray(prevBna);
                    pBNA->generateRandomMutations(100);
				}
			}
			
            {
                TrainingThreadMessage *pMsg = new TrainingThreadMessage(bitid);
                pMsg->setMessage("Result: " + QString::number((p*100)/nMemorySize) + "%");
                pMsg->setMaxExperiments(100);
                pMsg->setCompletedExperiments(100);
                this->sendMessage(pMsg);
            }
			QThread::sleep(m_nSleep);
		}
		QThread::sleep(m_nSleep);
	}
    std::cout << "Stop Training Thread\n";
}

void TrainingThread::sendMessage(TrainingThreadMessage *pMsg){
    if(m_pLastMessage != NULL && m_pLastMessage->equals(pMsg)){
		return;
	}

    if(m_pLastMessage != NULL){
        // delete m_pLastMessage;
        m_pLastMessage = NULL;
    }
	
    m_pLastMessage = pMsg;
    std::cout << "Training Thread: [" << pMsg->id() << "] " << pMsg->message().toStdString() << "\n";

    m_pReverseHashDServer->sendToAll(pMsg->toJson());
}
