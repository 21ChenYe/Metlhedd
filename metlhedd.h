#pragma once

#include <QWidget>
#include <QKeyEvent>

class Metlhedd : public QWidget {
    
  public:
      Metlhedd(QWidget *parent = 0);

  protected:

	  void paintEvent(QPaintEvent *);
      void keyPressEvent(QKeyEvent *);
	  void timerEvent(QTimerEvent *);
	  
  private:
	  QImage title;
	  QImage player[5];
	  QImage enemy[5];
	  QImage tile;
	  QImage bullet;
	  QImage background;
	  QImage instruction;

	  static const int WIDTH = 256;
	  static const int HEIGHT = 192;
	  static const int PHEIGHT = 48;
	  static const int PWIDTH = 36;

	  static const int DELAY = 140;  
	  static const int TRAP = 4;
	  static const int DRONE = 3;
	  static const int SLIDE = 3;
	  static const int JUMP = 4;
	  
	  
	  bool inGame;
	  bool jump;
	  bool fall;
	  bool slide;
	  bool shoot;
	  bool metl;
	  bool trap;
	  bool drone;
	  bool game;
	  
	  int bulletTime;
	  int airtime;
	  int slidespeed;
	  int height;
	  int tileOffset;
	  int entityFrame;
	  int timerId;
	  int enemyPos;
	  int playerHP;
	  int enemySpawn;
	  int enemyMove;
	  int instructionTime;
	  
	  void health(QPainter &qp);
	  void gameOver(QPainter &qp);
	  void randEnemy();
	  void checkCollision();
	  void loadImages();
	  void doDrawing();
	  void animate();
};