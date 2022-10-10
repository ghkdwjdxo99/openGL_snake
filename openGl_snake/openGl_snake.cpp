#include <gl/glut.h>					
#include <gl/gl.h>						
#include <gl/glu.h>

#include <stdio.h>

#define B_SIZE_ROW      20
#define B_SIZE_COL      20

#define BLOCK_SIZE      0.1

#define H_ROW           B_SIZE_ROW / 2 -1
#define H_COL           B_SIZE_COL / 2 -1
#define T_ROW           B_SIZE_ROW / 2 -1
#define T_COL           B_SIZE_COL / 2 -2

#define WALL            1
#define APPLE           2
#define SNAKE           3

#define __TEST__

typedef struct snake {
    struct snake* nextNode;
    int r;
    int c;
}snake;



char board[B_SIZE_ROW][B_SIZE_COL] = { 0, };    // 게임이 진행되는 보드 판

snake* sHead = (snake*)malloc(sizeof(snake));   // snake head (시작) 노드 생성
snake* sTail = (snake*)malloc(sizeof(snake));   // snake tail (끝) 노드 생성

void MyReshape(int NewWidth, int NewHeight) {
    glViewport(0, 0, NewWidth, NewHeight);	// Ori

    GLfloat WidthFactor = (GLfloat)NewWidth / (GLfloat)300;
    GLfloat HeightFactor = (GLfloat)NewHeight / (GLfloat)300;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor,
        -1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0); // Ori
}

void GameInit() {   // 게임 시작 전 배열 초기화
    // board 배열에 벽 입력
    for (int i = 0; i < 20; i++) {
        board[0][i] = WALL;
        board[19][i] = WALL;
        board[i][0] = WALL;
        board[i][19] = WALL;
    }
    /*
    // board 배열 내부 확인
    for (int r = 0; r < 20; r++) {
        for (int c = 0; c < 20; c++) {
            printf("%d ", board[r][c]);
        }
        printf("\n");
    }
    */
}
void snakeInit() {  // snake 초기화
    sHead->nextNode = sTail;
    sHead->r = H_ROW;
    sHead->c = H_COL;
    sTail->nextNode = NULL;
    sTail->r = T_ROW;
    sTail->c = T_COL;

    // board 배열에 snake 위치 입력
    board[sHead->r][sHead->c] = SNAKE;
    board[sTail->r][sTail->c] = SNAKE;
}
void DrawWall() {  // 외벽 출력 함수
    GLfloat TopX = -1;
    GLfloat TopY = 1;
    GLfloat BottomX = TopX + BLOCK_SIZE;
    GLfloat BottomY = TopY - BLOCK_SIZE;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    
    // 화면에 외벽 출력
    for (int r = 0; r < 20; r++) {
        for (int c = 0; c < 20; c++) {
            if (board[r][c] == WALL) {
                glBegin(GL_POLYGON);
                glVertex3f(TopX, TopY, 0.0);
                glVertex3f(TopX, BottomY, 0.0);
                glVertex3f(BottomX, BottomY, 0.0);
                glVertex3f(BottomX, TopY, 0.0);
                glEnd();
            }
            TopX += BLOCK_SIZE;
            BottomX += BLOCK_SIZE;
        }
        TopX = -1;
        BottomX = TopX + BLOCK_SIZE;
        TopY -= BLOCK_SIZE;
        BottomY -= BLOCK_SIZE;
    }
}
void DrawApple() {
    // 사과 그리기 함수
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); // 색상 변경
    DrawWall();

    glutReshapeFunc(MyReshape);	// 윈도우 사이즈 변경 시 호출 (그림 위치, 크기 동일하게 해줌)
    glutSwapBuffers( ); // glFlush()와 동일하지만 화면 전환 부드럽게 해줌 // glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); 사용해야함
}
// 윈도우 사이즈 바뀌어도 그림 위치, 크기 동일하게 함


// 아이들 함수
void MyIdle() {
    
    glutPostRedisplay(); // Display 함수 재실행
}
// 타이머 함수
void MyTimer(int Value) {
    
    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);
}
// 마우스 클릭 함수
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        // 마우스 클릭 시 board 배열에 사과 위치 넣기
        int rIdx = Y % 20;
        int cIdx = X % 20;
        board[rIdx][cIdx] = APPLE;
        glutPostRedisplay();    // 현재 윈도우를 다시 그릴것을 요청
    }
}
// 키보드 함수
void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
    
    glutPostRedisplay();
}

void ExitFunc(void) {
    free(sHead);
    free(sTail);
}

#ifdef __TEST__
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGB);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example Drawing");

    glClearColor(0.0, 0.0, 0.0, 0.0); // 색 기본값 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // 좌 우 하 상 전 후

    GameInit();
    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouseClick); // 마우스 클릭 콜백 함수					

    glutMainLoop();
    atexit(ExitFunc);
    return 0;
}
#endif

#ifndef __TEST__
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGB);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example Drawing");

    glClearColor(0.0, 0.0, 0.0, 0.0); // 색 기본값 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // 좌 우 하 상 전 후



    glutDisplayFunc(MyDisplay);

    glutReshapeFunc(MyReshape);	// 윈도우 사이즈 변경 시 호출 (그림 위치, 크기 동일하게 해줌)

    // glutIdleFunc(MyIdle); // 아이들 콜백 함수

    glutTimerFunc(40, MyTimer, 3); // 타이버 콜백 함수 (몇 밀리초마다 불리는지, 부를 함수, 함수 번호)

    glutMouseFunc(MyMouseClick); // 마우스 클릭 콜백 함수					

    glutKeyboardFunc(MyKeyboard); // 키보드 콜백 함수

    glutMainLoop();

    atexit(ExitFunc);   // 프로그램 종료 전 이 함수 실행 (동적 메모리 할당)
    return 0;
}
#endif