#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 段ボールの数
#define NUMCARDBOARD 30
#define NUMCONVEYOR 17
#define FRUITKIND 12

void Display(void);
void Reshape(int, int);
void PutSprite(int, int, int, pngInfo *);
void Mouse(int, int, int, int); // マウス関係
void PassiveMotion(int, int);
void Motion(int, int);
void PrintText(int, int, char *);

// 自分で作成
void Timer(int);
void CardboardMove(int);
void CardboardInstallation(int);
int SetCardboard(void);
int SetConveyor(void);
int SetPic(void);
int SetGoal(void);
int MakeCardboarOorder(int *);
void ComveyorChangeState(int);
int SetMob(void);
void GoalInstallation(void);
void MakeRondomTable(void);

char map[12][21] = {
    "00000000000000000000",
    "00000000000000000000",
    "000000000k0lZR0QYa0",
    "000000000W000W0W000",
    "0000000jZR000QYUYb00",
    "000000000W000W000000",
    "00000iZTZSZTZSZZZZZZ",
    "0000000X000X00000000",
    "00000hZP000OYVYVYc00",
    "0000000X000X0X0X0000",
    "0000000g000f0e0d0000",
    "00000000000000000000"};

int TurnInitialize[12][20] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int Turn[12][20] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

// ゴールをランダムにするための表
int RondomTable[FRUITKIND];

//  PNG画像を読み込むための変数
// OSとかによる型の大きさの違いを吸収している
// インスタンス化しているのと同じ
GLuint cardboardImg[NUMCARDBOARD];
pngInfo infoCardboard[NUMCARDBOARD];
int cardboardX[NUMCARDBOARD], cardboardY[NUMCARDBOARD];
int cardboarCounter = 0;
int cardboarOorder[NUMCARDBOARD];

GLuint conveyorImg[NUMCONVEYOR];
pngInfo infoConveyor[NUMCONVEYOR];
int conveyorState = 1;

GLuint gImg[FRUITKIND];
pngInfo infoG[FRUITKIND];
int gX[NUMCARDBOARD], gY[NUMCARDBOARD];

GLuint pImg[20];
pngInfo infoP[20];

// 得点
int point = 0;
int urapoint = 0;
int score = 0;
double consecutiveSuccess, magnification; // 得点関連

// ステータス
int gameStatus = 0;
int mouseX, mouseY;
int button;

int main(int argc, char **argv)
{
    //  一般的な準備
    srandom(12345);
    glutInit(&argc, argv);
    glutInitWindowSize(640, 384);
    glutCreateWindow("Fruits Separator");
    glClearColor(0.3, 0.3, 0.3, 1.0);

    //  テクスチャのアルファチャネルを有効にする設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

 
    // // 写真をセットする
    SetConveyor();
    SetGoal();
    GoalInstallation();
    SetPic();

    // マウス関連のコールバック関数
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(PassiveMotion);
    glutMotionFunc(Motion);

    //  コールバック関数の登録
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
    glutTimerFunc(500, Timer, 0);
    glutTimerFunc(50, CardboardMove, 0);
    glutTimerFunc(50, ComveyorChangeState, 0);
    glutTimerFunc(2000, CardboardInstallation, 0);

    printf("\nI'm in main\n\n");

    //  イベントループ突入
    glutMainLoop();

    return (0);
}

///
///  タイマー割り込みで画面を描画し直す
///
void Timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(50, Timer, 0);
}

///
///  ウィンドウのサイズ変更が発生したときに座標系を再設定する関数
///
void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
}

