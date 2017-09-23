#ifndef BNA_H
#define BNA_H

#include<QString>
#include<QVector>

class BNA {
	public:
		BNA();
		bool load(QString filename);
		bool save(QString filename);
		void randomGenerate(int nInput, int nOutput, int nSize);
		bool exportToDot(QString filename, QString graphname);
		bool exportToCpp(QString filename, QString funcname);

	private:
		unsigned int m_nInput;
		unsigned int m_nOutput;
		void normalize();
		struct BNAItem{
			BNAItem(unsigned short x, unsigned short y, unsigned char t) : X(x), Y(y), T(t){};
			BNAItem() : X(0), Y(0), T(0){};
			unsigned short X;
			unsigned short Y;
			unsigned char T;
		};
		QVector<BNAItem> m_vItems;
};

#endif // BNA_H
