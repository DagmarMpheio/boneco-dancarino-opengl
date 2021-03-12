/*especifica o sistema operativo(windows)*/
#include <windows.h>
/* Inclui os headers do OpenGL, GLU, e GLUT */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<C:\Program Files (x86)\CodeBlocks\MinGW\include\GL\glut.h>
#include <math.h>

//Protótipos
void init();//declaracao da funcao de iluminacao da cena
void display_callback(void);//declaracao da fucao para fazer o desenho
void reshape_callback(int w, int h);
void timer_callback(int);//declaracao da funcao para rotacao dos objectos com um tempo definido
void key(unsigned char key, int x, int y);//interacao


/*variaveis para a rotacao e inclinacao dos membros, corpo e olhos*/
float rot_r_braco = 0;//rotacao do braco direito
float inc_r_braco = 0;//inclinacao do braco direito
float rot_l_braco = 0;//rotacao do braco esquerdo
float inc_l_braco = 5;//inclinacao do braco esquerdo
float rot_r_pe = 0;//rotacao do pe direiro
float inc_r_pe = 10;//inclinacao do pe direito
float rot_l_pe = 0;//rotacao do braco esquerdo
float inc_l_pe = 20;//inclinacao do pe esquerdo

float rot_corpo = 0;//rotacao do corpo
float rot_olho = 0;//rotacao dos olhos

float vel = 0;//velocidade com os membros

/*stacks e slices*/
static int slicesTroco = 20;
static int stacksTronco = 20;

/*funcao principal*/
int main(){

   /* inicia o GLUT e alguns parâmetros do OpenGL */

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//estabelecer o modelo de cor a utilizar, neste caso RGB
    glutInitWindowSize(840,640);//dimensao da janela
    glutInitWindowPosition(100,100);//posicao da janela
    glutCreateWindow("Boneco com Sólidos geometricos");//titulo da janela
    init();//chamar a funcao init, trata a iluminacao da cena

    glutDisplayFunc(display_callback);//estabelece a funcao display_callback que ira mostrar todos os objectos na tela
    glutReshapeFunc(reshape_callback);//chamar a funcao que faz redemensiomento da tela
    glutTimerFunc(100, &timer_callback, 1);//funcao que executa uma accao em funcao do tempo
    glutKeyboardFunc(key);//interacao a partir do teclado
   /* função de controlo do GLUT */
   glutMainLoop();



   return 0;
}


/*funcao que a aplica a iluminacao completa da cena*/
void init(){
     //Matrizes da modelagem da luz
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {25.0};

    /* Matrizes da modelagem da luz*/
    GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    /*ilumanacao nos materiais*/
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);//comando para luz especular em frente
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);//comando para hablitar reflexos
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);//luz ambiente e difusa na parte frontal e traseira
    glEnable(GL_COLOR_MATERIAL);//habilitar a cor dos materiais

    /*aplicacao de luzes*/
    glLightfv(GL_LIGHT0, GL_POSITION, light_ambient);//luz ambiente
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);//luz difuza
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//luz especular
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);//luz direcional

    /*habilitar iluminacao na cena*/
    glEnable(GL_LIGHTING);//activar iluminacao
    glEnable(GL_LIGHT0);//activar as luzes


    glShadeModel(GL_SMOOTH);//seleciona sombreamento plano ou suave

    glEnable(GL_DEPTH_TEST);//activa ou desativa as capacidades GL do lado do servidor

    glClearColor(0.0, 0.0, 0.0, 0.0);//cor do fundo, modelo RGBA

    glColor3f(0.0, 0.0, 1.0);//cor do objecto

    return;
}
/*funcao que desenha os membros*/
void desenha_membro(float rot, int desenha_mao_pe)
{
	glColor3f(0.0f, 0.0f, 1.0f);//cor dos membros superiores

	glTranslatef(0.0f, -1.0f, 0.5f);//posicao geral dos membros

    /*obj entre matrizes*/
	glPushMatrix();
        //glRotatef(45, 0.0f, 0.0f, 1.0f);
        glTranslatef(1*sin(rot), 1*cos(rot), -0.5f);//posicao dos membros em funcao dos movimentos
        glTranslatef((0.25f * sin(vel)) - 0.1f, 0.0f, 0.0f);//comando para posicao dos membros em da
        glutSolidSphere(0.25f, 20, 20);//membros superior e inferiores(pernas e bracos)
	glPopMatrix();
        /*accoes a serem executadas se as maos se moverem*/
		if (desenha_mao_pe){
           glColor3f(0.0f, 1.0f, 0.0f);//cor dos maos e pes
           /*obj entre matrizes*/
           glPushMatrix();
               glScalef(1.0,1.0,1.1);//mudanca de escala da mãos
               glTranslatef(1.35*sin(rot), 1.35*cos(rot), -0.5f);//posicao das maos e pes, para nao desaparecem na medida que obj move-e
               glTranslatef((0.5f * sin(vel)) - 0.1f, 0.0f, 0.0f);//posicao das maos e pes, a medida que a velocidade varia
               glutSolidSphere(0.1f, 20, 20);//maos e pes
           glPopMatrix();
        }

	glTranslatef(0.0f, 1.0f, -0.5f);

	/*glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);*/
}

