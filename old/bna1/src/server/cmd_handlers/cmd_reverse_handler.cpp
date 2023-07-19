#include <cmd_reverse_handler.h>
#include <bna.h>
#include <memory.h>
#include <QFile>
#include <QJsonDocument>

QString CmdReverseHandler::cmd(){
	return "reverse";
}

void CmdReverseHandler::handle(QWebSocket *pClient, IReverseHashDServer *pReverseHashDServer, QJsonObject req){
	int rid = 0;
	if(req.contains("id")){
		rid = req["id"].toInt();
	}

    QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["rid"] = rid;
	
	if(!req.contains("md5")){
		pReverseHashDServer->sendMessageError(pClient, cmd(), rid, Error(400, "Not found parameter 'md5'"));
		return;
	}
    QString hash = req["md5"].toString();
    QVector<BNABit> vOutput;
    QVector<BNABit> vInputs;
    QString answer_bin = "";
    BNAConvertHEXStringToVBool(hash, vInputs, 128);
    BinaryNeuralAcidProject *pBNAProject = pReverseHashDServer->getBNAProject();
    int nCount = pBNAProject->getOutputBits();
    for (int bitid = 0; bitid < nCount; bitid++) {
        BNABit bResult = B_0;
        BinaryNeuralAcid *pBNA = pBNAProject->getBNA(bitid);
        bResult = pBNA->calc(vInputs, 0);
        vOutput.push_back(bResult);
        answer_bin += (bResult == B_1 ? "1" : "0");
	}
	jsonData["output_count"] = vOutput.size();
    QString answer_hex = BNAConvertVBoolHEXString(vOutput);
	QByteArray text = QByteArray::fromHex(answer_hex.toLatin1());
	jsonData["answer_bin"] = answer_bin;
	jsonData["answer_text"] = QString(text);
	jsonData["answer_hex"] = answer_hex;
	jsonData["answer_base64"] = QString(text.toBase64());
	jsonData["request_md5"] = hash; // TODO check comeback reverse
	QByteArray result_md5 = QCryptographicHash::hash(text, QCryptographicHash::Md5);
	jsonData["result_md5"] = QString(result_md5.toHex());

    QJsonObject jsonData2;
    // step2
    {
        QString sText_bin = BNAConvertHexToBin(hash);
        QByteArray md5Text = QCryptographicHash::hash(text, QCryptographicHash::Md5);
        QString sMd5Text = QString(md5Text.toHex());

        int nPrevResult = BNACalculateBinDistance(BNAConvertHexToBin(hash), BNAConvertHexToBin(sMd5Text));

        // bool
        bool bExists = true;
        while(bExists){
            bExists = false;
            QByteArray textNew = BNATryBrutFast1(text, hash);
            textNew = BNATryBrutFast2(textNew, hash);

            QByteArray md5TextNew = QCryptographicHash::hash(textNew, QCryptographicHash::Md5);
            QString sMd5TextNew = QString(md5TextNew.toHex());

            QString sTextNew_bin = BNAConvertHexToBin(sMd5TextNew);
            int nNewResult = BNACalculateBinDistance(sText_bin, sTextNew_bin);
            if(nNewResult > nPrevResult){
                bExists = true;
                text = QByteArray(textNew);
                nPrevResult = nNewResult;
                // std::cout << "Distance: " << nPrevResult << "\n";
            }
        }

        text = QString(text).toLatin1();
        md5Text = QCryptographicHash::hash(text, QCryptographicHash::Md5);
        sMd5Text = QString(md5Text.toHex());

        nPrevResult = BNACalculateBinDistance(BNAConvertHexToBin(hash), BNAConvertHexToBin(sMd5Text));
        // std::cout << "Start: " << nPrevResult << "\n";

        bExists = true;
        while(bExists){
            bExists = false;
            QByteArray textNew = BNATryBrutFast1(text, hash);
            textNew = BNATryBrutFast2(textNew, hash);

            QByteArray md5TextNew = QCryptographicHash::hash(textNew, QCryptographicHash::Md5);
            QString sMd5TextNew = QString(md5TextNew.toHex());

            QString sTextNew_bin = BNAConvertHexToBin(sMd5TextNew);

            int nNewResult = BNACalculateBinDistance(sText_bin, sTextNew_bin);
            if(nNewResult > nPrevResult){
                bExists = true;
                text = QByteArray(textNew);
                nPrevResult = nNewResult;
                // std::cout << "Distance: " << nPrevResult << "\n";
            }
        }

        jsonData2["answer_text"] = QString(text);
        QByteArray result_md5 = QCryptographicHash::hash(text, QCryptographicHash::Md5);
        jsonData2["result_md5"] = QString(result_md5.toHex());
        jsonData2["distance"] = nPrevResult;
        jsonData["step2"] = jsonData2;
    }


    /*QString alphabet = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890[]{}:,.<>/?\"'\\*&^%$#!-+=";
    for(int i = 0; i < text.size(); i++){
        char ch = text.at(i);
        // if(alphabet.contains(QChar(ch));
    }*/

	QJsonDocument doc(jsonData);
	QString message = doc.toJson(QJsonDocument::Compact);
	qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] >>> " << message;
	pReverseHashDServer->sendMessage(pClient, jsonData);
}
