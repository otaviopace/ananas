#ifndef ANANAS_VTTY_H
#define ANANAS_VTTY_H

#include "kernel/dev/tty.h"
#include "ivideo.h"
#include "kernel/lock.h"

class IVideo;

class VTTY : public TTY
{
public:
	VTTY(const Ananas::CreateDeviceProperties& cdp);

	Result Attach() override;
	Result Detach() override;

	Result Print(const char* buffer, size_t len) override;

	IVideo& GetVideo() const {
		return v_video;
	}

	IVideo::Pixel& PixelAt(int x, int y)
	{
		return v_buffer[y * v_video.GetWidth() + x];
	}

	bool IsActive() const
	{
		return v_active;
	}

	void Activate();
	void Deactivate();

private:
	Mutex v_mutex{"vtty_mutex"};
	IVideo& v_video;
	IVideo::Pixel* v_buffer;
	bool v_active = false;
	teken_t v_teken;
};


#endif /* ANANAS_VTTY_H */