/*desenhar o boneco*/
void desenha_boneco(float rot)
{

	glColor3f(1.0f, 0.0f, 0.0f);//cor do tronco e da cabeca

    glRotatef(rot_olho, 0.0f, 1.0f, 0.0f);//rotacao do olho

	glRotatef(10.0f + 45*sin(rot), 0.0f, 0.0f, 1.0f);//rotacao do tronco com animacao no eixo z
	glTranslatef(0.0f ,0.75f, 0.0f);//posicao tronco
	glutSolidSphere(0.75f, slicesTroco, stacksTronco);//tronco do boneco

    /*tranformacoes na cabeca*/
	glTranslatef(0.0f, 1.0f, 0.0f);//posicao da cabeca
	glutSolidCube(0.6);//cabeca

    /*obj entre matrizes*/
	glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);//cor dos olhos
        glTranslatef(0.1f, 0.2f, 0.28f);//posicao geral dos olhos
        glutSolidSphere(0.05f, 10, 10);//olho direito
        glTranslatef(-0.1f, 0.0f, 0.0f);//posicao dos olhos
        glutSolidSphere(0.05f, 10, 10);//olho esquerdo
	glPopMatrix();

    /*Propriedades do nariz*/
	glColor3f(1.0f, 0.5f, 0.0f);//cor do nariz
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);//rotacao do nariz no eixo x
	glutSolidCone(0.08f, 0.5f, 10, 2);//nariz do boneco
}

/*funcao para mostrar os objectos com as devidas propriedades e caracteristicas*/
void display_callback(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//funcao que limpa os buffer atraves das constastes usadas
    glLoadIdentity();//funcao que faz o reset da matriz model-view

    gluLookAt (0.0f, 2.0f, 10.0f , 0.0f, 0.6f, 0.0f, 0.0f, 1.0f, 0.0f);//funcao que define uma transformacao de visualizacao(vistas)

    desenha_boneco(rot_corpo);//chamar a funcao que desenha o corpo do boneco(cabeca, tronco, olhos e nariz)
    desenha_membro(1.0f + rot_r_braco, 1);//funcao para criar o braco direito com respectiva mao
    desenha_membro(-1.0f + rot_l_braco, 1);//funcao para criar o braco esquerda com respectiva mao
    desenha_membro(2.5f + rot_r_pe, 1);//funcao para criar o perna direito com respectivo pe
    desenha_membro(-2.5f + rot_l_pe, 1);//funcao para criar o perna esquerda com respectivo pe

    glutSwapBuffers();//funcao que realiza a troca na camada em uso para janela actual
    //Promove o conteudo buffer traaseiro da camada em uso para se tornar conteudo do buffer frontal
    return;
}

/*personalizacao da rotacao dos objectos em funcao do tempo*/
void timer_callback(int n)
{
    glutPostRedisplay();//marca a janela actual como se precisa de ser exibida novamente

    /*rotacoes dos objectos em funcao do tempo*/
    inc_r_braco += 0.5;//incrementa 0.5 no angulo de inclinacao do braco direiro com o passar do tempo
    rot_r_braco = sin(inc_r_braco)/5;//a medida que incrementa o angulo de inclinacao, incrementa o angulo de rotacao do obj

    inc_l_braco += 0.5;//incrementa 0.5 no angulo de inclinacao do braco esqerdo com o passar do tempo
    rot_l_braco = sin(inc_l_braco)/5;//a medida que incrementa o angulo de inclinacao, incrementa o angulo de rotacao do obj

    inc_r_pe += 0.5;//incrementa 0.5 no angulo de inclinacao do pe direito com o passar do tempo
    rot_r_pe = sin(inc_r_pe)/5;//a medida que incrementa o angulo de inclinacao, incrementa o angulo de rotacao do obj

    inc_l_pe += 0.5;//incrementa 0.5 no angulo de inclinacao do pe esquerdo com o passar do tempo
    rot_l_pe = sin(inc_l_pe)/5;//a medida que incrementa o angulo de inclinacao, incrementa o angulo de rotacao do obj

    vel += 0.2;//a velocidade com os membros movem-se
    rot_corpo += 0.1;//incrementa 0.1 no angulo de rotacao do corpo com o passar do tempo
    rot_olho += 1; //incrementa 1 no angulo de rotacao dos olhos com o passar do tempo

	glutTimerFunc(50, timer_callback, 1);//chama a funcao do tempo a cada 50 ms
}

/*funcao de redimensionamento da janela*/
void reshape_callback(int w, int h){

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);/*especifica a transformacao afim de x e y de
    coodernadas normalizadasdo dispositivo para a coodernadas da janela*/


    glMatrixMode (GL_PROJECTION);//matriz de projecao
    glLoadIdentity();//funcao que faz o reset da matriz model-view

    gluPerspective(20, (float)w/(float)h, 1.0, 100.0);//define a matriz de projeccao prespectiva

    glMatrixMode (GL_MODELVIEW);//mattriz de visualizacao
    return;
}
/*funcao de interacao com usuario*/
void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slicesTroco++;
            stacksTronco++;
            break;

        case '-':
            if (slicesTroco>3 && stacksTronco>3)
            {
                slicesTroco--;
                stacksTronco--;
            }
            break;
    }

    glutPostRedisplay();
}


