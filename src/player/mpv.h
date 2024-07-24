#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>

#include <mpv/client.h>
#include <mpv/render.h>
#include <mpv/render_gl.h>

namespace mpv {

using opengl_fbo = mpv_opengl_fbo;
using f_get_proc_address = decltype(mpv_opengl_init_params::get_proc_address);
using f_update_callback = mpv_render_update_fn;

struct media_t {
	std::string video;
	std::optional<std::string> audio;
	std::optional<std::string> subtitle;
};

template<typename F>
struct c_callback_t {
	F fn;
	void *ctx;
};

class ctx {

public:
	ctx();
	~ctx();

public:
	void destroy();

	void init_gl(c_callback_t<f_get_proc_address> gpa, c_callback_t<f_update_callback> uc);
	void do_render(opengl_fbo &fbo);

public:
	void load(const media_t& m);

private:
	void wakeup();
	void wait_event();
	void handle_event(mpv_event *event);

private:
	using ctx_ptr_t =
		std::unique_ptr<mpv_handle, decltype(&mpv_terminate_destroy)>;
	ctx_ptr_t ctx_;

	using renderer_ptr_t =
		std::unique_ptr<mpv_render_context, decltype(&mpv_render_context_free)>;
	renderer_ptr_t renderer_;

	std::atomic_bool stop_;
	std::thread event_thrd_;

	std::mutex event_mtx_;
	std::condition_variable event_cv_;
};

} // namespace mpv
