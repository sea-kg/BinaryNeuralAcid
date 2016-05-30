#include "vertex.h"
#include "../operations/and.h"
#include "../operations/or.h"
#include "../operations/xor.h"

namespace reversehash {
	Vertex::Vertex(){
		m_pIn1 = NULL;
		m_pIn2 = NULL;
		m_pOperation = NULL;
	};
	
	// -----------------------------------------------------------------
	
	bool Vertex::out(){
		bool bIn1 = m_pIn1 != NULL ? m_pIn1->out() : false;
		bool bIn2 = m_pIn2 != NULL ? m_pIn2->out() : false;
		return m_pOperation != NULL ? m_pOperation->calc(bIn1, bIn2) : false;
	};
	
	// -----------------------------------------------------------------
	
	QString Vertex::type(){
		return QString("Vertex");
	};
	
	// -----------------------------------------------------------------
	
	QString Vertex::name(){
		return m_sName;
	};
	
	// -----------------------------------------------------------------
	
	void Vertex::setName(QString sName){
		m_sName = sName;
	};
	
	// -----------------------------------------------------------------
	
	void Vertex::setOperation(QString sOper){
		if(sOper == "|"){
			m_pOperation = new OperationOr();
		}else if(sOper == "^"){
			m_pOperation = new OperationXor();
		}else if(sOper == "&"){
			m_pOperation = new OperationAnd();
		}else{
			m_pOperation = NULL;
		}
	};
	
	// -----------------------------------------------------------------
	
	QString Vertex::operation(){
		return m_pOperation != NULL ? m_pOperation->type() : "0";
	};
	
	// -----------------------------------------------------------------
	
	void Vertex::setIn1(IVertexOut *pIn){
		m_pIn1 = pIn;
	};
	
	// -----------------------------------------------------------------
	
	void Vertex::setIn2(IVertexOut *pIn){
		m_pIn2 = pIn;
	};
	
	// -----------------------------------------------------------------
	
	IVertexOut *Vertex::in1(){
		return m_pIn1;
	};
	
	// -----------------------------------------------------------------
	
	IVertexOut *Vertex::in2(){
		return m_pIn2;
	};
	
	// -----------------------------------------------------------------
}
