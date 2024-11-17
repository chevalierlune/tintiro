#include "glut.h"
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS
#define PI 3.14159

//Diceの構造体
typedef struct 
{
    double RotAngleX;
    double RotAngleY;
    double RotAngleZ;
    double Speed; // 回転速度
    int Animation; // 0:オフ、1:オン
    int targetFace; // 目標の面（1～6）
    int directionX; // X軸回転方向: 1 or -1
    int directionY; // Y軸回転方向: 1 or -1
    int directionZ; // Z軸回転方向: 1 or -1
} Dice;

Dice dice[3]; // 3つのサイコロ

// 視点の座標をグローバル変数として宣言
double eyeX = 0.0, eyeY = 0.0, eyeZ = 12.0;
double centerX = 0.0, centerY = 0.0, centerZ = 0.0;
double P = 0.05;

// スタートボタンとベットボタンの位置とサイズ
float buttonX = -0.2, buttonY = -0.7, buttonWidth = 0.4, buttonHeight = 0.2;
float betButtonX = -0.2, betButtonY = -0.4, betButtonWidth = 0.4, betButtonHeight = 0.2;

// サイコロの停止面を保存する変数
char diceStatus[3][50];

// 所持金とベット額の初期値
int money = 100;
int bet = 0;

// 各面の目標角度を設定する関数
void setTargetAngles(int face, double* angleX, double* angleY, double* angleZ) 
{
    switch (face) 
    {
    case 1: // 1の面が上に来る
        *angleX = 0.0;
        *angleY = 90.0;
        *angleZ = 0.0;
        break;
    case 2: // 2の面が上に来る
        *angleX = 180.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 3: // 3の面が上に来る
        *angleX = 90.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 4: // 4の面が上に来る
        *angleX = -90.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 5: // 5の面が上に来る
        *angleX = 0.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 6: // 6の面が上に来る
        *angleX = 0.0;
        *angleY = -90.0;
        *angleZ = 0.0;
        break;
    }
}

// 初期化関数
void init(void)
{
    srand((unsigned int)time(NULL));  // ランダムシードを初期化
    for (int i = 0; i < 3; i++) 
    {
        dice[i].RotAngleX = 0.0;
        dice[i].RotAngleY = 0.0;
        dice[i].RotAngleZ = 0.0;
        dice[i].Speed = (rand() % 10 + 5) * 2; // ランダムな速度設定（10.0 - 30.0）
        dice[i].Animation = 0; // アニメーションオフ
        dice[i].targetFace = 1; // 初期目標面
        dice[i].directionX = (rand() % 2) * 2 - 1; // -1 または 1
        dice[i].directionY = (rand() % 2) * 2 - 1; // -1 または 1
        dice[i].directionZ = (rand() % 2) * 2 - 1; // -1 または 1
        snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Face ?", i + 1);
    }
}

// 目を描画するための関数
void drawDot(float x, float y, float z)
{
    glPushMatrix();
    glTranslated(x, y, z);
    glutSolidSphere(0.1, 20, 20); // 小さな球体を描画
    glPopMatrix();
}

// 赤点を描画するための関数
void drawDotColor(float x, float y, float z, int r, int g, int b)
{
    glPushMatrix();
    glColor3ub(r, g, b); // RGB値を使って色を設定
    glTranslated(x, y, z);
    glutSolidSphere(0.1, 20, 20); // 小さな球体を描画
    glPopMatrix();
}

// サイコロの各面を描画するための関数
void drawDiceFaces()
{
    // 面1 (左側面)
    glPushMatrix();
    glTranslated(-0.5, 0.0, 0.0);
    glRotated(90, 0, 1, 0);
    glColor3f(0.0, 0.0, 0.0);
    drawDotColor(0.0, 0.0, 0.0, 255, 0, 0);
    glPopMatrix();

    // 面2 (背面)
    glPushMatrix();
    glTranslated(0.0, 0.0, -0.5);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // 面3 (上面)
    glPushMatrix();
    glTranslated(0.0, 0.5, 0.0);
    glRotated(90, 1, 0, 0);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.0, 0.0, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // 面4 (下面)
    glPushMatrix();
    glTranslated(0.0, -0.5, 0.0);
    glRotated(90, 1, 0, 0);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.2, -0.2, 0.0);
    drawDot(-0.2, 0.2, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // 面5 (前面)
    glPushMatrix();
    glTranslated(0.0, 0.0, 0.5);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.2, -0.2, 0.0);
    drawDot(0.0, 0.0, 0.0);
    drawDot(-0.2, 0.2, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // 面6 (右側面)
    glPushMatrix();
    glTranslated(0.5, 0.0, 0.0);
    glRotated(90, 0, 1, 0);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.2, -0.2, 0.0);
    drawDot(-0.2, 0.0, 0.0);
    drawDot(0.2, 0.0, 0.0);
    drawDot(-0.2, 0.2, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();
}

