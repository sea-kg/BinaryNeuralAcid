#ifndef LINE_H
#define LINE_H
#include <QString>

namespace crack_hash {
    class Line {
        public:
            Line();
            Line(QString, QString, QString);
            Line(QString, QString);
            QString getDotString();
            QString from() const;
            QString to() const;
            QString label() const;
            bool equal(const Line &line);
        private:
            QString m_sFrom;
            QString m_sTo;
            QString m_sLabel;
    };
}

#endif // LINE_H
