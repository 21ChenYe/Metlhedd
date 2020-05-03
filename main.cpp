/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QPainter>
#include <QTime>
#include <QApplication>
#include <QString>
#include <QtCore/qmath.h>
#include <stdlib.h>
#include "metlhedd.h"

Metlhedd::Metlhedd(QWidget *parent) : QWidget(parent) {

    setStyleSheet("background-color:grey;");
	
    inGame = false;
	jump = false;
	fall = false;
	slide = false;
	metl = false;
	shoot = false;
	trap = false;
	drone = false; 
	
	
    resize(WIDTH, HEIGHT);
    loadImages();
    
	tileOffset = 0;
	entityFrame = 0;
	height = 0;
	airtime = 0;
	slidespeed = 24;
	bulletTime = 0;
	enemyPos = 220;
	playerHP = 3;
	enemySpawn = 100;
	enemyMove = 252;
	instructionTime = 200;
}
void Metlhedd::loadImages(){
	
	title.load("title.png");
	player[0].load("player.png");
	player[1].load("player2.png");
	player[2].load("player3.png");
	player[SLIDE].load("playerslide.png");
	player[JUMP].load("playerjump.png");
	
	enemy[0].load("metl1.png");
	enemy[1].load("metl2.png");
	enemy[2].load("metl3.png");
	enemy[DRONE].load("drone.png");
	enemy[TRAP].load("trap.png");
	bullet.load("bullet.png");
	tile.load("tile.png");
	instruction.load("instructions.png");
	background.load("background.png");
}


void Metlhedd::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);  
    doDrawing();
}    
void Metlhedd::doDrawing(){
	QPainter qp(this);
	if(!inGame){
		qp.drawImage(0, 0, title);
	}else if(inGame && instructionTime > 0){
		qp.drawImage(0, 0, instruction);
		instructionTime -= 5;
	}else if(inGame && instructionTime == 0){
		
		qp.drawImage(0+tileOffset,160, tile);
		qp.drawImage(32+tileOffset,160, tile);
		qp.drawImage(64+tileOffset,160, tile);
		qp.drawImage(96+tileOffset,160, tile);
		qp.drawImage(128+tileOffset,160, tile);
		qp.drawImage(160+tileOffset,160, tile);
		qp.drawImage(192+tileOffset,160, tile);
		qp.drawImage(224+tileOffset,160, tile);
		qp.drawImage(256+tileOffset,160, tile);
		qp.drawImage(0,75,background);
		if(playerHP > 0){
			if(jump){
				qp.drawImage(36, 112 - height, player[JUMP]);
			}else if(slide){
				qp.drawImage(36, 124, player[SLIDE]);
			}else{
				qp.drawImage(36, 112, player[entityFrame]);
			}
			health(qp);
			
		}
		if(metl){
			qp.drawImage(enemyMove-35, 116, enemy[entityFrame]);
		}else if(drone){
			qp.drawImage(enemyMove-35, 92, enemy[DRONE]);
		}else if(trap){
			qp.drawImage(enemyMove-35, 128, enemy[TRAP]);
		}
		
		if(shoot){
			qp.drawImage(72+bulletTime, 116, bullet);
		}
		
	}
	
	if(playerHP <= 0){
		inGame = false;
		gameOver(qp);
	}
}
void Metlhedd::keyPressEvent(QKeyEvent *e) {
    
    int key = e->key();
    
    if (key == Qt::Key_Space && inGame != true) {
		inGame = true;
		timerId = startTimer(DELAY);
    }

	if (key == Qt::Key_Up && !slide) {
		jump = true;
	}
	if (key == Qt::Key_Down && !jump){
		slide = true;
	}
	if (key == Qt::Key_Right && !jump && !slide){
		shoot = true;
	}

    QWidget::keyPressEvent(e);    
}

void Metlhedd::animate(){
	
	if(drone || trap || metl){
		enemyMove -= 14;
	}
	tileOffset -= 4;
	if(tileOffset == -32){
		tileOffset = 0;
	}
	entityFrame += 1;
	if(entityFrame == 3){
	entityFrame = 0;
	}
	
	
	if(jump){
		height = -pow((airtime-4),2)+24;	
		airtime++;
		if(airtime > 9){
			jump = false;
			airtime = 0;
			height = 0;
		}
	}
	
	if(slide){
		slidespeed -= 2;
		if(slidespeed == 0){
			slidespeed = 24;
			slide = false;
		}
	}
	
	if(shoot){
		bulletTime += 30;
		if(bulletTime >= 180){
			bulletTime = 0;
			shoot = false;
		}
	}
}

void Metlhedd::checkCollision(){
	if(shoot && metl && ((72+bulletTime+6) >= enemyMove)){
		metl = false;
		enemyMove = 224;
	}else if(metl && enemyMove <= 56){
		metl = false;
		enemyMove = 224;
		playerHP--;
	}
	if(drone && (enemyMove <= 56 && enemyMove >= 28)){
		if(!slide){
			drone = false;
			enemyMove = 224;
			playerHP--;
		}
	}else if(drone && (enemyMove <= 0)){
		drone = false;
		enemyMove = 224;
	}
	
	if(trap && (enemyMove <= 56 && enemyMove >= 28)){
		if(!jump){
			trap = false;
			enemyMove = 224;
			playerHP--;
		}
	}else if(trap && (enemyMove <= 0)){
		trap = false;
		enemyMove = 224;
	}
}

void Metlhedd::randEnemy(){
	int enemyChoice;
	if(!metl && !drone && !trap && instructionTime == 0){
		enemySpawn -= 20;
	}
	if(enemySpawn == 0){
		enemyChoice = rand()%3+1;
		metl = true * (enemyChoice == 3);
		drone = true * (enemyChoice == 2);
		trap = true * (enemyChoice == 1);
		enemySpawn = 200;
	}
}
void Metlhedd::health(QPainter &qp) {
    
    QString message = QString("HP: %1").arg(playerHP);
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);

    qp.setFont(font);
    int h = 20;
    int w = 35;

    qp.translate(QPoint(w, h));
	
    qp.drawText(-textWidth/2, 0, message); 
}

void Metlhedd::gameOver(QPainter &qp) {
    
    QString message = "Game Over";
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);

    qp.setFont(font);
    int h = 172;
    int w = 230;

    qp.translate(QPoint(w/2, h/2));
	
    qp.drawText(-textWidth/2, 0, message); 
	killTimer(timerId); 
	
}

void Metlhedd::timerEvent(QTimerEvent *e){
	Q_UNUSED(e)
	randEnemy();
	animate();
	checkCollision();
	repaint();
	
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Metlhedd window;
	window.setWindowTitle("Metlhedd");
	window.show();
	
    return app.exec();
}

