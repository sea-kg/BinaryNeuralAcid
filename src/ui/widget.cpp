#include "widget.h"
#include <GL/glu.h>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFileInfo>
#include <qmath.h>
#include <iostream>
#include <QApplication>
#include "../vertex_graph.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    shaderProgram = new QGLShaderProgram( this );

    cameraDistance = 15;
    cameraX = 0;
    cameraY = 0;
    cameraZ = cameraDistance;

    alpha = M_PI / 6;
    theta = M_PI / 6;
}

void GLWidget::loadGraph(QString filename){
	// TODO
}

void GLWidget::initializeGL() {
    glClearColor(0, 0, 0, 1);
    initShaders();
	qDebug("version: %s\n", glGetString(GL_VERSION));
}

void GLWidget::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2, w/2, -h/2, h/2, -w, w);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::point(int x, int y, int z, QString name) {
	// glTranslatef(0, 0, 0);

	float d = 0.2;
	glColor4ub (176, 50, 153, 115);
	glBegin(GL_LINES);
		glColor4ub (176, 50, 153, 115);
		
		glVertex3f(x + d, y, z);
		glVertex3f(x, y - d, z);

		glVertex3f(x, y - d, z);
		glVertex3f(x - d, y, z);
		
		glVertex3f(x - d, y, z);
		glVertex3f(x, y + d, z);
		
		glVertex3f(x, y + d, z);
		glVertex3f(x + d, y, z);

		glVertex3f(x, y + d, z);
		glVertex3f(x, y, z + d);
		
		glVertex3f(x, y, z + d);
		glVertex3f(x, y - d, z);
		
		glVertex3f(x, y - d, z);
		glVertex3f(x, y, z - d);
		
		glVertex3f(x, y, z - d);
		glVertex3f(x, y + d, z);
		
		glVertex3f(x + d, y, z);
		glVertex3f(x, y, z + d);
		
		glVertex3f(x, y, z + d);
		glVertex3f(x - d, y, z);
		
		glVertex3f(x - d, y, z);
		glVertex3f(x, y, z - d);
		
		glVertex3f(x, y, z - d);
		glVertex3f(x + d, y, z);
	glEnd();

	/*GLUquadricObj *quadric = gluNewQuadric();
    glColor4ub (176, 50, 153, 115);
    // gluQuadricDrawStyle(quadric, (GLenum)GLU_SMOOTH);
    // gluQuadricDrawStyle(quadric, (GLenum)GLU_FILL);
    // gluQuadricDrawStyle(quadric, (GLenum)GLU_POINT);
    gluQuadricDrawStyle(quadric, (GLenum)GLU_LINE);
    gluSphere(quadric, 0.2, 10, 10);
    // glTranslatef(-1*x, -1*y, -1*z);
	gluDeleteQuadric(quadric);
	glColor4ub (255, 255, 255, 255);
	renderText(x + 0.5, y, z, name);*/
}

