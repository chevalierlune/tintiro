#include "glut.h"
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS
#define PI 3.14159

//Dice�̍\����
typedef struct 
{
    double RotAngleX;
    double RotAngleY;
    double RotAngleZ;
    double Speed; // ��]���x
    int Animation; // 0:�I�t�A1:�I��
    int targetFace; // �ڕW�̖ʁi1�`6�j
    int directionX; // X����]����: 1 or -1
    int directionY; // Y����]����: 1 or -1
    int directionZ; // Z����]����: 1 or -1
} Dice;

Dice dice[3]; // 3�̃T�C�R��

// ���_�̍��W���O���[�o���ϐ��Ƃ��Đ錾
double eyeX = 0.0, eyeY = 0.0, eyeZ = 12.0;
double centerX = 0.0, centerY = 0.0, centerZ = 0.0;
double P = 0.05;

// �X�^�[�g�{�^���ƃx�b�g�{�^���̈ʒu�ƃT�C�Y
float buttonX = -0.2, buttonY = -0.7, buttonWidth = 0.4, buttonHeight = 0.2;
float betButtonX = -0.2, betButtonY = -0.4, betButtonWidth = 0.4, betButtonHeight = 0.2;

// �T�C�R���̒�~�ʂ�ۑ�����ϐ�
char diceStatus[3][50];

// �������ƃx�b�g�z�̏����l
int money = 100;
int bet = 0;

// �e�ʂ̖ڕW�p�x��ݒ肷��֐�
void setTargetAngles(int face, double* angleX, double* angleY, double* angleZ) 
{
    switch (face) 
    {
    case 1: // 1�̖ʂ���ɗ���
        *angleX = 0.0;
        *angleY = 90.0;
        *angleZ = 0.0;
        break;
    case 2: // 2�̖ʂ���ɗ���
        *angleX = 180.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 3: // 3�̖ʂ���ɗ���
        *angleX = 90.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 4: // 4�̖ʂ���ɗ���
        *angleX = -90.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 5: // 5�̖ʂ���ɗ���
        *angleX = 0.0;
        *angleY = 0.0;
        *angleZ = 0.0;
        break;
    case 6: // 6�̖ʂ���ɗ���
        *angleX = 0.0;
        *angleY = -90.0;
        *angleZ = 0.0;
        break;
    }
}

// �������֐�
void init(void)
{
    srand((unsigned int)time(NULL));  // �����_���V�[�h��������
    for (int i = 0; i < 3; i++) 
    {
        dice[i].RotAngleX = 0.0;
        dice[i].RotAngleY = 0.0;
        dice[i].RotAngleZ = 0.0;
        dice[i].Speed = (rand() % 10 + 5) * 2; // �����_���ȑ��x�ݒ�i10.0 - 30.0�j
        dice[i].Animation = 0; // �A�j���[�V�����I�t
        dice[i].targetFace = 1; // �����ڕW��
        dice[i].directionX = (rand() % 2) * 2 - 1; // -1 �܂��� 1
        dice[i].directionY = (rand() % 2) * 2 - 1; // -1 �܂��� 1
        dice[i].directionZ = (rand() % 2) * 2 - 1; // -1 �܂��� 1
        snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Face ?", i + 1);
    }
}

// �ڂ�`�悷�邽�߂̊֐�
void drawDot(float x, float y, float z)
{
    glPushMatrix();
    glTranslated(x, y, z);
    glutSolidSphere(0.1, 20, 20); // �����ȋ��̂�`��
    glPopMatrix();
}

// �ԓ_��`�悷�邽�߂̊֐�
void drawDotColor(float x, float y, float z, int r, int g, int b)
{
    glPushMatrix();
    glColor3ub(r, g, b); // RGB�l���g���ĐF��ݒ�
    glTranslated(x, y, z);
    glutSolidSphere(0.1, 20, 20); // �����ȋ��̂�`��
    glPopMatrix();
}

// �T�C�R���̊e�ʂ�`�悷�邽�߂̊֐�
void drawDiceFaces()
{
    // ��1 (������)
    glPushMatrix();
    glTranslated(-0.5, 0.0, 0.0);
    glRotated(90, 0, 1, 0);
    glColor3f(0.0, 0.0, 0.0);
    drawDotColor(0.0, 0.0, 0.0, 255, 0, 0);
    glPopMatrix();

    // ��2 (�w��)
    glPushMatrix();
    glTranslated(0.0, 0.0, -0.5);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // ��3 (���)
    glPushMatrix();
    glTranslated(0.0, 0.5, 0.0);
    glRotated(90, 1, 0, 0);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.0, 0.0, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // ��4 (����)
    glPushMatrix();
    glTranslated(0.0, -0.5, 0.0);
    glRotated(90, 1, 0, 0);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.2, -0.2, 0.0);
    drawDot(-0.2, 0.2, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // ��5 (�O��)
    glPushMatrix();
    glTranslated(0.0, 0.0, 0.5);
    glColor3f(0.0, 0.0, 0.0);
    drawDot(-0.2, -0.2, 0.0);
    drawDot(0.2, -0.2, 0.0);
    drawDot(0.0, 0.0, 0.0);
    drawDot(-0.2, 0.2, 0.0);
    drawDot(0.2, 0.2, 0.0);
    glPopMatrix();

    // ��6 (�E����)
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

// �T�C�R���̕`��
void drawDice(int index)
{
    glPushMatrix();
    glTranslated(0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidCube(1.0); // �T�C�R���̖{�̂�`��
    drawDiceFaces(); // �T�C�R���̊e�ʂ�`��
    glPopMatrix();
}

// �Q����������`�悷��֐�
void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    while (*text) 
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// ��������`�悷��֐�
void drawMoney(float x, float y, int amount)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Money: $%d", amount);
    drawText(x, y, buffer);
}

// �x�b�g�z��`�悷��֐�
void drawBet(float x, float y, int amount)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Bet : $%d", amount);
    drawText(x, y, buffer);
}

