#ifndef BNA_H
#define BNA_H

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QMap>

class BNAVar{
    public:
        BNAVar();
        QString name();
        void name(QString name);
        bool val();
        void val(bool bVal);
    private:
        bool m_bVal;
        QString m_sName;
};

// -----------------------------------------------------------------

class IBNAOper {
    public:
        virtual QString type() = 0;
        virtual bool calc(bool b1, bool b2) = 0;
};

// -----------------------------------------------------------------

class BNAOperXor : public IBNAOper{
    public:
        virtual QString type();
        virtual bool calc(bool b1, bool b2);
};

// -----------------------------------------------------------------

class BNAOperNotXor : public IBNAOper{
    public:
        virtual QString type();
        virtual bool calc(bool b1, bool b2);
};

// -----------------------------------------------------------------

class BNAOperAnd : public IBNAOper {
    public:
        virtual QString type();
        virtual bool calc(bool b1, bool b2);
};

// -----------------------------------------------------------------

class BNAOperOr : public IBNAOper {
    public:
        virtual QString type();
        virtual bool calc(bool b1, bool b2);
};

// -----------------------------------------------------------------

class BNAExpr{
    public:
        BNAExpr();
        void op1(BNAVar *pVar1);
        BNAVar *op1();
        void op2(BNAVar *pVar2);
        BNAVar *op2();
        void oper(IBNAOper *pOper);
        IBNAOper *oper();
        void out(BNAVar *pVarOut);
        BNAVar *out();
        void exec();

    private:
        BNAVar *m_pVar1;
        BNAVar *m_pVar2;
        BNAVar *m_pVarOut;
        IBNAOper *m_pOper;
};

// -----------------------------------------------------------------

class BNAItem{
    public:
        BNAItem(unsigned short x, unsigned short y, unsigned char t);
        BNAItem();
        unsigned short getX();
        unsigned short getY();
        unsigned char getT();

        void setX(unsigned short x);
        void setY(unsigned short y);
        void setT(unsigned char t);

        void readXYT(QDataStream &stream);
        void writeXYT(QDataStream &stream);

    private:
        unsigned short m_nX;
        unsigned short m_nY;
        unsigned char m_cT;
};

// -----------------------------------------------------------------

class BNA {
	public:
		BNA();
        ~BNA();
		bool load(QString filename);
		bool save(QString filename);
		void randomGenerate(int nInput, int nOutput, int nSize);
		bool exportToDot(QString filename, QString graphname);
		bool exportToCpp(QString filename, QString funcname);
        QByteArray exportToByteArray();
        void importFromByteArray(QByteArray &data);
		QJsonObject toJson();
        void generateRandomMutations(int nRandomCicles);
        void appendRandomData(int nRandomCicles);
        bool calc(const QVector<bool> &vInputs, int nOutput);
	private:

		unsigned int m_nInput;
		unsigned int m_nOutput;
		void normalize();
		QVector<BNAItem> m_vItems;
        QVector<IBNAOper *> m_vOpers;
        int m_nOperSize;

        void clearResources();
        void updateExprs();
        QVector<BNAExpr *> m_vCalcExprs;
        QVector<BNAVar *> m_vCalcVars;
        QVector<BNAVar *> m_vCalcOutVars;

};

#endif // BNA_H
