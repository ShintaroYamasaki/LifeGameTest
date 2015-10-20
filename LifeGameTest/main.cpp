//
//  main.cpp
//  LifeGameTest
//
//  Created by 山崎 慎太郎 on 2014/12/11.
//  Copyright (c) 2014年 山崎 慎太郎. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>

/// 画面高さ
#define WINDOW_HEIGHT 800
/// 画面幅
#define WINDOW_WIDTH 800
/// 目盛り数
#define TICK_NUM 31

char title[] = "Life Game";

/// セル色情報(白黒)
bool cells[TICK_NUM * TICK_NUM + 1];

/// セル幅
static double interval = (double) WINDOW_WIDTH / (double) TICK_NUM;

/// 初期処理
void init() {
    // セル色情報の初期化
    for (int i = 0; i < TICK_NUM * TICK_NUM; i++) {
        cells[i] = false;
    }
}

/// マス目描画
void drawGlid() {
    // マス目を書く
    for (int i = 0; i < TICK_NUM; i++) {
        // 縦線
        glBegin(GL_LINES);
            glColor3b(0.0, 0.0, 0.0);
            glVertex2d(interval * i, 0);
            glVertex2d(interval * i, WINDOW_HEIGHT);
        glEnd();
    
        // 横線
        glBegin(GL_LINES);
            glColor3b(0.0, 0.0, 0.0);
            glVertex2d(0, interval * i);
            glVertex2d(WINDOW_WIDTH, interval * i);
        glEnd();
    }
    
    glFlush();
}

/// 世代更新
void updateGeneration() {
    /// 色づくセル
    int onCells[TICK_NUM * TICK_NUM];
    /// 色が消えるセル
    int offCells[TICK_NUM * TICK_NUM];
    
    /// onCellsの数
    int onNum = 0;
    /// offCellsの数
    int offNum = 0;
    
    // 世代更新
    for (int i = 0; i < TICK_NUM * TICK_NUM; i++) {
        /// 色づいている周辺セルの数
        int count = 0;
        
        int x = i % TICK_NUM;
        int y = i / TICK_NUM;
        
        
        if (y != 0) {
            if (x != 0)
                // 左上セル
                if (cells[i - TICK_NUM - 1]) count += 1;
            
            // 上セル
            if (cells[i - TICK_NUM]) count += 1;
            
            if (x != interval * TICK_NUM - 1)
                // 右上セル
                if (cells[i - TICK_NUM + 1]) count += 1;
            
        }
        
        if (x != 0)
            // 左セル
            if (cells[i - 1]) count += 1;
        
        if (x != interval * TICK_NUM - 1)
            // 右セル
            if (cells[i + 1]) count += 1;
        
        if (y != interval * TICK_NUM - 1) {
            if (x != 0)
                // 左下セル
                if (cells[i + TICK_NUM - 1]) count += 1;
            
            // 下セル
            if (cells[i + TICK_NUM]) count += 1;
            
            if (x != interval * TICK_NUM - 1)
                // 右下セル
                if (cells[i + TICK_NUM + 1]) count += 1;
        }
        
        
        // セルが色づいていない and 周りに3匹いる → 生成
        if (!cells[i] && count == 3) {
            // セル描画
            glBegin(GL_QUADS);
                glColor4d(0.0, 0.0, 0.0, 1.0);
                glVertex2d(interval * x, interval * y);
                glVertex2d(interval * x, interval * (y + 1));
                glVertex2d(interval * (x + 1), interval * (y + 1));
                glVertex2d(interval * (x + 1), interval * y);
            glEnd();
            
            glFlush();
            
            onCells[onNum] = i;
            
            onNum += 1;
        }
        // セルが色づいている and 周りに2匹または3匹いない → 削除
        if (cells[i] && !(count == 2 || count == 3)) {
            // セル描画
            glBegin(GL_QUADS);
                glColor4d(1.0, 1.0, 1.0, 1.0);
                glVertex2d(interval * x, interval * y);
                glVertex2d(interval * x, interval * (y + 1));
                glVertex2d(interval * (x + 1), interval * (y + 1));
                glVertex2d(interval * (x + 1), interval * y);
            glEnd();
            
            // 枠線描画
            glBegin(GL_LINE_LOOP);
                glColor4d(0.0, 0.0, 0.0, 1.0);
                glVertex2d(interval * x, interval * y);
                glVertex2d(interval * x, interval * (y + 1));
                glVertex2d(interval * (x + 1), interval * (y + 1));
                glVertex2d(interval * (x + 1), interval * y);
            glEnd();
            
            glFlush();
            
            offCells[offNum] = i;
            
            offNum += 1;
            
        }
    }
    
    for (int i = 0; i < onNum; i++) {
        cells[onCells[i]] = true;
    }
    for (int i = 0; i < offNum; i++) {
        cells[offCells[i]] = false;
    }
}

/// リセット
void reset() {
    // 画面背景を白に
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawGlid();
    
    init();
}

void disp(void)
{
    reset();
    
}

void resize(int w, int h)
{
    /* ウィンドウ全体をビューポートにする */
    glViewport(0, 0, w, h);
    
    /* 変換行列の初期化 */
    glLoadIdentity();
    
    /* スクリーン上の座標系をマウスの座標系に一致させる */
    glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

/// マウスアクション
void mouse(int button, int state, int x, int y)
{
    switch (button) {
        // 左ボタン
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                int i = x / interval;
                int j = y / interval;
                
                // クリックされたセルをtrueに
                cells[j * TICK_NUM + i] = true;
                
                
                // セル描画
                glBegin(GL_QUADS);
                    glColor4d(0.0, 0.0, 0.0, 1.0);
                    glVertex2d(interval * i, interval * j);
                    glVertex2d(interval * i, interval * (j + 1));
                    glVertex2d(interval * (i + 1), interval * (j + 1));
                    glVertex2d(interval * (i + 1), interval * j);
                glEnd();
                
                glFlush();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                int i = x / interval;
                int j = y / interval;
                
                // クリックされたセルをfalseに
                cells[j * TICK_NUM + i] = false;
                
                
                // セル描画
                glBegin(GL_QUADS);
                    glColor4d(1.0, 1.0, 1.0, 1.0);
                    glVertex2d(interval * i, interval * j);
                    glVertex2d(interval * i, interval * (j + 1));
                    glVertex2d(interval * (i + 1), interval * (j + 1));
                    glVertex2d(interval * (i + 1), interval * j);
                glEnd();
                
                // 枠線描画
                glBegin(GL_LINE_LOOP);
                    glColor4d(0.0, 0.0, 0.0, 1.0);
                    glVertex2d(interval * i, interval * j);
                    glVertex2d(interval * i, interval * (j + 1));
                    glVertex2d(interval * (i + 1), interval * (j + 1));
                    glVertex2d(interval * (i + 1), interval * j);
                glEnd();
                
                glFlush();
            }

            break;
    }
}

/// キーアクション
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        //
        case ' ':
            updateGeneration();
            
            break;
        case 'r':
            reset();
            
            break;
        default:
            break;
    }
}

int main(int argc , char ** argv) {
    glutInit(&argc , argv);
    glutInitWindowPosition(100 , 50);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    
    init();
    
    glutCreateWindow(title);
    glutDisplayFunc(disp);
    
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}