///
/// 文字を表示する
///
void PrintText(int x, int y, char *s)
{
    glColor3ub(54, 71, 99);
    glRasterPos2i(x, y);

    for (int i = 0; i < strlen(s); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
}


///
/// 段ボールを動かす cardboardImg
///
void CardboardMove(int t)
{
    // printf("CardboardMove\n");
    int i, j;
    int cx, cy; // 段ボールがある場所
    int rx, ry; // その時の余り
    char k;

    if (gameStatus == 1)
    {

        // ベルトコンベアを動かせるかどうかの配列を初期化
        for (i = 0; i < 12; i++)
        {
            for (j = 0; j < 20; j++)
            {
                Turn[i][j] = TurnInitialize[i][j];
            }
        }

        // 段ボールを動かす
        for (i = 0; i < NUMCARDBOARD; i++)
        {
            cx = (cardboardX[i] + 16) / 32;
            cy = (cardboardY[i] + 8 + 16) / 32;
            rx = (cardboardX[i] + 16) % 32;
            ry = (cardboardY[i] + 8 + 16) % 32;

            // 段ボールがあるベルトコンベアを動かせなくする
            Turn[cy][cx] = 0;

            // 段ボールがある写真によって動かす方向を変える
            if (map[cy][cx] == (char)'O')
            {
                if (ry < 16)
                {
                    cardboardY[i]++;
                }
                else
                {
                    cardboardX[i]++;
                }
            }
            else if (map[cy][cx] == (char)'O' * -1)
            {
                cardboardY[i]++;
            }
            else if (map[cy][cx] == (char)'P')
            {
                if (ry < 16)
                {
                    cardboardY[i]++;
                }
                else
                {
                    cardboardX[i]--;
                }
            }
            else if (map[cy][cx] == (char)'P' * -1)
            {
                cardboardY[i]++;
            }
            else if (map[cy][cx] == (char)'Q')
            {
                if (ry < 16)
                {
                    cardboardX[i]++;
                }
                else
                {
                    cardboardY[i]--;
                }
            }
            else if (map[cy][cx] == (char)'Q' * -1)
            {
                cardboardY[i]--;
            }
            else if (map[cy][cx] == (char)'R')
            {
                if (ry < 16)
                {
                    cardboardX[i]--;
                }
                else
                {
                    cardboardY[i]--;
                }
            }
            else if (map[cy][cx] == (char)'R' * -1)
            {
                cardboardY[i]--;
            }
            else if (map[cy][cx] == (char)'S')
            {
                if (rx < 16)
                {
                    cardboardY[i]--;
                }
                else
                {
                    cardboardX[i]--;
                }
            }
            else if (map[cy][cx] == (char)'S' * -1)
            {
                cardboardX[i]--;
            }
            else if (map[cy][cx] == (char)'T')
            {
                if (rx < 16)
                {
                    cardboardY[i]++;
                }
                else
                {
                    cardboardX[i]--;
                }
            }
            else if (map[cy][cx] == (char)'T' * -1)
            {
                cardboardX[i]--;
            }
            else if (map[cy][cx] == (char)'U')
            {
                if (rx < 16)
                {
                    cardboardX[i]++;
                }
                else
                {
                    cardboardY[i]--;
                }
            }
            else if (map[cy][cx] == (char)'U' * -1)
            {
                cardboardX[i]++;
            }

            else if (map[cy][cx] == (char)'V')
            {
                if (rx < 16)
                {
                    cardboardX[i]++;
                }
                else
                {
                    cardboardY[i]++;
                }
            }
            else if (map[cy][cx] == (char)'V' * -1)
            {
                cardboardX[i]++;
            }
            else if (map[cy][cx] == (char)'W')
            {
                cardboardY[i]--;
            }
            else if (map[cy][cx] == (char)'X')
            {
                cardboardY[i]++;
            }
            else if (map[cy][cx] == (char)'Y')
            {
                cardboardX[i]++;
            }
            else if (map[cy][cx] == (char)'Z')
            {
                cardboardX[i]--;
            }
            else
            {
                for (k = 0; k < FRUITKIND; k++)
                {
                    // 車についた場合
                    if (map[cy][cx] == (char)'a' + k)
                    {
                        if (map[cy][cx] == (char)cardboarOorder[i] + (char)'a')
                        {
                            point++;
                            consecutiveSuccess++;
                            printf("得点 = %d\n", point);
                        }
                        else
                        {
                            consecutiveSuccess = 0;
                            urapoint++;
                        }
                        magnification += 1 + (consecutiveSuccess / 10);
                        cardboardY[i] = -100;
                        cardboardX[i] = -100;

                        if ((point + urapoint) == NUMCARDBOARD) // NUMCARDBOARD
                        {
                            score = (double)point * magnification;
                            gameStatus = 2;
                        }
                    }
                }
            }
        }
    }

    glutTimerFunc(40, CardboardMove, 0);
}

///
///  段ボールを配置する
///
void CardboardInstallation(int t)
{
    if (gameStatus == 1)
    {
        // 段ボールが出てくる場所を設定
        if (cardboarCounter < NUMCARDBOARD)
        {
            cardboardX[cardboarCounter] = 640 - 17;
            cardboardY[cardboarCounter] = 32 * 6 - 8;
        }
        cardboarCounter++;
    }

    glutTimerFunc(4000, CardboardInstallation, 0);
}

///
/// 車を配置する
///
void GoalInstallation(void)
{
    int i, j;
    char k;
    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 20; j++)
        {
            for (k = 0; k < FRUITKIND; k++)
            {
                if (map[i][j] == (char)'a' + k)
                {
                    gX[(int)k] = j * 32;
                    gY[(int)k] = i * 32;
                }
            }
        }
    }
}

