#ifndef BNA_H
#define BNA_H

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <bna_expr.h>
#include <bna_item.h>

class BNA {
	public:
		BNA();
		bool load(QString filename);
		bool save(QString filename);
		void randomGenerate(int nInput, int nOutput, int nSize);
		bool exportToDot(QString filename, QString graphname);
		bool exportToCpp(QString filename, QString funcname);
		QJsonObject toJson();
		void generateRandomMutations();
        bool calc(const QVector<bool> &vInputs, int nOutput);
	private:

		unsigned int m_nInput;
		unsigned int m_nOutput;
		void normalize();
		QVector<BNAItem> m_vItems;

        void updateExprs();
        QVector<BNAExpr *> m_vCalcExprs;
        QVector<BNAVar *> m_vCalcVars;
        QVector<BNAVar *> m_vCalcOutVars;
};

#endif // BNA_H