// サイコロの描画
void drawDice(int index)
{
    glPushMatrix();
    glTranslated(0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidCube(1.0); // サイコロの本体を描画
    drawDiceFaces(); // サイコロの各面を描画
    glPopMatrix();
}

// ２次元文字を描画する関数
void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    while (*text) 
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// 所持金を描画する関数
void drawMoney(float x, float y, int amount)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Money: $%d", amount);
    drawText(x, y, buffer);
}

// ベット額を描画する関数
void drawBet(float x, float y, int amount)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Bet : $%d", amount);
    drawText(x, y, buffer);
}

// ボタンを描画する関数
void drawButton(float x, float y, float width, float height, const char* label)
{
    glColor3f(0, 255, 255);// ボタンの色を水色に設定
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    // ボタンのラベルを描画
    glColor3f(255, 0, 0); // テキストの色を赤に設定
    drawText(x + width / 2 - 0.05 * strlen(label) / 3, y + height / 2 - 0.05 / 2, label);//文字の場所
}

//チンチロの得点配分
int tintiro(int x, int y, int z, int bet) 
{
    int re;//返し値
    if (x == y && x == z && y == z) // すべての目が同じ場合
    { 
        if (x == 1) 
        {
            re = 5 * bet; // 1,1,1は5倍
        }
        else 
        {
            re = 3 * bet; // その他のゾロ目は3倍
        }
    }
    else if ((x == 4 && y == 5 && z == 6) || (x == 4 && y == 6 && z == 5) || (x == 5 && y == 4 && z == 6) || (x == 5 && y == 6 && z == 4) || (x == 6 && y == 4 && z == 5) || (x == 6 && y == 5 && z == 4)) 
    {
        re = 2 * bet;// 4, 5, 6 の場合は2倍
    }
    else if ((x == 1 && y == 2 && z == 3) || (x == 1 && y == 3 && z == 2) || (x == 2 && y == 1 && z == 3) || (x == 2 && y == 3 && z == 1) || (x == 3 && y == 1 && z == 2) || (x == 3 && y == 2 && z == 1)) 
    {
        re = -1 * bet;// 1, 2, 3 の場合はベット額を失う
    }
    else if (x == y || y == z || z == x)
    {
        re = bet; // 二つの目が同じ場合
    }
    else 
    {
        re=0; // その他の場合
    }
    return re;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 200.0); // 透視投影
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);//視点移動出来るように

    glEnable(GL_DEPTH_TEST);
    for (int i = 0; i < 3; i++) 
    {
        glPushMatrix();
        glTranslatef(i * 2.0 - 2.0, 0.0, 0.0); // 3つのサイコロを横に並べる
        glRotatef(dice[i].RotAngleX, 1.0, 0.0, 0.0);
        glRotatef(dice[i].RotAngleY, 0.0, 1.0, 0.0);
        glRotatef(dice[i].RotAngleZ, 0.0, 0.0, 1.0);
        drawDice(i);
        glPopMatrix();
    }
    glDisable(GL_DEPTH_TEST);

    // 2D描画用の設定
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 2D投影
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(255, 255, 0);
    // 所持金とベット額の描画
    drawMoney(-0.9, 0.9, money);
    drawBet(-0.9, -0.95, bet);

    // スタートボタンの描画
    drawButton(buttonX, buttonY, buttonWidth, buttonHeight, "START");

    // ベットボタンの描画
    drawButton(betButtonX, betButtonY, betButtonWidth, betButtonHeight, "BET $10");

    // サイコロの停止面を表示
    for (int i = 0; i < 3; i++) 
    {
        drawText(-0.9, 0.8 - i * 0.1, diceStatus[i]);
    }
    glutSwapBuffers();
}



