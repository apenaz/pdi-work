#include <GL/glut.h> 

// Função call back chamada para fazer o desenho
void DesenhaNaTela(void)
{
    glClear(GL_COLOR_BUFFER_BIT); 

    glutSwapBuffers();
} 

// Inicializa parâmetros de rendering
void Inicializa (void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
} 

// Programa principal 

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(320,240);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Primeiro Programa em OpenGL");
    glutDisplayFunc(DesenhaNaTela);
    Inicializa();
    glutMainLoop();
}