// �{�^����`�悷��֐�
void drawButton(float x, float y, float width, float height, const char* label)
{
    glColor3f(0, 255, 255);// �{�^���̐F�𐅐F�ɐݒ�
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    // �{�^���̃��x����`��
    glColor3f(255, 0, 0); // �e�L�X�g�̐F��Ԃɐݒ�
    drawText(x + width / 2 - 0.05 * strlen(label) / 3, y + height / 2 - 0.05 / 2, label);//�����̏ꏊ
}

//�`���`���̓��_�z��
int tintiro(int x, int y, int z, int bet) 
{
    int re;//�Ԃ��l
    if (x == y && x == z && y == z) // ���ׂĂ̖ڂ������ꍇ
    { 
        if (x == 1) 
        {
            re = 5 * bet; // 1,1,1��5�{
        }
        else 
        {
            re = 3 * bet; // ���̑��̃]���ڂ�3�{
        }
    }
    else if ((x == 4 && y == 5 && z == 6) || (x == 4 && y == 6 && z == 5) || (x == 5 && y == 4 && z == 6) || (x == 5 && y == 6 && z == 4) || (x == 6 && y == 4 && z == 5) || (x == 6 && y == 5 && z == 4)) 
    {
        re = 2 * bet;// 4, 5, 6 �̏ꍇ��2�{
    }
    else if ((x == 1 && y == 2 && z == 3) || (x == 1 && y == 3 && z == 2) || (x == 2 && y == 1 && z == 3) || (x == 2 && y == 3 && z == 1) || (x == 3 && y == 1 && z == 2) || (x == 3 && y == 2 && z == 1)) 
    {
        re = -1 * bet;// 1, 2, 3 �̏ꍇ�̓x�b�g�z������
    }
    else if (x == y || y == z || z == x)
    {
        re = bet; // ��̖ڂ������ꍇ
    }
    else 
    {
        re=0; // ���̑��̏ꍇ
    }
    return re;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 200.0); // �������e
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);//���_�ړ��o����悤��

    glEnable(GL_DEPTH_TEST);
    for (int i = 0; i < 3; i++) 
    {
        glPushMatrix();
        glTranslatef(i * 2.0 - 2.0, 0.0, 0.0); // 3�̃T�C�R�������ɕ��ׂ�
        glRotatef(dice[i].RotAngleX, 1.0, 0.0, 0.0);
        glRotatef(dice[i].RotAngleY, 0.0, 1.0, 0.0);
        glRotatef(dice[i].RotAngleZ, 0.0, 0.0, 1.0);
        drawDice(i);
        glPopMatrix();
    }
    glDisable(GL_DEPTH_TEST);

    // 2D�`��p�̐ݒ�
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 2D���e
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(255, 255, 0);
    // �������ƃx�b�g�z�̕`��
    drawMoney(-0.9, 0.9, money);
    drawBet(-0.9, -0.95, bet);

    // �X�^�[�g�{�^���̕`��
    drawButton(buttonX, buttonY, buttonWidth, buttonHeight, "START");

    // �x�b�g�{�^���̕`��
    drawButton(betButtonX, betButtonY, betButtonWidth, betButtonHeight, "BET $10");

    // �T�C�R���̒�~�ʂ�\��
    for (int i = 0; i < 3; i++) 
    {
        drawText(-0.9, 0.8 - i * 0.1, diceStatus[i]);
    }
    glutSwapBuffers();
}



