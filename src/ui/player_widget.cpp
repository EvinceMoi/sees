#include "player_widget.h"

#include <iostream>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QTimer>

static void *get_proc_address(void *ctx, const char *name) {
	(void)ctx;

	auto curr_ctx = QOpenGLContext::currentContext();
	if (!curr_ctx) {
		return nullptr;
	}

	return reinterpret_cast<void *>(curr_ctx->getProcAddress(name));
}

static void update_callback(void *ctx) {
	auto pw = static_cast<PlayerWidget *>(ctx);
	pw->update();
}

PlayerWidget::PlayerWidget(QWidget *parent) : QOpenGLWidget(parent) {
	QTimer::singleShot(0, [this]() {
		mpv::media_t m;
		m.video = "/home/evin/Downloads/old/FLVA-047/FLVA-047.mp4";
		m.audio = "/home/evin/tmp/nmdr.mp3";
		m.subtitle = "/home/evin/tmp/test.ass";
		mpv_.load(m);
	});
}

PlayerWidget::~PlayerWidget() {
	mpv_.destroy();
}

void PlayerWidget::initializeGL() {
	auto f = context()->functions();
	f->initializeOpenGLFunctions();
	f->glClearColor(0.f, 0.f, 0.f, 1.f);

	mpv_.init_gl(
		mpv::c_callback_t {
			get_proc_address,
			nullptr,
		},
		mpv::c_callback_t {update_callback, this});
}

void PlayerWidget::paintGL() {
	mpv::opengl_fbo fbo{
		.fbo = static_cast<int>(defaultFramebufferObject()),
		.w = static_cast<int>(devicePixelRatioF() * width()),
		.h = static_cast<int>(devicePixelRatioF() * height()),
		.internal_format = 0
	};

	mpv_.do_render(fbo);
}
