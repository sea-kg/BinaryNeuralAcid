#include "line.h"

namespace crack_hash {

    // ----------------------------------------------------------------

    Line::Line() {
        m_sFrom = "";
        m_sTo = "";
        m_sLabel = "";
    }

    // ----------------------------------------------------------------

    Line::Line(QString sFrom, QString sTo, QString sLabel) {
        m_sFrom = sFrom;
        m_sTo = sTo;
        m_sLabel = sLabel;
    }

    // ----------------------------------------------------------------

    Line::Line(QString sFrom, QString sTo) {
        m_sFrom = sFrom;
        m_sTo = sTo;
        m_sLabel = "";
    }

    // ----------------------------------------------------------------

    QString Line::getDotString() {
        return m_sFrom + " -> " + m_sTo + " [ label = \"" + m_sLabel + "\" ];";
        // return m_sFrom + " -> " + m_sTo + ";";
    }

    // ----------------------------------------------------------------

    QString Line::from() const {
        return m_sFrom;
    }

    // ----------------------------------------------------------------

    QString Line::to() const {
        return m_sTo;
    }

    // ----------------------------------------------------------------

    QString Line::label() const {
        return m_sLabel;
    }

    // ----------------------------------------------------------------

    bool Line::equal(const Line &line) {
        return line.from() == m_sFrom && line.to() == m_sTo;
    }

    // ----------------------------------------------------------------
}