///
/// コンベアーを動かす cardboardImg
///
void ComveyorChangeState(int t)
{
    conveyorState = 1 - conveyorState;
    glutTimerFunc(250, ComveyorChangeState, 0);
}

///
///  ウィンドウの表示内容を更新する
///
void Display(void)
{
    int x, y; //  PNG画像を置く座標
    int i, j;
    char s[20];
    char t[20];
    char u[20];

    glutReshapeWindow(640, 384);
    glClear(GL_COLOR_BUFFER_BIT);
    PutSprite(pImg[0], 0, 0, &infoP[0]);

    if (gameStatus == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        PutSprite(pImg[1], 0, 0, &infoP[1]);
        PutSprite(pImg[2], 256, 275, &infoP[2]);
        if (mouseX > 256 && mouseY > 275 && mouseX < 256 + 128 && mouseY < 275 + 64)
        {
            if (button == 1)
            {
                PutSprite(pImg[4], 256, 275, &infoP[4]);
            }
            else if (button == 2)
            {
                PutSprite(pImg[2], 256, 275, &infoP[2]);
            }
            else
            {
                PutSprite(pImg[3], 256, 275, &infoP[3]);
            }
        }
    }
    else if (gameStatus == 1)
    {
        PrintText(25, 70, "CORRECT         of");
        sprintf(s, "%2d", point);
        sprintf(t, "%2d", urapoint + point);
        sprintf(u, "%2d", (int)consecutiveSuccess);
        PrintText(165, 70, s);
        PrintText(225, 70, t);
        PrintText(25, 120, "COMBO");
        PrintText(165, 120, u);

        for (i = 0; i < 12; i++)
        {
            for (j = 0; j < 20; j++)
            {
                x = j * 32;
                y = i * 32;
                if (map[i][j] == (char)'O')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[11], x, y, &infoConveyor[11]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[12], x, y, &infoConveyor[12]);
                    }
                }
                else if (map[i][j] == (char)'O' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[2], x, y, &infoConveyor[2]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[1], x, y, &infoConveyor[1]);
                    }
                }
                else if (map[i][j] == (char)'P')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[13], x, y, &infoConveyor[13]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[14], x, y, &infoConveyor[14]);
                    }
                }
                else if (map[i][j] == (char)'P' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[2], x, y, &infoConveyor[2]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[1], x, y, &infoConveyor[1]);
                    }
                }
                else if (map[i][j] == (char)'Q')
                {
                    if (conveyorState)
                    {

                        PutSprite(conveyorImg[9], x, y, &infoConveyor[9]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[10], x, y, &infoConveyor[10]);
                    }
                }
                else if (map[i][j] == (char)'Q' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[3], x, y, &infoConveyor[3]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[4], x, y, &infoConveyor[4]);
                    }
                }
                else if (map[i][j] == (char)'R')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[15], x, y, &infoConveyor[15]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[16], x, y, &infoConveyor[16]);
                    }
                }
                else if (map[i][j] == (char)'R' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[3], x, y, &infoConveyor[3]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[4], x, y, &infoConveyor[4]);
                    }
                }
                else if (map[i][j] == (char)'S')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[11], x, y, &infoConveyor[11]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[12], x, y, &infoConveyor[12]);
                    }
                }
                else if (map[i][j] == (char)'S' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[7], x, y, &infoConveyor[7]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[5], x, y, &infoConveyor[5]);
                    }
                }
                else if (map[i][j] == (char)'T')
                {
                    if (conveyorState)
                    {

                        PutSprite(conveyorImg[9], x, y, &infoConveyor[9]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[10], x, y, &infoConveyor[10]);
                    }
                }
                else if (map[i][j] == (char)'T' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[7], x, y, &infoConveyor[7]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[5], x, y, &infoConveyor[5]);
                    }
                }
                else if (map[i][j] == (char)'U')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[13], x, y, &infoConveyor[13]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[14], x, y, &infoConveyor[14]);
                    }
                }
                else if (map[i][j] == (char)'U' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[8], x, y, &infoConveyor[8]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[6], x, y, &infoConveyor[6]);
                    }
                }
                else if (map[i][j] == (char)'V')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[15], x, y, &infoConveyor[15]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[16], x, y, &infoConveyor[16]);
                    }
                }
                else if (map[i][j] == (char)'V' * -1)
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[8], x, y, &infoConveyor[8]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[6], x, y, &infoConveyor[6]);
                    }
                }
                else if (map[i][j] == (char)'W')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[3], x, y, &infoConveyor[3]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[4], x, y, &infoConveyor[4]);
                    }
                }
                else if (map[i][j] == (char)'X')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[2], x, y, &infoConveyor[2]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[1], x, y, &infoConveyor[1]);
                    }
                }
                else if (map[i][j] == (char)'Y')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[8], x, y, &infoConveyor[8]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[6], x, y, &infoConveyor[6]);
                    }
                }
                else if (map[i][j] == (char)'Z')
                {
                    if (conveyorState)
                    {
                        PutSprite(conveyorImg[7], x, y, &infoConveyor[7]);
                    }
                    else
                    {
                        PutSprite(conveyorImg[5], x, y, &infoConveyor[5]);
                    }
                }
            }
        }

        for (i = 0; i < NUMCARDBOARD; i++)
        {
            PutSprite(cardboardImg[i], cardboardX[i], cardboardY[i], &infoCardboard[i]);
        }

        for (i = 0; i < FRUITKIND; i++)
        {
            PutSprite(gImg[i], gX[i], gY[i], &infoG[i]);
        }
    }
    else if (gameStatus == 2)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        PutSprite(pImg[0], 0, 0, &infoP[0]);
        PrintText(230, 150, "Score");
        sprintf(s, "%5d", score);
        PrintText(345, 150, s);
        PutSprite(pImg[5], 192, 230, &infoP[5]);
        if (mouseX > 192 && mouseY > 230 && mouseX < 192 + 256 && mouseY < 230 + 64)
        {

            if (button == 1)
            {
                PutSprite(pImg[7], 192, 230, &infoP[7]);
            }
            else if (button == 2)
            {
                PutSprite(pImg[5], 192, 230, &infoP[5]);
            }
            else
            {
                PutSprite(pImg[6], 192, 230, &infoP[6]);
            }
        }
    }

    glFlush();
}


