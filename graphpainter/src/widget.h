#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>

class GLWidget : public QGLWidget
{
    Q_OBJECT

    float cameraX;
    float cameraY;
    float cameraZ;

    float alpha;
    float theta;

    float pressX;
    float pressY;

    float cameraDistance;

    QGLShaderProgram *shaderProgram;
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

	void point(int x, int y, int z, QString name);
	void line(int x1, int y1, int z1, int x2, int y2, int z2);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void initShaders();
signals:

public slots:

};

#endif // GLWIDGET_H