void GLWidget::line(int x1, int y1, int z1, int x2, int y2, int z2) {
	glTranslatef(0, 0, 0);
	glBegin(GL_LINES);
		glColor4ub (176, 50, 153, 115);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.01, height());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glEnable(GL_CULL_FACE);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    float x = cameraDistance * cos(alpha) * sin(theta);
    float y = cameraDistance * sin(alpha);
    float z = cameraDistance * cos(alpha) * cos(theta);
    gluLookAt(x, y, z,
              0, 0, 0,
              0, 1, 0);

	glTranslatef(0, 0, 0);
	
	QStringList args = QCoreApplication::arguments();
	
	/*graph::Graph *pGraph = new graph::Graph();
	
	if (args.size() == 2) {
		// todo load
		std::cout << args.at(1).toStdString();
	} else {
		pGraph->generateRandomGraph();
	}

	int nSide = 10;
	
	for (int i = 0; i < pGraph->points().size(); i++) {
		QString name = pGraph->points()[i];

		int z = i / (nSide * nSide); // 100 points in level
		int x = (i % (nSide * nSide)) / nSide;
		int y = (i % (nSide * nSide)) % nSide;
		point(x, y, z, name);
	}*/
	
	/*
	
	for (int i = 0; i < pGraph->lines().size(); i++) {
		QString name = pGraph->lines()[i];

		int z = i / (nSide * nSide); // 100 points in level
		int x = (i % (nSide * nSide)) / nSide;
		int y = (i % (nSide * nSide)) % nSide;
		point(x, y, z, name);
	}*/
	
	

	// point(0, 2, 1, "in02");

	line(
			0, 1, 0,
			0, 2, 1);

	
	glColor4ub (255, 255, 255, 255);

    //
    glBegin(GL_LINES);
      //glColor3ub(255, 255, 255);
      glVertex3f(-300, 0, 0);
      glVertex3f(300, 0, 0);
      glVertex3f(0, -300, 0);
      glVertex3f(0, 300, 0);
      glVertex3f(0, 0, -300);
      glVertex3f(0, 0, 300);
    glEnd();
    //
    renderText(270, 0, 0, "X");
    renderText(0, 270, 0, "Y");
    renderText(0, 0, 270, "Z");
    //
    /*glBegin(GL_TRIANGLE_STRIP);
        //glColor3ub(167, 58, 13);
        glVertex3f(10, 0, 10);
        glVertex3f(10, 0, -10);
        glVertex3f(-10, 0, 10);
        glVertex3f(-10, 0, -10);
    glEnd();*/
    glFlush();
}



void GLWidget::wheelEvent(QWheelEvent *event) {
    int d = cameraDistance - (event->delta()) / 120;
    if (d > 1) {
        cameraDistance = d;
        updateGL();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    pressX = event->x();
    pressY = event->y();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (pressY - event->y() > 0) {if (alpha + 0.01 < M_PI / 2) { alpha += 0.01; } }
    else if (pressY - event->y() < 0) { if ( -M_PI / 2 < alpha - 0.01) { alpha -= 0.01; }  }
    if (pressX - event->x() > 0) { theta += 0.01; }
    else if (pressX - event->x() < 0) { theta -= 0.01; }

    int x = cameraDistance * cos(alpha) * sin(theta);
    int y = cameraDistance * sin(alpha);
    int z = cameraDistance * cos(alpha) * cos(theta);

    pressX = event->x();
    pressY = event->y();
    updateGL();
}

void GLWidget::initShaders() {
/*
    // Compile vertex shader
    shaderProgram->addShaderFromSourceFile( QGLShader::Vertex, ":/vShader.vsh" );

    // Compile fragment shader
    shaderProgram->addShaderFromSourceFile( QGLShader::Fragment, ":/fShader.fsh" );



    // Compile vertex shader
    shaderProgram->addShaderFromSourceFile( QGLShader::Vertex, ":/vShader.glsl" );

    // Compile fragment shader
    shaderProgram->addShaderFromSourceFile( QGLShader::Fragment, ":/fShader.glsl" );

    // Bind vertex array object
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject( this );
    vao->bind();

    // Bind shader pipeline for use
    shaderProgram->link();
    shaderProgram->bind();
    */

    /*// Compile vertex shader
    if (!shaderProgram->addShaderFromSourceFile(QGLShader::Vertex, ":/vShader.glsl"))
        close();

    // Compile fragment shader
    if (!shaderProgram->addShaderFromSourceFile(QGLShader::Fragment, ":/fShader.glsl"))
        close();

        */

    if (!shaderProgram->addShaderFromSourceCode(QGLShader::Vertex,
                                                "void main()\n{gl_Position = ftransform();}"))
        close();

    // Compile fragment shader
    if (!shaderProgram->addShaderFromSourceCode(QGLShader::Fragment,
                                                "void main()\n{gl_FragColor = vec4(0.4, 0.5, 0.0, 1.0);}"))
        close();

    qDebug() << shaderProgram->log();

    // Link shader pipeline
    if (!shaderProgram->link())
        close();

    // Bind shader pipeline for use
    if (!shaderProgram->bind())
        close();
}