///
///  num番のPNG画像を座標(x,y)に表示する
///　作ってもらったからこれを使いまわせばよい！！！
void PutSprite(int num, int x, int y, pngInfo *info)
{
    int w, h; //  テクスチャの幅と高さ

    w = info->Width; //  テクスチャの幅と高さを取得する
    h = info->Height;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, num);
    glColor4ub(255, 255, 255, 255); // ここの透明度で炎の透け感を演出できる

    glBegin(GL_QUADS); //  幅w, 高さhの四角形

    glTexCoord2i(0, 0); // co ordinate なんか一回正方形にされてしまうので引き延ばす必要がある　ゴムの写真のイメージ
    glVertex2i(x, y);   // ゴムの写真のイメージ

    glTexCoord2i(0, 1);
    glVertex2i(x, y + h);

    glTexCoord2i(1, 1);
    glVertex2i(x + w, y + h);

    glTexCoord2i(1, 0);
    glVertex2i(x + w, y);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

///
/// 段ボールオブジェクトを作成
///
int SetCardboard(void)
{
    char fname[20];

    MakeCardboarOorder(cardboarOorder);
    for (int i = 0; i < NUMCARDBOARD; i++)
    {
        // 果物の順番をランダムにする　（多分）
        sprintf(fname, "cardboard%02d.png", RondomTable[cardboarOorder[i]]);
        cardboardImg[i] = pngBind(fname, PNG_NOMIPMAP, PNG_ALPHA,
                                  &infoCardboard[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
        // 段ボールが出てくる場所を設定 (最初は隠しておく)
        cardboardX[i] = -100;
        cardboardY[i] = -100;
    }

    printf("box{");
    for (int i = 0; i < NUMCARDBOARD; i++)
    {
        printf("%d ", cardboarOorder[i]);
    }
    printf("}\n");

    return (0);
}


///
/// コンベアーの写真をセット
///
int SetConveyor(void)
{
    char fname[20];

    for (int i = 0; i < NUMCONVEYOR; i++)
    {
        sprintf(fname, "conveyor%02d.png", i);
        conveyorImg[i] = pngBind(fname, PNG_NOMIPMAP, PNG_ALPHA,
                                 &infoConveyor[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }

    return (0);
}

///
/// 車の写真をセット
///
int SetGoal(void)
{
    char fname[20];

    for (int i = 0; i < FRUITKIND; i++)
    {
        sprintf(fname, "g%02d.png", RondomTable[i]);
        gImg[i] = pngBind(fname, PNG_NOMIPMAP, PNG_ALPHA,
                          &infoG[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }

    return (0);
}

///
/// その他の写真をセット
///
int SetPic(void)
{
    // pImg
    // infoP
    char fname[20];

    for (int i = 0; i < 8; i++)
    {
        sprintf(fname, "p%02d.png", i);
        pImg[i] = pngBind(fname, PNG_NOMIPMAP, PNG_ALPHA,
                          &infoP[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }

    return (0);
}

///
/// ゴールをランダムにする
///
void MakeRondomTable(void)
{
    int i, j;
    int tmp, r;

    srand((unsigned int)time(NULL));

    for (i = 0; i < FRUITKIND; i++)
    {
        RondomTable[i] = i;
    }

    // 適当に10回入れ替えを行う
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < FRUITKIND; j++)
        {
            r = rand() % FRUITKIND;
            tmp = RondomTable[j];
            RondomTable[j] = RondomTable[r];
            RondomTable[r] = tmp;
        }
    }
    printf("Rancomtable = {");
    for (i = 0; i < FRUITKIND; i++)
    {
        printf("%d ", RondomTable[i]);
    }
    printf("}\n");
}

///
/// ランダムに段ボールの順番を決める
///
int MakeCardboarOorder(int *cardboarOorder)
{
    int i, j, r, tmp;
    // 現在時刻の情報で初期化
    srand((unsigned int)time(NULL));

    // 前半部分は0～9まで2個ずつ入れる
    for (i = 0; i < FRUITKIND; i++)
    {
        *(cardboarOorder + i) = i;
        *(cardboarOorder + i + FRUITKIND) = i;
    }

    // 後半部分0からkindまでの乱数を発生
    for (i = FRUITKIND * 2; i < NUMCARDBOARD; i++)
    {
        *(cardboarOorder + i) = rand() % FRUITKIND;
    }

    // 適当に10回入れ替えを行う
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < NUMCARDBOARD; j++)
        {
            r = rand() % NUMCARDBOARD;
            tmp = *(cardboarOorder + j);
            *(cardboarOorder + j) = *(cardboarOorder + r);
            *(cardboarOorder + r) = tmp;
        }
    }
    return (0);
}

///
/// マウスがクリックされたとき
///
void Mouse(int b, int s, int x, int y)
{
    mouseX = x;
    mouseY = y;
    button = 0;
    if (gameStatus == 0)
    {
        if (mouseX > 256 && mouseY > 275 && mouseX < 256 + 128 && mouseY < 275 + 64)
        {
            if (b == GLUT_LEFT_BUTTON)
            {
                button = 1;
                if (s == GLUT_UP)
                {
                    // 初期化の設定
                    button = 2;
                    point = 0;
                    urapoint = 0;
                    score = 0;
                    consecutiveSuccess = 0;
                    magnification = 1;
                    cardboarCounter = 0;
                    MakeRondomTable();
                    SetCardboard(); 
                    SetGoal();
                    GoalInstallation();
                    gameStatus = 1;
                }
            }
        }
    }
    else if (gameStatus == 1)
    {
        if (b == GLUT_LEFT_BUTTON)
        {
            if (s == GLUT_UP)
            {
                // printf("左ボタンアップ");
                if (Turn[y / 32][x / 32] == 1)
                {
                    map[y / 32][x / 32] *= -1;
                }
            }
        }
    }
    else if (gameStatus == 2)
    {
        button = 0;
        if (mouseX > 192 && mouseY > 230 && mouseX < 192 + 256 && mouseY < 230 + 64)
        {
            if (b == GLUT_LEFT_BUTTON)
            {
                button = 1;
                if (s == GLUT_UP)
                {
                    button = 2;
                    gameStatus = 0;
                }
            }
        }
    }
}

///
/// マウスが移動したとき
///
void PassiveMotion(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

///
/// ボタンを押しながらマウスが移動したとき
///
void Motion(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