void timer(int value)
{
    for (int i = 0; i < 3; i++) //サイコロ0,1,2
    {
        // サイコロの回転を更新
        if (dice[i].Animation) 
        {
            dice[i].RotAngleX += dice[i].Speed * dice[i].directionX;
            dice[i].RotAngleY += dice[i].Speed * dice[i].directionY;
            dice[i].RotAngleZ += dice[i].Speed * dice[i].directionZ;

            // 徐々に速度を減少させる
            dice[i].Speed *= 0.985;

            // 速度が一定の閾値以下になったら回転を停止する
            if (dice[i].Speed < 6.0)
            {
                dice[i].Animation = 0;
                // 停止した面を設定
                dice[i].targetFace = rand() % 6 + 1;
                snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Face %d", i + 1, dice[i].targetFace); 
                if (dice[0].Animation == 0 && dice[1].Animation == 0 && dice[2].Animation == 0)
                {
                    money += tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet);
                    printf("ダイス１;%d   ダイス２;%d   ダイス３;%d   スコア;%d\n", dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet));
                    bet = 0; // ベット額をリセット
                }
            }
        }
        else 
        {
            // 停止する面に対応する角度を設定
            double targetAngleX, targetAngleY, targetAngleZ;
            setTargetAngles(dice[i].targetFace, &targetAngleX, &targetAngleY, &targetAngleZ);
            // イージング関数を使って角度をなめらかに移行
            double easing = 0.007; 
            dice[i].RotAngleX += (targetAngleX - dice[i].RotAngleX) * easing;
            dice[i].RotAngleY += (targetAngleY - dice[i].RotAngleY) * easing;
            dice[i].RotAngleZ += (targetAngleZ - dice[i].RotAngleZ) * easing;
            double gap = 20; // 閾値
            if (fabs(dice[i].RotAngleX - targetAngleX) < gap && fabs(dice[i].RotAngleY - targetAngleY) < gap && fabs(dice[i].RotAngleZ - targetAngleZ) < gap)//fabsで絶対値
            {
                dice[i].Speed = 0;
                dice[i].RotAngleX = targetAngleX;
                dice[i].RotAngleY = targetAngleY;
                dice[i].RotAngleZ = targetAngleZ;
            }
            else //ターゲットになった面の角度・速度によって軌道修正
            {
                if (dice[i].RotAngleX < targetAngleX)
                    dice[i].RotAngleX += fabs(dice[i].Speed);
                else if (dice[i].RotAngleX > targetAngleX)
                    dice[i].RotAngleX -= fabs(dice[i].Speed);
                if (dice[i].RotAngleY < targetAngleY)
                    dice[i].RotAngleY += fabs(dice[i].Speed);
                else if (dice[i].RotAngleY > targetAngleY)
                    dice[i].RotAngleY -= fabs(dice[i].Speed);
                if (dice[i].RotAngleZ < targetAngleZ)
                    dice[i].RotAngleZ += fabs(dice[i].Speed);
                else if (dice[i].RotAngleZ > targetAngleZ)
                    dice[i].RotAngleZ -= fabs(dice[i].Speed);
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) 
    {
    case 'w':
        eyeY += P;
        centerY += P;
        break;
    case 's':
        eyeY -= P;
        centerY -= P;
        break;
    case 'a':
        eyeX -= P;
        centerX -= P;
        break;
    case 'd':
        eyeX += P;
        centerX += P;
        break;
    case 'q':
        eyeZ -= P;
        centerZ -= P;
        break;
    case 'e':
        eyeZ += P;
        centerZ += P;
        break;
    case 'r'://サイコロの目を初期値にする
        init();
        break;
    case 'o': // 'o'キーが押された場合、4, 5, 6 しか現れないサイコロに設定
        for (int i = 0; i < 3; i++) 
        {
            dice[i].targetFace = (rand() % 3) + 4; // 4, 5, 6 のいずれかを設定
            snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Face %d", i + 1, dice[i].targetFace);
            // 停止する面に対応する角度を設定
            double targetAngleX, targetAngleY, targetAngleZ;
            setTargetAngles(dice[i].targetFace, &targetAngleX, &targetAngleY, &targetAngleZ);

            // 目標角度にサイコロを回転
            dice[i].RotAngleX = targetAngleX;
            dice[i].RotAngleY = targetAngleY;
            dice[i].RotAngleZ = targetAngleZ;
            if (i == 2)
            {
                printf("ダイス１;%d   ダイス２;%d   ダイス３;%d   スコア;%d\n", dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet));
            }
        }

        // チンチロリンのルールに従い所持金を増減
        money += tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet);;
        bet = 0; // ベット額をリセット
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    {
        // マウスの座標取得
        float mouseX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
        float mouseY = 1 - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2;

        // スタートボタンのクリック判定
        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) 
        {
            // 回転を開始
            for (int i = 0; i < 3; i++) 
            {
                dice[i].Animation = 1;
                dice[i].Speed = (rand() % 10 + 2) * 3; // ランダムな速度設定（6.0 - 36.0）
                dice[i].directionX = (rand() % 2) * 2 - 1; // -1 または 1
                dice[i].directionY = (rand() % 2) * 2 - 1; // -1 または 1
                dice[i].directionZ = (rand() % 2) * 2 - 1; // -1 または 1
                snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Rolling...", i + 1);
            }
        }

        // ベットボタンのクリック判定
        if (mouseX >= betButtonX && mouseX <= betButtonX + betButtonWidth && mouseY >= betButtonY && mouseY <= betButtonY + betButtonHeight) 
        {
            if (money >= 10)
            {
                bet += 10; // ベット額を設定
                money -= 10; // ベット額を所持金から引く
                drawBet(-0.9, -0.95, bet); // ベット額を更新
            }
            else
                printf("賭けられるお金がありません。\nこれ以上賭けるならwindowを閉じて始めからやり直してください");
        }
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("チンチロリン");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
