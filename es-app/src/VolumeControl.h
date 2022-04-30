#pragma once
#ifndef ES_APP_VOLUME_CONTROL_H
#define ES_APP_VOLUME_CONTROL_H

#include <memory>

#include <unistd.h>
#include <fcntl.h>
#include <alsa/asoundlib.h>

/*!
Singleton pattern. Call getInstance() to get an object.
*/
class VolumeControl
{
    static const char * mixerName;
    static const char * mixerCard;
    int mixerIndex;
    snd_mixer_t* mixerHandle;
    snd_mixer_elem_t* mixerElem;
    snd_mixer_selem_id_t* mixerSelemId;

	int originalVolume;
	int internalVolume;

	static std::weak_ptr<VolumeControl> sInstance;

	VolumeControl();
	VolumeControl(const VolumeControl & right);
    VolumeControl & operator=(const VolumeControl & right);

public:
	static std::shared_ptr<VolumeControl> & getInstance();

	void init();
	void deinit();

	int getVolume() const;
	void setVolume(int volume);

	~VolumeControl();
};

#endif // ES_APP_VOLUME_CONTROL_H
