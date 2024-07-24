#pragma once

#include <QOpenGLWidget>

#include "player/mpv.h"

class PlayerWidget : public QOpenGLWidget {

public:
	PlayerWidget(QWidget *parent = nullptr);
	~PlayerWidget();

protected:
	void initializeGL() override;
	void paintGL() override;

private:
	mpv::ctx mpv_;
};