void timer(int value)
{
    for (int i = 0; i < 3; i++) //�T�C�R��0,1,2
    {
        // �T�C�R���̉�]���X�V
        if (dice[i].Animation) 
        {
            dice[i].RotAngleX += dice[i].Speed * dice[i].directionX;
            dice[i].RotAngleY += dice[i].Speed * dice[i].directionY;
            dice[i].RotAngleZ += dice[i].Speed * dice[i].directionZ;

            // ���X�ɑ��x������������
            dice[i].Speed *= 0.985;

            // ���x������臒l�ȉ��ɂȂ������]���~����
            if (dice[i].Speed < 6.0)
            {
                dice[i].Animation = 0;
                // ��~�����ʂ�ݒ�
                dice[i].targetFace = rand() % 6 + 1;
                snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Face %d", i + 1, dice[i].targetFace); 
                if (dice[0].Animation == 0 && dice[1].Animation == 0 && dice[2].Animation == 0)
                {
                    money += tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet);
                    printf("�_�C�X�P;%d   �_�C�X�Q;%d   �_�C�X�R;%d   �X�R�A;%d\n", dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet));
                    bet = 0; // �x�b�g�z�����Z�b�g
                }
            }
        }
        else 
        {
            // ��~����ʂɑΉ�����p�x��ݒ�
            double targetAngleX, targetAngleY, targetAngleZ;
            setTargetAngles(dice[i].targetFace, &targetAngleX, &targetAngleY, &targetAngleZ);
            // �C�[�W���O�֐����g���Ċp�x���Ȃ߂炩�Ɉڍs
            double easing = 0.007; 
            dice[i].RotAngleX += (targetAngleX - dice[i].RotAngleX) * easing;
            dice[i].RotAngleY += (targetAngleY - dice[i].RotAngleY) * easing;
            dice[i].RotAngleZ += (targetAngleZ - dice[i].RotAngleZ) * easing;
            double gap = 20; // 臒l
            if (fabs(dice[i].RotAngleX - targetAngleX) < gap && fabs(dice[i].RotAngleY - targetAngleY) < gap && fabs(dice[i].RotAngleZ - targetAngleZ) < gap)//fabs�Ő�Βl
            {
                dice[i].Speed = 0;
                dice[i].RotAngleX = targetAngleX;
                dice[i].RotAngleY = targetAngleY;
                dice[i].RotAngleZ = targetAngleZ;
            }
            else //�^�[�Q�b�g�ɂȂ����ʂ̊p�x�E���x�ɂ���ċO���C��
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
    case 'r'://�T�C�R���̖ڂ������l�ɂ���
        init();
        break;
    case 'o': // 'o'�L�[�������ꂽ�ꍇ�A4, 5, 6 ��������Ȃ��T�C�R���ɐݒ�
        for (int i = 0; i < 3; i++) 
        {
            dice[i].targetFace = (rand() % 3) + 4; // 4, 5, 6 �̂����ꂩ��ݒ�
            snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Face %d", i + 1, dice[i].targetFace);
            // ��~����ʂɑΉ�����p�x��ݒ�
            double targetAngleX, targetAngleY, targetAngleZ;
            setTargetAngles(dice[i].targetFace, &targetAngleX, &targetAngleY, &targetAngleZ);

            // �ڕW�p�x�ɃT�C�R������]
            dice[i].RotAngleX = targetAngleX;
            dice[i].RotAngleY = targetAngleY;
            dice[i].RotAngleZ = targetAngleZ;
            if (i == 2)
            {
                printf("�_�C�X�P;%d   �_�C�X�Q;%d   �_�C�X�R;%d   �X�R�A;%d\n", dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet));
            }
        }

        // �`���`�������̃��[���ɏ]���������𑝌�
        money += tintiro(dice[0].targetFace, dice[1].targetFace, dice[2].targetFace, bet);;
        bet = 0; // �x�b�g�z�����Z�b�g
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    {
        // �}�E�X�̍��W�擾
        float mouseX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
        float mouseY = 1 - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2;

        // �X�^�[�g�{�^���̃N���b�N����
        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) 
        {
            // ��]���J�n
            for (int i = 0; i < 3; i++) 
            {
                dice[i].Animation = 1;
                dice[i].Speed = (rand() % 10 + 2) * 3; // �����_���ȑ��x�ݒ�i6.0 - 36.0�j
                dice[i].directionX = (rand() % 2) * 2 - 1; // -1 �܂��� 1
                dice[i].directionY = (rand() % 2) * 2 - 1; // -1 �܂��� 1
                dice[i].directionZ = (rand() % 2) * 2 - 1; // -1 �܂��� 1
                snprintf(diceStatus[i], sizeof(diceStatus[i]), "Dice %d: Rolling...", i + 1);
            }
        }

        // �x�b�g�{�^���̃N���b�N����
        if (mouseX >= betButtonX && mouseX <= betButtonX + betButtonWidth && mouseY >= betButtonY && mouseY <= betButtonY + betButtonHeight) 
        {
            if (money >= 10)
            {
                bet += 10; // �x�b�g�z��ݒ�
                money -= 10; // �x�b�g�z���������������
                drawBet(-0.9, -0.95, bet); // �x�b�g�z���X�V
            }
            else
                printf("�q�����邨��������܂���B\n����ȏ�q����Ȃ�window����Ďn�߂����蒼���Ă�������");
        }
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("�d22�|0006�@�V��i��");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
