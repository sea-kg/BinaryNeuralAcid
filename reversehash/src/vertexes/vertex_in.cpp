#include "vertex_in.h"
#include <iostream>


namespace reversehash {

	VertexIn::VertexIn(){
		m_bValue = false;
        m_nNumber = -1;
	};

	// -----------------------------------------------------------------

	bool VertexIn::out(){
		return m_bValue;
	};
	
	// -----------------------------------------------------------------
	
	QString VertexIn::type(){
		return QString("VertexIn");
	};
	
	// -----------------------------------------------------------------
	
	QString VertexIn::name(){
		return m_sName;
	};
	
	// -----------------------------------------------------------------
	
	void VertexIn::setName(QString sName){
		m_sName = sName;
	};
	
	// -----------------------------------------------------------------
	
    void VertexIn::setNumber(int n){
        m_nNumber = n;
    }

    // -----------------------------------------------------------------

    int VertexIn::number(){
        return m_nNumber;
    }

    // -----------------------------------------------------------------

	void VertexIn::setValue(bool bValue){
		m_bValue = bValue;
    }

}
