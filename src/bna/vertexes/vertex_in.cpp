#include "vertex_in.h"
#include <iostream>


namespace reversehash {

	VertexIn::VertexIn(){
		m_bValue = false;
        m_nNumber = -1;
        m_nX = 0;
		m_nY = 0;
		m_nZ = 0;
	};

	// -----------------------------------------------------------------
	
	VertexIn::~VertexIn(){
		
	}

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
	
	void VertexIn::setXYZ(float x, float y, float z){
		m_nX = x;
		m_nY = y;
		m_nZ = z;
	}
	
	// -----------------------------------------------------------------
	
	float VertexIn::x(){
		return m_nX;
	}
	
	// -----------------------------------------------------------------
	
	float VertexIn::y(){
		return m_nY;
	}
	
	// -----------------------------------------------------------------
	
	float VertexIn::z(){
		return m_nZ;
	}
	
	// -----------------------------------------------------------------

	void VertexIn::setX(float x){
		m_nX = x;
	}
	
	// -----------------------------------------------------------------
	
	void VertexIn::setY(float y){
		m_nY = y;
	}
	
	// -----------------------------------------------------------------
	
	void VertexIn::setZ(float z){
		m_nZ = z;
	}
	
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
