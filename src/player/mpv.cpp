#include "mpv.h"

#include <iostream>
#include <list>
#include <format>
#include <ranges>

namespace mpv {

ctx::ctx()
	: ctx_(mpv_create(), &mpv_terminate_destroy),
	  renderer_(nullptr, &mpv_render_context_free), stop_(false) {
	event_thrd_ = std::thread {[this]() {
		wait_event();
	}};

	// mpv_set_option_string(ctx_.get(), "config", "yes");
	// mpv_set_option_string(ctx_.get(), "config-dir", "/config/path");
	// mpv_set_option_string(ctx_.get(), "input-conf", "/config/path");
	// mpv_set_option_string(ctx_.get(), "load-scripts", "/config/path");
	// mpv_set_option_string(ctx_.get(), "script", "/config/path");
	// mpv_set_option_string(ctx_.get(), "player-operation-mode",
	// "/config/path"); mpv_set_option_string(ctx_.get(), "input-app-events",
	// "/config/path"); // macos
	mpv_initialize(ctx_.get());

	mpv_set_option_string(ctx_.get(), "keepaspect", "yes");
	mpv_set_option_string(ctx_.get(), "gpu-api", "vulkan");
	mpv_set_option_string(ctx_.get(), "vo", "gpu");

	// https://mpv.io/manual/stable/#video-output-drivers-libmpv
	mpv_set_option_string(ctx_.get(), "vo", "libmpv");

	// https://mpv.io/manual/stable/#properties
	// eof-reached
	// cache-buffering-state
	// display-fps
	// video-bitrate/audio-bitrate/sub-bitrate
	// pause
	// duration
	// playback-time
	// mpv_observe_property(ctx_.get(), 0, const char *name, mpv_format format)
	mpv_set_wakeup_callback(
		ctx_.get(),
		[](void *p) -> void {
			auto c = static_cast<ctx *>(p);
			c->wakeup();
		},
		this);
}

ctx::~ctx() {
	event_thrd_.join();
}

void ctx::destroy() {
	stop_ = true;
	event_cv_.notify_one();
}

void ctx::init_gl(c_callback_t<f_get_proc_address> gpa, c_callback_t<f_update_callback> uc) {
	mpv_opengl_init_params init {
		gpa.fn, gpa.ctx,
	};
	mpv_render_param params[] {
		{MPV_RENDER_PARAM_API_TYPE, const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
		{MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &init},
		{MPV_RENDER_PARAM_INVALID, nullptr},
	};

	mpv_render_context *rr;
	if (mpv_render_context_create(&rr, ctx_.get(), params) < 0) {
		throw std::runtime_error("failed to create mpv render contenxt");
	}

	renderer_.reset(rr);

	mpv_render_context_set_update_callback(
		renderer_.get(), uc.fn, uc.ctx);
}

void ctx::do_render(opengl_fbo &fbo) {
	int flip_y = 1;
	mpv_render_param params[] = {
		{ MPV_RENDER_PARAM_OPENGL_FBO, &fbo },
		{ MPV_RENDER_PARAM_FLIP_Y, &flip_y },
		{ MPV_RENDER_PARAM_INVALID, nullptr }
	};

	mpv_render_context_render(renderer_.get(), params);
}

void ctx::load(const media_t& m) {
	std::list<std::string> command{"loadfile", m.video, "replace", "0"};
	std::list<std::string> extra;
	if (m.audio) {
		extra.push_front(std::move(std::format("audio-file={}", m.audio.value())));
		// extra.push_back(m.audio.value());
	}
	if (m.subtitle) {
		extra.push_back(std::move(std::format("sub-file={}", m.subtitle.value())));
		// extra.push_back(m.subtitle.value());
	}

	if (!extra.empty()) {
		auto joined = extra | std::views::join_with(',') | std::ranges::to<std::string>();
		command.push_back(std::move(joined));
		// command.push_back(std::move(std::format("external-files={}", joined)));
	}

	auto cs = command | std::views::join_with(' ') | std::ranges::to<std::string>();
	std::cout << "command: " << cs << std::endl;
	auto ec = mpv_command_string(ctx_.get(), cs.c_str());
	std::cout << "ec: " << mpv_error_string(ec) << std::endl;
}


void ctx::wakeup() {
	// wakeup trhead to get event
	// std::lock_guard lock(event_mtx_);
	event_cv_.notify_one();
}

void ctx::wait_event() {
	while (!stop_) {
		std::unique_lock lk(event_mtx_);
		event_cv_.wait(lk);

		if (stop_) {
			break;
		}

		while (true) {
			auto ev = mpv_wait_event(ctx_.get(), 0);
			if (ev->event_id == MPV_EVENT_NONE) {
				break;
			}

			handle_event(ev);
		}

		lk.unlock();
	}
}

void ctx::handle_event(mpv_event *event) {
	switch (event->event_id) {
	case MPV_EVENT_PROPERTY_CHANGE: {
		auto prop = static_cast<mpv_event_property *>(event->data);
		std::cout << "got event: " << prop->name << std::endl;
		break;
	}
	default:
		break;
	}
}

} // namespace mpv
